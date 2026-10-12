#!/usr/bin/env python3
"""Complete non-crediting TH09 ExAttack Type19 update target/COFF audit.

Every target-independent candidate field is bound to a reviewed original
TH09 address or a literal validated against the verified PE. The target's
complete 821-byte extent, all target/candidate operands, call identities and
full direct CFG are checked. No masking/rebasing can confer exactness credit.
The caller must compile the supplied object from maintained source separately.
"""

from __future__ import annotations

import argparse
from collections import Counter
import hashlib
import importlib.util
import json
from pathlib import Path
import struct

ROOT = Path(__file__).resolve().parents[1]
BASE = 0x0044A330
SIZE = 821
SYMBOL = '?ExAttackUpdateCallbackType19@@YIHPAUExAttackRecord@@@Z'

DESTINATIONS = {
    '??OZunTimer@@QAEIH@Z': 0x00403DE0,
    '??MZunTimer@@QAEIH@Z': 0x00401540,
    '??BZunTimer@@QAEMXZ': 0x004014F0,
    '??4ZunTimer@@QAEXH@Z': 0x00401500,
    '?FromAngleMagnitude@Float3@@QAEXMM@Z': 0x00441890,
    '??YFloat3@@QAEAAU0@ABU0@@Z': 0x00405730,
    '??GFloat3@@QBE?AU0@ABU0@@Z': 0x00401140,
    '?ExAttackInterpolate2D@@YIXPAM0000MM@Z': 0x0042AF80,
    '?ExecuteAnmIdx@AnmLoaded@@QAEXPAUAnmVm@@H@Z': 0x00401560,
    '?g_GameManager@@3UExAttackType19UpdateGameManagerView@@A': 0x004A7D90,
    '__real@00000000': 0x0048E314,
    '__real@3f800000': 0x0048E2A4,
    '__real@3e4f8c3d': 0x00491608,
    '__real@45800000': 0x00491654,
    '__real@3ecccccd': 0x00491650,
}
# Type19 target address fields are specific and independently reviewed: four
# distinct reads/addresses of the live GameManager Player slot +4, 11 direct
# calls and the remaining Float3/Timer/ANM and float-constant references.
GAME_MANAGER_FIELD = 0x004A7D94
EXPECTED_COUNT = 35
EXPECTED_INSTRUCTIONS = 254
EXPECTED_CALLS = 11
EXPECTED_BLOCKS = 30


def load_coff():
    spec = importlib.util.spec_from_file_location(
        'exattack_type19_coff', ROOT / 'scripts/compare-coff-function.py')
    if spec is None or spec.loader is None:
        raise ValueError('cannot load target-bound COFF parser')
    module = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(module)
    return module


def decode_owner(raw: bytes, *, expected_size: int | None = None):
    import capstone

    if expected_size is not None and len(raw) != expected_size:
        raise ValueError('unexpected complete Type19 target extent')
    decoder = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32)
    decoder.detail = True
    instructions = list(decoder.disasm(raw, BASE))
    if not instructions or sum(ins.size for ins in instructions) != len(raw):
        raise ValueError('incomplete Type19 physical owner decoding')
    if instructions[-1].mnemonic != 'ret':
        raise ValueError('Type19 owner lacks terminal RET')
    fields = []
    direct_calls = []
    for ins in instructions:
        if (ins.mnemonic == 'call' and
                (len(ins.operands) != 1 or
                 ins.operands[0].type != capstone.x86.X86_OP_IMM)):
            raise ValueError('unreviewed indirect Type19 call')
        for operand in ins.operands:
            if (operand.type == capstone.x86.X86_OP_MEM and
                    0x400000 <= operand.mem.disp < 0x4E7000):
                if ins.disp_size != 4:
                    raise ValueError('unsupported Type19 absolute memory operand')
                fields.append((ins.address - BASE + ins.disp_offset,
                               'DIR32', operand.mem.disp))
            elif ins.mnemonic == 'call' and operand.type == capstone.x86.X86_OP_IMM:
                if ins.imm_size != 4:
                    raise ValueError('unsupported Type19 direct call encoding')
                fields.append((ins.address - BASE + ins.imm_offset,
                               'REL32', operand.imm))
                direct_calls.append(operand.imm)
            elif (ins.mnemonic != 'call' and not ins.mnemonic.startswith('j') and
                  operand.type == capstone.x86.X86_OP_IMM and
                  0x400000 <= operand.imm < 0x4E7000):
                if ins.imm_size != 4:
                    raise ValueError('unreviewed Type19 address immediate')
                fields.append((ins.address - BASE + ins.imm_offset,
                               'DIR32', operand.imm))
    return sorted(fields), instructions, direct_calls


def bind_candidate(raw: bytes, rows: list[dict]):
    if len(rows) != EXPECTED_COUNT:
        raise ValueError('Type19 candidate is missing or adding address fields')
    linked = bytearray(raw)
    occupied = set()
    fields = []
    for row in rows:
        symbol = row['symbol']
        if symbol not in DESTINATIONS:
            raise ValueError('unreviewed Type19 COFF symbol: ' + symbol)
        offset = int(row['offset'])
        kind = row['type']
        if kind not in ('DIR32', 'REL32'):
            raise ValueError('unsupported Type19 relocation kind')
        covered = set(range(offset, offset + 4))
        if offset < 0 or offset + 4 > len(linked) or occupied.intersection(covered):
            raise ValueError('Type19 field outside physical extent or overlapping')
        occupied.update(covered)
        addend = int(row['addend'])
        if symbol.startswith('__real@') and addend != 0:
            raise ValueError('unexpected Type19 float literal addend')
        if symbol.startswith('?g_GameManager') and addend != 4:
            raise ValueError('unreviewed Type19 GameManager field offset')
        destination = DESTINATIONS[symbol] + addend
        effective = destination - (BASE + offset + 4 if kind == 'REL32' else 0)
        struct.pack_into('<I', linked, offset, effective & 0xFFFFFFFF)
        fields.append((offset, kind, destination))
    return bytes(linked), sorted(fields)


def diagnose(path: Path):
    import capstone
    from diagnostic_branch_alignment import direct_control_flow

    coff = load_coff()
    pe = coff.verified_target()
    target = coff.pe_bytes_at(pe, BASE, SIZE)
    target_fields, target_instructions, target_calls = decode_owner(target, expected_size=SIZE)
    if (len(target_fields) != EXPECTED_COUNT or
            len(target_instructions) != EXPECTED_INSTRUCTIONS or
            len(target_calls) != EXPECTED_CALLS or
            sum(kind == 'DIR32' and dst == GAME_MANAGER_FIELD
                for _, kind, dst in target_fields) != 4):
        raise ValueError('unexpected verified TH09 Type19 field/call inventory')
    for symbol, destination in DESTINATIONS.items():
        if symbol.startswith('__real@') and coff.pe_bytes_at(pe, destination, 4) != struct.pack(
                '<I', int(symbol[7:], 16)):
            raise ValueError('Type19 float literal identity disagrees: ' + symbol)
    raw, rows = coff.object_function(path, SYMBOL)
    linked, fields = bind_candidate(raw, rows)
    candidate_fields, candidate_instructions, candidate_calls = decode_owner(linked)
    if candidate_fields != fields:
        raise ValueError('Type19 candidate COFF rows do not cover decoded operands')
    expected = Counter((kind, dst) for _, kind, dst in target_fields)
    actual = Counter((kind, dst) for _, kind, dst in fields)
    if actual != expected:
        raise ValueError('Type19 independent target/candidate field identities differ')
    target_graph = direct_control_flow(target_instructions, capstone.x86.X86_OP_IMM)
    candidate_graph = direct_control_flow(candidate_instructions, capstone.x86.X86_OP_IMM)
    if len(target_graph) != EXPECTED_BLOCKS:
        raise ValueError('unexpected Type19 target direct CFG')
    if target_calls != candidate_calls:
        raise ValueError('Type19 ordered target/candidate call identities differ')
    differences = [i for i, (a, b) in enumerate(zip(target, linked)) if a != b]
    return dict(
        diagnostic_only=True,
        exactness_credit='none',
        object=str(path),
        target_bytes=SIZE,
        candidate_bytes=len(linked),
        target_instructions=len(target_instructions),
        candidate_instructions=len(candidate_instructions),
        target_fields=len(target_fields),
        candidate_fields=len(fields),
        distinct_player_slot_fields=actual[('DIR32', GAME_MANAGER_FIELD)],
        target_calls=len(target_calls),
        candidate_calls=len(candidate_calls),
        target_direct_blocks=len(target_graph),
        candidate_direct_blocks=len(candidate_graph),
        complete_direct_graphs_agree=target_graph == candidate_graph,
        complete_bytes_agree=linked == target,
        overlapping_differences=len(differences),
        missing_target_bytes=max(0, SIZE - len(linked)),
        extra_candidate_bytes=max(0, len(linked) - SIZE),
        first_differences=[hex(x) for x in differences[:20]],
        raw_sha256=hashlib.sha256(raw).hexdigest(),
        linked_sha256=hashlib.sha256(linked).hexdigest(),
    )


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('object', type=Path)
    parser.add_argument('--json', action='store_true')
    args = parser.parse_args()
    try:
        result = diagnose(args.object)
    except (OSError, ValueError, KeyError, ImportError, struct.error) as exc:
        print(json.dumps({'result': 'error', 'message': str(exc)}))
        return 2
    if args.json:
        print(json.dumps(result, indent=2))
    else:
        print('NON-CREDITING Type19 update: %d/%d bytes, %d fields, %d complete differing bytes; exact=%s' % (
            result['candidate_bytes'], SIZE, result['candidate_fields'],
            result['overlapping_differences'], result['complete_bytes_agree']))
    return 0 if result['complete_bytes_agree'] else 1


if __name__ == '__main__':
    raise SystemExit(main())
