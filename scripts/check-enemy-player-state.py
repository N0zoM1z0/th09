#!/usr/bin/env python3
"""Cold-check the accepted units affected by shared Enemy/Player interfaces.

State scope covers 104 units in six source/profile groups; damage scope checks
81 in three groups. Serial nice-10 builds grant no new exact or origin credit.
"""
from __future__ import annotations

import argparse
from collections import Counter, defaultdict
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
COUNTS = {'src/EclManager.cpp': 79, 'src/PlayerRuntime.cpp': 21,
          'src/PlayerSetUpdateState.cpp': 1, 'src/PlayerState3EffectSetup.cpp': 3}
SCOPES = {'state': (COUNTS, 6),
          'damage': ({'src/EclManager.cpp': 79, 'src/PlayerDamage.cpp': 2}, 3)}


def module(name, filename):
    spec = importlib.util.spec_from_file_location(name, ROOT/'scripts'/filename)
    result = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(result)
    return result


def groups(units, matches, scope='state'):
    counts, group_count = SCOPES[scope]
    grouped = defaultdict(list)
    for name in sorted({row['unit'] for row in matches}):
        unit = units[name]
        if unit['source'] in counts:
            grouped[(unit['source'], tuple(unit['profile']))].append(name)
    count = Counter()
    for (source, _), names in grouped.items():
        count[source] += len(names)
    if dict(count) != counts or len(grouped) != group_count:
        raise ValueError('shared Player interface affected-unit inventory changed')
    return sorted(grouped.items())


def inputs(scope='state'):
    hashes = {}
    def visit(path):
        name = str(path.relative_to(ROOT))
        if name in hashes:
            return
        data = path.read_bytes()
        hashes[name] = hashlib.sha256(data).hexdigest()
        for include in re.findall(rb'^\s*#\s*include\s+"([^"]+)"', data, re.M):
            child = path.parent/include.decode()
            if child.is_file():
                visit(child)
    for source in SCOPES[scope][0]:
        visit(ROOT/source)
    for name in ('config/match-units.toml', 'config/matches.csv', 'config/functions.csv',
                 'config/tools.lock.toml', 'config/target.toml', 'scripts/build-match-unit.py',
                 'scripts/compile-probe.sh', 'scripts/compare-coff-function.py',
                 'scripts/check-enemy-player-state.py', 'scripts/inspect-enemy-manager-update.py',
                 'scripts/inspect-ecl-complete.py', 'scripts/audit-ecl-callsite-identities.py',
                 'scripts/check-boss-frontiers.py', 'scripts/diagnostic_switch_graph.py',
                 'scripts/diagnostic_byte_spans.py', 'scripts/diagnostic_branch_alignment.py'):
        visit(ROOT/name)
    return hashes


def share_object(units, names):
    """Reuse one freshly compiled object for identical source/profile units."""
    first = units[names[0]]
    paths = set()
    for name in names:
        unit = units[name]
        if (unit['source'], unit['profile']) != (first['source'], first['profile']):
            raise ValueError('cannot share an object across different source/profile inputs')
        path = ROOT/unit['object']
        if path.is_symlink() or not path.resolve().is_relative_to((ROOT/'build/matching').resolve()):
            raise ValueError('dependent object is outside the canonical build directory')
        paths.add(path)
    source = ROOT/first['object']
    data = source.read_bytes()
    for path in paths - {source}:
        path.parent.mkdir(parents=True, exist_ok=True)
        path.write_bytes(data)


def run(scope='state'):
    coff = module('enemy_state_complete_coff', 'compare-coff-function.py')
    coff.verified_target()
    units = tomllib.loads((ROOT/'config/match-units.toml').read_text())['units']
    with (ROOT/'config/matches.csv').open(newline='') as stream:
        affected = groups(units, list(csv.DictReader(stream)), scope)
    before, checked, frontiers = inputs(scope), [], {}
    for (source, profile), names in affected:
        built = subprocess.run(['nice', '-n', '10', sys.executable, '-B',
                                'scripts/build-match-unit.py', '--unit', names[0]],
                               cwd=ROOT, capture_output=True, text=True)
        if built.returncode:
            raise ValueError('Player state dependent compile failed: '+built.stdout+built.stderr)
        share_object(units, names)
        for name in names:
            report = coff.compare_unit(name)
            if (report['result'] != 'exact' or report['matched_bytes'] != report['size']
                    or report['matched_compared_bytes'] != report['compared_size']):
                raise ValueError('Player state dependent lost complete equality: '+name)
            checked.append({key: report[key] for key in ('unit', 'size', 'compared_size', 'result')})
        if source == 'src/EclManager.cpp' and '/Ob1' in profile:
            obj = ROOT/units[names[0]]['object']
            sys.path.insert(0, str(ROOT/'scripts'))
            enemy = module('enemy_native_state_audit', 'inspect-enemy-manager-update.py').diagnose(obj)
            guard = module('enemy_native_state_guard', 'check-boss-frontiers.py')
            guard.guard_enemy_update(enemy)
            frontiers['enemy'] = enemy
            ecl = module('enemy_native_state_ecl', 'inspect-ecl-complete.py').report(obj)
            if (ecl['physical_bytes'] != 15564 or ecl['relocations'] != 598
                    or ecl['ordinary_different_bytes'] != 30 or ecl['field_different_bytes']
                    or ecl['table_different_bytes']):
                raise ValueError('shared Player state changed the protected RunEcl frontier')
            frontiers['runecl'] = {key: ecl[key] for key in (
                'physical_bytes', 'relocations', 'ordinary_different_bytes',
                'field_different_bytes', 'table_different_bytes')}
    if inputs(scope) != before:
        raise ValueError('shared Player interface producer inputs changed')
    return dict(diagnostic_only=True, exactness_credit='no new credit', scope=scope,
                authored_exact_delta=0, checked=checked, frontiers=frontiers,
                producer_inputs_sha256=before)


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--scope', choices=tuple(SCOPES), default='state')
    args = parser.parse_args()
    try:
        print(json.dumps(run(args.scope), indent=2))
        return 0
    except (OSError, ValueError, RuntimeError, KeyError, ImportError) as error:
        print('Enemy Player state replay rejected:', error, file=sys.stderr)
        return 2


if __name__ == '__main__':
    raise SystemExit(main())
