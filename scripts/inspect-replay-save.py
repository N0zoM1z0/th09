#!/usr/bin/env python3
"""Complete, original-bound non-crediting SaveReplay byte/field/call audit.

The original field census is decoded independently of candidate offsets.
Source bindings pin real COFF symbols and addends; defined literals are read
from their actual sections. A successful diagnostic is not an exact receipt.
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
BASE, SIZE = 0x00420C60, 1238
SYMBOL = '?SaveReplay@ReplayManagerView@@SIHPAU1@PBD1@Z'
TARGET_SHA256 = 'a225c9a1aae3db79d6e31b22e77ccba892b38c44fe9572233cd6df24edb1cef3'
RESIDUAL_OFFSETS = frozenset((0xB7, 0xBB, 0x3F6))

# SHA-pinned original instructions, not solved from candidate field positions.
TARGET_FIELDS = (
    (0x028, 'REL32', 0x00420C40),
    (0x06B, 'DIR32', 0x0048F18C),
    (0x071, 'DIR32', 0x004AD018),
    (0x076, 'REL32', 0x00401340),
    (0x1CB, 'DIR32', 0x0048F178),
    (0x1E1, 'REL32', 0x004200C0),
    (0x1E6, 'DIR32', 0x004AC879),
    (0x1EB, 'DIR32', 0x0048F168),
    (0x1F2, 'REL32', 0x004200C0),
    (0x1F8, 'DIR32', 0x004A7EAC),
    (0x1FF, 'DIR32', 0x004A1CD0),
    (0x205, 'DIR32', 0x0048F158),
    (0x20C, 'REL32', 0x004200C0),
    (0x212, 'DIR32', 0x004A7EA8),
    (0x220, 'DIR32', 0x004A7EA4),
    (0x22A, 'DIR32', 0x004A1CB8),
    (0x230, 'DIR32', 0x0048F148),
    (0x237, 'REL32', 0x004200C0),
    (0x23F, 'DIR32', 0x004A7D90),
    (0x246, 'REL32', 0x00415D30),
    (0x250, 'DIR32', 0x004A7DB0),
    (0x257, 'DIR32', 0x004A1C78),
    (0x25D, 'DIR32', 0x0048F134),
    (0x262, 'REL32', 0x004200C0),
    (0x26D, 'DIR32', 0x004A7DE8),
    (0x274, 'DIR32', 0x004A1C78),
    (0x279, 'DIR32', 0x004A7DB0),
    (0x280, 'DIR32', 0x004A1C78),
    (0x287, 'DIR32', 0x0048F11C),
    (0x28C, 'REL32', 0x004200C0),
    (0x295, 'DIR32', 0x004A810C),
    (0x2A0, 'DIR32', 0x00490F54),
    (0x2AA, 'DIR32', 0x004A8108),
    (0x2B0, 'DIR32', 0x00490F54),
    (0x2B8, 'DIR32', 0x0048F100),
    (0x2BD, 'REL32', 0x004200C0),
    (0x2C2, 'DIR32', 0x0048F0E8),
    (0x2C7, 'DIR32', 0x0048F0D0),
    (0x2CE, 'REL32', 0x004200C0),
    (0x322, 'DIR32', 0x004AC879),
    (0x338, 'DIR32', 0x004ACE0C),
    (0x33D, 'REL32', 0x004048B0),
    (0x349, 'DIR32', 0x004ACE0C),
    (0x35B, 'REL32', 0x004048B0),
    (0x365, 'DIR32', 0x004ACE0C),
    (0x370, 'REL32', 0x004048B0),
    (0x3A2, 'REL32', 0x00434020),
    (0x3AB, 'DIR32', 0x004AD018),
    (0x3B3, 'REL32', 0x00401360),
    (0x463, 'DIR32', 0x0048E068),
    (0x470, 'DIR32', 0x0048E064),
    (0x4C0, 'DIR32', 0x0048E060),
    (0x4C7, 'DIR32', 0x0048E05C),
)

# Reviewed source referents, including the real ZunMemory member ABI.
REFERENTS = {
    '??0ReplayDataView@@QAE@XZ': 0x00420C40,
    '??_C@_07OMKIOBLC@rep?5tmp?$AA@': 0x0048F18C,
    '?g_ZunMemory@@3VZunMemory@@A': 0x004AD018,
    '?Alloc@ZunMemory@@QAEPAXIPBD@Z': 0x00401340,
    '?g_ReplayInfoPlayerNameFormat@@3QBDB': 0x0048F178,
    '?AppendReplayFormat@@YAPADPADPBDZZ': 0x004200C0,
    '?g_ReplayPlayTimeText@@3PADA': 0x004AC879,
    '?g_ReplayInfoPlayTimeFormat@@3QBDB': 0x0048F168,
    '?g_GameManager@@3UGameManagerReplayView@@A': 0x004A7D90,
    '?g_ReplayDifficultyNames@@3PAPBDA': 0x004A1CD0,
    '?g_ReplayInfoDifficultyFormat@@3QBDB': 0x0048F158,
    '?g_ReplayModeNames@@3PAPBDA': 0x004A1CB8,
    '?g_ReplayInfoModeFormat@@3QBDB': 0x0048F148,
    '?IsReplayStateTwo@GameManagerReplayView@@QAEHXZ': 0x00415D30,
    '?g_ReplayCharacterNames@@3PAPBDA': 0x004A1C78,
    '?g_ReplayInfoCharacterFormat@@3QBDB': 0x0048F134,
    '?g_ReplayInfoVersusCharactersFormat@@3QBDB': 0x0048F11C,
    '?g_ReplayInitialHealth1@@3HA': 0x004A810C,
    '__real@3f000000': 0x00490F54,
    '?g_ReplayInitialHealth0@@3HA': 0x004A8108,
    '?g_ReplayInfoInitialHealthFormat@@3QBDB': 0x0048F100,
    '?g_ReplayVersionText@@3QBDB': 0x0048F0E8,
    '?g_ReplayInfoVersionFormat@@3QBDB': 0x0048F0D0,
    '?g_ReplayRng@@3UReplayRngView@@A': 0x004ACE0C,
    '?GetRandomU16InRange@ReplayRngView@@QAEHG@Z': 0x004048B0,
    '?Encode@Lzss@@SIPAEPAEHPAH@Z': 0x00434020,
    '?Free@ZunMemory@@QAEXPAX@Z': 0x00401360,
    '__imp__CreateFileA@28': 0x0048E068,
    '__imp__WriteFile@20': 0x0048E064,
    '__imp__CloseHandle@4': 0x0048E060,
    '__imp__GlobalFree@4': 0x0048E05C,
}

ALLOWED_GAME_MANAGER_ADDENDS = frozenset((0, 0x20, 0x58, 0x114, 0x118, 0x11C))
LITERALS = {
    '??_C@_07OMKIOBLC@rep?5tmp?$AA@': b'rep tmp\0',
    '__real@3f000000': struct.pack('<f', 0.5),
}


def module(name, filename):
    spec = importlib.util.spec_from_file_location(name, ROOT/'scripts'/filename)
    result = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(result)
    return result


def require_original(target):
    if len(target) != SIZE or hashlib.sha256(target).hexdigest() != TARGET_SHA256:
        raise ValueError('SaveReplay original complete extent/SHA-256 differs')


def decode(code):
    import capstone
    cs = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32)
    cs.detail = True
    instructions = list(cs.disasm(code, BASE))
    if (not instructions or sum(i.size for i in instructions) != len(code)
            or instructions[-1].mnemonic != 'ret'
            or instructions[-1].op_str != '4'):
        raise ValueError('SaveReplay incomplete owner or changed ret 4 transport')
    return instructions


def address_fields(instructions):
    import capstone
    fields = []
    for ins in instructions:
        if ins.mnemonic.startswith('j'):
            continue
        if ins.mnemonic == 'call' and ins.operands[0].type == capstone.x86.X86_OP_IMM:
            if ins.imm_size != 4:
                raise ValueError('SaveReplay non-REL32 direct call')
            fields.append((ins.address-BASE+ins.imm_offset, 'REL32', ins.operands[0].imm))
            continue
        for op in ins.operands:
            if (op.type == capstone.x86.X86_OP_MEM and ins.disp_size == 4
                    and 0x00400000 <= op.mem.disp < 0x004E7000):
                fields.append((ins.address-BASE+ins.disp_offset, 'DIR32', op.mem.disp))
            elif (op.type == capstone.x86.X86_OP_IMM and ins.imm_size == 4
                    and 0x00400000 <= op.imm < 0x004E7000):
                fields.append((ins.address-BASE+ins.imm_offset, 'DIR32', op.imm))
    return fields


def bind_candidate(raw, rows):
    if len(rows) != len(TARGET_FIELDS):
        raise ValueError('SaveReplay complete 53-field inventory differs')
    linked = bytearray(raw)
    fields = []
    previous_end = 0
    for row, (_, original_kind, original_va) in zip(rows, TARGET_FIELDS):
        symbol, kind, addend, off = (row['symbol'], row['type'],
                                    row['addend'], row['offset'])
        allowed = (ALLOWED_GAME_MANAGER_ADDENDS if symbol ==
                   '?g_GameManager@@3UGameManagerReplayView@@A' else {0})
        if (symbol not in REFERENTS or type(addend) is not int or addend not in allowed
                or kind not in ('DIR32', 'REL32') or kind != original_kind
                or REFERENTS[symbol] + addend != original_va):
            raise ValueError('SaveReplay source symbol/root/addend/order differs')
        if type(off) is not int or off < previous_end or off+4 > len(raw):
            raise ValueError('SaveReplay overlapping or out-of-range source fields')
        previous_end = off+4
        value = original_va - (BASE+off+4 if kind == 'REL32' else 0)
        struct.pack_into('<I', linked, off, value & 0xFFFFFFFF)
        fields.append((off, kind, original_va))
    return bytes(linked), fields


def literal_payload(data, row, size):
    if len(data) < 20:
        raise ValueError('SaveReplay truncated COFF header')
    machine, count = struct.unpack_from('<HH', data)
    optional = struct.unpack_from('<H', data, 16)[0]
    section, value = row.get('symbol_section'), row.get('symbol_value')
    if (machine != 0x14C or optional != 0 or type(section) is not int or
            not 1 <= section <= count or type(value) is not int or value < 0):
        raise ValueError('SaveReplay literal lacks an actual COFF definition')
    header = 20+(section-1)*40
    if header+40 > len(data):
        raise ValueError('SaveReplay truncated COFF section header')
    length, start = struct.unpack_from('<II', data, header+16)
    if (start < 20+count*40 or value+size > length or start+length > len(data)):
        raise ValueError('SaveReplay literal exceeds its actual section')
    return data[start+value:start+value+size]


def check_literals(data, rows, image, coff):
    seen = set()
    for row in rows:
        symbol = row['symbol']
        if symbol not in LITERALS:
            continue
        expected = LITERALS[symbol]
        if (literal_payload(data, row, len(expected)) != expected or
                coff.pe_bytes_at(image, REFERENTS[symbol], len(expected)) != expected):
            raise ValueError('SaveReplay actual source/original literal differs')
        seen.add(symbol)
    if seen != set(LITERALS):
        raise ValueError('SaveReplay incomplete literal definition inventory')
    return len(seen)


def call_identity(ins):
    import capstone
    op = ins.operands[0]
    if op.type == capstone.x86.X86_OP_IMM:
        return ('direct', op.imm)
    if (op.type == capstone.x86.X86_OP_MEM and op.mem.base == 0
            and op.mem.index == 0 and op.mem.disp in (0x48E068, 0x48E060, 0x48E05C)):
        return ('iat-cell', op.mem.disp)
    if op.type == capstone.x86.X86_OP_REG and ins.reg_name(op.reg) == 'edi':
        return ('register', 'edi')
    raise ValueError('SaveReplay unreviewed indirect call operand')


def call_inventory(instructions):
    # Register operands are recorded as operands, not asserted runtime callees.
    # Separately require the single real WriteFile IAT load into EDI.
    loads = [i for i in instructions if i.mnemonic == 'mov'
             and i.op_str == 'edi, dword ptr [0x48e064]']
    calls = [(i.address-BASE, call_identity(i)) for i in instructions
             if i.mnemonic == 'call']
    register_calls = [off for off, identity in calls if identity == ('register', 'edi')]
    if (len(calls) != 23 or len(loads) != 1 or len(register_calls) != 4
            or loads[0].address-BASE >= min(register_calls)):
        raise ValueError('SaveReplay call operands or WriteFile IAT load differ')
    return calls


def diagnose(path):
    import capstone
    from diagnostic_branch_alignment import direct_control_flow
    coff = module('save_replay_coff', 'compare-coff-function.py')
    image = coff.verified_target()
    target = coff.pe_bytes_at(image, BASE, SIZE)
    require_original(target)
    ti = decode(target)
    if len(ti) != 380 or tuple(address_fields(ti)) != TARGET_FIELDS:
        raise ValueError('SaveReplay independently decoded original census differs')
    raw, rows = coff.object_function(path, SYMBOL, include_symbol_locations=True)
    linked, fields = bind_candidate(raw, rows)
    ci = decode(linked)
    if address_fields(ci) != fields:
        raise ValueError('SaveReplay COFF fields do not equal decoded machine operands')
    literals = check_literals(path.read_bytes(), rows, image, coff)
    tc, cc = call_inventory(ti), call_inventory(ci)
    tg = direct_control_flow(ti, capstone.x86.X86_OP_IMM, call_identity)
    cg = direct_control_flow(ci, capstone.x86.X86_OP_IMM, call_identity)
    differences = [i for i, (x, y) in enumerate(zip(target, linked)) if x != y]
    return {
        'diagnostic_only': True, 'exactness_credit': 'none',
        'target_bytes': SIZE, 'candidate_bytes': len(raw),
        'target_sha256': TARGET_SHA256,
        'candidate_raw_sha256': hashlib.sha256(raw).hexdigest(),
        'object_sha256': hashlib.sha256(path.read_bytes()).hexdigest(),
        'target_instructions': len(ti), 'candidate_instructions': len(ci),
        'original_fields': len(TARGET_FIELDS), 'bound_fields': len(fields),
        'field_offset_deltas': [c[0]-t[0] for c, t in zip(fields, TARGET_FIELDS)],
        'literal_definitions': literals, 'ordered_calls': len(tc),
        'call_operands_agree': [x[1] for x in tc] == [x[1] for x in cc],
        'call_offsets_agree': tc == cc,
        'target_blocks': len(tg), 'candidate_blocks': len(cg),
        'direct_graphs_agree': tg == cg,
        'overlap_differences': len(differences), 'difference_offsets': differences,
        'missing_target_bytes': max(0, SIZE-len(raw)),
        'extra_candidate_bytes': max(0, len(raw)-SIZE),
        'complete_bytes_equal': target == linked,
    }


def guard(result):
    expected = {
        'diagnostic_only': True, 'exactness_credit': 'none',
        'target_bytes': SIZE, 'candidate_bytes': SIZE, 'target_sha256': TARGET_SHA256,
        'target_instructions': 380, 'candidate_instructions': 380,
        'original_fields': 53, 'bound_fields': 53, 'literal_definitions': 2,
        'ordered_calls': 23, 'call_operands_agree': True, 'call_offsets_agree': True,
        'target_blocks': 49, 'candidate_blocks': 49,
        'direct_graphs_agree': True, 'missing_target_bytes': 0,
        'extra_candidate_bytes': 0, 'field_offset_deltas': [0]*53,
    }
    if any(type(result.get(k)) is not type(v) or result[k] != v
           for k, v in expected.items()):
        raise ValueError('SaveReplay complete owner/census/frontier regressed')
    offsets, count = result.get('difference_offsets'), result.get('overlap_differences')
    if (type(count) is not int or not isinstance(offsets, list)
            or count != len(offsets) or offsets != sorted(set(offsets))
            or any(type(i) is not int or i not in RESIDUAL_OFFSETS for i in offsets)
            or result.get('complete_bytes_equal') is not (count == 0)):
        raise ValueError('SaveReplay mismatch outside the three reviewed residual bytes')


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('object', type=Path)
    parser.add_argument('--json', action='store_true')
    parser.add_argument('--guard', action='store_true', help='protect all 1235 equal bytes')
    args = parser.parse_args()
    try:
        result = diagnose(args.object)
        if args.guard:
            guard(result)
        if args.json:
            print(json.dumps(result, indent=2))
        else:
            print('NON-CREDITING SaveReplay:', result['candidate_bytes'], '/', SIZE,
                  'bytes;', result['overlap_differences'], 'differences; offsets',
                  ', '.join(hex(i) for i in result['difference_offsets']))
        return 0
    except (OSError, ValueError, KeyError, TypeError, ImportError, struct.error) as error:
        print('SaveReplay original-bound audit rejected:', error, file=sys.stderr)
        return 2


if __name__ == '__main__':
    raise SystemExit(main())
