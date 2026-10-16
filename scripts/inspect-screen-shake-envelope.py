#!/usr/bin/env python3
"""TH09 v1.50a CalcShakeEnvelope complete, non-crediting target/COFF oracle.

The 336-byte original uses 20 independently reviewed code-address fields.
Targets below come from existing exact TH09 Timer/RNG routines, known PE data,
and the source-present ANM/GameManager globals, NOT from fitting candidate
bytes. No equality percentage, partial match or accepted exact credit is given.
The caller must separately attest that a COFF object came from current source.
"""

import argparse
import importlib.util
import json
from pathlib import Path
import struct

ROOT = Path(__file__).resolve().parents[1]
BASE = 0x004227A0
SIZE = 336
SYMBOL = '?CalcShakeEnvelope@ScreenEffect@@SIHPAU1@@Z'

# Actual target instruction-field offsets (not inferred by matching the
# candidate). Each tuple contains offset, COFF kind, symbol, base destination.
# Addends come from the actual corresponding COFF field and are reviewed below.
FIELDS = [
    (0x006, 'DIR32', '?g_GameManager@@3UScreenEffectGameManagerView@@A', 0x004A7D90, 0x348),
    (0x021, 'REL32', '??EZunTimer@@QAEXH@Z', 0x00401510, 0),
    (0x02F, 'REL32', '??MZunTimer@@QAEIH@Z', 0x00401540, 0),
    (0x03A, 'REL32', '??BZunTimer@@QAEMXZ', 0x004014F0, 0),
    (0x04A, 'REL32', '??MZunTimer@@QAEIH@Z', 0x00401540, 0),
    (0x054, 'DIR32', '__real@3f800000', 0x0048E2A4, 0),
    (0x068, 'REL32', '??MZunTimer@@QAEIH@Z', 0x00401540, 0),
    (0x080, 'DIR32', '__real@4f800000', 0x0048E2A8, 0),
    (0x08A, 'REL32', '??BZunTimer@@QAEMXZ', 0x004014F0, 0),
    (0x09D, 'DIR32', '__real@4f800000', 0x0048E2A8, 0),
    (0x0A9, 'DIR32', '?g_ReplayRng@@3URngRuntimeView@@A', 0x004ACE0C, 0),
    (0x0B5, 'REL32', '?GetRandomU32InRange@RngRuntimeView@@QAEII@Z', 0x004048E0, 0),
    (0x0C9, 'DIR32', '?g_AnmManager@@3PAVAnmManager@@A', 0x004DC550, 0),
    (0x0D9, 'DIR32', '?g_AnmManager@@3PAVAnmManager@@A', 0x004DC550, 0),
    (0x0E4, 'DIR32', '?g_AnmManager@@3PAVAnmManager@@A', 0x004DC550, 0),
    (0x0EE, 'DIR32', '?g_ReplayRng@@3URngRuntimeView@@A', 0x004ACE0C, 0),
    (0x0F3, 'REL32', '?GetRandomU32InRange@RngRuntimeView@@QAEII@Z', 0x004048E0, 0),
    (0x105, 'DIR32', '?g_AnmManager@@3PAVAnmManager@@A', 0x004DC550, 0),
    (0x11F, 'DIR32', '?g_AnmManager@@3PAVAnmManager@@A', 0x004DC550, 0),
    (0x134, 'DIR32', '?g_AnmManager@@3PAVAnmManager@@A', 0x004DC550, 0),
]


def validate_manifest():
    if len(FIELDS) != 20 or sorted(FIELDS) != FIELDS:
        raise ValueError('unreviewed field count or order')
    occupied = set()
    for offset, kind, symbol, address, addend in FIELDS:
        chunk = set(range(offset, offset + 4))
        if (offset < 0 or offset + 4 > SIZE or occupied.intersection(chunk)
                or kind not in ('DIR32', 'REL32') or not symbol
                or not 0x400000 <= address <= 0x500000):
            raise ValueError('invalid/overlapping reviewed field')
        occupied.update(chunk)
        if symbol.startswith('__real@') and addend:
            raise ValueError('float constant has nonzero addend')


def require_candidate(rows, size):
    if size < 300 or size > 400:
        raise ValueError('unreviewed candidate function size')
    if len(rows) != len(FIELDS):
        raise ValueError('unreviewed COFF relocation count')
    occupied = set()
    for row, expected in zip(rows, FIELDS):
        offset, kind, symbol, address, addend = expected
        candidate_offset = int(row['offset'])
        chunk = set(range(candidate_offset, candidate_offset + 4))
        if candidate_offset < 0 or candidate_offset + 4 > size or occupied.intersection(chunk):
            raise ValueError('candidate relocation outside owner or overlapping')
        occupied.update(chunk)
        if (row['type'], row['symbol'], int(row['addend'])) != (kind, symbol, addend):
            raise ValueError('candidate field type/name/addend changed: ' + repr(row))


def target_encoding(base, field_offset, kind, destination, addend):
    address = destination + addend
    if kind == 'REL32':
        address -= base + field_offset + 4
    return address & 0xFFFFFFFF


def decode_complete(code, capstone, base=BASE):
    md = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32)
    md.detail = True
    inst = list(md.disasm(code, base))
    if not inst or sum(x.size for x in inst) != len(code):
        raise ValueError('incomplete target/candidate instruction decoding')
    return inst


def normalized_instructions(instructions, field_offsets):
    """Mask only proven 4-byte COFF fields and direct-branch displacements.

    The callsites are included in the reviewed field mask; unknown scalar
    immediates and register encodings remain fully visible. Never use the
    normalized stream as accepted exactness or as a source for addresses.
    """
    fields = set(field_offsets)
    consumed = set()
    result = []
    for ins in instructions:
        relative = ins.address - BASE
        data = bytearray(ins.bytes)
        for offset in fields:
            if relative <= offset < relative + ins.size:
                start = offset - relative
                if start + 4 > ins.size:
                    raise ValueError('reviewed field crosses instruction boundary')
                data[start:start + 4] = bytes(4)
                consumed.add(offset)
        if ins.mnemonic.startswith('j') and ins.imm_size:
            data[ins.imm_offset:ins.imm_offset + ins.imm_size] = bytes(ins.imm_size)
        result.append((relative, bytes(data)))
    if consumed != fields:
        raise ValueError('reviewed field does not correspond to decoded instruction')
    return result


def diagnose(path):
    import capstone
    from diagnostic_branch_alignment import direct_control_flow
    from ecl_shape_differences import changed_instructions

    validate_manifest()
    spec = importlib.util.spec_from_file_location('th09_screen_shake_coff', ROOT / 'scripts/compare-coff-function.py')
    coff = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(coff)
    pe = coff.verified_target()
    target = coff.pe_bytes_at(pe, BASE, SIZE)
    original, rows = coff.object_function(path, SYMBOL)
    require_candidate(rows, len(original))
    linked = bytearray(original)
    for actual, reviewed in zip(rows, FIELDS):
        offset, kind, symbol, destination, addend = reviewed
        expected_target = target_encoding(BASE, offset, kind, destination, addend)
        if struct.unpack_from('<I', target, offset)[0] != expected_target:
            raise ValueError('independent target-field identity disagrees: ' + symbol)
        value = target_encoding(BASE, int(actual['offset']), kind, destination, addend)
        struct.pack_into('<I', linked, int(actual['offset']), value)
    for symbol, destination in [('__real@3f800000', 0x0048E2A4),
                                ('__real@4f800000', 0x0048E2A8)]:
        if coff.pe_bytes_at(pe, destination, 4) != struct.pack('<I', int(symbol[7:], 16)):
            raise ValueError('reviewed target float payload differs: ' + symbol)

    target_ins = decode_complete(target, capstone)
    candidate_ins = decode_complete(linked, capstone)
    target_graph = direct_control_flow(target_ins, capstone.x86.X86_OP_IMM)
    candidate_graph = direct_control_flow(candidate_ins, capstone.x86.X86_OP_IMM)
    def calls(ins):
        return tuple(i.operands[0].imm for i in ins if i.mnemonic == 'call')
    dis = [i for i, (x, y) in enumerate(zip(target, linked)) if x != y]
    target_normalized = normalized_instructions(target_ins, (x[0] for x in FIELDS))
    candidate_normalized = normalized_instructions(candidate_ins, (x['offset'] for x in rows))
    changes = changed_instructions(candidate_normalized, target_normalized)
    return dict(
        diagnostic_only=True,
        exactness_credit='none; canonical exact requires strict equal full-owner cold replay',
        target_bytes=len(target), candidate_bytes=len(linked),
        bound_relocations=len(rows), reviewed_target_fields=len(FIELDS),
        target_instructions=len(target_ins), candidate_instructions=len(candidate_ins),
        target_calls=len(calls(target_ins)), candidate_calls=len(calls(candidate_ins)),
        ordered_calls_agree=calls(target_ins) == calls(candidate_ins),
        target_blocks=len(target_graph), candidate_blocks=len(candidate_graph),
        complete_direct_graphs_agree=target_graph == candidate_graph,
        overlapping_differences=len(dis), absent_target_bytes=max(0, SIZE-len(linked)),
        extra_candidate_bytes=max(0, len(linked)-SIZE),
        complete_bytes_agree=linked == target,
        normalized_instruction_equal=not changes,
        differing_normalized_spans=len(changes),
        first_normalized_spans=changes[:8],
        first_difference_offsets=[f'0x{i:03x}' for i in dis[:30]],
    )


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('object', type=Path)
    parser.add_argument('--json', action='store_true')
    args = parser.parse_args()
    result = diagnose(args.object)
    if args.json:
        print(json.dumps(result, indent=2))
    else:
        print('NON-CREDITING ScreenEffect::CalcShakeEnvelope complete target/COFF audit')
        for field in ('target_bytes', 'candidate_bytes', 'bound_relocations',
                      'target_instructions', 'candidate_instructions',
                      'target_calls', 'candidate_calls', 'ordered_calls_agree',
                      'target_blocks', 'candidate_blocks', 'complete_direct_graphs_agree',
                      'overlapping_differences', 'absent_target_bytes',
                      'extra_candidate_bytes', 'complete_bytes_agree',
                      'normalized_instruction_equal', 'differing_normalized_spans'):
            print(field+':', result[field])


if __name__ == '__main__':
    main()
