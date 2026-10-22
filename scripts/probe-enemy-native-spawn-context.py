#!/usr/bin/env python3
"""One original-bound Enemy experiment with genuine co-visible native bodies.

A successful diagnostic is not exact credit. Every native definition must
independently match its complete original extent before the caller is scored.
Only the selected case is compiled, serially at nice 10, in temporary storage.
"""
from __future__ import annotations

import argparse
import hashlib
import importlib.util
import json
from pathlib import Path
import re
import struct
import subprocess
import sys
import tempfile
import tomllib

ROOT = Path(__file__).resolve().parents[1]
TARGET_SHA256 = '10350095bcf95edb59e03bee9849a2dc8a7714b4927ad5909c569c550fce6822'
CASE_OWNERS = {
    'standalone': ('enemy-collision-append-box', 'float3-divide',
                   'player-collision-append-box', 'enemy-attached-effect-update',
                   'zun-timer-greater'),
    'spawn': ('effect-manager-spawn-effect',),
    'spawn-periodic': ('effect-manager-spawn-effect', 'zun-timer-has-ticked-every'),
    'constructor-periodic': ('bullet-spawn-descriptor-constructor',
                             'zun-timer-has-ticked-every'),
    'spawn-constructor-periodic': ('effect-manager-spawn-effect',
                                   'bullet-spawn-descriptor-constructor',
                                   'zun-timer-has-ticked-every'),
}
STANDALONE_CASES = frozenset(('standalone', 'standalone-periodic-first',
                              'standalone-periodic-last', 'standalone-pointer-view',
                              'standalone-vm-packed-color', 'standalone-periodic-add',
                              'standalone-periodic-add-constructor',
                              'standalone-bullet-primary', 'standalone-bullet-primary-periodic'))
CASE_OWNERS['standalone-vm-packed-color'] = CASE_OWNERS['standalone']
for _case in ('standalone-periodic-first', 'standalone-periodic-last'):
    CASE_OWNERS[_case] = CASE_OWNERS['standalone'] + ('zun-timer-has-ticked-every',)
CASE_OWNERS['standalone-pointer-view'] = (
    CASE_OWNERS['standalone'] + ('float3-pointer-identity-view',))
CASE_OWNERS['standalone-periodic-add'] = (
    CASE_OWNERS['standalone-periodic-first'] + ('float3-add',))
CASE_OWNERS['standalone-periodic-add-constructor'] = (
    CASE_OWNERS['standalone-periodic-first'] + ('float3-constructor', 'float3-add'))
CASE_OWNERS['standalone-bullet-primary'] = (
    CASE_OWNERS['standalone'] + ('bullet-pattern-primary',))
CASE_OWNERS['standalone-bullet-primary-periodic'] = (
    CASE_OWNERS['standalone-periodic-first'] + ('bullet-pattern-primary',))
SCALAR_COPY_CASES = frozenset('standalone-scalar-'+name for name in
                              ('draw', 'threshold', 'side', 'character', 'all'))
STANDALONE_CASES = STANDALONE_CASES | SCALAR_COPY_CASES
for _case in sorted(SCALAR_COPY_CASES):
    CASE_OWNERS[_case] = CASE_OWNERS['standalone']
CASES = tuple(CASE_OWNERS)
NATIVE_OWNERS = {
    'bullet-pattern-primary': (0x4130F0, 203,
        '?SpawnBulletPatternPrimary@EtamaController@@QAEPAUBullet@@PAUBulletSpawnDescriptor@@@Z', 1),
    'enemy-collision-append-box': (
        0x410250, 82, '?AppendPlayerCollisionBox@EnemyAppendCollisionView@@QAEXPBUFloat3@@0@Z', 2),
    'float3-divide': (0x40F5A0, 56, '??KFloat3@@QBE?AU0@M@Z', 2),
    'float3-constructor': (0x4010B0, 26, '??0Float3@@QAE@MMM@Z', 1),
    'float3-add': (0x401100, 54, '??HFloat3@@QBE?AU0@ABU0@@Z', 3),
    'player-collision-append-box': (
        0x40F8E0, 205, '?AppendBoxRecord@PlayerCollisionQueryStateView@@QAEXPBUPlayerPositionView@@0H@Z', 1),
    'enemy-attached-effect-update': (
        0x40F4C0, 151, '?EnemyCoreUpdateAttachedEffects@@YIXPAUEnemyCoreView@@@Z', 1),
    'zun-timer-greater': (0x403DE0, 20, '??OZunTimer@@QAEIH@Z', 4),
    'effect-manager-spawn-effect': (
        0x40CC00, 184, '?SpawnEffect@EffectManager@@QAEPAUEffect@@HPBUEffectFloat3@@HI@Z', 4),
    'zun-timer-has-ticked-every': (
        0x404920, 33, '?HasTickedEvery@ZunTimer@@QAEHH@Z', 1),
    'bullet-spawn-descriptor-constructor': (
        0x40D500, 28, '??0BulletSpawnDescriptorCtorView@@QAE@XZ', 1),
    'float3-pointer-identity-view': (
        0x4343D0, 3, '??BFloat3@@QAEPAMXZ', 1),
}


def digest(data):
    return hashlib.sha256(data).hexdigest()


def bullet_primary_body(data):
    """Use the real declaration dependencies and complete canonical method."""
    source = data.decode().replace('\r\n', '\n')
    selected = []
    boundaries = (
        ('struct BulletCancelCollisionView', 'struct BulletTimerCurrentView'),
        ('struct BulletPlayerView\n', 'struct BulletGameManagerView'),
        ('enum BulletTransformFlags', None),
        ('Bullet *EtamaController::SpawnBulletPatternPrimary(',
         'Bullet *EtamaController::SpawnBulletPatternSecondary('),
    )
    for begin, end in boundaries:
        if source.count(begin) != 1 or (end and source.count(end) != 1):
            raise ValueError('native bullet primary source boundary changed')
        first = source.index(begin)
        last = source.index(end) if end else source.index('};', first)+2
        if last <= first:
            raise ValueError('native bullet primary boundaries reversed')
        selected.append(source[first:last].strip())
    return '#include "BulletManager.hpp"\n#include "SoundPlayer.hpp"\n'+'\n'.join(selected)+'\n'


def native_bodies(case, sources):
    """Expose only selected real definitions, preserving the old two recipes."""
    selected = CASE_OWNERS[case]
    bodies = ''
    if 'bullet-pattern-primary' in selected:
        bodies += bullet_primary_body(sources['BulletManager.cpp'])
    if 'float3-pointer-identity-view' in selected:
        bodies += sources['VectorPointerViews.cpp'].decode() + '\n'
    if 'zun-timer-has-ticked-every' in selected:
        bodies += sources['ZunTimerHasTickedEvery.cpp'].decode() + '\n'
    if 'float3-add' in selected:
        source = sources['Float3Core.inl'].decode().replace('\r\n', '\n')
        boundaries = ('Float3::Float3(', 'Float3 Float3::operator+',
                      'Float3 Float3::operator-')
        if any(source.count(anchor) != 1 for anchor in boundaries):
            raise ValueError('native Float3 source boundaries changed')
        constructor, addition, following = (source.index(anchor) for anchor in boundaries)
        if not constructor < addition < following:
            raise ValueError('native Float3 source boundaries reversed')
        if 'float3-constructor' in selected:
            bodies += source[constructor:addition] + '\n'
        bodies += source[addition:following] + '\n'
    if 'bullet-spawn-descriptor-constructor' in selected:
        bodies += sources['BulletSpawnDescriptorCtor.cpp'].decode() + '\n'
    if 'effect-manager-spawn-effect' in selected:
        source = sources['EffectManager.cpp'].decode().replace('\r\n', '\n')
        entry = 'Effect *EffectManager::SpawnEffect('
        following = 'Effect *EffectManager::SpawnEffectWithVelocity('
        if source.count(entry) != 1 or source.count(following) != 1:
            raise ValueError('native SpawnEffect source boundaries changed')
        start, end = source.index(entry), source.index(following)
        if end <= start:
            raise ValueError('native SpawnEffect source boundaries reversed')
        bodies += '#include "EffectManager.hpp"\n' + source[start:end]
    return bodies


def standalone_source(case, bodies):
    """Keep the declaration-first carrier and expose the actual selected body."""
    if case not in STANDALONE_CASES:
        raise ValueError('not a reviewed standalone Enemy case')
    if (case in ({'standalone', 'standalone-vm-packed-color'} | SCALAR_COPY_CASES)) != (not bodies):
        raise ValueError('standalone native-body selection is incomplete')
    generated = ('#include "EclManager.hpp"\n'
                 '#include "EclRunState.inl"\n')
    if bodies and case != 'standalone-periodic-last':
        generated += bodies+'\n'
    generated += '#include "EnemyManagerCore.inl"\n'
    if case == 'standalone-periodic-last':
        generated += bodies+'\n'
    return generated


def replace_scalar_once(text, old, new):
    if text.count(old) != 1:
        raise ValueError('reviewed scalar read changed: '+old.splitlines()[0])
    return text.replace(old, new, 1)


def scalar_copy_context(text, case):
    if case not in SCALAR_COPY_CASES:
        raise ValueError('unknown Enemy scalar-copy context')
    case = case.removeprefix('standalone-scalar-')
    if case in ('draw', 'all'):
        old = '''                drawHead =
                    &manager->drawGroupHeads2AC410[enemy->drawGroup3387];'''
        new = '''                unsigned char drawGroup;
                memcpy(&drawGroup, &enemy->drawGroup3387, sizeof(drawGroup));
                drawHead = &manager->drawGroupHeads2AC410[drawGroup];'''
        text = replace_scalar_once(text, old, new)
    if case in ('threshold', 'all'):
        old = '''                if (enemy->specialAttackTimer5424 >
                        manager->specialAttackThreshold2AC44C)'''
        new = '''                int specialAttackThreshold;
                memcpy(&specialAttackThreshold, &manager->specialAttackThreshold2AC44C,
                       sizeof(specialAttackThreshold));
                if (enemy->specialAttackTimer5424 > specialAttackThreshold)'''
        text = replace_scalar_once(text, old, new)
    if case in ('side', 'all'):
        old = '                    EnemyCoreSideStateView *spawnSide = manager->sideState320;'
        new = '''                    EnemyCoreSideStateView *spawnSide;
                    memcpy(&spawnSide, &manager->sideState320, sizeof(spawnSide));'''
        text = replace_scalar_once(text, old, new)
    if case in ('character', 'all'):
        old = '''                    descriptor.count1 =
                        static_cast<short>(spawnSide->characterIndex20 == 13 ? 1 : 3);'''
        new = '''                    int characterIndex;
                    memcpy(&characterIndex, &spawnSide->characterIndex20, sizeof(characterIndex));
                    descriptor.count1 = static_cast<short>(characterIndex == 13 ? 1 : 3);'''
        text = replace_scalar_once(text, old, new)
    return text


def packed_vm_color(source):
    """A consumed whole-word color model, retaining native VM ownership."""
    first = '                reinterpret_cast<unsigned char *>(&enemy->primaryVm008.color2)[2] = 0xFF;'
    after = '                enemy->primaryVm008.flagsWord |= 0x20000;'
    if source.count(first) != 1 or source.count(after) != 1:
        raise ValueError('native VM color source contract changed')
    start, end = source.index(first), source.index(after)
    expected = '''                reinterpret_cast<unsigned char *>(&enemy->primaryVm008.color2)[2] = 0xFF;
                reinterpret_cast<unsigned char *>(&enemy->primaryVm008.color2)[1] = 0x60;
                reinterpret_cast<unsigned char *>(&enemy->primaryVm008.color2)[0] = 0x80;
                reinterpret_cast<unsigned char *>(&enemy->primaryVm008.color2)[3] =
                    static_cast<unsigned char>(
                        enemy->primaryVm008.color1 >> 24);
'''
    if source[start:end] != expected:
        raise ValueError('native VM byte values/order changed')
    replacement = '''                enemy->primaryVm008.color2 =
                    (enemy->primaryVm008.color1 & 0xFF000000UL) | 0x00FF6080UL;
'''
    return source[:start]+replacement+source[end:]


def compare_native_definition(raw, fields, unit, target):
    """Bind real COFF operands; reject changed native code despite a good caller."""
    if len(raw) != unit['size'] or len(target) != unit['size']:
        raise ValueError('native definition complete extent differs')
    expected = unit.get('relocations', [])
    if len(fields) != len(expected):
        raise ValueError('native definition field inventory differs')
    linked = bytearray(raw)
    previous = 0
    for row, reference in zip(fields, expected):
        offset, kind = row['offset'], row['type']
        if (type(offset) is not int or offset < previous or offset + 4 > len(raw)
                or (offset, kind, row['symbol'], row['addend']) !=
                   (reference['offset'], reference['type'], reference['symbol'], 0)
                or kind not in ('REL32', 'DIR32')):
            raise ValueError('native definition field identity/order/addend differs')
        destination = reference['target']
        value = (destination - (unit['target_address'] + offset + 4
                               if kind == 'REL32' else 0)) & 0xffffffff
        if struct.unpack_from('<I', target, offset)[0] != value:
            raise ValueError('native original operand disagrees with reviewed manifest')
        struct.pack_into('<I', linked, offset, value)
        previous = offset + 4
    if linked != target:
        raise ValueError('native definition complete bytes differ from original')
    return dict(bytes=len(raw), fields=len(fields), raw_sha256=digest(raw),
                linked_sha256=digest(linked), target_sha256=digest(target),
                complete_bytes_equal=True)


def require_caller_definition(fields, symbol, count):
    """The caller must reference the unique extracted same-object definition."""
    observed = [r for r in fields if r['symbol'] == symbol]
    if len(observed) != count:
        raise ValueError('native caller definition reference count differs')
    sections = set()
    for row in observed:
        section = row.get('symbol_section')
        if (row['type'] != 'REL32' or row['addend'] != 0
                or type(section) is not int or section <= 0
                or section == row.get('owner_section') or row.get('symbol_value') != 0):
            raise ValueError('native caller still references an opaque or unrelated definition')
        sections.add(section)
    if len(sections) != 1:
        raise ValueError('native caller references inconsistent definition sections')
    return dict(calls=count, section=sections.pop(), symbol_value=0)


def module(name, filename):
    spec = importlib.util.spec_from_file_location(name, ROOT/'scripts'/filename)
    value = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(value)
    return value


def run(case, output):
    sys.path.insert(0, str(ROOT/'scripts'))
    coff = module('native_spawn_coff', 'compare-coff-function.py')
    literals = module('native_spawn_literals', 'coff_literal_definitions.py')
    image = coff.verified_target()
    if digest(image) != TARGET_SHA256:
        raise ValueError('original Japanese TH09 v1.50a target required')
    units_path = ROOT/'config/match-units.toml'
    units_bytes = units_path.read_bytes()
    units = tomllib.loads(units_bytes.decode())['units']
    profile_unit = units['enemy-attached-effect-update']
    if profile_unit['source'] != 'src/EclManager.cpp':
        raise ValueError('reviewed ECL/Enemy source carrier changed')
    profile = profile_unit['profile']
    inputs = {}

    def collect(name):
        path = (ROOT/'src'/name).resolve()
        if not path.is_relative_to((ROOT/'src').resolve()):
            raise ValueError('project include escapes source root')
        if name in inputs:
            return
        data = path.read_bytes()
        inputs[name] = data
        for include in re.findall(rb'^\s*#\s*include\s*"([^"]+)"', data, re.M):
            collect((Path(name).parent/include.decode()).as_posix())

    standalone = case in STANDALONE_CASES
    if standalone:
        for name in ('EclManager.hpp', 'EclRunState.inl', 'EnemyManagerCore.inl'):
            collect(name)
    else:
        collect('EclManager.cpp')
    if 'float3-pointer-identity-view' in CASE_OWNERS[case]:
        collect('VectorPointerViews.cpp')
    observed = {ROOT/'src'/n: data for n, data in inputs.items()}
    native_sources = []
    if 'bullet-pattern-primary' in CASE_OWNERS[case]:
        native_sources.append('BulletManager.cpp')
    if 'float3-pointer-identity-view' in CASE_OWNERS[case]:
        native_sources.append('VectorPointerViews.cpp')
    if 'float3-add' in CASE_OWNERS[case]:
        native_sources.append('Float3Core.inl')
    if 'effect-manager-spawn-effect' in CASE_OWNERS[case]:
        native_sources.append('EffectManager.cpp')
    if 'zun-timer-has-ticked-every' in CASE_OWNERS[case]:
        native_sources.append('ZunTimerHasTickedEvery.cpp')
    if 'bullet-spawn-descriptor-constructor' in CASE_OWNERS[case]:
        native_sources.append('BulletSpawnDescriptorCtor.cpp')
    for name in native_sources:
        path = ROOT/'src'/name
        observed[path] = path.read_bytes()
    for name in ('config/tools.lock.toml', 'config/functions.csv',
                 'scripts/compile-probe.sh', 'scripts/compare-coff-function.py',
                 'scripts/inspect-enemy-manager-update.py', 'scripts/inspect-ecl-complete.py',
                 'scripts/check-boss-frontiers.py', 'scripts/probe-enemy-native-spawn-context.py',
                 'scripts/coff_literal_definitions.py'):
        path = ROOT/name
        observed[path] = path.read_bytes()
    observed[units_path] = units_bytes
    observed[coff.TARGET_MANIFEST] = coff.TARGET_MANIFEST.read_bytes()
    hashes = {p.relative_to(ROOT).as_posix(): digest(b) for p, b in observed.items()}
    bodies = native_bodies(case, {n: observed[ROOT/'src'/n] for n in native_sources})
    if standalone:
        generated = standalone_source(case, bodies)
    else:
        source = inputs['EclManager.cpp'].decode().replace('\r\n', '\n')
        anchor = '#include "EnemyManagerCore.inl"'
        if source.count(anchor) != 1:
            raise ValueError('reviewed Enemy lexical include changed')
        generated = source.replace(anchor, bodies+'\n'+anchor, 1)
    report = dict(state='incomplete', diagnostic_only=True, exactness_credit='none',
                  case=case, profile=profile, source_inputs_sha256=hashes,
                  target_sha256=digest(image), native_definitions={})
    report_path = output/(case+'.json')
    if report_path.is_symlink():
        raise ValueError('refusing symlinked report')
    report_path.write_text(json.dumps(report, indent=2)+'\n')
    try:
        with tempfile.TemporaryDirectory(prefix='native-spawn-', dir=output) as temporary:
            root = Path(temporary)
            for name, data in inputs.items():
                (root/name).write_bytes(data)
            if case == 'standalone-vm-packed-color':
                (root/'EnemyManagerCore.inl').write_text(
                    packed_vm_color(inputs['EnemyManagerCore.inl'].decode()))
            if case in SCALAR_COPY_CASES:
                (root/'EnemyManagerCore.inl').write_text(
                    scalar_copy_context(inputs['EnemyManagerCore.inl'].decode(), case))
            (root/'EclManager.cpp').write_text(generated)
            staged = {n: digest((root/n).read_bytes()) for n in inputs}
            staged['EclManager.cpp'] = digest((root/'EclManager.cpp').read_bytes())
            obj = root/'unit.obj'
            build = subprocess.run(['nice', '-n', '10', str(ROOT/'scripts/compile-probe.sh'),
                                    str(root/'EclManager.cpp'), str(obj), *profile],
                                   cwd=ROOT, capture_output=True, text=True)
            report.update(compile_exit=build.returncode, compile_log=build.stdout+build.stderr,
                          compiled_inputs_sha256=staged)
            if build.returncode:
                raise ValueError('pinned native compiler failed: '+report['compile_log'][-1000:])
            enemy = module('native_spawn_enemy', 'inspect-enemy-manager-update.py')
            raw, rows = coff.object_function(obj, enemy.SYMBOL, include_symbol_locations=True)
            report.update(raw_hex=raw.hex(), coff=rows)
            for name in CASE_OWNERS[case]:
                unit = units[name]
                address, size, symbol, count = NATIVE_OWNERS[name]
                if (unit['target_address'], unit['size'], unit['symbol']) != (address, size, symbol):
                    raise ValueError('canonical native target contract changed')
                body, fields = coff.object_function(obj, symbol, include_symbol_locations=True)
                native = dict(raw_hex=body.hex(), coff=fields,
                              **compare_native_definition(body, fields, unit,
                                                          coff.pe_bytes_at(image, address, size)))
                caller_rows = rows
                if standalone and name in ('float3-divide', 'player-collision-append-box'):
                    collision_rows = report['native_definitions'][
                        'enemy-collision-append-box']['coff']
                    caller_rows = (rows+collision_rows if name == 'float3-divide'
                                   else collision_rows)
                native['caller_definition'] = require_caller_definition(caller_rows, symbol, count)
                if fields and any(r['owner_section'] != native['caller_definition']['section']
                                  or r['owner_value'] != 0 for r in fields):
                    raise ValueError('native body and actual caller refer to different definitions')
                native['literal_definitions'] = literals.check_literal_definitions(
                    obj.read_bytes(), fields, image, coff,
                    {r['symbol']: r['target'] for r in unit.get('relocations', [])})
                if name == 'float3-pointer-identity-view':
                    if unit['source'] != 'src/VectorPointerViews.cpp' or unit['profile'] != profile:
                        raise ValueError('folded pointer-view source/profile changed')
                    native.update(original_origin='unknown', authored_exact_credit=0)
                report['native_definitions'][name] = native
            if standalone:
                report['enemy'] = enemy.diagnose_standalone(obj)
                report['runecl'] = None
                report['runecl_compilation_skipped'] = True
            else:
                report['enemy'] = enemy.diagnose(obj)
                ecl = module('native_spawn_ecl', 'inspect-ecl-complete.py')
                report['runecl'] = ecl.report(obj)
            frontier = module('native_spawn_frontier', 'check-boss-frontiers.py')
            try:
                report['frontier'] = frontier.guard_enemy_update(report['enemy'])
                report['frontier_guard_passed'] = True
            except RuntimeError as error:
                report.update(frontier_guard_passed=False, frontier_error=str(error))
            if staged != {n: digest((root/n).read_bytes()) for n in staged}:
                raise ValueError('staged source changed during compilation')
        if any(p.read_bytes() != data for p, data in observed.items()):
            raise ValueError('live source/provider changed during native experiment')
        report['state'] = 'complete-diagnostic'
    except (OSError, ValueError, KeyError, RuntimeError) as error:
        report.update(state='rejected', error=str(error))
        report_path.write_text(json.dumps(report, indent=2)+'\n')
        raise
    report_path.write_text(json.dumps(report, indent=2)+'\n')
    return report


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--case', choices=CASES, default='spawn')
    parser.add_argument('--output', type=Path, default=ROOT/'.analysis/enemy-native-spawn-context')
    args = parser.parse_args()
    output = args.output.resolve()
    if not output.is_relative_to((ROOT/'.analysis').resolve()) or output == (ROOT/'.analysis').resolve():
        parser.error('output must be below this repository .analysis directory')
    output.mkdir(parents=True, exist_ok=True)
    report_path = output/(args.case+'.json')
    if report_path.is_symlink():
        parser.error('refusing symlinked report')
    # Even target/source validation failure must invalidate an old success.
    report_path.write_text(json.dumps(dict(state='incomplete', diagnostic_only=True,
                                          exactness_credit='none', case=args.case))+'\n')
    try:
        report = run(args.case, output)
    except (OSError, ValueError, KeyError, RuntimeError) as error:
        print('Enemy native context rejected:', error, file=sys.stderr)
        return 2
    runecl_status = (str(report['runecl']['different_bytes'])+' RunEcl differences'
                    if report['runecl'] is not None else
                    'RunEcl not compiled; external original-call identity only')
    print(args.case, report['enemy']['candidate_bytes'], 'bytes;',
          report['enemy']['complete_differences'], 'Enemy differences;',
          runecl_status, ';',
          'native definitions exact; frontier', report['frontier_guard_passed'],
          '; diagnostic only')
    return 0


if __name__ == '__main__':
    raise SystemExit(main())
