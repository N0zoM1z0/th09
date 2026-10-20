#!/usr/bin/env python3
"""Original-bound, source-cold TH09 TitleScreen::OnUpdateOptions audit.

The 2045-byte original has 135 external operands, 51 ordered calls and 121
complete direct control-flow blocks. This inspector hashes and independently
decodes the original executable *before* binding genuine source COFF fields.
It cannot grant exactness credit, including when a local candidate is the
same length, or when two instructions happen to have similar shape.
"""

from __future__ import annotations

import argparse
import difflib
import hashlib
import importlib.util
import json
from pathlib import Path
import struct
import sys

ROOT = Path(__file__).resolve().parents[1]
BASE, SIZE = 0x004276EB, 2045
SYMBOL = '?OnUpdateOptions@TitleScreenView@@QAEHXZ'
TARGET_SHA256 = '869fec21781b663202298f8486ebf8d0d9e42c0c1d34f1d3b08bf860b247eb63'
ORIGINAL_INSTRUCTIONS = 551
ORIGINAL_FIELDS = 135
ORIGINAL_CALLS = 51
ORIGINAL_BLOCKS = 121
NATIVE_INPUT_SYMBOL = '?g_ReplayInputStates@@3PAUReplayInputState@@A'
NATIVE_INPUT_ROOT = 0x004ACE18
# Independently decoded original operands: two scrolling receivers and six
# front-word reads. The third native input slot starts at root + 2 * 0x8E.
ORIGINAL_INPUT_FIELDS = (
    (0x2DD, 0x004ACF34), (0x408, 0x004ACF3A),
    (0x44E, 0x004ACF3A), (0x49D, 0x004ACF3A),
    (0x4DC, 0x004ACF3A), (0x532, 0x004ACF34),
    (0x68F, 0x004ACF3A), (0x738, 0x004ACF3A),
)

DESTINATIONS = {
    # Shared Supervisor owner and methods used by the maintained consumer.
    # The older view/field aliases below remain for historical objects.
    '?g_Supervisor@@3VSupervisor@@A': 0x4B3100,
    '?LoadMusic@Supervisor@@QAEHH@Z': 0x42FC20,
    '?PlayMusic@Supervisor@@QAEHHH@Z': 0x431930,
    '?StopAudio@Supervisor@@QAEHXZ': 0x42FD30,
    '?ChangeCurrentScreen@TitleScreenView@@QAEHH@Z': 0x422F39,
    '?DrawTitleHelpText@@YAXPAUTitleAnmManagerView@@PAUAnmVmView@@IHPBD@Z': 0x43BE50,
    '?ExecuteScriptArray@AnmManager@@QAEXPAUAnmVm@@H@Z': 0x439560,
    '?IsPressedScrolling@InputView@@QAEGH@Z': 0x423158,
    '?LoadMusic@TitleSupervisorView@@QAEHH@Z': 0x42FC20,
    '?MoveCursorVertical@TitleScreenView@@QAEHH@Z': 0x42457A,
    '?OnUpdateStartMenu@TitleScreenView@@QAEHXZ': 0x429E23,
    '?PlayMenuSound@TitleScreenView@@QAEHHH@Z': 0x422F17,
    '?PlayMusic@TitleSupervisorView@@QAEHHH@Z': 0x431930,
    '?PlaySoundByIdx@SoundPlayer@@QAEXHH@Z': 0x43E2F0,
    '?QueueCommand@SoundPlayer@@QAEXHHPAD@Z': 0x43EE40,
    '?SetInterruptArray@AnmManager@@QAEXPAUAnmVm@@HF@Z': 0x436DE0,
    '?SetMenuSelectionSprites@TitleScreenView@@QAEXHHH@Z': 0x424DFF,
    '?SetSprite@AnmLoaded@@QAEHPAUAnmVm@@H@Z': 0x436AC0,
    '?StopAudio@TitleSupervisorView@@QAEHXZ': 0x42FD30,
    '?UpdateMenuSelection@TitleScreenView@@QAEXXZ': 0x4276D7,
    '?g_OptionA@@3EA': 0x4B3534,
    '?g_OptionB@@3EA': 0x4B3535,
    '?g_OptionC@@3EA': 0x4B3536,
    '?g_OptionD@@3EA': 0x4B3539,
    '?g_OptionUnknown3537@@3EA': 0x4B3537,
    '?g_SoundPlayer@@3VSoundPlayer@@A': 0x4DC698,
    '?g_AnmManager@@3PAVAnmManager@@A': 0x4DC550,
    '?g_TitleInput@@3UInputView@@A': 0x4ACF34,
    '?g_TitleInputFlags@@3IA': 0x4ACF3A,
    NATIVE_INPUT_SYMBOL: NATIVE_INPUT_ROOT,
    '?g_TitleMusicVolume@@3CA': 0x4B3542,
    '?g_TitleOptionsHelpText@@3PAPBDA': 0x4A1DDC,
    '?g_TitleSfxVolume@@3CA': 0x4B3543,
    '?g_TitleSupervisor@@3UTitleSupervisorView@@A': 0x4B3100,
    '??_C@_06EAGAPDIF@SetVol?$AA@': 0x48F518,
}


def require_native_input_references(references: list[dict], candidate_bytes: int) -> None:
    """Reject old subobject globals even when their numeric addresses agree."""
    if (type(candidate_bytes) is not int or candidate_bytes < SIZE or
            not isinstance(references, list) or len(references) != 8):
        raise ValueError('Options native input reference inventory changed')
    previous_end = 0
    for row, (original_offset, destination) in zip(references, ORIGINAL_INPUT_FIELDS):
        if (not isinstance(row, dict) or
                any(type(row.get(key)) is not int for key in
                    ('original_offset', 'original_target', 'root', 'addend')) or
                row.get('original_offset') != original_offset or
                row.get('original_target') != destination or
                row.get('type') != 'DIR32' or
                row.get('symbol') != NATIVE_INPUT_SYMBOL or
                row.get('root') != NATIVE_INPUT_ROOT or
                row.get('addend') != destination - NATIVE_INPUT_ROOT):
            raise ValueError('Options input must refer to the native array slot/member')
        offset = row.get('candidate_offset')
        if (type(offset) is not int or offset < previous_end or
                offset + 4 > candidate_bytes):
            raise ValueError('Options native input source field bounds/order changed')
        previous_end = offset + 4


def require_original_identity(target: bytes, expected_hash: str = TARGET_SHA256) -> None:
    if len(target) != SIZE or hashlib.sha256(target).hexdigest() != expected_hash:
        raise ValueError('original TH09 Options physical SHA256/extent changed')


def full_instructions(code: bytes, *, base: int = BASE):
    import capstone
    decoder = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32)
    decoder.detail = True
    instructions = list(decoder.disasm(code, base))
    if not instructions or sum(i.size for i in instructions) != len(code):
        raise ValueError('Options complete physical owner does not decode')
    if instructions[-1].mnemonic != 'ret' or instructions[-1].op_str:
        raise ValueError('Options source/target return ABI changed')
    return instructions


def absolute_operands(instructions, *, base: int = BASE):
    """Discover original absolute data and real call operands; exclude jumps."""
    import capstone
    fields, calls = [], []
    for instruction in instructions:
        if instruction.mnemonic.startswith('j'):
            continue
        if instruction.mnemonic == 'call':
            if (len(instruction.operands) != 1 or
                    instruction.operands[0].type != capstone.x86.X86_OP_IMM or
                    instruction.imm_size != 4):
                raise ValueError('Options has an unreviewed indirect/short call')
            address = instruction.operands[0].imm
            fields.append((instruction.address - base + instruction.imm_offset,
                           'REL32', address))
            calls.append(address)
            continue
        for operand in instruction.operands:
            if operand.type == capstone.x86.X86_OP_MEM and instruction.disp_size == 4:
                if 0x00400000 <= operand.mem.disp < 0x004E7000:
                    fields.append((instruction.address - base + instruction.disp_offset,
                                   'DIR32', operand.mem.disp))
            elif operand.type == capstone.x86.X86_OP_IMM and instruction.imm_size == 4:
                if 0x00400000 <= operand.imm < 0x004E7000:
                    fields.append((instruction.address - base + instruction.imm_offset,
                                   'DIR32', operand.imm))
    return fields, calls


def bind_candidate(raw: bytes, rows: list[dict], original: list[tuple],
                   *, symbols: dict[str, int] = DESTINATIONS) -> bytes:
    if not 1970 <= len(raw) <= 2120:
        raise ValueError('Options candidate has an unreviewed physical extent')
    if len(original) != ORIGINAL_FIELDS or len(rows) != ORIGINAL_FIELDS:
        raise ValueError('Options original/candidate 135-operand inventory changed')
    result = bytearray(raw)
    last_end = 0
    for row, (original_offset, kind, original_destination) in zip(rows, original):
        symbol = row['symbol']
        if symbol not in symbols:
            raise ValueError('Options unknown source COFF referent: ' + symbol)
        candidate_offset = row['offset']
        actual_destination = symbols[symbol] + row['addend']
        if (row['type'] != kind or actual_destination != original_destination or
                kind not in ('DIR32', 'REL32')):
            raise ValueError('Options original address identity/order/addend diverged')
        if (not isinstance(candidate_offset, int) or candidate_offset < last_end or
                candidate_offset + 4 > len(raw)):
            raise ValueError('Options candidate COFF field reordered/overlaps/out-of-range')
        last_end = candidate_offset + 4
        address_value = actual_destination
        if kind == 'REL32':
            address_value -= BASE + candidate_offset + 4
        struct.pack_into('<I', result, candidate_offset, address_value & 0xFFFFFFFF)
    return bytes(result)


def diagnose(path: Path, *, require_native_input: bool = False) -> dict:
    import capstone
    from diagnostic_branch_alignment import aligned_branch_conflicts, direct_control_flow

    spec = importlib.util.spec_from_file_location(
        'th09_options_complete_coff', ROOT / 'scripts/compare-coff-function.py')
    coff = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(coff)
    original = coff.pe_bytes_at(coff.verified_target(), BASE, SIZE)
    require_original_identity(original)
    target_ins = full_instructions(original)
    target_fields, target_calls = absolute_operands(target_ins)
    if (len(target_ins), len(target_fields), len(target_calls)) != (
            ORIGINAL_INSTRUCTIONS, ORIGINAL_FIELDS, ORIGINAL_CALLS):
        raise ValueError('original TH09 Options instruction/address/call census changed')

    raw, rows = coff.object_function(path, SYMBOL)
    linked = bind_candidate(raw, rows, target_fields)
    candidate_ins = full_instructions(linked)
    candidate_fields, candidate_calls = absolute_operands(candidate_ins)
    expected_fields = [(row['offset'], kind, target)
                       for row, (_, kind, target) in zip(rows, target_fields)]
    if candidate_fields != expected_fields or candidate_calls != target_calls:
        raise ValueError('candidate Options decoded source COFF operands or calls differ')

    target_graph = direct_control_flow(target_ins, capstone.x86.X86_OP_IMM)
    candidate_graph = direct_control_flow(candidate_ins, capstone.x86.X86_OP_IMM)
    if len(target_graph) != ORIGINAL_BLOCKS:
        raise ValueError('original TH09 Options 121-block graph drifted')

    def normal(instruction):
        if instruction.mnemonic.startswith('j') or instruction.mnemonic == 'call':
            return instruction.mnemonic, 'direct'
        return instruction.mnemonic, instruction.op_str

    alignment = difflib.SequenceMatcher(
        None, list(map(normal, target_ins)), list(map(normal, candidate_ins)),
        autojunk=False)
    branch_conflicts, unpaired = aligned_branch_conflicts(
        target_ins, candidate_ins, alignment.get_matching_blocks(),
        capstone.x86.X86_OP_IMM)
    input_references = [
        {'original_offset': field[0], 'original_target': field[2],
         'candidate_offset': row['offset'], 'type': row['type'],
         'symbol': row['symbol'], 'root': DESTINATIONS[row['symbol']],
         'addend': row['addend']}
        for row, field in zip(rows, target_fields)
        if field[1] == 'DIR32' and field[2] in (0x4ACF34, 0x4ACF3A)
    ]
    native_input = True
    try:
        require_native_input_references(input_references, len(linked))
    except ValueError:
        if require_native_input:
            raise
        native_input = False
    return {
        'diagnostic_only': True, 'exactness_credit': 'none',
        'target_bytes': len(original), 'candidate_bytes': len(linked),
        'target_instructions': len(target_ins),
        'candidate_instructions': len(candidate_ins),
        'original_fields': len(target_fields), 'bound_fields': len(rows),
        'ordered_calls': len(target_calls), 'ordered_calls_agree': candidate_calls == target_calls,
        'original_blocks': len(target_graph), 'candidate_blocks': len(candidate_graph),
        'direct_graphs_agree': target_graph == candidate_graph,
        'aligned_instructions': sum(block.size for block in alignment.get_matching_blocks()),
        'paired_branch_conflicts': [tuple(map(hex, row)) for row in branch_conflicts],
        'unpaired_branch_destinations': unpaired,
        'complete_differences': sum(a != b for a, b in zip(original, linked))
            if len(original) == len(linked) else None,
        'overlap_differences': sum(a != b for a, b in zip(original, linked)),
        'missing_target_bytes': max(0, len(original) - len(linked)),
        'extra_candidate_bytes': max(0, len(linked) - len(original)),
        'first_overlapping_difference': next((n for n, (a, b) in
            enumerate(zip(original, linked)) if a != b), None),
        'complete_bytes_equal': original == linked,
        'candidate_raw_sha256': hashlib.sha256(raw).hexdigest(),
        'target_sha256': TARGET_SHA256,
        'input_references': input_references,
        'native_input_array_references_verified': native_input,
    }


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('object', type=Path)
    parser.add_argument('--json', action='store_true', help='emit machine-readable full-owner audit')
    parser.add_argument('--require-native-input', action='store_true',
                        help='require the native ReplayInputState[2] root and all eight actual addends')
    args = parser.parse_args()
    try:
        report = diagnose(args.object, require_native_input=args.require_native_input)
    except (OSError, ImportError, ValueError, KeyError, struct.error) as error:
        print('Options source/target audit rejected:', error, file=sys.stderr)
        return 2
    if args.json:
        print(json.dumps(report, indent=2))
    else:
        print('NON-CREDITING:', report['candidate_bytes'], 'bytes',
              report['candidate_instructions'], 'instructions',
              report['bound_fields'], 'fields', report['candidate_raw_sha256'])
        print('complete owned bytes agree:', report['complete_bytes_equal'],
              'length delta:', report['candidate_bytes'] - report['target_bytes'],
              'first overlapping difference:', report['first_overlapping_difference'])
        print('ordered calls agree:', report['ordered_calls_agree'],
              'alignment:', report['aligned_instructions'], '/',
              report['target_instructions'])
        print('paired CFG conflicts:', report['paired_branch_conflicts'],
              'unpaired:', report['unpaired_branch_destinations'])
        print('complete direct block graphs:', report['original_blocks'],
              report['candidate_blocks'], 'edges/per-block calls/return cleanup agree:',
              report['direct_graphs_agree'])
        if report['complete_differences'] is not None:
            print('complete replay differences:', report['complete_differences'])
    return 0


if __name__ == '__main__':
    raise SystemExit(main())
