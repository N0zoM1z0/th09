#!/usr/bin/env python3
"""Cold-compare actual Timer setter contexts without changing maintained source."""
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
CASES = ('baseline', 'setter-before', 'pair-before', 'pair-after')
CALLEES = ('zun-timer-set-current', 'zun-timer-assign-int')


def load(name):
    spec = importlib.util.spec_from_file_location(name.replace('-', '_'), ROOT/'scripts'/name)
    module = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(module)
    return module


def digest(data):
    return hashlib.sha256(data).hexdigest()


def definition(source, signature):
    start = source.index(signature + '\n{')
    end = source.index('{', start) + 1
    depth = 1
    while depth:
        depth += (source[end] == '{') - (source[end] == '}')
        end += 1
    return source[start:end] + '\n\n'


def capture_inputs():
    sources = {}

    def visit(name):
        if name in sources:
            return
        data = (ROOT/'src'/name).read_bytes()
        sources[name] = data
        for child in re.findall(rb'^\s*#\s*include\s*"([^"]+)"', data, re.M):
            visit((Path(name).parent/child.decode()).as_posix())

    visit('EclManager.cpp')
    pins = {'src/'+name: digest(data) for name, data in sources.items()}
    for name in ('src/ZunTimer.cpp', 'config/match-units.toml', 'config/target.toml',
                 'config/functions.csv',
                 'config/tools.lock.toml', 'scripts/compile-probe.sh',
                 'scripts/compare-coff-function.py', 'scripts/probe-ecl-variant.py',
                 'scripts/inspect-ecl-complete.py', 'scripts/audit-ecl-callsite-identities.py',
                 'scripts/report-ecl-handler-boundaries.py', 'scripts/ecl_shape_differences.py',
                 'scripts/report-ecl-handler-shapes.py', 'scripts/inspect-ecl-call-inventory.py',
                 'scripts/report-ecl-opcodes.py', 'scripts/inspect-enemy-manager-update.py',
                 'scripts/diagnostic_switch_graph.py', 'scripts/diagnostic_byte_spans.py',
                 'scripts/diagnostic_branch_alignment.py', 'scripts/probe-runecl-setter-context.py'):
        pins[name] = digest((ROOT/name).read_bytes())
    return sources, pins


def require_unchanged(pins):
    if any(digest((ROOT/name).read_bytes()) != value for name, value in pins.items()):
        raise ValueError('Timer context source/profile/provider inputs changed during replay')


def require_staged(root, sources):
    if any((root/name).read_bytes() != data for name, data in sources.items()):
        raise ValueError('actual staged Timer context sources changed during replay')


def require_native_contract(units, profile):
    expected = {
        'zun-timer-set-current': ('?SetCurrent@ZunTimer@@QAEXH@Z', 0x4014D0, 25, []),
        'zun-timer-assign-int': ('??4ZunTimer@@QAEXH@Z', 0x401500, 9, [
            dict(offset=5, type='REL32', symbol='?SetCurrent@ZunTimer@@QAEXH@Z',
                 target=0x4014D0)]),
    }
    for name, (symbol, address, size, fields) in expected.items():
        unit = units[name]
        if (unit['source'] != 'src/ZunTimer.cpp'
                or unit['object'] != 'build/matching/ZunTimer.obj'
                or unit['symbol'] != symbol or unit['target_address'] != address
                or unit['size'] != size or unit.get('relocations', []) != fields
                or unit['profile'] != profile):
            raise ValueError('real Timer source/profile/ABI/extent contract changed')


def compare_callees(coff, image, obj, units, case):
    result = []
    for name in CALLEES[:0 if case == 'baseline' else 1 if case == 'setter-before' else 2]:
        unit = units[name]
        raw, fields = coff.object_function(obj, unit['symbol'], include_symbol_locations=True)
        expected = unit.get('relocations', [])
        if len(raw) != unit['size'] or len(fields) != len(expected):
            raise ValueError('actual Timer callee extent/field inventory changed')
        linked = bytearray(raw)
        for row, target in zip(fields, expected):
            if (any(row[key] != target[key] for key in ('offset', 'symbol', 'type'))
                    or row['addend'] != target.get('addend', 0)
                    or row['symbol_section'] <= 0):
                raise ValueError('actual Timer wrapper lost its same-object setter definition')
            value = target['target'] + row['addend']
            if row['type'] == 'REL32':
                value -= unit['target_address'] + row['offset'] + 4
            elif row['type'] != 'DIR32':
                raise ValueError('unreviewed Timer callee relocation kind')
            struct.pack_into('<I', linked, row['offset'], value & 0xFFFFFFFF)
        if linked != coff.pe_bytes_at(image, unit['target_address'], unit['size']):
            raise ValueError('actual Timer callee complete bytes differ from original')
        result.append(dict(unit=name, target_address=unit['target_address'],
                           complete_original_equal=True, raw_hex=raw.hex(), coff=fields))
    return result


def run(case, output):
    coff = load('compare-coff-function.py')
    image = coff.verified_target()  # Wrong targets must preserve earlier evidence.
    if case not in CASES:
        raise ValueError('unreviewed Timer source context')
    output = output.absolute()
    if (output.is_symlink() or not output.resolve().is_relative_to(ROOT/'.analysis')
            or output.suffix != '.json'):
        raise ValueError('output must be a JSON file below this repository .analysis')
    sources, pins = capture_inputs()
    probe = load('probe-ecl-variant.py')
    probe.attested_include_chain(sources)
    profile = probe.pinned_profile()
    manifest = tomllib.loads((ROOT/'config/match-units.toml').read_text())
    if manifest.get('schema_version') != 1 or manifest.get('target_sha256') != digest(image):
        raise ValueError('Timer context unit manifest is not bound to the original target')
    units = manifest['units']
    require_native_contract(units, profile)
    timer = (ROOT/'src/ZunTimer.cpp').read_text()
    setter = definition(timer, 'void ZunTimer::SetCurrent(int value)')
    wrapper = definition(timer, 'void ZunTimer::operator=(int value)')
    source = sources['EclManager.cpp'].decode()
    marker = 'int EclManager::RunEcl(EnemyView *enemy)'
    if source.count(marker) != 1:
        raise ValueError('RunEcl actual source boundary changed')
    if case in ('setter-before', 'pair-before'):
        source = source.replace(marker, setter + (wrapper if case == 'pair-before' else '') + marker, 1)
    elif case == 'pair-after':
        source += '\n' + setter + wrapper
    staged = {**sources, 'EclManager.cpp': source.encode()}
    probe.attested_include_chain(staged)
    output.parent.mkdir(parents=True, exist_ok=True)
    output.unlink(missing_ok=True)
    with tempfile.TemporaryDirectory(prefix='timer-context-', dir=output.parent) as temporary:
        root = Path(temporary)
        for name, data in staged.items():
            path = root/name
            path.parent.mkdir(parents=True, exist_ok=True)
            path.write_bytes(data)
        require_staged(root, staged)
        obj = root/'EclManager.obj'
        compiled = subprocess.run(['nice', '-n', '10', str(ROOT/'scripts/compile-probe.sh'),
                                   str(root/'EclManager.cpp'), str(obj), *profile],
                                  cwd=ROOT, capture_output=True, text=True)
        if compiled.returncode:
            raise ValueError('Timer context compiler failed: ' + (compiled.stdout+compiled.stderr)[-1500:])
        require_unchanged(pins)
        require_staged(root, staged)
        result = dict(case=case, diagnostic_only=True, exactness_credit='none',
                      profile=profile, target_sha256=digest(image),
                      staged_source_sha256={name: digest(data) for name, data in staged.items()},
                      source_inputs_sha256=pins,
                      native_callees=compare_callees(coff, image, obj, units, case))
        for key, script, symbol, function in (
                ('runecl', 'inspect-ecl-complete.py', '?RunEcl@EnemyEclManagerView@@QAEHPAUEnemyView@@@Z', 'report'),
                ('enemy', 'inspect-enemy-manager-update.py', '?OnUpdate@EnemyManagerView@@SIHPAU1@@Z', 'diagnose')):
            raw, fields = coff.object_function(obj, symbol, include_symbol_locations=True)
            result[key+'_physical'] = dict(bytes=len(raw), fields=len(fields), raw_hex=raw.hex(), coff=fields)
            try:
                result[key] = getattr(load(script), function)(obj)
            except (ValueError, KeyError) as error:
                result[key+'_rejected'] = str(error)
        require_unchanged(pins)
        require_staged(root, staged)
        output.write_text(json.dumps(result, indent=2)+'\n')
    return result


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--case', choices=CASES, default='baseline')
    parser.add_argument('--output', type=Path, default=ROOT/'.analysis/runecl-setter-context/latest.json')
    args = parser.parse_args()
    try:
        result = run(args.case, args.output)
    except (OSError, ValueError, KeyError, RuntimeError, ImportError) as error:
        parser.exit(2, 'Timer context replay failed: '+str(error)+'\n')
    print(json.dumps(dict(case=args.case, output=str(args.output), diagnostic_only=True,
                          exactness_credit='none', runecl_bytes=result['runecl_physical']['bytes'],
                          runecl_differences=result.get('runecl', {}).get('different_bytes'),
                          runecl_rejected=result.get('runecl_rejected'))))
