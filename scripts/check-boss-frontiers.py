#!/usr/bin/env python3
"""Cold replay TH09 ECL/Enemy/Etama/ExAttack/ANM/Title boss-and-leaf frontiers.

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
            validate_exattack_trail_initializer(callback_type, result)
            diagnostics['exattack_type' + callback_type + '_init'] = {
                'target_bytes': size, 'candidate_bytes': result['candidate_bytes'],
                'different_bytes': result['complete_differences_if_same_extent'],
                'direct_graphs_agree': result['complete_direct_graphs_agree'],
            }
        if protect_exact:
            build('exattack-type21-update')
            exact_units.append(canonical('exattack-type21-update'))
    return diagnostics, exact_units



def validate_exattack_trail_initializer(callback_type: str, result: dict) -> None:
    """Reject independent original/COFF ABI or complete-body regressions.

    Opcode/helper topology is not exactness: this remains noncrediting until a
    separate canonical target-bound match unit replays ALL bytes and fields.
    """
    specs = {'19': (763, 207, 57, 27, 9, 6),
             '21': (727, 198, 53, 25, 10, 9)}
    if callback_type not in specs:
        raise RuntimeError('unsupported ExAttack trail initializer type')
    size, instructions, relocations, calls, blocks, max_differences = specs[callback_type]
    if (result.get('target_bytes') != size or
            result.get('candidate_bytes') != size or
            result.get('target_instructions') != instructions or
            result.get('candidate_instructions') != instructions or
            result.get('relocations') != relocations or
            result.get('target_calls') != calls or
            result.get('candidate_calls') != calls or
            result.get('ordered_calls_agree') is not True or
            result.get('direct_blocks') != [blocks, blocks] or
            result.get('complete_direct_graphs_agree') is not True or
            not isinstance(result.get('complete_differences_if_same_extent'), int) or
            result['complete_differences_if_same_extent'] > max_differences or
            result.get('complete_bytes_agree') is not False):
        raise RuntimeError('ExAttack Type' + callback_type +
                           ' initializer full-owner 57/53-field/6/9-byte frontier regressed')


def run_player_collision(*, protect_exact: bool) -> tuple[dict, list[dict]]:
    """Cold replay exact 602-byte Player collision query and true helper owners.

    The query and the separately 121-byte exact item-box helper MUST be
    compiled from the same TU. Making the callee body visible tells VC7.1
    that its thiscall preserves ECX on the item-box success arm: absent that
    visibility the query gets an extra MOV ECX,[ESI] and grows to 604 bytes.
    All remaining collision helpers keep their own exact translation unit.
    """
    build('player-find-collision')
    exact = [canonical('player-find-collision')]
    # The large non-exact Player cancel/collision body is an independent
    # function in the real PlayerCollisionHelpers TU. Its full PE and all
    # twenty-six original code operands are source-cold checked before the
    # exact CheckGrazeCollision consumer and other same-TU siblings replay.
    build('player-check-graze-collision')
    bullet = noncrediting(parsed(['scripts/inspect-player-bullet-collision.py',
                                  'build/matching/PlayerCollisionHelpers.obj', '--json']),
                          name='Player CheckBulletCollision boss',
                          target_size=862, fields=26)
    if (bullet.get('candidate_bytes') != 849 or
            bullet.get('target_instructions') != 277 or
            bullet.get('candidate_instructions') != 266 or
            bullet.get('original_fields') != 26 or
            bullet.get('ordered_calls') != 12 or
            bullet.get('ordered_calls_agree') is not True or
            bullet.get('ordered_fields_agree') is not True or
            bullet.get('original_blocks') != 33 or
            bullet.get('candidate_blocks') not in (32, 33) or
            bullet.get('direct_graphs_agree') is not (bullet.get('candidate_blocks') == 33) or
            not isinstance(bullet.get('overlap_differences'), int) or
            bullet['overlap_differences'] > 804 or
            bullet.get('missing_target_bytes') != 13 or
            bullet.get('extra_candidate_bytes') != 0 or
            bullet.get('complete_bytes_equal') is not False):
        raise RuntimeError('Player 862-byte original/849-byte source 26-field/12-call collision boss regressed')
    if protect_exact:
        # Same just-built object; no stale target or duplicate source definition.
        exact.append(canonical('player-calc-item-box-collision'))
        siblings = (
            'player-check-graze-collision',
            'player-calc-laser-hitbox',
            'player-calc-item-collection-collision',
            'player-calc-circle-collision',
            'player-find-collision-at-player',
        )
        for name in siblings:
            if name != 'player-check-graze-collision':
                build(name)
            exact.append(canonical(name))
    return {'player_check_bullet_collision': {
        'target_bytes': 862,
        'candidate_bytes': bullet['candidate_bytes'],
        'original_fields': bullet['original_fields'],
        'bound_fields': bullet['bound_fields'],
        'ordered_calls': bullet['ordered_calls'],
        'target_blocks': bullet['original_blocks'],
        'candidate_blocks': bullet['candidate_blocks'],
        'overlap_differences': bullet['overlap_differences'],
        'missing_target_bytes': bullet['missing_target_bytes'],
    }}, exact


def run_decrypt_table(*, protect_exact: bool) -> tuple[dict, list[dict]]:
    """Check full original crypto-table leaf, plus exact decrypt/encrypt pair.

    The 8 x 12-byte crypt key table and three-byte original signature are
    checked independently in the already SHA-pinned 220-byte inspector; it
    intentionally returns 1 for valid non-exact results. Never mistake that
    ordinary near-match for acceptance of the separately exact Decrypt API.
    """
    obj = 'build/matching/FileDecryptTable.obj'
    build_nonexact('src/FileDecryptTable.cpp', obj,
                   reference_unit='file-system-decrypt')
    result = noncrediting(parsed(['scripts/inspect-file-decrypt-table.py',
                                  obj, '--json'], diagnostic=True),
                          name='FileSystem decryption-table scan',
                          target_size=220, fields=12)
    if (result['candidate_bytes'] != 220 or
            result['target_instructions'] != 77 or
            result['candidate_instructions'] != 77 or
            result['ordered_calls'] != 2 or
            result['target_blocks'] != 11 or
            result['candidate_blocks'] != 11 or
            not result['complete_direct_graphs_agree'] or
            result['complete_overlap_differences'] != 38 or
            result['complete_bytes_equal']):
        raise RuntimeError('TH09 FileSystem crypt-table target/COFF/graph frontier drifted')
    exact = []
    if protect_exact:
        build('file-system-decrypt')
        exact.append(canonical('file-system-decrypt'))
        build('file-system-encrypt')
        exact.append(canonical('file-system-encrypt'))
    return {'file_decrypt_table': {
        'target_bytes': 220,
        'candidate_bytes': result['candidate_bytes'],
        'bound_fields': result['bound_fields'],
        'ordered_calls': result['ordered_calls'],
        'ordinary_differences': result['complete_overlap_differences'],
        'direct_graphs_agree': result['complete_direct_graphs_agree'],
    }}, exact



def run_network_config_parser(*, protect_exact: bool) -> tuple[dict, list[dict]]:
    """One original-bound nonexact parser plus its true exact caller carrier."""
    build('supervisor-network-config')
    result = noncrediting(parsed(['scripts/inspect-network-config-parser.py',
                                  'build/matching/SupervisorNetworkConfig.obj', '--json']),
                          name='Network config parser', target_size=243,
                          fields=1)
    if (result.get('candidate_bytes') != 243 or
            result.get('target_instructions') != 116 or
            result.get('candidate_instructions') != 116 or
            result.get('original_fields') != 1 or
            result.get('ordered_calls') != 1 or
            not result.get('ordered_calls_agree') or
            result.get('target_blocks') != 33 or
            result.get('candidate_blocks') not in (33, 34) or
            result.get('direct_graphs_agree') is not (result.get('candidate_blocks') == 33) or
            not isinstance(result.get('overlap_differences'), int) or
            result['overlap_differences'] > 76 or
            result.get('missing_target_bytes') != 0 or
            result.get('extra_candidate_bytes') != 0 or
            result.get('complete_bytes_equal') is not False):
        raise RuntimeError('network parser 243-byte/1-field/1-call/33-block original boundary regressed')
    siblings = [canonical('supervisor-network-config')] if protect_exact else []
    return {'network_config_parser': {
        'target_bytes': 243, 'candidate_bytes': result['candidate_bytes'],
        'original_fields': result['original_fields'],
        'bound_fields': result['bound_fields'],
        'ordered_calls': result['ordered_calls'],
        'overlap_differences': result['overlap_differences'],
        'target_blocks': result['target_blocks'],
        'candidate_blocks': result['candidate_blocks'],
        'direct_graphs_agree': result['direct_graphs_agree'],
    }}, siblings


def run_front_random(*, protect_exact: bool) -> tuple[dict, list[dict]]:
    """Cold original-backed 693-byte Front weighted picker and exact family."""
    obj='build/matching/FrontMessageRandomSetup.obj'
    build_nonexact('src/FrontMessageRandomSetup.cpp',obj,
                   reference_unit='front-message-setup')
    owner=noncrediting(parsed(['scripts/inspect-front-message-random.py',obj,'--json']),
                       name='Front weighted random setup',target_size=693,fields=15)
    if (owner.get('candidate_bytes',0) < 682 or
            owner['candidate_bytes'] > 693 or
            owner.get('target_instructions') != 206 or
            not isinstance(owner.get('candidate_instructions'),int) or
            not 202 <= owner['candidate_instructions'] <= 206 or
            owner.get('original_fields') != 15 or
            owner.get('ordered_calls') != 6 or
            owner.get('ordered_calls_agree') is not True or
            owner.get('target_blocks') != 40 or
            owner.get('candidate_blocks') not in (40,41) or
            owner.get('direct_graphs_agree') is not (owner['candidate_blocks']==40) or
            not isinstance(owner.get('aligned_instructions'),int) or
            owner['aligned_instructions'] < 117 or
            not isinstance(owner.get('overlap_differences'),int) or
            owner['overlap_differences'] > 581 or
            owner.get('missing_target_bytes',99) > 11 or
            owner.get('extra_candidate_bytes') != 0 or
            owner.get('complete_bytes_equal') is not False):
        raise RuntimeError('Front random 693-byte/15-field/6-call original-bound frontier regressed')
    exact=[]
    if protect_exact:
        for unit in ('front-calc-callback', 'front-message-setup',
                     'front-message-load-for-side-count',
                     'front-message-initialize-runtime'):
            build(unit)
            exact.append(canonical(unit))
    return {'front_random_setup': {
        'target_bytes':693,'candidate_bytes':owner['candidate_bytes'],
        'original_fields':owner['original_fields'],
        'bound_fields':owner['bound_fields'],
        'ordered_calls':owner['ordered_calls'],
        'target_blocks':owner['target_blocks'],
        'candidate_blocks':owner['candidate_blocks'],
        'aligned_instructions':owner['aligned_instructions'],
        'overlap_differences':owner['overlap_differences'],
        'missing_target_bytes':owner['missing_target_bytes'],
        'direct_graphs_agree':owner['direct_graphs_agree'],
    }},exact


def run_received_frames(*, protect_exact: bool) -> tuple[dict, list[dict]]:
    # Network frame insertion has no relocations, but 213/213 equal extent
    # alone is not evidence of code equality. Bind its entire Japanese
    # original SHA, 67 instructions, nine blocks, no calls and ret 16 ABI.
    obj = 'build/matching/SupervisorFrameQueueInsert.obj'
    build_nonexact('src/SupervisorFrameQueueInsert.cpp', obj,
                   reference_unit='supervisor-frame-queue-insert-predicted')
    received = noncrediting(parsed(['scripts/inspect-supervisor-frame-queue.py',
                                    obj, '--json']),
                            name='Supervisor received-frame insertion',
                            target_size=213, fields=0, fields_key='relocation_fields')
    if (received['target_instructions'] != 67 or received['candidate_instructions'] != 67
            or received['target_calls'] != 0 or received['candidate_calls'] != 0
            or received['target_blocks'] != 9 or received['candidate_blocks'] != 9
            or not received['complete_direct_graphs_agree']):
        raise RuntimeError('Supervisor received-frame no-call/67-instruction/9-block gate changed')
    exact = []
    if protect_exact:
        build('supervisor-frame-queue-insert-predicted')
        exact.append(canonical('supervisor-frame-queue-insert-predicted'))
        build('supervisor-frame-queue-pop-frame')
        exact.append(canonical('supervisor-frame-queue-pop-frame'))
    return {'supervisor_received_frame': {
                'target_bytes': 213, 'candidate_bytes': received['candidate_bytes'],
                'ordinary_differences': received['complete_differences'],
                'relocations': received['relocation_fields'],
                'direct_graphs_agree': received['complete_direct_graphs_agree'],
           }}, exact


def run_camera_facing(*, protect_exact: bool) -> tuple[dict, list[dict]]:
    # The target-owned camera-facing method now compiles in the actual Render
    # translation unit, immediately after the already-exact TranslateRotation.
    # Protect this callee and the exact Render callers from TU-visibility drift.
    build('anm-manager-translate-rotation')
    candidate = noncrediting(parsed(['scripts/inspect-anm-camera-facing.py',
                                     'build/matching/AnmManagerRender.obj']),
                             name='Camera-facing projection boss',
                             target_size=622, fields=40)
    if (candidate.get('candidate_bytes') != 622 or
            candidate.get('target_instructions') != 201 or
            candidate.get('candidate_instructions') != 201 or
            candidate.get('original_fields') != 40 or
            candidate.get('ordered_calls') != 11 or
            not candidate.get('calls_agree') or
            candidate.get('original_blocks') != 8 or
            candidate.get('candidate_blocks') != 8 or
            not candidate.get('direct_graphs_agree') or
            candidate.get('overlap_differences', 999) > 24):
        raise RuntimeError('camera 622-byte/40-field/11-call/8-block frontier regressed')
    siblings = []
    if protect_exact:
        siblings += [canonical('anm-manager-translate-rotation'),
                     canonical('anm-manager-draw2d'),
                     canonical('anm-manager-draw-camera-facing-quad')]
        build('anm-manager-project-3d-quad')
        siblings.append(canonical('anm-manager-project-3d-quad'))
    return {'camera_facing_projection': {
        'target_bytes': 622, 'candidate_bytes': candidate['candidate_bytes'],
        'bound_fields': candidate['bound_fields'],
        'overlap_differences': candidate['overlap_differences'],
        'missing_target_bytes': candidate['missing_target_bytes'],
        'direct_graphs_agree': candidate['direct_graphs_agree'],
    }}, siblings




def run_replay_playback(*, protect_exact: bool) -> tuple[dict, list[dict]]:
    """Cold-audit a full replay-stage callback and its two exact TU siblings.

    The 32 original fields must be actual SHA-pinned PE operands, resolved
    through canonical referents, not guessed source-local labels. The lone
    side-state load motion and two absent source bytes are nonexact frontiers.
    """
    build('replay-manager-begin-recording-stage')
    current = noncrediting(parsed(['scripts/inspect-replay-begin-playback.py',
                                    'build/matching/ReplayInputNoEh.obj', '--json']),
                           name='Replay BeginPlaybackStage', target_size=540,
                           fields=32, fields_key='bound_fields')
    if (current.get('original_fields') != 32 or
            current.get('target_instructions') != 152 or
            not 538 <= current.get('candidate_bytes', 0) <= 540 or
            not 151 <= current.get('candidate_instructions', 0) <= 155 or
            current.get('ordered_calls') != 5 or
            not current.get('ordered_calls_agree') or
            current.get('target_blocks') != 12 or
            current.get('candidate_blocks') != 12 or
            not current.get('complete_direct_graphs_agree') or
            current.get('aligned_field_identities', -1) < 31 or
            current.get('complete_overlap_differences', 999) > 49 or
            current.get('missing_target_bytes', 999) > 2 or
            current.get('extra_source_bytes') != 0 or
            current.get('complete_bytes_equal')):
        raise RuntimeError('Replay playback 540-byte/32-field/5-call/12-block frontier regressed')
    exact = []
    if protect_exact:
        # Both use the exact SAME freshly-built no-EH VC7.1 ReplayInput object.
        exact += [canonical('replay-manager-begin-recording-stage'),
                  canonical('replay-manager-create')]
    return {'replay_begin_playback': {
        'target_bytes': 540, 'candidate_bytes': current['candidate_bytes'],
        'bound_fields': current['bound_fields'],
        'aligned_fields': current['aligned_field_identities'],
        'ordinary_differences': current['complete_overlap_differences'],
        'missing_target_bytes': current['missing_target_bytes'],
        'direct_graphs_agree': current['complete_direct_graphs_agree'],
    }}, exact


def run_screen_shake(*, protect_exact: bool) -> tuple[dict, list[dict]]:
    """Cold-source complete ScreenEffect envelope plus exact same-TU owners."""
    build('screen-effect-calc-shake')
    current = noncrediting(parsed(['scripts/inspect-screen-shake-envelope.py',
                                    'build/matching/ScreenEffect.obj', '--json']),
                           name='ScreenEffect CalcShakeEnvelope',
                           target_size=336, fields=20,
                           fields_key='bound_relocations')
    if (current.get('candidate_bytes') != 336 or
            current.get('reviewed_target_fields') != 20 or
            current.get('target_instructions') != 132 or
            current.get('candidate_instructions') != 132 or
            current.get('target_calls') != 8 or
            current.get('candidate_calls') != 8 or
            not current.get('ordered_calls_agree') or
            current.get('target_blocks') != 26 or
            current.get('candidate_blocks') != 26 or
            not current.get('complete_direct_graphs_agree') or
            current.get('absent_target_bytes') != 0 or
            current.get('extra_candidate_bytes') != 0 or
            not isinstance(current.get('overlapping_differences'), int) or
            current['overlapping_differences'] > 28 or
            current.get('complete_bytes_agree')):
        raise RuntimeError('ScreenEffect 336-byte/20-field/8-call/26-block nonexact frontier drifted')
    exact = []
    if protect_exact:
        # All six use the one freshly compiled ScreenEffect.obj and profile.
        for unit in ('screen-effect-calc-shake', 'screen-effect-calc-fade-in',
                     'screen-effect-calc-fade-out', 'screen-effect-calc-fade-hold',
                     'screen-effect-calc-arcade-pulse', 'screen-effect-initialize-timer'):
            exact.append(canonical(unit))
    return {'screen_shake_envelope': {
        'target_bytes': 336, 'candidate_bytes': current['candidate_bytes'],
        'bound_fields': current['bound_relocations'],
        'ordered_calls': current['candidate_calls'],
        'direct_graphs_agree': current['complete_direct_graphs_agree'],
        'ordinary_differences': current['overlapping_differences'],
    }}, exact


def run_effect(*, protect_exact: bool) -> tuple[dict, list[dict]]:
    # The 51-byte resource loader shares the live EffectManager TU with both
    # canonical 475-byte OnUpdate and 183-byte OnDraw. Compile that original
    # source TU once, then bind its nonexact loader independently from both
    # complete, previously exact sibling functions.
    build('effect-manager-update')
    diagnostic = noncrediting(parsed(['scripts/inspect-effect-added.py',
                                      'build/matching/EffectManager.obj', '--json'],
                                     diagnostic=True),
                              name='EffectManager AddedCallback',
                              target_size=51, fields=4,
                              fields_key='bound_relocations')
    if (diagnostic.get('candidate_bytes') != 51 or
            not isinstance(diagnostic.get('differing_bytes'), int) or
            diagnostic['differing_bytes'] > 8 or
            diagnostic.get('complete_bytes_equal') is not False or
            diagnostic.get('target_instructions') != 19 or
            diagnostic.get('candidate_instructions') != 19 or
            diagnostic.get('target_relocations') != 4 or
            diagnostic.get('original_blocks') != 3 or
            diagnostic.get('candidate_blocks') != 3 or
            not diagnostic.get('complete_direct_graphs_agree') or
            not diagnostic.get('ordered_calls_match')):
        raise RuntimeError('EffectManager added-loader 19-instruction/4-field/3-block/call gate failed')
    exact = []
    if protect_exact:
        exact.extend((canonical('effect-manager-update'),
                      canonical('effect-manager-draw'),
                      canonical('effect-manager-draw-shared')))
    return {'effect_added_callback': {
        'target_bytes': 51, 'candidate_bytes': diagnostic['candidate_bytes'],
        'original_fields': diagnostic['target_relocations'],
        'bound_fields': diagnostic['bound_relocations'],
        'ordinary_differences': diagnostic['differing_bytes'],
        'ordered_calls_agree': diagnostic['ordered_calls_match'],
    }}, exact


def run_game_manager(*, protect_exact: bool) -> tuple[dict, list[dict]]:
    """Independently cold-validate the newly exact 1,230-byte calc boss.

    Nine original owners use the pinned /O2 /Os profile, but the exact
    Supervisor loading VMs sibling intentionally uses the no-/Os profile.
    A single cached COFF object or a profile swap invalidates the other
    group: force each actual profile to compile its own complete source TU.
    """
    primary = ('game-manager-on-update',
               'game-manager-advance-timed-state',
               'game-manager-added-callback',
               'game-manager-deleted-callback',
               'game-manager-register-chain',
               'game-manager-on-draw',
               'game-manager-reset-runtime',
               'game-manager-cut-chain',
               'game-manager-format-current-date')
    build('game-manager-on-update')
    owners = [canonical('game-manager-on-update')]
    # This 1,689-byte worker must use *the same /Os build* as the newly
    # accepted large callback. The no-/Os supervisor capture profile yields
    # an unrelated 1,812-byte candidate and private switch symbols.
    gameplay = noncrediting(parsed(['scripts/inspect-gameplay-setup.py',
                                    'build/matching/GameManagerSetup.obj', '--json']),
                           name='Gameplay setup thread', target_size=1689,
                           fields=171)
    if (gameplay.get('candidate_bytes') != 1689 or
            gameplay.get('target_instructions') != 413 or
            gameplay.get('original_fields') != 172 or
            gameplay.get('ordered_calls') != 35 or
            gameplay.get('candidate_calls') != 35 or
            not gameplay.get('ordered_calls_agree') or
            gameplay.get('original_blocks') != 78 or
            gameplay.get('candidate_blocks') != 77 or
            gameplay.get('aligned_field_identities') != 155):
        raise RuntimeError('GameplaySetupThread original 172/171 field/35 call/78-block proof drifted')
    diagnostics = {'gameplay_setup_thread': {
        'target_bytes': 1689, 'candidate_bytes': gameplay['candidate_bytes'],
        'original_fields': gameplay['original_fields'],
        'bound_fields': gameplay['bound_fields'],
        'ordered_calls': gameplay['ordered_calls'],
        'original_blocks': gameplay['original_blocks'],
        'candidate_blocks': gameplay['candidate_blocks'],
        'field_change_spans': gameplay['field_change_spans'],
        'direct_graphs_agree': gameplay['direct_graphs_agree'],
    }}
    cleanup = noncrediting(parsed(['scripts/inspect-game-manager-cleanup.py',
                                   'build/matching/GameManagerSetup.obj']),
                           name='GameManager gameplay cleanup',
                           target_size=425, fields=41)
    if (cleanup.get('candidate_bytes') != 406 or
            cleanup.get('target_instructions') != 117 or
            cleanup.get('candidate_instructions') != 114 or
            cleanup.get('original_fields') != 43 or
            cleanup.get('ordered_calls') != 12 or
            not cleanup.get('ordered_calls_agree') or
            cleanup.get('original_blocks') != 24 or
            cleanup.get('candidate_blocks') != 25 or
            cleanup.get('mode2_lower_bound_branch_present') is not True or
            cleanup.get('aligned_field_identities') != 40 or
            cleanup.get('field_change_spans') != 3 or
            cleanup.get('aligned_instructions') < 104 or
            cleanup.get('overlap_differences') > 209 or
            cleanup.get('direct_graphs_agree')):
        raise RuntimeError('CleanupGameplayState mode-2 clamp / 406-byte original-bound frontier regressed')
    diagnostics['game_manager_cleanup'] = {
        'target_bytes': 425,
        'candidate_bytes': cleanup['candidate_bytes'],
        'original_fields': cleanup['original_fields'],
        'bound_fields': cleanup['bound_fields'],
        'aligned_field_identities': cleanup['aligned_field_identities'],
        'aligned_instructions': cleanup['aligned_instructions'],
        'overlap_differences': cleanup['overlap_differences'],
        'mode2_lower_bound_branch_present': cleanup['mode2_lower_bound_branch_present'],
        'direct_graphs_agree': cleanup['direct_graphs_agree'],
    }
    if protect_exact:
        owners.extend(canonical(name) for name in primary[1:])
        build('supervisor-setup-loading-vms-and-capture')
        owners.append(canonical('supervisor-setup-loading-vms-and-capture'))
    return diagnostics, owners


def run_enemy(*, protect_exact: bool) -> tuple[dict, list[dict]]:
    # The 3,900-byte enemy calc owner and 151-byte exact attached-effects
    # helper share the actual EnemyManagerCore TU. The collision append leaf
    # is a separate 82-byte owner invoked by the calc callback twice.
    build('enemy-attached-effect-update')
    main = noncrediting(parsed(['scripts/inspect-enemy-manager-update.py',
                                'build/matching/EnemyManagerCore.obj', '--json']),
                        name='EnemyManager OnUpdate', target_size=3900,
                        fields=97, fields_key='relocations')
    if (main.get('candidate_bytes') != 3900 or
            main.get('target_instructions') != 1026 or
            main.get('candidate_instructions') != 1026 or
            not isinstance(main.get('complete_differences'), int) or
            main['complete_differences'] > 39 or main.get('complete_bytes_agree') or
            main.get('relocation_fields_agree') != 97 or
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
    if (result.get('candidate_bytes') != 600 or
            result.get('target_instructions') != 157 or
            not isinstance(result.get('candidate_instructions'), int) or
            result['candidate_instructions'] > 160 or
            result.get('ordered_calls') != 13 or
            result.get('target_direct_blocks') != 17 or
            result.get('candidate_direct_blocks') != 17 or
            result.get('complete_direct_graphs_agree') is not True or
            not isinstance(result.get('overlapping_byte_differences'), int) or
            result['overlapping_byte_differences'] > 268 or
            result.get('missing_target_bytes') != 0 or
            result.get('extra_candidate_bytes') != 0 or
            result.get('complete_bytes_equal') is not False):
        raise RuntimeError('Etama OnDraw 600-byte/27-field/13-call/17-block frontier regressed')
    exact_units = [canonical('bullet-draw-single')] if protect_exact else []
    if protect_exact:
        build('etama-controller-update')
        exact_units.append(canonical('etama-controller-update'))
    return {'etama_draw': {
        'target_bytes': 600, 'candidate_bytes': result['candidate_bytes'],
        'overlapping_differences': result['overlapping_byte_differences'],
        'direct_graphs_agree': result['complete_direct_graphs_agree'],
    }}, exact_units


def run_exattack_type19_update(*, protect_exact: bool) -> tuple[dict, list[dict]]:
    # This complete 821-byte/35-field update callback is *not* in the ECL
    # translation unit. Compare its true independent source and protect the
    # exact Type19/21 draw callback that consumes the same physical Extra.
    obj = 'build/matching/ExAttackUpdateType19.obj'
    build_nonexact('src/ExAttackUpdateType19.cpp', obj,
                   reference_unit='exattack-type19-21-draw')
    result = noncrediting(parsed(['scripts/inspect-exattack-type19.py',
                                  obj, '--json'], diagnostic=True),
                          name='ExAttack Type19 update', target_size=821,
                          fields=35, fields_key='candidate_fields')
    if (result.get('target_fields') != 35 or
            result.get('target_instructions') != 254 or
            result.get('candidate_instructions') != 254 or
            result.get('target_calls') != 11 or
            result.get('candidate_calls') != 11 or
            result.get('target_direct_blocks') != 30 or
            result.get('candidate_direct_blocks') != 30 or
            result.get('distinct_player_slot_fields') != 4 or
            not result.get('complete_direct_graphs_agree')):
        raise RuntimeError('Type19 update 35-field/11-call/30-block/slot owner drifted')
    siblings = []
    if protect_exact:
        build('exattack-type19-21-draw')
        siblings.append(canonical('exattack-type19-21-draw'))
    return {'exattack_type19_update': {
        'target_bytes': 821, 'candidate_bytes': result['candidate_bytes'],
        'ordinary_differences': result['overlapping_differences'],
        'bound_fields': result['candidate_fields'],
        'ordered_fields_agree': result['ordered_fields_agree'],
        'ordered_calls': result['candidate_calls'],
        'direct_graphs_agree': result['complete_direct_graphs_agree'],
    }}, siblings


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


def guard_title_draw_result(result: dict) -> None:
    """Fail closed on regression of the 1,939-byte original renderer boss.

    This is a non-crediting *whole-owner* guard, not a local instruction
    similarity threshold or a promotion to canonical exact.  The producer
    independently binds every original source field and all 16 call identities.
    """
    noncrediting(result, name='Title DrawResult', target_size=1939, fields=82)
    if (result.get('candidate_bytes') != 1939 or
            result.get('target_instructions') != 577 or
            result.get('candidate_instructions') != 577 or
            result.get('original_fields') != 82 or
            result.get('ordered_calls') != 16 or
            not result.get('ordered_calls_agree') or
            result.get('original_blocks') != 92 or
            result.get('candidate_blocks') != 92 or
            not result.get('direct_graphs_agree') or
            not isinstance(result.get('aligned_instructions'), int) or
            result['aligned_instructions'] < 574 or
            result.get('paired_branch_conflicts') != [] or
            result.get('unpaired_branch_destinations', 2) > 1 or
            not isinstance(result.get('complete_differences'), int) or
            result['complete_differences'] > 22 or
            result.get('complete_bytes_equal') is not False):
        raise RuntimeError('Title DrawResult 1939-byte/82-field/16-call/92-block frontier drifted')


def run_title_open() -> dict:
    # These source-present owners are explicitly non-exact. Their independent
    # original operand inventories must agree before any similarity score.
    # The cursor trio is in the same TU as the exact TitleSetup thread; rebuild
    # it even with --skip-exact-siblings so a stale object cannot be reused.
    build('title-screen-setup-thread')
    cursor_obj = 'build/matching/TitleScreen.obj'
    cursor_reports = {}
    for variant, size, field_count, calls in (
            ('active', 211, 6, 4), ('reverse', 175, 2, 2),
            ('inactive', 175, 2, 2)):
        result = noncrediting(parsed(['scripts/inspect-title-character-cursors.py',
                                      '--variant', variant, cursor_obj]),
                              name='Title cursor ' + variant,
                              target_size=size, fields=field_count)
        if result['original_fields'] != field_count or result['ordered_calls'] != calls or not result['ordered_calls_agree']:
            raise RuntimeError('Title cursor original field/call census drifted')
        cursor_reports['title_cursor_' + variant] = {
            'target_bytes': size, 'candidate_bytes': result['candidate_bytes'],
            'bound_fields': result['bound_fields'],
            'overlap_differences': result['overlap_differences'],
            'extra_candidate_bytes': result['extra_candidate_bytes'],
            'direct_graphs_agree': result['direct_graphs_agree'],
        }
    update = noncrediting(parsed(['scripts/inspect-title-update-result.py', cursor_obj]),
                          name='Title OnUpdateResult', target_size=485, fields=34)
    if (update['original_fields'] != 34 or update['ordered_calls'] != 9 or
            not update['calls_agree'] or update['original_blocks'] != 40 or
            not update['direct_graphs_agree']):
        raise RuntimeError('Title OnUpdateResult 34-field/9-call/40-block audit failed')
    cursor_reports['title_update_result'] = {
        'target_bytes': 485, 'candidate_bytes': update['candidate_bytes'],
        'bound_fields': update['bound_fields'],
        'overlap_differences': update['overlap_differences'],
        'missing_target_bytes': update['missing_target_bytes'],
        'direct_graphs_agree': update['direct_graphs_agree'],
    }
    # The 2,045-byte Options update is a separate 121-block boss, not part of
    # the smaller TitleSetup exact receipts; never reuse a previously built
    # ignored COFF object without recompiling its current tracked source.
    options_obj = 'build/matching/TitleScreenOptions.obj'
    build_nonexact('src/TitleScreenOptions.cpp', options_obj,
                   reference_unit='title-screen-setup-thread')
    options = noncrediting(parsed(['scripts/inspect-title-options.py',
                                  options_obj, '--json']),
                           name='Title OnUpdateOptions', target_size=2045, fields=135)
    if (options['original_fields'] != 135 or options['target_instructions'] != 551 or
            options['ordered_calls'] != 51 or not options['ordered_calls_agree'] or
            options['original_blocks'] != 121 or
            not options['direct_graphs_agree']):
        raise RuntimeError('Title Options original 135-field/51-call/121-block identity regressed')
    cursor_reports['title_update_options'] = {
        'target_bytes': 2045, 'candidate_bytes': options['candidate_bytes'],
        'bound_fields': options['bound_fields'],
        'ordered_calls': options['ordered_calls'],
        'target_instructions': options['target_instructions'],
        'candidate_instructions': options['candidate_instructions'],
        'missing_target_bytes': options['missing_target_bytes'],
        'extra_candidate_bytes': options['extra_candidate_bytes'],
        'direct_graphs_agree': options['direct_graphs_agree'],
    }
    result_obj = 'build/matching/TitleResultDraw.obj'
    build_nonexact('src/TitleScreenResultDraw.cpp', result_obj,
                   reference_unit='title-screen-setup-thread')
    draw = noncrediting(parsed(['scripts/inspect-title-result-draw.py', result_obj, '--json']),
                        name='Title DrawResult', target_size=1939, fields=82)
    guard_title_draw_result(draw)
    replay_obj = 'build/matching/TitleReplaySave.obj'
    build_nonexact('src/TitleScreenReplaySaveDraw.cpp', replay_obj,
                   reference_unit='title-screen-setup-thread')
    replay = noncrediting(parsed(['scripts/inspect-title-replay-save-draw.py', replay_obj,
                                  '--json']), name='Title DrawReplaySave',
                          target_size=800, fields=41)
    if replay['original_fields'] != 41 or not replay['ordered_calls_agree']:
        raise RuntimeError('Title ReplaySave 41 original fields or call sequence regressed')
    return {**cursor_reports, 'title_draw_result': {
                'target_bytes': 1939, 'candidate_bytes': draw['candidate_bytes'],
                'ordinary_differences': draw['complete_differences'],
                'fields': draw['bound_fields'],
                'direct_graphs_agree': draw['direct_graphs_agree']},
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
        build('title-draw-callback')
        exact_units.append(canonical('title-draw-callback'))
    # Existing matches stay independent: none of these receipts establishes
    # the still-unresolved TitleScreen DrawResult/Options/ReplaySave owners.
    return {}, exact_units


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--owner', choices=['runecl', 'enemy', 'player', 'gamemgr', 'crypto', 'effect', 'etama', 'exattack', 'type3', 'type19', 'title', 'anm', 'network', 'parser', 'front', 'screen', 'replay', 'all'],
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
        if args.owner in ('type19', 'exattack', 'all'):
            owners, exact = run_exattack_type19_update(protect_exact=not args.skip_exact_siblings)
            report['owners'].update(owners)
            report['canonical_siblings'].extend(exact)
        if args.owner in ('type3', 'exattack', 'all'):
            owners, exact = run_exattack_type3(protect_exact=not args.skip_exact_siblings)
            report['owners'].update(owners)
            report['canonical_siblings'].extend(exact)
        if args.owner in ('crypto', 'all'):
            owners, exact = run_decrypt_table(protect_exact=not args.skip_exact_siblings)
            report['owners'].update(owners)
            report['canonical_siblings'].extend(exact)
        if args.owner in ('network', 'all'):
            owners, exact = run_received_frames(protect_exact=not args.skip_exact_siblings)
            report['owners'].update(owners)
            report['canonical_siblings'].extend(exact)
        if args.owner in ('parser', 'all'):
            owners, exact = run_network_config_parser(protect_exact=not args.skip_exact_siblings)
            report['owners'].update(owners)
            report['canonical_siblings'].extend(exact)
        if args.owner in ('front', 'all'):
            owners, exact = run_front_random(protect_exact=not args.skip_exact_siblings)
            report['owners'].update(owners)
            report['canonical_siblings'].extend(exact)
        if args.owner in ('anm', 'all'):
            owners, exact = run_camera_facing(protect_exact=not args.skip_exact_siblings)
            report['owners'].update(owners)
            report['canonical_siblings'].extend(exact)
        if args.owner in ('screen', 'all'):
            owners, exact = run_screen_shake(protect_exact=not args.skip_exact_siblings)
            report['owners'].update(owners)
            report['canonical_siblings'].extend(exact)
        if args.owner in ('replay', 'all'):
            owners, exact = run_replay_playback(protect_exact=not args.skip_exact_siblings)
            report['owners'].update(owners)
            report['canonical_siblings'].extend(exact)
        if args.owner in ('player', 'all'):
            owners, exact = run_player_collision(protect_exact=not args.skip_exact_siblings)
            report['owners'].update(owners)
            report['canonical_siblings'].extend(exact)
        if args.owner in ('gamemgr', 'all'):
            owners, exact = run_game_manager(protect_exact=not args.skip_exact_siblings)
            report['owners'].update(owners)
            report['canonical_siblings'].extend(exact)
        if args.owner in ('enemy', 'all'):
            owners, exact = run_enemy(protect_exact=not args.skip_exact_siblings)
            report['owners'].update(owners)
            report['canonical_siblings'].extend(exact)
        if args.owner in ('effect', 'all'):
            owners, exact = run_effect(protect_exact=not args.skip_exact_siblings)
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
