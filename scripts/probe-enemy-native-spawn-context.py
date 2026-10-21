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
    'spawn': ('effect-manager-spawn-effect',),
    'spawn-periodic': ('effect-manager-spawn-effect', 'zun-timer-has-ticked-every'),
    'constructor-periodic': ('bullet-spawn-descriptor-constructor',
                             'zun-timer-has-ticked-every'),
    'spawn-constructor-periodic': ('effect-manager-spawn-effect',
                                   'bullet-spawn-descriptor-constructor',
                                   'zun-timer-has-ticked-every'),
}
CASES = tuple(CASE_OWNERS)
NATIVE_OWNERS = {
    'effect-manager-spawn-effect': (
        0x40CC00, 184, '?SpawnEffect@EffectManager@@QAEPAUEffect@@HPBUEffectFloat3@@HI@Z', 4),
    'zun-timer-has-ticked-every': (
        0x404920, 33, '?HasTickedEvery@ZunTimer@@QAEHH@Z', 1),
    'bullet-spawn-descriptor-constructor': (
        0x40D500, 28, '??0BulletSpawnDescriptorCtorView@@QAE@XZ', 1),
}


def digest(data):
    return hashlib.sha256(data).hexdigest()


def native_bodies(case, sources):
    """Expose only selected real definitions, preserving the old two recipes."""
    selected = CASE_OWNERS[case]
    bodies = ''
    if 'zun-timer-has-ticked-every' in selected:
        bodies += sources['ZunTimerHasTickedEvery.cpp'].decode() + '\n'
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

    collect('EclManager.cpp')
    observed = {ROOT/'src'/n: data for n, data in inputs.items()}
    native_sources = ['EffectManager.cpp', 'ZunTimerHasTickedEvery.cpp']
    if 'bullet-spawn-descriptor-constructor' in CASE_OWNERS[case]:
        native_sources.append('BulletSpawnDescriptorCtor.cpp')
    for name in native_sources:
        path = ROOT/'src'/name
        observed[path] = path.read_bytes()
    for name in ('config/tools.lock.toml', 'config/functions.csv',
                 'scripts/compile-probe.sh', 'scripts/compare-coff-function.py',
                 'scripts/inspect-enemy-manager-update.py', 'scripts/inspect-ecl-complete.py',
                 'scripts/check-boss-frontiers.py', 'scripts/probe-enemy-native-spawn-context.py'):
        path = ROOT/name
        observed[path] = path.read_bytes()
    observed[units_path] = units_bytes
    observed[coff.TARGET_MANIFEST] = coff.TARGET_MANIFEST.read_bytes()
    hashes = {p.relative_to(ROOT).as_posix(): digest(b) for p, b in observed.items()}
    source = inputs['EclManager.cpp'].decode().replace('\r\n', '\n')
    bodies = native_bodies(case, {n: observed[ROOT/'src'/n] for n in native_sources})
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
            (root/'EclManager.cpp').write_text(generated)
            staged = {n: digest((root/n).read_bytes()) for n in inputs}
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
                native['caller_definition'] = require_caller_definition(rows, symbol, count)
                if fields and any(r['owner_section'] != native['caller_definition']['section']
                                  or r['owner_value'] != 0 for r in fields):
                    raise ValueError('native body and actual caller refer to different definitions')
                report['native_definitions'][name] = native
            report['enemy'] = enemy.diagnose(obj)
            ecl = module('native_spawn_ecl', 'inspect-ecl-complete.py')
            report['runecl'] = ecl.report(obj)
            frontier = module('native_spawn_frontier', 'check-boss-frontiers.py')
            try:
                report['frontier'] = frontier.guard_enemy_update(report['enemy'])
                report['frontier_guard_passed'] = True
            except RuntimeError as error:
                report.update(frontier_guard_passed=False, frontier_error=str(error))
            if staged != {n: digest((root/n).read_bytes()) for n in inputs}:
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
    print(args.case, report['enemy']['candidate_bytes'], 'bytes;',
          report['enemy']['complete_differences'], 'Enemy differences;',
          report['runecl']['different_bytes'], 'RunEcl differences;',
          'native definitions exact; frontier', report['frontier_guard_passed'],
          '; diagnostic only')
    return 0


if __name__ == '__main__':
    raise SystemExit(main())
