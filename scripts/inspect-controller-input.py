#!/usr/bin/env python3
"""Complete TH09 Controller::GetControllerInput PE/COFF diagnostic (no exact credit).

Bind target address fields from reviewed independent TH09 global, library-IAT,
and exact helper identities. An object must separately be compiled from the
maintained source with the pinned VC7.1 profile. This diagnostic never adjusts
field destinations to fit target bytes or discounts an ordinary mismatch.
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
BASE = 0x0042B410
SIZE = 808
SYMBOL = '?GetControllerInput@Controller@@YIGHHG@Z'
DESTINATIONS = {
    '?g_Supervisor@@3VSupervisor@@A': 0x004B3100,
    '?g_ReplayInputStates@@3PAUReplayInputState@@A': 0x004ACE18,
    '?SetButtonFromDirectInputState@Controller@@YIIPAGFGPAE@Z': 0x0042AC80,
    '?SetButtonFromJoystickButtons@Controller@@YIIPAGFGI@Z': 0x0042ACB0,
    '__imp__joyGetPosEx@8': 0x0048E23C,
    '?g_JoystickCaps@Controller@@3PAUtagJOYCAPSA@@A': 0x004AC8D8,
}
SUPERVISOR = '?g_Supervisor@@3VSupervisor@@A'
INPUT_STATES = '?g_ReplayInputStates@@3PAUReplayInputState@@A'
# Actual shared declarations and their target-backed fields. These are COFF
# identities, independently required even if an alias could link to equal bytes.
NATIVE_GLOBAL_FIELDS = (
    (0x00A, SUPERVISOR, 0x14),
    (0x018, INPUT_STATES, 0),
    (0x02D, SUPERVISOR, 0x18),
    (0x03C, SUPERVISOR, 0x14),
    (0x04D, SUPERVISOR, 0x14),
    (0x063, SUPERVISOR, 0x14),
    (0x08D, SUPERVISOR, 0x14),
    (0x17D, SUPERVISOR, 0x430),
    (0x18A, SUPERVISOR, 0x432),
)
# Independently decoded target field offsets, kinds and destinations. Includes
# the one true ADD address immediate and the IAT-indirect call memory operand.
TARGET_FIELDS = (
    (0x00A, 'DIR32', 0x004B3114),
    (0x018, 'DIR32', 0x004ACE18),
    (0x02D, 'DIR32', 0x004B3118),
    (0x03C, 'DIR32', 0x004B3114),
    (0x04D, 'DIR32', 0x004B3114),
    (0x063, 'DIR32', 0x004B3114),
    (0x08D, 'DIR32', 0x004B3114),
    *((offset, 'REL32', 0x0042AC80) for offset in (
        0x0D6, 0x0EA, 0x0FE, 0x112, 0x126, 0x13A, 0x14E, 0x162, 0x176)),
    (0x17D, 'DIR32', 0x004B3530),
    (0x18A, 'DIR32', 0x004B3532),
    (0x209, 'DIR32', 0x0048E23C),
    *((offset, 'REL32', 0x0042ACB0) for offset in (
        0x226, 0x235, 0x244, 0x253, 0x262, 0x271, 0x280, 0x28F, 0x29E)),
    (0x2AD, 'DIR32', 0x004AC900),
    (0x2B3, 'DIR32', 0x004AC8FC),
    (0x2DF, 'DIR32', 0x004AC908),
    (0x2E5, 'DIR32', 0x004AC904),
)


def validate_manifest():
    if len(TARGET_FIELDS) != 32 or sorted(TARGET_FIELDS) != list(TARGET_FIELDS):
        raise ValueError('target Controller field inventory is incomplete/unsorted')
    seen = set()
    for offset, kind, destination in TARGET_FIELDS:
        space = set(range(offset, offset + 4))
        if (offset < 0 or offset + 4 > SIZE or seen.intersection(space)
                or kind not in ('REL32', 'DIR32')
                or not 0x400000 <= destination < 0x4e7000):
            raise ValueError('target Controller field is out of bounds or overlapping')
        seen.update(space)
    if Counter((kind, dst) for _, kind, dst in TARGET_FIELDS) != Counter({
        ('DIR32', 0x4B3114): 5, ('DIR32', 0x4ACE18): 1,
        ('DIR32', 0x4B3118): 1, ('DIR32', 0x4B3530): 1,
        ('DIR32', 0x4B3532): 1, ('DIR32', 0x48E23C): 1,
        ('DIR32', 0x4AC900): 1, ('DIR32', 0x4AC8FC): 1,
        ('DIR32', 0x4AC908): 1, ('DIR32', 0x4AC904): 1,
        ('REL32', 0x42AC80): 9, ('REL32', 0x42ACB0): 9,
    }):
        raise ValueError('target Controller field destination inventory changed')


def decode_fields(code: bytes):
    import capstone

    decoder = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32)
    decoder.detail = True
    instructions = list(decoder.disasm(code, BASE))
    if not instructions or sum(i.size for i in instructions) != len(code):
        raise ValueError('incomplete Controller owner disassembly')
    found = []
    calls = []
    for ins in instructions:
        if ins.mnemonic == 'call':
            operand = ins.operands[0]
            if operand.type == capstone.x86.X86_OP_IMM:
                calls.append(operand.imm)
        for operand in ins.operands:
            if (operand.type == capstone.x86.X86_OP_MEM and
                    0x400000 <= operand.mem.disp < 0x4E7000):
                if ins.disp_size != 4:
                    raise ValueError('unsupported Controller absolute memory size')
                found.append((ins.address - BASE + ins.disp_offset,
                              'DIR32', operand.mem.disp))
            elif (ins.mnemonic == 'call' and
                  operand.type == capstone.x86.X86_OP_IMM):
                if ins.imm_size != 4:
                    raise ValueError('unsupported Controller direct call size')
                found.append((ins.address - BASE + ins.imm_offset,
                              'REL32', operand.imm))
            elif (not ins.mnemonic.startswith('j') and ins.mnemonic != 'call' and
                  operand.type == capstone.x86.X86_OP_IMM and
                  0x400000 <= operand.imm < 0x4E7000):
                if ins.imm_size != 4:
                    raise ValueError('unsupported Controller address immediate size')
                found.append((ins.address - BASE + ins.imm_offset,
                              'DIR32', operand.imm))
    return sorted(found), instructions, calls


def validate_native_globals(rows: list[dict]):
    native = [row for row in rows if row['symbol'] in (SUPERVISOR, INPUT_STATES)]
    expected = Counter(NATIVE_GLOBAL_FIELDS)
    actual = Counter((int(row['offset']), row['symbol'], int(row['addend']))
                     for row in native)
    if actual != expected:
        raise ValueError('Controller native global field identities disagree')
    for row in native:
        if (row['type'] != 'DIR32' or row.get('symbol_section') != 0
                or row.get('symbol_value') != 0):
            raise ValueError('Controller native globals must be undefined DIR32 symbols')
    return len(native)


def bind_candidate(code: bytes, rows: list[dict]):
    if len(rows) != len(TARGET_FIELDS):
        raise ValueError('Controller candidate does not cover all 32 fields')
    linked = bytearray(code)
    used = set()
    bound = []
    for row in rows:
        offset = int(row['offset'])
        symbol = row['symbol']
        if symbol not in DESTINATIONS:
            raise ValueError('unknown Controller external symbol: '+symbol)
        if row['type'] not in ('DIR32', 'REL32'):
            raise ValueError('unsupported Controller relocation kind')
        covered = set(range(offset, offset + 4))
        if offset < 0 or offset + 4 > len(code) or used.intersection(covered):
            raise ValueError('Controller relocation outside candidate extent')
        used.update(covered)
        dst = DESTINATIONS[symbol] + int(row['addend'])
        # The target indexes the two actual JOYCAPSA records with 0x194 stride.
        if symbol.endswith('tagJOYCAPSA@@A') and int(row['addend']) not in (36, 40, 44, 48):
            raise ValueError('unreviewed JOYCAPSA member offset')
        encoded = dst - (BASE + offset + 4 if row['type'] == 'REL32' else 0)
        struct.pack_into('<I', linked, offset, encoded & 0xFFFFFFFF)
        bound.append((offset, row['type'], dst))
    validate_native_globals(rows)
    return bytes(linked), sorted(bound)


def diagnose(obj: Path):
    import capstone

    validate_manifest()
    sp = importlib.util.spec_from_file_location('controller_coff', ROOT/'scripts/compare-coff-function.py')
    if sp is None or sp.loader is None:
        raise ValueError('cannot import target-bound COFF comparer')
    coff = importlib.util.module_from_spec(sp)
    sp.loader.exec_module(coff)
    target = coff.pe_bytes_at(coff.verified_target(), BASE, SIZE)
    target_fields, t_ins, t_calls = decode_fields(target)
    if target_fields != list(TARGET_FIELDS) or len(t_ins) != 246:
        raise ValueError('verified target Controller fields/instructions disagree')
    original, rows = coff.object_function(obj, SYMBOL, include_symbol_locations=True)
    linked, bound_fields = bind_candidate(original, rows)
    candidate_fields, c_ins, c_calls = decode_fields(linked)
    if candidate_fields != bound_fields:
        raise ValueError('candidate code operands do not match COFF field inventory')
    expected_destinations = Counter((kind, dst) for _, kind, dst in TARGET_FIELDS)
    if Counter((kind, dst) for _, kind, dst in candidate_fields) != expected_destinations:
        raise ValueError('Controller candidate contains missing or extra semantic fields')
    def direct_edges(ins):
        starts = {i.address for i in ins}
        edges = []
        for insn in ins:
            if not insn.mnemonic.startswith('j'):
                continue
            if not insn.operands or insn.operands[0].type != capstone.x86.X86_OP_IMM:
                raise ValueError('unreviewed indirect Controller branch')
            dst = insn.operands[0].imm
            if dst not in starts:
                raise ValueError('Controller branch not at an instruction boundary')
            edges.append((insn.address - BASE, dst - BASE))
        return edges
    td, cd = direct_edges(t_ins), direct_edges(c_ins)
    differences = [i for i, (a, b) in enumerate(zip(target, linked)) if a != b]
    return dict(
        diagnostic_only=True, exactness_credit='none',
        target_bytes=SIZE, candidate_bytes=len(linked),
        target_instructions=len(t_ins), candidate_instructions=len(c_ins),
        target_fields=len(TARGET_FIELDS), bound_candidate_fields=len(candidate_fields),
        native_global_fields_checked=validate_native_globals(rows),
        prefix_674_equal=linked[:674] == target[:674],
        controller_slot_fields=sum(dst in (0x4AC900, 0x4AC8FC, 0x4AC908, 0x4AC904)
                                   for _, _, dst in candidate_fields),
        ordered_direct_calls_agree=t_calls == c_calls,
        target_direct_branches=len(td), candidate_direct_branches=len(cd),
        complete_direct_branch_graphs_agree=td == cd,
        overlapping_differences=len(differences),
        missing_bytes=max(0, SIZE-len(linked)),
        excess_bytes=max(0, len(linked)-SIZE),
        first_difference_offset=hex(differences[0]) if differences else None,
        first_difference_offsets=[hex(x) for x in differences[:16]],
        complete_exact=linked == target,
        raw_sha256=hashlib.sha256(original).hexdigest(),
        linked_sha256=hashlib.sha256(linked).hexdigest(),
    )


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('object', type=Path)
    parser.add_argument('--json', action='store_true')
    args = parser.parse_args()
    try:
        report = diagnose(args.object)
    except (OSError, ValueError, KeyError, ImportError, struct.error) as exc:
        print(json.dumps({'result':'error','message':str(exc)}))
        return 2
    if args.json:
        print(json.dumps(report, indent=2))
    else:
        print('Controller input full owner: %d/%d bytes, %d fields, %d overlap differences, %d absent, exact=%s (diagnostic only)' % (
            report['candidate_bytes'], report['target_bytes'], report['bound_candidate_fields'],
            report['overlapping_differences'], report['missing_bytes'], report['complete_exact']))
    return 0 if report['complete_exact'] else 1


if __name__ == '__main__':
    raise SystemExit(main())
