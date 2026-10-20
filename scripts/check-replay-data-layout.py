#!/usr/bin/env python3
"""Replay the 33-byte constructor chain without promoting unknown origins.

--with-dependents cold-checks only the 61 accepted owners whose source includes
this layout, plus the existing nonexact consumers. Whole-product and runtime
closure remain separate. Every compile is serial and runs at nice level 10.
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
SOURCE = 'src/ReplayDataConstructors.cpp'
MODELS = {
    'replay-payload-constructor': (
        0x420830, 15, '61dabff3b73f4e562392c124ba87fc9d41d141b9cce11a1635314299e2600a6e',
        '??0ReplayPayloadView@@QAE@XZ', 7, '??0GameConfiguration@@QAE@XZ', 0x41A98B),
    'replay-data-constructor': (
        0x420C40, 18, '52a4edf7dc418fd43b7e6c542b8b27e2ca719a607b591e05a60910d199b4d114',
        '??0ReplayDataView@@QAE@XZ', 10, '??0ReplayPayloadView@@QAE@XZ', 0x420830),
}
DEPENDENT_COUNTS = {
    'src/GameManagerSetup.cpp': 10,
    'src/ReplayInput.cpp': 12,
    'src/ReplayPlaybackFrameAdvance.cpp': 1,
    'src/TitleScreen.cpp': 38,
}
RENDERERS = ('src/TitleScreenReplayMenuDraw.cpp', 'src/TitleScreenReplaySaveDraw.cpp')


def module(name, filename):
    spec = importlib.util.spec_from_file_location(name, ROOT/'scripts'/filename)
    result = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(result)
    return result


def ledger(filename):
    with (ROOT/'config'/filename).open(newline='') as stream:
        return list(csv.DictReader(stream))


def validate_models(units):
    profile = units['replay-manager-begin-recording-stage']['profile']
    for name, (address, size, _, symbol, offset, callee, destination) in MODELS.items():
        u = units[name]
        expected = dict(source=SOURCE, object='build/matching/ReplayDataConstructors.obj',
                        profile=profile, symbol=symbol, target_address=address, size=size,
                        relocations=[dict(offset=offset, type='REL32', symbol=callee, target=destination)])
        if any(u.get(key) != value for key, value in expected.items()):
            raise ValueError('replay constructor extent/profile/source/callee differs: '+name)
    native = units['game-configuration-constructor']
    if (native['source'] != 'src/GameConfiguration.cpp' or native['size'] != 12
            or native['symbol'] != '??0GameConfiguration@@QAE@XZ'
            or native['target_address'] != 0x41A98B):
        raise ValueError('replay payload lost the independently exact native configuration callee')


def validate_origin_policy(functions, origins, matches):
    by_address = {int(row['address'], 0): row for row in functions}
    original = {int(row['address'], 0): row for row in origins}
    accepted = {int(row['address'], 0) for row in matches}
    for address, *_ in MODELS.values():
        f, origin = by_address[address], original[address]
        if (origin['origin'] != 'unknown' or origin['disposition'] != 'review'
                or f['owner'] == 'authored' or f['status'] == 'matching'
                or f['source_file'] != SOURCE or address in accepted):
            raise ValueError('constructor byte equality was confused with authored-origin credit')


def dependent_groups(units, matches):
    groups = defaultdict(list)
    for name in {row['unit'] for row in matches}:
        u = units[name]
        if u['source'] in DEPENDENT_COUNTS:
            groups[(u['source'], tuple(u['profile']))].append(name)
    counts = Counter()
    for (source, _), names in groups.items():
        counts[source] += len(names)
    if dict(counts) != DEPENDENT_COUNTS:
        raise ValueError('replay-layout accepted dependent inventory changed')
    return [(source, list(profile), sorted(names))
            for (source, profile), names in sorted(groups.items())]


def input_hashes(sources):
    result = {}
    def visit(path):
        path = path.resolve()
        relative = str(path.relative_to(ROOT))
        if relative in result:
            return
        data = path.read_bytes()
        result[relative] = hashlib.sha256(data).hexdigest()
        for include in re.findall(r'^\s*#\s*include\s+"([^"]+)"', data.decode(), re.M):
            child = path.parent/include
            if not child.is_file():
                child = ROOT/'src'/include
            if child.is_file():
                visit(child)
    for source in sources:
        visit(ROOT/source)
    for path in ('config/match-units.toml', 'config/function-origins.csv',
                 'config/functions.csv', 'config/matches.csv', 'config/tools.lock.toml',
                 'scripts/compile-probe.sh', 'scripts/check-replay-data-layout.py',
                 'scripts/check-boss-frontiers.py'):
        result[path] = hashlib.sha256((ROOT/path).read_bytes()).hexdigest()
    return result


def build(name):
    completed = subprocess.run(['nice', '-n', '10', sys.executable, '-B',
                                'scripts/build-match-unit.py', '--unit', name],
                               cwd=ROOT, capture_output=True, text=True)
    if completed.returncode:
        raise ValueError('constructor/dependent build failed: '+completed.stdout+completed.stderr)


def require_exact(result):
    if (result.get('result') != 'exact' or result.get('matched_bytes') != result.get('size')
            or result.get('matched_compared_bytes') != result.get('compared_size')
            or result.get('compared_size', 0) < result.get('size', 0)):
        raise ValueError('replay-layout dependent no longer matches its complete original owner')
    return {key: result[key] for key in ('unit', 'size', 'compared_size', 'result')}


def diagnostic(script, obj, *flags):
    result = subprocess.run([sys.executable, '-B', 'scripts/'+script, str(obj), *flags],
                            cwd=ROOT, capture_output=True, text=True)
    if result.returncode:
        raise ValueError('replay-layout consumer audit failed: '+result.stdout+result.stderr)
    return json.loads(result.stdout)


def collect_frontiers(source, profile, obj):
    frontier = module('replay_layout_frontier_guards', 'check-boss-frontiers.py')
    results = {}
    if source == 'src/ReplayInput.cpp' and '/EHsc' not in profile:
        results['save'] = diagnostic('inspect-replay-save.py', obj, '--guard', '--json')
        p = diagnostic('inspect-replay-begin-playback.py', obj, '--json')
        if (p['candidate_bytes'] != 538 or p['bound_fields'] != 32
                or not p['ordered_calls_agree'] or not p['complete_direct_graphs_agree']
                or not set(p['difference_offsets']).issubset(frontier.REPLAY_BEGIN_ORIGINAL_RESIDUAL_OFFSETS)
                or p['field_offset_deltas'] != list(frontier.REPLAY_BEGIN_CURRENT_FIELD_OFFSET_DELTAS)):
            raise ValueError('typed replay layout changed the protected playback frontier')
        results['playback'] = p
    if source == 'src/GameManagerSetup.cpp' and '/Os' in profile:
        results['setup'] = diagnostic('inspect-gameplay-setup.py', obj, '--json')
        frontier.guard_gameplay_setup_thread(results['setup'])
        results['cleanup'] = diagnostic('inspect-game-manager-cleanup.py', obj)
        if (results['cleanup']['candidate_bytes'] != 410
                or results['cleanup']['overlap_differences'] != 210
                or not results['cleanup']['original_limit_double_store']
                or not results['cleanup']['mode2_lower_bound_branch_present']):
            raise ValueError('typed replay layout changed the cleanup frontier')
    if source == 'src/TitleScreen.cpp' and '/O1' in profile:
        for kind in ('active', 'reverse', 'inactive'):
            result = diagnostic('inspect-title-character-cursors.py', obj, '--variant', kind)
            if kind in ('reverse', 'inactive'):
                frontier.guard_title_cursor_reverse_inactive(kind, result)
            elif (result['candidate_bytes'] != 215 or result['overlap_differences'] != 185
                  or result['bound_fields'] != 6 or not result['ordered_calls_agree']):
                raise ValueError('typed replay layout changed the active cursor frontier')
            results['cursor_'+kind] = result
        result = diagnostic('inspect-title-update-result.py', obj)
        frontier.guard_title_update_result(result)
        results['update_result'] = result
    return results


def run(with_dependents=False):
    coff = module('replay_layout_coff', 'compare-coff-function.py')
    target = coff.verified_target()
    units = tomllib.loads((ROOT/'config/match-units.toml').read_text())['units']
    matches = ledger('matches.csv')
    validate_models(units)
    validate_origin_policy(ledger('functions.csv'), ledger('function-origins.csv'), matches)
    for address, size, expected, *_ in MODELS.values():
        if hashlib.sha256(coff.pe_bytes_at(target, address, size)).hexdigest() != expected:
            raise ValueError('original replay construction complete SHA differs')
    sources = [SOURCE]+(list(DEPENDENT_COUNTS)+list(RENDERERS) if with_dependents else [])
    before = input_hashes(sources)
    build('replay-payload-constructor')
    constructors = [require_exact(coff.compare_unit(name)) for name in MODELS]
    report = dict(diagnostic_only=True, exactness_credit='none; constructor origin remains unknown',
                  authored_exact_delta=0, constructor_bytes=33, constructors=constructors,
                  dependent_units=[], frontiers={}, producer_inputs_sha256=before)
    if with_dependents:
        for source, profile, names in dependent_groups(units, matches):
            build(names[0])
            report['dependent_units'].extend(require_exact(coff.compare_unit(name)) for name in names)
            report['frontiers'].update(collect_frontiers(source, profile, ROOT/units[names[0]]['object']))
        flags = units['title-screen-setup-thread']['profile']
        for source in RENDERERS:
            obj = 'build/diagnostics/replay-layout-'+Path(source).stem+'.obj'
            result = subprocess.run(['nice', '-n', '10', str(ROOT/'scripts/compile-probe.sh'),
                                     source, obj, *flags], cwd=ROOT, capture_output=True, text=True)
            if result.returncode:
                raise ValueError('replay-layout renderer compile failed: '+result.stdout+result.stderr)
            if 'MenuDraw' in source:
                data = diagnostic('inspect-title-replay-menu-draw.py', obj, '--json')
                module('layout_menu_guard', 'check-boss-frontiers.py').guard_title_replay_menu(data)
                report['frontiers']['draw_replay_menu'] = data
            else:
                data = diagnostic('inspect-title-replay-save-draw.py', obj, '--json')
                module('layout_save_guard', 'check-boss-frontiers.py').guard_title_replay_save(data)
                report['frontiers']['draw_replay_save'] = data
    if input_hashes(sources) != before:
        raise ValueError('replay-layout source/profile inputs changed during production')
    return report


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--with-dependents', action='store_true')
    args = parser.parse_args()
    try:
        print(json.dumps(run(args.with_dependents), indent=2))
        return 0
    except (OSError, ValueError, KeyError, TypeError, ImportError, RuntimeError) as error:
        print('replay construction audit rejected:', error, file=sys.stderr)
        return 2


if __name__ == '__main__':
    raise SystemExit(main())
