#!/usr/bin/env python3
"""Cold-check the shared GameManager prefix and Front's five native data reads.

Only the 12 accepted units whose source includes GameManagerMode.hpp are
rebuilt, serially at nice10, plus the nonexact Front callback. This preserves
existing source/profile proofs; it awards no new exact or origin credit.
"""
from __future__ import annotations

import argparse
from collections import Counter, defaultdict
import csv
import hashlib
import importlib.util
import json
import os
from pathlib import Path
import re
import subprocess
import sys
import tomllib

ROOT = Path(__file__).resolve().parents[1]
HEADER = 'src/GameManagerMode.hpp'
EXPECTED = {
    'src/EndingAddedCallback.cpp': 1, 'src/FrontMessageInitializeRuntime.cpp': 1,
    'src/FrontMessageLoad.cpp': 1, 'src/FrontMessageReleaseForSideCount.cpp': 1,
    'src/FrontResultBonus.cpp': 1, 'src/GameManagerMode.cpp': 4,
    'src/GameManagerSelectOpponent.cpp': 1, 'src/PlayerGameplayState.cpp': 2,
}


def module(name, filename):
    spec = importlib.util.spec_from_file_location(name, ROOT/'scripts'/filename)
    result = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(result)
    return result


def source_inputs(source):
    result = {}
    def visit(path):
        if path.is_symlink() or not path.resolve().is_relative_to((ROOT/'src').resolve()):
            raise ValueError('GameManager dependency escapes or aliases src')
        name = path.relative_to(ROOT).as_posix()
        if name in result:
            return
        data = path.read_bytes()
        result[name] = hashlib.sha256(data).hexdigest()
        for child in re.findall(rb'^\s*#\s*include\s*"([^"]+)"', data, re.M):
            dependency = path.parent/child.decode()
            if dependency.is_file():
                visit(dependency)
    visit(ROOT/source)
    return result


def affected_groups(units, matches):
    result = defaultdict(list)
    closures = {}
    for name in sorted({row['unit'] for row in matches}):
        unit = units[name]
        source = unit['source']
        if source not in closures:
            closures[source] = source_inputs(source)
        if HEADER in closures[source]:
            result[(source, tuple(unit['profile']))].append(name)
    counts = Counter()
    for (source, _), names in result.items():
        counts[source] += len(names)
    if dict(counts) != EXPECTED or len(result) != 9:
        raise ValueError('shared GameManager affected-unit inventory changed')
    return sorted(result.items())


def inputs():
    hashes = source_inputs('src/FrontAddedCallback.cpp')
    for source in EXPECTED:
        hashes.update(source_inputs(source))
    for name in ('config/functions.csv', 'config/matches.csv', 'config/match-units.toml',
                 'config/target.toml', 'config/tools.lock.toml', 'scripts/compile-probe.sh',
                 'scripts/build-match-unit.py', 'scripts/compare-coff-function.py',
                 'scripts/check-front-game-data.py', 'scripts/check-enemy-player-state.py',
                 'scripts/inspect-front-added.py', 'scripts/check-boss-frontiers.py',
                 'scripts/inspect-effect-type34.py', 'scripts/diagnostic_branch_alignment.py'):
        hashes[name] = hashlib.sha256((ROOT/name).read_bytes()).hexdigest()
    return hashes


def run():
    coff = module('front_game_coff', 'compare-coff-function.py')
    image = coff.verified_target()
    units = tomllib.loads((ROOT/'config/match-units.toml').read_text())['units']
    with (ROOT/'config/matches.csv').open(newline='') as stream:
        groups = affected_groups(units, list(csv.DictReader(stream)))
    before, checked, failures = inputs(), [], []
    share = module('front_game_object_share', 'check-enemy-player-state.py')
    for _group, names in groups:
        built = subprocess.run(['nice', '-n', '10', sys.executable, '-B',
                                'scripts/build-match-unit.py', '--unit', names[0]],
                               cwd=ROOT, capture_output=True, text=True)
        if built.returncode:
            raise ValueError('GameManager dependent compile failed: '+built.stdout+built.stderr)
        share.share_object(units, names)
        for name in names:
            try:
                report = coff.compare_unit(name)
                if (report['result'] != 'exact' or report['matched_bytes'] != report['size']
                        or report['matched_compared_bytes'] != report['compared_size']):
                    raise ValueError('incomplete original equality')
                checked.append({key: report[key] for key in ('unit', 'size', 'compared_size', 'result')})
            except ValueError as error:
                failures.append(dict(unit=name, error=str(error)))
    boss = module('front_game_boss', 'check-boss-frontiers.py')
    frontier, _ = boss.run_front_added(protect_exact=False)
    if inputs() != before:
        raise ValueError('shared GameManager producer inputs changed')
    return dict(diagnostic_only=True, exactness_credit='no new credit',
                target_sha256=hashlib.sha256(image).hexdigest(), checked=checked,
                failures=failures, front=frontier, producer_inputs_sha256=before)


def main():
    argparse.ArgumentParser(description=__doc__).parse_args()
    try:
        # Also lower the diagnostic callback's inherited compiler priority.
        os.nice(max(0, 10-os.getpriority(os.PRIO_PROCESS, 0)))
        report = run()
        print(json.dumps(report, indent=2))
        return 2 if report['failures'] else 0
    except (ValueError, RuntimeError, OSError, KeyError, ImportError) as error:
        print('Front GameManager replay rejected:', error, file=sys.stderr)
        return 2


if __name__ == '__main__':
    raise SystemExit(main())
