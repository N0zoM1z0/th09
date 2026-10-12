#!/usr/bin/env python3
"""Complete, noncrediting TH09 ExAttack14/22 Draw PE/COFF codegen audit.

The 959-byte original function is hash-attested independently of the current
candidate. All 53 original address operands and 20 ordered direct/12 indirect
calls are decoded rather than borrowed from guessed candidate identities.
Only reviewed TH09 function/global/literal targets may bind COFF relocation
symbols. The diagnostic never suppresses differing operands or grants credit.
"""

from __future__ import annotations

import argparse
import hashlib
import importlib.util
import json
from pathlib import Path
import struct

ROOT = Path(__file__).resolve().parents[1]
BASE = 0x00444130
SIZE = 959
TARGET_OWNER_SHA256 = 'af3c06ea2c9f68861067f5feb784206035dd67ea3eb07ef3994ed4ec17df74f4'
SYMBOL = '?ExAttackDrawCallbackType14_22@@YIHPAUExAttackRecord@@@Z'

# Address identities come from the verified TH09 target instructions and the
# already-reviewed callee/data owners. This is a name-to-identity allowlist,
# not a mechanism for solving addresses from the candidate's byte stream.
DESTINATIONS = {
    '?AddNormalizeAngle@@YIMMM@Z': 0x0042AED0,
    '??BZunTimer@@QAEHXZ': 0x00435F00,
    '?g_GameManager@@3UAsciiGameManagerView@@A': 0x004A7D90,
    '?TransformPopupX@AsciiGameManagerView@@QAEMM@Z': 0x00401680,
    '?TransformPopupY@AsciiGameManagerView@@QAEMM@Z': 0x004016B0,
    '__real@40490fdb': 0x0048E2C4,
    '__real@40c90fdb': 0x0048E4C4,
    '?FromAngleMagnitude@Float3@@QAEXMM@Z': 0x00441890,
    '??YFloat3@@QAEAAU0@ABU0@@Z': 0x00405730,
    '__real@3e4f8c3d': 0x00491608,
    '?g_AnmManager@@3PAVAnmManager@@A': 0x004DC550,
    '?DrawTriangleFan@AnmManager@@QAEHPAUAnmVm@@PAUVertexDiffuseXyzrhw@@H@Z': 0x0043B7B0,
    '?FlushVertexBuffer@AnmManager@@QAEXXZ': 0x004396A0,
    '?g_Supervisor@@3UExAttackType1422DrawSupervisorView@@A': 0x004B3100,
    '__real@3d4f8c3d': 0x00491604,
    '?ClearVertexShader@AnmManager@@QAEXXZ': 0x0040E380,
    '?ClearSprite@AnmManager@@QAEXXZ': 0x0040E390,
    '?ClearTexture@AnmManager@@QAEXXZ': 0x004015F0,
    '?ClearColorOp@AnmManager@@QAEXXZ': 0x00415D00,
    '?ClearBlendMode@AnmManager@@QAEXXZ': 0x0040E370,
    '?ClearZWrite@AnmManager@@QAEXXZ': 0x00415D10,
}


def bind_candidate(raw: bytes, rows: list[dict]) -> tuple[bytes, list[tuple]]:
    """Bind only independently identified real TH09 relocations."""
    if len(rows) != 53:
        raise ValueError('Type14/22 draw requires all 53 actual COFF fields')
    linked = bytearray(raw)
    used = set()
    bound = []
    for row in rows:
        symbol, kind = row['symbol'], row['type']
        offset, addend = int(row['offset']), int(row['addend'])
        if symbol not in DESTINATIONS or kind not in ('REL32', 'DIR32'):
            raise ValueError('unreviewed ExAttack draw relocation symbol/kind')
        is_code_symbol = symbol.startswith('?') and not symbol.startswith('?g_') and not symbol.startswith('??')
        is_code_symbol |= symbol.startswith('??Y') or symbol.startswith('??B')
        if kind != ('REL32' if is_code_symbol else 'DIR32'):
            raise ValueError('ExAttack draw call/data relocation identity altered')
        if addend != (8 if symbol.startswith('?g_Supervisor@@3UExAttackType1422') else 0):
            raise ValueError('unreviewed ExAttack draw field addend')
        covered = set(range(offset, offset + 4))
        if offset < 0 or offset + 4 > len(linked) or used.intersection(covered):
            raise ValueError('ExAttack draw COFF field outside owner extent or overlapping')
        used.update(covered)
        dest = DESTINATIONS[symbol] + addend
        value = dest - (BASE + offset + 4 if kind == 'REL32' else 0)
        struct.pack_into('<I', linked, offset, value & 0xffffffff)
        bound.append((offset, kind, dest))
    return bytes(linked), sorted(bound)


def decode_owner(data: bytes):
    import capstone

    cs = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32)
    cs.detail = True
    inst = list(cs.disasm(data, BASE))
    if not inst or sum(x.size for x in inst) != len(data) or inst[-1].mnemonic != 'ret':
        raise ValueError('ExAttack draw instruction decoding/return is incomplete')
    if inst[-1].op_str:
        raise ValueError('ExAttack draw fastcall callee cleanup unexpected')
    fields, calls, indirect = [], [], 0
    for x in inst:
        for op in x.operands:
            if op.type == capstone.x86.X86_OP_MEM and 0x400000 <= op.mem.disp < 0x4E8000:
                if x.disp_size != 4:
                    raise ValueError('unsupported ExAttack draw absolute memory operand')
                fields.append((x.address - BASE + x.disp_offset, 'DIR32', op.mem.disp))
            elif x.mnemonic == 'call' and op.type == capstone.x86.X86_OP_IMM:
                if x.imm_size != 4:
                    raise ValueError('unsupported ExAttack draw direct call operand')
                fields.append((x.address - BASE + x.imm_offset, 'REL32', op.imm))
                calls.append(op.imm)
            elif x.mnemonic != 'call' and not x.mnemonic.startswith('j') and op.type == capstone.x86.X86_OP_IMM and 0x400000 <= op.imm < 0x4E8000:
                if x.imm_size != 4:
                    raise ValueError('unsupported ExAttack draw global/literal immediate')
                fields.append((x.address - BASE + x.imm_offset, 'DIR32', op.imm))
        if x.mnemonic == 'call' and x.operands[0].type != capstone.x86.X86_OP_IMM:
            indirect += 1
    return inst, sorted(fields), calls, indirect


def inspect(path: Path) -> dict:
    spec = importlib.util.spec_from_file_location('exattack14_coff', ROOT/'scripts/compare-coff-function.py')
    if spec is None or spec.loader is None:
        raise ValueError('target-bound COFF parser unavailable')
    coff = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(coff)
    target = coff.pe_bytes_at(coff.verified_target(), BASE, SIZE)
    if hashlib.sha256(target).hexdigest() != TARGET_OWNER_SHA256:
        raise ValueError('verified original Type14/22 draw owner identity drifted')
    t_ins, t_fields, t_calls, t_ind = decode_owner(target)
    if len(t_ins) != 290 or len(t_fields) != 53 or len(t_calls) != 20 or t_ind != 12:
        raise ValueError('original TH09 Type14/22 physical/call/field inventory drifted')
    for symbol, dest in DESTINATIONS.items():
        if symbol.startswith('__real@'):
            expected = struct.pack('<I', int(symbol[7:], 16))
            if coff.pe_bytes_at(coff.verified_target(), dest, 4) != expected:
                raise ValueError('ExAttack draw literal does not match actual target float')
    raw, rows = coff.object_function(path, SYMBOL)
    candidate, candidate_fields = bind_candidate(raw, rows)
    c_ins, c_fields, c_calls, c_ind = decode_owner(candidate)
    if c_fields != candidate_fields:
        raise ValueError('ExAttack draw COFF fields do not cover decoded code operands')
    # Preserve order, multiplicity and distinct call ownership, not just set equality.
    if [(kind, dest) for _, kind, dest in t_fields] != [(kind, dest) for _, kind, dest in c_fields]:
        raise ValueError('ExAttack draw candidate call/data field order or destinations differ')
    if c_calls != t_calls or c_ind != t_ind:
        raise ValueError('ExAttack draw candidate direct/indirect call sequence changed')
    differences = [i for i, (x, y) in enumerate(zip(target, candidate)) if x != y]
    return {
        'diagnostic_only': True, 'exactness_credit': 'none',
        'target_bytes': SIZE, 'candidate_bytes': len(candidate),
        'target_instructions': len(t_ins), 'candidate_instructions': len(c_ins),
        'target_fields': len(t_fields), 'candidate_fields': len(c_fields),
        'direct_calls': len(c_calls), 'indirect_calls': c_ind,
        'field_order_and_destinations_equal': True,
        'ordered_direct_calls_equal': True,
        'overlapping_differences': len(differences),
        'absent_target_bytes': max(0, SIZE-len(candidate)),
        'extra_candidate_bytes': max(0, len(candidate)-SIZE),
        'first_differences': [hex(x) for x in differences[:24]],
        'complete_bytes_equal': candidate == target,
        'target_sha256': hashlib.sha256(target).hexdigest(),
        'raw_sha256': hashlib.sha256(raw).hexdigest(),
        'resolved_sha256': hashlib.sha256(candidate).hexdigest(),
    }


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('object', type=Path)
    parser.add_argument('--json', action='store_true')
    args = parser.parse_args()
    try:
        result = inspect(args.object)
    except (OSError, ValueError, ImportError, KeyError, struct.error) as error:
        print(json.dumps({'error':str(error)}))
        return 2
    if args.json:
        print(json.dumps(result, indent=2))
    else:
        print('Type14/22 draw: %d/%d physical bytes, %d independently bound fields, %d direct + %d indirect calls, %d differences + %d extra bytes, exact=%s (NON-CREDITING)' % (
            result['candidate_bytes'], result['target_bytes'],result['candidate_fields'],
            result['direct_calls'],result['indirect_calls'],result['overlapping_differences'],
            result['extra_candidate_bytes'],result['complete_bytes_equal']))
    return 0 if result['complete_bytes_equal'] else 1


if __name__ == '__main__':
    raise SystemExit(main())
