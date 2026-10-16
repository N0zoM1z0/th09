#!/usr/bin/env python3
"""Complete, non-crediting MoveRandomBiased original-PE/COFF diagnostic.

Attests the complete 516-byte TH09 target and individually checks its 28 real
address operands against original code, exact/locally reviewed callee identities
and original literal payloads.  Binds candidate relocations from actual COFF,
not by patching or aligning bytes against the target.  An unequal owner is
NON-EXACT even if its call sequence/CFG is equal or only a short span differs.
The caller must compile the CURRENT EclManager TU before inspecting its object.
"""

from __future__ import annotations

import argparse
import hashlib
import importlib.util
import json
from pathlib import Path
import struct

ROOT = Path(__file__).resolve().parents[1]
BASE = 0x00407E30
SIZE = 516
TARGET_SHA256 = 'c7d16fba9a2e0759836560b3977fa5c277966929a33d6183a2bb8672ba5319ab'
SYMBOL = '?MoveRandomBiased@Th09EclRunLate@@YIXPAUEnemyView@@PAUTh09EclRawInstructionHeaderView@@@Z'

# Addresses and exact operand offsets read independently from the original
# Japanese PE: ECL helper/callee boundaries, the RNG global, and literal
# payloads. These identities are NOT found by fitting the candidate COFF.
REFERENTS = {
    '?g_Rng@@3URngRuntimeView@@A': 0x004ACE0C,
    '?GetRandomU32InRange@RngRuntimeView@@QAEII@Z': 0x004048E0,
    '?GetRandomF32InRange@RngRuntimeView@@QAEMM@Z': 0x00404910,
    '?GetRandomF32SignedInRange@RngRuntimeView@@QAEMM@Z': 0x00406200,
    '?AddNormalizeAngle@Th09EclRunControl@@YGMMM@Z': 0x0042AED0,
    '??BFloat3@@QAEPAMXZ': 0x004343D0,
    '?ResolveInt@Th09EclRunControl@@YIHPAUEnemyView@@H@Z': 0x00405760,
    '?ResolveFloat@EnemyView@@QAEMM@Z': 0x004068A0,
    '??4Th09EclTimerStorageView@@QAEXH@Z': 0x00401500,
    '?StartTimedPolarDisplacement@Th09EclRunMovement@@YIXPAUEnemyView@@PAUTh09EclRawInstructionHeaderView@@M@Z': 0x00407200,
    '__real@43c00000': 0x0048E45C,
    '__real@4016cbe4': 0x0048E458,
    '__real@3f490fdb': 0x0048E454,
    '__real@42400000': 0x0048E3B0,
    '__real@00000000': 0x0048E314,
}

# Original operand positions, established by decoding the TARGET physical owner.
# Target position is never used to move/solve the candidate's actual field.
TARGET_OPERANDS = [
    (0x00B, 'DIR32', '?g_Rng@@3URngRuntimeView@@A'),
    (0x012, 'REL32', '?GetRandomU32InRange@RngRuntimeView@@QAEII@Z'),
    (0x05A, 'DIR32', '?g_Rng@@3URngRuntimeView@@A'),
    (0x060, 'DIR32', '__real@43c00000'),
    (0x079, 'REL32', '?GetRandomF32InRange@RngRuntimeView@@QAEMM@Z'),
    (0x07F, 'DIR32', '__real@4016cbe4'),
    (0x088, 'REL32', '?AddNormalizeAngle@Th09EclRunControl@@YGMMM@Z'),
    (0x092, 'REL32', '?GetRandomF32InRange@RngRuntimeView@@QAEMM@Z'),
    (0x098, 'DIR32', '__real@3f490fdb'),
    (0x0A1, 'REL32', '?AddNormalizeAngle@Th09EclRunControl@@YGMMM@Z'),
    (0x0B7, 'DIR32', '?g_Rng@@3URngRuntimeView@@A'),
    (0x0C9, 'DIR32', '__real@43c00000'),
    (0x0E2, 'REL32', '?GetRandomF32InRange@RngRuntimeView@@QAEMM@Z'),
    (0x0E8, 'DIR32', '__real@3f490fdb'),
    (0x0F6, 'REL32', '?GetRandomF32InRange@RngRuntimeView@@QAEMM@Z'),
    (0x0FC, 'DIR32', '__real@4016cbe4'),
    (0x105, 'REL32', '?AddNormalizeAngle@Th09EclRunControl@@YGMMM@Z'),
    (0x111, 'DIR32', '?g_Rng@@3URngRuntimeView@@A'),
    (0x116, 'REL32', '?GetRandomF32SignedInRange@RngRuntimeView@@QAEMM@Z'),
    (0x124, 'REL32', '??BFloat3@@QAEPAMXZ'),
    (0x130, 'DIR32', '__real@42400000'),
    (0x145, 'DIR32', '__real@00000000'),
    (0x160, 'DIR32', '__real@42400000'),
    (0x172, 'DIR32', '__real@00000000'),
    (0x191, 'REL32', '?ResolveInt@Th09EclRunControl@@YIHPAUEnemyView@@H@Z'),
    (0x1B4, 'REL32', '?ResolveFloat@EnemyView@@QAEMM@Z'),
    (0x1EE, 'REL32', '??4Th09EclTimerStorageView@@QAEXH@Z'),
    (0x1FA, 'REL32', '?StartTimedPolarDisplacement@Th09EclRunMovement@@YIXPAUEnemyView@@PAUTh09EclRawInstructionHeaderView@@M@Z'),
]


def validate_manifest():
    if len(TARGET_OPERANDS) != 28 or any(
            left[0] >= right[0] for left, right in zip(TARGET_OPERANDS, TARGET_OPERANDS[1:])):
        raise ValueError('unreviewed original operand inventory')
    if any(off < 0 or off + 4 > SIZE or typ not in ('REL32', 'DIR32')
           or name not in REFERENTS for off, typ, name in TARGET_OPERANDS):
        raise ValueError('invalid original operand bounds/kind/name')
    if any(next_off < off+4 for (off, _, _), (next_off, _, _) in
           zip(TARGET_OPERANDS, TARGET_OPERANDS[1:])):
        raise ValueError('overlapping original operand fields')
    for _, typ, name in TARGET_OPERANDS:
        if typ != ('DIR32' if name.startswith(('?g_', '__real@')) else 'REL32'):
            raise ValueError('original operand kind does not match its reviewed role')


def encoded(kind, offset, destination):
    if kind not in ('DIR32', 'REL32') or not 0 <= offset <= 0xFFFFFF:
        raise ValueError('unsupported field kind/offset')
    return (destination - (BASE + offset + 4 if kind == 'REL32' else 0)) & 0xffffffff


def bind_candidate(raw, relocations):
    """Fail closed on missing/reordered/unknown COFF fields or false addends."""
    validate_manifest()
    if len(raw) not in (SIZE, SIZE+1):
        raise ValueError('unreviewed candidate physical extent')
    if len(relocations) != len(TARGET_OPERANDS):
        raise ValueError('candidate 28-field inventory incomplete')
    linked = bytearray(raw)
    bound = []
    previous_end = -1
    for row, (_, expected_kind, expected_name) in zip(relocations, TARGET_OPERANDS):
        off = int(row['offset'])
        if (row['type'], row['symbol'], int(row['addend'])) != (
                expected_kind, expected_name, 0):
            raise ValueError('candidate operand identity/type/addend/order drift')
        if off < previous_end or off < 0 or off + 4 > len(linked):
            raise ValueError('candidate operand overlap or out of bounds')
        previous_end = off + 4
        target = REFERENTS[expected_name]
        struct.pack_into('<I', linked, off, encoded(expected_kind, off, target))
        bound.append((off, expected_kind, target))
    return bytes(linked), bound


def address_fields(instructions, capstone):
    """Recover actual instruction address operands independently of COFF."""
    fields, calls = [], []
    for ins in instructions:
        off = ins.address - BASE
        if ins.mnemonic == 'call':
            if len(ins.operands) != 1 or ins.operands[0].type != capstone.x86.X86_OP_IMM or ins.imm_size != 4:
                raise ValueError('unreviewed indirect/relative call operand')
            destination = ins.operands[0].imm
            fields.append((off + ins.imm_offset, 'REL32', destination))
            calls.append(destination)
            continue
        if ins.mnemonic.startswith('j'):
            continue
        if ins.disp_size == 4:
            for op in ins.operands:
                if op.type == capstone.x86.X86_OP_MEM and 0x400000 <= op.mem.disp < 0x4e7000:
                    if op.mem.base or op.mem.index:
                        raise ValueError('unreviewed indexed global operand')
                    fields.append((off + ins.disp_offset, 'DIR32', op.mem.disp))
        if ins.imm_size == 4:
            for op in ins.operands:
                if op.type == capstone.x86.X86_OP_IMM and 0x400000 <= op.imm < 0x4e7000:
                    fields.append((off + ins.imm_offset, 'DIR32', op.imm))
    return sorted(fields), calls


def inspect(obj):
    import capstone
    from diagnostic_branch_alignment import direct_control_flow

    spec = importlib.util.spec_from_file_location('ecl_biased_coff', ROOT/'scripts/compare-coff-function.py')
    coff = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(coff)
    image = coff.verified_target()
    target = coff.pe_bytes_at(image, BASE, SIZE)
    if hashlib.sha256(target).hexdigest() != TARGET_SHA256:
        raise ValueError('original complete MoveRandomBiased identity drifted')
    for name, destination in REFERENTS.items():
        if name.startswith('__real@') and coff.pe_bytes_at(image, destination, 4) != struct.pack('<I', int(name[7:], 16)):
            raise ValueError('original float literal payload differs: ' + name)
    for off, kind, name in TARGET_OPERANDS:
        if struct.unpack_from('<I', target, off)[0] != encoded(kind, off, REFERENTS[name]):
            raise ValueError('original target address-field encoding differs: ' + name)

    raw, rows = coff.object_function(obj, SYMBOL)
    linked, bound = bind_candidate(raw, rows)
    decoder = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32)
    decoder.detail = True
    ti = list(decoder.disasm(target, BASE))
    ci = list(decoder.disasm(linked, BASE))
    if (not ti or not ci or sum(i.size for i in ti) != SIZE
            or sum(i.size for i in ci) != len(linked) or
            ti[-1].mnemonic != 'ret' or ci[-1].mnemonic != 'ret'):
        raise ValueError('incomplete disassembly or unreviewed return')
    t_fields, t_calls = address_fields(ti, capstone)
    c_fields, c_calls = address_fields(ci, capstone)
    expected = sorted((off, kind, REFERENTS[name]) for off,kind,name in TARGET_OPERANDS)
    if t_fields != expected or c_fields != bound:
        raise ValueError('decoded instruction operands do not match reviewed original/current COFF fields')
    if t_calls != c_calls or len(t_calls) != 14:
        raise ValueError('ordered direct call identities/count differ')
    target_graph = direct_control_flow(ti, capstone.x86.X86_OP_IMM)
    candidate_graph = direct_control_flow(ci, capstone.x86.X86_OP_IMM)
    # Only AFTER independently binding all physical fields, normalize those
    # known field payloads and direct-branch displacements for an aligned
    # diagnostic; keep scalar immediates/register bytes and real offsets.
    shape_spec = importlib.util.spec_from_file_location(
        'biased_ecl_shape', ROOT/'scripts/report-ecl-handler-shapes.py')
    shape = importlib.util.module_from_spec(shape_spec)
    shape_spec.loader.exec_module(shape)
    diff_spec = importlib.util.spec_from_file_location(
        'biased_ecl_changed_instructions', ROOT/'scripts/ecl_shape_differences.py')
    diff = importlib.util.module_from_spec(diff_spec)
    diff_spec.loader.exec_module(diff)
    t_stream = shape.normalized_stream(target, BASE, {off for off, _, _ in expected})
    c_stream = shape.normalized_stream(linked, BASE, {off for off, _, _ in bound})
    changes = diff.changed_instructions(c_stream, t_stream)
    differences = [f'0x{n:03X}' for n,(a,b) in enumerate(zip(target, linked)) if a!=b]
    return {
        'diagnostic_only':True,
        'exactness_credit':'none; canonical exact requires full equal owner under cold pinned match-unit replay',
        'object':str(obj),
        'target_bytes':SIZE,
        'candidate_bytes':len(linked),
        'target_instructions':len(ti),
        'candidate_instructions':len(ci),
        'bound_fields':len(bound),
        'ordered_calls':len(t_calls),
        'target_direct_blocks':len(target_graph),
        'candidate_direct_blocks':len(candidate_graph),
        'complete_direct_graphs_agree':target_graph==candidate_graph,
        'overlapping_differences':len(differences),
        'extra_candidate_bytes':max(0,len(linked)-SIZE),
        'complete_bytes_equal':linked==target,
        'difference_offsets':differences,
        'normalized_instruction_changes':changes,
        'raw_sha256':hashlib.sha256(raw).hexdigest(),
        'linked_sha256':hashlib.sha256(linked).hexdigest(),
        'target_sha256':TARGET_SHA256,
    }


def main():
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('object',type=Path)
    parser.add_argument('--json',action='store_true')
    parser.add_argument('--explain',action='store_true',help='show fully bound instruction-shape differences')
    args=parser.parse_args()
    try:
        result=inspect(args.object)
    except (OSError, ValueError, KeyError, ImportError, RuntimeError, struct.error) as exc:
        print(json.dumps({'error':str(exc)}))
        return 2
    if args.json:
        print(json.dumps(result,indent=2))
    else:
        print('MoveRandomBiased: %d/%d bytes, %d reviewed COFF fields, %d ordered calls, %d/%d CFG blocks, %d overlapping differences (+%d bytes); exact=%s (NON-CREDITING)' % (
            result['candidate_bytes'],result['target_bytes'],result['bound_fields'],result['ordered_calls'],
            result['candidate_direct_blocks'],result['target_direct_blocks'],result['overlapping_differences'],
            result['extra_candidate_bytes'],result['complete_bytes_equal']))
        if args.explain:
            print('Reviewed normalized instruction differences:')
            for row in result['normalized_instruction_changes']:
                print(' ',row['change'])
                for off, data in row['target']:
                    print('    target    +0x%03X: %s' % (off, data))
                for off, data in row['candidate']:
                    print('    candidate +0x%03X: %s' % (off, data))
    return 0 if result['complete_bytes_equal'] else 1


if __name__=='__main__':
    raise SystemExit(main())
