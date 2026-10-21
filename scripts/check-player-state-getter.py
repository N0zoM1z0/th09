#!/usr/bin/env python3
"""Replay the Player state getter alias; shared physical origin stays unknown."""
from __future__ import annotations

import argparse
import csv
import hashlib
import importlib.util
import json
from pathlib import Path
import re
import subprocess
import sys
import tomllib

ROOT = Path(__file__).resolve().parents[1]
ADDRESS = 0x435EC0
BODY_SHA256 = '19d200194d49b34530548575f37a1bb4203ee60f18d42f0084e86b7a8c899faa'
SOURCE = 'src/PlayerGetUpdateState.cpp'
UNIT = 'player-get-update-state-alias'
SYMBOL = '?GetUpdateState@PlayerLifecycleView@@QAEHXZ'


def validate_contract(units, function, origin, matches):
    reference = units['player-set-update-state']
    if (reference['source'] != 'src/PlayerSetUpdateState.cpp'
            or reference['target_address'] != 0x403C20 or reference['size'] != 12):
        raise ValueError('native Player profile reference changed')
    expected = dict(source=SOURCE, object='build/matching/PlayerGetUpdateState.obj',
                    profile=reference['profile'],
                    functions=['PlayerLifecycleView::GetUpdateState'], symbol=SYMBOL,
                    target_address=ADDRESS, size=3, relocations=[])
    if any(units[UNIT].get(k) != v for k, v in expected.items()):
        raise ValueError('Player getter alias source/profile/ABI/extent changed')
    if (int(function['address'], 0) != ADDRESS or function['size'] != '3'
            or function['span_end'] != '0x00435EC2' or function['source_file'] != SOURCE
            or function['status'] != 'unclassified' or function['match_percent'] != '0.00'
            or any(function[k] for k in ('proposed_name', 'module', 'owner'))
            or int(origin['address'], 0) != ADDRESS or origin['origin'] != 'unknown'
            or origin['disposition'] != 'review'
            or any(int(row['address'], 0) == ADDRESS for row in matches)):
        raise ValueError('shared getter bytes were confused with authored origin/exact credit')


def read_rows(name):
    with (ROOT/'config'/name).open(newline='') as stream:
        return list(csv.DictReader(stream))


def run():
    spec = importlib.util.spec_from_file_location('player_getter_coff',
                                                 ROOT/'scripts/compare-coff-function.py')
    coff = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(coff)
    image = coff.verified_target()
    target = coff.pe_bytes_at(image, ADDRESS, 3)
    if hashlib.sha256(target).hexdigest() != BODY_SHA256:
        raise ValueError('original shared getter complete-body SHA changed')
    units = tomllib.loads((ROOT/'config/match-units.toml').read_text())['units']
    function = next(r for r in read_rows('functions.csv') if int(r['address'], 0) == ADDRESS)
    origin = next(r for r in read_rows('function-origins.csv') if int(r['address'], 0) == ADDRESS)
    validate_contract(units, function, origin, read_rows('matches.csv'))
    inputs = {}

    def collect(path):
        name = path.relative_to(ROOT).as_posix()
        if name in inputs:
            return
        data = path.read_bytes()
        inputs[name] = hashlib.sha256(data).hexdigest()
        if name.startswith('src/'):
            for include in re.findall(rb'^\s*#\s*include\s*"([^"]+)"', data, re.M):
                collect(path.parent/include.decode())

    collect(ROOT/SOURCE)
    for name in ('config/match-units.toml', 'config/functions.csv',
                 'config/function-origins.csv', 'config/matches.csv',
                 'config/target.toml', 'config/tools.lock.toml',
                 'scripts/build-match-unit.py', 'scripts/compile-probe.sh',
                 'scripts/compare-coff-function.py', 'scripts/check-player-state-getter.py'):
        collect(ROOT/name)
    subprocess.run(['nice', '-n', '10', sys.executable, '-B',
                    str(ROOT/'scripts/build-match-unit.py'), '--unit', UNIT],
                   cwd=ROOT, check=True, stdout=sys.stderr)
    proof = coff.compare_unit(UNIT)
    raw, fields = coff.object_function(ROOT/units[UNIT]['object'], SYMBOL)
    if (proof['result'] != 'exact' or proof['compared_size'] != 3
            or proof['matched_compared_bytes'] != 3 or fields or raw != target):
        raise ValueError('complete actual Player getter alias replay failed')
    if any(hashlib.sha256((ROOT/n).read_bytes()).hexdigest() != h for n, h in inputs.items()):
        raise ValueError('Player getter source/profile/provider changed during replay')
    return dict(unit=UNIT, target_sha256=hashlib.sha256(image).hexdigest(),
                target_address=hex(ADDRESS), physical_bytes=3, fields=0,
                complete_original_equal=True, body_sha256=BODY_SHA256,
                exactness_credit='none', original_origin='unknown',
                unique_original_class='unknown', whole_native_product_closed=False,
                source_commit=subprocess.check_output(['git', 'rev-parse', 'HEAD'],
                                                      cwd=ROOT, text=True).strip(),
                inputs_sha256=inputs)


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--json', action='store_true')
    args = parser.parse_args()
    try:
        report = run()
    except (OSError, KeyError, TypeError, ValueError, subprocess.CalledProcessError) as error:
        print('Player getter alias rejected:', error, file=sys.stderr)
        return 1
    if args.json:
        print(json.dumps(report, indent=2))
    else:
        print('Player state getter source alias: complete 3/3 bytes; origin unknown, no authored credit.')
    return 0


if __name__ == '__main__':
    raise SystemExit(main())
