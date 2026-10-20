#!/usr/bin/env python3
"""Replay two logical aliases of one shared identity body, with no origin credit."""
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
ADDRESS = 0x4343D0
BODY_SHA256 = '58367ffa2a0179375018fa0f5c26da24391e42ebe0ed8fdda35a21fc7bdc396f'
SOURCE = 'src/VectorPointerViews.cpp'
ALIASES = {
    'float3-pointer-identity-view': ('Float3::operator float*', '??BFloat3@@QAEPAMXZ'),
    'player-position-pointer-identity-view': (
        'PlayerPositionView::operator float*', '??BPlayerPositionView@@QAEPAMXZ'),
}


def validate_contract(units, function, origin, matches):
    reference = units['ex-attack-initialize-type3']
    if (reference['target_address'] != 0x442580 or reference['size'] != 454 or
            reference['source'] != 'src/ExAttackInitType3.cpp'):
        raise ValueError('Type3 caller compiler reference changed')
    for name, (label, symbol) in ALIASES.items():
        unit = units[name]
        expected = dict(source=SOURCE, object='build/matching/VectorPointerViews.obj',
                        profile=reference['profile'], functions=[label], symbol=symbol,
                        target_address=ADDRESS, size=3, relocations=[])
        if any(unit.get(key) != value for key, value in expected.items()):
            raise ValueError('shared identity alias source/profile/ABI/extent changed: ' + name)
    if (int(function['address'], 0) != ADDRESS or function['size'] != '3' or
            function['span_end'] != '0x004343D2' or function['source_file'] != SOURCE or
            function['status'] != 'unclassified' or function['match_percent'] != '0.00' or
            any(function[key] for key in ('proposed_name', 'module', 'owner')) or
            int(origin['address'], 0) != ADDRESS or origin['origin'] != 'unknown' or
            origin['disposition'] != 'review' or
            any(int(row['address'], 0) == ADDRESS for row in matches)):
        raise ValueError('shared identity bytes were confused with unique origin/exact credit')


def read_rows(filename):
    with (ROOT / 'config' / filename).open(newline='') as stream:
        return list(csv.DictReader(stream))


def run():
    spec = importlib.util.spec_from_file_location('pointer_identity_coff',
                                                 ROOT / 'scripts/compare-coff-function.py')
    coff = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(coff)
    image = coff.verified_target()
    target = coff.pe_bytes_at(image, ADDRESS, 3)
    if hashlib.sha256(target).hexdigest() != BODY_SHA256:
        raise ValueError('original shared identity full-body SHA changed')
    units = tomllib.loads((ROOT / 'config/match-units.toml').read_text())['units']
    function = next(row for row in read_rows('functions.csv')
                    if int(row['address'], 0) == ADDRESS)
    origin = next(row for row in read_rows('function-origins.csv')
                  if int(row['address'], 0) == ADDRESS)
    validate_contract(units, function, origin, read_rows('matches.csv'))
    inputs = {}

    def collect(path):
        name = str(path.relative_to(ROOT))
        if name in inputs:
            return
        data = path.read_bytes()
        inputs[name] = hashlib.sha256(data).hexdigest()
        if name.startswith('src/'):
            for include in re.findall(rb'^\s*#\s*include\s*"([^"]+)"', data, re.M):
                collect(path.parent / include.decode())

    collect(ROOT / SOURCE)
    for name in ('config/match-units.toml', 'config/functions.csv',
                 'config/function-origins.csv', 'config/matches.csv',
                 'config/target.toml', 'config/tools.lock.toml',
                 'scripts/build-match-unit.py', 'scripts/compile-probe.sh',
                 'scripts/compare-coff-function.py', 'scripts/check-vector-pointer-views.py'):
        collect(ROOT / name)
    subprocess.run(['nice', '-n', '10', sys.executable, '-B',
                    str(ROOT / 'scripts/build-match-unit.py'), '--unit', next(iter(ALIASES))],
                   cwd=ROOT, check=True, stdout=sys.stderr)
    proofs = []
    for name in ALIASES:
        proof = coff.compare_unit(name)
        raw, fields = coff.object_function(ROOT / units[name]['object'], units[name]['symbol'])
        if (proof['result'] != 'exact' or proof['compared_size'] != 3 or
                proof['matched_compared_bytes'] != 3 or fields or raw != target):
            raise ValueError('complete shared identity alias replay failed: ' + name)
        proofs.append(dict(unit=name, symbol=units[name]['symbol'], bytes=len(raw), fields=0,
                           complete_original_equal=True, raw_sha256=hashlib.sha256(raw).hexdigest()))
    if any(hashlib.sha256((ROOT / name).read_bytes()).hexdigest() != digest
           for name, digest in inputs.items()):
        raise ValueError('shared identity source/profile/provider inputs changed during replay')
    return dict(target_sha256=hashlib.sha256(image).hexdigest(), target_address=hex(ADDRESS),
                original_body_sha256=BODY_SHA256, aliases=proofs, physical_bodies=1,
                physical_bytes=3, exactness_credit='none', original_origin='unknown',
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
        print('shared identity replay rejected:', error, file=sys.stderr)
        return 1
    if args.json:
        print(json.dumps(report, indent=2))
    else:
        print('Two logical interfaces, one complete 3-byte physical identity; origin unknown, no credit.')
    return 0


if __name__ == '__main__':
    raise SystemExit(main())
