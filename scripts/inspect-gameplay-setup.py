#!/usr/bin/env python3
"""Read-only complete gameplay setup replay; diagnostic only, no match credit."""
import argparse
import capstone
import difflib
import hashlib
import importlib.util
from pathlib import Path
import struct

from diagnostic_branch_alignment import aligned_branch_conflicts, direct_control_flow

ROOT = Path(__file__).resolve().parents[1]
BASE, SIZE = 0x41AF2D, 1689
SYMBOL = '?GameplaySetupThread@@YIXPAX@Z'
# TH09 instructions, callee entries and IAT operands reviewed independently.
# Old ReleaseStageObject is deliberately bound to its real canonical entry,
# not the full-release target, so the former caller contract fails visibly.
DESTINATIONS = {
    '?AdvanceTimedState@GameManagerSetupLayout@@QAEXXZ': 0x41AA07,
    '?BeginLoading@@YIXPAVSupervisor@@@Z': 0x4307C0,
    '?CreateGameSubsystem@@YIPAXXZ': 0x418840,
    '?CreateStageObject@@YIPAXHPAX@Z': 0x421670,
    '?FinishLoading@@YIXPAVSupervisor@@@Z': 0x4304A0,
    '?FormatCurrentDateString@@YIXPAD@Z': 0x41A796,
    '?Initialize@GameConfiguration@@QAEXXZ': 0x41A8A2,
    '?InitializeGameSubsystems@@YIXXZ': 0x41CDE0,
    '?RegisterSecondarySubsystem@@YIPAXXZ': 0x4157D0,
    '?RegisterSharedSubsystem@@YIPAXHHH@Z': 0x40D320,
    '?RegisterSubsystem0@@YIPAXH@Z': 0x403AE0,
    '?RegisterSubsystem1@@YIPAXHHH@Z': 0x41F170,
    '?RegisterSubsystem2@@YIPAXH@Z': 0x4150D0,
    '?RegisterSubsystem3@@YIPAXHHH@Z': 0x40D320,
    '?RegisterSubsystem4@@YIPAXH@Z': 0x412240,
    '?RegisterSubsystem5@@YIPAXH@Z': 0x404600,
    '?RegisterSubsystem6@@YIPAXH@Z': 0x41A5D0,
    '?ReleaseStageObject@@YIXPAX@Z': 0x420C10,
    '?ResetGameManager@@YIXPAUGameManagerSetupLayout@@@Z': 0x41A7C3,
    '?SetStageObjectMode@@YIXPAXH@Z': 0x4217E0,
    '?SupervisorCalculateFps@@YIXH@Z': 0x4316B0,
    '?UpdateProgress@GameManagerSetupLayout@@QAEXXZ': 0x415910,
    '?g_GameConfiguration@@3PAUGameConfiguration@@A': 0x4A7E78,
    '?g_GameManager@@3UGameManagerSetupLayout@@A': 0x4A7D90,
    '?g_ReplayPlayTimeText@@3PADA': 0x4AC879,
    '?g_SetupSeedSource@@3GA': 0x4ACE0C,
    '?g_SetupStatus@@3HA': 0x4AC884,
    '?g_Supervisor@@3VSupervisor@@A': 0x4B3100,
    '__imp__Sleep@4': 0x48E058,
    '__imp__timeGetTime@0': 0x48E238,
    '__real@40000000': 0x48EF5C,
    '_free': 0x47B249,
    '??2@YAPAXI@Z': 0x47B24E,
    '?Release@ReplayManagerSetupReleaseView@@QAEXXZ': 0x420AD0,
    '?InitializeViewports@Supervisor@@QAEXXZ': 0x4307C0,
    '?PreloadPlayerAnmResources@@YIHXZ': 0x41CDE0,
}


def call_signature(ins):
    if len(ins.operands) != 1:
        raise ValueError('unreviewed call operand count')
    operand = ins.operands[0]
    if operand.type == capstone.x86.X86_OP_IMM:
        return 'direct', operand.imm
    if operand.type == capstone.x86.X86_OP_MEM:
        memory = operand.mem
        reviewed_iat = {DESTINATIONS['__imp__Sleep@4'], DESTINATIONS['__imp__timeGetTime@0']}
        if (memory.segment or memory.base or memory.index or
                memory.scale != 1 or memory.disp not in reviewed_iat):
            raise ValueError('unreviewed indirect call operand')
        return 'iat-cell', memory.disp
    raise ValueError('unreviewed call operand')


def address_fields(instructions):
    """Decode actual address operands independently of candidate COFF fields."""
    fields = []
    for ins in instructions:
        offset = ins.address - BASE
        is_jump = ins.group(capstone.CS_GRP_JUMP)
        if ins.mnemonic == 'call' and call_signature(ins)[0] == 'direct':
            fields.append((offset + ins.imm_offset, 'REL32', ins.operands[0].imm))
        if ins.disp_size == 4:
            for operand in ins.operands:
                if (operand.type == capstone.x86.X86_OP_MEM and
                        0x400000 <= operand.mem.disp < 0x4E7000):
                    fields.append((offset + ins.disp_offset, 'DIR32', operand.mem.disp))
        if ins.imm_size == 4 and not is_jump and ins.mnemonic != 'call':
            for operand in ins.operands:
                if (operand.type == capstone.x86.X86_OP_IMM and
                        0x400000 <= operand.imm < 0x4E7000):
                    fields.append((offset + ins.imm_offset, 'DIR32', operand.imm))
    return sorted(fields)


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('object', type=Path)
    parser.add_argument('--summary', action='store_true')
    parser.add_argument('--instructions', action='store_true')
    args = parser.parse_args()
    spec = importlib.util.spec_from_file_location(
        'coff', ROOT / 'scripts/compare-coff-function.py')
    coff = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(coff)
    target = coff.pe_bytes_at(coff.verified_target(), BASE, SIZE)
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
    expected_fields = sorted((row['offset'], row['type'],
                              DESTINATIONS[row['symbol']] + row['addend'])
                             for row in rows)
    target_fields, candidate_fields = address_fields(t), address_fields(c)
    if candidate_fields != expected_fields:
        raise ValueError('COFF fields do not cover actual candidate address operands')

    def normalized(i):
        if i.mnemonic.startswith('j'):
            return i.mnemonic, 'direct'
        return i.mnemonic, i.op_str

    matcher = difflib.SequenceMatcher(
        None, list(map(normalized, t)), list(map(normalized, c)), autojunk=False)
    calls = lambda ins: [call_signature(i) for i in ins if i.mnemonic == 'call']
    tc, cc = calls(t), calls(c)
    print('NON-CREDITING:', len(code), 'bytes', len(c), 'instructions',
          len(rows), 'fields', raw_hash)
    print('decoded target/candidate address fields:',
          len(target_fields), len(candidate_fields),
          'candidate COFF operand coverage agrees:', candidate_fields == expected_fields)
    print('complete owned bytes agree:', code == target,
          'length delta:', len(code) - len(target),
          'first overlapping difference:', next(
              (n for n, (a, b) in enumerate(zip(target, code)) if a != b), None))
    print('ordered calls agree:', tc == cc, 'census:', len(tc), len(cc))
    print('call mismatches:', [(n, a, b) for n, (a, b) in enumerate(zip(tc, cc)) if a != b])
    print('normalized alignment:',
          sum(b.size for b in matcher.get_matching_blocks()), '/', len(t))
    conflicts, unpaired = aligned_branch_conflicts(
        t, c, matcher.get_matching_blocks(), capstone.x86.X86_OP_IMM)
    print('paired CFG conflicts:', [tuple(map(hex, row)) for row in conflicts],
          'unpaired:', unpaired)
    target_cfg = direct_control_flow(t, capstone.x86.X86_OP_IMM, call_signature)
    candidate_cfg = direct_control_flow(c, capstone.x86.X86_OP_IMM, call_signature)
    print('complete direct block graphs:', len(target_cfg), len(candidate_cfg),
          'edges/per-block calls/return cleanup agree:', target_cfg == candidate_cfg)
    if len(code) == len(target):
        print('complete replay differences:', sum(a != b for a, b in zip(code, target)))
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
