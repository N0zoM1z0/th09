#!/usr/bin/env python3
"""Cold replay the current TH09 large-owner ECL/Etama/ExAttack frontiers.

All non-exact outputs are diagnostics, never partial match credit. Every object
is first rebuilt from the live source using the target-bound match-unit tool.
For unchanged canonical siblings we additionally demand full match-unit replay.
No stored experiment, previous object or historical percentage is accepted.
"""

from __future__ import annotations

import argparse
import json
from pathlib import Path
import subprocess
import sys
import tempfile

ROOT = Path(__file__).resolve().parents[1]
PYTHON = sys.executable


def execute(args: list[str], *, diagnostic: bool = False) -> str:
    command = [PYTHON, *args]
    completed = subprocess.run(command, cwd=ROOT, capture_output=True, text=True)
    # A proven near match intentionally exits 1. Only an independently parsed
    # non-crediting diagnostic may use that exit code; build/oracle errors fail.
    allowed = (0, 1) if diagnostic else (0,)
    if completed.returncode not in allowed:
        raise RuntimeError(f"{args[0]} exited {completed.returncode}: "
                           f"{(completed.stderr + completed.stdout)[-1200:]}")
    return completed.stdout


def parsed(args: list[str], *, diagnostic: bool = False) -> dict:
    out = execute(args, diagnostic=diagnostic)
    try:
        document = json.loads(out)
    except (ValueError, TypeError) as exc:
        raise RuntimeError(f"malformed diagnostic result: {args[0]}: {out[-350:]}") from exc
    if not isinstance(document, dict) or 'error' in document or document.get('result') == 'error':
        raise RuntimeError(f"diagnostic rejected input: {args[0]}: {document}")
    return document


def noncrediting(result: dict, *, name: str, target_size: int,
                 fields: int, size_key: str = 'target_bytes',
                 fields_key: str = 'bound_fields') -> dict:
    if (result.get('diagnostic_only') is not True or
            not str(result.get('exactness_credit', '')).lower().startswith('none')):
        raise RuntimeError(f"{name} did not produce an expressly non-crediting diagnostic")
    if result.get(size_key) != target_size or result.get(fields_key) != fields:
        raise RuntimeError(f"{name} target extent/field inventory changed: {result}")
    return result


def canonical(unit: str) -> dict:
    result = parsed(['scripts/compare-coff-function.py', '--unit', unit, '--json'])
    # Some authored logical extents own additional switch tables and/or
    # alignment bytes in the strict physical comparison (e.g. 215/232 and
    # 2195/2216). Both extents must match, not equal one another.
    if (result.get('result') != 'exact' or result.get('matched_bytes') != result.get('size')
            or result.get('matched_compared_bytes') != result.get('compared_size')
            or result.get('compared_size', -1) < result.get('size', 0)):
        raise RuntimeError(f"canonical sibling no longer replays completely: {unit}")
    return {'unit': unit, 'logical_bytes': result['size'],
            'physical_bytes': result['compared_size'],
            'relocations': len(result.get('relocations', [])), 'result': 'exact'}


def build(unit: str) -> None:
    execute(['scripts/build-match-unit.py', '--unit', unit])


def run_ecl(*, include_trail: bool, include_interpreter: bool,
            protect_exact: bool) -> tuple[dict, list[dict]]:
    build('ecl-pop-context')
    object_file = 'build/matching/EclManager.obj'
    diagnostics: dict[str, dict] = {}
    exact_units = [canonical('ecl-pop-context')] if protect_exact else []
    if include_interpreter:
        with tempfile.TemporaryDirectory(prefix='th09-runecl-') as directory:
            destination = str(Path(directory) / 'complete.json')
            execute(['scripts/inspect-ecl-complete.py', object_file, '--output', destination],
                    diagnostic=True)
            complete = json.loads(Path(destination).read_text())
        noncrediting(complete, name='RunEcl', target_size=15564, fields=598,
                     size_key='physical_bytes', fields_key='relocations')
        shapes = noncrediting(parsed(['scripts/report-ecl-handler-shapes.py', '--json', object_file]),
                              name='RunEcl handlers', target_size=14792, fields=148,
                              size_key='target_code_bytes', fields_key='unique_handlers')
        if shapes.get('identity_audit_passed') is not True:
            raise RuntimeError('RunEcl handler shapes lack complete target identity')
        biased = noncrediting(parsed(['scripts/inspect-ecl-biased-movement.py', '--json', object_file],
                                    diagnostic=True), name='ECL MoveRandomBiased',
                              target_size=516, fields=28)
        diagnostics.update(runecl={
            'physical_bytes': complete['physical_bytes'],
            'ordinary_differences': complete['ordinary_different_bytes'],
            'field_differences': complete['field_different_bytes'],
            'table_differences': complete['table_different_bytes'],
            'matching_handler_shapes': shapes['matching_handler_shapes'],
            'handlers': shapes['unique_handlers'],
        }, ecl_move_random_biased={
            'target_bytes': 516, 'candidate_bytes': biased['candidate_bytes'],
            'overlapping_differences': biased['overlapping_differences'],
            'direct_graphs_agree': biased['complete_direct_graphs_agree'],
        })
    if include_trail:
        for callback_type, size, relocations in (('19', 763, 57), ('21', 727, 53)):
            result = noncrediting(parsed(['scripts/inspect-exattack-trail-init.py', '--type',
                                          callback_type, object_file]),
                                  name='ExAttack init ' + callback_type,
                                  target_size=size, fields=relocations, fields_key='relocations')
            diagnostics['exattack_type' + callback_type + '_init'] = {
                'target_bytes': size, 'candidate_bytes': result['candidate_bytes'],
                'different_bytes': result['complete_differences_if_same_extent'],
                'direct_graphs_agree': result['complete_direct_graphs_agree'],
            }
        if protect_exact:
            build('exattack-type21-update')
            exact_units.append(canonical('exattack-type21-update'))
    return diagnostics, exact_units


def run_etama(*, protect_exact: bool) -> tuple[dict, list[dict]]:
    build('bullet-draw-single')
    result = noncrediting(parsed(['scripts/inspect-etama-draw.py',
                                 'build/matching/EtamaControllerDraw.obj', '--json'],
                                diagnostic=True), name='Etama OnDraw',
                          target_size=600, fields=27)
    exact_units = [canonical('bullet-draw-single')] if protect_exact else []
    if protect_exact:
        build('etama-controller-update')
        exact_units.append(canonical('etama-controller-update'))
    return {'etama_draw': {
        'target_bytes': 600, 'candidate_bytes': result['candidate_bytes'],
        'overlapping_differences': result['overlapping_byte_differences'],
        'direct_graphs_agree': result['complete_direct_graphs_agree'],
    }}, exact_units


def run_title(*, protect_exact: bool) -> tuple[dict, list[dict]]:
    # TitleSetup is compiled from the large TitleScreen TU, whereas both
    # MusicRoom owners share the separate TitleScreenMusicRoom TU.
    exact_units: list[dict] = []
    if protect_exact:
        build('title-screen-setup-thread')
        exact_units.append(canonical('title-screen-setup-thread'))
        build('title-screen-music-room')
        exact_units.extend((canonical('title-screen-music-room'),
                            canonical('title-screen-draw-music-room')))
    # Existing matches stay independent: none of these receipts establishes
    # the still-unresolved TitleScreen DrawResult/Options/DrawCallback owners.
    return {}, exact_units


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--owner', choices=['runecl', 'etama', 'exattack', 'title', 'all'],
                        default='all')
    parser.add_argument('--skip-exact-siblings', action='store_true',
                        help='diagnostics only; explicitly omit the extra canonical regressions')
    args = parser.parse_args()
    report = {'diagnostic_only': True, 'exactness_credit': 'none',
              'owners': {}, 'canonical_siblings': []}
    try:
        execute(['scripts/verify-target.py'])
        if args.owner in ('runecl', 'exattack', 'all'):
            owners, exact = run_ecl(include_interpreter=args.owner in ('runecl', 'all'),
                                    include_trail=args.owner in ('exattack', 'all'),
                                    protect_exact=not args.skip_exact_siblings)
            report['owners'].update(owners)
            report['canonical_siblings'].extend(exact)
        if args.owner in ('etama', 'all'):
            owners, exact = run_etama(protect_exact=not args.skip_exact_siblings)
            report['owners'].update(owners)
            report['canonical_siblings'].extend(exact)
        if args.owner in ('title', 'all'):
            owners, exact = run_title(protect_exact=not args.skip_exact_siblings)
            report['owners'].update(owners)
            report['canonical_siblings'].extend(exact)
        print(json.dumps(report, indent=2))
        return 0
    except (OSError, RuntimeError, ValueError, KeyError, json.JSONDecodeError) as exc:
        print('boss frontier replay failed:', exc, file=sys.stderr)
        return 1


if __name__ == '__main__':
    raise SystemExit(main())
