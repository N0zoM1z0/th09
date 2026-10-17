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


def normalized_owner_shape(instructions, fields, *, base: int = BASE):
    """Keep actual offsets, mask only audited operands and internal jump displacements.

    The caller must first independently bind/verify every COFF field and decode
    the complete owner. A normalized shape is a *diagnostic*, not exactness.
    """
    starts = {ins.address for ins in instructions}
    if len(starts) != len(instructions):
        raise ValueError('duplicate Type14/22 instruction boundary')
    field_list = sorted((int(offset), kind, dest) for offset, kind, dest in fields)
    if any(left.address + len(left.bytes) != right.address for left, right in zip(instructions, instructions[1:])):
        raise ValueError('noncontiguous Type14/22 instruction stream')
    field_index = 0
    previous_field_end = -1
    jumps = []
    normalized = []
    for ins in instructions:
        start = ins.address - base
        end = start + len(ins.bytes)
        data = bytearray(ins.bytes)
        while field_index < len(field_list) and field_list[field_index][0] < end:
            offset, kind, _ = field_list[field_index]
            if offset < previous_field_end:
                raise ValueError('Type14/22 overlapping reviewed fields')
            if offset < start or offset + 4 > end:
                raise ValueError('Type14/22 reviewed field crosses an instruction')
            if kind not in ('REL32', 'DIR32'):
                raise ValueError('Type14/22 unknown reviewed field kind')
            data[offset-start:offset-start+4] = bytes(4)
            previous_field_end = offset + 4
            field_index += 1
        if ins.mnemonic.startswith('j'):
            if not ins.op_str.startswith('0x'):
                raise ValueError('Type14/22 indirect or unknown jump destination')
            try:
                destination = int(ins.op_str, 16)
            except ValueError as error:
                raise ValueError('Type14/22 unparseable jump destination') from error
            if destination not in starts:
                raise ValueError('Type14/22 jump does not reach an instruction boundary')
            if ins.imm_size not in (1, 4) or not 1 <= ins.imm_offset <= len(data)-ins.imm_size:
                raise ValueError('Type14/22 unsupported jump displacement')
            actual_displacement = int.from_bytes(ins.bytes[ins.imm_offset:ins.imm_offset+ins.imm_size], 'little', signed=True)
            if ins.address + len(ins.bytes) + actual_displacement != destination:
                raise ValueError('Type14/22 decoded jump and encoded displacement disagree')
            data[ins.imm_offset:ins.imm_offset+ins.imm_size] = bytes(ins.imm_size)
            jumps.append((start, destination - base, ins.mnemonic))
        normalized.append((start, bytes(data)))
    if field_index != len(field_list):
        raise ValueError('Type14/22 reviewed field outside decoded instruction stream')
    return normalized, jumps


def aligned_shape_changes(candidate_stream, target_stream):
    """Pair only exact normalized opcode encodings, retaining unmatched spans."""
    spec = importlib.util.spec_from_file_location(
        'exattack1422_aligned_spans', ROOT / 'scripts/ecl_shape_differences.py')
    if spec is None or spec.loader is None:
        raise ValueError('aligned instruction diff is unavailable')
    module = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(module)
    return module.changed_instructions(candidate_stream, target_stream)


def inspect(path: Path, *, explain: bool = False) -> dict:
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
    target_stream, target_jumps = normalized_owner_shape(t_ins, t_fields)
    candidate_stream, candidate_jumps = normalized_owner_shape(c_ins, c_fields)
    differences = [i for i, (x, y) in enumerate(zip(target, candidate)) if x != y]
    result = {
        'diagnostic_only': True, 'exactness_credit': 'none',
        'target_bytes': SIZE, 'candidate_bytes': len(candidate),
        'target_instructions': len(t_ins), 'candidate_instructions': len(c_ins),
        'target_fields': len(t_fields), 'candidate_fields': len(c_fields),
        'direct_calls': len(c_calls), 'indirect_calls': c_ind,
        'target_direct_jumps': len(target_jumps),
        'candidate_direct_jumps': len(candidate_jumps),
        'field_order_and_destinations_equal': True,
        'ordered_direct_calls_equal': True,
        'overlapping_differences': len(differences),
        'absent_target_bytes': max(0, SIZE-len(candidate)),
        'extra_candidate_bytes': max(0, len(candidate)-SIZE),
        'first_differences': [hex(x) for x in differences[:24]],
        # Never hide the other original-bound differing code bytes from the
        # boss gate. Exact prior byte positions need preservation even when
        # the candidate is longer than the original owner.
        'difference_offsets': differences,
        'complete_bytes_equal': candidate == target,
        'target_sha256': hashlib.sha256(target).hexdigest(),
        'raw_sha256': hashlib.sha256(raw).hexdigest(),
        'resolved_sha256': hashlib.sha256(candidate).hexdigest(),
    }
    if explain:
        changes = aligned_shape_changes(candidate_stream, target_stream)
        result['normalized_aligned_changes'] = changes
        result['normalized_aligned_change_spans'] = len(changes)
    return result


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('object', type=Path)
    parser.add_argument('--json', action='store_true')
    parser.add_argument('--explain', action='store_true',
                        help='show independently bound, aligned instruction differences')
    parser.add_argument('--max-spans', type=int, default=12,
                        help='maximum aligned difference spans in text output (default: 12)')
    args = parser.parse_args()
    if args.max_spans < 1:
        parser.error('--max-spans must be positive')
    try:
        result = inspect(args.object, explain=args.explain)
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
        if args.explain:
            changes = result['normalized_aligned_changes']
            print('Direct jumps target/candidate: %d/%d; aligned spans: %d (diagnostic only)' % (
                result['target_direct_jumps'], result['candidate_direct_jumps'], len(changes)))
            print('Only independently bound four-byte fields and verified internal jump displacements were zeroed.')
            for row in changes[:args.max_spans]:
                print(' ', row['change'])
                for offset, data in row['target']:
                    print('    target    +0x%04X: %s' % (offset, data))
                for offset, data in row['candidate']:
                    print('    candidate +0x%04X: %s' % (offset, data))
            if len(changes) > args.max_spans:
                print('... %d further mismatched spans not displayed' % (len(changes)-args.max_spans))
    return 0 if result['complete_bytes_equal'] else 1


if __name__ == '__main__':
    raise SystemExit(main())
