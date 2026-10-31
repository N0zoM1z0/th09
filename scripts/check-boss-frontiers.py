#!/usr/bin/env python3
"""Cold replay TH09 ECL/Enemy/Etama/ExAttack/ANM/Title boss-and-leaf frontiers.

All non-exact outputs are diagnostics, never partial match credit. Canonical
carriers are rebuilt using the target-bound match-unit tool. A standalone
nonexact owner uses its recorded diagnostic profile and pinned compiler.
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


def run_network_message() -> dict:
    # This standalone source has no canonical exact siblings. The producer
    # binds its live project inputs before/after one low-priority compile.
    current = noncrediting(parsed(['scripts/inspect-network-message.py',
                                  '--build', '--guard', '--json']),
                           name='Supervisor network message', target_size=808,
                           fields=46)
    return {'network_message': current}


# Complete exact siblings of the still-nonexact RunEcl and Title large owners.
# A source file is not necessarily one compiler carrier: some reviewed units
# are compiled in alternate original-profile objects, and those must also be
# rebuilt rather than silently reusing the main diagnostic's object.
# These are manifest-verified cardinalities, not historical exactness totals.
ECL_EXACT_CARRIERS = {
    'build/matching/EclManager.obj': 76,
    'build/matching/EclManager-Type11.obj': 4,
}
TITLE_EXACT_CARRIERS = {
    'build/matching/TitleScreen.obj': 37,
    'build/matching/TitleScoreRecord.obj': 1,
}
# The true 622-byte camera-facing boss is compiled *inside* this Render TU.
# Protect every canonical function sharing its optimizer context, not merely
# the three nearby rendering leaves. Etama OnDraw also has a second exact
# same-carrier lifecycle callback beyond the bullet sprite drawing leaf.
ANM_RENDER_EXACT_CARRIERS = {'build/matching/AnmManagerRender.obj': 16}
ETAMA_DRAW_EXACT_CARRIERS = {'build/matching/EtamaControllerDraw.obj': 2}
# One real BulletManager.cpp compiler profile has three independently named
# physical object carriers, all previously exact. The 2,216-byte physical
# OnUpdate includes its compiler switch/data tail; all 18 source functions
# need a complete original-bound replay after changing shared declarations.
BULLET_MAIN_EXACT_CARRIERS = {
    'build/matching/BulletManager.obj': 15,
    'build/matching/BulletManagerPatterns.obj': 2,
    'build/matching/BulletManagerSelectSprite.obj': 1,
}
BULLET_ADVANCE_EXACT_CARRIERS = {
    'build/matching/BulletAdvanceTransformProgram.obj': 1,
}

# The resource-added callback is still nonexact inside EffectManager.cpp.
# All fifteen existing original-bound exact source owners share the SAME
# /O2 /Ob1 VC7.1 EffectManager carrier; guard the entire optimization cohort,
# not merely the three most visible update/draw callbacks.
EFFECT_MANAGER_EXACT_CARRIERS = {'build/matching/EffectManager.obj': 15}
# The 321-byte still-open ExAttackController::Spawn shares its real /Ob1
# compiler context with four exact lifecycle/spawn helpers. Two adjacent
# special-member functions happen to compile byte-for-byte like the retail
# under distinct /Ob1 and /Os profiles, but both remain origin UNKNOWN:
# generated/defaulted vs explicitly authored cannot be resolved by codegen.
EXATTACK_CONTROLLER_EXACT_CARRIERS = {
    'build/matching/ExAttackController.obj': 4,
}
# ScreenEffect's sixteen standard exact owners and two independent full
# functions use *different* VC7.1 compiler profiles. DrawSquare is /Ob0;
# RegisterChain is built without /EHsc, not inside the 336-byte shake boss'
# ordinary /O2 /Ob1 /EHsc carrier. A source-only census without profile
# separation previously silently skipped twelve already exact functions.
# Count, physical carrier and the ENTIRE separately reviewed original VC7.1
# command-line profile. A reference unit changing its own flags or object is
# not permission to silently rebaseline exact regression verification.
_SCREEN_EFFECT_BASE_PROFILE = (
    '/MT', '/EHsc', '/Gs', '/DNDEBUG', '/Zi', '/Gy', '/GF', '/Oi',
    '/Gr', '/O2', '/Ob1', '/Oy-', '/I', 'src')
SCREEN_EFFECT_EXACT_PROFILES = {
    'screen-effect-calc-shake': (
        16, 'build/matching/ScreenEffect.obj', _SCREEN_EFFECT_BASE_PROFILE),
    'screen-effect-register-chain': (
        1, 'build/matching/ScreenEffectRegister.obj',
        tuple(flag for flag in _SCREEN_EFFECT_BASE_PROFILE if flag != '/EHsc')),
    'screen-effect-draw-square': (
        1, 'build/matching/ScreenEffectDrawSquare.obj',
        tuple('/Ob0' if flag == '/Ob1' else flag
              for flag in _SCREEN_EFFECT_BASE_PROFILE)),
}

# Replay's 540-byte BeginPlaybackStage is compiled in the separate no-EH
# carrier alongside two exact functions, whereas TEN OTHER exact ReplayInput
# source functions require the true /EHsc compiler profile. The two groups
# cannot be inferred from the source filename or a cached ReplayInput.obj.
REPLAY_INPUT_EXACT_PROFILES = {
    'replay-manager-begin-recording-stage': (
        2, 'build/matching/ReplayInputNoEh.obj',
        tuple(flag for flag in _SCREEN_EFFECT_BASE_PROFILE if flag != '/EHsc')),
    'replay-input-update': (
        10, 'build/matching/ReplayInput.obj', _SCREEN_EFFECT_BASE_PROFILE),
}


def exact_source_carriers(source: str, expected: dict[str, int],
                          units: dict | None = None) -> dict[str, list[str]]:
    """Check the *full* exact manifest inventory and its compiler-carrier split.

    Nonexact owners must not be counted as exact siblings. Every returned
    member still needs its independent original-bound canonical replay; the
    manifest discovery by itself never grants matching credit.
    """
    if units is None:
        with (ROOT / 'config/match-units.toml').open('rb') as stream:
            units = tomllib.load(stream)['units']
    groups: dict[str, list[str]] = {}
    profiles: dict[str, tuple[str, ...]] = {}
    seen_functions: set[str] = set()
    for name, spec in units.items():
        if spec.get('source') != source:
            continue
        obj = spec.get('object')
        flags = spec.get('profile')
        funcs = spec.get('functions')
        if (not isinstance(obj, str) or not obj.startswith('build/matching/') or
                not isinstance(flags, list) or not flags or
                not all(isinstance(flag, str) for flag in flags) or
                not isinstance(funcs, list) or not funcs or
                any(not isinstance(func, str) or not func or
                    func in seen_functions for func in funcs)):
            raise RuntimeError(f'non-canonical exact carrier metadata: {name}')
        seen_functions.update(funcs)
        profile = tuple(flags)
        if obj in profiles and profile != profiles[obj]:
            raise RuntimeError(f'mixed compiler profiles in exact carrier: {obj}')
        profiles[obj] = profile
        groups.setdefault(obj, []).append(name)
    observed = {obj: len(group) for obj, group in groups.items()}
    if observed != expected:
        raise RuntimeError(f'exact sibling carrier inventory drifted for {source}: '
                           f'{observed!r} != {expected!r}')
    return groups


def cold_replay_exact_source(source: str, expected: dict[str, int],
                             *, already_built: tuple[str, ...] = ()) -> list[dict]:
    """Rebuild every physical carrier once, verify *every* exact original unit."""
    groups = exact_source_carriers(source, expected)
    if any(obj not in groups for obj in already_built):
        raise RuntimeError('prebuilt exact carrier not in the validated inventory')
    replays = []
    for obj, names in groups.items():
        if obj not in already_built:
            build(names[0])
        for name in names:
            replays.append(canonical(name))
    return replays


def exact_source_profile_groups(source: str,
                                references: dict[str, tuple[int, str, tuple[str, ...]]],
                                units: dict | None = None) -> dict[str, list[str]]:
    """Inventory every exact member of a source under its distinct real profiles.

    Unlike distinct physical carriers, two profile builds may overwrite the
    *same* object path. Partition by the **full pinned compiler flags** and
    insist that each separate compiler run uses the reviewed output object.
    Never transfer source results from one optimization/EH profile to another.
    """
    if units is None:
        with (ROOT / 'config/match-units.toml').open('rb') as stream:
            units = tomllib.load(stream)['units']
    all_names = {name for name, spec in units.items()
                 if spec.get('source') == source}
    if not all_names or not references:
        raise RuntimeError('no exact same-source profile inventory was reviewed')
    groups: dict[str, list[str]] = {}
    seen: set[str] = set()
    seen_profiles: set[tuple[str, ...]] = set()
    for reference, contract in references.items():
        if (not isinstance(contract, tuple) or len(contract) != 3):
            raise RuntimeError('missing pinned exact compiler profile contract: '+reference)
        amount, pinned_obj, pinned_flags = contract
        unit = units.get(reference)
        if (not isinstance(unit, dict) or unit.get('source') != source or
                type(amount) is not int or amount < 1 or
                not isinstance(pinned_obj, str) or
                not pinned_obj.startswith('build/matching/') or
                not isinstance(pinned_flags, tuple) or not pinned_flags or
                not all(isinstance(x, str) and x for x in pinned_flags) or
                unit.get('object') != pinned_obj or
                unit.get('profile') != list(pinned_flags)):
            raise RuntimeError('unreviewed exact compiler profile reference: '+reference)
        profile = pinned_flags
        if profile in seen_profiles:
            raise RuntimeError('duplicate exact compiler profile reference: '+reference)
        seen_profiles.add(profile)
        group = {name: spec for name, spec in units.items()
                 if name in all_names and spec.get('profile') == list(profile)}
        # The nested check enforces full source-function identity, concrete
        # build/matching carrier, unique functions and one correct object.
        validated = exact_source_carriers(
            source, {pinned_obj: amount}, units=group)
        members = validated[pinned_obj]
        if reference not in members or seen.intersection(members):
            raise RuntimeError('reviewed exact compiler profile inventory overlaps')
        groups[reference] = members
        seen.update(members)
    if seen != all_names:
        raise RuntimeError('exact same-source profile coverage incomplete: '
                           f'{sorted(all_names - seen)}')
    return groups


def cold_replay_exact_profiles(source: str,
                               references: dict[str, tuple[int, str, tuple[str, ...]]],
                               *, already_built: tuple[str, ...] = ()) -> list[dict]:
    """One actual cold compile per reviewed profile and every exact full body."""
    groups = exact_source_profile_groups(source, references)
    if any(ref not in groups for ref in already_built):
        raise RuntimeError('unreviewed already-built exact profile')
    results = []
    for ref, names in groups.items():
        if ref not in already_built:
            build(ref)
        results.extend(canonical(unit) for unit in names)
    return results


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

# Source-cold full original PE/COFF audit yields exactly these 30 positions.
# The prior guard restricted only the two OPCODE WINDOWS, accidentally letting
# a 30-byte candidate damage previously matching bytes *inside* a window.
# A legitimate source fix may remove any of these, never introduce another.
RUNECL_ORIGINAL_RESIDUAL_OFFSETS = frozenset((
    0x0278, 0x027A, 0x027B, 0x027C, 0x027D, 0x027E, 0x0280, 0x30B0,
    0x30B1, 0x30B2, 0x30B3, 0x30B4, 0x30B5, 0x30B6, 0x30B7, 0x30B9,
    0x30BA, 0x30BB, 0x30BC, 0x30BD, 0x30BE, 0x30BF, 0x30C0, 0x30C1,
    0x30C2, 0x30C4, 0x30C5, 0x30C6, 0x30C8, 0x30CA,
))


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
        if len(slots) != 1 or offset not in RUNECL_ORIGINAL_RESIDUAL_OFFSETS:
            raise RuntimeError('RunEcl damaged an already matched original physical byte')
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


# Original-PE bound complete MoveRandomBiased @0x00407E30 (516 physical bytes).
# The real pinned ECL translation unit currently emits 517 bytes. Original
# x87 pointer aliasing is still different: retail materializes &Y via
# LEA ECX,[EAX+4] after the exact Float3::operator float* call; VC7.1
# emits MOV ECX,EAX and FCOMP [ECX+4] instead. The resulting one-byte
# drift changes 148 *overlapping* physical positions through the return.
# All remaining 368 positions already agree with the SHA-attested original;
# they are not expendable merely because their source owner is still open.
BIASED_MOVEMENT_TARGET_SHA = 'c7d16fba9a2e0759836560b3977fa5c277966929a33d6183a2bb8672ba5319ab'
BIASED_MOVEMENT_RESIDUAL_RUNS = (
    (0x134, 0x139), (0x165, 0x194), (0x195, 0x1A6),
    (0x1A7, 0x1B7), (0x1B8, 0x1C2), (0x1C3, 0x1C8),
    (0x1C9, 0x1CE), (0x1CF, 0x1D4), (0x1D5, 0x1DA),
    (0x1DB, 0x1E2), (0x1E3, 0x1E8), (0x1ED, 0x1F1),
    (0x1F2, 0x1FD), (0x1FE, 0x204),
)
BIASED_MOVEMENT_RESIDUAL_OFFSETS = frozenset(
    i for low, high in BIASED_MOVEMENT_RESIDUAL_RUNS for i in range(low, high))


def guard_biased_movement(report: dict) -> dict:
    """Fail closed on new wrong TH09 bytes in the ECL movement helper.

    Protect all 28 independent original call/data operands, the whole owner,
    two independently decoded alias-scheduling instruction spans and every
    byte which already matches. No incomplete/nonexact comparison earns
    codegen-exact credit (even if this diagnostic later becomes all equal).
    """
    noncrediting(report, name='ECL MoveRandomBiased', target_size=516, fields=28)
    words = report.get('difference_offsets')
    count = report.get('overlapping_differences')
    size = report.get('candidate_bytes')
    if (report.get('target_sha256') != BIASED_MOVEMENT_TARGET_SHA or
            size not in (516, 517) or type(count) is not int or
            len(BIASED_MOVEMENT_RESIDUAL_OFFSETS) != 148 or
            not 0 <= count <= 148 or
            not isinstance(words, list) or len(words) != count or
            report.get('extra_candidate_bytes') != size - 516 or
            report.get('target_instructions') != 145 or
            report.get('candidate_instructions') != 145 or
            report.get('ordered_calls') != 14 or
            report.get('target_direct_blocks') != 24 or
            report.get('candidate_direct_blocks') != 24 or
            report.get('complete_direct_graphs_agree') is not True or
            report.get('complete_bytes_equal') is not (size == 516 and count == 0)):
        raise RuntimeError('ECL biased movement original extent/fields/calls/graph or 148-byte ceiling regressed')
    parsed_offsets = []
    for word in words:
        if (not isinstance(word, str) or
                not re.fullmatch(r'0x[0-9A-Fa-f]+', word)):
            raise RuntimeError('ECL biased movement residual field malformed')
        offset = int(word, 16)
        if offset not in BIASED_MOVEMENT_RESIDUAL_OFFSETS:
            raise RuntimeError('ECL biased movement damaged an already matched original byte')
        parsed_offsets.append(offset)
    if parsed_offsets != sorted(set(parsed_offsets)):
        raise RuntimeError('ECL biased movement residuals duplicated or unsorted')
    # A full byte-difference count by itself is not enough: the retail and
    # linked source can preserve a count while damaging an unrelated helper.
    # Their only normalized instruction replacements currently occur at the
    # Float3 conversion Y-alias and its subsequent second comparison.
    windows = ((0x134, 0x139), (0x164, 0x168))
    changes = report.get('normalized_instruction_changes')
    if not isinstance(changes, list) or len(changes) > 2:
        raise RuntimeError('ECL biased movement normalized instruction frontier regressed')
    observed_windows = set()
    for change in changes:
        if not isinstance(change, dict) or change.get('change') != 'replace':
            raise RuntimeError('ECL biased movement unexpected normalized change')
        pair_windows = set()
        for side in ('target', 'candidate'):
            entries = change.get(side)
            if not isinstance(entries, list) or not entries:
                raise RuntimeError('ECL biased movement missing normalized instruction')
            for ins in entries:
                if (not isinstance(ins, list) or len(ins) != 2 or
                        type(ins[0]) is not int or
                        not isinstance(ins[1], str) or
                        not re.fullmatch(r'[0-9a-fA-F]+', ins[1])):
                    raise RuntimeError('ECL biased movement malformed normalized instruction')
                slots = [i for i, (low, high) in enumerate(windows)
                         if low <= ins[0] < high]
                if len(slots) != 1:
                    raise RuntimeError('ECL biased movement changed previously matched instruction')
                pair_windows.add(slots[0])
        if len(pair_windows) != 1 or observed_windows.intersection(pair_windows):
            raise RuntimeError('ECL biased movement replaced an unrelated instruction window')
        observed_windows.update(pair_windows)
    return report


def guard_runecl_timeout_semantics(report: dict, *, ordinary_differences: int) -> dict:
    """Reject a missing/spoofed opcode-155 semantic check; never award exact credit.

    Each report is generated from the genuine, whole-PE-linked 15,564-byte
    owner and the current VC7.1 candidate; a byte-count alone does not prove
    behavior of the 32-bit flag update. Keep the second guard independent of
    the exact-byte gate so we can distinguish byte schedule from behavior.
    """
    expected = {
        'diagnostic_only': True, 'exactness_credit': 'none',
        'full_owner_original_sha256': RUNECL_TARGET_OWNER_SHA,
        'full_owner_ordinary_differences': ordinary_differences,
        'flag_transition_equal': True,
        'behavior_cases': 3072, 'wire_values_tested': 256, 'flags_tested': 12,
        'original_opcode_va': '0x40b770', 'opcode_bytes': 45,
        'original_instructions': 12, 'candidate_instructions': 12,
        'original_flag_prefix_instructions': 8,
        'candidate_flag_prefix_instructions': 8,
    }
    if not isinstance(report, dict) or any(report.get(key) != value
                                           for key, value in expected.items()):
        raise RuntimeError('RunEcl original opcode-155 machine-semantic gate regressed')
    return {'source': 'decoded original PE / current VC7.1 source COFF',
            'flag_transition_equal': True,
            'behavior_cases': report['behavior_cases'],
            'exactness_credit': 'none'}


def guard_runecl_timer_jump(report: dict, *, ordinary_differences: int) -> dict:
    """Guard real opcode-4 timer argument/receiver and ECL-pointer behavior.

    The supplier checks the entire source-cold, original-SHA-bound 15,564-byte
    linked owner before decoding either 29-byte instruction window. It
    emulates only reviewed original i386 forms and verifies a real thiscall
    and pointer advance. This is semantic provenance, NOT byte-exact credit.
    """
    required = {
        'diagnostic_only': True, 'exactness_credit': 'none',
        'full_owner_original_sha256': RUNECL_TARGET_OWNER_SHA,
        'full_owner_ordinary_differences': ordinary_differences,
        'original_opcode_va': '0x408937', 'opcode_bytes': 29,
        'original_instructions': 8, 'candidate_instructions': 8,
        'timer_call_va': '0x401500', 'redispatch_va': '0x408750',
        'time_values_tested': 6, 'offsets_tested': 5, 'contexts_tested': 3,
        'behavior_cases': 90, 'timer_and_advance_equal': True,
    }
    if not isinstance(report, dict) or any(report.get(key) != value
                                           for key, value in required.items()):
        raise RuntimeError('RunEcl original opcode-4 timer-jump semantic gate regressed')
    return {'source': 'decoded original PE / current VC7.1 source COFF',
            'timer_and_advance_equal': True,
            'behavior_cases': report['behavior_cases'],
            'exactness_credit': 'none'}




# All RunEcl 0x0C-byte timer instances now use the genuine ZunTimer ABI.
# Guard every separately exact timer method used here, including the unusual
# /Ob0 comparison carrier and the independent 20-byte equality owner.
# The one-byte/folded int getter @0x435F00 retains UNKNOWN origin; no credit.
RUNECL_TIMER_VC71_PROFILE = (
    '/MT', '/EHsc', '/Gs', '/DNDEBUG', '/Zi', '/Gy', '/GF',
    '/Oi', '/Gr', '/O2', '/Ob1', '/Oy-', '/I', 'src')
RUNECL_TIMER_VC71_OB0_PROFILE = tuple(
    '/Ob0' if option == '/Ob1' else option
    for option in RUNECL_TIMER_VC71_PROFILE)
RUNECL_REAL_TIMER_CARRIERS = {
    'src/ZunTimer.cpp': ('build/matching/ZunTimer.obj', RUNECL_TIMER_VC71_PROFILE),
    'src/BackgroundStageScript.cpp': (
        'build/matching/BackgroundStageScript.obj', RUNECL_TIMER_VC71_OB0_PROFILE),
    'src/EnemyScheduleRuntimeAdvance.cpp': (
        'build/matching/EnemyScheduleRuntimeAdvance.obj', RUNECL_TIMER_VC71_PROFILE),
}
RUNECL_REAL_TIMER_EXACT_CARRIERS = (
    ('src/ZunTimer.cpp', (
        ('zun-timer-assign-int', 0x00401500, '??4ZunTimer@@QAEXH@Z'),
        ('zun-timer-set-current', 0x004014D0, '?SetCurrent@ZunTimer@@QAEXH@Z'),
        ('zun-timer-post-decrement', 0x00406670, '??FZunTimer@@QAEXH@Z'),
        ('zun-timer-add-assign-int', 0x00406640, '??YZunTimer@@QAEXH@Z'),
        ('zun-timer-add-assign-float', 0x00406650, '??YZunTimer@@QAEXM@Z'),
        ('zun-timer-float-conversion', 0x004014F0, '??BZunTimer@@QAEMXZ'))),
    ('src/BackgroundStageScript.cpp', (
        ('zun-timer-greater-equal', 0x00401520, '??PZunTimer@@QAEIH@Z'),)),
    ('src/EnemyScheduleRuntimeAdvance.cpp', (
        ('zun-timer-equal-int', 0x00404950, '??8ZunTimer@@QAEHH@Z'),)),
)


def replay_runecl_real_timer_exact() -> list[dict]:
    """Cold exact eight authentic timer bodies from three real VC7 profiles."""
    with (ROOT/'config/match-units.toml').open('rb') as stream:
        units = tomllib.load(stream)['units']
    results = []
    objects = set()
    for source, members in RUNECL_REAL_TIMER_EXACT_CARRIERS:
        group = [units.get(name) for name, _, _ in members]
        if any(not isinstance(u, dict) for u in group):
            raise RuntimeError('RunEcl true ZunTimer exact callee missing')
        obj, flags = group[0].get('object'), group[0].get('profile')
        if (source not in RUNECL_REAL_TIMER_CARRIERS or
                obj != RUNECL_REAL_TIMER_CARRIERS[source][0] or
                flags != list(RUNECL_REAL_TIMER_CARRIERS[source][1]) or
                obj in objects):
            raise RuntimeError('RunEcl ZunTimer original carrier/profile drifted')
        objects.add(obj)
        for (name, address, symbol), unit in zip(members, group):
            if (unit.get('source') != source or unit.get('object') != obj or
                    unit.get('profile') != flags or unit.get('target_address') != address or
                    unit.get('symbol') != symbol):
                raise RuntimeError('RunEcl genuine ZunTimer callee referent drifted: '+name)
        build(members[0][0])
        results.extend(canonical(name) for name, _, _ in members)
    return results


# RunEcl opcodes 124..157 genuinely call the 32-/42-byte ASCII boss VM
# members and SoundPlayer's two exact member methods. The older 'BossUi::
# SetBossMarkerState' source alias was WRONG: its observed callee @0x4067D0
# is SetBossMarkerInterrupt(short), not AsciiManager::SetBossMarkerState
# @0x40F890. These separate original functions must never be interchanged.
RUNECL_REAL_STATE_EXACT_CARRIERS = (
    ('src/AsciiBossMarkerVm.cpp', (
        ('ascii-boss-marker-set-interrupt', 0x004067D0,
         '?SetBossMarkerInterrupt@AsciiBossMarkerVmView@@QAEXHF@Z'),
        ('ascii-boss-marker-set-position', 0x004067F0,
         '?SetBossMarkerPosition@AsciiBossMarkerVmView@@QAEXHPBUFloat3@@@Z'))),
    ('src/SoundPlayerProcessQueues.cpp', (
        ('soundplayer-play-sound-by-idx', 0x0043E2F0,
         '?PlaySoundByIdx@SoundPlayer@@QAEXHH@Z'),
        ('soundplayer-play-positioned-sound-by-idx', 0x0043E380,
         '?PlaySoundPositionedByIdx@SoundPlayer@@QAEXHM@Z'))),
)


def replay_runecl_real_state_callees(*, shared_with_enemy_prechecked: bool) -> list[dict]:
    """Protect 4 true owners once per cold source carrier, no double credit.

    The full all-sweep checks the ASCII twins and positioned audio later in
    Enemy's independently cold callee sweep. Focused RunEcl must check all
    four itself; all-mode adds only the distinct PlaySoundByIdx owner.
    """
    with (ROOT / 'config/match-units.toml').open('rb') as stream:
        manifest = tomllib.load(stream)['units']
    exact = []
    seen = set()
    for source, group in RUNECL_REAL_STATE_EXACT_CARRIERS:
        units = [manifest.get(name) for name, _, _ in group]
        if any(not isinstance(u, dict) for u in units):
            raise RuntimeError('missing genuine RunEcl boss/sound exact callee')
        obj = units[0].get('object')
        flags = units[0].get('profile')
        if (not isinstance(obj, str) or not obj.startswith('build/matching/') or
                not isinstance(flags, list) or not flags or obj in seen):
            raise RuntimeError('RunEcl boss/sound callee carrier/profile drifted')
        seen.add(obj)
        for (name, address, symbol), unit in zip(group, units):
            if (unit.get('source') != source or unit.get('object') != obj or
                    unit.get('profile') != flags or unit.get('symbol') != symbol or
                    unit.get('target_address') != address):
                raise RuntimeError('RunEcl exact boss/sound callee identity changed: '+name)
        # In all-mode, Enemy's true-source replay covers both ASCII owners
        # and positioned sound, so count only the separate 140-byte sound.
        if source == 'src/AsciiBossMarkerVm.cpp' and shared_with_enemy_prechecked:
            continue
        build(group[0][0])
        exact.extend(canonical(name) for name, _, _ in group
                     if not shared_with_enemy_prechecked or
                     name == 'soundplayer-play-sound-by-idx')
    return exact


# RunEcl opcodes 161/162 use the *real* side EtamaController pointer.
# Both original directly called methods are already canonical exact three-byte
# no-op leaves; old private BulletControllerLateView symbols were merely fake
# aliases numerically retargeted to the real original addresses. The genuine
# shared C++ member identity must survive when other TU declarations change.
RUNECL_REAL_ETAMA_REMOVE_CALLEES = (
    ('etama-controller-remove-all-bullets', 0x00412590,
     '?RemoveAllBullets@EtamaController@@QAEXH@Z'),
    ('etama-controller-remove-bullets-in-radius', 0x004125A0,
     '?RemoveBulletsInRadius@EtamaController@@QAEXPBUFloat3@@M@Z'),
)


def validate_runecl_real_etama_callees(units: dict) -> None:
    fragment = (ROOT/'src/EclRunLate.inl').read_text()
    audit = (ROOT/'scripts/audit-ecl-callsite-identities.py').read_text()
    if (fragment.count('EtamaController *bulletController08;') != 1 or
            fragment.count('inline EtamaController *BulletController(') != 1 or
            'struct BulletControllerLateView' in fragment or
            'BulletControllerLateView@Th09EclRunLate' in audit or
            fragment.count('->RemoveAllBullets(') != 1 or
            fragment.count('->RemoveBulletsInRadius(') != 1):
        raise RuntimeError('RunEcl side Etama uses a fake controller/method C++ identity')
    seen = []
    for name, address, symbol in RUNECL_REAL_ETAMA_REMOVE_CALLEES:
        unit = units.get(name)
        if (not isinstance(unit, dict) or
                unit.get('source') != 'src/EtamaRemoval.cpp' or
                unit.get('object') != 'build/matching/EtamaRemoval.obj' or
                unit.get('profile') != list(_SCREEN_EFFECT_BASE_PROFILE) or
                unit.get('target_address') != address or
                unit.get('symbol') != symbol or
                unit.get('size') != 3 or
                unit.get('relocations') != []):
            raise RuntimeError('RunEcl genuine Etama three-byte exact callee drifted: '+name)
        seen.append(unit.get('object'))
    if len(set(seen)) != 1:
        raise RuntimeError('RunEcl true Etama removal members split between C++ carriers')


def replay_runecl_real_etama_exact() -> list[dict]:
    """Cold replay the two real three-byte member callees, once per TU."""
    with (ROOT/'config/match-units.toml').open('rb') as stream:
        units = tomllib.load(stream)['units']
    validate_runecl_real_etama_callees(units)
    build('etama-controller-remove-all-bullets')
    return [canonical(name) for name, _, _ in RUNECL_REAL_ETAMA_REMOVE_CALLEES]


# Original RunEcl has eight direct references to the independently exact
# native global AddNormalizeAngle, NOT a source-private __stdcall alias.
# All argument types are float: the retail callee at 0x42AED0 reads the two
# stack arguments and terminates in RET 8. The real /Gr C++ owner is fastcall
# but has exactly this physical argument convention for two floating values.
RUNECL_REAL_ANGLE_SYMBOL = '?AddNormalizeAngle@@YIMMM@Z'
RUNECL_REAL_ANGLE_ORIGINAL_VA = 0x0042AED0
RUNECL_REAL_ANGLE_CALL_OFFSETS = frozenset((
    0x0C36, 0x0CAB, 0x1192, 0x120C, 0x12D4, 0x1370, 0x1F7E, 0x33DB,
))
RUNECL_REAL_ANGLE_LEAVES = (
    ('runtime-add-normalize-angle', 0x0042AED0,
     RUNECL_REAL_ANGLE_SYMBOL, 111),
    ('runtime-vector-angle', 0x00405700,
     '?VectorAngle@Th09EclRunControl@@YGMMM@Z', 15),
)
RUNECL_REAL_ANGLE_EXATTACK_FRAGMENTS = (
    'ExAttackInitType8.inl', 'ExAttackUpdateType8Type9.inl',
    'ExAttackUpdateType11Type12.inl', 'ExAttackUpdateType7.inl',
    'ExAttackUpdateType21.inl', 'EffectDoubleFanFamily.inl',
    'EffectFanVariants.inl', 'EffectFanFinalCallbacks.inl',
)


def validate_runecl_real_angle_owners(units: dict) -> None:
    """Require the exact global *C++* original, not merely its numeric VA."""
    header = (ROOT/'src/RuntimeAngleMath.hpp').read_text()
    control = (ROOT/'src/EclRunControl.inl').read_text()
    runtime = (ROOT/'src/RuntimeAngleMath.cpp').read_text()
    aliases = (ROOT/'scripts/audit-ecl-callsite-identities.py').read_text()
    if ('float __fastcall AddNormalizeAngle(float angle, float delta);' not in header or
            'float __stdcall AddNormalizeAngle' in header or
            '#include "RuntimeAngleMath.hpp"' not in control or
            'using ::AddNormalizeAngle;' not in control or
            'float __stdcall AddNormalizeAngle' in control or
            'float __stdcall AddNormalizeAngle' in runtime or
            '#include "RuntimeAngleMath.hpp"' not in runtime or
            '?AddNormalizeAngle@Th09EclRunControl@@YGMMM@Z' in aliases):
        raise RuntimeError('RunEcl real AddNormalizeAngle shared-header ABI regressed')
    for filename in RUNECL_REAL_ANGLE_EXATTACK_FRAGMENTS:
        source = (ROOT/'src'/filename).read_text()
        if ('#include "RuntimeAngleMath.hpp"' not in source or
                'float __stdcall AddNormalizeAngle' in source):
            raise RuntimeError('ExAttack shared carrier redeclares a fake global angle ABI: '+filename)
    for unit, va, symbol, size in RUNECL_REAL_ANGLE_LEAVES:
        entry = units.get(unit)
        if (not isinstance(entry, dict) or
                entry.get('source') != 'src/RuntimeAngleMath.cpp' or
                entry.get('object') != 'build/matching/RuntimeAngleMath.obj' or
                entry.get('profile') != list(_SCREEN_EFFECT_BASE_PROFILE) or
                entry.get('symbol') != symbol or entry.get('target_address') != va or
                entry.get('size') != size):
            raise RuntimeError('RunEcl genuine angle original method/profile drifted: '+unit)
    exact_ref_count = 0
    for entry in units.values():
        if entry.get('source') != 'src/EclManager.cpp':
            continue
        for field in entry['relocations']:
            sym = field['symbol']
            if 'AddNormalizeAngle' not in sym:
                continue
            if (sym != RUNECL_REAL_ANGLE_SYMBOL or
                    (field['type'],field['target'],field.get('addend',0)) !=
                    ('REL32', RUNECL_REAL_ANGLE_ORIGINAL_VA, 0)):
                raise RuntimeError('ECL/ExAttack exact source contains false angle-call identity')
            exact_ref_count += 1
    if exact_ref_count != 40:
        raise RuntimeError('ECL/Enemy/position/Effect 40 exact C++ angle callsites missing')


def guard_runecl_real_angle_callsite_report(report: dict) -> dict:
    """Fail closed on the eight real direct-call sites inside 15,564-byte boss."""
    calls = report.get('direct_calls')
    if (report.get('diagnostic_only') is not True or
            report.get('exactness_credit') != 'none' or
            report.get('audit_passed') is not True or
            report.get('failures') != [] or
            report.get('accounted_relocations') != 598 or
            report.get('total_relocations') != 598 or
            report.get('candidate_physical_bytes') != 15564 or
            report.get('target_physical_bytes') != 15564 or
            not isinstance(calls,list) or len(calls) != 375):
        raise RuntimeError('RunEcl true angle callsite whole-owner audit incomplete')
    angle = [item for item in calls if isinstance(item,dict) and
             'AddNormalizeAngle' in item.get('symbol','')]
    offsets = set()
    for entry in angle:
        offset = entry.get('candidate_offset')
        if (type(offset) is not int or
                entry.get('symbol') != RUNECL_REAL_ANGLE_SYMBOL or
                entry.get('expected_destination') != RUNECL_REAL_ANGLE_ORIGINAL_VA or
                entry.get('target_destination') != RUNECL_REAL_ANGLE_ORIGINAL_VA or
                entry.get('identity_agrees') is not True or
                not isinstance(entry.get('provenance'),dict) or
                entry['provenance'].get('canonical_units') !=
                ['runtime-add-normalize-angle'] or offset in offsets):
            raise RuntimeError('RunEcl angle call is not the exact original real method')
        offsets.add(offset)
    if offsets != RUNECL_REAL_ANGLE_CALL_OFFSETS:
        raise RuntimeError('RunEcl original eight true angle callsite offsets changed')
    return {'diagnostic_only':True,'exactness_credit':'none',
            'original_bound_angle_calls':len(offsets)}


def replay_runecl_real_angle_exact() -> list[dict]:
    """Cold compile the real 111-byte angle method and its 15-byte sibling."""
    with (ROOT/'config/match-units.toml').open('rb') as stream:
        units = tomllib.load(stream)['units']
    validate_runecl_real_angle_owners(units)
    build('runtime-add-normalize-angle')
    return [canonical(name) for name,_,_,_ in RUNECL_REAL_ANGLE_LEAVES]


def guard_effect_type26_update(result: dict) -> dict:
    """Require complete original bytes now that this209-byte owner is exact."""
    noncrediting(result, name='Effect26 update', target_size=209,
                 fields=10, fields_key='fields')
    calls = [['direct', 0x0042AED0], ['direct', 0x0042AED0]]
    if (result.get('target_sha256') !=
            'e22152173d39a96b45457ee3d79faec4d4e53a8afa309dfad53cc3509419f7a4' or
            result.get('candidate_bytes') != 209 or
            result.get('target_instructions') != 64 or
            result.get('candidate_instructions') != 64 or
            result.get('target_blocks') != 8 or result.get('candidate_blocks') != 8 or
            result.get('missing_bytes') != 0 or result.get('extra_bytes') != 0 or
            any(result.get(key) is not True for key in (
                'all_original_fields_agree', 'field_referent_order_agrees',
                'ordered_calls_agree', 'direct_graphs_agree')) or
            result.get('target_calls') != calls or result.get('candidate_calls') != calls):
        raise RuntimeError('Effect26 original extent/fields/calls/graph changed')
    globals_ = [r for r in result.get('bindings', [])
                if r.get('symbol', '').startswith('?g_GameManager@@')]
    if (len(globals_) != 1 or
            any(globals_[0].get(key) != value for key, value in {
                'offset': 0x34, 'type': 'DIR32',
                'symbol': '?g_GameManager@@3UAsciiGameManagerView@@A',
                'target': 0x004A7D90, 'addend': 4,
                'effective_target': 0x004A7D94}.items())):
        raise RuntimeError('Effect26 native GameManager field identity changed')
    if (result.get('complete_bytes_agree') is not True or
            result.get('difference_offsets') != [] or
            result.get('overlapping_differences') != 0):
        raise RuntimeError('Effect26 complete original bytes regressed')
    return {'target_bytes': 209, 'candidate_bytes': 209, 'fields': 10,
            'ordinary_differences': 0}


def run_ecl(*, include_trail: bool, include_interpreter: bool,
            protect_exact: bool,
            shared_real_callees_in_enemy: bool = False) -> tuple[dict, list[dict]]:
    with (ROOT/'config/match-units.toml').open('rb') as stream:
        validate_runecl_real_angle_owners(tomllib.load(stream)['units'])
    build('ecl-pop-context')
    object_file = 'build/matching/EclManager.obj'
    diagnostics: dict[str, dict] = {
        'effect_type26_update': guard_effect_type26_update(parsed([
            'scripts/inspect-effect-double-fan.py', object_file, '--json']))}
    exact_units = (cold_replay_exact_source(
        'src/EclManager.cpp', ECL_EXACT_CARRIERS,
        already_built=('build/matching/EclManager.obj',))
        if protect_exact else [])
    if protect_exact:
        exact_units.extend(replay_runecl_real_angle_exact())
    if include_interpreter:
        with tempfile.TemporaryDirectory(prefix='th09-runecl-') as directory:
            destination = str(Path(directory) / 'complete.json')
            execute(['scripts/inspect-ecl-complete.py', object_file, '--output', destination],
                    diagnostic=True)
            complete = json.loads(Path(destination).read_text())
        shapes = parsed(['scripts/report-ecl-handler-shapes.py', '--json', object_file])
        verified_runecl = validate_runecl_byte_frontier(complete, shapes)
        # A second independent original-body/COFF graph census validates
        # all 375 calls; demand the eight distinct canonical angle referents.
        with tempfile.TemporaryDirectory(prefix='th09-runecl-angle-') as directory:
            audit_path = Path(directory) / 'callsite-report.json'
            execute(['scripts/audit-ecl-callsite-identities.py', object_file,
                     '--output', str(audit_path)], diagnostic=True)
            angle_audit = guard_runecl_real_angle_callsite_report(
                json.loads(audit_path.read_text()))
        mismatches = verified_runecl['remaining_opcode_mismatches']
        # The other still-different handler must pass exactly the authentic
        # ZunTimer thiscall receiver/stack argument and redispatch behavior.
        timer_jump = parsed(['scripts/audit-ecl-timer-jump.py', object_file])
        timer_jump_summary = guard_runecl_timer_jump(
            timer_jump, ordinary_differences=verified_runecl['ordinary_differences'])
        # Always check the real Etama source/manifest symbols, even in the
        # noncrediting --skip-exact-siblings path. Canonical byte replay below
        # is gated by the caller's explicit exact-regression request.
        with (ROOT/'config/match-units.toml').open('rb') as stream:
            validate_runecl_real_etama_callees(tomllib.load(stream)['units'])
        if protect_exact:
            exact_units.extend(replay_runecl_real_etama_exact())
            exact_units.extend(replay_runecl_real_state_callees(
                shared_with_enemy_prechecked=shared_real_callees_in_enemy))
            # Shared genuine ZunTimer class: exact source-cold bodies from
            # the normal owner, /Ob0 greater-equal and separate equality TU.
            exact_units.extend(replay_runecl_real_timer_exact())
        # Codegen-near opcode 155 is also checked for actual behavior, using
        # decoded PE/COFF operations rather than trusting its C++ description.
        # This protects bit 24 and the other 31 existing flag bits but grants
        # no function exactness or evidence of original source spelling.
        timeout_semantics = parsed(['scripts/audit-ecl-timeout-bit.py', object_file])
        timeout_summary = guard_runecl_timeout_semantics(
            timeout_semantics, ordinary_differences=verified_runecl['ordinary_differences'])
        biased = guard_biased_movement(parsed(
            ['scripts/inspect-ecl-biased-movement.py', '--json', object_file],
            diagnostic=True))
        diagnostics.update(runecl_timer_jump_semantics=timer_jump_summary,
                           runecl_timeout_bit_semantics=timeout_summary, runecl={
            'physical_bytes': complete['physical_bytes'],
            'ordinary_differences': complete['ordinary_different_bytes'],
            'field_differences': complete['field_different_bytes'],
            'table_differences': complete['table_different_bytes'],
            'matching_handler_shapes': shapes['matching_handler_shapes'],
            'handlers': shapes['unique_handlers'],
            'remaining_opcode_mismatches': mismatches,
            'ordinary_differences_by_opcode': verified_runecl['differences_by_opcode'],
            'canonical_angle_calls':angle_audit['original_bound_angle_calls'],
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
        # The Type21 update is one of the 26 separately replayed exact ECL
        # siblings above; do not rebuild/double-count its shared carrier.
    return diagnostics, exact_units



def validate_exattack_trail_initializer(callback_type: str, result: dict) -> None:
    """Reject independent original/COFF ABI or complete-body regressions.

    Opcode/helper topology is not exactness: this remains noncrediting until a
    separate canonical target-bound match unit replays ALL bytes and fields.
    """
    # Indexed history induction closes both former preheader scheduling gaps.
    # The diagnostic path now protects every byte, including the old residuals;
    # canonical units separately replay the full original relocation manifests.
    specs = {
        '19': (763, 207, 57, 27, 9,
               'fc7b630cff17c32048de02f169f5c39710727027941aa502db823e357563cf67'),
        '21': (727, 198, 53, 25, 10,
               'b5271179d169d482952d887b03ad08e10a19c74654590e32598551d0a523ca47'),
    }
    if callback_type not in specs:
        raise RuntimeError('unsupported ExAttack trail initializer type')
    size, instructions, relocations, calls, blocks, target_sha = specs[callback_type]
    offsets = result.get('difference_offsets')
    if not isinstance(offsets, list) or offsets:
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
            result.get('normalized_aligned_instructions') != instructions or
            type(result.get('complete_differences_if_same_extent')) is not int or
            result['complete_differences_if_same_extent'] != 0 or
            result.get('complete_bytes_agree') is not True):
        raise RuntimeError('ExAttack Type' + callback_type +
                           ' initializer full-owner 57/53-field exact frontier regressed')


PLAYER_MOVEMENT_TARGET_SHA = '64ea64884e978540c718d1691a4ae1b9866aaa8ec2296bafb158c4d8b1c628ca'

# The 129 full physical difference positions were observed on the freshly
# compiled, complete original-PE/COFF-linked 1900-byte movement owner.
# All 1771 other physical bytes (including both original switch tables and
# the post-ret alignment byte) are ALREADY equal and may not regress.
PLAYER_MOVEMENT_ORIGINAL_RESIDUAL_OFFSETS = frozenset((
    0x0127, 0x0128, 0x0129, 0x012A, 0x012B, 0x012C, 0x012D, 0x012E, 0x0186, 0x0187, 0x0188,
    0x018A, 0x018C, 0x018D, 0x018E, 0x01DD, 0x01ED, 0x0209, 0x0219, 0x0223, 0x0233, 0x0236,
    0x028E, 0x0293, 0x029A, 0x02AA, 0x02B4, 0x02C4, 0x02D4, 0x02E4, 0x02F3, 0x0303, 0x0353,
    0x0363, 0x0385, 0x0395, 0x03A2, 0x03B2, 0x03BA, 0x03CA, 0x03D7, 0x03E7, 0x03F7, 0x0407,
    0x0416, 0x0426, 0x0458, 0x045D, 0x048F, 0x0494, 0x04BF, 0x04C4, 0x04D1, 0x04D4, 0x04D7,
    0x04DD, 0x0533, 0x0534, 0x0535, 0x0536, 0x0537, 0x0538, 0x0539, 0x053A, 0x053B, 0x053C,
    0x053D, 0x053E, 0x053F, 0x0540, 0x0541, 0x0542, 0x0543, 0x0544, 0x0545, 0x0546, 0x0547,
    0x0548, 0x0549, 0x054A, 0x054B, 0x054C, 0x054D, 0x054E, 0x054F, 0x0550, 0x0551, 0x0552,
    0x0553, 0x0554, 0x0555, 0x0556, 0x0557, 0x0558, 0x0559, 0x055A, 0x055B, 0x055C, 0x055D,
    0x055E, 0x055F, 0x0560, 0x0561, 0x0562, 0x0563, 0x0564, 0x0565, 0x0566, 0x0567, 0x0568,
    0x0569, 0x056F, 0x0598, 0x059A, 0x05A3, 0x05A5, 0x05AB, 0x05AD, 0x05B0, 0x05B5, 0x05B6,
    0x05B7, 0x05B8, 0x05B9, 0x05BA, 0x05BB, 0x05BC, 0x05BD, 0x05BE,
))


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
            len(PLAYER_MOVEMENT_ORIGINAL_RESIDUAL_OFFSETS) != 129 or
            not set(changed).issubset(PLAYER_MOVEMENT_ORIGINAL_RESIDUAL_OFFSETS) or
            result.get('missing_target_bytes') != 0 or
            result.get('extra_candidate_bytes') != 0 or
            result.get('full_byte_agreement') is not (count == 0)):
        raise RuntimeError('Player movement complete physical owner, 66 fields or 129-byte frontier regressed')
    return {'target_bytes': 1900, 'candidate_bytes': 1900,
            'ordinary_differences': count, 'fields': 66,
            'calls': 23, 'aligned_instructions': result['normalized_aligned_instructions']}


PLAYER_ADDED_SHA256 = 'b9b3e4115f8fb3857c373215f960b8eb0a73967cb33d581bb19d0cc8e868fb98'


def guard_player_added_native_owners(result: dict) -> None:
    expected = [
        {'offset': 0x30, 'type': 'DIR32',
         'symbol': '?g_GameManager@@3UGameManagerResetView@@A',
         'target': 0x004A7D90, 'addend': 0x35C,
         'effective_target': 0x004A80EC},
        *[{'offset': offset, 'type': 'REL32',
           'symbol': '??4ZunTimer@@QAEXH@Z', 'target': 0x00401500,
           'addend': 0, 'effective_target': 0x00401500}
          for offset in (0xA9, 0x136, 0x176, 0x1F6)],
    ]
    if result.get('native_owner_fields') != expected:
        raise RuntimeError('Player added-state native Timer/coordinate identity changed')


def guard_player_added_state(result: dict) -> dict:
    noncrediting(result, name='Player added state', target_size=572, fields=25)
    guard_player_added_native_owners(result)
    if (result.get('target_sha256') != PLAYER_ADDED_SHA256 or
            result.get('candidate_bytes') != 572 or result.get('code_bytes') != 552 or
            result.get('fields_agree') != 25 or
            result.get('target_instructions') != 136 or
            result.get('candidate_instructions') != 136 or
            result.get('target_calls') != 11 or result.get('candidate_calls') != 11 or
            result.get('ordered_calls_agree') is not True or
            result.get('target_blocks') != 20 or result.get('candidate_blocks') != 20 or
            result.get('direct_graph_agrees') is not True or
            result.get('switch_table_agrees') is not True):
        raise RuntimeError('Player added-state complete owner/fields/calls/graph changed')
    # The complete owner is now canonical exact; every code/table byte is protected.
    checked = physical_difference_windows(
        result, count_key='complete_differences', windows=(),
        equality_key='complete_bytes_equal')
    return {'target_bytes': 572, 'candidate_bytes': 572, 'bound_fields': 25,
            'ordinary_differences': checked['differences'], 'ordered_calls': 11}


def run_player_added_state(*, protect_exact: bool) -> tuple[dict, list[dict]]:
    # One actual source-cold carrier covers the boss and all three exact resets.
    build('player-added-collision-reset')
    evidence = parsed(['scripts/inspect-player-added-state.py',
                       'build/matching/PlayerAddedState.obj', '--json'])
    owner = guard_player_added_state(evidence)
    exact = []
    if protect_exact:
        exact = cold_replay_exact_source(
            'src/PlayerAddedState.cpp', {'build/matching/PlayerAddedState.obj': 4},
            already_built=('build/matching/PlayerAddedState.obj',))
    return {'player_added_state': owner}, exact


PLAYER_MOVEMENT_INTERRUPT_SYMBOL = '?SetInterrupt@AnmVm@@QAEXF@Z'


def validate_player_movement_interrupt_abi(units: dict) -> None:
    unit = units.get('anm-vm-set-interrupt')
    reference = units.get('player-on-update')
    if (not isinstance(unit, dict) or not isinstance(reference, dict) or
            unit.get('source') != 'src/AnmManagerRuntime.cpp' or
            unit.get('object') != 'build/matching/AnmManagerRuntime.obj' or
            unit.get('symbol') != PLAYER_MOVEMENT_INTERRUPT_SYMBOL or
            unit.get('target_address') != 0x00406790 or unit.get('size') != 18 or
            unit.get('relocations') != [] or
            not unit.get('profile') or unit.get('profile') != reference.get('profile')):
        raise RuntimeError('Player movement genuine AnmVm interrupt ABI changed')


def guard_player_movement_interrupt(report: dict) -> None:
    original = report.get('target_fields')
    actual = report.get('actual_resolved_fields')
    if not isinstance(original, list) or not isinstance(actual, list):
        raise RuntimeError('Player movement original/source field inventory absent')
    target = [row for row in original if isinstance(row, dict) and
              row.get('effective_destination') == 0x00406790]
    source = [row for row in actual if isinstance(row, dict) and
              (row.get('effective_destination') == 0x00406790 or
               'SetInterrupt' in str(row.get('symbol', '')))]
    if (len(target) != 1 or len(source) != 1 or
            target[0].get('offset') != 0x31D or target[0].get('type') != 'REL32' or
            source[0].get('offset') != 0x31D or source[0].get('type') != 'REL32' or
            source[0].get('symbol') != PLAYER_MOVEMENT_INTERRUPT_SYMBOL or
            source[0].get('addend') != 0 or source[0].get('owner_local') is not False or
            source[0].get('destination') != 0x00406790 or
            source[0].get('effective_destination') != 0x00406790):
        raise RuntimeError('Player movement original +0x31D native interrupt identity changed')


def replay_player_movement_interrupt_exact() -> list[dict]:
    with (ROOT/'config/match-units.toml').open('rb') as stream:
        validate_player_movement_interrupt_abi(tomllib.load(stream)['units'])
    build('anm-vm-set-interrupt')
    return [canonical('anm-vm-set-interrupt')]


def run_player_movement(*, protect_exact: bool) -> tuple[dict, list[dict]]:
    with (ROOT/'config/match-units.toml').open('rb') as stream:
        validate_player_movement_interrupt_abi(tomllib.load(stream)['units'])
    obj = 'build/matching/PlayerMovement.obj'
    build_nonexact('src/PlayerMovement.cpp', obj, reference_unit='player-on-update')
    report = parsed(['scripts/inspect-player-movement.py', obj])
    owner = guard_player_movement(report)
    guard_player_movement_interrupt(report)
    exact = []
    if protect_exact:
        build('player-on-update')
        exact.append(canonical('player-on-update'))
        exact.extend(replay_player_movement_interrupt_exact())
    return {'player_movement': owner}, exact


PLAYER_BULLET_COLLISION_SHA256 = '9dcfe1aca4ac06526072014ee6f956120d9c1d84cc1bba044638556b9b82445c'
# Fifty original-equal positions in the reviewed 849-byte candidate, including
# the five recovered by the explicit type-2/type-3 cancel branches. Protect the
# actual positions: a smaller mismatch count alone can hide a byte regression.
PLAYER_BULLET_COLLISION_MATCHED = frozenset(
    offset for start, end in (
        (0, 7), (16, 17), (45, 47), (48, 49), (50, 52), (103, 104),
        (110, 111), (192, 193), (277, 278), (291, 292), (297, 298),
        (303, 304), (373, 374), (389, 390), (471, 473), (475, 478),
        (479, 480), (500, 501), (589, 591), (604, 605), (617, 618),
        (623, 624), (626, 628), (632, 635), (637, 640), (660, 661),
        (757, 758), (785, 786), (795, 796), (813, 815), (839, 841),
    ) for offset in range(start, end))


def guard_player_bullet_collision(result: dict) -> dict:
    """Retain complete-owner evidence and matching bytes without exact credit."""
    bullet = noncrediting(result, name='Player CheckBulletCollision boss',
                         target_size=862, fields=26)
    size = bullet.get('candidate_bytes')
    count = bullet.get('overlap_differences')
    offsets = bullet.get('difference_offsets')
    instructions = bullet.get('candidate_instructions')
    if (bullet.get('target_sha256') != PLAYER_BULLET_COLLISION_SHA256 or
            type(size) is not int or not 849 <= size <= 862 or
            bullet.get('target_instructions') != 277 or
            type(instructions) is not int or not 266 <= instructions <= 277 or
            bullet.get('original_fields') != 26 or
            bullet.get('ordered_calls') != 12 or
            bullet.get('ordered_calls_agree') is not True or
            bullet.get('ordered_fields_agree') is not True or
            bullet.get('literal_definitions') != 4 or
            bullet.get('original_blocks') != 33 or
            bullet.get('candidate_blocks') not in (32, 33) or
            bullet.get('direct_graphs_agree') is not (bullet.get('candidate_blocks') == 33) or
            type(count) is not int or not 0 <= count <= 799 or
            not isinstance(offsets, list) or
            any(type(o) is not int or not 0 <= o < size for o in offsets) or
            offsets != sorted(set(offsets)) or len(offsets) != count or
            PLAYER_BULLET_COLLISION_MATCHED.intersection(offsets) or
            bullet.get('missing_target_bytes') != 862 - size or
            bullet.get('extra_candidate_bytes') != 0 or
            bullet.get('complete_bytes_equal') is not (size == 862 and count == 0) or
            (bullet.get('complete_bytes_equal') and
             (instructions != 277 or bullet.get('direct_graphs_agree') is not True))):
        raise RuntimeError('Player 862-byte collision boss target/fields/matching bytes regressed')
    return bullet


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
    bullet = guard_player_bullet_collision(parsed([
        'scripts/inspect-player-bullet-collision.py',
        'build/matching/PlayerCollisionHelpers.obj', '--json']))
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


# Historical unmatched positions of the 76-difference parser. Retained for
# frozen diagnostic controls; the active parser gate below now requires zero.
# The source-cold inspector independently SHA-checks original instructions and
# the sole CRT strncmp call, and publishes ALL observed difference offsets.
NETWORK_PARSER_ORIGINAL_SHA = 'b47499422bdbe8fb915ffa4009afae9f1cf923dc04c3668e100cc2d3afad647e'
NETWORK_PARSER_RESIDUAL_OFFSETS = frozenset((
    0x88, 0x90, 0x95, 0x96, 0x97, 0x98, 0x99, 0x9A, 0x9B, 0x9C, 0x9D, 0x9E,
    0x9F, 0xA0, 0xA1, 0xA2, 0xA3, 0xA4, 0xA5, 0xA6, 0xA7, 0xA8, 0xA9, 0xAB,
    0xAE, 0xAF, 0xB0, 0xB1, 0xB2, 0xB3, 0xB4, 0xB5, 0xB6, 0xB7, 0xB8, 0xB9,
    0xBA, 0xBB, 0xBC, 0xBD, 0xBE, 0xBF, 0xC0, 0xC1, 0xC2, 0xC3, 0xC4, 0xC5,
    0xC6, 0xC7, 0xC8, 0xC9, 0xCA, 0xCB, 0xCC, 0xCD, 0xCE, 0xCF, 0xD0, 0xD1,
    0xD2, 0xD3, 0xD4, 0xD5, 0xD7, 0xD8, 0xD9, 0xDA, 0xDB, 0xDC, 0xDD, 0xDE,
    0xDF, 0xE0, 0xE1, 0xE2,
))


def guard_network_parser_original(result: dict) -> dict:
    """Preserve every equal original physical byte; no local exactness credit."""
    noncrediting(result, name='Network config parser', target_size=243, fields=1)
    offsets = result.get('difference_offsets')
    count = result.get('overlap_differences')
    if (result.get('target_sha256') != NETWORK_PARSER_ORIGINAL_SHA or
            result.get('candidate_bytes') != 243 or
            result.get('target_instructions') != 116 or
            result.get('candidate_instructions') != 116 or
            result.get('original_fields') != 1 or
            result.get('ordered_calls') != 1 or
            result.get('ordered_calls_agree') is not True or
            result.get('target_blocks') != 33 or
            result.get('candidate_blocks') not in (33, 34) or
            result.get('direct_graphs_agree') is not (result.get('candidate_blocks') == 33) or
            type(count) is not int or not 0 <= count <= 76 or
            not isinstance(offsets, list) or len(offsets) != count or
            any(type(offset) is not int or not 0 <= offset < 243 for offset in offsets) or
            offsets != sorted(set(offsets)) or
            not set(offsets).issubset(NETWORK_PARSER_RESIDUAL_OFFSETS) or
            result.get('missing_target_bytes') != 0 or
            result.get('extra_candidate_bytes') != 0 or
            result.get('complete_bytes_equal') is not (count == 0)):
        raise RuntimeError('network parser 243-byte original SHA/field/graph/residual-offset frontier regressed')
    return result


def run_network_config_parser(*, protect_exact: bool) -> tuple[dict, list[dict]]:
    """Protect the complete exact parser and its true same-TU caller."""
    build('supervisor-network-config')
    result = guard_network_parser_original(parsed(
        ['scripts/inspect-network-config-parser.py',
         'build/matching/SupervisorNetworkConfig.obj', '--json']))
    if result['complete_bytes_equal'] is not True:
        raise RuntimeError('canonical 243-byte network config parser regressed')
    siblings = [canonical(name) for name in (
        'supervisor-network-config-parser', 'supervisor-network-config',
    )] if protect_exact else []
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


FRONT_ADDED_SHA256 = 'a1faabe117043306799d2c256fb5d0d76d0755b3c8e9015019c57dbd6a8377e1'
# Original-bound residual positions after the fourteen-byte source correction.
# Every other byte must stay equal; counts cannot justify an offset exchange.
FRONT_ADDED_RESIDUAL_MASK = bytes.fromhex(
    '0000000000f8ff1ffc3f0042f0ffe0ff011082ff07ff3f00080100000000c0ff030000c21f00'
    '000000000000e0f7fbffff1bfffbbeef270483f9fffbfaf3ff03000000000000000000000000'
    '00000000000000000000000000'
)
FRONT_ADDED_CALLEES = (
    ('front-message-runtime-reset', 'src/FrontMessageRuntimeReset.cpp',
     '?Reset@FrontMessageRuntimeResetView@@QAEHXZ', 0x00415C90, 23),
    ('front-message-load-for-side-count', 'src/FrontMessageLoad.cpp',
     '?LoadForSideCount@FrontMessageRuntimeView@@QAEHPAD0H@Z', 0x004182E0, 405),
    ('front-message-release-for-side-count', 'src/FrontMessageReleaseForSideCount.cpp',
     '?ReleaseForSideCount@FrontMessageRuntimeView@@QAEXH@Z', 0x00417520, 271),
    ('front-message-initialize-runtime', 'src/FrontMessageInitializeRuntime.cpp',
     '?InitializeMessageRuntime@FrontMessageOwnerView@@QAEXXZ', 0x004181E0, 133),
)


SUPERVISOR_UPDATE_SHA256 = 'c3ccfdefb056e202f64d3f31a313e200b1494fa49184d0fcd5ea3c6de248327c'
# Residuals include the 44 currently absent owner bytes. Protect the other
# 161 original positions independently of a total mismatch count.
SUPERVISOR_UPDATE_RESIDUAL_MASK = bytes.fromhex(
    '000080e3fffffffffffbffffbffffefbefbfffee7fffffff3fdff7fdf77d7fffbeffefe7'
    'f7df0800209efff7bffff9fefffffefdffffffffffffffffffffffdffffff7f3ff7fefff'
    'ffffffffffeffffffffff3fffffaffffff1f88ffffeffffff9fffff7fffffff7ff1f3131'
    '3131ff3f133331f1ff1311ffffffffff0f'
)

SUPERVISOR_UPDATE_CALLEES = (
    ('supervisor-subthread-is-running', 'src/SupervisorSubthread.cpp',
     '?SupervisorSubthreadIsRunning@@YIHPAVSupervisor@@@Z', 0x0042ED70, 14),
    ('supervisor-check-version', 'src/SupervisorVersion.cpp',
     '?CheckVersion@SupervisorVersionView@@QAEHPBDHH@Z', 0x00430150, 199),
    ('supervisor-load-version-data', 'src/SupervisorVersion.cpp',
     '?SupervisorAddedLoadDat@@YIHXZ', 0x0042F100, 145),
    ('anm-manager-execute-script-array', 'src/AnmManagerRuntime.cpp',
     '?ExecuteScriptArray@AnmManager@@QAEXPAUAnmVm@@H@Z', 0x00439560, 53),
    ('anm-manager-service-preloaded-anims', 'src/AnmManagerLoad.cpp',
     '?ServicePreloadedAnims@AnmManager@@QAEHXZ', 0x0043CDD0, 87),
    ('soundplayer-update-fades', 'src/SoundPlayerProcessQueues.cpp',
     '?UpdateFades@SoundPlayer@@QAEXXZ', 0x0042FBE0, 54),
    ('supervisor-register-chain', 'src/SupervisorRegisterChain.cpp',
     '?SupervisorRegisterChain@@YIHXZ', 0x00432180, 182),
    ('title-screen-register', 'src/TitleScreenRegister.cpp',
     '?RegisterTitleScreen@@YIHH@Z', 0x0042AB13, 201),
    ('ending-register-chain', 'src/Ending.cpp',
     '?RegisterChain@Ending@@SIHXZ', 0x0040F080, 223),
    ('game-manager-register-chain', 'src/GameManagerSetup.cpp',
     '?RegisterChain@GameManagerSetupLayout@@SIHXZ', 0x0041B9D0, 127),
)


def guard_supervisor_update(result: dict) -> dict:
    noncrediting(result, name='Supervisor OnUpdate', target_size=996, fields=92,
                 size_key='target_owned_bytes', fields_key='candidate_fields')
    offsets = result.get('original_difference_offsets')
    overlap, absent = result.get('overlapping_differences'), result.get('absent_bytes')
    allowed = {n for n in range(996)
               if SUPERVISOR_UPDATE_RESIDUAL_MASK[n//8] & (1 << (n % 8))}
    size = result.get('candidate_owned_bytes')
    if (result.get('target_subthread_predicate_calls') != 2 or
            result.get('candidate_subthread_predicate_calls') != 2 or
            result.get('subthread_predicate_calls_agree') is not True or
            result.get('original_owned_sha256') != SUPERVISOR_UPDATE_SHA256 or
            result.get('target_authored_bytes') != 844 or
            result.get('target_fields') != 92 or
            result.get('target_instructions') != 217 or
            type(size) is not int or not 952 <= size <= 996 or
            result.get('candidate_pre_table_bytes') != size - 152 or
            result.get('target_calls') != 27 or result.get('candidate_calls') != 27 or
            result.get('target_title_register_calls') != 1 or
            result.get('candidate_title_register_calls') != 1 or
            result.get('shared_title_register_call_agree') is not True or
            result.get('external_field_inventory_equal') is not True or
            result.get('missing_external_fields') != [] or result.get('extra_external_fields') != [] or
            result.get('target_table_counts') != [10, 4, 6, 15] or
            result.get('candidate_table_counts') != [10, 4, 6, 15] or
            result.get('case_index_alignment_equal') is not True or
            type(overlap) is not int or overlap < 0 or
            type(absent) is not int or absent != 996 - size or
            result.get('excess_bytes') != 0 or
            not isinstance(offsets, list) or len(offsets) != overlap + absent or
            any(type(n) is not int for n in offsets) or offsets != sorted(set(offsets)) or
            not set(offsets).issubset(allowed) or
            any(n not in offsets for n in range(size, 996)) or
            result.get('complete_equal') is not (len(offsets) == 0) or
            (not offsets and result.get('candidate_instructions') != 217)):
        raise RuntimeError('Supervisor OnUpdate complete original/field/call/table frontier regressed')
    return result


def run_supervisor_update(*, protect_exact: bool) -> tuple[dict, list[dict]]:
    obj = 'build/matching/SupervisorUpdate.obj'
    build_nonexact('src/SupervisorUpdate.cpp', obj, reference_unit='supervisor-register-chain')
    result = guard_supervisor_update(parsed(['scripts/inspect-supervisor-update.py', obj], diagnostic=True))
    exact = []
    if protect_exact:
        with (ROOT/'config/match-units.toml').open('rb') as stream:
            units = tomllib.load(stream)['units']
        for name, source, symbol, address, size in SUPERVISOR_UPDATE_CALLEES:
            unit = units[name]
            profile = list(_SCREEN_EFFECT_BASE_PROFILE)
            if name in ('supervisor-check-version', 'title-screen-register'):
                profile.remove('/Oi')
            if name == 'title-screen-register':
                # The actual Title registrar retains its separately proven /O1 profile.
                profile[profile.index('/O2')] = '/O1'
            if name == 'game-manager-register-chain':
                profile.insert(profile.index('/Ob1'), '/Os')
            if (unit['source'] != source or unit['symbol'] != symbol or
                    unit['target_address'] != address or unit['size'] != size or
                    unit['profile'] != profile):
                raise RuntimeError('Supervisor native dependency/source/profile drift: ' + name)
            build(name)
            exact.append(canonical(name))
    return {'supervisor_update': {k: result[k] for k in (
        'target_authored_bytes', 'target_owned_bytes', 'candidate_owned_bytes',
        'candidate_fields', 'candidate_calls', 'overlapping_differences', 'absent_bytes',
        'external_field_inventory_equal', 'complete_equal')}}, exact


def guard_front_added(result: dict) -> dict:
    noncrediting(result, name='Front AddedCallback', target_size=706, fields=37)
    count, offsets = result.get('differences'), result.get('difference_offsets')
    allowed = {n for n in range(706) if FRONT_ADDED_RESIDUAL_MASK[n//8] & (1 << (n % 8))}
    if (result.get('target_sha256') != FRONT_ADDED_SHA256 or
            result.get('candidate_bytes') != 706 or result.get('original_fields') != 37 or
            result.get('target_instructions') != 192 or
            type(result.get('candidate_instructions')) is not int or
            not 189 <= result['candidate_instructions'] <= 192 or
            result.get('ordered_calls') != 21 or result.get('calls_agree') is not True or
            result.get('target_blocks') != 19 or result.get('candidate_blocks') != 19 or
            result.get('direct_graphs_agree') is not True or
            type(count) is not int or not 0 <= count <= 222 or
            not isinstance(offsets, list) or len(offsets) != count or
            any(type(n) is not int for n in offsets) or offsets != sorted(set(offsets)) or
            not set(offsets).issubset(allowed) or
            (count == 0 and result['candidate_instructions'] != 192) or
            result.get('complete_bytes_equal') is not (count == 0)):
        raise RuntimeError('Front AddedCallback complete original byte/field/call/graph frontier regressed')
    return result


def run_front_added(*, protect_exact: bool, shared_callees_prechecked: bool = False) -> tuple[dict, list[dict]]:
    obj = 'build/matching/FrontAddedCallback.obj'
    build_nonexact('src/FrontAddedCallback.cpp', obj, reference_unit='front-message-setup')
    result = guard_front_added(parsed(['scripts/inspect-front-added.py', obj, '--json']))
    exact = []
    if protect_exact:
        with (ROOT/'config/match-units.toml').open('rb') as stream:
            units = tomllib.load(stream)['units']
        for name, source, symbol, address, size in FRONT_ADDED_CALLEES:
            unit = units[name]
            if (unit['source'] != source or unit['symbol'] != symbol or
                    unit['target_address'] != address or unit['size'] != size or
                    unit['profile'] != list(_SCREEN_EFFECT_BASE_PROFILE)):
                raise RuntimeError('Front native message callee/source/profile drift: ' + name)
            if shared_callees_prechecked and name in (
                    'front-message-load-for-side-count', 'front-message-initialize-runtime'):
                continue
            build(name)
            exact.append(canonical(name))
    return {'front_added_callback': result}, exact


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


RECEIVED_FRAME_TARGET_SHA256 = (
    'c7d33d3a0ca15df698c2c66916c24aa0b0b7527e769c08bbd1d0ed88ac245c17'
)
RECEIVED_FRAME_RESIDUAL_OFFSETS = frozenset((
    0x1C, 0x1D, 0x1E, 0x1F, 0x20, 0x21, 0x22, 0x23, 0x24,
    0x25, 0x26, 0x27, 0x28, 0x2A, 0x2B, 0x2C, 0x2D, 0x2E,
    0x2F, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38,
    0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0xBC, 0xBD, 0xBE,
))


def guard_received_frames(received: dict) -> None:
    """Protect all 178 matching bytes of the original received-frame leaf."""
    noncrediting(received, name='Supervisor received-frame insertion',
                 target_size=213, fields=0, fields_key='relocation_fields')
    if (received.get('target_sha256') != RECEIVED_FRAME_TARGET_SHA256 or
            received.get('candidate_bytes') != 213 or
            received.get('target_instructions') != 67 or
            received.get('candidate_instructions') != 67 or
            received.get('target_calls') != 0 or received.get('candidate_calls') != 0 or
            received.get('target_blocks') != 9 or received.get('candidate_blocks') != 9 or
            received.get('complete_direct_graphs_agree') is not True):
        raise RuntimeError('Supervisor received-frame no-call/67-instruction/9-block gate changed')
    physical_difference_windows(
        {**received, 'difference_offsets': received.get('differing_offsets')},
        count_key='complete_differences', windows=((0, 213, 35),),
        equality_key='complete_bytes_equal')
    observed = {int(x, 16) for x in received['differing_offsets']}
    if not observed.issubset(RECEIVED_FRAME_RESIDUAL_OFFSETS):
        raise RuntimeError('Supervisor received-frame damaged an already matched original byte')


def run_received_frames(*, protect_exact: bool) -> tuple[dict, list[dict]]:
    # Network frame insertion has no relocations, but 213/213 equal extent
    # alone is not evidence of code equality. Bind its entire Japanese
    # original SHA, 67 instructions, nine blocks, no calls and ret 16 ABI.
    obj = 'build/matching/SupervisorFrameQueueInsert.obj'
    build_nonexact('src/SupervisorFrameQueueInsert.cpp', obj,
                   reference_unit='supervisor-frame-queue-insert-predicted')
    received = parsed(['scripts/inspect-supervisor-frame-queue.py', obj, '--json'])
    guard_received_frames(received)
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


# The genuine SDK types, ordinary locals and /Ob0 close the entire owner.
CAMERA_FACING_TARGET_SHA = '36fd70db08a5570fe316d9e3beb541f7a39a90ef10b1429ad767e09c973f1865'
CAMERA_FACING_RESIDUAL_OFFSETS = frozenset()


def run_camera_facing(*, protect_exact: bool) -> tuple[dict, list[dict]]:
    # The target-owned camera-facing method now compiles in the actual Render
    # translation unit, immediately after the already-exact TranslateRotation.
    # Protect this callee and the exact Render callers from TU-visibility drift.
    build('anm-manager-project-camera-facing-quad')
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
            len(differences) != amount or amount != 0 or
            any(type(offset) is not int or
                offset not in CAMERA_FACING_RESIDUAL_OFFSETS for offset in differences) or
            differences != sorted(set(differences)) or
            candidate.get('complete_bytes_equal') is not (amount == 0)):
        raise RuntimeError('camera complete original 622-byte equality regressed')
    siblings = []
    if protect_exact:
        siblings = cold_replay_exact_source(
            'src/AnmManagerRender.cpp', ANM_RENDER_EXACT_CARRIERS,
            already_built=('build/matching/AnmManagerRender.obj',))
        # This 595-byte projection is a separately compiled caller/callee,
        # and cannot be inferred exact from the native Render carrier alone.
        build('anm-manager-project-3d-quad')
        siblings.append(canonical('anm-manager-project-3d-quad'))
    return {'camera_facing_projection': {
        'target_bytes': 622, 'candidate_bytes': candidate['candidate_bytes'],
        'bound_fields': candidate['bound_fields'],
        'overlap_differences': candidate['overlap_differences'],
        'missing_target_bytes': candidate['missing_target_bytes'],
        'direct_graphs_agree': candidate['direct_graphs_agree'],
    }}, siblings




# Exact 49 original-linked mismatching physical positions of the current
# 540-byte replay-stage owner (source produces 538, two bytes still missing).
# Every other compared byte already matches SHA-pinned Japanese TH09 v1.50a.
# The true original first publishes Replay header 1E7, then reads the
# global side-0 pointer before publishing 1E8. A narrowly scoped volatile
# lvalue for that ONE real pointer read restores the retail ordering of ALL
# 32 independently original-bound operands. That moves the first residual
# group (old 0x14B..0x15F) to nine register-selection bytes, and decreases
# the complete overlapping mismatch count from 49 to 39 without changing
# candidate size, calls or CFG. The last 30 mismatches are seed zero-extension
# and final tail timing. All remaining 499 bytes out of the current 538 are
# protected; unlike the old set, the nine residual register bytes have moved.
# A future proposal may CLEAR any of these, never introduce another.
REPLAY_BEGIN_TARGET_SHA = 'af0dd0d85e36de147b67c07e8790bd200adb04f2862496dac928b3731dff5f64'
REPLAY_BEGIN_ORIGINAL_RESIDUAL_OFFSETS = frozenset((
    0x152, 0x156, 0x15C, 0x162, 0x168, 0x16C, 0x172, 0x178, 0x17A,
    0x1FB, 0x1FC, 0x1FD, 0x1FE,
    0x200, 0x201, 0x202, 0x203, 0x204, 0x205, 0x206, 0x207,
    0x208, 0x209, 0x20A, 0x20B, 0x20C, 0x20D, 0x20E, 0x20F,
    0x210, 0x211, 0x212, 0x213, 0x214, 0x215, 0x216, 0x217,
    0x218, 0x219,
))
REPLAY_BEGIN_CURRENT_FIELD_OFFSET_DELTAS = (0,) * 30 + (-1, -2)


# Original-v1.50a PE/COFF pinned native memory ABI in large replay and
# recorder methods. The old fake ZunMemoryReplayView produced 76 phony C++
# global/method linker names to the correct numeric native addresses. Real
# VC7.1 /EHsc and no-EH profiles both preserve their entire exact code.
REPLAY_REAL_MEMORY_SYMBOLS = {
    'g': ('?g_ZunMemory@@3VZunMemory@@A', 'DIR32', 0x004AD018),
    'a': ('?Alloc@ZunMemory@@QAEPAXIPBD@Z', 'REL32', 0x00401340),
    'f': ('?Free@ZunMemory@@QAEXPAX@Z', 'REL32', 0x00401360),
    'r': ('?AddToRegistry@ZunMemory@@QAEPAXPAXIPAD@Z', 'REL32', 0x00401380),
}
# Independently reviewed original fixed field locations, not inferred from
# current manifest. Every PE-bound original absolute data operand and direct
# member call is accounted for by its complete canonical source comparison.
REPLAY_REAL_MEMORY_SITES = {
    'replay-manager-record-input': (460, 25, {
        'g': (0xDB, 0x107, 0x11B, 0x132, 0x146),
        'a': (0x10C, 0x123, 0x13A, 0x14E),
        'r': (0xE0,),
    }),
    'replay-manager-load-replay-data': (591, 14, {
        'g': (0x75, 0x9D, 0x118, 0x123, 0x23D),
        'a': (0xA2,),
        'f': (0x7A, 0x11D, 0x128, 0x242),
    }),
    'replay-manager-begin-recording-stage': (1323, 82, {
        'g': (0x51, 0x1EE, 0x201, 0x214, 0x22A, 0x246, 0x268,
              0x28D, 0x2B3, 0x3CC, 0x3E6, 0x3FD, 0x411, 0x441,
              0x453, 0x465, 0x477, 0x4A1),
        'a': (0x3D4, 0x3EE, 0x405, 0x419),
        'f': (0x1F3, 0x206, 0x219, 0x22F, 0x446, 0x458,
              0x46A, 0x47C, 0x4A6),
        'r': (0x56, 0x24B, 0x26D, 0x292, 0x2B8),
    }),
    'replay-manager-create': (361, 28, {
        'g': (0x34,),
        'r': (0x39,),
    }),
    'replay-manager-release-core': (104, 8, {
        'g': (0x46, 0x5B),
        'f': (0x4B, 0x60),
    }),
    'replay-manager-release': (318, 20, {
        'g': (0x2E, 0x40, 0x52, 0x64, 0x8C, 0xBE, 0xE6),
        'f': (0x33, 0x45, 0x57, 0x69, 0x91, 0xC3, 0xEB),
    }),
}
REPLAY_REAL_MEMORY_CALLEES = {
    'zun-memory-alloc': ('a', 0x00401340, 19),
    'zun-memory-free': ('f', 0x00401360, 19),
    'zun-memory-add-to-registry': ('r', 0x00401380, 10),
}


def validate_replay_real_memory_abi(units: dict) -> None:
    """All 76 original PE ZunMemory source fields must use one native C++ ABI."""
    code = (ROOT/'src/ReplayInput.cpp').read_text()
    header = (ROOT/'src/ZunMemory.hpp').read_text()
    if ('#include "ZunMemory.hpp"' not in code or
            'ZunMemoryReplayView' in code or
            'extern ZunMemory g_ZunMemory;' not in header or
            any('.' + method + '(' not in code for method in
                ('Alloc', 'Free', 'AddToRegistry'))):
        raise RuntimeError('Replay big callback source reintroduced fake ZunMemory class')
    exact_source_profile_groups('src/ReplayInput.cpp', REPLAY_INPUT_EXACT_PROFILES, units)
    listed = []
    expected = []
    for name, (size, count, site_groups) in REPLAY_REAL_MEMORY_SITES.items():
        row = units.get(name)
        if (not isinstance(row, dict) or
                row.get('source') != 'src/ReplayInput.cpp' or
                row.get('target_address') is None or row.get('size') != size or
                len(row.get('relocations', [])) != count):
            raise RuntimeError('Replay source exact large callback original extent drifted: '+name)
        current = [
            (field.get('offset'),field.get('symbol'),field.get('type'),
             field.get('target'),field.get('addend',0))
            for field in row['relocations'] if
            'ZunMemory' in field.get('symbol','') or
            any(method+'@' in field.get('symbol','') for method in
                ('Alloc', 'Free', 'AddToRegistry'))]
        required = sorted((off, *REPLAY_REAL_MEMORY_SYMBOLS[short], 0)
                          for short, offsets in site_groups.items()
                          for off in offsets)
        if current != required:
            raise RuntimeError('Replay original PE field offset/kind/global/method VA changed: '+name)
        listed.extend(current)
        expected.extend(required)
    if len(expected) != 76 or len(listed) != 76:
        raise RuntimeError('Replay large callback source has incomplete 76-field native memory census')
    for name, (short, address, size) in REPLAY_REAL_MEMORY_CALLEES.items():
        row = units.get(name)
        if (not isinstance(row, dict) or
                row.get('source') != 'src/ZunMemoryRelease.cpp' or
                row.get('object') != 'build/matching/ZunMemoryRelease.obj' or
                row.get('symbol') != REPLAY_REAL_MEMORY_SYMBOLS[short][0] or
                row.get('target_address') != address or row.get('size') != size or
                row.get('profile') != list(_SCREEN_EFFECT_BASE_PROFILE)):
            raise RuntimeError('Replay genuine ZunMemory callee is not source-cold exact: '+name)
    # No missing/unreviewed fake aliases in other existing exact Replay units.
    for unit in units.values():
        if unit.get('source') != 'src/ReplayInput.cpp':
            continue
        for field in unit.get('relocations', []):
            if 'ZunMemoryReplayView' in field.get('symbol',''):
                raise RuntimeError('Replay canonical exact unit has fake memory linker symbol')


def replay_replay_real_memory_exact() -> list[dict]:
    """Build one genuine source TU and replay all three existing exact leaves."""
    with (ROOT/'config/match-units.toml').open('rb') as stream:
        validate_replay_real_memory_abi(tomllib.load(stream)['units'])
    build('zun-memory-alloc')
    return [canonical(unit) for unit in REPLAY_REAL_MEMORY_CALLEES]


def run_replay_save(*, protect_exact: bool, already_checked: bool = False) -> tuple[dict, list[dict]]:
    """Protect SaveReplay's complete owner and the two real carrier profiles."""
    if not already_checked:
        build('replay-manager-begin-recording-stage')
    current = parsed(['scripts/inspect-replay-save.py',
                      'build/matching/ReplayInputNoEh.obj', '--json', '--guard'])
    exact = []
    if protect_exact and not already_checked:
        exact = cold_replay_exact_profiles(
            'src/ReplayInput.cpp', REPLAY_INPUT_EXACT_PROFILES,
            already_built=('replay-manager-begin-recording-stage',))
    return {'replay_save': current}, exact


def run_replay_playback(*, protect_exact: bool) -> tuple[dict, list[dict]]:
    """Cold-audit the 540-byte boss, twelve exact owners and true ZunMemory leaves.

    The 32 original fields must be actual SHA-pinned PE operands, resolved
    through canonical referents, not guessed source-local labels. The lone
    side-state load motion and two absent source bytes are nonexact frontiers.
    """
    with (ROOT/'config/match-units.toml').open('rb') as stream:
        validate_replay_real_memory_abi(tomllib.load(stream)['units'])
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
            current.get('aligned_field_identities') != 32 or
            current.get('field_change_spans') != [] or
            current.get('field_offset_deltas') !=
                list(REPLAY_BEGIN_CURRENT_FIELD_OFFSET_DELTAS) or
            current.get('target_sha256') != REPLAY_BEGIN_TARGET_SHA or
            type(current.get('complete_overlap_differences')) is not int or
            current['complete_overlap_differences'] > 39 or
            current['complete_overlap_differences'] < 0 or
            current.get('missing_target_bytes', 999) > 2 or
            current.get('missing_target_bytes') !=
                max(0, 540 - current['candidate_bytes']) or
            current.get('extra_source_bytes') != 0 or
            current.get('complete_bytes_equal') is not False):
        raise RuntimeError('Replay playback 540-byte/32-field/5-call/12-block frontier regressed')
    # Count-based acceptance was unsafe: a changed compiler candidate could
    # fix one bad byte while breaking another that already equaled the PE.
    offsets = current.get('difference_offsets')
    if (len(REPLAY_BEGIN_ORIGINAL_RESIDUAL_OFFSETS) != 39 or
            not isinstance(offsets, list) or
            len(offsets) != current['complete_overlap_differences'] or
            offsets != sorted(set(x for x in offsets if type(x) is int)) or
            any(type(x) is not int or not 0 <= x < current['candidate_bytes']
                for x in offsets) or
            not set(offsets).issubset(REPLAY_BEGIN_ORIGINAL_RESIDUAL_OFFSETS)):
        raise RuntimeError('Replay playback damaged an already matching original physical byte')
    exact = []
    if protect_exact:
        # Preserve the already cold-built two-function /no-EH TU, then build
        # the distinct ten-function /EHsc carrier. Every member gets its own
        # original-bound, entire-physical-function canonical comparison.
        exact = cold_replay_exact_profiles(
            'src/ReplayInput.cpp', REPLAY_INPUT_EXACT_PROFILES,
            already_built=('replay-manager-begin-recording-stage',))
        exact.extend(replay_replay_real_memory_exact())
    return {'replay_begin_playback': {
        'target_bytes': 540, 'candidate_bytes': current['candidate_bytes'],
        'bound_fields': current['bound_fields'],
        'aligned_fields': current['aligned_field_identities'],
        'field_offset_deltas': current['field_offset_deltas'],
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
        # Keep the boss' real /EHsc /O2 /Ob1 object from the cold diagnostic,
        # check all sixteen same-profile exact owners, then independently
        # rebuild /no-EH RegisterChain and /Ob0 DrawSquare. Each canonical
        # comparison covers the COMPLETE real original PE/COFF owner.
        exact = cold_replay_exact_profiles(
            'src/ScreenEffect.cpp', SCREEN_EFFECT_EXACT_PROFILES,
            already_built=('screen-effect-calc-shake',))
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


# The 407-byte exact EffectManager::InitializeEffect physically calls the
# independently exact 41-byte *actual* AnmLoaded::SetAndExecuteScriptIdx.
# The older EffectAnmLoadedMethodView overlay changed the C++ member symbol
# while preserving the target numeric VA; enforce both native link ownership
# and the reviewed original full-physical field offset for that big caller.
EFFECT_NATIVE_ANM_SITE = (
    'effect-manager-initialize-effect', 0x0040CA60, 407,
    0x0E6, '?SetAndExecuteScriptIdx@AnmLoaded@@QAEXPAUAnmVm@@H@Z', 0x00403E00,
)


def validate_effect_real_anm_loaded_abi(units: dict) -> None:
    """Fail closed on invented ANM method, moved field, or fake TU origin."""
    source = (ROOT/'src/EffectManager.cpp').read_text()
    header = (ROOT/'src/AnmManager.hpp').read_text()
    effect_header = (ROOT/'src/EffectManager.hpp').read_text()
    if ('#include "AnmManager.hpp"' not in source or
            'EffectAnmLoadedMethodView' in source or
            'this->effectAnm->SetAndExecuteScriptIdx(' not in source or
            'class AnmLoaded' not in header or
            'void SetAndExecuteScriptIdx(AnmVm *vm, int scriptIndex);' not in header or
            'AnmLoaded *effectAnm;' not in effect_header):
        raise RuntimeError('EffectManager originally exact ANM source has fake C++ method owner')
    name,va,size,offset,symbol,callee_va = EFFECT_NATIVE_ANM_SITE
    caller = units.get(name)
    if (not isinstance(caller,dict) or
            caller.get('source')!='src/EffectManager.cpp' or
            caller.get('object')!='build/matching/EffectManager.obj' or
            caller.get('target_address')!=va or caller.get('size')!=size or
            caller.get('profile')!=[f for f in _SCREEN_EFFECT_BASE_PROFILE if f != '/EHsc']):
        raise RuntimeError('EffectManager 407-byte exact InitializeEffect original carrier changed')
    expected_site=(offset,'REL32',symbol,callee_va,0)
    found=[]
    for unit_name,unit in units.items():
        if unit.get('source')!='src/EffectManager.cpp':
            continue
        for field in unit.get('relocations',[]):
            val=field.get('symbol','')
            if 'SetAndExecuteScriptIdx' in val or 'EffectAnmLoadedMethodView' in val:
                found.append((unit_name,(field.get('offset'),field.get('type'),
                          val,field.get('target'),field.get('addend',0))))
    if found != [(name,expected_site)]:
        raise RuntimeError('EffectManager exact original ANM callsite/ABI changed')
    callee=units.get('anm-loaded-set-and-execute-script-idx')
    if (not isinstance(callee,dict) or
            callee.get('symbol')!=symbol or callee.get('target_address')!=callee_va or
            callee.get('source')!='src/AnmLoadedExecute.cpp' or
            callee.get('object')!='build/matching/AnmLoadedExecute.obj' or
            callee.get('size')!=41 or
            callee.get('profile') != [
                '/MT','/EHsc','/Gs','/DNDEBUG','/Zi','/Gy','/GF','/Oi','/Gr',
                '/O2','/Ob1','/Oy-','/I','src']):
        raise RuntimeError('EffectManager original ANM callee is not its authentic 41-byte exact source')


def replay_effect_real_anm_loaded_exact() -> list[dict]:
    with (ROOT/'config/match-units.toml').open('rb') as stream:
        validate_effect_real_anm_loaded_abi(tomllib.load(stream)['units'])
    build('anm-loaded-set-and-execute-script-idx')
    return [canonical('anm-loaded-set-and-execute-script-idx')]



EFFECT_GAME_MANAGER_SITES = {
    'effect-manager-update': (0x0040CDD0, 475, (
        (0x07, 'DIR32', '?g_GameManager@@3UAsciiGameManagerView@@A', 0x004A7D90, 0x134),
        (0x1D, 'DIR32', '?g_GameManager@@3UAsciiGameManagerView@@A', 0x004A7D90, 0x34))),
    'effect-manager-draw': (0x0040CFB0, 183, (
        (0x45, 'DIR32', '?g_GameManager@@3UAsciiGameManagerView@@A', 0x004A7D90, 0),
        (0x50, 'REL32', '?TransformPopupX@AsciiGameManagerView@@QAEMM@Z', 0x00401680, 0),
        (0x5B, 'DIR32', '?g_GameManager@@3UAsciiGameManagerView@@A', 0x004A7D90, 0),
        (0x60, 'REL32', '?TransformPopupY@AsciiGameManagerView@@QAEMM@Z', 0x004016B0, 0))),
    'effect-manager-create': (0x0040D320, 327, (
        (0x52, 'DIR32', '?g_GameManager@@3UAsciiGameManagerView@@A', 0x004A7D90, 0x38),
        (0x58, 'DIR32', '?g_GameManager@@3UAsciiGameManagerView@@A', 0x004A7D90, 0),
        (0x68, 'DIR32', '?g_GameManager@@3UAsciiGameManagerView@@A', 0x004A7D90, 0))),
}


def validate_effect_real_game_manager_abi(units: dict) -> None:
    """Bind the exact Effect callers to shared source, not numeric aliases."""
    source = (ROOT/'src/EffectManager.cpp').read_text()
    if ('#include "AsciiGameManagerView.hpp"' not in source or
            'EffectGameManager' in source or
            'g_GameManager.sides[0].flags' not in source or
            'sideIndex * sizeof(AsciiGameManagerSideView)' not in source or
            '(1 - sideIndex) * sizeof(AsciiGameManagerSideView)' not in source):
        raise RuntimeError('Effect GameManager has a split owner or invented third typed side')
    for name, (address, size, sites) in EFFECT_GAME_MANAGER_SITES.items():
        unit = units.get(name)
        if (not isinstance(unit, dict) or unit.get('source') != 'src/EffectManager.cpp'
                or unit.get('object') != 'build/matching/EffectManager.obj'
                or unit.get('target_address') != address or unit.get('size') != size
                or unit.get('profile') != [f for f in _SCREEN_EFFECT_BASE_PROFILE if f != '/EHsc']):
            raise RuntimeError('Effect GameManager original caller/source/profile changed')
        for offset, kind, symbol, target, addend in sites:
            rows = [row for row in unit.get('relocations', []) if row.get('offset') == offset]
            # Omitted metadata is not an assertion that actual COFF addend is zero.
            # Complete canonical replay verifies its original +134/+34/+38 bytes.
            if (len(rows) != 1 or
                    (rows[0].get('type'), rows[0].get('symbol'), rows[0].get('target')) !=
                    (kind, symbol, target) or rows[0].get('addend', addend) != addend):
                raise RuntimeError('Effect GameManager original native field identity changed')
    for axis, address in (('x', 0x00401680), ('y', 0x004016B0)):
        unit = units.get('game-manager-transform-popup-' + axis)
        if (not isinstance(unit, dict) or unit.get('source') != 'src/GameManagerCoordinates.cpp'
                or unit.get('object') != 'build/matching/GameManagerCoordinates.obj'
                or unit.get('symbol') != '?TransformPopup' + axis.upper() + '@AsciiGameManagerView@@QAEMM@Z'
                or unit.get('target_address') != address or unit.get('size') != 46
                or unit.get('profile') != list(_SCREEN_EFFECT_BASE_PROFILE)):
            raise RuntimeError('Effect GameManager coordinate callee has no native exact owner')


def run_effect(*, protect_exact: bool) -> tuple[dict, list[dict]]:
    with (ROOT/'config/match-units.toml').open('rb') as stream:
        units = tomllib.load(stream)['units']
    validate_effect_real_anm_loaded_abi(units)
    validate_effect_real_game_manager_abi(units)
    # The 51-byte resource loader lives inside the optimizer carrier with
    # *fifteen* canonical exact original functions. One cold real TU build
    # must protect every one of them, not just the update/draw nearby leaves.
    build('effect-manager-update')
    diagnostic = parsed(['scripts/inspect-effect-added.py',
                         'build/matching/EffectManager.obj', '--json'],
                        diagnostic=True)
    guard_effect_added(diagnostic)
    exact = []
    if protect_exact:
        exact = cold_replay_exact_source(
            'src/EffectManager.cpp', EFFECT_MANAGER_EXACT_CARRIERS,
            already_built=('build/matching/EffectManager.obj',))
        exact.extend(replay_effect_real_anm_loaded_exact())
    return {'effect_added_callback': {
        'target_bytes': 51, 'candidate_bytes': diagnostic['candidate_bytes'],
        'original_fields': diagnostic['target_relocations'],
        'bound_fields': diagnostic['bound_relocations'],
        'ordinary_differences': diagnostic['differing_bytes'],
        'ordered_calls_agree': diagnostic['ordered_calls_match'],
    }}, exact


GAMEPLAY_SETUP_TARGET_SHA = '8dc5dd11495f34c6b1998653e0094195d6d891fff374d403750141e9936f0d6a'

# Independent physical target/source referent MULTISETS (not merely code
# offsets). All 35 callee identities are equal; the residual entries are
# genuine GameManager storage cursor/reload roles. Target-only 5 vs source-only
# 4 explains the outstanding 172-vs-171 field count. Preserve the ability to
# narrow these gaps; forbid unexplained NEW wrong referents.
GAMEPLAY_SETUP_TARGET_ONLY_REFERENTS = (
    ('DIR32', 0x004A7D90, 3),
    ('DIR32', 0x004A7DC4, 1),
    ('DIR32', 0x004A7E34, 1),
)
GAMEPLAY_SETUP_SOURCE_ONLY_REFERENTS = (
    ('DIR32', 0x004A7DB8, 1),
    ('DIR32', 0x004A7E28, 1),
    ('DIR32', 0x004A7E44, 2),
)


def guard_gameplay_setup_field_referents(result: dict) -> None:
    """Reject novel unresolved operand targets even at equal field totals."""
    readings = []
    for field, original_limits in (
            ('original_only_field_referents', GAMEPLAY_SETUP_TARGET_ONLY_REFERENTS),
            ('source_only_field_referents', GAMEPLAY_SETUP_SOURCE_ONLY_REFERENTS)):
        records = result.get(field)
        if not isinstance(records, list):
            raise RuntimeError('GameplaySetupThread missing original-referent multiset evidence')
        values = []
        for entry in records:
            if not isinstance(entry, dict) or set(entry) != {'type', 'address', 'count'}:
                raise RuntimeError('GameplaySetupThread malformed original-referent evidence')
            typ, address, count = entry['type'], entry['address'], entry['count']
            if (typ not in ('DIR32', 'REL32') or type(address) is not int or
                    type(count) is not int or count < 1):
                raise RuntimeError('GameplaySetupThread invalid operand type/address/multiplicity')
            values.append((typ, address, count))
        if values != sorted(values) or len(values) != len(set((t, a) for t, a, _ in values)):
            raise RuntimeError('GameplaySetupThread duplicate or shuffled operand identities')
        ceiling = {(t, a): n for t, a, n in original_limits}
        if any((t, a) not in ceiling or n > ceiling[(t, a)] for t, a, n in values):
            raise RuntimeError('GameplaySetupThread new incorrect original/codegen field referent')
        readings.append(sum(n for _, _, n in values))
    # The original 172-field inventory and source 171-field inventory are
    # checked elsewhere; their multiset-count difference must ALSO balance.
    if readings[0] - readings[1] != result.get('original_fields', 0) - result.get('bound_fields', 0):
        raise RuntimeError('GameplaySetupThread referent multiset count does not reconcile')


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
    '7ffff73fffbffffffffb0f000000000000000000000000fefffff7bdef7defff'
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
            sum(bin(x).count('1') for x in GAMEPLAY_SETUP_RESIDUAL_MASK) != 904 or
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
            result['aligned_field_identities'] < 164 or
            type(result.get('field_change_spans')) is not int or
            not 0 <= result['field_change_spans'] <= 15 or
            type(result.get('aligned_instructions')) is not int or
            # The genuine byte player-type parameter emits XOR CL,CL in an
            # already unlike physical position. The full785-byte match guard
            # above stays unchanged; instruction correspondence is diagnostic.
            result['aligned_instructions'] < 393 or
            type(count) is not int or not 0 <= count <= 904 or
            result.get('complete_bytes_equal') is not (count == 0) or
            not isinstance(differences, list) or len(differences) != count or
            any(type(i) is not int or not 0 <= i < 1689
                for i in differences) or
            differences != sorted(set(differences)) or
            any(not (GAMEPLAY_SETUP_RESIDUAL_MASK[i // 8] & (1 << (i % 8)))
                for i in differences)):
        raise RuntimeError('GameplaySetupThread 1689-byte/171-field/35-call/904-residual frontier drifted')
    if result.get('native_replay_calls') != [
            dict(symbol='?Create@ReplayManagerView@@SIPAU1@HPBD@Z',
                 target=0x421670, count=2),
            dict(symbol='?DispatchReplayStage@@YIXPAUReplayStageDispatchView@@H@Z',
                 target=0x4217E0, count=2)]:
        raise RuntimeError('GameplaySetupThread native Replay Create/Dispatch identities drifted')
    if result.get('native_opponent_selection_calls') != dict(
            symbol='?SelectOpponentConfiguration@GameManagerOpponentSelectionView@@QAEXXZ',
            target=0x415910, count=4):
        raise RuntimeError('GameplaySetupThread native opponent-selection identity drifted')
    creators = result.get('native_creator_calls')
    if (not isinstance(creators, list) or
            any(not isinstance(row, dict) or type(row.get('count')) is not int
                for row in creators) or creators != [
            dict(unit=unit, symbol=symbol, target=target, count=count)
            for unit, symbol, target, count in (
                ('background-create', '?Create@Background@@SIPAU1@H@Z', 0x403AE0, 1),
                ('player-register-chain', '?PlayerRegisterChain@@YIPAUPlayerLifecycleView@@EHH@Z', 0x41F170, 1),
                ('etama-controller-register', '?Register@EtamaController@@SIPAU1@H@Z', 0x4150D0, 1),
                ('effect-manager-create', '?Create@EffectManager@@SIPAU1@HHH@Z', 0x40D320, 2),
                ('enemy-manager-create', '?Create@EnemyManagerView@@SIPAU1@H@Z', 0x412240, 1),
                ('card-attack-create', '?Create@CardAttack@@SIPAU1@H@Z', 0x404600, 1),
                ('front-side-create', '?Create@FrontSide@@SIPAU1@H@Z', 0x41A5D0, 1),
                ('exattack-register', '?Register@ExAttackController@@SIPAU1@XZ', 0x4157D0, 1))]):
        raise RuntimeError('GameplaySetupThread native creator identities drifted')
    guard_gameplay_setup_field_referents(result)


def guard_game_manager_cleanup(result: dict) -> dict:
    """The complete newly exact 425-byte owner; diagnostics grant no credit."""
    noncrediting(result, name='GameManager gameplay cleanup', target_size=425, fields=43)
    digest = 'e31f3c176b3b735008612b07823982a646248cbae4fb8ac826e2f2cff2d6a798'
    expected = dict(target_bytes=425, candidate_bytes=425,
                    target_instructions=117, candidate_instructions=117,
                    original_fields=43, bound_fields=43, ordered_calls=12,
                    ordered_calls_agree=True, original_blocks=24, candidate_blocks=24,
                    mode2_lower_bound_branch_present=True, original_limit_double_store=True,
                    aligned_field_identities=43, field_change_spans=0,
                    aligned_instructions=117, overlap_differences=0,
                    missing_target_bytes=0, extra_candidate_bytes=0,
                    direct_graphs_agree=True, complete_bytes_equal=True,
                    target_sha256=digest, source_linked_sha256=digest, difference_offsets=[])
    trace = dict(default_reads=[0x169], all_writes=[0xA2,0x179,0x180],
                 normal_clamp_branch=[0x17E])
    if (any(type(result.get(key)) is not type(value) or result[key] != value
            for key, value in expected.items())
            or result.get('original_limit_publication') != trace
            or result.get('candidate_limit_publication') != trace):
        raise RuntimeError('CleanupGameplayState complete 425-byte/43-field exact owner regressed')
    return result


def run_game_manager(*, protect_exact: bool,
                     replay_callees_prechecked: bool = False) -> tuple[dict, list[dict]]:
    """Independently cold-validate the newly exact 1,230-byte calc boss.

    Ten original owners use the pinned /O2 /Os profile, but the exact
    Supervisor loading VMs sibling intentionally uses the no-/Os profile.
    A single cached COFF object or a profile swap invalidates the other
    group: force each actual profile to compile its own complete source TU.
    """
    primary = ('game-manager-on-update',
               'game-manager-cleanup-gameplay-state',
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
        'native_opponent_selection_calls': gameplay['native_opponent_selection_calls'],
        'original_blocks': gameplay['original_blocks'],
        'candidate_blocks': gameplay['candidate_blocks'],
        'field_change_spans': gameplay['field_change_spans'],
        'original_only_field_referents': gameplay['original_only_field_referents'],
        'source_only_field_referents': gameplay['source_only_field_referents'],
        'aligned_field_identities': gameplay['aligned_field_identities'],
        'ordinary_differences': gameplay['overlap_differences'],
        'direct_graphs_agree': gameplay['direct_graphs_agree'],
    }}
    cleanup = guard_game_manager_cleanup(parsed([
        'scripts/inspect-game-manager-cleanup.py', 'build/matching/GameManagerSetup.obj']))
    owners.append(canonical('game-manager-cleanup-gameplay-state'))
    diagnostics['game_manager_cleanup'] = {
        'target_bytes': 425,
        'candidate_bytes': cleanup['candidate_bytes'],
        'original_fields': cleanup['original_fields'],
        'bound_fields': cleanup['bound_fields'],
        'aligned_field_identities': cleanup['aligned_field_identities'],
        'aligned_instructions': cleanup['aligned_instructions'],
        'overlap_differences': cleanup['overlap_differences'],
        'mode2_lower_bound_branch_present': cleanup['mode2_lower_bound_branch_present'],
        'original_limit_double_store': cleanup['original_limit_double_store'],
        'original_limit_publication': cleanup['original_limit_publication'],
        'candidate_limit_publication': cleanup['candidate_limit_publication'],
        'direct_graphs_agree': cleanup['direct_graphs_agree'],
    }
    if protect_exact:
        owners.extend(canonical(name) for name in primary[2:])
        build('supervisor-setup-loading-vms-and-capture')
        owners.append(canonical('supervisor-setup-loading-vms-and-capture'))
        # Gameplay cleanup now calls the same exact, 133-byte
        # FrontMessageOwnerView::InitializeMessageRuntime as FrontAddedCallback.
        # The --owner all Front sweep already replayed this true member;
        # a focused gamemgr replay must independently build and check it.
        if not replay_callees_prechecked:
            build('front-message-initialize-runtime')
            owners.append(canonical('front-message-initialize-runtime'))
        # The 1,689-byte worker genuinely creates and fully releases the
        # 0x164-byte ReplayManager via two separately exact real member ABI
        # boundaries. In focused gamemgr mode force each pinned carrier; in
        # --owner all mode the earlier Replay sweep has already recompiled
        # BOTH ReplayInput profiles and independently exact-replayed BOTH
        # members, so do not double-count their canonical evidence.
        for sibling in ('replay-manager-release', 'replay-manager-create'):
            if replay_callees_prechecked:
                continue
            build(sibling)
            owners.append(canonical(sibling))
        # Dispatch is a separate 21-byte owner, outside the ReplayInput sweep.
        build('replay-stage-dispatch')
        owners.append(canonical('replay-stage-dispatch'))
        # The four worker calls share the declaration with this existing
        # exact 799-byte implementation; no UpdateProgress adapter exists.
        build('game-manager-select-opponent')
        owners.append(canonical('game-manager-select-opponent'))
    return diagnostics, owners


# Independent original SHA-256 values for the full physical owner boundaries.
ENEMY_UPDATE_SHA256 = 'a7f01c2ff0bda79d1fd17ac9170dc3560d4089457455d7928776df8556f1bc75'
ENEMY_COLLISION_SHA256 = 'e2be799860dde7b752f525cb13f4135c4666917745006a511e734e10cd2ff9c0'
ENEMY_UPDATE_WINDOWS = ((0x19A, 0x1A8, 2), (0x251, 0x2C1, 11),
                        (0x843, 0x854, 0), (0x9BC, 0xA83, 0))
ENEMY_COLLISION_WINDOWS = ((0x1A, 0x40, 31),)

# Fresh original SHA-pinned PE/full-COFF 3,900-byte owner: only these 13
# positions remain mismatched. Preserve the other 3,887 matching physical bytes.
ENEMY_UPDATE_ORIGINAL_RESIDUAL_OFFSETS = frozenset((
    0x019C, 0x01A3, 0x0252, 0x0263, 0x026D, 0x0272, 0x0294, 0x029A,
    0x029C, 0x02A5, 0x02A9, 0x02B7, 0x02BD,
))


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
    if result.get('native_timer_greater') != dict(
            unit='zun-timer-greater', source='src/EclManager.cpp', calls=4,
            bytes=20, fields=0, same_object_definition=True,
            original_body_sha256='f4946c0b7b22e556c3b758435c2c7bdd468104acc285e40dda5ec2a737d36126'):
        raise RuntimeError('Enemy native Timer greater definition proof changed')
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
    checked = physical_difference_windows(result, count_key='complete_differences',
                                          windows=ENEMY_UPDATE_WINDOWS)
    actual = {int(x, 16) for x in result['difference_offsets']}
    if not actual.issubset(ENEMY_UPDATE_ORIGINAL_RESIDUAL_OFFSETS):
        raise RuntimeError('EnemyManager changed an already matched original physical byte')
    return checked


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



# These five independently canonical-exact callees now have their REAL C++
# owner/symbol references in the 3,900-byte Enemy update. Keep all physical
# source-carrier profiles protected: ANM ExecuteScript has its separate /Ob0
# carrier; the two ASCII boss-marker members share a single /Ob1 object.
# All target VAs were established independently of the candidate COFF binder.
ENEMY_REAL_CALLEE_EXACT_CARRIERS = (
    ('src/SoundPlayerProcessQueues.cpp',
     (('soundplayer-play-positioned-sound-by-idx', 0x0043E380,
       '?PlaySoundPositionedByIdx@SoundPlayer@@QAEXHM@Z'),)),
    ('src/AnmManagerExecuteScript.cpp',
     (('anm-manager-execute-script', 0x00436F30,
       '?ExecuteScript@AnmManager@@QAEHPAUAnmVm@@@Z'),)),
    ('src/AsciiBossMarkerVm.cpp',
     (('ascii-boss-marker-set-position', 0x004067F0,
       '?SetBossMarkerPosition@AsciiBossMarkerVmView@@QAEXHPBUFloat3@@@Z'),
      ('ascii-boss-marker-set-interrupt', 0x004067D0,
       '?SetBossMarkerInterrupt@AsciiBossMarkerVmView@@QAEXHF@Z'))),
    ('src/AsciiManagerBossMarker.cpp',
     (('ascii-manager-set-boss-marker-state', 0x0040F890,
       '?SetBossMarkerState@AsciiManager@@QAEXHH@Z'),)),
)


def replay_enemy_real_callee_exact() -> list[dict]:
    """Cold verify all five authentic callee-family members once per carrier.

    This is *additional* exact-sibling regression of already-accepted source,
    never extra exactness credit for the still-unmatched Enemy OnUpdate boss.
    Fail closed on a missing/repointed unit or a split marker source profile.
    """
    with (ROOT / 'config/match-units.toml').open('rb') as stream:
        registry = tomllib.load(stream)['units']
    result = []
    objects = set()
    for source, group in ENEMY_REAL_CALLEE_EXACT_CARRIERS:
        members = [registry.get(name) for name, _, _ in group]
        if any(not isinstance(member, dict) for member in members):
            raise RuntimeError('Enemy authentic callee match unit absent')
        obj = members[0].get('object')
        profile = members[0].get('profile')
        if (not isinstance(obj, str) or not obj.startswith('build/matching/') or
                obj in objects or not isinstance(profile, list) or not profile or
                any(not isinstance(flag, str) or not flag for flag in profile)):
            raise RuntimeError('Enemy authentic callee carrier/profile changed')
        objects.add(obj)
        for (name, address, symbol), member in zip(group, members):
            if (member.get('target_address') != address or
                    member.get('object') != obj or
                    member.get('profile') != profile or
                    member.get('symbol') != symbol or member.get('source') != source):
                raise RuntimeError('Enemy exact callee identity/carrier changed: '+name)
        build(group[0][0])
        result.extend(canonical(name) for name, _, _ in group)
    return result


def run_enemy(*, protect_exact: bool) -> tuple[dict, list[dict]]:
    # One source-cold EclManager TU now owns the nonexact 3,900-byte
    # calc boss, exact 151-byte effects leaf, original-exact 56-byte Float3
    # division, and both exact 205/82-byte collision helpers. The real
    # Float3 divider + AppendBoxRecord bodies eliminate ten genuine target
    # mismatches inside the caller; canonical replays protect all four leaves.
    build('enemy-attached-effect-update')
    main = parsed(['scripts/inspect-enemy-manager-update.py',
                   'build/matching/EclManager.obj', '--json'])
    main_frontier = guard_enemy_update(main)

    # Use the same freshly compiled EclManager.obj for every source-
    # commit-relevant codegen check; a historical PlayerCollisionAppendBox
    # object is no longer an independent source of authority.
    obj = 'build/matching/EclManager.obj'
    collision = parsed(['scripts/inspect-enemy-collision-box.py', obj, '--json'])
    collision_frontier = guard_enemy_collision(collision)
    exact = []
    if protect_exact:
        exact.extend(cold_replay_exact_source(
            'src/EclManager.cpp', ECL_EXACT_CARRIERS, already_built=(obj,)))
        build('enemy-manager-draw-impl')
        exact.append(canonical('enemy-manager-draw-impl'))
        # OnUpdate's real 0x214-byte bullet descriptor and actual Etama
        # dispatch cross the Enemy TU. Cold-verify both independent original
        # exact callees, not a private lookalike method ABI.
        for dependency in ('bullet-spawn-descriptor-constructor',
                           'bullet-pattern-primary'):
            build(dependency)
            exact.append(canonical(dependency))
        # Independently replay every genuine Sound/ANM/ASCII target after
        # correcting the Enemy TU's formerly duplicated fake C++ symbols.
        exact.extend(replay_enemy_real_callee_exact())
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


BULLET_REAL_SOUND_FIELDS = {
    '?g_SoundPlayer@@3VSoundPlayer@@A': ('DIR32', 0x004DC698),
    '?PlaySoundByIdx@SoundPlayer@@QAEXHH@Z': ('REL32', 0x0043E2F0),
    '?PlaySoundPositionedByIdx@SoundPlayer@@QAEXHM@Z': ('REL32', 0x0043E380),
}


def validate_bullet_sound_real_owners(units: dict) -> None:
    """Reject a fake receiver, wrong real sound source, or missing true calls.

    Each previously exact function's complete target-bound field identity is
    independently checked again by canonical() after the cold original-profile
    build. The pure guard pins the C++ type/callee ownership (which bare
    numerical address matching cannot prove by itself).
    """
    groups = (
        ('src/BulletManager.cpp', BULLET_MAIN_EXACT_CARRIERS,
         'BulletSoundPlayerView', ('PlaySoundByIdx(', 'PlaySoundPositionedByIdx(')),
        ('src/BulletAdvanceTransformProgram.cpp', BULLET_ADVANCE_EXACT_CARRIERS,
         'BulletAdvanceSoundPlayerView', ('PlaySoundByIdx(', 'PlaySoundPositionedByIdx(')),
    )
    expected = {'?g_SoundPlayer@@3VSoundPlayer@@A': 9,
                '?PlaySoundByIdx@SoundPlayer@@QAEXHH@Z': 6,
                '?PlaySoundPositionedByIdx@SoundPlayer@@QAEXHM@Z': 3}
    actual = Counter()
    for source, carriers, fake, calls in groups:
        exact_source_carriers(source, carriers, units)
        content = (ROOT / source).read_text()
        if ('#include "SoundPlayer.hpp"' not in content or fake in content or
                any(('g_SoundPlayer.'+call) not in content for call in calls)):
            raise RuntimeError('Bullet/Etama source uses a fake or missing SoundPlayer owner: '+source)
        for unit in units.values():
            if unit.get('source') != source:
                continue
            if unit.get('profile') != list(_SCREEN_EFFECT_BASE_PROFILE):
                raise RuntimeError('Bullet source original compiler profile changed')
            for row in unit['relocations']:
                symbol = row['symbol']
                if 'SoundPlayer' in symbol or 'PlaySound' in symbol:
                    real = BULLET_REAL_SOUND_FIELDS.get(symbol)
                    if real is None or (row['type'], row['target'],
                                        row.get('addend', 0)) != (*real, 0):
                        raise RuntimeError('Bullet/Etama exact unit uses a false sound link identity')
                    actual[symbol] += 1
    if dict(actual) != expected:
        raise RuntimeError('Bullet/Etama original sound call/global cardinality changed')
    for name, symbol, addr, size in (
            ('soundplayer-play-sound-by-idx',
             '?PlaySoundByIdx@SoundPlayer@@QAEXHH@Z', 0x0043E2F0, 140),
            ('soundplayer-play-positioned-sound-by-idx',
             '?PlaySoundPositionedByIdx@SoundPlayer@@QAEXHM@Z', 0x0043E380, 153)):
        row = units.get(name)
        if (not isinstance(row, dict) or
                row.get('source') != 'src/SoundPlayerProcessQueues.cpp' or
                row.get('object') != 'build/matching/SoundPlayerPlaySound.obj' or
                row.get('symbol') != symbol or
                row.get('target_address') != addr or row.get('size') != size or
                row.get('profile') != list(_SCREEN_EFFECT_BASE_PROFILE)):
            raise RuntimeError('Bullet genuine sound callee source/provenance changed: '+name)


# CardAttack's original large OnUpdate and Start callback are two further
# genuine consumers of SoundPlayer::PlaySoundByIdx. Their old private
# CardAttack*SoundPlayerView aliases produced phantom C++ linker identities;
# replacing them with SoundPlayer is raw-byte neutral for nine existing exact
# source bodies across five genuine original VC7.1 compilation carriers.
CARD_REAL_SOUND_EXACT_CARRIERS = {
    'build/matching/CardAttack.obj': 5,
    'build/matching/CardAttackCtor.obj': 1,
    'build/matching/CardAttackOnDraw.obj': 1,
    'build/matching/CardAttackOnUpdate.obj': 1,
}
CARD_START_SOUND_EXACT_CARRIERS = {'build/matching/CardAttackStart.obj': 1}


def validate_card_real_sound_owners(units: dict) -> None:
    expected = (('src/CardAttack.cpp', CARD_REAL_SOUND_EXACT_CARRIERS,
                 'CardAttackSoundPlayerView'),
                ('src/CardAttackStart.cpp', CARD_START_SOUND_EXACT_CARRIERS,
                 'CardAttackStartSoundPlayerView'))
    sound = units.get('soundplayer-play-sound-by-idx')
    if (not isinstance(sound,dict) or
            sound.get('source') != 'src/SoundPlayerProcessQueues.cpp' or
            sound.get('object') != 'build/matching/SoundPlayerPlaySound.obj' or
            sound.get('target_address') != 0x0043E2F0 or
            sound.get('symbol') != '?PlaySoundByIdx@SoundPlayer@@QAEXHH@Z' or
            sound.get('size') != 140 or
            sound.get('profile') != list(_SCREEN_EFFECT_BASE_PROFILE)):
        raise RuntimeError('Card real SoundPlayer exact method source changed')
    original_sound_fields = {
        '?g_SoundPlayer@@3VSoundPlayer@@A': ('DIR32', 0x004DC698),
        '?PlaySoundByIdx@SoundPlayer@@QAEXHH@Z': ('REL32', 0x0043E2F0),
    }
    # Independently fixed original retail owner extents, field offsets and
    # the full two-entry sound call pair. Pure provenance tests must reject
    # malformed COFF manifests before any new compiler run receives credit.
    exact_sound_sites = {
        'card-attack-on-update': (0x004041F0, 536, (402, 425)),
        'card-attack-start': (0x00403E50, 923, (562, 567)),
    }
    counts=Counter()
    for source,carriers,fake in expected:
        exact_source_carriers(source, carriers, units)
        text=(ROOT/source).read_text()
        if ('#include "SoundPlayer.hpp"' not in text or
                fake in text or 'g_SoundPlayer.PlaySoundByIdx(' not in text):
            raise RuntimeError('Card large callback uses false SoundPlayer receiver: '+source)
        for unit_name, unit in units.items():
            if unit.get('source') != source:
                continue
            flags = unit.get('profile')
            if (not isinstance(flags,list) or
                    any(x not in ('/Ob0','/Ob1') for x in flags if x.startswith('/Ob')) or
                    '/O2' not in flags or '/Gr' not in flags):
                raise RuntimeError('Card original VC7.1 compile family changed')
            sound_rows=[r for r in unit['relocations']
                        if 'SoundPlayer' in r['symbol'] or 'PlaySound' in r['symbol']]
            pinned = exact_sound_sites.get(unit_name)
            if pinned is None and sound_rows:
                raise RuntimeError('Card unreviewed callback acquired new sound fields')
            if pinned is not None:
                original_va, original_size, offsets=pinned
                if (unit.get('target_address') != original_va or
                        unit.get('size') != original_size or
                        tuple(row['offset'] for row in sound_rows) != offsets or
                        tuple(row['symbol'] for row in sound_rows) !=
                        tuple(original_sound_fields)):
                    raise RuntimeError('Card original 536/923-byte sound field extent/order drifted')
            for field in sound_rows:
                symbol=field['symbol']
                if (symbol not in original_sound_fields or
                        (field['type'],field['target'],field.get('addend',0)) !=
                        (*original_sound_fields[symbol],0)):
                    raise RuntimeError('Card large callback authentic sound field drifted')
                counts[symbol]+=1
    if dict(counts) != {'?g_SoundPlayer@@3VSoundPlayer@@A':2,
                        '?PlaySoundByIdx@SoundPlayer@@QAEXHH@Z':2}:
        raise RuntimeError('Card large callback original sound call order/count drifted')


def run_card_real_sound(*, protect_exact: bool,
                        sound_prechecked: bool = False) -> tuple[dict, list[dict]]:
    with (ROOT/'config/match-units.toml').open('rb') as file:
        units=tomllib.load(file)['units']
    validate_card_real_sound_owners(units)
    if not protect_exact:
        return {},[]
    exact=cold_replay_exact_source('src/CardAttack.cpp',CARD_REAL_SOUND_EXACT_CARRIERS)
    exact.extend(cold_replay_exact_source('src/CardAttackStart.cpp',
                                         CARD_START_SOUND_EXACT_CARRIERS))
    if not sound_prechecked:
        build('soundplayer-play-sound-by-idx')
        exact.append(canonical('soundplayer-play-sound-by-idx'))
    return {},exact


# The SAME real global 111-byte fastcall math function is consumed by the
# big 2,216-byte EtamaController::OnUpdate and four neighboring exact Bullet
# functions. Earlier BulletManager.cpp compiled `__stdcall` fake math labels
# at the correct numeric VA; preserve full original address AND C++ identity.
BULLET_REAL_ANGLE_CALLS = {
    'bullet-update-polar-acceleration': 0x52,
    'bullet-update-aimed-direction-change': 0x81,
    'bullet-update-boundary-bounce': 0x8D,
    'etama-spawn-single-bullet': 0x400,
    'etama-controller-update': 0x546,
}


def validate_bullet_real_angle_abi(units: dict) -> None:
    """Protect five true original math call offsets across 18 exact leaves/bosses."""
    source=(ROOT/'src/BulletManager.cpp').read_text()
    if (source.count('#include "RuntimeAngleMath.hpp"')!=1 or
            'extern float __stdcall AddNormalizeAngle' in source or
            source.count('AddNormalizeAngle(')!=5):
        raise RuntimeError('Bullet main Etama controller uses false math ABI/source')
    math=units.get('runtime-add-normalize-angle')
    if (not isinstance(math,dict) or
            math.get('source')!='src/RuntimeAngleMath.cpp' or
            math.get('object')!='build/matching/RuntimeAngleMath.obj' or
            math.get('symbol')!='?AddNormalizeAngle@@YIMMM@Z' or
            math.get('target_address')!=0x0042AED0 or math.get('size')!=111 or
            math.get('profile')!=list(_SCREEN_EFFECT_BASE_PROFILE)):
        raise RuntimeError('Bullet math callee must be genuine original 111-byte exact global')
    primary=units.get('etama-controller-update')
    if (not isinstance(primary,dict) or
            primary.get('source')!='src/BulletManager.cpp' or
            primary.get('target_address')!=0x004146F0 or
            primary.get('size')!=2195 or primary.get('compare_size')!=2216 or
            primary.get('profile')!=list(_SCREEN_EFFECT_BASE_PROFILE) or
            len(primary.get('relocations',[]))!=88):
        raise RuntimeError('Etama big OnUpdate original 2216-byte exact carrier was changed')
    observed={}
    all_sound=[]
    for name,unit in units.items():
        if unit.get('source')!='src/BulletManager.cpp':
            continue
        if unit.get('profile')!=list(_SCREEN_EFFECT_BASE_PROFILE):
            raise RuntimeError('Bullet math caller compiler profile changed')
        for field in unit.get('relocations',[]):
            if 'AddNormalizeAngle' in field.get('symbol',''):
                if name in observed:
                    raise RuntimeError('Bullet original math call count/identity duplicated')
                observed[name]=(field.get('offset'),field.get('type'),
                                field.get('symbol'),field.get('target'),
                                field.get('addend',0))
    if set(observed)!=set(BULLET_REAL_ANGLE_CALLS):
        raise RuntimeError('Bullet Etama expected five actual global math calls disappeared')
    for unit,offset in BULLET_REAL_ANGLE_CALLS.items():
        if observed[unit]!=(offset,'REL32','?AddNormalizeAngle@@YIMMM@Z',0x0042AED0,0):
            raise RuntimeError('Bullet original Etama callsite symbol/offset/VA changed: '+unit)
    inspector=(ROOT/'scripts/inspect-etama-update.py').read_text()
    if ('?AddNormalizeAngle@@YGMMM@Z' in inspector or
            '?AddNormalizeAngle@@YIMMM@Z' not in inspector):
        raise RuntimeError('Etama original-owner update inspector uses obsolete math symbol')


def replay_bullet_real_angle_exact(*, already_checked: bool) -> list[dict]:
    with (ROOT/'config/match-units.toml').open('rb') as f:
        validate_bullet_real_angle_abi(tomllib.load(f)['units'])
    if already_checked:
        return []
    build('runtime-add-normalize-angle')
    return [canonical('runtime-add-normalize-angle')]


# Four big Etama OnUpdate scripts dispatch through the REAL AnmManager
# singleton/class. The previous BulletAnmManagerView alias hid different C++
# decorated names behind the same original numeric global/callee addresses.
# Pin every independent retail PE operand location, not only the counts.
BULLET_TRUE_ANM_GLOBAL = '?g_AnmManager@@3PAVAnmManager@@A'
BULLET_TRUE_ANM_SCRIPT = '?ExecuteScript@AnmManager@@QAEHPAUAnmVm@@@Z'
BULLET_TRUE_ANM_SITES = (
    (0x176, 'DIR32', BULLET_TRUE_ANM_GLOBAL, 0x004DC550),
    (0x17B, 'REL32', BULLET_TRUE_ANM_SCRIPT, 0x00436F30),
    (0x3F4, 'DIR32', BULLET_TRUE_ANM_GLOBAL, 0x004DC550),
    (0x3F9, 'REL32', BULLET_TRUE_ANM_SCRIPT, 0x00436F30),
    (0x596, 'DIR32', BULLET_TRUE_ANM_GLOBAL, 0x004DC550),
    (0x5A2, 'REL32', BULLET_TRUE_ANM_SCRIPT, 0x00436F30),
    (0x837, 'DIR32', BULLET_TRUE_ANM_GLOBAL, 0x004DC550),
    (0x83D, 'REL32', BULLET_TRUE_ANM_SCRIPT, 0x00436F30),
)


def validate_bullet_true_anm_manager_abi(units: dict) -> None:
    """Enforce real 9,776-byte AnmManager callee in 2,216-byte Etama boss."""
    source=(ROOT/'src/BulletManager.cpp').read_text()
    header=(ROOT/'src/AnmManager.hpp').read_text()
    if (source.count('#include "AnmManager.hpp"')!=1 or
            'BulletAnmManagerView' in source or
            source.count('g_AnmManager->ExecuteScript(')!=6 or
            'extern AnmManager *g_AnmManager;' not in header or
            'int ExecuteScript(AnmVm *vm);' not in header):
        raise RuntimeError('Etama OnUpdate uses fabricated ANM manager source ABI')
    exact_source_carriers('src/BulletManager.cpp',BULLET_MAIN_EXACT_CARRIERS,units)
    unit=units.get('etama-controller-update')
    if (not isinstance(unit,dict) or
            unit.get('source')!='src/BulletManager.cpp' or
            unit.get('object')!='build/matching/BulletManager.obj' or
            unit.get('target_address')!=0x004146F0 or unit.get('size')!=2195 or
            unit.get('compare_size')!=2216 or
            len(unit.get('relocations',[]))!=88 or
            unit.get('profile')!=list(_SCREEN_EFFECT_BASE_PROFILE)):
        raise RuntimeError('Etama true ANM caller lost entire 2,216-byte original source owner')
    found=[(f.get('offset'),f.get('type'),f.get('symbol'),f.get('target'),f.get('addend',0))
           for f in unit['relocations'] if
           'AnmManager' in f.get('symbol','') or 'ExecuteScript@' in f.get('symbol','')]
    expected=[(*site,0) for site in BULLET_TRUE_ANM_SITES]
    if found!=expected:
        raise RuntimeError('Etama OnUpdate original native ANM field offsets/order/callee drifted')
    for name,row in units.items():
        if row.get('source')!='src/BulletManager.cpp':
            continue
        if row.get('profile')!=list(_SCREEN_EFFECT_BASE_PROFILE):
            raise RuntimeError('Etama native ANM source real VC7.1 profile drifted')
        if any('BulletAnmManagerView' in f.get('symbol','')
               for f in row.get('relocations',[])):
            raise RuntimeError('Etama exact neighbor reintroduced fake ANM C++ class')
    callee=units.get('anm-manager-execute-script')
    profile=tuple('/Ob0' if flag=='/Ob1' else flag
                  for flag in _SCREEN_EFFECT_BASE_PROFILE)
    if (not isinstance(callee,dict) or
            callee.get('source')!='src/AnmManagerExecuteScript.cpp' or
            callee.get('object')!='build/matching/AnmManagerExecuteScriptExact.obj' or
            callee.get('target_address')!=0x00436F30 or
            callee.get('symbol')!=BULLET_TRUE_ANM_SCRIPT or
            callee.get('size')!=9358 or callee.get('compare_size')!=9776 or
            callee.get('profile')!=list(profile)):
        raise RuntimeError('Etama original script callee not independently canonical 9,776-byte exact')


def replay_bullet_true_anm_exact(*, already_checked: bool) -> list[dict]:
    with (ROOT/'config/match-units.toml').open('rb') as stream:
        validate_bullet_true_anm_manager_abi(tomllib.load(stream)['units'])
    if already_checked:
        return []
    build('anm-manager-execute-script')
    return [canonical('anm-manager-execute-script')]


# The side EtamaController's last pointer (+0x25E1BC) is an actual
# AnmLoaded*, not a second source-only BulletAnmLoadedView C++ class.
# The real standalone original AnmLoaded::SetSprite @0x00436AC0 is 189/189
# byte exact. Cover all eight original call fields across BulletManager,
# Bullet Advance and Enemy ECL *plus* the exact laser/draw consumers that
# once recast the same pointer to the native class.
BULLET_TRUE_LOADED_SET_SPRITE = '?SetSprite@AnmLoaded@@QAEHPAUAnmVm@@H@Z'
BULLET_TRUE_LOADED_CALLS = {
    'etama-controller-select-bullet-sprite': (
        'src/BulletManager.cpp', 0x004124E0, 161, 161, (0x52,0x82,0x96)),
    'etama-spawn-single-bullet': (
        'src/BulletManager.cpp', 0x00412960, 1893, 1932, (0x577,0x605)),
    'bullet-advance-transform-program': (
        'src/BulletAdvanceTransformProgram.cpp',0x004141C0,1236,1320,(0xCC,)),
    'enemy-ecl-opcode2': (
        'src/EnemyEclOpcode2.cpp',0x0040C530,574,574,(0x71,0x158)),
}
BULLET_TRUE_LOADED_CARRIERS = {
    'src/EnemyEclOpcode2.cpp': {'build/matching/EnemyEclOpcode2.obj':1},
    'src/BulletLaserSpawn.cpp': {'build/matching/BulletLaserSpawn.obj':1},
    'src/EtamaControllerDraw.cpp': {'build/matching/EtamaControllerDraw.obj':2},
}


def validate_bullet_true_loaded_abi(units: dict) -> None:
    """Original PE field-level native C++ pointer proof across four real TUs."""
    bullet_header=(ROOT/'src/BulletManager.hpp').read_text()
    anm_header=(ROOT/'src/AnmManager.hpp').read_text()
    if (bullet_header.count('class AnmLoaded;')!=1 or
            bullet_header.count('AnmLoaded *bulletAnm;')!=1 or
            'BulletAnmLoadedView' in bullet_header or
            'offsetof(EtamaController, bulletAnm) == 0x25E1BC' not in bullet_header or
            'int SetSprite(AnmVm *vm, int spriteIndex);' not in anm_header):
        raise RuntimeError('Etama side controller original +0x25E1BC has fake ANM C++ storage/ABI')
    for source in ('src/BulletManager.cpp','src/BulletAdvanceTransformProgram.cpp',
                   'src/EnemyEclOpcode2.cpp','src/EtamaControllerDraw.cpp',
                   'src/BulletLaserSpawn.cpp'):
        actual=(ROOT/source).read_text()
        if ('BulletAnmLoadedView' in actual or
                '#include "AnmManager.hpp"' not in actual):
            raise RuntimeError('Etama / Enemy code imported fake ANM class: '+source)
    drawing=(ROOT/'src/EtamaControllerDraw.cpp').read_text()
    laser=(ROOT/'src/BulletLaserSpawn.cpp').read_text()
    if ('controller->bulletAnm = anm;' not in drawing or
            re.search(r'controller->bulletAnm\s*->SetAndExecuteScriptIdx\(', drawing) is None or
            'this->bulletAnm->SetSprite(' not in laser):
        raise RuntimeError('Etama/laser exact sources are not true native AnmLoaded receivers')
    exact_source_carriers('src/BulletManager.cpp',BULLET_MAIN_EXACT_CARRIERS,units)
    exact_source_carriers('src/BulletAdvanceTransformProgram.cpp',
                          BULLET_ADVANCE_EXACT_CARRIERS,units)
    for source,carriers in BULLET_TRUE_LOADED_CARRIERS.items():
        exact_source_carriers(source,carriers,units)
    names=set()
    for name,(source,va,size,complete,sites) in BULLET_TRUE_LOADED_CALLS.items():
        row=units.get(name)
        if (not isinstance(row,dict) or row.get('source')!=source or
                row.get('target_address')!=va or row.get('size')!=size or
                row.get('compare_size',size)!=complete or
                row.get('profile')!=list(_SCREEN_EFFECT_BASE_PROFILE)):
            raise RuntimeError('original Etama/Enemy/Advance exact SetSprite owner changed: '+name)
        relevant=[(f.get('offset'),f.get('type'),f.get('symbol'),f.get('target'),f.get('addend',0))
                  for f in row['relocations'] if
                  'SetSprite@' in f.get('symbol','') or
                  'BulletAnmLoadedView' in f.get('symbol','')]
        expected=[(offset,'REL32',BULLET_TRUE_LOADED_SET_SPRITE,0x00436AC0,0)
                  for offset in sites]
        if relevant!=expected:
            raise RuntimeError('native original SetSprite C++ identity/field offset/order changed: '+name)
        names.add(name)
    if len(names)!=4:
        raise RuntimeError('true Bullet AnmLoaded SetSprite original caller census incomplete')
    for unit in units.values():
        if unit.get('source') not in (
                'src/BulletManager.cpp','src/BulletAdvanceTransformProgram.cpp',
                'src/EtamaControllerDraw.cpp','src/EnemyEclOpcode2.cpp',
                'src/BulletLaserSpawn.cpp'):
            continue
        if any('BulletAnmLoadedView' in f.get('symbol','') for f in unit.get('relocations',[])):
            raise RuntimeError('false C++ AnmLoaded class reintroduced on exact sibling')
    callee=units.get('anm-loaded-set-sprite')
    if (not isinstance(callee,dict) or
            callee.get('source')!='src/AnmLoadedSetSprite.cpp' or
            callee.get('object')!='build/matching/AnmLoadedSetSprite.obj' or
            callee.get('target_address')!=0x00436AC0 or callee.get('size')!=189 or
            callee.get('symbol')!=BULLET_TRUE_LOADED_SET_SPRITE or
            callee.get('profile')!=list(_SCREEN_EFFECT_BASE_PROFILE)):
        raise RuntimeError('native AnmLoaded SetSprite actual independent 189-byte exact method drifted')


def replay_bullet_true_loaded_exact(*, etama_prechecked: bool) -> list[dict]:
    """Extra full owner replays outside the 18+1 Bullet/Advance carriers."""
    with (ROOT/'config/match-units.toml').open('rb') as stream:
        validate_bullet_true_loaded_abi(tomllib.load(stream)['units'])
    result=[]
    for source,carriers in BULLET_TRUE_LOADED_CARRIERS.items():
        if etama_prechecked and source=='src/EtamaControllerDraw.cpp':
            continue
        result.extend(cold_replay_exact_source(source,carriers))
    build('anm-loaded-set-sprite')
    result.append(canonical('anm-loaded-set-sprite'))
    return result


def run_bullet_sound(*, protect_exact: bool, sound_prechecked: bool = False,
                     real_angle_prechecked: bool = False,
                     real_anm_prechecked: bool = False,
                     etama_prechecked: bool = False) -> tuple[dict, list[dict]]:
    """Cold recheck all nineteen genuine Etama/Bullet source carriers.

    In the complete all-owner sweep the two independently exact sound leaves
    have already been checked by RunEcl and Enemy; in focused bullet mode
    compile the genuine sound source too. The 19 exact owners are never
    promoted by this function; canonical() only verifies retained ledger rows.
    """
    with (ROOT / 'config/match-units.toml').open('rb') as stream:
        units = tomllib.load(stream)['units']
    validate_bullet_sound_real_owners(units)
    validate_bullet_real_angle_abi(units)
    validate_bullet_true_anm_manager_abi(units)
    validate_bullet_true_loaded_abi(units)
    if not protect_exact:
        return {}, []
    exact = cold_replay_exact_source('src/BulletManager.cpp', BULLET_MAIN_EXACT_CARRIERS)
    exact.extend(cold_replay_exact_source(
        'src/BulletAdvanceTransformProgram.cpp', BULLET_ADVANCE_EXACT_CARRIERS))
    if not sound_prechecked:
        build('soundplayer-play-sound-by-idx')
        exact.extend((canonical('soundplayer-play-sound-by-idx'),
                      canonical('soundplayer-play-positioned-sound-by-idx')))
    exact.extend(replay_bullet_real_angle_exact(
        already_checked=real_angle_prechecked))
    exact.extend(replay_bullet_true_anm_exact(
        already_checked=real_anm_prechecked))
    exact.extend(replay_bullet_true_loaded_exact(etama_prechecked=etama_prechecked))
    return {}, exact


def unique_canonical_siblings(checks: list[dict]) -> list[dict]:
    """Report distinct validated originals without double-counting a replay.

    Calling a complete Oracle twice is allowed; presenting two identical
    function addresses as two separate exact credits is not.
    """
    unique: dict[str, dict] = {}
    for check in checks:
        unit = check.get('unit') if isinstance(check, dict) else None
        if not isinstance(unit, str) or not unit:
            raise RuntimeError('empty canonical sibling identity')
        if unit in unique:
            if unique[unit] != check:
                raise RuntimeError('same canonical unit has conflicting cold replay results: '+unit)
            continue
        unique[unit] = check
    return list(unique.values())


# The genuine Etama OnDraw translation unit contains a different 232-byte
# exact bullet-draw leaf, which invokes the same retail 0x0042AED0 math
# routine as 15K RunEcl. The former Ecl/Etama source-only stdcall forward
# declaration was NOT the C++ owner of this native __fastcall runtime leaf.
ETAMA_TRUE_ANGLE_CALL = ('bullet-draw-single', 0x00413AF0, 215, 232,
                         0xB7, '?AddNormalizeAngle@@YIMMM@Z', 0x0042AED0)


def validate_etama_real_angle_abi(units: dict) -> None:
    """Reject invented Etama math callee ABI and wrong full callsite VA."""
    source=(ROOT/'src/EtamaControllerDraw.cpp').read_text()
    header=(ROOT/'src/RuntimeAngleMath.hpp').read_text()
    if (source.count('#include "RuntimeAngleMath.hpp"') != 1 or
            'extern float __stdcall AddNormalizeAngle' in source or
            source.count('AddNormalizeAngle(') != 1 or
            header.count('float __fastcall AddNormalizeAngle(float angle, float delta);') != 1):
        raise RuntimeError('Etama uses non-native angle runtime math C++ source/ABI')
    name,va,logical,physical,offset,method,call_target=ETAMA_TRUE_ANGLE_CALL
    src=units.get(name)
    if (not isinstance(src,dict) or src.get('target_address')!=va or
            src.get('size')!=logical or src.get('compare_size')!=physical or
            src.get('source')!='src/EtamaControllerDraw.cpp' or
            src.get('object')!='build/matching/EtamaControllerDraw.obj' or
            src.get('profile')!=list(_SCREEN_EFFECT_BASE_PROFILE)):
        raise RuntimeError('Etama independently exact DrawSingleBullet source/profile changed')
    fields=src.get('relocations')
    found=[row for row in fields if row.get('symbol')==method or
           'AddNormalizeAngle' in row.get('symbol','')]
    if (len(found)!=1 or
            (found[0].get('offset'),found[0].get('type'),found[0].get('symbol'),
             found[0].get('target'),found[0].get('addend',0)) !=
            (offset,'REL32',method,call_target,0)):
        raise RuntimeError('Etama original bullet-draw math callsite/callee identity changed')
    unit=units.get('runtime-add-normalize-angle')
    if (not isinstance(unit,dict) or unit.get('source')!='src/RuntimeAngleMath.cpp' or
            unit.get('object')!='build/matching/RuntimeAngleMath.obj' or
            unit.get('profile')!=list(_SCREEN_EFFECT_BASE_PROFILE) or
            unit.get('symbol')!=method or unit.get('target_address')!=call_target or
            unit.get('size')!=111 or unit.get('relocations') is None):
        raise RuntimeError('Etama uses nonexistent/altered original 111-byte math callee')
    # Guard the second exact same-TU callback: a private $L compiler rebinding
    # cannot count as a new source function or retarget the original 832 bytes.
    added=units.get('etama-controller-added-callback')
    if (not isinstance(added,dict) or
            added.get('source')!=src['source'] or added.get('object')!=src['object'] or
            added.get('target_address')!=0x00413E40 or added.get('size')!=598 or
            added.get('compare_size')!=832):
        raise RuntimeError('Etama 832-byte already-exact AddedCallback lost shared source carrier')


def replay_etama_real_angle_exact(*, already_checked: bool) -> list[dict]:
    with (ROOT/'config/match-units.toml').open('rb') as f:
        validate_etama_real_angle_abi(tomllib.load(f)['units'])
    if already_checked:
        return []
    build('runtime-add-normalize-angle')
    return [canonical('runtime-add-normalize-angle')]


def run_etama(*, protect_exact: bool,
              real_angle_prechecked: bool = False) -> tuple[dict, list[dict]]:
    with (ROOT/'config/match-units.toml').open('rb') as stream:
        validate_etama_real_angle_abi(tomllib.load(stream)['units'])
    build('bullet-draw-single')
    result = noncrediting(parsed(['scripts/inspect-etama-draw.py',
                                 'build/matching/EtamaControllerDraw.obj', '--json'],
                                diagnostic=True), name='Etama OnDraw',
                          target_size=600, fields=27)
    guard_etama_draw(result)
    exact_units = []
    if protect_exact:
        exact_units = cold_replay_exact_source(
            'src/EtamaControllerDraw.cpp', ETAMA_DRAW_EXACT_CARRIERS,
            already_built=('build/matching/EtamaControllerDraw.obj',))
        build('etama-controller-update')
        exact_units.append(canonical('etama-controller-update'))
        exact_units.extend(replay_etama_real_angle_exact(
            already_checked=real_angle_prechecked))
    return {'etama_draw': {
        'target_bytes': 600, 'candidate_bytes': result['candidate_bytes'],
        'overlapping_differences': result['overlapping_byte_differences'],
        'direct_graphs_agree': result['complete_direct_graphs_agree'],
    }}, exact_units


TYPE19_UPDATE_TARGET_SHA256 = '38c97cf98c0183ae6f75446457d495e4d8e94683e4c2920461fd580aaf184014'
# Historical 44-byte residual with real Float3 visibility and the live Player
# slot. The active exact owner below now protects all 821 physical bytes.
TYPE19_UPDATE_ORIGINAL_RESIDUAL_OFFSETS = frozenset((
    0x008, 0x009, 0x00A, 0x00C, 0x00F, 0x014, 0x029, 0x045,
    0x048, 0x04B, 0x068, 0x069, 0x09E, 0x0B4, 0x0F4, 0x0FA,
    0x111, 0x12A, 0x15E, 0x173, 0x1AB, 0x1B4, 0x1BC, 0x1C7,
    0x1E8, 0x2AA, 0x2B2, 0x2BE, 0x2C2, 0x2C6,
    0x2CA, 0x2D6, 0x2E2, 0x2E4, 0x2E7, 0x2EB, 0x2EE, 0x2F4,
    0x2F9, 0x2FF, 0x306, 0x309, 0x323, 0x326,
))


def guard_exattack_type19_update(result: dict) -> dict:
    noncrediting(result, name='ExAttack Type19 update', target_size=821,
                 fields=35, fields_key='candidate_fields')
    if (result.get('target_sha256') != TYPE19_UPDATE_TARGET_SHA256 or
            result.get('candidate_bytes') != 821 or
            result.get('target_fields') != 35 or
            result.get('target_instructions') != 254 or
            result.get('candidate_instructions') != 254 or
            result.get('literal_definitions_checked') != 5 or
            result.get('literal_definitions_original_equal') is not True or
            result.get('target_calls') != 11 or
            result.get('candidate_calls') != 11 or
            result.get('target_direct_blocks') != 30 or
            result.get('candidate_direct_blocks') != 30 or
            result.get('distinct_player_slot_fields') != 4 or
            result.get('ordered_fields_agree') is not True or
            result.get('missing_target_bytes') != 0 or
            result.get('extra_candidate_bytes') != 0 or
            result.get('complete_direct_graphs_agree') is not True):
        raise RuntimeError('Type19 update original/35-field/11-call/30-block owner drifted')
    if (result.get('complete_bytes_agree') is not True or
            result.get('overlapping_differences') != 0 or
            result.get('difference_offsets') != []):
        raise RuntimeError('canonical 821-byte Type19 update regressed')
    return physical_difference_windows(
        result, count_key='overlapping_differences', windows=((0, 821, 0),),
        equality_key='complete_bytes_agree')


def run_exattack_type19_update(*, protect_exact: bool,
                              shared_ecl_prechecked: bool = False) -> tuple[dict, list[dict]]:
    # Ordinary FromAngleMagnitude visibility preserves ECX between the real
    # calls. Reuse its one definition in the actual ECL carrier; a standalone
    # callback loses that compiler observation. Protect every affected profile.
    obj = 'build/matching/EclManager.obj'
    if not shared_ecl_prechecked:
        build('ecl-pop-context')
    result = parsed(['scripts/inspect-exattack-type19.py', obj, '--json'],
                    diagnostic=True)
    guard_exattack_type19_update(result)
    siblings = []
    if protect_exact:
        if not shared_ecl_prechecked:
            siblings.extend(cold_replay_exact_source(
                'src/EclManager.cpp', ECL_EXACT_CARRIERS,
                already_built=(obj,)))
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
# Exact 15 candidate-vs-original physical-byte positions of the 780-byte
# TH09 Type3 boss; the other 765 bytes are already identical.
TYPE3_UPDATE_ORIGINAL_RESIDUAL_OFFSETS = frozenset((
    0x0196, 0x0197, 0x0198, 0x0199, 0x019C, 0x019D, 0x019E, 0x019F,
    0x01A0, 0x01A2, 0x01A3, 0x01A4, 0x01A5, 0x01A6, 0x01A7,
))
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
    checked = physical_difference_windows(
        result, count_key='ordinary_differences', windows=((0x196, 0x1A8, 15),),
        equality_key='complete_bytes_equal')
    actual = {int(x, 16) for x in result['difference_offsets']}
    if not actual.issubset(TYPE3_UPDATE_ORIGINAL_RESIDUAL_OFFSETS):
        raise RuntimeError('ExAttack Type3 changed an already matched original byte')
    return checked



# Type3 uses the genuine GameManager and AnmVm boundaries. Keep the whole
# four-method coordinate carrier, exact Type4 caller and interrupt leaf exact.
EXATTACK_TYPE3_NATIVE_CARRIERS = (
    ('src/GameManagerCoordinates.cpp', 'build/matching/GameManagerCoordinates.obj', (
        ('game-manager-transform-popup-x', 0x00401680,
         '?TransformPopupX@AsciiGameManagerView@@QAEMM@Z', 46),
        ('game-manager-transform-popup-y', 0x004016B0,
         '?TransformPopupY@AsciiGameManagerView@@QAEMM@Z', 46),
        ('game-manager-inverse-transform-popup-x', 0x00442420,
         '?InverseTransformPopupX@AsciiGameManagerView@@QAEMM@Z', 46),
        ('game-manager-inverse-transform-popup-y', 0x00442450,
         '?InverseTransformPopupY@AsciiGameManagerView@@QAEMM@Z', 46))),
    ('src/ExAttackInitType4.cpp', 'build/matching/ExAttackInitType4.obj', (
        ('exattack-type4-initialize', 0x00442A60,
         '?ExAttackInitializeCallbackType4@@YIHPAUExAttackRecord@@@Z', 368),)),
    ('src/AnmManagerRuntime.cpp', 'build/matching/AnmManagerRuntime.obj', (
        ('anm-vm-set-interrupt', 0x00406790,
         '?SetInterrupt@AnmVm@@QAEXF@Z', 18),)),
)
EXATTACK_TYPE3_GAME_MANAGER_VC71_PROFILE = (
    '/MT', '/EHsc', '/Gs', '/DNDEBUG', '/Zi', '/Gy', '/GF',
    '/Oi', '/Gr', '/O2', '/Ob1', '/Oy-', '/I', 'src')


def replay_exattack_type3_native_exact() -> list[dict]:
    """Rebuild three authentic source carriers / six already exact owners.

    The fake Type3/Type4 g_GameManager member declarations were two distinct
    native linker names for one real TH09 global @0x4A7D90. The real callee
    sources independently establish the inverse-coordinate function addresses.
    All five VM calls use the real AnmVm::SetInterrupt with its separately
    exact 18-byte body. This does not confer exactness on the Type3 update.
    """
    with (ROOT/'config/match-units.toml').open('rb') as stream:
        units = tomllib.load(stream)['units']
    results = []
    for source, obj, entries in EXATTACK_TYPE3_NATIVE_CARRIERS:
        for name, va, symbol, size in entries:
            unit = units.get(name)
            if (not isinstance(unit, dict) or unit.get('source') != source or
                    unit.get('object') != obj or unit.get('target_address') != va or
                    unit.get('symbol') != symbol or unit.get('size') != size or
                    unit.get('profile') != list(EXATTACK_TYPE3_GAME_MANAGER_VC71_PROFILE)):
                raise RuntimeError('ExAttack Type3 native exact source/profile/callee drift: '+name)
        build(entries[0][0])
        results.extend(canonical(name) for name, _, _, _ in entries)
    return results


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
        exact.extend(replay_exattack_type3_native_exact())
    return {'exattack_type3_update': {
        'target_bytes': 780, 'candidate_bytes': result['candidate_bytes'],
        'ordinary_differences': validated['differences'],
        'original_z_order': result['original_z_order']['classification'],
        'candidate_z_order': result['candidate_z_order']['classification'],
        'direct_graphs_agree': result['direct_graphs_agree'],
    }}, exact


EXATTACK_1422_DRAW_SHA256 = 'af3c06ea2c9f68861067f5feb784206035dd67ea3eb07ef3994ed4ec17df74f4'
# Full original-SHA/53-field comparison of the959-byte native ECL caller.
# The reviewed shared-layer replacement restores402 old mismatches and changes
# 22 former byte coincidences. Pin525 residuals and protect434 equal bytes;
# subsequent unreviewed same-count exchanges still fail closed.
EXATTACK_1422_DRAW_ORIGINAL_RESIDUAL_MASK = bytes.fromhex(
    '2000ffffffffffffffeffdffffffffffffffffff5fcdffffff7fffffffffcfff'
    'fffffffffffffffffdffffffffffffffbffffdfedfaffffdfedfafffbfffffff'
    'ff3ffdff24010400044800000002000000002400024020000040000000480204'
    '100049000000000000000000000000000000000000000000'
)

EXATTACK_1422_DRAW_EXACT_FAMILY = (
    'exattack-type14-init', 'exattack-type22-init', 'exattack-type14-22-update',
)


def guard_exattack_type1422_draw(result: dict) -> dict:
    """Noncrediting original/COFF coverage of the complete 959-byte draw boss.

    Decode and bind the entire callback, all 53 address fields and the exact
    20 direct/12 indirect call sequence and all twelve SDK argument/device
    traces in the independent inspector. A near match here cannot credit a
    function or bypass the three exact siblings.
    """
    noncrediting(result, name='ExAttack Type14/22 Draw', target_size=959,
                 fields=53, fields_key='candidate_fields')
    import importlib.util
    spec = importlib.util.spec_from_file_location(
        'exattack1422_com_guard', ROOT/'scripts/inspect-exattack-type14-22-draw.py')
    inspector = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(inspector)
    try:
        inspector.require_com_calls(result.get('target_COM_calls'))
        inspector.require_com_calls(result.get('candidate_COM_calls'))
    except ValueError as error:
        raise RuntimeError('ExAttack Type14/22 original COM argument evidence changed') from error
    size = result.get('candidate_bytes')
    count = result.get('overlapping_differences')
    candidate_jumps = result.get('candidate_direct_jumps')
    first = result.get('first_differences')
    if (result.get('target_sha256') != EXATTACK_1422_DRAW_SHA256 or
            type(size) is not int or size != 959 or
            result.get('target_instructions') != 290 or
            type(result.get('candidate_instructions')) is not int or
            not 290 <= result['candidate_instructions'] <= 291 or
            result.get('target_fields') != 53 or
            result.get('literal_definitions_checked') != 4 or
            result.get('literal_definitions_original_equal') is not True or
            result.get('native_polar_bytes') != 32 or
            result.get('native_polar_original_equal') is not True or
            result.get('native_polar_same_object_calls') != 2 or
            result.get('reachable_successors_and_calls_equal') is not True or
            result.get('target_reachable_blocks') != 17 or
            result.get('candidate_reachable_blocks') != 17 or
            result.get('target_unreachable_padding_bytes') != 0 or
            result.get('candidate_unreachable_padding_bytes') not in (0, 8) or
            result.get('target_layout_jumps') != 0 or
            result.get('candidate_layout_jumps') not in (0, 1) or
            result.get('candidate_unreachable_padding_bytes') != 8 * result.get('candidate_layout_jumps', -1) or
            result.get('direct_calls') != 20 or
            result.get('indirect_calls') != 12 or
            result.get('field_order_and_destinations_equal') is not True or
            result.get('ordered_direct_calls_equal') is not True or
            result.get('target_direct_jumps') != 9 or
            type(candidate_jumps) is not int or candidate_jumps != 9 + result.get('candidate_layout_jumps', -1) or
            type(count) is not int or not 0 <= count <= 525 or
            len(EXATTACK_1422_DRAW_ORIGINAL_RESIDUAL_MASK) != 120 or
            sum(x.bit_count() for x in EXATTACK_1422_DRAW_ORIGINAL_RESIDUAL_MASK) != 525 or
            result.get('absent_target_bytes') != 0 or
            result.get('extra_candidate_bytes') != size - 959 or
            result.get('complete_bytes_equal') is not (size == 959 and count == 0) or
            not isinstance(first, list) or len(first) != min(count, 24) or
            any(not isinstance(x, str) or not re.fullmatch(r'0x[0-9a-f]+', x)
                for x in first) or
            [int(x, 16) for x in first] !=
                sorted(set(int(x, 16) for x in first)) or
            any(int(x, 16) >= 959 for x in first)):
        raise RuntimeError('ExAttack Type14/22 959-byte SHA/53-field/32-call/9-jump original owner regressed')
    # The public producer must supply every full original-linked physical
    # mismatch, not only the first 24 diagnostic offsets. This also detects
    # a candidate that silently breaks an already matched code/data operand.
    actual = result.get('difference_offsets')
    if (not isinstance(actual, list) or len(actual) != count or
            any(type(i) is not int or not 0 <= i < 959 for i in actual) or
            actual != sorted(set(actual)) or
            [int(x, 16) for x in first] != actual[:24] or
            any(not (EXATTACK_1422_DRAW_ORIGINAL_RESIDUAL_MASK[i // 8] &
                     (1 << (i % 8))) for i in actual)):
        raise RuntimeError('ExAttack Type14/22 changed an already matching original physical byte')
    if count == 0 and (result.get('resolved_sha256') != EXATTACK_1422_DRAW_SHA256 or
                       result.get('field_positions_equal') is not True or
                       result.get('address_graphs_agree') is not True or
                       result.get('candidate_instructions') != 290 or
                       result.get('candidate_layout_jumps') != 0 or
                       result.get('candidate_unreachable_padding_bytes') != 0 or
                       candidate_jumps != 9):
        raise RuntimeError('ExAttack Type14/22 incomplete full-equality evidence')
    return {'target_bytes': 959, 'candidate_bytes': size,
            'target_instructions': 290, 'candidate_instructions': result['candidate_instructions'],
            'bound_fields': 53, 'ordered_direct_calls': 20,
            'ordered_indirect_calls': 12, 'verified_COM_argument_sets': 12,
            'direct_jumps': candidate_jumps,
            'overlap_differences': count, 'extra_candidate_bytes': size - 959}


def run_exattack_type1422_draw(*, protect_exact: bool,
                              already_built: tuple[str, ...] = ()) -> tuple[dict, list[dict]]:
    # The real polar body is required in this source context; it stays singly
    # defined in the ECL carrier rather than copied into a second game TU.
    obj = 'build/matching/EclManager.obj'
    if any(item not in ECL_EXACT_CARRIERS for item in already_built):
        raise RuntimeError('unreviewed ExAttack draw prebuilt ECL carrier')
    if obj not in already_built:
        build('ecl-pop-context')
    result = parsed(['scripts/inspect-exattack-type14-22-draw.py', obj, '--json'],
                    diagnostic=True)
    draw = guard_exattack_type1422_draw(result)
    exact = []
    if protect_exact:
        exact.extend(cold_replay_exact_source(
            'src/EclManager.cpp', ECL_EXACT_CARRIERS,
            already_built=tuple(dict.fromkeys((obj, *already_built)))))
        for unit in EXATTACK_1422_DRAW_EXACT_FAMILY:
            build(unit)
            exact.append(canonical(unit))
    return {'exattack_type14_22_draw': draw}, exact


EXATTACK_CTOR_CODEGEN_EVIDENCE = {
    'exattack_record_constructor': (15, 1,
        'e50af3e2df667c5e2d7a761e3fecfe20b3cd7805fedbb0aeef8f2e68375ea9c0',
        [0x00403390], 7),
    'exattack_controller_constructor': (28, 2,
        '39e362e7d5d17bbcc30c972966ca0114607fca2df3c3be4e4aa7d4e18685d81d',
        [0x004157A0, 0x00401470], 11),
}


def guard_exattack_constructor_codegen(result: dict) -> dict:
    """Guard codegen equality without pretending source-origin is proven.

    Natural empty out-of-line and implicit special-member C++ both produce
    the reviewed TH09 retail code. The two functions have REAL physical
    ownership, but the target's byte stream cannot settle authorship.
    """
    if not isinstance(result, dict) or set(result) != set(EXATTACK_CTOR_CODEGEN_EVIDENCE):
        raise RuntimeError('ExAttack constructor owner evidence missing')
    for name, (size, fields, digest, targets, instructions) in EXATTACK_CTOR_CODEGEN_EVIDENCE.items():
        d = result[name]
        if (not isinstance(d, dict) or d.get('diagnostic_only') is not True or
                d.get('exactness_credit') != 'none' or
                d.get('origin_classification') != 'unknown/review' or
                d.get('original_full_extent_bytes') != size or
                d.get('candidate_full_extent_bytes') != size or
                d.get('original_sha256') != digest or
                d.get('bound_original_fields') != fields or
                d.get('relocation_target_addresses') != targets or
                d.get('original_instructions') != instructions or
                d.get('original_physical_bytes_equal') is not True):
            raise RuntimeError('ExAttack constructor full codegen/origin evidence regressed: '+name)
    return result


# ExAttack Spawn's original COFF global @0x004DC698 and thiscall target
# @0x0043E2F0 have true SoundPlayer symbols and one real source body. The
# earlier private ExAttackSoundPlayerView was a source-only linker alias.
EXATTACK_REAL_SOUND_GLOBAL = '?g_SoundPlayer@@3VSoundPlayer@@A'
EXATTACK_REAL_SOUND_CALLEE = '?PlaySoundByIdx@SoundPlayer@@QAEXHH@Z'
EXATTACK_SOUNDPLAYER_CARRIER = 'build/matching/SoundPlayerPlaySound.obj'


def validate_exattack_controller_real_sound(units: dict) -> None:
    """Check two real original-linked sound fields and actual compiler owner."""
    spawn = units.get('exattack-controller-spawn')
    sound = units.get('soundplayer-play-sound-by-idx')
    if (not isinstance(spawn, dict) or
            spawn.get('source') != 'src/ExAttackController.cpp' or
            spawn.get('object') != 'build/matching/ExAttackController.obj' or
            spawn.get('symbol') != '?Spawn@ExAttackController@@QAEPAUExAttackRecord@@HPBUFloat3@@HH@Z' or
            spawn.get('size') != 321 or
            spawn.get('target_address') != 0x004151F0 or
            spawn.get('profile') != list(_SCREEN_EFFECT_BASE_PROFILE)):
        raise RuntimeError('ExAttack Spawn authentic VC7.1 source/ABI/profile drifted')
    fields = spawn.get('relocations')
    if not isinstance(fields, list) or len(fields) != 7 or any(
            not isinstance(row, dict) for row in fields):
        raise RuntimeError('ExAttack Spawn COFF relocation inventory incomplete')
    expected = ((0x8E, 'DIR32', EXATTACK_REAL_SOUND_GLOBAL, 0x004DC698),
                (0x93, 'REL32', EXATTACK_REAL_SOUND_CALLEE, 0x0043E2F0))
    for row, (offset, kind, symbol, destination) in zip(fields, expected):
        if (row.get('offset'), row.get('type'), row.get('symbol'),
                row.get('target'), row.get('addend', 0)) != (
                    offset, kind, symbol, destination, 0):
            raise RuntimeError('ExAttack Spawn uses a false SoundPlayer global/call')
    if (not isinstance(sound, dict) or
            sound.get('source') != 'src/SoundPlayerProcessQueues.cpp' or
            sound.get('object') != EXATTACK_SOUNDPLAYER_CARRIER or
            sound.get('symbol') != EXATTACK_REAL_SOUND_CALLEE or
            sound.get('target_address') != 0x0043E2F0 or
            sound.get('size') != 140 or
            sound.get('profile') != list(_SCREEN_EFFECT_BASE_PROFILE)):
        raise RuntimeError('ExAttack real SoundPlayer callee is not canonical')
    source = (ROOT / 'src/ExAttackController.cpp').read_text()
    header = (ROOT / 'src/SoundPlayer.hpp').read_text()
    if ('#include "SoundPlayer.hpp"' not in source or
            'ExAttackSoundPlayerView' in source or
            'extern SoundPlayer g_SoundPlayer;' not in header or
            'void PlaySoundByIdx(int index, int pan);' not in header or
            'g_SoundPlayer.PlaySoundByIdx(45,' not in source):
        raise RuntimeError('ExAttack source reintroduced fake SoundPlayer type')


def replay_exattack_real_sound_exact(*, prechecked: bool) -> list[dict]:
    """Cold-check the original sound callee only when not already done."""
    with (ROOT / 'config/match-units.toml').open('rb') as stream:
        units = tomllib.load(stream)['units']
    validate_exattack_controller_real_sound(units)
    if prechecked:
        return []
    build('soundplayer-play-sound-by-idx')
    return [canonical('soundplayer-play-sound-by-idx')]


def run_exattack_controller(*, protect_exact: bool,
                            sound_prechecked: bool = False) -> tuple[dict, list[dict]]:
    """Cold-run real sound callee, both ctor profiles, and four siblings."""
    # This original-profile carrier also defines nonexact 321-byte Spawn and
    # two reviewed but origin-UNKNOWN constructors. Never alter the C++ source
    # just to force an implicit-vs-explicit authorship story.
    build('exattack-release-records')
    exact = (cold_replay_exact_source(
        'src/ExAttackController.cpp', EXATTACK_CONTROLLER_EXACT_CARRIERS,
        already_built=('build/matching/ExAttackController.obj',))
        if protect_exact else [])

    with (ROOT / 'config/match-units.toml').open('rb') as stream:
        manifest = tomllib.load(stream)['units']
    unit = manifest['exattack-release-records']
    validate_exattack_controller_real_sound(manifest)
    if protect_exact:
        exact.extend(replay_exattack_real_sound_exact(prechecked=sound_prechecked))
    if (unit.get('source') != 'src/ExAttackController.cpp' or
            unit.get('object') != 'build/matching/ExAttackController.obj' or
            unit.get('profile') != list(_SCREEN_EFFECT_BASE_PROFILE)):
        raise RuntimeError('ExAttack real /Ob1 compiler profile/physical carrier drifted')
    size_flags = [('/Os' if flag == '/Ob1' else flag)
                  for flag in _SCREEN_EFFECT_BASE_PROFILE]
    if (size_flags.count('/Os') != 1 or '/Ob1' in size_flags):
        raise RuntimeError('ExAttack original size-optimized profile missing')
    output = ROOT / 'build/matching/ExAttackControllerCtorOs.obj'
    output.resolve().relative_to((ROOT / 'build').resolve())
    if output.is_symlink():
        raise RuntimeError('ExAttack alternate ctor object path is symlinked')
    if output.exists():
        output.unlink()
    proc = subprocess.run([
        str(ROOT / 'scripts/compile-probe.sh'),
        'src/ExAttackController.cpp', str(output.relative_to(ROOT)), *size_flags],
        cwd=ROOT, capture_output=True, text=True)
    if proc.returncode or not output.is_file():
        raise RuntimeError('ExAttack original /Os constructor cold compile failed: '
                           +(proc.stderr + proc.stdout)[-1000:])
    reviewed = parsed([
        'scripts/inspect-exattack-constructors.py', '--normal',
        'build/matching/ExAttackController.obj', '--size-optimized',
        'build/matching/ExAttackControllerCtorOs.obj', '--json'])
    ctor = guard_exattack_constructor_codegen(reviewed)
    return {name: {'target_bytes': item['original_full_extent_bytes'],
                   'candidate_bytes': item['candidate_full_extent_bytes'],
                   'relocations': item['bound_original_fields'],
                   'codegen_identical': True, 'origin': 'unknown/review',
                   'exactness_credit': 'none'}
            for name, item in ctor.items()}, exact


TITLE_DRAW_RESULT_SHA256 = '92279dc6d2b9ad5051fb902e6172d88eb7f9febe460d44459f16af0f45c74b0a'
TITLE_DRAW_RESULT_RESIDUAL_WINDOWS = ((0x0E, 0x25, 21), (0x39, 0x3A, 1))
# Complete SHA-pinned 1,939-byte DrawResult PE/COFF physical residuals.
# +0x13 and +0x18 were ALREADY equal inside the first residual window.
# The other 1,917 bytes (including both positions) cannot regress.
TITLE_DRAW_RESULT_ORIGINAL_RESIDUAL_OFFSETS = frozenset((
    0x0E, 0x0F, 0x10, 0x11, 0x12, 0x14, 0x15, 0x16, 0x17, 0x19, 0x1A,
    0x1B, 0x1C, 0x1D, 0x1E, 0x1F, 0x20, 0x21, 0x22, 0x23, 0x24, 0x39,
))


def guard_title_draw_result(result: dict) -> None:
    """Fail closed on regression of the 1,939-byte original renderer boss.

    This is a non-crediting *whole-owner* guard, not a local instruction
    similarity threshold or a promotion to canonical exact.  The producer
    independently binds every original source field and all 16 call identities.
    """
    noncrediting(result, name='Title DrawResult', target_size=1939, fields=82)
    if result.get('game_manager_fields') != [{
            'offset': 0x3F, 'type': 'DIR32',
            'symbol': '?g_GameManager@@3UAsciiGameManagerView@@A',
            'target': 0x004A7D90, 'addend': 0x11C,
            'effective_target': 0x004A7EAC}]:
        raise RuntimeError('Title DrawResult shared GameManager field identity drifted')
    if (result.get('target_sha256') != TITLE_DRAW_RESULT_SHA256 or
            result.get('candidate_bytes') != 1939 or
            result.get('target_instructions') != 577 or
            result.get('candidate_instructions') != 577 or
            result.get('original_fields') != 82 or
            result.get('literal_definitions_checked') != 15 or
            result.get('literal_definitions_original_equal') is not True or
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
    observed = {int(x, 16) for x in result['difference_offsets']}
    if not observed.issubset(TITLE_DRAW_RESULT_ORIGINAL_RESIDUAL_OFFSETS):
        raise RuntimeError('Title DrawResult damaged an already matched original byte')


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


TITLE_OPTIONS_TARGET_SHA256 = (
    '869fec21781b663202298f8486ebf8d0d9e42c0c1d34f1d3b08bf860b247eb63'
)
TITLE_OPTIONS_RESIDUAL_RUNS = (
    (0x015, 0x016), (0x2D6, 0x2D7), (0x540, 0x54D), (0x551, 0x552),
    (0x558, 0x559), (0x564, 0x565), (0x595, 0x596), (0x599, 0x5AE),
    (0x5AF, 0x5BE), (0x5BF, 0x5CF), (0x5D0, 0x5DA), (0x5DB, 0x5ED),
    (0x5EE, 0x5FF), (0x600, 0x608), (0x609, 0x60B), (0x60C, 0x611),
    (0x612, 0x618), (0x619, 0x63A), (0x63B, 0x655), (0x656, 0x65F),
    (0x660, 0x6AE), (0x6B1, 0x740), (0x741, 0x75A), (0x75B, 0x761),
    (0x762, 0x763), (0x764, 0x7F0), (0x7F1, 0x7FD),
)
TITLE_OPTIONS_RESIDUAL_OFFSETS = frozenset(
    offset for start, end in TITLE_OPTIONS_RESIDUAL_RUNS for offset in range(start, end))


def guard_title_options(result: dict) -> None:
    """Guard all 2,045 original bytes, 135 operands and 51 ordered calls.

    The current 2,048-byte candidate has 610 overlapping differences. Preserve
    all 1,435 equal original positions while allowing that residual set to
    shrink. A complete exact diagnostic still receives no exact credit.
    """
    noncrediting(result, name='Title OnUpdateOptions', target_size=2045, fields=135)
    if (type(result.get('literal_definitions_checked')) is not int or
            result['literal_definitions_checked'] != 1):
        raise RuntimeError('Title OnUpdateOptions lost its actual read-only SetVol definition')
    source_size = result.get('candidate_bytes')
    mismatch = result.get('overlap_differences')
    aligned = result.get('aligned_instructions')
    unpaired = result.get('unpaired_branch_destinations')
    offsets = result.get('difference_offsets')
    if (result.get('target_sha256') != TITLE_OPTIONS_TARGET_SHA256 or
            not isinstance(offsets, list) or
            any(type(offset) is not int or offset not in TITLE_OPTIONS_RESIDUAL_OFFSETS
                for offset in offsets) or
            offsets != sorted(set(offsets)) or mismatch != len(offsets)):
        raise RuntimeError('Title OnUpdateOptions changed an already equal original byte')
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


TITLE_REPLAY_SAVE_RESIDUAL_OFFSETS = frozenset((
    0x00D, 0x018, 0x01F, 0x02C, 0x031, 0x032, 0x033, 0x034, 0x035, 0x036,
    0x037, 0x038, 0x074, 0x07C, 0x086, 0x08F, 0x09E, 0x0D3, 0x0D6, 0x0D9,
    0x0E8, 0x0F4, 0x101, 0x104, 0x10F, 0x117, 0x128, 0x130, 0x149, 0x151,
    0x154, 0x160, 0x165, 0x17D, 0x196, 0x19E, 0x1A6, 0x1AA, 0x1AE, 0x1C9,
    0x1CC, 0x1D4, 0x1D5, 0x1D6, 0x1D7, 0x1D8, 0x1D9, 0x1DA, 0x1DB, 0x1DC,
    0x1DD, 0x1DE, 0x1DF, 0x1E0, 0x1E1, 0x1E4, 0x1F4, 0x1FB, 0x223, 0x226,
    0x239, 0x23C, 0x263, 0x280, 0x281, 0x282, 0x283, 0x284, 0x285, 0x286,
    0x287, 0x288, 0x289, 0x28A, 0x28B, 0x28C, 0x28D, 0x28E, 0x28F, 0x290,
    0x294, 0x295, 0x296, 0x297, 0x298, 0x299, 0x29E, 0x2A1, 0x2A7, 0x2AA,
    0x2AD, 0x2B8, 0x2C3, 0x2C7, 0x2E3, 0x2F3,
))


def guard_title_replay_save(result: dict) -> None:
    """Protect all 800 linked bytes outside the reviewed 96 residuals.

    The complete target/COFF comparison remains diagnostic. A separate complete
    canonical source build and comparison is required for exactness credit.
    """
    noncrediting(result, name='Title DrawReplaySave 800-byte owner',
                 target_size=800, fields=41)
    if (result.get('target_sha256') != TITLE_REPLAY_SAVE_TARGET_SHA256 or
            type(result.get('candidate_bytes')) is not int or
            result['candidate_bytes'] != 800 or
            result.get('target_instructions') != 245 or
            result.get('candidate_instructions') != 245 or
            result.get('original_fields') != 41 or
            result.get('literal_definitions_checked') != 18 or
            result.get('literal_definitions_original_equal') is not True or
            result.get('ordered_calls') != 4 or
            result.get('ordered_calls_agree') is not True or
            result.get('original_blocks') != 28 or
            result.get('candidate_blocks') != 28 or
            result.get('direct_graphs_agree') is not True or
            result.get('paired_branch_conflicts') != [] or
            type(result.get('unpaired_branch_destinations')) is not int or
            not 0 <= result['unpaired_branch_destinations'] <= 5 or
            type(result.get('aligned_instructions')) is not int or
            not 184 <= result['aligned_instructions'] <= 245 or
            type(result.get('complete_differences')) is not int or
            not 0 <= result['complete_differences'] <= 96):
        raise RuntimeError('Title DrawReplaySave 800/245/41/4/28 frontier drifted')
    physical_difference_windows(
        result, count_key='complete_differences', windows=((0, 800, 96),),
        equality_key='complete_bytes_equal')
    observed = {int(x, 16) for x in result['difference_offsets']}
    if not observed.issubset(TITLE_REPLAY_SAVE_RESIDUAL_OFFSETS):
        raise RuntimeError('Title DrawReplaySave damaged an already matched original byte')


TITLE_REPLAY_MENU_TARGET_SHA256 = (
    'd0201b3aae3727f7aa773187ac9ac6d21727c86342b657c780cb9353cdc39ed0'
)
# Complete source-cold original comparison: protect all 1,251 equal bytes,
# including every previously equal position. These are actual unlike bytes,
# not a broad interval in which unrelated regressions may hide.
TITLE_REPLAY_MENU_RESIDUAL_OFFSETS = frozenset((
    0x185, 0x18B, 0x193, 0x19D, 0x19E, 0x1A0, 0x1A1,
    0x1B6, 0x1BC, 0x1D3, 0x24A, 0x25A, 0x25C, 0x271,
    0x272, 0x29D, 0x2A3, 0x2C6, 0x2D7, 0x2F0, 0x2F5,
    0x2FD, 0x304, 0x308, 0x316, 0x319, 0x31C, 0x31E, 0x321,
))
TITLE_REPLAY_MENU_MATCHED_OFFSETS = (
    frozenset(range(1280)) - TITLE_REPLAY_MENU_RESIDUAL_OFFSETS
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
            type(size) is not int or size != 1280 or
            result.get('target_instructions') != 376 or
            type(result.get('candidate_instructions')) is not int or
            result['candidate_instructions'] != 376 or
            result.get('original_fields') != 57 or
            result.get('original_calls') != 13 or
            result.get('candidate_calls') != 13 or
            result.get('ordered_calls_agree') is not True or
            result.get('original_blocks') != 72 or
            result.get('direct_graphs_agree') is not True or
            type(candidate_blocks) is not int or
            candidate_blocks != 72 or
            type(aligned) is not int or not 350 <= aligned <= 376 or
            type(count) is not int or not 0 <= count <= 29 or
            not isinstance(diffs, list) or len(diffs) != count or
            any(type(i) is not int or not 0 <= i < size for i in diffs) or
            diffs != sorted(set(diffs)) or
            result.get('missing_target_bytes') != 1280 - size or
            result.get('extra_candidate_bytes') != 0 or
            result.get('complete_bytes_equal') is not
                    (size == 1280 and count == 0)):
        raise RuntimeError(
            'Title DrawReplayMenu 1280-byte/57-field/13-call/72-block frontier drifted')
    if not TITLE_REPLAY_MENU_MATCHED_OFFSETS.isdisjoint(diffs):
        raise RuntimeError('Title DrawReplayMenu damaged an already matched original byte')


def run_title_replay_menu() -> dict:
    """Cold-check the separate menu source without rebuilding unrelated TUs."""
    obj = 'build/matching/TitleScreenReplayMenuDraw.obj'
    build_nonexact('src/TitleScreenReplayMenuDraw.cpp', obj,
                   reference_unit='title-screen-setup-thread')
    menu = parsed(['scripts/inspect-title-replay-menu-draw.py', obj, '--json'])
    guard_title_replay_menu(menu)
    return {'title_draw_replay_menu': {
        'target_bytes': 1280, 'candidate_bytes': menu['candidate_bytes'],
        'bound_fields': menu['bound_fields'],
        'ordered_calls': menu['candidate_calls'],
        'original_blocks': menu['original_blocks'],
        'candidate_blocks': menu['candidate_blocks'],
        'direct_graphs_agree': menu['direct_graphs_agree'],
        'aligned_instructions': menu['aligned_instructions'],
        'overlap_differences': menu['overlap_differences'],
        'missing_target_bytes': menu['missing_target_bytes']}}


# These two 175-byte non-exact Title leaves are not arbitrary near-matches:
# fresh VC7.1/retail-bound comparison proves that precisely 38 bytes differ
# in BOTH functions. All other 137 physical bytes (including the two original
# call fields, final selected-character branch and ret 16) already agree.
# The independently SHA-pinned original target and actual COFF field/call
# inventory are also enforced by inspect-title-character-cursors.py.
# Preserve every previously equal byte while allowing genuine narrowing.
TITLE_CURSOR_REVERSE_INACTIVE_TARGET_SHA256 = {
    'reverse': '4039ba94e3b451d55013c80bd3e2099b66a8d13f71969c0d8d2166862dedc68b',
    'inactive': '5bc94b6728de835e7d4e15fa8d28fb3f98089a92499051e1db9233cca4e9a13f',
}
TITLE_CURSOR_REVERSE_INACTIVE_ORIGINAL_RESIDUAL_OFFSETS = frozenset((
    0x08, 0x09, 0x0A, 0x0B, 0x11, 0x14, 0x15, 0x16, 0x17, 0x18,
    0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F, 0x20, 0x21, 0x22,
    0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2A, 0x2B, 0x2C,
    0x2D, 0x2E, 0x36, 0x55, 0x63, 0x64, 0x65, 0x66,
))


def guard_title_cursor_reverse_inactive(variant: str, report: dict) -> None:
    """Prohibit even one *new* unequal byte in either complete 175-byte leaf.

    The 5-block retail and 4-block source graphs remain distinct. Byte counts,
    complete field identity and a matching tail do not create exactness credit.
    A later true five-block/equal-byte source remains eligible for independent
    canonical matching and strict Factory replay; this guard never grants it.
    """
    if variant not in TITLE_CURSOR_REVERSE_INACTIVE_TARGET_SHA256:
        raise RuntimeError('unreviewed Title cursor residual variant')
    noncrediting(report, name='Title cursor '+variant, target_size=175, fields=2)
    raw = report.get('difference_offsets')
    if not isinstance(raw, list) or len(raw) > len(TITLE_CURSOR_REVERSE_INACTIVE_ORIGINAL_RESIDUAL_OFFSETS):
        raise RuntimeError('Title cursor residual inventory absent/expanded')
    # These come from the independent PE-to-relocated-COFF physical byte
    # comparison. Reject malformed/canonicalized-away duplicate positions.
    offsets = []
    for item in raw:
        if not isinstance(item, str) or not item.startswith('0x'):
            raise RuntimeError('Title cursor byte offset is not an original-relative hex position')
        try:
            number = int(item, 16)
        except ValueError as error:
            raise RuntimeError('Title cursor malformed hex offset') from error
        if item != hex(number):
            raise RuntimeError('Title cursor byte offset has an ambiguous representation')
        offsets.append(number)
    if (offsets != sorted(set(offsets)) or
            not set(offsets).issubset(TITLE_CURSOR_REVERSE_INACTIVE_ORIGINAL_RESIDUAL_OFFSETS) or
            report.get('original_sha256') != TITLE_CURSOR_REVERSE_INACTIVE_TARGET_SHA256[variant] or
            report.get('variant') != variant or
            report.get('target_bytes') != 175 or
            report.get('candidate_bytes') != 175 or
            report.get('target_instructions') != 55 or
            report.get('candidate_instructions') != 55 or
            report.get('original_fields') != 2 or
            report.get('ordered_calls') != 2 or
            report.get('ordered_calls_agree') is not True or
            report.get('original_blocks') != 5 or
            type(report.get('candidate_blocks')) is not int or
            report['candidate_blocks'] not in (4, 5) or
            type(report.get('direct_graphs_agree')) is not bool or
            (report['direct_graphs_agree'] and report['candidate_blocks'] != 5) or
            (not offsets and not report['direct_graphs_agree']) or
            type(report.get('overlap_differences')) is not int or
            report['overlap_differences'] != len(offsets) or
            report.get('missing_target_bytes') != 0 or
            report.get('extra_candidate_bytes') != 0 or
            report.get('complete_bytes_equal') is not (len(offsets) == 0)):
        raise RuntimeError('Title cursor '+variant+' full-original residual/graph frontier regressed')


def guard_title_config_snapshot_owner(units: dict) -> None:
    """Keep the exact StartMenu copy bound to Supervisor's real config data.

    At owner +0x47, the original loads 0x004B3488 before REP MOVSD copies
    0x33 DWORDs. The real root is g_Supervisor at 0x004B3100; the cold COFF
    addend is config388's 0x388. A fake independent global at 0x004B3488
    would also link to the same bytes, so byte equality alone cannot protect
    the C++ data-owner identity established here.
    """
    unit = units.get('title-screen-start-menu', {})
    rows = unit.get('relocations', [])
    field = [row for row in rows if row.get('offset') == 0x47]
    if (unit.get('source') != 'src/TitleScreen.cpp' or
            unit.get('object') != 'build/matching/TitleScreen.obj' or
            unit.get('symbol') != '?OnUpdateStartMenu@TitleScreenView@@QAEHXZ' or
            unit.get('target_address') != 0x00429E23 or
            unit.get('size') != 1560 or unit.get('compare_size') != 1592 or
            len(rows) != 107 or len(field) != 1 or
            field[0].get('type') != 'DIR32' or
            field[0].get('symbol') != '?g_Supervisor@@3VSupervisor@@A' or
            field[0].get('target') != 0x004B3100 or
            any('g_TitleConfigSnapshot' in row.get('symbol', '') for row in rows)):
        raise RuntimeError('Title StartMenu original config copy lost the genuine Supervisor owner')


def run_title_result() -> dict:
    """Replay only the complete DrawResult owner using its recorded profile."""
    result_obj = 'build/matching/TitleResultDraw.obj'
    build_nonexact('src/TitleScreenResultDraw.cpp', result_obj,
                   reference_unit='title-screen-setup-thread')
    draw = parsed(['scripts/inspect-title-result-draw.py', result_obj, '--json'])
    guard_title_draw_result(draw)
    return {'title_draw_result': {
        'target_bytes': 1939, 'candidate_bytes': draw['candidate_bytes'],
        'ordinary_differences': draw['complete_differences'],
        'fields': draw['bound_fields'],
        'literal_definitions_checked': draw['literal_definitions_checked'],
        'direct_graphs_agree': draw['direct_graphs_agree']}}


def run_title_open() -> dict:
    # These source-present owners are explicitly non-exact. Their independent
    # original operand inventories must agree before any similarity score.
    # The cursor trio is in the same TU as the exact TitleSetup thread; rebuild
    # it even with --skip-exact-siblings so a stale object cannot be reused.
    with (ROOT / 'config/match-units.toml').open('rb') as stream:
        guard_title_config_snapshot_owner(tomllib.load(stream)['units'])
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
        if variant in TITLE_CURSOR_REVERSE_INACTIVE_TARGET_SHA256:
            guard_title_cursor_reverse_inactive(variant, result)
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
    options = parsed(['scripts/inspect-title-options.py', options_obj,
                      '--require-native-input', '--json'])
    guard_title_options(options)
    cursor_reports['title_update_options'] = {
        'target_bytes': 2045, 'candidate_bytes': options['candidate_bytes'],
        'literal_definitions_checked': options['literal_definitions_checked'],
        'bound_fields': options['bound_fields'],
        'ordered_calls': options['ordered_calls'],
        'target_instructions': options['target_instructions'],
        'candidate_instructions': options['candidate_instructions'],
        'missing_target_bytes': options['missing_target_bytes'],
        'extra_candidate_bytes': options['extra_candidate_bytes'],
        'direct_graphs_agree': options['direct_graphs_agree'],
    }
    menu_report = run_title_replay_menu()
    draw_report = run_title_result()
    replay_obj = 'build/matching/TitleReplaySave.obj'
    build_nonexact('src/TitleScreenReplaySaveDraw.cpp', replay_obj,
                   reference_unit='title-screen-setup-thread')
    replay = noncrediting(parsed(['scripts/inspect-title-replay-save-draw.py', replay_obj,
                                  '--json']), name='Title DrawReplaySave',
                          target_size=800, fields=41)
    guard_title_replay_save(replay)
    return {**cursor_reports, **menu_report, **draw_report,
            'title_draw_replay_save': {
                'target_bytes': 800, 'candidate_bytes': replay['candidate_bytes'],
                'fields': replay['bound_fields'],
                'first_overlapping_difference': replay['first_overlapping_difference'],
                'candidate_instructions': replay['candidate_instructions'],
                'aligned_instructions': replay['aligned_instructions'],
                'ordered_calls': replay['ordered_calls'],
                'direct_graphs_agree': replay['direct_graphs_agree']}}



# Two genuine, separately compiled SoundPlayer members originally consumed
# by Title result/cursor/menu (0x43E2F0) and exact MusicRoom (0x43EE40).
# The old SoundPlayerView class invented separate C++ linker identities for
# ONE real global @0x4DC698. Verify original physical callees as independent
# source bodies, not just their agreed CALL REL32 instructions.
TITLE_REAL_SOUND_CARRIERS = (
    ('soundplayer-play-sound-by-idx', 0x0043E2F0,
     '?PlaySoundByIdx@SoundPlayer@@QAEXHH@Z', 140,
     'build/matching/SoundPlayerPlaySound.obj'),
    ('soundplayer-queue-command', 0x0043EE40,
     '?QueueCommand@SoundPlayer@@QAEXHHPAD@Z', 105,
     'build/matching/SoundPlayerLifecycle.obj'),
)
TITLE_REAL_SOUND_VC71_PROFILE = (
    '/MT', '/EHsc', '/Gs', '/DNDEBUG', '/Zi', '/Gy', '/GF',
    '/Oi', '/Gr', '/O2', '/Ob1', '/Oy-', '/I', 'src')


def replay_title_real_sound_exact(*, play_sound_prechecked: bool) -> list[dict]:
    """Rebuild genuine Title audio leaves, avoiding duplicate all-mode credit."""
    with (ROOT/'config/match-units.toml').open('rb') as stream:
        units=tomllib.load(stream)['units']
    exact=[]
    for name,va,symbol,size,obj in TITLE_REAL_SOUND_CARRIERS:
        u=units.get(name)
        if (not isinstance(u,dict) or
                u.get('source')!='src/SoundPlayerProcessQueues.cpp' or
                u.get('object')!=obj or u.get('target_address')!=va or
                u.get('symbol')!=symbol or u.get('size')!=size or
                u.get('profile')!=list(TITLE_REAL_SOUND_VC71_PROFILE)):
            raise RuntimeError('Title true SoundPlayer exact callee identity/profile drift: '+name)
        if play_sound_prechecked and name=='soundplayer-play-sound-by-idx':
            # --owner all first verified the very same source/VA with RunEcl;
            # do not build/credit it twice in a distinct exact sibling list.
            continue
        build(name)
        exact.append(canonical(name))
    return exact


# Two complete already-exact MusicRoom functions (2258/209 bytes, 74/6
# fields) must call native AnmManager/AnmLoaded/Supervisor methods rather
# than fabricated TitleAnmManagerView/TitleAnmLoadedView/free-cdecl names.
# The pointer g_TitleMusicDescriptionAnm is physically the original
# 0x004B36CC, now typed as the native AnmLoaded class. Numeric destinations
# alone do not establish C++ linker identity; keep EVERY changed PE field
# offset, kind, call order and native decorated symbol.
TITLE_MUSIC_NATIVE_SYMBOLS = {
    'G': ('?g_AnmManager@@3PAVAnmManager@@A','DIR32',0x004DC550),
    'T': ('?DrawTextLeft@AnmManager@@QAAXPAUAnmVm@@KKPBDZZ','REL32',0x0043BDC0),
    'S': ('?g_Supervisor@@3VSupervisor@@A','DIR32',0x004B3100),
    'F': ('?FadeOutMusic@Supervisor@@QAEHM@Z','REL32',0x0042FE20),
    'P': ('?PlayAudio@Supervisor@@QAEHPADH@Z','REL32',0x00431A30),
    'D': ('?g_TitleMusicDescriptionAnm@@3PAVAnmLoaded@@A','DIR32',0x004B36CC),
    'I': ('?SetAndExecuteScriptIdx@AnmLoaded@@QAEXPAUAnmVm@@H@Z','REL32',0x00403E00),
    'L': ('?LoadSurface@AnmManager@@QAEHHPBD@Z','REL32',0x0043CAE0),
    'Z': ('?SetInterruptArray@AnmManager@@QAEXPAUAnmVm@@HF@Z','REL32',0x00436DE0),
    'A': ('?ExecuteScriptArray@AnmManager@@QAEXPAUAnmVm@@H@Z','REL32',0x00439560),
    'E': ('?ExecuteScript@AnmManager@@QAEHPAUAnmVm@@@Z','REL32',0x00436F30),
    'R': ('?DrawNoRotation@AnmManager@@QAEHPAUAnmVm@@@Z','REL32',0x0043A950),
}
TITLE_MUSIC_NATIVE_ORIGINAL_SITES = {
    'title-screen-music-room': (
        (0x216,'G'), (0x21B,'T'), (0x348,'S'), (0x350,'F'),
        (0x3D0,'S'), (0x3D5,'P'), (0x41B,'S'), (0x422,'P'),
        (0x436,'D'), (0x440,'I'), (0x48C,'G'), (0x497,'L'),
        (0x4A8,'G'), (0x4BD,'Z'), (0x4C5,'G'), (0x4D0,'A'),
        (0x696,'I'), (0x6C1,'G'), (0x6C6,'T'), (0x701,'G'),
        (0x70A,'T'), (0x865,'D'), (0x871,'I'), (0x8A8,'G'),
        (0x8AE,'E')),
    'title-screen-draw-music-room': (
        (0x07C,'G'), (0x081,'R'), (0x0B6,'G'), (0x0BC,'R')),
}
TITLE_MUSIC_NATIVE_BOUNDS = {
    'title-screen-music-room': (0x00426E05,2258,74),
    'title-screen-draw-music-room': (0x004244A9,209,6),
}
TITLE_MUSIC_REAL_INDEPENDENT_CALLEES = {
    'anm-manager-draw-text-left': (
        '?DrawTextLeft@AnmManager@@QAAXPAUAnmVm@@KKPBDZZ',0x0043BDC0,
        'src/AnmManagerUtility.cpp',139),
    'anm-manager-load-surface': (
        '?LoadSurface@AnmManager@@QAEHHPBD@Z',0x0043CAE0,
        'src/AnmManagerLoad.cpp',486),
    'anm-manager-set-interrupt-array': (
        '?SetInterruptArray@AnmManager@@QAEXPAUAnmVm@@HF@Z',0x00436DE0,
        'src/AnmManagerRuntime.cpp',72),
}


def validate_title_music_native_abi(units: dict) -> None:
    """Protect original large MusicRoom source and 29 actual native call fields."""
    source=(ROOT/'src/TitleScreenMusicRoom.cpp').read_text()
    inspector=(ROOT/'scripts/inspect-title-music-room.py').read_text()
    if (source.count('#include "AnmManager.hpp"')!=1 or
            source.count('#include "Supervisor.hpp"')!=1 or
            'TitleAnmManagerView' in source or
            'TitleAnmLoadedView' in source or
            'TitleSupervisorView' in source or
            'DrawTitleMusicText' in source or
            'g_TitleAnmManager' in source or
            'g_TitleSupervisor' in source or
            'extern AnmLoaded *g_TitleMusicDescriptionAnm;' not in source or
            'AnmLoaded *musicAnm;' not in source or
            'g_AnmManager->DrawTextLeft(' not in source or
            'g_Supervisor.PlayAudio(' not in source or
            'offsetof(TitleScreenView, musicAnm) == 0x11B90' not in source or
            'DrawTitleMusicText' in inspector or
            '?DrawTextLeft@AnmManager@@QAAXPAUAnmVm@@KKPBDZZ' not in inspector):
        raise RuntimeError('MusicRoom exact game source still uses fabricated ANM/Supervisor ABI')
    profile=('/MT','/EHsc','/Gs','/DNDEBUG','/Zi','/Gy','/GF',
             '/Gr','/O1','/Ob1','/Oy-','/I','src')
    for name,(va,size,nfields) in TITLE_MUSIC_NATIVE_BOUNDS.items():
        unit=units.get(name)
        if (not isinstance(unit,dict) or
                unit.get('source')!='src/TitleScreenMusicRoom.cpp' or
                unit.get('object')!='build/matching/TitleScreenMusicRoom.obj' or
                unit.get('target_address')!=va or unit.get('size')!=size or
                unit.get('profile')!=list(profile) or
                len(unit.get('relocations',[]))!=nfields):
            raise RuntimeError('MusicRoom original function/profile/full field extent changed: '+name)
        expected=[(off,*TITLE_MUSIC_NATIVE_SYMBOLS[k],0)
                  for off,k in TITLE_MUSIC_NATIVE_ORIGINAL_SITES[name]]
        sites={off for off,k in TITLE_MUSIC_NATIVE_ORIGINAL_SITES[name]}
        actual=[(f.get('offset'),f.get('symbol'),f.get('type'),f.get('target'),f.get('addend',0))
                for f in unit['relocations'] if f.get('offset') in sites]
        if actual!=expected or any('TitleAnm' in f.get('symbol','') or
                                   'TitleSupervisorView' in f.get('symbol','') or
                                   'DrawTitleMusicText' in f.get('symbol','')
                                   for f in unit['relocations']):
            raise RuntimeError('MusicRoom original 29 method/global field offsets, kinds or C++ symbols changed: '+name)
    for name,(symbol,va,source_file,size) in TITLE_MUSIC_REAL_INDEPENDENT_CALLEES.items():
        unit=units.get(name)
        if (not isinstance(unit,dict) or
                unit.get('source')!=source_file or
                unit.get('target_address')!=va or unit.get('symbol')!=symbol or
                unit.get('size')!=size or
                unit.get('profile')!=list(_SCREEN_EFFECT_BASE_PROFILE)):
            raise RuntimeError('MusicRoom native independent original exact ANM method altered: '+name)


def replay_title_music_native_callees() -> list[dict]:
    """Cold check three independently maintained native called source owners."""
    with (ROOT/'config/match-units.toml').open('rb') as stream:
        validate_title_music_native_abi(tomllib.load(stream)['units'])
    results=[]
    for name in TITLE_MUSIC_REAL_INDEPENDENT_CALLEES:
        build(name)
        results.append(canonical(name))
    return results


def run_title(*, protect_exact: bool, play_sound_prechecked: bool = False) -> tuple[dict, list[dict]]:
    # TitleSetup plus 36 other original-byte exact owners use TitleScreen.obj
    # under the same pinned /O1 profile. ScoreRecord::InsertIntoTable uses an
    # independent /O2 carrier despite sharing TitleScreen.cpp source. Both
    # must be cold-replayed before claiming the open Title bosses stayed safe.
    with (ROOT/'config/match-units.toml').open('rb') as stream:
        validate_title_music_native_abi(tomllib.load(stream)['units'])
    exact_units: list[dict] = []
    if protect_exact:
        exact_units = cold_replay_exact_source(
            'src/TitleScreen.cpp', TITLE_EXACT_CARRIERS)
        build('title-screen-music-room')
        exact_units.extend((canonical('title-screen-music-room'),
                            canonical('title-screen-draw-music-room')))
        build('title-draw-callback')
        exact_units.append(canonical('title-draw-callback'))
        exact_units.extend(replay_title_real_sound_exact(
            play_sound_prechecked=play_sound_prechecked))
        exact_units.extend(replay_title_music_native_callees())
    # These receipts do not establish exactness for the separately diagnosed
    # DrawResult, ReplayMenu, ReplaySave, or Options owners.
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
    """Protect the accepted complete owner; supplied diagnostics grant no credit."""
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
    timestamps = [dict(offset=offset, iat=0x48E238,
                       symbol='__imp__timeGetTime@0')
                  for offset in (0x3C1, 0x6B2)]
    if (physical != 1776 or missing != 0 or diffs != 0 or
            result.get('complete_equal') is not True or
            source.get('code_bytes') != 1734 or
            source.get('alignment_hex') != '8bff' or
            source.get('instructions') != 449 or
            len(result['fields']) != 106 or
            table != list(PAUSE_TARGET_TABLE) or
            source.get('ordered_direct_calls') != calls or
            source.get('internal_branches') != target.get('internal_branches') or
            source.get('imported_calls') != timestamps or
            target.get('imported_calls') != timestamps):
        raise RuntimeError('PauseMenu accepted complete original equality regressed')
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


# The two 1KB+ menu callback bodies and the 15-byte resume method all
# receive the same genuine SoundPlayer singleton at 0x004DC698. Original
# TH09 dispatch resolves the small Pause wrapper at 0x00423498 to the real
# 105-byte SoundPlayer::QueueCommand, not to another invented C++ receiver.
PAUSE_REAL_SOUND_GLOBAL = '?g_SoundPlayer@@3VSoundPlayer@@A'
PAUSE_REAL_SOUND_PLAY = '?PlaySoundByIdx@SoundPlayer@@QAEXHH@Z'
PAUSE_REAL_SOUND_RESUME = '?ResumeAfterPause@SoundPlayer@@QAEXXZ'
PAUSE_REAL_SOUND_QUEUE = '?QueueCommand@SoundPlayer@@QAEXHHPAD@Z'
# SHA-backed retail menu original callsite offsets (global receiver and next
# member-call operand). This is independent of candidate manifest spelling.
PAUSE_REAL_SOUND_CALLSITE_PAIRS = {
    'ascii-menu-state4-update': (
        (0x121,0x12C), (0x146,0x151), (0x16E,0x173),
        (0x26A,0x275), (0x28F,0x29A), (0x2BB,0x2C0),
        (0x389,0x394), (0x3AE,0x3B9), (0x3DA,0x3DF)),
    'ascii-menu-state5-update': (
        (0x1B5,0x1C0), (0x1DD,0x1E2),
        (0x2E6,0x2F1), (0x312,0x317)),
}
PAUSE_REAL_SOUND_CARRIERS = (
    ('ascii-menu-state5-update',0x00434E90,1213,1236,
     'src/AsciiMenuState5Update.cpp', 'build/matching/AsciiMenuState5Update.obj',
     '?OnUpdate@AsciiMenuState5@@QAEHXZ'),
    ('soundplayer-resume-after-pause',0x00423498,15,15,
     'src/SoundPlayerPause.cpp', 'build/matching/SoundPlayerPause.obj',
     PAUSE_REAL_SOUND_RESUME),
    ('soundplayer-play-sound-by-idx',0x0043E2F0,140,140,
     'src/SoundPlayerProcessQueues.cpp', 'build/matching/SoundPlayerPlaySound.obj',
     PAUSE_REAL_SOUND_PLAY),
    ('soundplayer-queue-command',0x0043EE40,105,105,
     'src/SoundPlayerProcessQueues.cpp', 'build/matching/SoundPlayerLifecycle.obj',
     PAUSE_REAL_SOUND_QUEUE),
)


def validate_pause_real_sound(units: dict) -> None:
    """Original sound call targets and real C++ linker spelling are mandatory."""
    sources = ('src/AsciiManagerMenu.cpp','src/AsciiMenuState5Update.cpp',
               'src/SoundPlayerPause.cpp')
    for source in sources:
        contents = (ROOT/source).read_text()
        if ('#include "SoundPlayer.hpp"' not in contents or
                'AsciiSoundPlayerView' in contents or
                ('SoundPlayerPause.cpp' in source and
                 'void SoundPlayer::ResumeAfterPause()' not in contents)):
            raise RuntimeError('Pause source uses fabricated SoundPlayer identity: '+source)
    header = (ROOT/'src/SoundPlayer.hpp').read_text()
    if (header.count('void ResumeAfterPause();') != 1 or
            'extern SoundPlayer g_SoundPlayer;' not in header):
        raise RuntimeError('Pause real sound resume declaration changed')
    for name,address,size,physical,source,obj,symbol in PAUSE_REAL_SOUND_CARRIERS:
        row=units.get(name)
        if (not isinstance(row,dict) or
                row.get('target_address')!=address or row.get('size')!=size or
                row.get('compare_size',size)!=physical or
                row.get('source')!=source or row.get('object')!=obj or
                row.get('symbol')!=symbol or
                row.get('profile')!=list(_SCREEN_EFFECT_BASE_PROFILE)):
            raise RuntimeError('Pause genuine exact callback/callee origin drifted: '+name)
    counted=0
    for name,pairs in PAUSE_REAL_SOUND_CALLSITE_PAIRS.items():
        row=units.get(name)
        if not isinstance(row,dict) or row.get('source') not in sources[:2]:
            raise RuntimeError('Pause menu original callback source missing')
        sound=[f for f in row['relocations'] if
               'SoundPlayer' in f['symbol'] or 'PlaySound' in f['symbol']]
        expected=[(offset,symbol,kind,target,0) for pair in pairs
                  for offset,symbol,kind,target in (
                    (pair[0],PAUSE_REAL_SOUND_GLOBAL,'DIR32',0x004DC698),
                    (pair[1],PAUSE_REAL_SOUND_PLAY,'REL32',0x0043E2F0))]
        actual=[(f['offset'],f['symbol'],f['type'],f['target'],f.get('addend',0))
                for f in sound]
        if actual!=expected:
            raise RuntimeError('Pause exact menu callback original SoundPlayer field/site census changed')
        counted+=len(sound)
    if counted!=26:
        raise RuntimeError('Pause original sound field census incomplete')
    wrapper=units['soundplayer-resume-after-pause']['relocations']
    if (len(wrapper)!=2 or
            [(r.get('offset'),r.get('symbol'),r.get('type'),
              r.get('target'),r.get('addend',0)) for r in wrapper] != [
                (0x01,'??_C@_07OGJCKFMD@UnPause?$AA@','DIR32',0x0048F510,0),
                (0x0A,PAUSE_REAL_SOUND_QUEUE,'REL32',0x0043EE40,0)]):
        raise RuntimeError('Pause resume wrapper not bound to real QueueCommand')
    inspector=(ROOT/'scripts/inspect-pause-menu.py').read_text()
    if ('AsciiSoundPlayerView' in inspector or
            PAUSE_REAL_SOUND_RESUME not in inspector or
            PAUSE_REAL_SOUND_GLOBAL not in inspector or
            PAUSE_REAL_SOUND_PLAY not in inspector):
        raise RuntimeError('Pause physical-target inspector permits false C++ audio identity')


def replay_pause_real_sound_exact() -> list[dict]:
    """Cold-check independent original source carriers for two big callbacks."""
    build('ascii-menu-state5-update')
    build('soundplayer-resume-after-pause')
    build('soundplayer-play-sound-by-idx')
    build('soundplayer-queue-command')
    results = [canonical(name) for name,_,_,_,_,_,_ in PAUSE_REAL_SOUND_CARRIERS]
    data = parsed(['scripts/probe-pause-native-audio.py', '--check-resume-object',
                   'build/matching/SoundPlayerPause.obj'])
    if (data.get('diagnostic_only') is not True or data.get('exactness_credit') != 'none'
            or data.get('native_resume', {}).get('complete_equal') is not True
            or data.get('native_resume', {}).get('bytes') != 15
            or data.get('native_resume', {}).get('fields') != 2
            or data.get('resume_literal_bytes') != 8):
        raise RuntimeError('Pause resume actual UnPause definition proof absent')
    results[1]['literal_definitions_checked'] = 1
    return results


def run_pause(*, protect_exact: bool) -> tuple[dict, list[dict]]:
    with (ROOT/'config/match-units.toml').open('rb') as stream:
        validate_pause_real_sound(tomllib.load(stream)['units'])
    obj = 'build/matching/AsciiManagerMenu.obj'
    build('pause-menu-update')
    report = parsed(['scripts/inspect-pause-menu.py', '--json', obj])
    exact = []
    if protect_exact:
        # These two real methods share a separate canonical object. Their
        # opaque call boundary is part of the accepted Pause source family.
        build('anm-vm-is-visible')
        exact = [canonical('pause-menu-update')]
        exact.extend(canonical(u) for u in PAUSE_EXACT_SIBLINGS)
        exact.extend(replay_pause_real_sound_exact())
    return {'pause_menu_update': guard_pause_menu(report)}, exact


# Genuine native SoundPlayer provenance across five further long callbacks,
# not just nearby 3-byte or constructor leaves.  The source-only fake receiver
# once made 12 canonical exact units numerically linkable but declared an
# inconsistent C++ symbol for one global and its three real member methods.
AUDIO_TRUE_GLOBAL = '?g_SoundPlayer@@3VSoundPlayer@@A'
AUDIO_TRUE_PLAY = '?PlaySoundByIdx@SoundPlayer@@QAEXHH@Z'
AUDIO_TRUE_QUEUE = '?QueueCommand@SoundPlayer@@QAEXHHPAD@Z'
AUDIO_TRUE_PROCESS = '?ProcessQueues@SoundPlayer@@QAEHXZ'
AUDIO_TRUE_METHODS = {
    AUDIO_TRUE_GLOBAL: ('DIR32',0x004DC698),
    AUDIO_TRUE_PLAY: ('REL32',0x0043E2F0),
    AUDIO_TRUE_QUEUE: ('REL32',0x0043EE40),
    AUDIO_TRUE_PROCESS: ('REL32',0x0043F1F0),
}
AUDIO_TRUE_CARRIERS = {
    'src/ExAttackUpdateType14Type22.cpp': {'build/matching/ExAttackUpdateType14Type22.obj':1},
    'src/FrontCalcCallback.cpp': {'build/matching/FrontCalcCallback.obj':1},
    'src/GameWindowRender.cpp': {'build/matching/GameWindowRender.obj':1},
    'src/SupervisorAudio.cpp': {'build/matching/SupervisorAudio.obj':8},
    'src/SupervisorDeletedCallback.cpp': {'build/matching/SupervisorDeletedCallback.obj':1},
}
# Original PE-bound full function extents (not candidate-derived length hints).
AUDIO_TRUE_EXACT_OWNERS = {
    'exattack-type14-22-update': ('src/ExAttackUpdateType14Type22.cpp',0x443DF0,805,824),
    'front-calc-callback': ('src/FrontCalcCallback.cpp',0x417630,1491,1491),
    'game-window-render': ('src/GameWindowRender.cpp',0x42DC40,326,326),
    'supervisor-play-music': ('src/SupervisorAudio.cpp',0x431930,254,254),
    'supervisor-play-audio': ('src/SupervisorAudio.cpp',0x431A30,169,169),
    'supervisor-load-music': ('src/SupervisorAudio.cpp',0x42FC20,207,207),
    'supervisor-start-loaded-music': ('src/SupervisorAudio.cpp',0x42FCF0,53,53),
    'supervisor-release-music': ('src/SupervisorAudio.cpp',0x42FD90,130,130),
    'supervisor-stop-audio': ('src/SupervisorAudio.cpp',0x42FD30,81,81),
    'supervisor-fade-out-music': ('src/SupervisorAudio.cpp',0x42FE20,148,148),
    'supervisor-audio-has-flag-bit1': ('src/SupervisorAudio.cpp',0x41A9AC,12,12),
    'supervisor-deleted-callback': ('src/SupervisorDeletedCallback.cpp',0x431B70,405,405),
}
# Every native sound global/member operand offset is attested independently
# against the original PE by the complete canonical comparator.
AUDIO_TRUE_ORIGINAL_FIELDS = {
    'exattack-type14-22-update': ((0x15F,AUDIO_TRUE_GLOBAL),(0x164,AUDIO_TRUE_PLAY)),
    'front-calc-callback': ((0x3C6,AUDIO_TRUE_GLOBAL),(0x3CB,AUDIO_TRUE_PLAY)),
    'game-window-render': ((0x6F,AUDIO_TRUE_GLOBAL),(0x76,AUDIO_TRUE_PROCESS)),
    'supervisor-play-music': ((0xAC,AUDIO_TRUE_GLOBAL),(0xB1,AUDIO_TRUE_QUEUE),
                              (0xBE,AUDIO_TRUE_GLOBAL),(0xC3,AUDIO_TRUE_QUEUE)),
    'supervisor-play-audio': ((0x69,AUDIO_TRUE_GLOBAL),(0x6E,AUDIO_TRUE_QUEUE)),
    'supervisor-load-music': ((0xBA,AUDIO_TRUE_GLOBAL),(0xBF,AUDIO_TRUE_QUEUE)),
    'supervisor-start-loaded-music': ((0x29,AUDIO_TRUE_GLOBAL),(0x2E,AUDIO_TRUE_QUEUE)),
    'supervisor-release-music': ((0x6C,AUDIO_TRUE_GLOBAL),(0x71,AUDIO_TRUE_QUEUE)),
    'supervisor-stop-audio': ((0x31,AUDIO_TRUE_GLOBAL),(0x3C,AUDIO_TRUE_QUEUE),
                              (0x46,AUDIO_TRUE_QUEUE)),
    'supervisor-fade-out-music': ((0x7C,AUDIO_TRUE_GLOBAL),(0x81,AUDIO_TRUE_QUEUE)),
    'supervisor-audio-has-flag-bit1': (),
    'supervisor-deleted-callback': ((0xB1,AUDIO_TRUE_GLOBAL),(0xB6,AUDIO_TRUE_QUEUE)),
}
AUDIO_TRUE_EXACT_CALLEES = {
    'soundplayer-play-sound-by-idx': (0x43E2F0,140,140,AUDIO_TRUE_PLAY),
    'soundplayer-queue-command': (0x43EE40,105,105,AUDIO_TRUE_QUEUE),
    'soundplayer-process-queues': (0x43F1F0,1414,1456,AUDIO_TRUE_PROCESS),
}


def validate_audio_true_cpp_owners(units: dict) -> None:
    """Fail closed on fake linker names and any original callsite changes."""
    if len(AUDIO_TRUE_EXACT_OWNERS) != 12:
        raise RuntimeError('native sound callback inventory is not complete')
    observations = set()
    for source,carriers in AUDIO_TRUE_CARRIERS.items():
        groups=exact_source_carriers(source,carriers,units)
        observations.update(name for names in groups.values() for name in names)
        body=(ROOT/source).read_text()
        if ('#include "SoundPlayer.hpp"' not in body or
                'SoundPlayerView' in body or
                'g_SoundPlayer.' not in body):
            raise RuntimeError('original game/ExAttack/Front/Supervisor source has fake sound C++ owner: '+source)
    if observations != set(AUDIO_TRUE_EXACT_OWNERS):
        raise RuntimeError('native sound exact source carrier census incomplete')
    total_real_fields=0
    for name,(source,va,size,physical) in AUDIO_TRUE_EXACT_OWNERS.items():
        row=units.get(name)
        if (not isinstance(row,dict) or row.get('source')!=source or
                row.get('target_address')!=va or row.get('size')!=size or
                row.get('compare_size',size)!=physical or
                row.get('profile')!=list(_SCREEN_EFFECT_BASE_PROFILE)):
            raise RuntimeError('native source-cold sound callback function/profile/boundary changed: '+name)
        if name=='supervisor-audio-has-flag-bit1' and row.get('symbol') != (
                '?HasFlagBit1@SupervisorAudioGameManagerView@?A0xfb5c156d@@QAEHXZ'):
            raise RuntimeError('SupervisorAudio anonymous owner label drifted')
        if source=='src/SupervisorAudio.cpp':
            anon='@?A0xfb5c156d@@'
        elif source=='src/SupervisorDeletedCallback.cpp':
            anon='@?A0x4471bc32@@'
        else:anon=None
        for field in row['relocations']:
            symbol=field['symbol']
            if '@?A0x' in symbol and (anon is None or anon not in symbol):
                raise RuntimeError('anonymous namespace C++ symbol hash was not original-VA bound')
        actual=[(x['offset'],x['symbol'],x['type'],x['target'],x.get('addend',0))
                for x in row['relocations'] if
                x['symbol'] in AUDIO_TRUE_METHODS or
                'SoundPlayerView' in x['symbol'] or 'PlaySoundByIdx@' in x['symbol'] or
                'QueueCommand@' in x['symbol'] or 'ProcessQueues@' in x['symbol']]
        expected=[(offset,symbol,*AUDIO_TRUE_METHODS[symbol],0)
                  for offset,symbol in AUDIO_TRUE_ORIGINAL_FIELDS[name]]
        if actual!=expected:
            raise RuntimeError('native audio original source field/call order or VA changed: '+name)
        total_real_fields+=len(actual)
    if total_real_fields!=25:
        raise RuntimeError('original sound global/member field count incorrect')
    for name,(va,size,physical,symbol) in AUDIO_TRUE_EXACT_CALLEES.items():
        row=units.get(name)
        if (not isinstance(row,dict) or
                row.get('source')!='src/SoundPlayerProcessQueues.cpp' or
                row.get('target_address')!=va or row.get('size')!=size or
                row.get('compare_size',size)!=physical or
                row.get('symbol')!=symbol or
                row.get('profile')!=list(_SCREEN_EFFECT_BASE_PROFILE)):
            raise RuntimeError('native audio call does not target an independently exact true C++ member: '+name)
    inspector=(ROOT/'scripts/replay-front-calc-regions.py').read_text()
    if ('FrontCalcSoundPlayerView' in inspector or
            AUDIO_TRUE_GLOBAL not in inspector or
            AUDIO_TRUE_PLAY not in inspector):
        raise RuntimeError('FrontCalc physical original-region inspector uses fake sound linking')


def run_audio_true_exact(*, protect_exact: bool) -> tuple[dict, list[dict]]:
    """Cold check ALL 12 existing exact owners, plus three actual sound callees."""
    with (ROOT/'config/match-units.toml').open('rb') as stream:
        units=tomllib.load(stream)['units']
    validate_audio_true_cpp_owners(units)
    if not protect_exact:
        return {}, []
    exact=[]
    for source,carriers in AUDIO_TRUE_CARRIERS.items():
        exact.extend(cold_replay_exact_source(source,carriers))
    # The real sound methods are distinct source/target owners. Their flags
    # and physical size are tested independently by each canonical replay.
    for name in AUDIO_TRUE_EXACT_CALLEES:
        build(name)
        exact.append(canonical(name))
    return {},exact


def inspect_player_position_pair(obj: str) -> dict:
    """Original-hashed complete comparisons from the same freshly built TU."""
    import importlib.util
    spec = importlib.util.spec_from_file_location(
        'position_pair', ROOT/'scripts/probe-player-position-visibility.py')
    module = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(module)
    with (ROOT/'config/match-units.toml').open('rb') as stream:
        units = tomllib.load(stream)['units']
    coff = module.load_coff()
    owners = {}
    for owner in module.OWNERS:
        result = module.compare(coff, ROOT/obj, owner, units)
        if (result['complete_bytes_equal'] is not True or
                result['candidate_bytes'] != 249 or len(result['bindings']) != 8 or
                result['overlapping_differences'] != 0 or
                result['missing_bytes'] != 0 or result['extra_bytes'] != 0):
            raise RuntimeError('Player position Type4/8 complete original owner drifted')
        owners['player_position_type'+str(owner[0])] = {
            'physical_bytes': 249, 'fields': 8, 'different_bytes': 0}
    return owners


def run_player_type11(*, protect_exact: bool,
                      include_detached_math: bool = True) -> tuple[dict, list[dict]]:
    """Replay the genuine shared source once per profile, including its bosses."""
    owners, _ = run_ecl(include_trail=False, include_interpreter=True,
                        protect_exact=False)
    obj = 'build/matching/EclManager.obj'
    trail = parsed(['scripts/inspect-exattack-type19.py', obj, '--json'],
                   diagnostic=True)
    guard_exattack_type19_update(trail)
    enemy = parsed(['scripts/inspect-enemy-manager-update.py', obj, '--json'])
    guard_enemy_update(enemy)
    exact = []
    if protect_exact:
        exact.extend(cold_replay_exact_source(
            'src/EclManager.cpp', ECL_EXACT_CARRIERS, already_built=(obj,)))
        if include_detached_math:
            exact.extend(cold_replay_exact_source(
                'src/Float3.cpp', {'build/matching/Float3.obj': 4}))
    else:
        build('player-position-callback-type11')
    caller = parsed(['scripts/inspect-player-position-type11.py',
                     'build/matching/EclManager-Type11.obj'])
    if (caller.get('diagnostic_only') is not True or
            caller.get('exactness_credit') != 'none' or
            caller.get('target_sha256') !=
            '3d6ac30acacf80d7a3b9919c4afeb541a9a42b8793e7f7035eab86d384760e92' or
            caller.get('target_bytes') != 732 or caller.get('candidate_bytes') != 732 or
            caller.get('relocations') != 36 or
            caller.get('target_instructions') != 233 or
            caller.get('candidate_instructions') != 233 or
            caller.get('target_calls') != 35 or caller.get('candidate_calls') != 35 or
            caller.get('ordered_calls_agree') is not True or
            caller.get('direct_blocks') != [11, 11] or
            caller.get('complete_direct_graphs_agree') is not True or
            type(caller.get('complete_differences_if_same_extent')) is not int or
            caller.get('complete_differences_if_same_extent') != 0 or
            caller.get('difference_offsets') != [] or
            caller.get('complete_bytes_agree') is not True):
        raise RuntimeError('Type11 complete original owner no longer agrees')
    owners.update(inspect_player_position_pair(obj))
    owners.update(player_position_type11={'physical_bytes': 732, 'fields': 36,
                                         'different_bytes': 0},
                  exattack_type19_update={'physical_bytes': 821,
                                          'different_bytes': trail['overlapping_differences']},
                  enemy_manager_update={'physical_bytes': 3900,
                                        'different_bytes': enemy['complete_differences']})
    return owners, exact



EFFECT34_DRAW_SHA256 = 'd32652c2e08ffbd02a103e379724cde773716f964bbb962e9e4cbd12f58c88a2'
# Effect34 draw was promoted to a canonical exact unit after recovering the
# actual fan-cursor lifetime and the layer/lineColor initialization order.
# The inspector is non-crediting; the canonical 53-field match unit supplies
# separate complete PE/COFF exactness proof. Do not allow any residual bytes.
def guard_effect_type34_draw(result: dict) -> dict:
    noncrediting(result, name='Effect 34 draw', target_size=946,
                 fields=53, fields_key='fields')
    if (result.get('target_sha256') != EFFECT34_DRAW_SHA256 or
            result.get('target_instructions') != 286 or
            result.get('literal_definitions_checked') != 4 or
            result.get('candidate_address_field_count') != 53 or
            result.get('candidate_fields_cover_instructions') is not True or
            result.get('target_blocks') != 16 or
            result.get('candidate_blocks') != 16 or
            result.get('candidate_instructions') != 286 or
            result.get('candidate_bytes') != 946 or
            result.get('ordinal_graphs_agree') is not True or
            result.get('ordered_calls_agree') is not True or
            result.get('field_referent_order_agrees') is not True or
            result.get('COM_immediate_arguments_agree') is not True or
            result.get('missing_bytes') != 0 or
            result.get('extra_bytes') != 0):
        raise RuntimeError('Effect 34 original extent/fields/calls/SDK arguments changed')
    calls = result.get('target_calls', [])
    if (len(calls) != 32 or
            sum(row[0] == 'direct' for row in calls) != 20 or
            result.get('candidate_calls') != calls or
            len(result.get('target_COM_arguments', [])) != 12 or
            result.get('candidate_COM_arguments') != result.get('target_COM_arguments')):
        raise RuntimeError('Effect 34 original direct/COM call trace changed')
    offsets = result.get('difference_offsets')
    if (offsets != [] or result.get('overlapping_differences') != 0 or
            result.get('complete_bytes_agree') is not True or
            result.get('resolved_sha256') != EFFECT34_DRAW_SHA256 or
            result.get('all_original_fields_agree') is not True or
            result.get('direct_graphs_agree') is not True):
        raise RuntimeError('Effect 34 lost complete original-bound byte equality')
    return {'target_bytes': 946, 'candidate_bytes': result['candidate_bytes'],
            'fields': 53, 'ordinary_differences': len(offsets),
            'extra_bytes': result['extra_bytes']}




EFFECT29_DRAW_SHA256 = '8ecf5d55f2ef0de67316f09b3a6d9422b4475bf9a7fdf1ea9dd10919247a3455'
# Complete original-linked residual set from the maintained 2205-byte source.
# Missing tail bytes receive no equality claim. All 1035 old-equal positions
# remain protected when a later source model changes length or scheduling.
EFFECT29_DRAW_RESIDUAL_RUNS = (
    (0x13, 0x14), (0x23, 0x24), (0x26, 0x27), (0x53, 0x54),
    (0x5A, 0x5B), (0x5D, 0x5E), (0x91, 0x92), (0x94, 0x95),
    (0xA4, 0xA5), (0xAD, 0xAE), (0xB0, 0xB1), (0xEF, 0xF0),
    (0xF2, 0xF3), (0xF5, 0xF6), (0x10A, 0x10B), (0x10D, 0x10E),
    (0x112, 0x113), (0x135, 0x136), (0x13E, 0x13F), (0x150, 0x151),
    (0x153, 0x154), (0x156, 0x157), (0x167, 0x168), (0x16E, 0x16F),
    (0x17C, 0x184), (0x185, 0x187), (0x188, 0x191), (0x192, 0x1C1),
    (0x1C2, 0x1FD), (0x1FE, 0x22C), (0x22D, 0x230), (0x231, 0x233),
    (0x235, 0x25A), (0x25B, 0x26C), (0x26D, 0x273), (0x274, 0x27E),
    (0x27F, 0x297), (0x298, 0x2C2), (0x2C3, 0x331), (0x332, 0x33B),
    (0x33E, 0x342), (0x343, 0x379), (0x37A, 0x3A5), (0x3A6, 0x3AD),
    (0x3AF, 0x3EF), (0x3F0, 0x403), (0x404, 0x44F), (0x452, 0x453),
    (0x462, 0x463), (0x46B, 0x46C), (0x46E, 0x46F), (0x4AE, 0x4AF),
    (0x4B1, 0x4B2), (0x4C9, 0x4CA), (0x4CC, 0x4CD), (0x4D1, 0x4D2),
    (0x4D4, 0x4D5), (0x4E8, 0x4E9), (0x4FD, 0x4FE), (0x505, 0x506),
    (0x513, 0x514), (0x519, 0x51A), (0x526, 0x527), (0x534, 0x53C),
    (0x53D, 0x53F), (0x540, 0x549), (0x54A, 0x579), (0x57A, 0x58F),
    (0x592, 0x593), (0x5A2, 0x5A3), (0x5AB, 0x5AC), (0x5AE, 0x5AF),
    (0x5EE, 0x5EF), (0x5F1, 0x5F2), (0x609, 0x60A), (0x60C, 0x60D),
    (0x611, 0x612), (0x614, 0x615), (0x628, 0x629), (0x63D, 0x63E),
    (0x645, 0x646), (0x653, 0x654), (0x659, 0x65A), (0x666, 0x667),
    (0x674, 0x67C), (0x67D, 0x67F), (0x680, 0x689), (0x68A, 0x6B9),
    (0x6BA, 0x6CF), (0x6D2, 0x6D3), (0x6E2, 0x6E3), (0x6EB, 0x6EC),
    (0x6EE, 0x6EF), (0x72E, 0x72F), (0x731, 0x732), (0x749, 0x74A),
    (0x74C, 0x74D), (0x751, 0x752), (0x754, 0x755), (0x768, 0x769),
    (0x77D, 0x77E), (0x785, 0x786), (0x793, 0x794), (0x799, 0x79A),
    (0x7A6, 0x7A7), (0x7B4, 0x7BC), (0x7BD, 0x7BF), (0x7C0, 0x7C9),
    (0x7CA, 0x7F9), (0x7FA, 0x84C), (0x84D, 0x863), (0x864, 0x89D),
)
EFFECT29_DRAW_CALLS = (
    0x401680, 0x4016b0, 0x441890, 0x405730, 0x401680, 0x4016b0, 0x43b7b0, 0x4010b0,
    0x441890, 0x405730, 0x401680, 0x4016b0, 0x441890, 0x405730, 0x401680, 0x4016b0,
    0x43b7b0, 0x4010b0, 0x42aed0, 0x441890, 0x405730, 0x401680, 0x4016b0, 0x441890,
    0x405730, 0x401680, 0x4016b0, 0x43b7b0, 0x4010b0, 0x42aed0, 0x441890, 0x405730,
    0x401680, 0x4016b0, 0x441890, 0x405730, 0x401680, 0x4016b0, 0x43b7b0, 0x4010b0,
    0x42aed0, 0x441890, 0x405730, 0x401680, 0x4016b0, 0x441890, 0x405730, 0x401680,
    0x4016b0, 0x43b7b0, 0x4010b0, 0x42aed0, 0x441890, 0x405730, 0x401680, 0x4016b0,
    0x441890, 0x405730, 0x401680, 0x4016b0, 0x43b7b0, 0x4010b0, 0x42aed0, 0x441890,
    0x405730, 0x401680, 0x4016b0, 0x441890, 0x405730, 0x401680, 0x4016b0, 0x43b7b0,
)

def guard_effect_type29_draw(document: dict) -> dict:
    original = document.get('original', {})
    if not isinstance(original, dict):
        raise RuntimeError('Effect 29 malformed original evidence')
    if (document.get('diagnostic_only') is not True or
            document.get('exactness_credit') != 'none' or
            document.get('maintained_source_present') is not True or
            document.get('object_source_attested') is not False or
            original.get('target_address') != 0x448750 or
            original.get('target_bytes') != 2220 or
            original.get('target_sha256') != EFFECT29_DRAW_SHA256 or
            original.get('table_pointer_address') != 0x4A0E14 or
            original.get('prefix_sha256') !=
            '318a2c5a7e469179735911b94d8837b8b96651b2c89153ea2a15023ec20942b8' or
            original.get('reachable_instructions') != 672 or
            original.get('physical_instructions') != 675 or
            original.get('returns') != [0x448FFB] or
            original.get('following_CC_bytes') != 4):
        raise RuntimeError('Effect 29 original boundary/source evidence changed')
    result = document.get('comparison', {})
    if not isinstance(result, dict):
        raise RuntimeError('Effect 29 malformed comparison evidence')
    noncrediting(result, name='Effect 29 draw', target_size=2220,
                 fields=129, fields_key='fields')
    size = result.get('candidate_bytes')
    if (result.get('target_sha256') != EFFECT29_DRAW_SHA256 or
            result.get('target_address') != 0x448750 or
            result.get('target_instructions') != 675 or
            type(size) is not int or not 2205 <= size <= 2220 or
            result.get('missing_bytes') != 2220 - size or
            result.get('extra_bytes') != 0 or
            result.get('target_blocks') != 41 or
            result.get('candidate_blocks') != 41 or
            result.get('field_referent_order_agrees') is not True or
            result.get('ordered_calls_agree') is not True):
        raise RuntimeError('Effect 29 full owner/fields/calls changed')
    expected_calls = [['direct', address] for address in EFFECT29_DRAW_CALLS]
    if (result.get('target_calls') != expected_calls or
            result.get('candidate_calls') != expected_calls):
        raise RuntimeError('Effect 29 original ordered call trace changed')
    offsets = result.get('difference_offsets')
    known = {i for a, b in EFFECT29_DRAW_RESIDUAL_RUNS for i in range(a, b)}
    if (not isinstance(offsets, list) or
            any(type(i) is not int or i not in known or i >= size for i in offsets) or
            offsets != sorted(set(offsets)) or
            result.get('overlapping_differences') != len(offsets) or
            len(offsets) > 1170):
        raise RuntimeError('Effect 29 changed an already equal original byte')
    equal = not offsets and size == 2220
    if result.get('complete_bytes_agree') is not equal:
        raise RuntimeError('Effect 29 contradictory complete-byte equality')
    if equal and (result.get('resolved_sha256') != EFFECT29_DRAW_SHA256 or
                  result.get('candidate_instructions') != 675 or
                  result.get('all_original_fields_agree') is not True or
                  result.get('direct_graphs_agree') is not True):
        raise RuntimeError('Effect 29 incomplete equality evidence')
    return {'target_bytes': 2220, 'candidate_bytes': size, 'fields': 129,
            'ordinary_differences': len(offsets), 'missing_bytes': 2220 - size,
            'source_present': True, 'exactness_credit': 'none'}


def run_effect_type34(*, protect_exact: bool,
                      protect_update_exact: bool = True,
                      already_built: tuple[str, ...] = ()) -> tuple[dict, list[dict]]:
    obj = 'build/matching/EclManager.obj'
    allowed = set(ECL_EXACT_CARRIERS) | {'build/matching/EffectUpdateType34.obj'}
    if any(item not in allowed for item in already_built):
        raise RuntimeError('unreviewed Effect 34 prebuilt carrier')
    if obj not in already_built:
        build('effect-initialize-callback-type34')
    draw = parsed(['scripts/inspect-effect-type34.py', obj, '--json'])
    summary = guard_effect_type34_draw(draw)
    effect29 = guard_effect_type29_draw(parsed([
        'scripts/inspect-effect-type29.py', obj, '--json']))
    effect26 = guard_effect_type26_update(parsed([
        'scripts/inspect-effect-double-fan.py', obj, '--json']))
    # The same compiler source also owns the existing open bosses. Reuse this
    # freshly built object rather than rebuilding the ECL carrier per reader.
    with tempfile.TemporaryDirectory(prefix='th09-effect34-ecl-') as directory:
        destination = str(Path(directory)/'complete.json')
        execute(['scripts/inspect-ecl-complete.py', obj, '--output', destination],
                diagnostic=True)
        complete = json.loads(Path(destination).read_text())
    shapes = parsed(['scripts/report-ecl-handler-shapes.py', '--json', obj])
    validate_runecl_byte_frontier(complete, shapes)
    enemy = parsed(['scripts/inspect-enemy-manager-update.py', obj, '--json'])
    guard_enemy_update(enemy)
    trail = parsed(['scripts/inspect-exattack-type19.py', obj, '--json'], diagnostic=True)
    guard_exattack_type19_update(trail)
    biased = parsed(['scripts/inspect-ecl-biased-movement.py', '--json', obj], diagnostic=True)
    guard_biased_movement(biased)
    exact = []
    if protect_exact:
        exact.extend(cold_replay_exact_source(
            'src/EclManager.cpp', ECL_EXACT_CARRIERS,
            already_built=tuple(item for item in ECL_EXACT_CARRIERS
                                if item == obj or item in already_built)))
        if protect_update_exact:
            update_obj = 'build/matching/EffectUpdateType34.obj'
            if update_obj not in already_built:
                build('effect-update-callback-type34')
            exact.append(canonical('effect-update-callback-type34'))
    return {'effect_type29_draw': effect29,
            'effect_type34_draw': summary, 'effect_type26_update': effect26,
            'runecl': {'ordinary_differences': complete['ordinary_different_bytes']},
            'enemy_manager_update': {'ordinary_differences': enemy['complete_differences']},
            'exattack_type19_update': {'ordinary_differences': trail['overlapping_differences']}}, exact

def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--owner', choices=['runecl', 'enemy', 'bullet', 'card', 'audio', 'player', 'added', 'positions', 'movement', 'gamemgr', 'crypto', 'effect', 'effect34', 'effect29', 'etama', 'exattack', 'type3', 'type11', 'type19', 'title', 'resultdraw', 'replaymenu', 'anm', 'network', 'netmessage', 'queue', 'parser', 'front', 'frontadded', 'supervisor', 'screen', 'replay', 'replaysave', 'pause', 'all'],
                        default='all')
    parser.add_argument('--skip-exact-siblings', action='store_true',
                        help='diagnostics only; explicitly omit the extra canonical regressions')
    args = parser.parse_args()
    report = {'diagnostic_only': True, 'exactness_credit': 'none',
              'owners': {}, 'canonical_siblings': []}
    try:
        execute(['scripts/verify-target.py'])
        if args.owner in ('type11', 'positions'):
            owners, exact = run_player_type11(
                protect_exact=not args.skip_exact_siblings,
                include_detached_math=args.owner == 'type11')
            report['owners'].update(owners)
            report['canonical_siblings'].extend(exact)
        if args.owner in ('effect34', 'effect29', 'all'):
            owners, exact = run_effect_type34(
                protect_exact=not args.skip_exact_siblings,
                protect_update_exact=args.owner != 'effect29')
            report['owners'].update(owners)
            report['canonical_siblings'].extend(exact)
        if args.owner in ('audio', 'all'):
            owners, exact = run_audio_true_exact(
                protect_exact=not args.skip_exact_siblings)
            report['owners'].update(owners)
            report['canonical_siblings'].extend(exact)
        if args.owner in ('pause', 'all'):
            owners, exact = run_pause(protect_exact=not args.skip_exact_siblings)
            report['owners'].update(owners)
            report['canonical_siblings'].extend(exact)
        if args.owner in ('runecl', 'exattack', 'all'):
            owners, exact = run_ecl(include_interpreter=args.owner in ('runecl', 'all'),
                                    include_trail=args.owner in ('exattack', 'all'),
                                    protect_exact=not args.skip_exact_siblings,
                                    shared_real_callees_in_enemy=args.owner == 'all')
            report['owners'].update(owners)
            report['canonical_siblings'].extend(exact)
        if args.owner in ('type19', 'exattack', 'all'):
            owners, exact = run_exattack_type19_update(
                protect_exact=not args.skip_exact_siblings,
                shared_ecl_prechecked=args.owner in ('exattack', 'all'))
            report['owners'].update(owners)
            report['canonical_siblings'].extend(exact)
        if args.owner in ('type3', 'exattack', 'all'):
            owners, exact = run_exattack_type3(protect_exact=not args.skip_exact_siblings)
            report['owners'].update(owners)
            report['canonical_siblings'].extend(exact)
        if args.owner in ('exattack', 'all'):
            owners, exact = run_exattack_type1422_draw(
                protect_exact=not args.skip_exact_siblings)
            report['owners'].update(owners)
            report['canonical_siblings'].extend(exact)
        if args.owner in ('exattack', 'all'):
            owners, exact = run_exattack_controller(
                protect_exact=not args.skip_exact_siblings,
                sound_prechecked=args.owner == 'all')
            report['owners'].update(owners)
            report['canonical_siblings'].extend(exact)
        if args.owner in ('crypto', 'all'):
            owners, exact = run_decrypt_table(protect_exact=not args.skip_exact_siblings)
            report['owners'].update(owners)
            report['canonical_siblings'].extend(exact)
        if args.owner in ('queue', 'network', 'all'):
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
        if args.owner in ('frontadded', 'all'):
            owners, exact = run_front_added(
                protect_exact=not args.skip_exact_siblings,
                shared_callees_prechecked=args.owner == 'all')
            report['owners'].update(owners)
            report['canonical_siblings'].extend(exact)
        if args.owner in ('supervisor', 'all'):
            owners, exact = run_supervisor_update(protect_exact=not args.skip_exact_siblings)
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
        if args.owner in ('netmessage', 'all'):
            report['owners'].update(run_network_message())
        if args.owner in ('replay', 'all'):
            owners, exact = run_replay_playback(protect_exact=not args.skip_exact_siblings)
            report['owners'].update(owners)
            report['canonical_siblings'].extend(exact)
        if args.owner in ('replaysave', 'all'):
            owners, exact = run_replay_save(
                protect_exact=not args.skip_exact_siblings,
                already_checked=args.owner == 'all')
            report['owners'].update(owners)
            report['canonical_siblings'].extend(exact)
        if args.owner in ('movement', 'all'):
            owners, exact = run_player_movement(protect_exact=not args.skip_exact_siblings)
            report['owners'].update(owners)
            report['canonical_siblings'].extend(exact)
        if args.owner in ('added', 'all'):
            owners, exact = run_player_added_state(protect_exact=not args.skip_exact_siblings)
            report['owners'].update(owners)
            report['canonical_siblings'].extend(exact)
        if args.owner in ('player', 'all'):
            owners, exact = run_player_collision(protect_exact=not args.skip_exact_siblings)
            report['owners'].update(owners)
            report['canonical_siblings'].extend(exact)
        if args.owner in ('gamemgr', 'all'):
            owners, exact = run_game_manager(
                protect_exact=not args.skip_exact_siblings,
                replay_callees_prechecked=args.owner == 'all' and
                not args.skip_exact_siblings)
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
            owners, exact = run_etama(
                protect_exact=not args.skip_exact_siblings,
                real_angle_prechecked=args.owner == 'all')
            report['owners'].update(owners)
            report['canonical_siblings'].extend(exact)
        if args.owner in ('card', 'all'):
            owners, exact = run_card_real_sound(
                protect_exact=not args.skip_exact_siblings,
                sound_prechecked=args.owner == 'all')
            report['owners'].update(owners)
            report['canonical_siblings'].extend(exact)
        if args.owner in ('bullet', 'all'):
            owners, exact = run_bullet_sound(
                protect_exact=not args.skip_exact_siblings,
                sound_prechecked=args.owner == 'all',
                real_angle_prechecked=args.owner == 'all',
                real_anm_prechecked=args.owner == 'all',
                etama_prechecked=args.owner == 'all')
            report['owners'].update(owners)
            report['canonical_siblings'].extend(exact)
        if args.owner in ('title', 'all'):
            owners, exact = run_title(protect_exact=not args.skip_exact_siblings,
                                      play_sound_prechecked=args.owner == 'all')
            report['owners'].update(owners)
            report['canonical_siblings'].extend(exact)
            report['owners'].update(run_title_open())
        if args.owner == 'replaymenu':
            report['owners'].update(run_title_replay_menu())
        if args.owner == 'resultdraw':
            report['owners'].update(run_title_result())
        if args.owner == 'all':
            report['canonical_siblings'] = unique_canonical_siblings(
                report['canonical_siblings'])
        print(json.dumps(report, indent=2))
        return 0
    except (OSError, RuntimeError, ValueError, KeyError, json.JSONDecodeError) as exc:
        print('boss frontier replay failed:', exc, file=sys.stderr)
        return 1


if __name__ == '__main__':
    raise SystemExit(main())
