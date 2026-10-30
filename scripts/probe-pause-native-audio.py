#!/usr/bin/env python3
"""Compile real Pause audio contexts; compare complete native bodies and caller.

Diagnostic only. This does not infer the original translation-unit partition,
accept the nonexact Pause callback, or establish native product/runtime closure.
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
NATIVE = {
    'soundplayer-resume-after-pause': (0x423498, 15),
    'supervisor-stop-audio': (0x42FD30, 81),
    'soundplayer-queue-command': (0x43EE40, 105),
    'supervisor-is-music-preloaded': (0x42ED40, 13),
}
CASES = {
    'baseline': (),
    'embedded-footer': (),
    'animate-cursor': (),
    'deferred-time': (),
    'deferred-time-opaque': (),
    'resume': ('soundplayer-resume-after-pause',),
    'stop': ('supervisor-stop-audio',),
    'audio': ('soundplayer-resume-after-pause', 'supervisor-stop-audio'),
    'all': tuple(NATIVE),
}


def digest(data):
    return hashlib.sha256(data).hexdigest()


def definition(text, marker):
    """Select one unchanged, brace-delimited definition from these real files."""
    if text.count(marker) != 1:
        raise ValueError('native source definition is not unique: '+marker)
    start = text.index(marker)
    opening = text.index('{', start)
    end, depth = opening + 1, 1
    while depth and end < len(text):
        depth += (text[end] == '{') - (text[end] == '}')
        end += 1
    if depth:
        raise ValueError('incomplete native source definition: '+marker)
    if text[end:end + 1] == ';':
        end += 1
    return text[start:end]


def control_context(source, case):
    """Move the real common exit or traverse the same seven native VMs."""
    body = definition(source, 'int PauseMenu::OnUpdate()')
    if digest(body.encode()) != '13460fb9dbe5ee8e73675604e80de437c1ca704bfc810082691d173fde5d54c7':
        raise ValueError('reviewed Pause control-flow source changed')
    original = body
    loop = ('    for (i = 0; i < 7; i++) {\n'
            '        g_AnmManager->ExecuteScript(&this->menuSprites[i]);\n    }')
    if body.count(loop) != 1:
        raise ValueError('reviewed Pause animation loop changed')
    if case == 'animate-cursor':
        body = body.replace(loop,
            '    AnmVm *end = &this->menuSprites[7];\n'
            '    for (AnmVm *vm = &this->menuSprites[0]; vm != end; ++vm) {\n'
            '        g_AnmManager->ExecuteScript(vm);\n    }')
    elif case == 'embedded-footer':
        start = body.index('\n'+loop)
        footer = body[start:body.rfind('\n}')]
        body = body[:start]+'\n}'
        # Every break in this reviewed body exits the outer state switch.
        body = body.replace('break;', 'goto animatePause;')
        marker = '    case PAUSE_MENU_STATE_CONFIRM_A_YES:'
        if body.count(marker) != 1:
            raise ValueError('reviewed Pause confirmation arm changed')
        body = body.replace(marker,
            '    default:\n    animatePause:\n'+footer+'\n\n'+marker)
    elif case in ('deferred-time', 'deferred-time-opaque'):
        clock = 'AsciiSupervisor()->systemTime79C = timeGetTime();'
        anchor = '    const int stateAtDispatch = this->state;'
        if (body.count(clock) != 4 or body.count(anchor) != 1
                or 'refreshTimestamp' in body):
            raise ValueError('reviewed Pause timestamp decisions changed')
        body = body.replace(anchor, anchor+'\n    bool refreshTimestamp = false;', 1)
        result_begin = body.index('g_Supervisor.StopAudio();')
        result_end = body.index('return 0;', result_begin)
        result_arm = body[result_begin:result_end]
        if result_arm.count(clock) != 1:
            raise ValueError('reviewed result-only timestamp changed')
        body = body[:result_begin] + result_arm.replace(
            clock, 'PAUSE_RESULT_TIMESTAMP') + body[result_end:]
        body = body.replace(clock, 'refreshTimestamp = true;')
        body = body.replace('PAUSE_RESULT_TIMESTAMP', clock)
        body = body.replace(loop,
            '    if (refreshTimestamp) {\n        '+clock+'\n    }\n\n'+loop, 1)
        if body.count(clock) != 2 or body.count('refreshTimestamp = true;') != 3:
            raise ValueError('Pause timestamp deferral is incomplete')
    else:
        raise ValueError('unreviewed Pause control context')
    result = source.replace(original, body)
    if case == 'deferred-time-opaque':
        if any(marker in result for marker in
               ('int AnmVm::IsVisible()', 'void AnmVm::SetInvisible()')):
            raise ValueError('native VM visibility methods must stay external')
        marker = 'int AsciiGameManagerView::HasFlagBit0()'
        result = result.replace(definition(result, marker), '', 1)
    return result


def source_context(case, sources):
    if case not in CASES:
        raise ValueError('unreviewed Pause audio context')
    if case in ('embedded-footer', 'animate-cursor',
                'deferred-time', 'deferred-time-opaque'):
        return control_context(sources['AsciiManagerMenu.cpp'], case)
    selected = CASES[case]
    prefix = []
    if 'soundplayer-queue-command' in selected:
        text = sources['SoundPlayerProcessQueues.cpp']
        prefix += ['#include <string.h>',
                   definition(text, 'enum SoundPlayerCommandOpcode'),
                   definition(text, 'void SoundPlayer::QueueCommand(')]
    if 'supervisor-is-music-preloaded' in selected:
        prefix.append(definition(sources['SupervisorRenderState.cpp'],
                                 'int Supervisor::IsMusicPreloaded()'))
    if 'soundplayer-resume-after-pause' in selected:
        prefix.append(definition(sources['SoundPlayerPause.cpp'],
                                 'void SoundPlayer::ResumeAfterPause()'))
    if 'supervisor-stop-audio' in selected:
        text = sources['SupervisorAudio.cpp']
        prefix += ['namespace {',
                   definition(text, 'struct SupervisorAudioMidiOutputView'),
                   definition(text, 'struct SupervisorAudioLayout'), '}',
                   definition(text, 'int Supervisor::StopAudio()')]
    source = sources['AsciiManagerMenu.cpp']
    marker = 'int PauseMenu::OnUpdate()'
    if source.count(marker) != 1:
        raise ValueError('Pause source definition is not unique')
    return source.replace(marker, '\n'.join(prefix)+'\n\n'+marker, 1)


def compare_native(raw, fields, unit, target):
    """Check full code, every actual operand and the independent original field."""
    if len(raw) != unit['size'] or len(target) != unit['size']:
        raise ValueError('native complete extent differs')
    expected = unit.get('relocations', [])
    if len(fields) != len(expected):
        raise ValueError('native field inventory differs')
    linked, previous = bytearray(raw), 0
    for row, reference in zip(fields, expected):
        offset, kind = row['offset'], row['type']
        symbol = row['symbol']
        # This private type's name is bound to its selected real declaration.
        # Only its compiler-generated anonymous-namespace token may differ.
        if re.fullmatch(r'\?StopPlayback@SupervisorAudioMidiOutputView@'
                        r'\?A0x[0-9a-f]+@@QAEXXZ', symbol):
            symbol = re.sub(r'\?A0x[0-9a-f]+', '?A0xLOCAL', symbol)
            expected_symbol = re.sub(r'\?A0x[0-9a-f]+', '?A0xLOCAL',
                                     reference['symbol'])
        else:
            expected_symbol = reference['symbol']
        addend = reference.get('addend', 0)
        if (type(offset) is not int or offset < previous or offset + 4 > len(raw)
                or kind not in ('DIR32', 'REL32')
                or (offset, kind, symbol, row['addend']) !=
                   (reference['offset'], reference['type'], expected_symbol, addend)
                or struct.unpack_from('<I', raw, offset)[0] != addend):
            raise ValueError('native field identity/order/addend differs')
        value = (reference['target'] + addend -
                 (unit['target_address'] + offset + 4 if kind == 'REL32' else 0))
        value &= 0xffffffff
        if struct.unpack_from('<I', target, offset)[0] != value:
            raise ValueError('native original field disagrees with manifest')
        struct.pack_into('<I', linked, offset, value)
        previous = offset + 4
    if linked != target:
        raise ValueError('native complete bytes differ from original')
    return dict(bytes=len(raw), fields=len(fields), raw_sha256=digest(raw),
                target_sha256=digest(target), complete_equal=True)


def definition_references(rows, symbol, count):
    observed = [row for row in rows if row['symbol'] == symbol]
    if len(observed) != count:
        raise ValueError('native same-object reference count differs')
    sections = set()
    for row in observed:
        section = row.get('symbol_section')
        if (row['type'] != 'REL32' or row['addend'] != 0
                or type(section) is not int or section <= 0
                or section == row.get('owner_section') or row.get('symbol_value') != 0):
            raise ValueError('native call still references an opaque/foreign definition')
        sections.add(section)
    if len(sections) != 1:
        raise ValueError('native calls reference different definition sections')
    return dict(calls=count, section=sections.pop(), value=0)


def module(name, filename):
    spec = importlib.util.spec_from_file_location(name, ROOT/'scripts'/filename)
    result = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(result)
    return result


def resume_object_proof(path, coff, image, unit):
    """The canonical instruction replay also needs the actual eight data bytes."""
    from coff_literal_definitions import check_literal_definitions

    if (unit['target_address'], unit['size']) != NATIVE['soundplayer-resume-after-pause']:
        raise ValueError('native resume original contract changed')
    raw, fields = coff.object_function(path, unit['symbol'], include_symbol_locations=True)
    target = coff.pe_bytes_at(image, unit['target_address'], unit['size'])
    proof = compare_native(raw, fields, unit, target)
    definitions = check_literal_definitions(
        path.read_bytes(), fields, image, coff,
        {row['symbol']: row['target'] for row in unit['relocations']})
    if (len(definitions) != 1 or definitions[0]['target'] != 0x48F510
            or definitions[0]['size'] != 8 or definitions[0]['references'] != [1]
            or definitions[0]['sha256'] != digest(b'UnPause\0')):
        raise ValueError('native resume UnPause payload/identity differs')
    proof['literal_definitions'] = definitions
    return dict(diagnostic_only=True, exactness_credit='none',
                provenance='Supplied object; source/profile not attested.',
                native_resume=proof, resume_literal_bytes=8)


def emit(result, output):
    rendered = json.dumps(result, indent=2)+'\n'
    if output:
        output.parent.mkdir(parents=True, exist_ok=True)
        with output.open('x') as stream:
            stream.write(rendered)
    else:
        print(rendered, end='')


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--case', choices=CASES, default='all')
    parser.add_argument('--check-resume-object', type=Path,
                        help='Check an existing full resume definition and actual UnPause data; no compile.')
    parser.add_argument('--output', type=Path)
    args = parser.parse_args()
    output = args.output.resolve() if args.output else None
    if output is not None:
        output.relative_to((ROOT/'.analysis').resolve())
        if output.exists():
            raise ValueError('refusing to replace existing evidence')
    sys.path.insert(0, str(ROOT/'scripts'))
    from coff_literal_definitions import check_literal_definitions

    coff = module('pause_audio_coff', 'compare-coff-function.py')
    inspect = module('pause_audio_inspect', 'inspect-pause-menu.py')
    frontier = module('pause_audio_frontier', 'check-boss-frontiers.py')
    image = coff.verified_target()
    units = tomllib.loads((ROOT/'config/match-units.toml').read_text())['units']
    if args.check_resume_object:
        emit(resume_object_proof(args.check_resume_object, coff, image,
                                 units['soundplayer-resume-after-pause']), output)
        return
    profile = units['ascii-manager-on-update']['profile']
    inputs = {}

    def collect(name):
        if name in inputs:
            return
        data = (ROOT/'src'/name).read_bytes()
        inputs[name] = data
        for include in re.findall(rb'^\s*#\s*include\s*"([^"]+)"', data, re.M):
            collect((Path(name).parent/include.decode()).as_posix())

    for name in ('AsciiManagerMenu.cpp', 'SoundPlayerPause.cpp', 'SupervisorAudio.cpp',
                 'SupervisorRenderState.cpp', 'SoundPlayerProcessQueues.cpp'):
        collect(name)
    sources = {name: data.decode().replace('\r\n', '\n') for name, data in inputs.items()}
    source = source_context(args.case, sources)
    result = dict(diagnostic_only=True, exactness_credit='none', case=args.case,
                  target_sha256=digest(image), profile=profile,
                  input_sha256={name: digest(data) for name, data in inputs.items()},
                  source_sha256=digest(source.encode()), native={},
                  recipe_sha256=digest(Path(__file__).read_bytes()),
                  diagnostic_provider_sha256={
                      name: digest((ROOT/'scripts'/name).read_bytes())
                      for name in ('inspect-pause-menu.py', 'check-boss-frontiers.py')})
    (ROOT/'build/diagnostics').mkdir(parents=True, exist_ok=True)
    with tempfile.TemporaryDirectory(prefix='pause-native-audio-',
                                     dir=ROOT/'build/diagnostics') as tmp:
        stage = Path(tmp)
        for name, data in inputs.items():
            path = stage/name
            path.parent.mkdir(parents=True, exist_ok=True)
            path.write_bytes(data)
        src, obj = stage/'AsciiManagerMenu.cpp', stage/'Pause.obj'
        src.write_text(source)
        run = subprocess.run(['nice', '-n', '10', str(ROOT/'scripts/compile-probe.sh'),
                              str(src), str(obj), *profile], cwd=ROOT,
                             capture_output=True, text=True)
        result.update(compile_exit=run.returncode, compile_log=run.stdout+run.stderr)
        if run.returncode:
            raise RuntimeError(result['compile_log'])
        raw, rows = coff.object_function(obj, inspect.SYMBOL, include_symbol_locations=True)
        result.update(raw_hex=raw.hex(), raw_sha256=digest(raw), coff=rows)
        native_fields = []
        for name in CASES[args.case]:
            unit = units[name]
            if ((unit['target_address'], unit['size']) != NATIVE[name]
                    or unit['profile'] != profile):
                raise ValueError('native original contract/profile changed: '+name)
            body, fields = coff.object_function(obj, unit['symbol'],
                                                include_symbol_locations=True)
            target = coff.pe_bytes_at(image, unit['target_address'], unit['size'])
            proof = compare_native(body, fields, unit, target)
            destinations = {row['symbol']: row['target']
                            for row in unit.get('relocations', [])}
            proof['literal_definitions'] = check_literal_definitions(
                obj.read_bytes(), fields, image, coff, destinations)
            proof.update(raw_hex=body.hex(), coff=fields)
            result['native'][name] = proof
            native_fields.extend(fields)
        for name in CASES[args.case]:
            if name in ('soundplayer-resume-after-pause', 'supervisor-stop-audio'):
                references, count = rows, 1
            else:
                references = native_fields
                count = 3 if name == 'soundplayer-queue-command' else 1
            result['native'][name]['definition_references'] = definition_references(
                references, units[name]['symbol'], count)
        result['caller'] = inspect.report(obj)
        # A valid native definition cannot waive any complete-caller requirement.
        try:
            frontier.guard_pause_menu(result['caller'])
            result['caller_frontier_passed'] = True
        except RuntimeError as error:
            result.update(caller_frontier_passed=False, caller_frontier_error=str(error))
    for name, data in inputs.items():
        if (ROOT/'src'/name).read_bytes() != data:
            raise ValueError('source changed during probe: '+name)
    emit(result, output)
    if not result['caller_frontier_passed']:
        raise RuntimeError(result['caller_frontier_error'])


if __name__ == '__main__':
    main()
