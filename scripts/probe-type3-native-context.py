#!/usr/bin/env python3
"""Cold Type3 source/context controls; no source adoption or exact credit.

Run one case on the maintained source or the rejected four-byte word model.
Reports retain source/tool hashes, actual COFF and complete-owner comparisons.
Temporary sources and compiler objects clean themselves on failure. Coordinate
helper cases accept only the unqualified baseline source.
"""
from __future__ import annotations

import argparse
import gzip
import hashlib
import importlib.util
import json
from pathlib import Path
import re
import subprocess
import sys
import tempfile
import tomllib

ROOT = Path(__file__).resolve().parents[1]
SOURCE = 'ExAttackUpdateType3.cpp'
CASES = {
    'baseline': (),
    'coordinates': ('GameManagerCoordinates.cpp',),
    'collision': ('PlayerCollisionHelpers.cpp',),
    'float-core': ('Float3Core.inl',),
    'float-ops': ('Float3Core.inl', 'Float3.cpp'),
    'interrupt': ('AnmManagerRuntime.cpp',),
    'pointer-views': ('VectorPointerViews.cpp',),
    'timer-plain': (),
    'timer-assignment': ('ZunTimer.cpp',),
    'timer-reset': ('ZunTimer.cpp',),
    'timer-all': ('ZunTimer.cpp',),
    'sample-array': (),
    'sample-query': (),
    'sample-cursor': (),
    'history-root': (),
    'sample-references': (),
    'coordinate-indexed': (),
    'coordinate-middle': (),
}
VARIANTS = ('baseline', 'forwarded-collision')
INTERRUPT_SYMBOL = '?SetInterrupt@AnmVm@@QAEXF@Z'
POINTER_VIEW_UNITS = (
    ('float3-pointer-identity-view', '??BFloat3@@QAEPAMXZ'),
    ('player-position-pointer-identity-view', '??BPlayerPositionView@@QAEPAMXZ'),
)
TIMER_COMPARE_SYMBOL = '??PZunTimer@@QAEIH@Z'
TIMER_BODY_CASES = ('timer-assignment', 'timer-reset', 'timer-all')
TIMER_ASSIGNMENT = '''void ZunTimer::operator=(int value)
{
    this->SetCurrent(value);
}
'''
TIMER_SETTER = '''void ZunTimer::SetCurrent(int value)
{
    this->current = value;
    this->subFrame = (float)value;
    this->previous = -999999;
}
'''


def module(name, filename):
    spec = importlib.util.spec_from_file_location(name, ROOT / 'scripts' / filename)
    result = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(result)
    return result


def inputs_for(case):
    if case not in CASES:
        raise ValueError('unknown Type3 native context')
    # This existing producer also enforces the reviewed maintained-source SHA.
    word = module('context_word_inputs', 'probe-exattack-type3-word-order.py')
    inputs = word.source_inputs()
    def collect(name):
        if name in inputs:
            return
        path = ROOT / 'src' / name
        if path.is_symlink() or not path.resolve().is_relative_to((ROOT / 'src').resolve()):
            raise ValueError('native context source escapes or aliases src')
        data = path.read_bytes()
        inputs[name] = data
        for include in re.findall(rb'^\s*#\s*include\s*"([^"]+)"', data, re.M):
            collect((Path(name).parent / include.decode()).as_posix())
    for filename in CASES[case]:
        collect(filename)
    return inputs, word.PROFILE


def report_name(case, variant):
    suffix = '' if variant == 'baseline' else '-' + variant
    return 'type3-context-' + case + suffix + '.json.gz'


def sample_container(source, case):
    """Group actual query objects, preserving computations and short-circuiting.

    This is a source representation hypothesis, not a recovered stack layout.
    No callee, field type, qualification or inlining attribute is introduced.
    """
    if case not in ('sample-array', 'sample-query'):
        raise ValueError('unknown Type3 sample container')
    names = (('collisionSize', 'current', 'middle', 'tail')
             if case == 'sample-query' else ('current', 'middle', 'tail'))
    for name in names:
        declaration = '    PlayerPositionView ' + name + ';'
        if source.count(declaration) != 1:
            raise ValueError('actual Type3 query declaration changed: ' + name)
        source = source.replace(declaration, '', 1)
    if case == 'sample-array':
        storage = '    PlayerPositionView samples[3];\n'
        replacements = {name: 'samples[%d]' % n for n, name in enumerate(names)}
    else:
        storage = ('    struct CollisionSamples\n    {\n' +
                   ''.join('        PlayerPositionView ' + name + ';\n' for name in names) +
                   '    } samples;\n')
        replacements = {name: 'samples.' + name for name in names}
    for name, replacement in replacements.items():
        source = re.sub(r'\b' + name + r'\b', replacement, source)
    anchor = '    PlayerPositionView *collision ='
    if source.count(anchor) != 1:
        raise ValueError('actual Type3 collision-size pointer changed')
    return source.replace(anchor, storage + anchor, 1)


HISTORY_CASES = ('sample-cursor', 'history-root', 'sample-references')


def history_access(source, case):
    """Change actual history traversal, keeping the original sample indices."""
    if case not in HISTORY_CASES:
        raise ValueError('unreviewed Type3 history context')
    word = module('history_source_anchors', 'probe-exattack-type3-word-order.py')
    replace = word.replace_once
    if case == 'history-root':
        source = replace(source, '    switch (extra->state00)',
            '    Float3 *history = extra->history10;\n\n    switch (extra->state00)')
        return source.replace('extra->history10[', 'history[')
    if case == 'sample-cursor':
        source = replace(source, '    PlayerPositionView middle;',
            '    const Float3 *sample = &extra->history10[12];\n    PlayerPositionView middle;')
        source = replace(source, '    PlayerPositionView tail;',
            '    sample += 12;\n    PlayerPositionView tail;')
        names = ((12, 'sample->'), (24, 'sample->'))
    else:
        names = ((12, 'middleHistory.'), (24, 'tailHistory.'))
        for index, name in ((12, 'middle'), (24, 'tail')):
            source = replace(source, '    PlayerPositionView '+name+';',
                '    const Float3 &'+name+'History = extra->history10['+str(index)+'];\n'
                '    PlayerPositionView '+name+';')
    for index, prefix in names:
        for axis in ('x', 'y'):
            source = replace(source, 'extra->history10['+str(index)+'].'+axis,
                             prefix+axis)
    return source


def coordinate_samples(source, case):
    """Convert actual samples by index through an ordinary inline output helper."""
    if case not in ('coordinate-indexed', 'coordinate-middle'):
        raise ValueError('unreviewed Type3 coordinate context')
    starts = ['    PlayerPositionView '+name+';' for name in ('current', 'middle', 'tail')]
    end_marker = '    if (reinterpret_cast<PlayerLifecycleView *>'
    callback = 'int __fastcall ExAttackUpdateCallbackType3('
    if any(source.count(marker) != 1 for marker in starts+[end_marker, callback]):
        raise ValueError('Type3 sample/callback boundaries changed')
    begin, end = source.index(starts[0]), source.index(end_marker)
    positions = [source.index(marker) for marker in starts]+[end]
    if positions != sorted(positions):
        raise ValueError('Type3 sample order changed')
    blocks = []
    for i, (name, index, root) in enumerate((('current', 0, 'position'),
                                            ('middle', 12, 'extra->history10'),
                                            ('tail', 24, 'extra->history10'))):
        block = source[positions[i]:positions[i+1]]
        x = 'position->x' if name == 'current' else root+'['+str(index)+'].x'
        y = 'record->position20.y' if name == 'current' else root+'['+str(index)+'].y'
        if (block.count('InverseTransformPopupX('+x+')') != 1 or
                block.count('InverseTransformPopupY('+y+')') != 1):
            raise ValueError('Type3 actual coordinate source changed')
        if (block.count('InverseTransformPopupX(') != 1 or
                block.count('InverseTransformPopupY(') != 1 or
                block.count(name+'.z = 0.0f;') != 1):
            raise ValueError('Type3 original sample operations changed')
        if case == 'coordinate-middle' and name != 'middle':
            blocks.append(block)
        else:
            blocks.append('    PlayerPositionView '+name+';\n    ConvertType3Sample('
                          +name+', '+root+', '+str(index)+');\n\n')
    helper = ("inline void ConvertType3Sample(PlayerPositionView &out, const Float3 *history, int index)\n"
              "{\n    out.x = g_GameManager.InverseTransformPopupX(history[index].x);\n"
              "    out.y = g_GameManager.InverseTransformPopupY(history[index].y);\n"
              "    out.z = 0.0f;\n}\n\n")
    source = source[:begin]+''.join(blocks)+source[end:]
    return source.replace(callback, helper+callback, 1)


def render_source(inputs, case, variant):
    if case not in CASES or variant not in VARIANTS:
        raise ValueError('unknown Type3 source/context variant')
    source = inputs[SOURCE]
    if case in ('coordinate-indexed', 'coordinate-middle') and variant != 'baseline':
        raise ValueError('coordinate contexts require the unqualified baseline source')
    if variant != 'baseline':
        word = module('context_word_variant', 'probe-exattack-type3-word-order.py')
        source = dict(word.variants(source.decode()))[variant].encode()
    if case in ('coordinate-indexed', 'coordinate-middle'):
        return coordinate_samples(source.decode(), case).encode()
    if case in HISTORY_CASES:
        return history_access(source.decode(), case).encode()
    if case in ('sample-array', 'sample-query'):
        return sample_container(source.decode(), case).encode()
    if case in TIMER_BODY_CASES:
        timer = inputs['ZunTimer.cpp'].decode().replace('\r\n', '\n')
        if timer.count(TIMER_ASSIGNMENT) != 1 or timer.count(TIMER_SETTER) != 1:
            raise ValueError('actual Timer reset definitions changed')
        if case == 'timer-all':
            prefix = timer
        else:
            prefix = '#include "ZunTimer.hpp"\n'
            if case == 'timer-reset':
                prefix += TIMER_SETTER
            prefix += TIMER_ASSIGNMENT
        return prefix.encode() + b'\n' + source
    if case != 'interrupt':
        prefix = ''.join('#include "' + name + '"\n' for name in CASES[case])
        return prefix.encode() + source
    # The complete runtime CPP owns a different partial AnmVm view. Reuse its
    # actual method and typedef with the caller's complete native VM layout.
    runtime = inputs['AnmManagerRuntime.cpp'].decode().replace('\r\n', '\n')
    alias = 'typedef short i16;'
    body = ('void AnmVm::SetInterrupt(i16 interrupt)\n'
            '{\n    pendingInterrupt = interrupt;\n}\n')
    if runtime.count(alias) != 1 or runtime.count(body) != 1:
        raise ValueError('native AnmVm interrupt source changed')
    return source + ('\n' + alias + '\n' + body).encode()


def require_interrupt_unit(unit, profile):
    if (unit['source'], unit['symbol'], unit['target_address'], unit['size'],
            unit['profile'], unit.get('relocations', [])) != (
            'src/AnmManagerRuntime.cpp', INTERRUPT_SYMBOL, 0x406790, 18, profile, []):
        raise ValueError('native AnmVm interrupt owner/profile changed')


def require_pointer_view_units(units, profile):
    for name, symbol in POINTER_VIEW_UNITS:
        unit = units[name]
        if (unit['source'], unit['symbol'], unit['target_address'], unit['size'],
                unit['profile'], unit.get('relocations', [])) != (
                'src/VectorPointerViews.cpp', symbol, 0x4343D0, 3, profile, []):
            raise ValueError('native pointer-view alias extent/profile changed: ' + name)


def require_timer_compare_unit(unit, profile):
    if profile.count('/Ob1') != 1 or '/Ob0' in profile:
        raise ValueError('Type3 comparison context lost its existing Ob1 profile')
    canonical_profile = ['/Ob0' if flag == '/Ob1' else flag for flag in profile]
    if (unit['source'], unit['object'], unit['symbol'], unit['target_address'],
            unit['size'], unit['profile'], unit.get('relocations', [])) != (
            'src/BackgroundStageScript.cpp', 'build/matching/BackgroundStageScript.obj',
            TIMER_COMPARE_SYMBOL, 0x401520, 20, canonical_profile, []):
        raise ValueError('canonical Timer comparison source/profile/ABI changed')


def timer_body_units(case, units, profile):
    names = ['zun-timer-assign-int']
    if case == 'timer-reset':
        names.append('zun-timer-set-current')
    elif case == 'timer-all':
        names = [name for name, unit in units.items()
                 if unit['source'] == 'src/ZunTimer.cpp']
        if len(names) != 17:
            raise ValueError('reviewed complete Timer unit inventory changed')
    elif case != 'timer-assignment':
        raise ValueError('unknown Timer body context')
    selected = {name: units[name] for name in names}
    if any(unit['source'] != 'src/ZunTimer.cpp' or unit['profile'] != profile
           for unit in selected.values()):
        raise ValueError('native Timer body source/profile changed')
    return selected


def prove_timer_bodies(obj, selected, target, coff):
    """Check actual code, operands and local data before reporting visibility."""
    literals = module('context_timer_literals', 'coff_literal_definitions.py')
    proofs = []
    for name, unit in selected.items():
        raw, rows = coff.object_function(obj, unit['symbol'],
                                        include_symbol_locations=True)
        expected = unit.get('relocations', [])
        if len(raw) != unit['size'] or len(rows) != len(expected):
            raise ValueError('native Timer extent/field count changed: ' + name)
        linked = bytearray(raw)
        for actual, known in zip(rows, expected):
            if (actual['offset'], actual['type'], actual['symbol'], actual['addend']) != (
                    known['offset'], known['type'], known['symbol'], known.get('addend', 0)):
                raise ValueError('native Timer operand identity changed: ' + name)
            value = known['target'] + actual['addend']
            if actual['type'] == 'REL32':
                value -= unit['target_address'] + actual['offset'] + 4
            elif actual['type'] != 'DIR32':
                raise ValueError('unreviewed native Timer relocation type')
            linked[actual['offset']:actual['offset'] + 4] = (value & 0xffffffff).to_bytes(4, 'little')
        if linked != coff.pe_bytes_at(target, unit['target_address'], unit['size']):
            raise ValueError('visible native Timer lost complete original equality: ' + name)
        definitions = literals.check_literal_definitions(
            obj.read_bytes(), rows, target, coff,
            {row['symbol']: row['target'] for row in expected})
        proofs.append(dict(unit=name, symbol=unit['symbol'], bytes=len(raw),
                           complete_original_equal=True, raw_hex=raw.hex(),
                           coff=rows, literal_definitions=definitions))
    return proofs


def compiled_inputs(inputs, case, variant):
    staged = dict(inputs)
    staged[SOURCE] = render_source(inputs, case, variant)
    if case == 'timer-plain':
        name = 'ZunTimerGreaterEqual.inl'
        annotated = b'__declspec(noinline) inline unsigned int ZunTimer::operator>=(int value)'
        plain = b'unsigned int ZunTimer::operator>=(int value)'
        if staged[name].count(annotated) != 1:
            raise ValueError('actual Timer comparison definition changed')
        # The shared body and native signature are unchanged. This is only
        # an isolated ordinary out-of-line definition, not a header adoption.
        staged[name] = plain + staged[name].split(annotated, 1)[1]
    return staged


def run(output, case, variant='baseline'):
    if case not in CASES or variant not in VARIANTS:
        raise ValueError('unknown Type3 native context')
    coff = module('context_coff', 'compare-coff-function.py')
    audit = module('context_audit', 'inspect-exattack-type3-schedule.py')
    guard = module('context_guard', 'check-boss-frontiers.py')
    target = coff.verified_target()
    report_path = output / report_name(case, variant)
    if report_path.is_symlink():
        raise ValueError('refusing symlinked context report')
    report_path.unlink(missing_ok=True)
    inputs, profile = inputs_for(case)
    manifest = (ROOT / 'config/match-units.toml').read_bytes()
    units = tomllib.loads(manifest.decode())['units']
    unit = units['ex-attack-initialize-type3']
    if (unit['source'], unit['target_address'], unit['size'], unit['profile']) != (
            'src/ExAttackInitType3.cpp', 0x00442580, 454, profile):
        raise ValueError('Type3 original-profile reference changed')
    if case == 'interrupt':
        require_interrupt_unit(units['anm-vm-set-interrupt'], profile)
    if case == 'pointer-views':
        require_pointer_view_units(units, profile)
    if case == 'timer-plain':
        require_timer_compare_unit(units['zun-timer-greater-equal'], profile)
    timer_units = timer_body_units(case, units, profile) if case in TIMER_BODY_CASES else {}
    tool_names = (
        'config/tools.lock.toml', 'config/target.toml', 'scripts/compile-probe.sh',
        'scripts/compare-coff-function.py', 'scripts/inspect-exattack-type3-schedule.py',
        'scripts/inspect-exattack-type3.py', 'scripts/inspect-exattack-type01.py',
        'scripts/diagnostic_branch_alignment.py', 'scripts/check-boss-frontiers.py',
        'scripts/coff_literal_definitions.py',
        'scripts/probe-exattack-type3-word-order.py', 'scripts/probe-type3-native-context.py')
    tools = {name: (ROOT / name).read_bytes() for name in tool_names}
    build_root = ROOT / 'build' / 'diagnostics'
    build_root.mkdir(parents=True, exist_ok=True)
    with (tempfile.TemporaryDirectory(prefix='type3-' + case + '-', dir=output) as scratch,
          tempfile.TemporaryDirectory(prefix='type3-context-', dir=build_root) as binary):
        source_root = Path(scratch)
        staged = compiled_inputs(inputs, case, variant)
        for filename, content in staged.items():
            path = source_root / filename
            path.parent.mkdir(parents=True, exist_ok=True)
            path.write_bytes(content)
        source = source_root / SOURCE
        source_digest = hashlib.sha256(source.read_bytes()).hexdigest()
        obj = Path(binary) / 'unit.obj'
        compiled = subprocess.run(
            ['nice', '-n', '10', str(ROOT / 'scripts/compile-probe.sh'),
             str(source), str(obj), *profile], cwd=ROOT, capture_output=True, text=True)
        if compiled.returncode or not obj.is_file():
            raise RuntimeError('Type3 context compiler failed: ' +
                               (compiled.stdout + compiled.stderr)[-1600:])
        if any((source_root/name).read_bytes() != data for name, data in staged.items()):
            raise ValueError('actual compiled Type3 project inputs changed')
        native_timer_bodies = (prove_timer_bodies(obj, timer_units, target, coff)
                               if timer_units else None)
        diagnostic = audit.context_report(obj)
        raw, rows = coff.object_function(obj, audit.SYMBOL, include_symbol_locations=True)
        if timer_units:
            calls = [row for row in rows if row['symbol'] == '??4ZunTimer@@QAEXH@Z']
            if (len(calls) != 1 or calls[0].get('symbol_section', 0) <= 0
                    or calls[0].get('symbol_value') != 0
                    or calls[0]['type'] != 'REL32' or calls[0]['addend'] != 0):
                raise ValueError('Type3 lost its actual same-object Timer reset call')
        try:
            strict = audit.analyze(obj)
            guard.guard_exattack_type3_update(strict)
            frontier = {'passed': True}
        except (ValueError, RuntimeError) as error:
            frontier = {'passed': False, 'reason': str(error)}
        if case == 'baseline' and variant == 'baseline' and not frontier['passed']:
            raise ValueError('maintained Type3 source no longer passes its frontier')
        native_interrupt = None
        native_pointer_views = None
        native_timer_compare = None
        if case == 'interrupt':
            body, fields = coff.object_function(
                obj, INTERRUPT_SYMBOL, include_symbol_locations=True)
            if fields or body != coff.pe_bytes_at(target, 0x406790, 18):
                raise ValueError('visible native interrupt lost complete original equality')
            native_interrupt = dict(unit='anm-vm-set-interrupt', bytes=18,
                                    fields=0, complete_original_equal=True,
                                    raw_hex=body.hex(), coff=fields)
        if case == 'pointer-views':
            aliases = []
            for name, symbol in POINTER_VIEW_UNITS:
                body, fields = coff.object_function(
                    obj, symbol, include_symbol_locations=True)
                if fields or body != coff.pe_bytes_at(target, 0x4343D0, 3):
                    raise ValueError('visible native pointer view lost complete original equality')
                aliases.append(dict(unit=name, symbol=symbol, bytes=3, fields=0,
                                    complete_original_equal=True, raw_hex=body.hex()))
            native_pointer_views = dict(aliases=aliases, physical_bodies=1,
                                        physical_bytes=3, original_origin='unknown',
                                        authored_exact_credit=0)
        if case == 'timer-plain':
            body, fields = coff.object_function(obj, TIMER_COMPARE_SYMBOL,
                                                include_symbol_locations=True)
            if fields or body != coff.pe_bytes_at(target, 0x401520, 20):
                raise ValueError('plain native Timer comparison lost complete original equality')
            calls = [row for row in rows if row['symbol'] == TIMER_COMPARE_SYMBOL]
            if (len(calls) != 2 or any(row.get('symbol_section', 0) <= 0
                                      or row.get('symbol_value') != 0
                                      or row['type'] != 'REL32' or row['addend'] != 0
                                      for row in calls)):
                raise ValueError('Type3 lost its actual same-object Timer comparison calls')
            native_timer_compare = dict(unit='zun-timer-greater-equal', bytes=20,
                                        fields=0, complete_original_equal=True,
                                        raw_hex=body.hex(), call_fields=calls)
        report = {
            'diagnostic_only': True, 'exactness_credit': 'none', 'case': case,
            'source_variant': variant, 'native_interrupt': native_interrupt,
            'native_pointer_views': native_pointer_views,
            'native_timer_compare': native_timer_compare,
            'native_timer_bodies': native_timer_bodies,
            'source_commit': subprocess.check_output(
                ['git', 'rev-parse', 'HEAD'], cwd=ROOT, text=True).strip(),
            'target_sha256': hashlib.sha256(target).hexdigest(), 'profile': profile,
            'source_inputs': {name: hashlib.sha256(data).hexdigest()
                              for name, data in inputs.items()},
            'compiled_source_sha256': source_digest,
            'compiled_input_sha256': {name: hashlib.sha256(data).hexdigest()
                                      for name, data in staged.items()},
            'manifest_sha256': hashlib.sha256(manifest).hexdigest(),
            'tool_hashes': {name: hashlib.sha256(data).hexdigest()
                            for name, data in tools.items()},
            'diagnostic': diagnostic, 'production_frontier': frontier,
            'raw_hex': raw.hex(), 'coff': rows, 'compiler_artifacts_retained': False,
        }
    if (inputs_for(case)[0] != inputs or
            (ROOT / 'config/match-units.toml').read_bytes() != manifest or
            any((ROOT / name).read_bytes() != data for name, data in tools.items())):
        raise ValueError('Type3 context inputs changed during compilation')
    return report


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--case', choices=tuple(CASES), default='baseline')
    parser.add_argument('--variant', choices=VARIANTS, default='baseline')
    parser.add_argument('--output', type=Path,
                        default=ROOT / '.analysis' / 'type3-native-context')
    args = parser.parse_args()
    output = args.output.resolve()
    if args.output.is_symlink() or not output.is_relative_to((ROOT / '.analysis').resolve()):
        parser.error('output must be below repository .analysis without a symlink')
    output.mkdir(parents=True, exist_ok=True)
    report = run(output, args.case, args.variant)
    path = output / report_name(args.case, args.variant)
    with gzip.open(path, 'wt', encoding='utf-8') as stream:
        json.dump(report, stream, indent=2)
        stream.write('\n')
    d = report['diagnostic']
    print(args.case, args.variant, d['candidate_bytes'], 'bytes;', d['bound_fields'], 'fields;',
          d['overlap_differences'], 'overlap differences;',
          d['missing_target_bytes'], 'missing;', 'frontier', report['production_frontier']['passed'])
    return 0


if __name__ == '__main__':
    raise SystemExit(main())
