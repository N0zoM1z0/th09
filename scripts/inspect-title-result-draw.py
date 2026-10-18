#!/usr/bin/env python3
"""Read-only complete DrawResult replay; diagnostic only, never match credit."""
import argparse
import capstone
import difflib
import hashlib
import importlib.util
import json
from pathlib import Path
import struct
from diagnostic_branch_alignment import aligned_branch_conflicts, direct_control_flow

ROOT = Path(__file__).resolve().parents[1]
BASE, SIZE = 0x423D16, 1939
DRAW_RESULT_ORIGINAL_SHA256 = '92279dc6d2b9ad5051fb902e6172d88eb7f9febe460d44459f16af0f45c74b0a'
SYMBOL = '?DrawResult@TitleScreenView@@QAEHXZ'
# TH09-local target operands and literal contents reviewed independently.
DESTINATIONS = {
    '?AddFormatText@AsciiManager@@QAAXPAUFloat3@@PBDZZ': 0x434330,
    '?AddString@AsciiManager@@QAEXPAUFloat3@@PBD@Z': 0x4342A0,
    '?g_AsciiManager@@3VAsciiManager@@A': 0x4CE458,
    '?g_GameManager@@3UGameManagerModeView@@A': 0x4A7D90,
    '?g_TitleAlphabet@@3PADA': 0x4A1CF4,
    # Retained only to inspect source-bound historical objects. Current source
    # addresses the same +0x11C field through the reviewed GameManager root.
    '?g_TitleNameTableIndex@@3HA': 0x4A7EAC,
    '?g_TitleRankLabels@@3PAPBDA': 0x4A1D68,
    '?g_TitleScoreTable@@3PAY144UTitleScoreRecordView@@A': 0x4A8380,
    '??_C@_01IDAFKMJL@_?$AA@': 0x48F5A8,
    '??_C@_04FOHLOOIG@?$CF?48s?$AA@': 0x48F5A0,
    '??_C@_0BA@EHBBDLHE@Lunatic?5Ranking?$AA@': 0x48F5E8,
    '??_C@_0N@JAEECOBI@Hard?5Ranking?$AA@': 0x48F5F8,
    '??_C@_0N@MELGJLPO@Easy?5Ranking?$AA@': 0x48F628,
    '??_C@_0O@POHKDEEG@Extra?5Ranking?$AA@': 0x48F5D8,
    '??_C@_0P@COFLBEKD@?$CFs?5?$CF?48s?5?$CF?49d?$CFd?$AA@': 0x48F618,
    '??_C@_0P@KJMNBHBM@Normal?5Ranking?$AA@': 0x48F608,
    '__real@3ccccccd': 0x48F598,
    '__real@3f800000': 0x48E2A4,
    '__real@3f99999a': 0x48E830,
    '__real@40000000': 0x48EF5C,
    '__real@41400000': 0x48F59C,
    '__real@41800000': 0x48E470,
    '__real@c1000000': 0x48F594,
}


def original_address_fields(instructions):
    """Decode original physical absolute operands and direct call destinations.

    A jump's immediate is a control-flow destination, not an external code/data
    field. Indexed absolute x86 addressing is still a valid DIR32 data operand.
    """
    result = []
    for ins in instructions:
        if ins.mnemonic.startswith('j'):
            continue
        if ins.mnemonic == 'call':
            if (len(ins.operands) != 1 or
                    ins.operands[0].type != capstone.x86.X86_OP_IMM or
                    ins.imm_size != 4):
                raise ValueError('unreviewed Title renderer indirect/short call')
            result.append((ins.address - BASE + ins.imm_offset,
                           'REL32', ins.operands[0].imm))
            continue
        for operand in ins.operands:
            if operand.type == capstone.x86.X86_OP_MEM and ins.disp_size == 4:
                if 0x00400000 <= operand.mem.disp < 0x004E7000:
                    result.append((ins.address - BASE + ins.disp_offset,
                                   'DIR32', operand.mem.disp))
            elif operand.type == capstone.x86.X86_OP_IMM and ins.imm_size == 4:
                if 0x00400000 <= operand.imm < 0x004E7000:
                    result.append((ins.address - BASE + ins.imm_offset,
                                   'DIR32', operand.imm))
    return result


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('object', type=Path)
    parser.add_argument('--instructions', action='store_true')
    parser.add_argument('--summary', action='store_true')
    parser.add_argument('--json', action='store_true', help='emit structured complete-owner diagnostic')
    args = parser.parse_args()
    spec = importlib.util.spec_from_file_location('coff', ROOT / 'scripts/compare-coff-function.py')
    coff = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(coff)
    target = coff.pe_bytes_at(coff.verified_target(), BASE, SIZE)
    target_sha256 = hashlib.sha256(target).hexdigest()
    if (BASE, SIZE) == (0x00423D16, 1939) and target_sha256 != DRAW_RESULT_ORIGINAL_SHA256:
        raise ValueError('original TH09 DrawResult full 1939-byte owner digest differs')
    code, rows = coff.object_function(args.object, SYMBOL)
    raw_hash = hashlib.sha256(code).hexdigest()
    for row in rows:
        value = DESTINATIONS[row['symbol']] + row['addend']
        if row['type'] == 'REL32':
            value -= BASE + row['offset'] + 4
        elif row['type'] != 'DIR32':
            raise ValueError('unreviewed relocation type')
        struct.pack_into('<I', code, row['offset'], value & 0xFFFFFFFF)
    decoder = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32)
    decoder.detail = True
    t, c = list(decoder.disasm(target, BASE)), list(decoder.disasm(code, BASE))
    if sum(i.size for i in t) != len(target) or sum(i.size for i in c) != len(code):
        raise ValueError('incomplete function decode')
    reviewed_inventories = {
        (0x00423D16, 1939): (577, 82),  # DrawResult
        (0x004239F6, 800): (245, 41),   # DrawReplaySave sibling
    }
    if (BASE, SIZE) in reviewed_inventories:
        # The replay-save sibling imports this inspector with its own genuine
        # TH09 owner, literal and callee manifest. Each original operand list
        # is decoded from the private target, not constructed from COFF fields.
        target_instructions, expected_fields = reviewed_inventories[(BASE, SIZE)]
        original_fields = original_address_fields(t)
        candidate_fields = original_address_fields(c)
        if (len(t), len(original_fields), len(rows)) != (
                target_instructions, expected_fields, expected_fields):
            raise ValueError('original Title renderer extent / field census changed')
        if len(candidate_fields) != len(rows):
            raise ValueError('DrawResult candidate machine operands are not complete')
        for row, original, candidate in zip(rows, original_fields, candidate_fields):
            if row['symbol'] not in DESTINATIONS:
                raise ValueError('unreviewed Title renderer symbol')
            dest = DESTINATIONS[row['symbol']] + row['addend']
            target_off, target_kind, target_dest = original
            if (row['type'], dest) != (target_kind, target_dest):
                raise ValueError('original Title renderer operand identity/order changed')
            expected_original = dest - (BASE + target_off + 4 if target_kind == 'REL32' else 0)
            if struct.unpack_from('<I', target, target_off)[0] != expected_original & 0xFFFFFFFF:
                raise ValueError('Title renderer original field encoding changed')
            if candidate != (row['offset'], row['type'], dest):
                raise ValueError('candidate Title renderer COFF field not decoded at original identity')

    def normalized(i):
        if i.mnemonic.startswith('j') or i.mnemonic == 'call':
            return i.mnemonic, 'direct'
        return i.mnemonic, i.op_str
    matcher = difflib.SequenceMatcher(None, list(map(normalized, t)), list(map(normalized, c)), autojunk=False)
    calls = lambda ins: [i.operands[0].imm for i in ins if i.mnemonic == 'call']
    # Physical linked differences are a complete-owner byte census. Never
    # compare an aligned-normalized instruction slice in place of these bytes.
    difference_offsets = ([f'0x{i:x}' for i, (a, b) in
                           enumerate(zip(target, code)) if a != b]
                          if len(code) == len(target) else None)
    first = int(difference_offsets[0], 16) if difference_offsets else None
    conflicts, unpaired = aligned_branch_conflicts(t, c, matcher.get_matching_blocks(), capstone.x86.X86_OP_IMM)
    target_cfg = direct_control_flow(t, capstone.x86.X86_OP_IMM)
    candidate_cfg = direct_control_flow(c, capstone.x86.X86_OP_IMM)
    report = {
        'diagnostic_only': True, 'exactness_credit': 'none',
        'target_bytes': len(target), 'candidate_bytes': len(code),
        'target_sha256': target_sha256,
        'target_instructions': len(t), 'candidate_instructions': len(c),
        'original_fields': len(original_address_fields(t)),
        'bound_fields': len(rows),
        'ordered_calls': len(calls(t)), 'ordered_calls_agree': calls(t) == calls(c),
        'original_blocks': len(target_cfg), 'candidate_blocks': len(candidate_cfg),
        'direct_graphs_agree': target_cfg == candidate_cfg,
        'aligned_instructions': sum(b.size for b in matcher.get_matching_blocks()),
        'paired_branch_conflicts': [tuple(map(hex, row)) for row in conflicts],
        'unpaired_branch_destinations': unpaired,
        'first_overlapping_difference': first,
        'complete_differences': len(difference_offsets) if difference_offsets is not None else None,
        'difference_offsets': difference_offsets,
        'complete_bytes_equal': code == target,
        'candidate_raw_sha256': raw_hash,
    }
    if args.json:
        print(json.dumps(report, indent=2))
        return
    print('NON-CREDITING:', len(code), 'bytes', len(c), 'instructions', len(rows), 'fields', raw_hash)
    print('complete owned bytes agree:', code == target,
          'length delta:', len(code) - len(target), 'first overlapping difference:', first)
    print('ordered calls agree:', calls(t) == calls(c), 'alignment:',
          sum(b.size for b in matcher.get_matching_blocks()), '/', len(t))
    print('paired CFG conflicts:', report['paired_branch_conflicts'], 'unpaired:', unpaired)
    print('complete direct block graphs:', len(target_cfg), len(candidate_cfg),
          'edges/per-block calls/return cleanup agree:', target_cfg == candidate_cfg)
    if report['complete_differences'] is not None:
        print('complete replay differences:', report['complete_differences'])
    if args.summary:
        return
    if args.instructions:
        for n, i in enumerate(c):
            print(n, hex(i.address), i.mnemonic, i.op_str)
    else:
        for tag, a, b, x, y in matcher.get_opcodes():
            if tag != 'equal':
                print(tag, [(hex(i.address), i.mnemonic, i.op_str) for i in t[a:b]],
                      '=>', [(hex(i.address), i.mnemonic, i.op_str) for i in c[x:y]])


if __name__ == '__main__':
    main()
