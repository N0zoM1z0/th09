#!/usr/bin/env python3
"""Probe complete Title owner/callee contexts without granting exactness credit."""
from pathlib import Path
import argparse
import hashlib
import importlib.util
import json
import os
import re
import struct
import subprocess
import sys
import tomllib

ROOT = Path(__file__).resolve().parents[1]
CASES = ('baseline', 'full-declaration', 'full-native-main',
         'full-native-family', 'full-native-ordinal')
BRIDGE = '''
extern const char *g_TitleOptionsHelpText[9];
typedef char OptionsWholeOwnerSize[sizeof(TitleScreenView) == 0x1B388 ? 1 : -1];
typedef char OptionsWholeInputPrefix[sizeof(InputView) == 8 ? 1 : -1];
typedef char OptionsWholeSnapshotSize[sizeof(TitleConfigSnapshotView) == 0xCC ? 1 : -1];
typedef char OptionsWholeSnapshotAD[offsetof(TitleConfigSnapshotView, valueAD) == 0xAD ? 1 : -1];
typedef char OptionsWholeSnapshotWindow[offsetof(TitleConfigSnapshotView, windowed) == 0xB1 ? 1 : -1];
'''
EXTRA = '''#include "AnmManager.hpp"
#include "ReplayInputState.hpp"
typedef signed char i8;
'''


def replace_once(source, old, new):
    if source.count(old) != 1:
        raise ValueError('Title source anchor changed: ' + old[:80])
    return source.replace(old, new, 1)


def render(inputs, case):
    if case not in CASES:
        raise ValueError('unknown Title owner context')
    full = inputs['TitleScreen.cpp'].decode().replace('\r\n', '\n')
    options = inputs['TitleScreenOptions.cpp'].decode().replace('\r\n', '\n')
    leaf = inputs['TitleScreenChangeLeaf.cpp'].decode().replace('\r\n', '\n')
    if case == 'baseline':
        return options
    body = options[options.index('int TitleScreenView::OnUpdateOptions()'):]
    body = body.replace('optionsFrameCounter', 'screenFrameCounter')
    body = body.replace('titleAnm->SetSprite(',
                        'reinterpret_cast<AnmLoaded *>(titleAnm)->SetSprite(')
    start = options.index('struct TitleConfigSnapshotView {')
    end = options.index('\n};', start) + 3
    full = replace_once(full, 'struct TitleConfigSnapshotView {\n    u32 words[0x33];\n};',
                        options[start:end])
    full = full.replace('unknown0C918', 'optionsUiReady')
    full = replace_once(full,
        'struct InputView {\n    u16 currentInput;\n    u16 IsPressedScrolling(i32 mask);\n};',
        'struct InputView {\n    u16 currentInput;\n    u16 previous02;\n'
        '    u16 repeatOutput04;\n    u16 frontState06;\n'
        '    u16 IsPressedScrolling(i32 mask);\n};')
    if case == 'full-declaration':
        end = full.index('\n};', full.index('struct TitleScreenView {')) + 3
        return EXTRA + full[:end] + BRIDGE + '\n' + body
    if case == 'full-native-main':
        return EXTRA + full + '\n' + BRIDGE + '\n' + body
    screen = leaf[leaf.index('int TitleChangeScreenView::ChangeCurrentScreen'):]
    screen = screen.replace('TitleChangeScreenView::', 'TitleScreenView::')
    screen = screen.replace('phaseTimer', 'stateTimer2')
    result = EXTRA + full + '\n' + BRIDGE + '\n' + screen + '\n' + body
    if case == 'full-native-ordinal':
        start = result.index('        if (g_ReplayInputStates[2].word06 & 0x1001)')
        end = result.index('        if (g_ReplayInputStates[2].word06 & 0xA)', start)
        block = replace_once(result[start:end], 'switch (keyboardSelection)',
                             'switch (static_cast<u32>(keyboardSelection) - 6u)')
        for old, new in ((6, 0), (7, 1), (8, 2)):
            block = replace_once(block, 'case %d:' % old, 'case %d:' % new)
        result = result[:start] + block + result[end:]
    return result


def load(name, filename):
    spec = importlib.util.spec_from_file_location(name, ROOT / 'scripts' / filename)
    module = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(module)
    return module


def check_screen_body(obj, units, coff, image):
    unit = units['title-change-current-screen-leaf']
    symbol = '?ChangeCurrentScreen@TitleScreenView@@QAEHH@Z'
    body, fields = coff.object_function(obj, symbol, include_symbol_locations=True)
    if (unit['target_address'] != 0x422F39 or unit['size'] != 46 or
            len(body) != 46 or fields or
            body != coff.pe_bytes_at(image, unit['target_address'], 46)):
        raise ValueError('actual ChangeCurrentScreen body differs from original')
    _, owner = coff.object_function(obj, '?OnUpdateOptions@TitleScreenView@@QAEHXZ',
                                    include_symbol_locations=True)
    references = [row for row in owner if row['symbol'] == symbol]
    if len(references) != 2:
        raise ValueError('native screen call inventory differs')
    data = obj.read_bytes()
    for row in references:
        if (row['symbol_section'] <= 0 or row['type'] != 'REL32' or row['addend'] or
                row['symbol_value'] != 0):
            raise ValueError('screen call lacks its actual same-object definition')
        extent, pointer = struct.unpack_from('<II', data,
            20 + (row['symbol_section'] - 1) * 40 + 16)
        if extent < 46 or data[pointer:pointer + 46] != body:
            raise ValueError('screen call COFF coordinates disagree with its body')
    return dict(bytes=46, fields=0, raw_hex=body.hex(),
                complete_original_equal=True, actual_caller_references=references)


def check_called_definitions(obj, owner_fields, units, coff, image, literals):
    """Prove actual positive call definitions before using their target bindings."""
    data = obj.read_bytes()
    known = {unit['symbol']: unit for unit in units.values()}
    screen = '?ChangeCurrentScreen@TitleScreenView@@QAEHH@Z'
    known[screen] = dict(units['title-change-current-screen-leaf'], symbol=screen)
    checked = {}
    for row in owner_fields:
        if row['type'] != 'REL32' or row['symbol_section'] <= 0:
            continue
        symbol = row['symbol']
        if symbol not in known or row['addend']:
            raise ValueError('unbound actual Title call definition: ' + symbol)
        unit = known[symbol]
        body, fields = coff.object_function(obj, symbol, include_symbol_locations=True)
        expected = sorted(unit.get('relocations', []), key=lambda x: x['offset'])
        size = unit.get('compare_size', unit['size'])
        if len(body) != size or len(fields) != len(expected):
            raise ValueError('actual Title callee extent/fields differ: ' + symbol)
        coff.validate_owner_local_destinations(fields, expected, unit['target_address'], size)
        linked = bytearray(body)
        for actual, wanted in zip(sorted(fields, key=lambda x: x['offset']), expected):
            if (actual['offset'], actual['type']) != (wanted['offset'], wanted['type']):
                raise ValueError('actual Title callee fields differ: ' + symbol)
            if actual['symbol'] != wanted['symbol']:
                local = actual['symbol_value'] - actual['owner_value']
                if not (actual['symbol'].startswith('$L') and wanted['symbol'].startswith('$L')
                        and actual['symbol_section'] == actual['owner_section']
                        and actual['symbol_section'] > 0 and 0 <= local < size
                        and unit['target_address'] + local == wanted['target']):
                    raise ValueError('actual Title callee identity differs: ' + symbol)
            value = wanted['target'] + actual['addend']
            if actual['type'] == 'REL32':
                value -= unit['target_address'] + actual['offset'] + 4
            elif actual['type'] != 'DIR32':
                raise ValueError('unsupported Title callee relocation')
            struct.pack_into('<I', linked, actual['offset'], value & 0xffffffff)
        if linked != coff.pe_bytes_at(image, unit['target_address'], size):
            raise ValueError('actual Title callee bytes differ: ' + symbol)
        literals.check_literal_definitions(data, fields, image, coff,
            {actual['symbol']: wanted['target'] for actual, wanted in
             zip(sorted(fields, key=lambda x:x['offset']), expected)})
        extent, pointer = struct.unpack_from('<II', data,
            20 + (row['symbol_section'] - 1) * 40 + 16)
        start = row['symbol_value']
        if start < 0 or start + size > extent or data[pointer+start:pointer+start+size] != body:
            raise ValueError('actual Title call definition coordinates differ')
        checked.setdefault(symbol, dict(bytes=size, target=unit['target_address'],
                                        complete_original_equal=True, calls=[]))['calls'].append(row)
    return checked


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--case', choices=CASES, required=True)
    parser.add_argument('--output', type=Path, required=True)
    args = parser.parse_args()
    output = args.output.resolve()
    try:
        output.relative_to(ROOT / '.analysis')
        if output == ROOT / '.analysis' or output.exists():
            raise ValueError('use a fresh directory below .analysis')
        sys.path.insert(0, str(ROOT / 'scripts'))
        coff = load('title_owner_coff', 'compare-coff-function.py')
        audit = load('title_owner_options', 'inspect-title-options.py')
        guard = load('title_owner_guard', 'check-boss-frontiers.py')
        literals = load('title_owner_literals', 'coff_literal_definitions.py')
        image = coff.verified_target()
        sha = lambda data: hashlib.sha256(data).hexdigest()
        provider_files = ('scripts/compile-probe.sh', 'scripts/compare-coff-function.py',
            'scripts/inspect-title-options.py', 'scripts/check-boss-frontiers.py',
            'scripts/coff_literal_definitions.py', 'scripts/diagnostic_branch_alignment.py',
            'scripts/probe-title-options-owner-context.py', 'config/match-units.toml',
            'config/tools.lock.toml', 'config/target.toml')
        providers = {name: sha((ROOT / name).read_bytes()) for name in provider_files}
        units = tomllib.loads((ROOT / 'config/match-units.toml').read_text())['units']
        sound = units['title-screen-play-menu-sound']
        profile = sound['profile']
        if profile != units['title-screen-setup-thread']['profile']:
            raise ValueError('Title source profiles differ')
        sources = {}

        def collect(name):
            if name in sources:
                return
            data = (ROOT / 'src' / name).read_bytes()
            sources[name] = data
            for include in re.findall(rb'^\s*#\s*include\s*"([^"\r\n]+)"', data, re.M):
                collect((Path(name).parent / include.decode()).as_posix())

        for name in ('TitleScreen.cpp', 'TitleScreenOptions.cpp', 'TitleScreenChangeLeaf.cpp'):
            collect(name)
        source = render(sources, args.case).encode()
        output.mkdir(parents=True)
        for name, data in sources.items():
            path = output / name
            path.parent.mkdir(parents=True, exist_ok=True)
            path.write_bytes(data)
        (output / 'candidate.cpp').write_bytes(source)
        obj = output / 'unit.obj'
        if hasattr(os, 'sched_setaffinity'):
            os.sched_setaffinity(0, {min(os.sched_getaffinity(0))})
        command = ['nice', '-n', '10', str(ROOT / 'scripts/compile-probe.sh'),
                   str(output / 'candidate.cpp'), str(obj), *profile]
        run = subprocess.run(command, cwd=ROOT, capture_output=True, text=True)
        (output / 'compile.log').write_text(run.stdout + run.stderr)
        receipt = dict(case=args.case, command=command, compile_exit=run.returncode,
                       source_sha256=sha(source), target_sha256=sha(image),
                       source_inputs={name: sha(data) for name, data in sources.items()},
                       provider_inputs=providers, profile=profile,
                       source_commit=subprocess.check_output(
                           ['git', 'rev-parse', 'HEAD'], cwd=ROOT, text=True).strip(),
                       diagnostic_only=True, exactness_credit='none')
        (output / 'compile.json').write_text(json.dumps(receipt, indent=2) + '\n')
        if run.returncode:
            raise ValueError('compiler rejected Title owner context; see compile.log')
        raw, fields = coff.object_function(obj, audit.SYMBOL, include_symbol_locations=True)
        result = dict(raw_hex=raw.hex(), raw_sha256=sha(raw), coff=fields,
                      physical_bytes=len(raw), fields=len(fields))
        (output / 'result.json').write_text(json.dumps(result, indent=2) + '\n')
        code, refs = coff.object_function(obj, sound['symbol'], include_symbol_locations=True)
        expected = sound['relocations']
        if len(code) != 34 or len(refs) != 3:
            raise ValueError('actual menu-sound body extent/fields differ')
        linked = bytearray(code)
        for actual, wanted in zip(refs, expected):
            if any(actual[k] != wanted[k] for k in ('offset', 'type', 'symbol')) or actual['addend']:
                raise ValueError('actual menu-sound binding differs')
            value = wanted['target']
            if actual['type'] == 'REL32':
                value -= sound['target_address'] + actual['offset'] + 4
            struct.pack_into('<I', linked, actual['offset'], value & 0xffffffff)
        if linked != coff.pe_bytes_at(image, sound['target_address'], len(linked)):
            raise ValueError('actual menu-sound body differs')
        literals.check_literal_definitions(obj.read_bytes(), refs, image, coff,
            {row['symbol']: row['target'] for row in expected})
        result['menu_sound_original_equal'] = True
        if args.case in ('full-native-family', 'full-native-ordinal'):
            result['native_screen'] = check_screen_body(obj, units, coff, image)
        result['actual_called_definitions'] = check_called_definitions(
            obj, fields, units, coff, image, literals)
        try:
            report = audit.diagnose(obj, require_native_input=True)
            result['diagnostic'] = report
            try:
                guard.guard_title_options(report)
                result['frontier_pass'] = True
            except RuntimeError as error:
                result.update(frontier_pass=False, frontier_rejection=str(error))
        except (ValueError, KeyError) as error:
            result['diagnostic_rejection'] = str(error)
        for name, data in sources.items():
            if (ROOT / 'src' / name).read_bytes() != data:
                raise ValueError('Title source changed during compilation')
        for name, expected in providers.items():
            if sha((ROOT / name).read_bytes()) != expected:
                raise ValueError('Title provider changed during compilation')
        (output / 'result.json').write_text(json.dumps(result, indent=2) + '\n')
        print(json.dumps(dict(case=args.case, candidate_bytes=len(raw), fields=len(fields),
            overlap_differences=result.get('diagnostic', {}).get('overlap_differences'),
            frontier_pass=result.get('frontier_pass'), exactness_credit='none')))
    except (OSError, ValueError, KeyError) as error:
        print('Title owner probe rejected: ' + str(error), file=sys.stderr)
        return 2
    return 0


if __name__ == '__main__':
    raise SystemExit(main())
