#!/usr/bin/env python3
"""Independent original-PE/full-COFF audit for TH09's camera-facing boss.

ProjectCameraFacingQuad is a 622-byte source-present/non-exact owner.  This
verifies every physical original operand/call before applying the source's
independently reviewed relocation symbols and addends.  Complete near matches
are diagnostics, never accepted exactness or proof of original TU placement.
"""

from __future__ import annotations

import argparse
import hashlib
import importlib.util
import json
from pathlib import Path
import struct
import sys

ROOT = Path(__file__).resolve().parents[1]
BASE, SIZE = 0x00439820, 622
SYMBOL = '?ProjectCameraFacingQuad@AnmManager@@QAEHPAUAnmVm@@@Z'
TARGET_SHA256 = '36fd70db08a5570fe316d9e3beb541f7a39a90ef10b1429ad767e09c973f1865'
FLOAT3_CTOR = '??0Float3@@QAE@MMM@Z'
FLOAT3_FLOATPTR = '??BFloat3@@QAEPAMXZ'
FLOAT3_SUBTRACT = '??GFloat3@@QBE?AU0@ABU0@@Z'
D3DX_IDENTITY = '?D3DXMatrixIdentity@@YIPAUD3DXMATRIX@@PAU1@@Z'
D3DX_PROJECT = '?D3DXVec3Project@@YGPAUD3DXVECTOR3@@PAU1@PBU1@PBU_D3DVIEWPORT8@@PBUD3DXMATRIX@@33@Z'
D3DX_LENGTH = '?D3DXVec3Length@@YIMPBUD3DXVECTOR3@@@Z'
TRANSLATE = '?TranslateRotation@AnmManager@@QAEXPAUVertexTex1DiffuseXyzrhw@@MMMMMM@Z'
CONTEXT = '?g_AnmProjectionContext@@3PAUAnmProjectionContextView@@A'
QUAD = '?g_AnmRenderQuad@@3PAUVertexTex1DiffuseXyzrhw@@A'

# Physical field offsets, kinds, actual COFF identities and target-owned
# referents. D3DX_IDENTITY is a historical source alias to the independently
# exact matrix-identity helper at 0x401300, NOT a retail original name.
HEAD = (
    (0x02B, 'REL32', FLOAT3_CTOR, 0x4010B0, 0),
    (0x036, 'REL32', D3DX_IDENTITY, 0x401300, 0),
    (0x041, 'REL32', FLOAT3_FLOATPTR, 0x4343D0, 0),
    (0x057, 'DIR32', CONTEXT, 0x4B3448, 0),
    (0x07C, 'REL32', D3DX_PROJECT, 0x44EBB7, 0),
    (0x085, 'DIR32', '__real@00000000', 0x48E314, 0),
    (0x099, 'DIR32', '__real@3f800000', 0x48E2A4, 0),
    (0x0A9, 'DIR32', CONTEXT, 0x4B3448, 0),
    (0x0D0, 'REL32', D3DX_PROJECT, 0x44EBB7, 0),
    (0x0E3, 'REL32', FLOAT3_SUBTRACT, 0x401140, 0),
    (0x0FC, 'REL32', D3DX_LENGTH, 0x401290, 0),
    (0x102, 'DIR32', '__real@3f000000', 0x490F54, 0),
)
QUAD_FIELDS = (
    (0x149, 0), (0x165, 28), (0x183, 56), (0x1A1, 84),
    (0x1B5, 92), (0x1BA, 64), (0x1C0, 36), (0x1C6, 8),
    (0x1D8, 0), (0x1E1, 0), (0x1E7, 28), (0x1F0, 28),
    (0x1F6, 56), (0x1FF, 56), (0x205, 84), (0x20E, 84),
    (0x21F, 4), (0x228, 4), (0x22E, 32), (0x237, 32),
    (0x23D, 60), (0x246, 60), (0x24C, 88), (0x255, 88),
)
CALL_FIELDS = (
    (0x14E, 'REL32', TRANSLATE, 0x4397E0, 0),
    (0x16C, 'REL32', TRANSLATE, 0x4397E0, 0),
    (0x18A, 'REL32', TRANSLATE, 0x4397E0, 0),
    (0x1A8, 'REL32', TRANSLATE, 0x4397E0, 0),
)
FIELDS = tuple(sorted(HEAD + CALL_FIELDS + tuple(
    (offset, 'DIR32', QUAD, 0x4DC558, addend) for offset, addend in QUAD_FIELDS)))
EXPECTED_CALLS = (0x4010B0, 0x401300, 0x4343D0, 0x44EBB7, 0x44EBB7,
                  0x401140, 0x401290, 0x4397E0, 0x4397E0, 0x4397E0, 0x4397E0)


def encoded(kind: str, addr: int, offset: int, base: int = BASE) -> int:
    if kind not in ('DIR32', 'REL32'):
        raise ValueError('camera owner has unsupported address field')
    return (addr - (base + offset + 4 if kind == 'REL32' else 0)) & 0xFFFFFFFF


def validate_original(target: bytes, *, sha256: str = TARGET_SHA256, fields=FIELDS):
    if len(target) != SIZE or hashlib.sha256(target).hexdigest() != sha256:
        raise ValueError('TH09 original camera physical owner identity differs')
    if len(fields) != 40:
        raise ValueError('camera target requires all forty separately reviewed physical fields')
    previous_end = 0
    for off, kind, symbol, destination, addend in fields:
        if off < previous_end or off + 4 > SIZE or not symbol:
            raise ValueError('camera original address field overlaps or escapes owner')
        previous_end = off + 4
        if struct.unpack_from('<I', target, off)[0] != encoded(kind, destination + addend, off):
            raise ValueError('camera original operand differs from reviewed identity: ' + symbol)


def bind_candidate(raw: bytes, rows: list[dict], *, fields=FIELDS) -> bytes:
    if len(raw) < 570 or len(raw) > 710:
        raise ValueError('camera nonexact probe extent outside reviewed range')
    if len(rows) != len(fields) or len(rows) != 40:
        raise ValueError('camera source has missing or extra COFF operands')
    linked = bytearray(raw)
    previous_end = 0
    for row, (_, kind, symbol, dest, addend) in zip(rows, fields):
        off = row['offset']
        if (row['type'], row['symbol'], row['addend']) != (kind, symbol, addend):
            raise ValueError('camera candidate changed original ordered operand identity/kind/addend')
        if not isinstance(off, int) or off < previous_end or off + 4 > len(raw):
            raise ValueError('camera candidate fields reordered, overlapping or outside owner')
        previous_end = off + 4
        struct.pack_into('<I', linked, off, encoded(kind, dest + addend, off))
    return bytes(linked)


def decoded_fields(code: bytes):
    """Read absolute data references and direct-call operands, not jump immediates."""
    import capstone
    disassembler = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32)
    disassembler.detail = True
    instructions = list(disassembler.disasm(code, BASE))
    if not instructions or sum(i.size for i in instructions) != len(code):
        raise ValueError('camera owner does not decode over the complete physical extent')
    if instructions[-1].mnemonic != 'ret' or instructions[-1].op_str != '4':
        raise ValueError('camera owner does not retain observed ret 4 ABI')
    fields, calls = [], []
    for ins in instructions:
        if ins.mnemonic.startswith('j'):
            continue
        if ins.mnemonic == 'call':
            if (len(ins.operands) != 1 or ins.operands[0].type != capstone.x86.X86_OP_IMM
                    or ins.imm_size != 4):
                raise ValueError('camera owner contains unknown indirect/short call')
            calls.append(ins.operands[0].imm)
            fields.append((ins.address - BASE + ins.imm_offset, 'REL32', ins.operands[0].imm))
            continue
        for op in ins.operands:
            if op.type == capstone.x86.X86_OP_MEM and ins.disp_size == 4:
                if 0x400000 <= op.mem.disp < 0x4E7000:
                    fields.append((ins.address - BASE + ins.disp_offset, 'DIR32', op.mem.disp))
            elif op.type == capstone.x86.X86_OP_IMM and ins.imm_size == 4:
                if 0x400000 <= op.imm < 0x4E7000:
                    fields.append((ins.address - BASE + ins.imm_offset, 'DIR32', op.imm))
    return instructions, fields, calls


def diagnose(path: Path):
    import capstone
    from diagnostic_branch_alignment import direct_control_flow
    spec = importlib.util.spec_from_file_location(
        'camera_th09_coff', ROOT / 'scripts/compare-coff-function.py')
    coff = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(coff)
    target = coff.pe_bytes_at(coff.verified_target(), BASE, SIZE)
    validate_original(target)
    ti, tf, tcalls = decoded_fields(target)
    if tf != [(off, kind, value + addend) for off, kind, _, value, addend in FIELDS]:
        raise ValueError('camera original decoded fields diverge from reviewed physical manifest')
    if tuple(tcalls) != EXPECTED_CALLS or len(ti) != 201:
        raise ValueError('camera original calls/instruction inventory diverged')
    raw, rows = coff.object_function(path, SYMBOL)
    linked = bind_candidate(raw, rows)
    ci, cf, ccalls = decoded_fields(linked)
    if cf != [(row['offset'], kind, target + addend)
              for row, (_, kind, _, target, addend) in zip(rows, FIELDS)]:
        raise ValueError('camera source operands do not match its actual decoded COFF field sites')
    if ccalls != tcalls:
        raise ValueError('camera source changes original call identities/order')
    tg = direct_control_flow(ti, capstone.x86.X86_OP_IMM)
    cg = direct_control_flow(ci, capstone.x86.X86_OP_IMM)
    differences = [offset for offset, (original, candidate) in
                   enumerate(zip(target, linked)) if original != candidate]
    return {
        'diagnostic_only': True, 'exactness_credit': 'none',
        'target_bytes': SIZE, 'candidate_bytes': len(raw),
        'target_instructions': len(ti), 'candidate_instructions': len(ci),
        'original_fields': len(tf), 'bound_fields': len(rows),
        'ordered_calls': len(tcalls), 'calls_agree': True,
        'original_blocks': len(tg), 'candidate_blocks': len(cg),
        'direct_graphs_agree': tg == cg,
        'overlap_differences': len(differences),
        'difference_offsets': differences,
        'extra_candidate_bytes': max(0, len(linked) - SIZE),
        'missing_target_bytes': max(0, SIZE - len(linked)),
        'complete_bytes_equal': target == linked,
        'candidate_raw_sha256': hashlib.sha256(raw).hexdigest(),
        'target_sha256': TARGET_SHA256,
    }


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('object', type=Path)
    args = parser.parse_args()
    try:
        print(json.dumps(diagnose(args.object), indent=2))
        return 0
    except (OSError, ValueError, KeyError, ImportError, struct.error) as error:
        print('Camera-facing owner rejected:', error, file=sys.stderr)
        return 2


if __name__ == '__main__':
    raise SystemExit(main())
