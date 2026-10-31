#!/usr/bin/env python3
"""Cold-replay exact TH09 units for *explicitly selected* source carriers.

Never scans or builds every source automatically. A requested exact unit expands
into the full same-source canonical exact cohort, including alternate VC7.1
profiles. The caller must separately identify/include changed header or .inl
consumers; this is not a whole-product dependency graph or an exactness oracle
for nonmatching bosses. The canonical comparator independently checks original
PE bytes, physical tails/tables and each manifest relocation.
"""
from __future__ import annotations

import argparse
from collections import defaultdict
import csv
import json
from pathlib import Path
import subprocess
import sys
import tomllib

ROOT = Path(__file__).resolve().parents[1]


def load_carriers(root: Path = ROOT) -> tuple[dict, dict[str, str]]:
    with (root / 'config/match-units.toml').open('rb') as stream:
        manifest = tomllib.load(stream)
    if manifest.get('schema_version') != 1:
        raise ValueError('unreviewed canonical match manifest')
    units = manifest['units']
    with (root / 'config/matches.csv').open(newline='') as stream:
        rows = list(csv.DictReader(stream))
    chosen: dict[str, str] = {}
    for row in rows:
        name = row['unit']
        if (name in chosen or name not in units or row['status'] != 'matching' or
                row['match_percent'] != '100.00' or
                int(row['size'], 0) != units[name]['size'] or
                int(row['address'], 0) != units[name]['target_address']):
            raise ValueError('missing/ambiguous original-bound exact manifest: ' + name)
        chosen[name] = units[name]['source']
    if not chosen:
        raise ValueError('there are no canonical exact units')
    return units, chosen


def plan(units: dict, matched: dict[str, str], sources: list[str], units_requested: list[str]) -> list[dict]:
    selected = set(sources)
    for name in units_requested:
        if name not in matched:
            raise ValueError('requested unit lacks a canonical original-bound exact match: ' + name)
        selected.add(matched[name])
    if not selected:
        raise ValueError('explicit --source or --unit is required; never cold-replay all')
    missing = selected - set(matched.values())
    if missing:
        raise ValueError('selected sources have no canonical exact units: ' + ', '.join(sorted(missing)))
    carriers: dict[tuple[str, str, tuple[str, ...]], list[str]] = defaultdict(list)
    object_profiles: dict[str, tuple[str, tuple[str, ...]]] = {}
    for name, source in matched.items():
        if source not in selected:
            continue
        unit = units[name]
        obj, profile = unit['object'], tuple(unit['profile'])
        if (not source.startswith('src/') or not obj.startswith('build/matching/') or
                not profile or not all(isinstance(x, str) and x for x in profile) or
                unit['source'] != source or 'functions' not in unit):
            raise ValueError('unsafe source/object/profile for exact unit: ' + name)
        identity = (source, profile)
        if obj in object_profiles and object_profiles[obj] != identity:
            raise ValueError('one output object has conflicting source/compiler profiles: ' + obj)
        object_profiles[obj] = identity
        carriers[(source, obj, profile)].append(name)
    return [dict(source=source, object=obj, profile=list(profile), units=sorted(names))
            for (source, obj, profile), names in sorted(carriers.items())]


def command(argv: list[str], *, cwd: Path = ROOT, timeout: int = 300) -> str:
    result = subprocess.run(argv, cwd=cwd, check=True, capture_output=True,
                            text=True, timeout=timeout)
    return result.stdout


def run(groups: list[dict], *, root: Path = ROOT, invoke=command) -> list[dict]:
    invoke([sys.executable, '-B', 'scripts/verify-target.py'], cwd=root)
    invoke([sys.executable, '-B', 'scripts/validate-tracking.py', '--require-target'], cwd=root)
    results = []
    for group in groups:
        names = group['units']
        # The compiler may use static data, inline definitions and optimization
        # scope outside the selected function. Compile the actual whole carrier.
        invoke(['nice', '-n', '10', sys.executable, '-B', 'scripts/build-match-unit.py',
                '--unit', names[0]], cwd=root)
        passed = []
        for unit in names:
            raw = invoke([sys.executable, '-B', 'scripts/compare-coff-function.py',
                          '--unit', unit, '--json'], cwd=root)
            record = json.loads(raw)
            if (record.get('result') != 'exact' or
                    record.get('matched_bytes') != record.get('size') or
                    record.get('matched_compared_bytes') != record.get('compared_size') or
                    record.get('compared_size', -1) < record.get('size', 0)):
                raise ValueError('original-bound exact regression in ' + unit)
            passed.append(dict(unit=unit, physical_bytes=record['compared_size'],
                               relocations=len(record['relocations'])))
        results.append(dict(source=group['source'], object=group['object'],
                            profile=group['profile'], passed=passed))
    return results


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--source', action='append', default=[],
                        help='exact source file; may be repeated')
    parser.add_argument('--unit', action='append', default=[],
                        help='canonical unit; expands to ALL exact same-source profiles')
    parser.add_argument('--dry-run', action='store_true',
                        help='show plan without compiler or comparator calls')
    args = parser.parse_args()
    try:
        units, matches = load_carriers()
        cohorts = plan(units, matches, args.source, args.unit)
        if args.dry_run:
            print(json.dumps(dict(selected_sources=sorted({x['source'] for x in cohorts}),
                                  carriers=cohorts, exact_units=sum(len(x['units']) for x in cohorts),
                                  compiler_jobs=len(cohorts)), indent=2))
            return 0
        result = run(cohorts)
        print(json.dumps(dict(result='passed', compiler_jobs=len(result),
                              exact_units=sum(len(x['passed']) for x in result),
                              physical_bytes=sum(y['physical_bytes'] for x in result for y in x['passed']),
                              relocation_fields=sum(y['relocations'] for x in result for y in x['passed']),
                              carriers=result, scope='selected sources only; no whole-product claim'), indent=2))
        return 0
    except (OSError, ValueError, KeyError, TypeError, subprocess.SubprocessError,
            json.JSONDecodeError, tomllib.TOMLDecodeError) as error:
        print('scoped exact replay rejected: ' + str(error), file=sys.stderr)
        return 1


if __name__ == '__main__':
    raise SystemExit(main())
