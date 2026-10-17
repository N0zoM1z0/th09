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
from collections import Counter
import re
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


# These offsets belong to the independently reviewed full original physical
# 15,564-byte RunEcl owner, not a standalone 148-handler reconstruction.
# The two residuals are 7 ordinary bytes of opcode 4 and 23 of opcode 155.
RUNECL_TARGET_OWNER_SHA = 'e9433504219119eba40ed4d11537782178135effeb6c95d74b6274e0e7f5e7ea'
RUNECL_WINDOWS = {4: (0x277, 29, 7, 8), 155: (0x30B0, 45, 23, 12)}


def validate_runecl_byte_frontier(complete: dict, shapes: dict) -> dict:
    """Reject byte/code/table/handler regressions, granting zero exact credit.

    A successful diagnostic may remove existing differences or finish a handler.
    Only canonical source-commit-bound exact replay can accept the whole owner.
    """
    noncrediting(complete, name='RunEcl physical original',
                 target_size=15564, fields=598,
                 size_key='physical_bytes', fields_key='relocations')
    noncrediting(shapes, name='RunEcl independently reviewed handlers',
                 target_size=14792, fields=148,
                 size_key='target_code_bytes', fields_key='unique_handlers')
    if (complete.get('target_owner_sha256') != RUNECL_TARGET_OWNER_SHA or
            complete.get('identity_audit_passed') is not True or
            shapes.get('identity_audit_passed') is not True or
            shapes.get('candidate_code_bytes') != 14792 or
            complete.get('field_different_bytes') != 0 or
            complete.get('table_different_bytes') != 0):
        raise RuntimeError('RunEcl original identity/code alignment/fields/table regressed')
    differences = complete.get('differences')
    amount = complete.get('ordinary_different_bytes')
    if (not isinstance(amount, int) or isinstance(amount, bool) or
            amount < 0 or amount > 30 or
            complete.get('different_bytes') != amount or
            not isinstance(differences, list) or len(differences) != amount):
        raise RuntimeError('RunEcl full physical residual increased or is incomplete')
    # A zero-byte residual is still a non-crediting diagnostic. Check that
    # original-bound byte equality agrees with the complete residual count.
    if complete.get('complete_bytes_equal') is not (amount == 0):
        raise RuntimeError('RunEcl full physical equality disagrees with residual bytes')
    seen = set()
    counts = {4: 0, 155: 0}
    for entry in differences:
        if not isinstance(entry, dict):
            raise RuntimeError('RunEcl residual contains an unknown record')
        offset, actual, original = (entry.get('offset'), entry.get('candidate'),
                                    entry.get('target'))
        if (not isinstance(offset, int) or isinstance(offset, bool) or
                offset in seen or not isinstance(actual, int) or
                not isinstance(original, int) or
                not (0 <= actual <= 255 and 0 <= original <= 255) or
                actual == original or entry.get('relocation_field') is not False):
            raise RuntimeError('RunEcl byte differences are duplicated/unreviewed')
        seen.add(offset)
        slots = [opcode for opcode, (begin, size, _, _) in RUNECL_WINDOWS.items()
                 if begin <= offset < begin + size]
        if len(slots) != 1:
            raise RuntimeError('RunEcl changed an already matched physical opcode')
        counts[slots[0]] += 1
    for opcode, (_, _, ceiling, _) in RUNECL_WINDOWS.items():
        if counts[opcode] > ceiling:
            raise RuntimeError('RunEcl opcode %d residual byte frontier regressed' % opcode)
    mismatches = shapes.get('mismatches')
    if (not isinstance(mismatches, list) or
            shapes.get('mismatching_handler_shapes') != len(mismatches) or
            shapes.get('matching_handler_shapes') != 148 - len(mismatches) or
            shapes.get('matching_handler_shapes', 0) < 146):
        raise RuntimeError('RunEcl complete 148-handler shape census regressed')
    seen_opcodes = set()
    for mismatch in mismatches:
        if not isinstance(mismatch, dict):
            raise RuntimeError('RunEcl unreviewed handler mismatch record')
        opcode = mismatch.get('opcode')
        if opcode not in RUNECL_WINDOWS or opcode in seen_opcodes:
            raise RuntimeError('RunEcl regressed a previously matching handler')
        seen_opcodes.add(opcode)
        begin, size, _, instrs = RUNECL_WINDOWS[opcode]
        if (mismatch.get('candidate_offset') != begin or
                mismatch.get('target_offset') != begin or
                mismatch.get('candidate_bytes') != size or
                mismatch.get('target_bytes') != size or
                mismatch.get('candidate_instructions') != instrs or
                mismatch.get('target_instructions') != instrs or
                mismatch.get('matching_instructions', 0) < 6 or
                mismatch.get('exact_shape') is not False):
            raise RuntimeError('RunEcl opcode physical extent or instruction audit regressed')
    for opcode in RUNECL_WINDOWS:
        if (opcode in seen_opcodes) != (counts[opcode] > 0):
            raise RuntimeError('RunEcl byte residual and opcode shape disagree')
    return {'remaining_opcode_mismatches': sorted(seen_opcodes),
            'ordinary_differences': amount,
            'differences_by_opcode': counts,
            'matching_handler_shapes': shapes['matching_handler_shapes']}


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
        shapes = parsed(['scripts/report-ecl-handler-shapes.py', '--json', object_file])
        verified_runecl = validate_runecl_byte_frontier(complete, shapes)
        mismatches = verified_runecl['remaining_opcode_mismatches']
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
            'ordinary_differences_by_opcode': verified_runecl['differences_by_opcode'],
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
    # The two exact original target ranges were independently read from TH09
    # v1.50a. Each current residual is exclusively an adjacent setup-schedule
    # permutation (Type19: vertex-add/stack-store; Type21: vertex-add/history-LEA).
    # The window is a strict no-regression bound, not a source patch or credit.
    specs = {
        '19': (763, 207, 57, 27, 9, 0x1BB, 0x1C1,
               'fc7b630cff17c32048de02f169f5c39710727027941aa502db823e357563cf67'),
        '21': (727, 198, 53, 25, 10, 0x1C2, 0x1CB,
               'b5271179d169d482952d887b03ad08e10a19c74654590e32598551d0a523ca47'),
    }
    if callback_type not in specs:
        raise RuntimeError('unsupported ExAttack trail initializer type')
    size, instructions, relocations, calls, blocks, first, end, target_sha = specs[callback_type]
    offsets = result.get('difference_offsets')
    if (not isinstance(offsets, list) or
            len(offsets) > end - first or
            any(not isinstance(off, int) or isinstance(off, bool) or
                not first <= off < end for off in offsets) or
            len(set(offsets)) != len(offsets) or
            offsets != sorted(offsets)):
        raise RuntimeError('ExAttack Type' + callback_type +
                           ' initializer changed a previously matching physical byte')
    if (result.get('diagnostic_only') is not True or
            result.get('exactness_credit') != 'none' or
            result.get('target_sha256') != target_sha or
            result.get('target_bytes') != size or
            result.get('candidate_bytes') != size or
            result.get('target_instructions') != instructions or
            result.get('candidate_instructions') != instructions or
            result.get('relocations') != relocations or
            result.get('target_calls') != calls or
            result.get('candidate_calls') != calls or
            result.get('ordered_calls_agree') is not True or
            result.get('direct_blocks') != [blocks, blocks] or
            result.get('complete_direct_graphs_agree') is not True or
            not isinstance(result.get('normalized_aligned_instructions'), int) or
            result['normalized_aligned_instructions'] < instructions - 1 or
            result.get('complete_differences_if_same_extent') != len(offsets) or
            result.get('complete_bytes_agree') is not (len(offsets) == 0)):
        raise RuntimeError('ExAttack Type' + callback_type +
                           ' initializer full-owner 57/53-field/6/9-byte frontier regressed')


PLAYER_MOVEMENT_TARGET_SHA = '64ea64884e978540c718d1691a4ae1b9866aaa8ec2296bafb158c4d8b1c628ca'


def guard_player_movement(result: dict) -> dict:
    """Check the complete 1,900-byte Player boss, not an aligned excerpt.

    This non-crediting bound preserves all original relocation/table identities
    and the source's verified 144 -> 129 byte improvement. Source-cold work
    may narrow the residual further, but similarity does not confer exactness.
    """
    noncrediting(result, name='Player movement boss', target_size=1835,
                 fields=66, size_key='target_code_bytes', fields_key='relocations')
    changed = result.get('difference_offsets')
    count = result.get('full_difference_count')
    if (result.get('target_extent_sha256') != PLAYER_MOVEMENT_TARGET_SHA or
            result.get('target_physical_bytes') != 1900 or
            result.get('candidate_code_bytes') != 1835 or
            result.get('candidate_physical_bytes') != 1900 or
            result.get('candidate_post_return_alignment_bytes') != 1 or
            result.get('candidate_table_bytes') != 64 or
            result.get('target_instructions') != 489 or
            result.get('candidate_instructions') != 489 or
            result.get('internal_relocations') != 18 or
            result.get('direct_calls') != 23 or
            result.get('direct_call_order_agrees') is not True or
            result.get('candidate_operand_fields_cover_actual_records') is not True or
            result.get('field_identity_occurrences_agree') is not True or
            not isinstance(result.get('normalized_aligned_instructions'), int) or
            result['normalized_aligned_instructions'] < 433 or
            type(count) is not int or not 0 <= count <= 129 or
            not isinstance(changed, list) or len(changed) != count or
            any(type(x) is not int or not 0 <= x < 1900 for x in changed) or
            changed != sorted(set(changed)) or
            result.get('missing_target_bytes') != 0 or
            result.get('extra_candidate_bytes') != 0 or
            result.get('full_byte_agreement') is not (count == 0)):
        raise RuntimeError('Player movement complete physical owner, 66 fields or 129-byte frontier regressed')
    return {'target_bytes': 1900, 'candidate_bytes': 1900,
            'ordinary_differences': count, 'fields': 66,
            'calls': 23, 'aligned_instructions': result['normalized_aligned_instructions']}


def run_player_movement(*, protect_exact: bool) -> tuple[dict, list[dict]]:
    obj = 'build/matching/PlayerMovement.obj'
    build_nonexact('src/PlayerMovement.cpp', obj, reference_unit='player-on-update')
    report = parsed(['scripts/inspect-player-movement.py', obj])
    owner = guard_player_movement(report)
    exact = []
    if protect_exact:
        build('player-on-update')
        exact.append(canonical('player-on-update'))
    return {'player_movement': owner}, exact


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
    """Cold-replay the NEW exact 220-byte crypt selector and its exact siblings.

    Canonical full-extent/relocation comparison grants the exact proof. The
    separately original-SHA-pinned interpreter is a redundant *noncrediting*
    target+source instruction, address-operand, three-return and CFG audit.
    Both must agree; neither a locally equal diagnostic nor a prior object
    alone can make this owner exact.
    """
    build('file-system-try-decrypt-table')
    selector = canonical('file-system-try-decrypt-table')
    obj = 'build/matching/FileDecryptTable.obj'
    result = noncrediting(parsed(['scripts/inspect-file-decrypt-table.py',
                                  obj, '--json']),
                          name='FileSystem exact decryption-table selector audit',
                          target_size=220, fields=12)
    if (result.get('target_sha256') !=
            'c9195eb6365a7e3d6905b3e2cead12ed480c4fd9ad968992db2b397e9b6fd8a7' or
            result.get('candidate_bytes') != 220 or
            result.get('target_instructions') != 77 or
            result.get('candidate_instructions') != 77 or
            result.get('ordered_calls') != 2 or
            result.get('target_blocks') != 11 or
            result.get('candidate_blocks') != 11 or
            result.get('complete_direct_graphs_agree') is not True or
            result.get('complete_overlap_differences') != 0 or
            result.get('missing_target_bytes') != 0 or
            result.get('extra_candidate_bytes') != 0 or
            result.get('complete_bytes_equal') is not True or
            selector['logical_bytes'] != 220 or
            selector['physical_bytes'] != 220 or
            selector['relocations'] != 12):
        raise RuntimeError('TH09 FileSystem exact crypt-table 220-byte/12-field owner regressed')
    exact = [selector]
    if protect_exact:
        build('file-system-decrypt')
        exact.append(canonical('file-system-decrypt'))
        build('file-system-encrypt')
        exact.append(canonical('file-system-encrypt'))
    return {'file_decrypt_table': {
        'target_bytes': 220,
        'candidate_bytes': 220,
        'exact': True,
        'ordinary_differences': 0,
        'bound_fields': 12,
        'ordered_calls': 2,
        'target_blocks': 11,
        'candidate_blocks': 11,
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
    if (owner.get('candidate_bytes',0) < 681 or
            owner['candidate_bytes'] > 693 or
            owner.get('target_instructions') != 206 or
            not isinstance(owner.get('candidate_instructions'),int) or
            not 201 <= owner['candidate_instructions'] <= 206 or
            owner.get('original_fields') != 15 or
            owner.get('ordered_calls') != 6 or
            owner.get('ordered_calls_agree') is not True or
            owner.get('target_blocks') != 40 or
            owner.get('candidate_blocks') not in (40,41) or
            owner.get('direct_graphs_agree') is not (owner['candidate_blocks']==40) or
            not isinstance(owner.get('aligned_instructions'),int) or
            owner['aligned_instructions'] < 118 or
            not isinstance(owner.get('overlap_differences'),int) or
            owner['overlap_differences'] > 569 or
            owner.get('missing_target_bytes',99) > 12 or
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


# Original-PE bound: source's typed three-vector workspace now reproduces
# every camera-facing byte except this aggregate-return copy scheduling span.
# All earlier stack-frame, local-home and call operand differences are closed.
CAMERA_FACING_TARGET_SHA = '36fd70db08a5570fe316d9e3beb541f7a39a90ef10b1429ad767e09c973f1865'
CAMERA_FACING_RESIDUAL_OFFSETS = frozenset((
    0xE9, 0xEA, 0xEB, 0xED, 0xEE, 0xF0, 0xF1, 0xF2,
    0xF3, 0xF4, 0xF5, 0xF6, 0xF7))


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
            candidate.get('target_sha256') != CAMERA_FACING_TARGET_SHA or
            candidate.get('missing_target_bytes') != 0 or
            candidate.get('extra_candidate_bytes') != 0):
        raise RuntimeError('camera 622-byte/40-field/11-call/8-block original owner regressed')
    differences = candidate.get('difference_offsets')
    amount = candidate.get('overlap_differences')
    if (type(amount) is not int or not isinstance(differences, list) or
            len(differences) != amount or amount > 13 or
            any(type(offset) is not int or
                offset not in CAMERA_FACING_RESIDUAL_OFFSETS for offset in differences) or
            differences != sorted(set(differences)) or
            candidate.get('complete_bytes_equal') is not (amount == 0)):
        raise RuntimeError('camera typed-vector 13-byte original-bound copy frontier regressed')
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


SCREEN_SHAKE_ORIGINAL_SHA256 = (
    '1d33957d51f8cc170471b9673a938899281cfa07fbed746eb0843e62e2597472'
)
# The complete 26 unlike bytes after reconstructing the real fade/hold
# declaration lifetime, authenticated against the private retail PE. This
# is a RESIDUAL SET, NOT source code or an accepted partial exactness score.
# A valid future source improvement may remove mismatches from this set, but
# may not introduce mismatches at any of the 310 already matching positions.
SCREEN_SHAKE_RESIDUAL_OFFSETS = (
    0x00B, 0x00D, 0x012, 0x018, 0x01A, 0x01F, 0x026, 0x02A, 0x036,
    0x044, 0x05B, 0x05E, 0x066, 0x085, 0x092, 0x095, 0x098, 0x0A4,
    0x109, 0x115, 0x123, 0x12D, 0x13B, 0x142, 0x147, 0x14B,
)


def guard_screen_shake_frontier(diagnostic: dict) -> dict:
    """Fail closed on EVERY newly mismatching original-linked byte.

    This audit binds the entire source COFF function and all actual Japanese
    TH09 v1.50a PE call/data destinations; zero local differences never
    substitute for a source-commit-bound canonical exact Oracle receipt.
    """
    noncrediting(diagnostic, name='ScreenEffect CalcShakeEnvelope',
                 target_size=336, fields=20, fields_key='bound_relocations')
    actual = diagnostic.get('difference_offsets')
    count = diagnostic.get('overlapping_differences')
    if (diagnostic.get('target_sha256') != SCREEN_SHAKE_ORIGINAL_SHA256 or
            len(SCREEN_SHAKE_RESIDUAL_OFFSETS) != 26 or
            not isinstance(actual, list) or
            type(count) is not int or count > 26 or count < 0 or
            len(actual) != count or
            any(type(i) is not int or not 0 <= i < 336 for i in actual) or
            actual != sorted(set(actual)) or
            not set(actual).issubset(SCREEN_SHAKE_RESIDUAL_OFFSETS) or
            diagnostic.get('candidate_bytes') != 336 or
            diagnostic.get('target_instructions') != 132 or
            diagnostic.get('candidate_instructions') != 132 or
            diagnostic.get('reviewed_target_fields') != 20 or
            diagnostic.get('target_calls') != 8 or
            diagnostic.get('candidate_calls') != 8 or
            diagnostic.get('ordered_calls_agree') is not True or
            diagnostic.get('target_blocks') != 26 or
            diagnostic.get('candidate_blocks') != 26 or
            diagnostic.get('complete_direct_graphs_agree') is not True or
            diagnostic.get('absent_target_bytes') != 0 or
            diagnostic.get('extra_candidate_bytes') != 0 or
            diagnostic.get('complete_bytes_agree') is not (count == 0)):
        raise RuntimeError('ScreenEffect 336-byte/20-field/8-call/26-block/26-residual original frontier regressed')
    return diagnostic


def run_screen_shake(*, protect_exact: bool) -> tuple[dict, list[dict]]:
    """Cold-source complete ScreenEffect envelope plus exact same-TU owners."""
    build('screen-effect-calc-shake')
    current = noncrediting(parsed(['scripts/inspect-screen-shake-envelope.py',
                                    'build/matching/ScreenEffect.obj', '--json']),
                           name='ScreenEffect CalcShakeEnvelope',
                           target_size=336, fields=20,
                           fields_key='bound_relocations')
    guard_screen_shake_frontier(current)
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


EFFECT_ADDED_ORIGINAL_SHA = 'ddad6b720c7c7d539d71ce27f74660951ebb6abb17fb58a616752a91b5760087'
# Complete, target-bound *unmatched* byte spans of the 51-byte owner:
# +0x03..+0x09 (seven bytes), +0x0D (one byte). They describe the genuine
# ANM global-load/receiver instruction schedule, not relocation destinations.
EFFECT_ADDED_RESIDUAL_WINDOWS = ((0x03, 0x0A, 7), (0x0D, 0x0E, 1))


def guard_effect_added(diagnostic: dict) -> dict:
    """Reject newly broken bytes in the original owner, not just 8-count.

    The producer SHA-attests the retail TH09 PE and resolves every real
    relocation/call field independently of the candidate. This remains
    strictly non-crediting even if a future build has zero remaining bytes.
    """
    noncrediting(diagnostic, name='EffectManager AddedCallback',
                 target_size=51, fields=4, fields_key='bound_relocations')
    if (diagnostic.get('target_sha256') != EFFECT_ADDED_ORIGINAL_SHA or
            diagnostic.get('candidate_bytes') != 51 or
            diagnostic.get('target_instructions') != 19 or
            diagnostic.get('candidate_instructions') != 19 or
            diagnostic.get('target_relocations') != 4 or
            diagnostic.get('original_blocks') != 3 or
            diagnostic.get('candidate_blocks') != 3 or
            diagnostic.get('complete_direct_graphs_agree') is not True or
            diagnostic.get('ordered_calls_match') is not True or
            type(diagnostic.get('differing_bytes')) is not int or
            diagnostic['differing_bytes'] > 8):
        raise RuntimeError('EffectManager 51-byte/4-field/2-call original-bound frontier drifted')
    return physical_difference_windows(
        diagnostic, count_key='differing_bytes',
        windows=EFFECT_ADDED_RESIDUAL_WINDOWS,
        equality_key='complete_bytes_equal')


def run_effect(*, protect_exact: bool) -> tuple[dict, list[dict]]:
    # The 51-byte resource loader shares the live EffectManager TU with both
    # canonical 475-byte OnUpdate and 183-byte OnDraw. Compile that original
    # source TU once, then bind its nonexact loader independently from both
    # complete, previously exact sibling functions.
    build('effect-manager-update')
    diagnostic = parsed(['scripts/inspect-effect-added.py',
                         'build/matching/EffectManager.obj', '--json'],
                        diagnostic=True)
    guard_effect_added(diagnostic)
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


GAMEPLAY_SETUP_TARGET_SHA = '8dc5dd11495f34c6b1998653e0094195d6d891fff374d403750141e9936f0d6a'
# Bit i is 1 iff the cold VC7.1 candidate still differs at original PE
# physical offset i. This is an independently reviewed, SOURCE-BOUND
# NON-CREDITING residual mask; it does not contain or patch target bytes.
# A future improvement may clear bits, never change already equal bytes.
GAMEPLAY_SETUP_RESIDUAL_MASK = bytes.fromhex(
    '0000000000000000000000000000000000000000000000000000000000000000'
    '0000000000000000000000000000000000000000000000000000000800000010'
    'c0fffffffffffffffffffefffffffffffffffffffffffffffffffffdffffffff'
    'ffffffffffbfffffffffffffffffffffffff7ffffffffffffeffeffffffddf9f'
    '20088078eeffffffffffffbffffffffffffddfffffffffbfffffffffff3df7f7'
    '7ffff73fffbffffffffb0f0000000000000000000080e3fffffff7bdef7defff'
    'ffffffffff170000000000000000000000000000'
)


def guard_gameplay_setup_thread(result: dict) -> None:
    """Fail closed against regressions outside the 1,689-byte original.

    The entire target/code owner, ALL original relocation/call identities,
    branch graph and every source-linked residual byte are checked upstream by
    inspect-gameplay-setup.py. Neither a close match nor zero local residuals
    bypasses the separate canonical, source-commit-bound exactness workflow.
    """
    noncrediting(result, name='Gameplay setup thread', target_size=1689, fields=171)
    differences = result.get('difference_offsets')
    count = result.get('overlap_differences')
    if (result.get('target_sha256') != GAMEPLAY_SETUP_TARGET_SHA or
            len(GAMEPLAY_SETUP_RESIDUAL_MASK) != 212 or
            sum(bin(x).count('1') for x in GAMEPLAY_SETUP_RESIDUAL_MASK) != 911 or
            result.get('candidate_bytes') != 1689 or
            result.get('target_instructions') != 413 or
            result.get('candidate_instructions') != 416 or
            result.get('original_fields') != 172 or
            result.get('ordered_calls') != 35 or
            result.get('candidate_calls') != 35 or
            result.get('ordered_calls_agree') is not True or
            result.get('original_blocks') != 78 or
            result.get('candidate_blocks') != 77 or
            result.get('direct_graphs_agree') is not False or
            type(result.get('aligned_field_identities')) is not int or
            result['aligned_field_identities'] < 157 or
            type(result.get('field_change_spans')) is not int or
            not 0 <= result['field_change_spans'] <= 15 or
            type(result.get('aligned_instructions')) is not int or
            result['aligned_instructions'] < 394 or
            type(count) is not int or not 0 <= count <= 911 or
            result.get('complete_bytes_equal') is not (count == 0) or
            not isinstance(differences, list) or len(differences) != count or
            any(type(i) is not int or not 0 <= i < 1689
                for i in differences) or
            differences != sorted(set(differences)) or
            any(not (GAMEPLAY_SETUP_RESIDUAL_MASK[i // 8] & (1 << (i % 8)))
                for i in differences)):
        raise RuntimeError('GameplaySetupThread 1689-byte/171-field/35-call/911-residual frontier drifted')


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
    guard_gameplay_setup_thread(gameplay)
    diagnostics = {'gameplay_setup_thread': {
        'target_bytes': 1689, 'candidate_bytes': gameplay['candidate_bytes'],
        'original_fields': gameplay['original_fields'],
        'bound_fields': gameplay['bound_fields'],
        'ordered_calls': gameplay['ordered_calls'],
        'original_blocks': gameplay['original_blocks'],
        'candidate_blocks': gameplay['candidate_blocks'],
        'field_change_spans': gameplay['field_change_spans'],
        'aligned_field_identities': gameplay['aligned_field_identities'],
        'ordinary_differences': gameplay['overlap_differences'],
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


# Independent original SHA-256 values for the full physical owner boundaries.
ENEMY_UPDATE_SHA256 = 'a7f01c2ff0bda79d1fd17ac9170dc3560d4089457455d7928776df8556f1bc75'
ENEMY_COLLISION_SHA256 = 'e2be799860dde7b752f525cb13f4135c4666917745006a511e734e10cd2ff9c0'
ENEMY_UPDATE_WINDOWS = ((0x19A, 0x1A8, 2), (0x251, 0x2C1, 11),
                        (0x843, 0x854, 0), (0x9BC, 0xA83, 16))
ENEMY_COLLISION_WINDOWS = ((0x1A, 0x40, 31),)


def physical_difference_windows(result: dict, *, count_key: str, windows: tuple,
                                equality_key: str = "complete_bytes_agree") -> dict:
    """Reject changed matched regions without granting partial/exact credit."""
    words = result.get('difference_offsets')
    count = result.get(count_key)
    if (type(count) is not int or not isinstance(words, list) or
            len(words) != count or not all(
                isinstance(word, str) and re.fullmatch(r'0x[0-9a-fA-F]+', word)
                for word in words)):
        raise RuntimeError('Physical owner residual count/format malformed')
    offsets = [int(word, 16) for word in words]
    if offsets != sorted(set(offsets)) or count > sum(x[2] for x in windows):
        raise RuntimeError('Physical owner residual duplicated, unsorted or increased')
    per_window = [0] * len(windows)
    for off in offsets:
        locations = [i for i, (start, end, _) in enumerate(windows)
                     if start <= off < end]
        if len(locations) != 1:
            raise RuntimeError('Physical owner residual changed previously matched code')
        per_window[locations[0]] += 1
    if any(actual > window[2] for actual, window in zip(per_window, windows)):
        raise RuntimeError('Physical owner regressed a reviewed original window')
    if result.get(equality_key) is not (count == 0):
        raise RuntimeError('Physical owner full equality/count contradictory')
    return {'differences': count, 'per_window': per_window}


def guard_enemy_update(result: dict) -> dict:
    noncrediting(result, name='EnemyManager OnUpdate', target_size=3900,
                 fields=97, fields_key='relocations')
    if (result.get('target_sha256') != ENEMY_UPDATE_SHA256 or
            result.get('candidate_bytes') != 3900 or
            result.get('code_bytes') != 3883 or
            result.get('target_instructions') != 1026 or
            result.get('candidate_instructions') != 1026 or
            result.get('relocation_fields_agree') != 97 or
            result.get('target_calls') != 69 or
            result.get('candidate_calls') != 69 or
            result.get('ordered_calls_agree') is not True or
            result.get('target_direct_blocks') != 170 or
            result.get('candidate_direct_blocks') != 170 or
            result.get('reviewed_control_flow_agrees') is not True or
            result.get('switch_table_destinations_agree') is not True):
        raise RuntimeError('EnemyManager 3900-byte original/COFF/call/graph changed')
    return physical_difference_windows(result, count_key='complete_differences',
                                    windows=ENEMY_UPDATE_WINDOWS)


def guard_enemy_collision(result: dict) -> dict:
    noncrediting(result, name='Enemy collision append leaf', target_size=82,
                 fields=2, fields_key='bound_call_fields')
    if (result.get('target_sha256') != ENEMY_COLLISION_SHA256 or
            result.get('candidate_bytes') != 82 or
            result.get('target_instructions') != 30 or
            result.get('candidate_instructions') != 30 or
            result.get('ordered_call_addresses') != ['0x40f5a0', '0x40f8e0']):
        raise RuntimeError('Enemy append collision original SHA/ABI/calls changed')
    # This owner is canonical exact through the genuine AppendBoxRecord
    # definition visible in the shared EnemyManagerCore translation unit. Preserve the independent non-crediting original-byte diagnostic,
    # but never allow it to regress to the earlier 31-difference source.
    if (result.get('ordinary_differing_bytes') != 0 or
            result.get('complete_bytes_agree') is not True):
        raise RuntimeError('canonical Enemy collision wrapper byte equality regressed')
    return physical_difference_windows(result, count_key='ordinary_differing_bytes',
                                    windows=ENEMY_COLLISION_WINDOWS)


def run_enemy(*, protect_exact: bool) -> tuple[dict, list[dict]]:
    # One source-cold EnemyManagerCore TU now owns the nonexact 3,900-byte
    # calc boss, exact 151-byte effects leaf, original-exact 56-byte Float3
    # division, and both exact 205/82-byte collision helpers. The real
    # Float3 divider + AppendBoxRecord bodies eliminate ten genuine target
    # mismatches inside the caller; canonical replays protect all four leaves.
    build('enemy-attached-effect-update')
    main = parsed(['scripts/inspect-enemy-manager-update.py',
                   'build/matching/EnemyManagerCore.obj', '--json'])
    main_frontier = guard_enemy_update(main)

    # Use the same freshly compiled EnemyManagerCore.obj for every source-
    # commit-relevant codegen check; a historical PlayerCollisionAppendBox
    # object is no longer an independent source of authority.
    obj = 'build/matching/EnemyManagerCore.obj'
    collision = parsed(['scripts/inspect-enemy-collision-box.py', obj, '--json'])
    collision_frontier = guard_enemy_collision(collision)
    exact = [canonical('enemy-attached-effect-update')] if protect_exact else []
    if protect_exact:
        exact.extend((canonical('float3-divide'),
                      canonical('player-collision-append-box'),
                      canonical('enemy-collision-append-box')))
        build('enemy-manager-draw-impl')
        exact.append(canonical('enemy-manager-draw-impl'))
    return {'enemy_manager_update': {
                'target_bytes': 3900,
                'candidate_bytes': main['candidate_bytes'],
                'bound_fields': main['relocations'],
                'ordinary_differences': main_frontier['differences'],
                'ordered_calls': main['candidate_calls'],
                'direct_graphs_agree': main['reviewed_control_flow_agrees'],
                'switch_destinations_agree': main['switch_table_destinations_agree']},
            'enemy_collision_append': {
                'target_bytes': 82,
                'candidate_bytes': collision['candidate_bytes'],
                'ordinary_differences': collision_frontier['differences'],
                'ordered_calls': collision['ordered_call_addresses'],
                'bound_fields': collision['bound_call_fields']}}, exact


# These are the 32 disjoint, *physical* original-v1.50a byte runs that
# differ in the currently cold-compiled 600-byte Etama::OnDraw. The aggregate
# count alone is insufficient: a rewrite could replace a formerly exact byte
# with a new mismatch while preserving the same count. This is a source-bound
# no-regression ceiling, never an acceptance/partial-exactness allowance.
ETAMA_DRAW_ORIGINAL_SHA = '6216f7ba73f38523afb6c40385b2b74190a657dc0388b8b8192d65d7f7e8f6b1'
ETAMA_DRAW_RESIDUAL_RUNS = (
    (0x028, 0x02A), (0x02E, 0x032), (0x034, 0x03A), (0x03B, 0x040),
    (0x041, 0x042), (0x04A, 0x04B), (0x04F, 0x050), (0x062, 0x064),
    (0x068, 0x06A), (0x06D, 0x06E), (0x07A, 0x07C), (0x087, 0x088),
    (0x0D0, 0x0D2), (0x0E4, 0x0E6), (0x0FD, 0x0FF), (0x10A, 0x10B),
    (0x110, 0x112), (0x11A, 0x11C), (0x122, 0x123), (0x127, 0x149),
    (0x14A, 0x14C), (0x14D, 0x165), (0x166, 0x17B), (0x182, 0x184),
    (0x18B, 0x18E), (0x190, 0x1A4), (0x1A6, 0x202), (0x203, 0x216),
    (0x217, 0x21F), (0x23C, 0x23D), (0x23F, 0x240), (0x245, 0x246),
)


def guard_etama_draw(result: dict) -> None:
    """Protect the complete original 600-byte Laser renderer frontier.

    The inspector cold-binds all 27 original COFF operands and 13 real calls;
    this second gate keeps all 332 *already matching* physical bytes matched.
    A hypothetical 0-byte diagnostic still gives zero exactness credit.
    """
    noncrediting(result, name='Etama OnDraw', target_size=600, fields=27)
    offsets = result.get('difference_offsets')
    if (result.get('target_sha256') != ETAMA_DRAW_ORIGINAL_SHA or
            result.get('candidate_bytes') != 600 or
            result.get('target_instructions') != 157 or
            type(result.get('candidate_instructions')) is not int or
            not 157 <= result['candidate_instructions'] <= 160 or
            result.get('ordered_calls') != 13 or
            result.get('target_direct_blocks') != 17 or
            result.get('candidate_direct_blocks') != 17 or
            result.get('complete_direct_graphs_agree') is not True or
            result.get('missing_target_bytes') != 0 or
            result.get('extra_candidate_bytes') != 0 or
            type(result.get('overlapping_byte_differences')) is not int or
            not 0 <= result['overlapping_byte_differences'] <= 268 or
            not isinstance(offsets, list) or
            len(offsets) != result['overlapping_byte_differences'] or
            result.get('complete_bytes_equal') is not (len(offsets) == 0)):
        raise RuntimeError('Etama OnDraw original 600-byte full-owner census regressed')
    if (any(type(off) is not int or off < 0 or off >= 600 or
            not any(lo <= off < hi for lo, hi in ETAMA_DRAW_RESIDUAL_RUNS)
            for off in offsets) or
            offsets != sorted(set(offsets))):
        raise RuntimeError('Etama OnDraw changed previously matching physical bytes')


def run_etama(*, protect_exact: bool) -> tuple[dict, list[dict]]:
    build('bullet-draw-single')
    result = noncrediting(parsed(['scripts/inspect-etama-draw.py',
                                 'build/matching/EtamaControllerDraw.obj', '--json'],
                                diagnostic=True), name='Etama OnDraw',
                          target_size=600, fields=27)
    guard_etama_draw(result)
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


TYPE3_UPDATE_SHA256 = '19acde6cfdea958c572992f6832e09f51630ce2508f4d983eba4f3cb87e05eb4'
TYPE3_MIDDLE_Z_ORIGINAL = {
    'middle_y_call_offset': '0x18e',
    'middle_z_store_offset': '0x196',
    'tail_x_load_offset': '0x19d',
    'classification': 'matches-original-chronology',
}


def classify_type3_z_chronology(order: dict) -> str:
    """Recheck actual instruction offsets; a claimed label is not evidence.

    The full source-cold inspector computes these offsets from decoded real
    instructions. Recompute here so manufactured or stale diagnostic JSON
    cannot pass the standalone frontier guard by changing only its label.
    """
    names = ('middle_y_call_offset', 'middle_z_store_offset',
             'tail_x_load_offset')
    if (not isinstance(order, dict) or set(order) != set(names) | {'classification'}
            or not all(isinstance(order[name], str) and
                       re.fullmatch(r'0x[0-9a-f]+', order[name]) for name in names)):
        raise RuntimeError('Type3 Y-call / middle-Z / tail-X offset evidence malformed')
    y, z, tail = (int(order[name], 16) for name in names)
    if (len({y, z, tail}) != 3 or
            not all(0 <= offset < 780 for offset in (y, z, tail))):
        raise RuntimeError('Type3 middle-Z chronology has duplicate/out-of-owner offsets')
    if y < z < tail:
        observed = 'matches-original-chronology'
    elif z < y < tail:
        observed = 'zero-before-middle-y-call'
    elif y < tail < z:
        observed = 'zero-after-tail-x-load'
    else:
        observed = 'unreviewed-order'
    if order.get('classification') != observed:
        raise RuntimeError('Type3 claimed middle-Z classification disagrees with offsets')
    return observed


def guard_exattack_type3_update(result: dict) -> dict:
    """Protect all 780 bytes and the target's post-Y/pre-tail-X Z store.

    Current natural source expresses the original Y-then-Z statement order,
    but pinned VC7.1 hoists the *tail X load* before the Z store. The remaining
    15 bytes are non-exact. Do not accept the superficially closer old variant
    that puts the zero *before the real middle-Y call*.
    """
    noncrediting(result, name='ExAttack Type3 update', target_size=780, fields=38)
    original = result.get('original_z_order')
    candidate = result.get('candidate_z_order')
    target_class = classify_type3_z_chronology(original)
    candidate_class = classify_type3_z_chronology(candidate)
    if (result.get('target_sha256') != TYPE3_UPDATE_SHA256 or
            result.get('candidate_bytes') != 780 or
            result.get('target_instructions') != 240 or
            result.get('candidate_instructions') != 240 or
            result.get('target_fields') != 38 or
            result.get('ordered_calls') != 24 or
            result.get('calls_agree') is not True or
            result.get('target_blocks') != 17 or
            result.get('candidate_blocks') != 17 or
            result.get('direct_graphs_agree') is not True or
            original != TYPE3_MIDDLE_Z_ORIGINAL or
            target_class != 'matches-original-chronology' or
            candidate['middle_y_call_offset'] != '0x18e' or
            candidate_class not in ('zero-after-tail-x-load',
                                    'matches-original-chronology') or
            (result.get('complete_bytes_equal') is True and
             candidate_class != 'matches-original-chronology')):
        raise RuntimeError('ExAttack Type3 original identity, full graph or Z chronology changed')
    return physical_difference_windows(
        result, count_key='ordinary_differences', windows=((0x196, 0x1A8, 15),),
        equality_key='complete_bytes_equal')


def run_exattack_type3(*, protect_exact: bool) -> tuple[dict, list[dict]]:
    # Separate TU from the known exact Type3 initializer: build its *current*
    # maintained update source before linking the full 38-field diagnostic.
    object_file = 'build/matching/ExAttackUpdateType3.obj'
    build_nonexact('src/ExAttackUpdateType3.cpp', object_file,
                   reference_unit='ex-attack-initialize-type3')
    result = parsed(['scripts/inspect-exattack-type3-schedule.py', object_file])
    validated = guard_exattack_type3_update(result)
    exact = []
    if protect_exact:
        build('ex-attack-initialize-type3')
        exact.append(canonical('ex-attack-initialize-type3'))
    return {'exattack_type3_update': {
        'target_bytes': 780, 'candidate_bytes': result['candidate_bytes'],
        'ordinary_differences': validated['differences'],
        'original_z_order': result['original_z_order']['classification'],
        'candidate_z_order': result['candidate_z_order']['classification'],
        'direct_graphs_agree': result['direct_graphs_agree'],
    }}, exact


TITLE_DRAW_RESULT_SHA256 = '92279dc6d2b9ad5051fb902e6172d88eb7f9febe460d44459f16af0f45c74b0a'
TITLE_DRAW_RESULT_RESIDUAL_WINDOWS = ((0x0E, 0x25, 21), (0x39, 0x3A, 1))


def guard_title_draw_result(result: dict) -> None:
    """Fail closed on regression of the 1,939-byte original renderer boss.

    This is a non-crediting *whole-owner* guard, not a local instruction
    similarity threshold or a promotion to canonical exact.  The producer
    independently binds every original source field and all 16 call identities.
    """
    noncrediting(result, name='Title DrawResult', target_size=1939, fields=82)
    if (result.get('target_sha256') != TITLE_DRAW_RESULT_SHA256 or
            result.get('candidate_bytes') != 1939 or
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
            type(result.get('complete_differences')) is not int or
            not 0 <= result['complete_differences'] <= 22):
        raise RuntimeError('Title DrawResult 1939-byte/82-field/16-call/92-block frontier drifted')
    # Noncrediting future narrowing is allowed only within these *actual*
    # unbound source-mismatched original byte positions. Preserve the entire
    # remaining 1,939-byte owner, including source COFF fields and jump graph.
    physical_difference_windows(
        result, count_key='complete_differences',
        windows=TITLE_DRAW_RESULT_RESIDUAL_WINDOWS,
        equality_key='complete_bytes_equal')


def guard_title_update_result(result: dict) -> None:
    """Guard the complete 485-byte original and the 484-byte source frontier.

    This remains non-crediting, even if all the bytes converge in a later build.
    The producer independently SHA-pins the original and validates all 34
    source relocations, nine calls, decoded instructions and direct CFG blocks.
    """
    noncrediting(result, name='Title OnUpdateResult', target_size=485, fields=34)
    source_size = result.get('candidate_bytes')
    mismatch = result.get('overlap_differences')
    if (type(source_size) is not int or not 484 <= source_size <= 485 or
            result.get('target_instructions') != 147 or
            result.get('candidate_instructions') != 147 or
            result.get('original_fields') != 34 or
            result.get('ordered_calls') != 9 or
            result.get('calls_agree') is not True or
            result.get('original_blocks') != 40 or
            result.get('candidate_blocks') != 40 or
            result.get('direct_graphs_agree') is not True or
            type(mismatch) is not int or not 0 <= mismatch <= 218 or
            result.get('missing_target_bytes') != 485 - source_size or
            result.get('extra_candidate_bytes') != 0 or
            result.get('complete_bytes_equal') is not
            (source_size == 485 and mismatch == 0)):
        raise RuntimeError('Title OnUpdateResult 485-byte/34-field/9-call/40-block frontier drifted')


def guard_title_options(result: dict) -> None:
    """Guard all 2,045 original bytes, 135 operands and 51 ordered calls.

    The current 2,048-byte candidate has 610 overlapping differences. A
    complete exact candidate is allowed through, without awarding exact credit.
    """
    noncrediting(result, name='Title OnUpdateOptions', target_size=2045, fields=135)
    source_size = result.get('candidate_bytes')
    mismatch = result.get('overlap_differences')
    aligned = result.get('aligned_instructions')
    unpaired = result.get('unpaired_branch_destinations')
    if (type(source_size) is not int or not 2045 <= source_size <= 2048 or
            result.get('target_instructions') != 551 or
            type(result.get('candidate_instructions')) is not int or
            not 551 <= result['candidate_instructions'] <= 554 or
            result.get('original_fields') != 135 or
            result.get('ordered_calls') != 51 or
            result.get('ordered_calls_agree') is not True or
            result.get('original_blocks') != 121 or
            result.get('candidate_blocks') != 121 or
            result.get('direct_graphs_agree') is not True or
            type(aligned) is not int or not 540 <= aligned <= 551 or
            result.get('paired_branch_conflicts') != [] or
            type(unpaired) is not int or not 0 <= unpaired <= 5 or
            type(mismatch) is not int or not 0 <= mismatch <= 610 or
            result.get('missing_target_bytes') != 0 or
            result.get('extra_candidate_bytes') != source_size - 2045 or
            result.get('complete_bytes_equal') is not
            (source_size == 2045 and mismatch == 0)):
        raise RuntimeError('Title OnUpdateOptions 2045-byte/135-field/51-call/121-block frontier drifted')


TITLE_REPLAY_SAVE_TARGET_SHA256 = (
    '1d2ba588efb25a86d2e04da91812d3026a78a666effca5cc4738797e8d5b463c'
)


def guard_title_replay_save(result: dict) -> None:
    """Keep the 800-byte original replay renderer's recovered direct CFG.

    This is a complete-owner original-bound diagnostic, NOT canonical exact
    credit. Only source-commit-bound Factory acceptance may grant that.
    """
    noncrediting(result, name='Title DrawReplaySave 800-byte owner',
                 target_size=800, fields=41)
    size = result.get('candidate_bytes')
    if (result.get('target_sha256') != TITLE_REPLAY_SAVE_TARGET_SHA256 or
            type(size) is not int or not 799 <= size <= 800 or
            result.get('target_instructions') != 245 or
            result.get('candidate_instructions') != 245 or
            result.get('original_fields') != 41 or
            result.get('ordered_calls') != 4 or
            result.get('ordered_calls_agree') is not True or
            result.get('original_blocks') != 28 or
            result.get('candidate_blocks') != 28 or
            result.get('direct_graphs_agree') is not True or
            result.get('paired_branch_conflicts') != [] or
            type(result.get('unpaired_branch_destinations')) is not int or
            not 0 <= result['unpaired_branch_destinations'] <= 5 or
            type(result.get('aligned_instructions')) is not int or
            not 173 <= result['aligned_instructions'] <= 245 or
            result.get('complete_bytes_equal') is not
                    (size == 800 and result.get('complete_differences') == 0) or
            (size == 799 and (result.get('complete_differences') is not None or
                              result.get('difference_offsets') is not None)) or
            (size == 800 and
                (type(result.get('complete_differences')) is not int or
                 not 0 <= result['complete_differences'] <= 800 or
                 not isinstance(result.get('difference_offsets'), list) or
                 len(result['difference_offsets']) != result['complete_differences']))):
        raise RuntimeError('Title DrawReplaySave recovered 799/245/41/4/28 frontier drifted')


TITLE_REPLAY_MENU_TARGET_SHA256 = (
    'd0201b3aae3727f7aa773187ac9ac6d21727c86342b657c780cb9353cdc39ed0'
)


def guard_title_replay_menu(result: dict) -> None:
    """Protect the original 1,280-byte Title replay-menu renderer.

    Complete retail-PE SHA, each of the 57 original address operands,
    13 ordered calls and direct graph are decoded by the independent
    inspector. This local gate never grants canonical exactness credit.
    """
    noncrediting(result, name='Title DrawReplayMenu 1280-byte owner',
                 target_size=1280, fields=57)
    size = result.get('candidate_bytes')
    diffs = result.get('difference_offsets')
    count = result.get('overlap_differences')
    aligned = result.get('aligned_instructions')
    candidate_blocks = result.get('candidate_blocks')
    if (result.get('target_sha256') != TITLE_REPLAY_MENU_TARGET_SHA256 or
            type(size) is not int or not 1258 <= size <= 1280 or
            result.get('target_instructions') != 376 or
            type(result.get('candidate_instructions')) is not int or
            not 368 <= result['candidate_instructions'] <= 376 or
            result.get('original_fields') != 57 or
            result.get('original_calls') != 13 or
            result.get('candidate_calls') != 13 or
            result.get('ordered_calls_agree') is not True or
            result.get('original_blocks') != 72 or
            type(result.get('direct_graphs_agree')) is not bool or
            type(candidate_blocks) is not int or
            not 70 <= candidate_blocks <= 72 or
            (result.get('direct_graphs_agree') is True and candidate_blocks != 72) or
            type(aligned) is not int or not 238 <= aligned <= 376 or
            type(count) is not int or not 0 <= count <= 1187 or
            not isinstance(diffs, list) or len(diffs) != count or
            any(type(i) is not int or not 0 <= i < size for i in diffs) or
            diffs != sorted(set(diffs)) or
            result.get('missing_target_bytes') != 1280 - size or
            result.get('extra_candidate_bytes') != 0 or
            result.get('complete_bytes_equal') is not
                    (size == 1280 and count == 0)):
        raise RuntimeError(
            'Title DrawReplayMenu 1280-byte/57-field/13-call/72-block frontier drifted')


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
    update = parsed(['scripts/inspect-title-update-result.py', cursor_obj])
    guard_title_update_result(update)
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
    options = parsed(['scripts/inspect-title-options.py', options_obj, '--json'])
    guard_title_options(options)
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
    menu_obj = 'build/matching/TitleScreenReplayMenuDraw.obj'
    build_nonexact('src/TitleScreenReplayMenuDraw.cpp', menu_obj,
                   reference_unit='title-screen-setup-thread')
    menu = parsed(['scripts/inspect-title-replay-menu-draw.py', menu_obj,
                   '--json'])
    guard_title_replay_menu(menu)
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
    guard_title_replay_save(replay)
    return {**cursor_reports, 'title_draw_replay_menu': {
                'target_bytes': 1280,
                'candidate_bytes': menu['candidate_bytes'],
                'bound_fields': menu['bound_fields'],
                'ordered_calls': menu['candidate_calls'],
                'original_blocks': menu['original_blocks'],
                'candidate_blocks': menu['candidate_blocks'],
                'direct_graphs_agree': menu['direct_graphs_agree'],
                'aligned_instructions': menu['aligned_instructions'],
                'missing_target_bytes': menu['missing_target_bytes']},
            'title_draw_result': {
                'target_bytes': 1939, 'candidate_bytes': draw['candidate_bytes'],
                'ordinary_differences': draw['complete_differences'],
                'fields': draw['bound_fields'],
                'direct_graphs_agree': draw['direct_graphs_agree']},
            'title_draw_replay_save': {
                'target_bytes': 800, 'candidate_bytes': replay['candidate_bytes'],
                'fields': replay['bound_fields'],
                'first_overlapping_difference': replay['first_overlapping_difference'],
                'candidate_instructions': replay['candidate_instructions'],
                'aligned_instructions': replay['aligned_instructions'],
                'ordered_calls': replay['ordered_calls'],
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


PAUSE_SHA256 = 'a4cee1fd2b347628d97a2786ccc238b87e21489ab440c9d0b2f0e27d41f7f9b7'
PAUSE_TARGET_TABLE = (
    '0x43481b', '0x434881', '0x43499a', '0x434acc',
    '0x434b54', '0x434c58', '0x434b54', '0x434c58',
    '0x434d74', '0x434d9a',
)
PAUSE_CALL_CENSUS = {
    '0x40e2d0': 14, '0x43e2f0': 12, '0x4010b0': 8,
    '0x434370': 2, '0x415d20': 2, '0x436f30': 2,
    '0x403e00': 1, '0x436ac0': 1, '0x434380': 1,
    '0x423498': 1, '0x4343b0': 1, '0x42fd30': 1,
}
PAUSE_EXACT_SIBLINGS = (
    'anm-vm-is-visible', 'anm-vm-set-invisible',
    'ascii-menu-state4-update', 'ascii-menu-state5-draw',
    'ascii-menu-state4-draw', 'ascii-manager-draw-low-prio',
    'ascii-manager-draw-high-prio', 'ascii-manager-register-startup',
    'ascii-manager-draw-high-prio-impl', 'ascii-game-manager-has-flag-bit0',
    'ascii-manager-on-update',
)


def guard_pause_menu(result: dict) -> dict:
    """Noncrediting cold audit of code, alignment and 10-table physical owner."""
    target, source = result.get('target'), result.get('candidate')
    if (result.get('diagnostic_only') is not True or
            result.get('exactness_credit') != 'none' or
            result.get('target_owner_sha256') != PAUSE_SHA256 or
            not isinstance(target, dict) or not isinstance(source, dict)):
        raise RuntimeError('PauseMenu original physical identity absent')
    calls = target.get('ordered_direct_calls')
    physical, missing = result.get('physical_bytes'), result.get('missing_bytes')
    diffs = result.get('overlap_differences')
    table = source.get('table_destinations')
    if (target.get('code_bytes') != 1734 or
            target.get('alignment_hex') != '8bff' or
            target.get('table_offset') != 1736 or
            target.get('instructions') != 449 or
            len(target.get('fields', [])) != 106 or
            Counter(x[1] for x in target['fields']) != {'DIR32': 60, 'REL32': 46} or
            target.get('table_destinations') != list(PAUSE_TARGET_TABLE) or
            len(target.get('internal_branches', [])) != 52 or
            type(physical) is not int or not 1732 <= physical <= 1776 or
            source.get('table_offset') != physical - 40 or
            not 1692 <= source.get('code_bytes', 0) <= 1734 or
            source.get('alignment_hex') not in ('', '8bff', '90', '9090') or
            source.get('table_offset') != source.get('code_bytes', -1) +
                len(bytes.fromhex(source.get('alignment_hex', ''))) or
            not 449 <= source.get('instructions', 0) <= 454 or
            not 106 <= len(result.get('fields', [])) <= 107 or
            len(source.get('fields', [])) != len(result.get('fields', [])) or
            not isinstance(table, list) or len(table) != 10 or
            table[4] != table[6] or table[5] != table[7] or
            len(set(table)) != 8 or
            not isinstance(calls, list) or Counter(calls) != PAUSE_CALL_CENSUS or
            Counter(source.get('ordered_direct_calls', [])) != PAUSE_CALL_CENSUS or
            not 52 <= len(source.get('internal_branches', [])) <= 54 or
            type(diffs) is not int or not 0 <= diffs <= 1453 or
            type(missing) is not int or missing != 1776 - physical or
            result.get('excess_bytes') != 0 or
            result.get('complete_equal') is not (diffs == 0 and missing == 0)):
        raise RuntimeError('PauseMenu source code/table/field/call frontier regressed')
    # Unlike an aggregate similarity score, every linked physical byte
    # residual must refer to a distinct location in the compared owner.
    residual = result.get('differences')
    if not isinstance(residual, list) or len(residual) != diffs:
        raise RuntimeError('PauseMenu physical differences incomplete')
    previous = -1
    for entry in residual:
        if not isinstance(entry, dict):
            raise RuntimeError('PauseMenu malformed byte evidence')
        offset, actual, expected = (entry.get('offset'), entry.get('candidate'),
                                    entry.get('target'))
        if (type(offset) is not int or not previous < offset < physical or
                type(actual) is not int or type(expected) is not int or
                not 0 <= actual <= 255 or not 0 <= expected <= 255 or
                actual == expected):
            raise RuntimeError('PauseMenu duplicated, foreign or equal byte record')
        previous = offset
    return {'target_bytes': 1776, 'candidate_bytes': physical,
            'candidate_code_bytes': source['code_bytes'],
            'bound_fields': len(result['fields']), 'calls': len(calls),
            'switch_entries': 10, 'overlap_differences': diffs,
            'missing_target_bytes': missing}


def run_pause(*, protect_exact: bool) -> tuple[dict, list[dict]]:
    obj = 'build/matching/AsciiManagerMenu.obj'
    build_nonexact('src/AsciiManagerMenu.cpp', obj,
                   reference_unit='ascii-manager-on-update')
    report = parsed(['scripts/inspect-pause-menu.py', '--json', obj])
    exact = [canonical(u) for u in PAUSE_EXACT_SIBLINGS] if protect_exact else []
    return {'pause_menu_update': guard_pause_menu(report)}, exact


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--owner', choices=['runecl', 'enemy', 'player', 'movement', 'gamemgr', 'crypto', 'effect', 'etama', 'exattack', 'type3', 'type19', 'title', 'anm', 'network', 'parser', 'front', 'screen', 'replay', 'pause', 'all'],
                        default='all')
    parser.add_argument('--skip-exact-siblings', action='store_true',
                        help='diagnostics only; explicitly omit the extra canonical regressions')
    args = parser.parse_args()
    report = {'diagnostic_only': True, 'exactness_credit': 'none',
              'owners': {}, 'canonical_siblings': []}
    try:
        execute(['scripts/verify-target.py'])
        if args.owner in ('pause', 'all'):
            owners, exact = run_pause(protect_exact=not args.skip_exact_siblings)
            report['owners'].update(owners)
            report['canonical_siblings'].extend(exact)
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
        if args.owner in ('movement', 'all'):
            owners, exact = run_player_movement(protect_exact=not args.skip_exact_siblings)
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
