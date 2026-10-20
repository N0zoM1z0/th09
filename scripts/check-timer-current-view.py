#!/usr/bin/env python3
"""Verify the native Timer conversion alias without counting shared code twice."""
from __future__ import annotations

import csv
import argparse
import hashlib
import importlib.util
import json
from pathlib import Path
import re
import subprocess
import sys
import tomllib

ROOT = Path(__file__).resolve().parents[1]
ALIAS = 'zun-timer-current-value-alias'
CANONICAL = 'text-helper-get-height'
ADDRESS = 0x435F00
BODY_SHA256 = '916ae10aa3fd76a4d5b25b2fb66e9df0fd04957c7ab10930268d1ed7808be689'


def validate_contract(units, function, matches):
    reference = units['zun-timer-set-current']
    alias = units[ALIAS]
    expected = dict(source='src/ZunTimer.cpp', object='build/matching/ZunTimer.obj',
                    profile=reference['profile'], functions=['ZunTimer::operator int'],
                    symbol='??BZunTimer@@QAEHXZ', target_address=ADDRESS, size=4,
                    relocations=[])
    if any(alias.get(key) != value for key, value in expected.items()):
        raise ValueError('Timer current alias source/profile/ABI/extent changed')
    canonical = units[CANONICAL]
    if (canonical['source'] != 'src/TextHelper.cpp'
            or canonical['object'] != 'build/matching/TextHelper.obj'
            or canonical['functions'] != ['TextHelper::GetHeight']
            or canonical['symbol'] != '?GetHeight@TextHelper@@QAEHXZ'
            or canonical['target_address'] != ADDRESS or canonical['size'] != 4
            or canonical.get('relocations', []) != []):
        raise ValueError('existing TextHelper getter identity changed')
    credited = [row for row in matches if int(row['address'], 0) == ADDRESS]
    if (len(credited) != 1 or credited[0]['unit'] != CANONICAL
            or any(row['unit'] == ALIAS for row in matches)
            or int(function['address'], 0) != ADDRESS or function['size'] != '4'
            or function['proposed_name'] != 'TextHelper::GetHeight'
            or function['module'] != 'TextHelper'
            or function['source_file'] != 'src/TextHelper.cpp'
            or function['owner'] != 'authored' or function['status'] != 'matching'
            or function['match_percent'] != '100.00'):
        raise ValueError('shared Timer alias changed existing physical ownership/exact credit')


def rows(name):
    with (ROOT/'config'/name).open(newline='') as stream:
        return list(csv.DictReader(stream))


def run():
    spec = importlib.util.spec_from_file_location('timer_current_coff',
                                                 ROOT/'scripts/compare-coff-function.py')
    coff = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(coff)
    image = coff.verified_target()
    original = coff.pe_bytes_at(image, ADDRESS, 4)
    if hashlib.sha256(original).hexdigest() != BODY_SHA256:
        raise ValueError('original shared getter body changed')
    units = tomllib.loads((ROOT/'config/match-units.toml').read_text())['units']
    matches = rows('matches.csv')
    function = next(row for row in rows('functions.csv') if int(row['address'], 0) == ADDRESS)
    validate_contract(units, function, matches)
    siblings = [row['unit'] for row in matches
                if units[row['unit']]['source'] == 'src/ZunTimer.cpp']
    if len(siblings) != 16 or any(units[name]['profile'] != units[ALIAS]['profile']
                                 or units[name]['object'] != units[ALIAS]['object']
                                 for name in siblings):
        raise ValueError('Timer source/profile cohort changed')
    pins = {}

    def visit(path):
        name = path.relative_to(ROOT).as_posix()
        if name in pins:
            return
        data = path.read_bytes()
        pins[name] = hashlib.sha256(data).hexdigest()
        for include in re.findall(rb'^\s*#\s*include\s*"([^"]+)"', data, re.M):
            visit(path.parent/include.decode())

    visit(ROOT/'src/ZunTimer.cpp')
    for name in ('config/match-units.toml', 'config/matches.csv', 'config/functions.csv',
                 'config/tools.lock.toml', 'config/target.toml', 'scripts/build-match-unit.py',
                 'scripts/compile-probe.sh', 'scripts/compare-coff-function.py',
                 'scripts/check-timer-current-view.py'):
        visit(ROOT/name)
    subprocess.run(['nice', '-n', '10', sys.executable, '-B',
                    'scripts/build-match-unit.py', '--unit', ALIAS], cwd=ROOT,
                   check=True, stdout=sys.stderr)
    proofs = [coff.compare_unit(name) for name in [*siblings, ALIAS]]
    if any(p['result'] != 'exact' for p in proofs):
        raise ValueError('Timer source lost complete original byte/field equality')
    body, fields = coff.object_function(ROOT/units[ALIAS]['object'], units[ALIAS]['symbol'])
    if fields or body != original:
        raise ValueError('Timer current alias body/fields changed')
    if any(hashlib.sha256((ROOT/name).read_bytes()).hexdigest() != digest
           for name, digest in pins.items()):
        raise ValueError('Timer current source/profile inputs changed during replay')
    return dict(alias_unit=ALIAS, canonical_physical_unit=CANONICAL,
                canonical_name='TextHelper::GetHeight', physical_bytes=4,
                complete_original_equal=True, authored_exact_delta=0,
                alias_only=True, exactness_credit='already counted by the canonical TextHelper row',
                checked=proofs, source_inputs_sha256=pins)


if __name__ == '__main__':
    argparse.ArgumentParser(description=__doc__).parse_args()
    try:
        print(json.dumps(run(), indent=2))
    except (OSError, ValueError, KeyError, ImportError, subprocess.CalledProcessError) as exc:
        print('Timer current alias replay rejected: '+str(exc), file=sys.stderr)
        raise SystemExit(2)
