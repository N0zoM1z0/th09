#!/usr/bin/env python3
"""TH09 v1.50a TitleDrawCallback: complete non-crediting source/PE oracle.

Checks the full target 329-byte function, independently reviewed callee/data
identities, twenty relocation fields, decoded instructions, direct call/branch
block graphs and the current single-DEC source gap. No partial exact credit.
"""

import argparse
import difflib
import importlib.util
import json
from pathlib import Path
import struct

import capstone
from diagnostic_branch_alignment import direct_control_flow

ROOT = Path(__file__).resolve().parents[1]
BASE = 0x00424898
SIZE = 329
SYMBOL = '?TitleDrawCallback@@YIHPAX@Z'
GAP_OFFSET = 0x101

# Target absolute field positions and independent addresses: exact TH09
# callees/data, plus four source-present Title draw helper boundaries.
# This is intentionally not extracted from matching target field values.
FIELDS = [
    (0x00D, 'DIR32', '?g_Supervisor@@3VSupervisor@@A', 0x004B3100),
    (0x012, 'REL32', '?ConfigureGameplayViewport@Supervisor@@QAEXH@Z', 0x00401390),
    (0x025, 'DIR32', '?g_AnmManager@@3PAVAnmManager@@A', 0x004DC550),
    (0x035, 'DIR32', '?g_AnmManager@@3PAVAnmManager@@A', 0x004DC550),
    (0x03B, 'REL32', '?CopySurfaceToBackbuffer@AnmManager@@QAEXHHHHH@Z', 0x0043BFA0),
    (0x065, 'DIR32', '?g_AnmManager@@3PAVAnmManager@@A', 0x004DC550),
    (0x06A, 'REL32', '?SpriteHasTexture@AnmManager@@QAEHPAUAnmVm@@@Z', 0x00423469),
    (0x088, 'REL32', '??YFloat3@@QAEAAU0@ABU0@@Z', 0x00405730),
    (0x08E, 'DIR32', '?g_AnmManager@@3PAVAnmManager@@A', 0x004DC550),
    (0x094, 'DIR32', '__real@00000000', 0x0048E314),
    (0x0AB, 'REL32', '?Draw2D@AnmManager@@QAEHPAUAnmVm@@@Z', 0x0043AB50),
    (0x0B2, 'REL32', '?DrawNoRotation@AnmManager@@QAEHPAUAnmVm@@@Z', 0x0043A950),
    (0x0E4, 'DIR32', '?g_AnmManager@@3PAVAnmManager@@A', 0x004DC550),
    (0x0EA, 'REL32', '?DrawNoRotation@AnmManager@@QAEHPAUAnmVm@@@Z', 0x0043A950),
    (0x107, 'REL32', '?DrawReplayMenu@TitleDrawView@@QAEHXZ', 0x004234F6),
    (0x121, 'REL32', '?DrawReplaySave@TitleDrawView@@QAEHXZ', 0x004239F6),
    (0x12A, 'REL32', '?DrawResult@TitleDrawView@@QAEHXZ', 0x00423D16),
    (0x133, 'REL32', '?DrawMusicRoom@TitleDrawView@@QAEHXZ', 0x004244A9),
    (0x139, 'DIR32', '?g_AnmManager@@3PAVAnmManager@@A', 0x004DC550),
    (0x13E, 'REL32', '?FlushVertexBuffer@AnmManager@@QAEXXZ', 0x004396A0),
]


def expected_candidate_offset(target_offset, has_gap):
    if target_offset >= SIZE:
        raise ValueError('reviewed field is outside the target extent')
    if has_gap and target_offset > GAP_OFFSET:
        return target_offset - 1
    if has_gap and target_offset == GAP_OFFSET:
        raise ValueError('field overlaps the reviewed missing DEC')
    return target_offset


def load_coff():
    spec = importlib.util.spec_from_file_location(
        'title_draw_coff', ROOT / 'scripts/compare-coff-function.py')
    mod = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(mod)
    return mod


def decoded(data):
    md = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32)
    md.detail = True
    instructions = list(md.disasm(data, BASE))
    if sum(ins.size for ins in instructions) != len(data) or not instructions:
        raise ValueError('function extent not completely decoded')
    return instructions


def normalized(instructions):
    values = []
    for ins in instructions:
        data = bytearray(ins.bytes)
        if (ins.mnemonic.startswith('j') or ins.mnemonic == 'call') and ins.imm_size:
            data[ins.imm_offset:ins.imm_offset + ins.imm_size] = bytes(ins.imm_size)
        values.append((ins.address - BASE, ins.mnemonic, bytes(data)))
    return values


def diagnose(object_path):
    coff = load_coff()
    verified = coff.verified_target()
    target = coff.pe_bytes_at(verified, BASE, SIZE)
    raw, relocations = coff.object_function(object_path, SYMBOL)
    if len(raw) not in (SIZE - 1, SIZE):
        raise ValueError('unreviewed owner extent')
    has_gap = len(raw) == SIZE - 1
    if has_gap and target[GAP_OFFSET] != 0x48:
        raise ValueError('target DEC EAX at gap has changed')
    if coff.pe_bytes_at(verified, 0x0048E314, 4) != bytes(4):
        raise ValueError('target float zero literal changed')
    if len(relocations) != len(FIELDS):
        raise ValueError('unreviewed relocation count')

    linked = bytearray(raw)
    zero_target = bytearray(target)
    zero_candidate = bytearray(raw)
    for row, expected in zip(relocations, FIELDS):
        offset, kind, symbol, dest = expected
        candidate_offset = expected_candidate_offset(offset, has_gap)
        if (row['offset'], row['type'], row['symbol'], row['addend']) != (candidate_offset, kind, symbol, 0):
            raise ValueError('COFF field identity/offset mismatch: ' + repr(row))
        value = dest
        target_value = dest
        if kind == 'REL32':
            value -= BASE + candidate_offset + 4
            target_value -= BASE + offset + 4
        value &= 0xFFFFFFFF
        target_value &= 0xFFFFFFFF
        if struct.unpack_from('<I', target, offset)[0] != target_value:
            raise ValueError('reviewed destination does not match target field: ' + symbol)
        struct.pack_into('<I', linked, candidate_offset, value)
        zero_candidate[candidate_offset:candidate_offset+4] = bytes(4)
        zero_target[offset:offset+4] = bytes(4)

    t_inst = decoded(target)
    c_inst = decoded(linked)
    t_graph = direct_control_flow(t_inst, capstone.x86.X86_OP_IMM)
    c_graph = direct_control_flow(c_inst, capstone.x86.X86_OP_IMM)
    a = normalized(decoded(zero_target))
    b = normalized(decoded(zero_candidate))
    changes = []
    for kind, ta, tb, ca, cb in difflib.SequenceMatcher(
        None, [(x[1], x[2]) for x in a], [(x[1], x[2]) for x in b],
        autojunk=False).get_opcodes():
        if kind != 'equal':
            changes.append({
                'kind': kind,
                'target': [(hex(o), name, rawbytes.hex()) for o, name, rawbytes in a[ta:tb]],
                'candidate': [(hex(o), name, rawbytes.hex()) for o, name, rawbytes in b[ca:cb]],
            })
    one_dec_gap = changes == [{
        'kind': 'delete',
        'target': [(hex(GAP_OFFSET), 'dec', '48')],
        'candidate': [],
    }]
    return {
        'diagnostic_only': True,
        'target_bytes': SIZE,
        'candidate_bytes': len(linked),
        'bound_relocations': len(relocations),
        'target_instructions': len(t_inst),
        'candidate_instructions': len(c_inst),
        'target_calls': sum(i.mnemonic == 'call' for i in t_inst),
        'candidate_calls': sum(i.mnemonic == 'call' for i in c_inst),
        'complete_direct_graph_equal': t_graph == c_graph,
        'target_blocks': len(t_graph),
        'candidate_blocks': len(c_graph),
        'single_reviewed_dec_gap': one_dec_gap,
        'normalized_instruction_differences': changes,
        'complete_bytes_equal': linked == target,
        'credit': 'none; canonical exact requires a cold equal-size full-owner replay',
    }


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('object', type=Path)
    parser.add_argument('--json', action='store_true')
    args = parser.parse_args()
    report = diagnose(args.object)
    if args.json:
        print(json.dumps(report, indent=2))
    else:
        print('NON-CREDITING TitleDrawCallback complete target comparison')
        for key in ('target_bytes', 'candidate_bytes', 'bound_relocations',
                    'target_instructions', 'candidate_instructions',
                    'target_calls', 'candidate_calls', 'target_blocks',
                    'candidate_blocks', 'complete_direct_graph_equal',
                    'single_reviewed_dec_gap', 'complete_bytes_equal'):
            print(key + ':', report[key])
        print('normalized differences:', report['normalized_instruction_differences'])


if __name__ == '__main__':
    main()
