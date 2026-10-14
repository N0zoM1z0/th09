#!/usr/bin/env python3
"""Cold replay TH09 boss and companion-leaf ECL/Enemy/Etama/ExAttack frontiers.

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
import tomllib

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


def build_nonexact(source: str, obj: str, *, reference_unit: str) -> None:
    """Cold compile a non-exact TU using a proven sibling's pinned VC7.1 profile.

    This is only a reproducible source/object observation, not an exact unit
    or a claim that the original executable used this TU ownership.
    """
    with (ROOT / 'config/match-units.toml').open('rb') as stream:
        units = tomllib.load(stream)['units']
    unit = units[reference_unit]
    flags = unit['profile']
    if not flags or not all(isinstance(x, str) for x in flags):
        raise RuntimeError('non-exact diagnostic lacks a reviewed VC7.1 profile')
    output = ROOT / obj
    output.resolve().relative_to((ROOT / 'build').resolve())
    # Fail closed on stale prior diagnostics: the next object must be produced
    # by this invocation, not merely be present at a leftover ignored path.
    if output.is_symlink():
        raise RuntimeError('refusing a symlink at the non-exact object path')
    if output.exists():
        output.unlink()
    command = [str(ROOT / 'scripts/compile-probe.sh'), source, obj, *flags]
    response = subprocess.run(command, cwd=ROOT, capture_output=True, text=True)
    if response.returncode != 0 or not output.is_file():
        raise RuntimeError('cold non-exact build failed: ' +
                           (response.stderr + response.stdout)[-1600:])


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
        mismatches = [row['opcode'] for row in shapes['mismatches']]
        if any(opcode not in (4, 155) for opcode in mismatches):
            raise RuntimeError('RunEcl regressed a previously matching handler')
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
            'remaining_opcode_mismatches': mismatches,
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


def run_enemy(*, protect_exact: bool) -> tuple[dict, list[dict]]:
    # The 3,900-byte enemy calc owner and 151-byte exact attached-effects
    # helper share the actual EnemyManagerCore TU. The collision append leaf
    # is a separate 82-byte owner invoked by the calc callback twice.
    build('enemy-attached-effect-update')
    main = noncrediting(parsed(['scripts/inspect-enemy-manager-update.py',
                                'build/matching/EnemyManagerCore.obj', '--json']),
                        name='EnemyManager OnUpdate', target_size=3900,
                        fields=97, fields_key='relocations')
    if (main.get('relocation_fields_agree') != 97 or
            main.get('target_calls') != 69 or main.get('candidate_calls') != 69 or
            not main.get('ordered_calls_agree') or
            main.get('target_direct_blocks') != 170 or
            main.get('candidate_direct_blocks') != 170 or
            not main.get('reviewed_control_flow_agrees') or
            not main.get('switch_table_destinations_agree')):
        raise RuntimeError('EnemyManager 97-field / 69-call / 170-block owner changed')

    obj = 'build/matching/EnemyCollisionBoxAppend.obj'
    build_nonexact('src/EnemyCollisionBoxAppend.cpp', obj,
                   reference_unit='enemy-attached-effect-update')
    collision = noncrediting(parsed(['scripts/inspect-enemy-collision-box.py', obj, '--json'],
                                  diagnostic=True), name='Enemy collision append leaf',
                             target_size=82, fields=2, fields_key='bound_call_fields')
    if (collision.get('target_instructions') != 30 or
            collision.get('ordered_call_addresses') != ['0x40f5a0', '0x40f8e0']):
        raise RuntimeError('Enemy collision append original call/ABI census changed')
    exact = [canonical('enemy-attached-effect-update')] if protect_exact else []
    if protect_exact:
        build('enemy-manager-draw-impl')
        exact.append(canonical('enemy-manager-draw-impl'))
    return {'enemy_manager_update': {
                'target_bytes': 3900,
                'candidate_bytes': main['candidate_bytes'],
                'bound_fields': main['relocations'],
                'ordinary_differences': main['complete_differences'],
                'ordered_calls': main['candidate_calls'],
                'direct_graphs_agree': main['reviewed_control_flow_agrees'],
                'switch_destinations_agree': main['switch_table_destinations_agree']},
            'enemy_collision_append': {
                'target_bytes': 82,
                'candidate_bytes': collision['candidate_bytes'],
                'ordinary_differences': collision['ordinary_differing_bytes'],
                'ordered_calls': collision['ordered_call_addresses'],
                'bound_fields': collision['bound_call_fields']}}, exact


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


def run_exattack_type3(*, protect_exact: bool) -> tuple[dict, list[dict]]:
    # Separate TU from the known exact Type3 initializer: build its *current*
    # maintained update source before linking the full 38-field diagnostic.
    object_file = 'build/matching/ExAttackUpdateType3.obj'
    build_nonexact('src/ExAttackUpdateType3.cpp', object_file,
                   reference_unit='ex-attack-initialize-type3')
    result = noncrediting(parsed(['scripts/inspect-exattack-type3-schedule.py', object_file]),
                          name='ExAttack Type3 update', target_size=780, fields=38)
    if not result['calls_agree'] or result['ordered_calls'] != 24 or not result['direct_graphs_agree']:
        raise RuntimeError('ExAttack Type3 full call / 17-block graph regressed')
    exact = []
    if protect_exact:
        build('ex-attack-initialize-type3')
        exact.append(canonical('ex-attack-initialize-type3'))
    return {'exattack_type3_update': {
        'target_bytes': 780, 'candidate_bytes': result['candidate_bytes'],
        'ordinary_differences': result['ordinary_differences'],
        'original_z_order': result['original_z_order']['classification'],
        'candidate_z_order': result['candidate_z_order']['classification'],
        'direct_graphs_agree': result['direct_graphs_agree'],
    }}, exact


def run_title_open() -> dict:
    # These source-present owners are explicitly non-exact. Their independent
    # original operand inventories must agree before any similarity score.
    result_obj = 'build/matching/TitleResultDraw.obj'
    build_nonexact('src/TitleScreenResultDraw.cpp', result_obj,
                   reference_unit='title-screen-setup-thread')
    draw = noncrediting(parsed(['scripts/inspect-title-result-draw.py', result_obj, '--json']),
                        name='Title DrawResult', target_size=1939, fields=82)
    if (draw['original_fields'] != 82 or draw['ordered_calls'] != 16 or
            not draw['ordered_calls_agree'] or not draw['direct_graphs_agree']):
        raise RuntimeError('Title DrawResult original-bound calls or graph regressed')
    callback_obj = 'build/matching/TitleDrawCallback.obj'
    build_nonexact('src/TitleDrawCallback.cpp', callback_obj,
                   reference_unit='title-screen-setup-thread')
    callback = parsed(['scripts/inspect-title-draw-callback.py', callback_obj, '--json'])
    if (not callback.get('diagnostic_only') or callback.get('target_bytes') != 329 or
            callback.get('bound_relocations') != 20 or callback.get('target_calls') != 12 or
            callback.get('candidate_calls') != 12 or
            not callback.get('complete_direct_graph_equal')):
        raise RuntimeError('Title DrawCallback complete original field/call/graph gate failed')
    replay_obj = 'build/matching/TitleReplaySave.obj'
    build_nonexact('src/TitleScreenReplaySaveDraw.cpp', replay_obj,
                   reference_unit='title-screen-setup-thread')
    replay = noncrediting(parsed(['scripts/inspect-title-replay-save-draw.py', replay_obj,
                                  '--json']), name='Title DrawReplaySave',
                          target_size=800, fields=41)
    if replay['original_fields'] != 41 or not replay['ordered_calls_agree']:
        raise RuntimeError('Title ReplaySave 41 original fields or call sequence regressed')
    return {'title_draw_result': {
                'target_bytes': 1939, 'candidate_bytes': draw['candidate_bytes'],
                'ordinary_differences': draw['complete_differences'],
                'fields': draw['bound_fields'],
                'direct_graphs_agree': draw['direct_graphs_agree']},
            'title_draw_callback': {
                'target_bytes': 329, 'candidate_bytes': callback['candidate_bytes'],
                'fields': callback['bound_relocations'],
                'single_reviewed_dec_gap': callback['single_reviewed_dec_gap'],
                'direct_graphs_agree': callback['complete_direct_graph_equal']},
            'title_draw_replay_save': {
                'target_bytes': 800, 'candidate_bytes': replay['candidate_bytes'],
                'fields': replay['bound_fields'],
                'first_overlapping_difference': replay['first_overlapping_difference'],
                'direct_graphs_agree': replay['direct_graphs_agree']}}


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
    parser.add_argument('--owner', choices=['runecl', 'enemy', 'etama', 'exattack', 'type3', 'title', 'all'],
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
        if args.owner in ('type3', 'exattack', 'all'):
            owners, exact = run_exattack_type3(protect_exact=not args.skip_exact_siblings)
            report['owners'].update(owners)
            report['canonical_siblings'].extend(exact)
        if args.owner in ('enemy', 'all'):
            owners, exact = run_enemy(protect_exact=not args.skip_exact_siblings)
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
            report['owners'].update(run_title_open())
        print(json.dumps(report, indent=2))
        return 0
    except (OSError, RuntimeError, ValueError, KeyError, json.JSONDecodeError) as exc:
        print('boss frontier replay failed:', exc, file=sys.stderr)
        return 1


if __name__ == '__main__':
    raise SystemExit(main())
