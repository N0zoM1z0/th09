#!/usr/bin/env python3
"""Complete original-bound Player charge-update diagnostic; no exactness credit.

All 1,210 original bytes, 70 operands and 58 calls are checked independently of
seven older region checks. An object alone does not attest its source/build.
The fixed referents are reviewed TH09 boundaries, not unique C++ owner claims.
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
BASE, SIZE = 0x0041F810, 1210
SYMBOL = '?UpdateChargeAttack@PlayerChargeAttackView@@QAEHXZ'
TARGET_SHA256 = '6c24be1a012fc3fa6148bd09954443318cc9db33d6b69a26ca1a778dfe6e5015'
TARGET_BLOCKS = 53

REFERENTS = {
    '??4ZunTimer@@QAEXH@Z': 0x00401500,
    '??8ZunTimer@@QAEHH@Z': 0x00404950,
    '??BZunTimer@@QAEHXZ': 0x00435F00,
    '??BZunTimer@@QAEMXZ': 0x004014F0,
    '??EZunTimer@@QAEXH@Z': 0x00401510,
    '??FZunTimer@@QAEXH@Z': 0x00406670,
    '??MZunTimer@@QAEIH@Z': 0x00401540,
    '??OZunTimer@@QAEIH@Z': 0x00403DE0,
    '??PZunTimer@@QAEIH@Z': 0x00401520,
    '??ZZunTimer@@QAEXH@Z': 0x0041F300,
    '?ActivateMeterVms@FrontSide@@QAEXXZ': 0x0041A270,
    '?ConfigureState3Effect13@PlayerStateEffectView@@QAEPAUEffect@@XZ': 0x00403C30,
    '?ConfigureState3Effect14@PlayerStateEffectView@@QAEPAUEffect@@XZ': 0x00403C90,
    '?ConfigureState3Effect15@PlayerStateEffectView@@QAEPAUEffect@@XZ': 0x00403CF0,
    '?DeactivateMeterVms@FrontSide@@QAEXXZ': 0x0041A2D0,
    '?FindActiveEnemyBySideCategory@EnemyManagerView@@QAEPAUEnemyView@@H@Z': 0x0040F7D0,
    '?HasTicked@ZunTimer@@QAEHXZ': 0x0040F810,
    '?IsHeld@ReplayInputState@@QAEGG@Z': 0x00415CB0,
    '?PlaySoundByIdx@SoundPlayer@@QAEXHH@Z': 0x0043E2F0,
    '?SpawnShots@PlayerSpawnShotsView@@QAIXHH@Z': 0x0041F4C0,
    '?StartAttack@CardAttackStartView@@QAEXHHHPBD@Z': 0x00403E50,
    '?SubtractResourceClamped@PlayerResourceSubtractView@@QAEXM@Z': 0x0041F310,
    '?WasPressed@ReplayInputState@@QAEGG@Z': 0x00415CD0,
    '?g_GameManager@@3UAsciiGameManagerView@@A': 0x004A7D90,
    '?g_ReplayInputStates@@3PAUReplayInputState@@A': 0x004ACE18,
    '?g_SoundPlayer@@3VSoundPlayer@@A': 0x004DC698,
    '?g_Supervisor@@3VSupervisor@@A': 0x004B3100,
    '__ftol2': 0x0047B1D4,
    '__real@00000000': 0x0048E314,
    '__real@42c80000': 0x0048E3C8,
    '__real@43480000': 0x0048EEFC,
    '__real@43960000': 0x0048EF00,
    '__real@43c80000': 0x0048E3C4,
}

# Decoded independently from the complete SHA-pinned original instruction stream.
TARGET_FIELDS = (
    (0x016, 'DIR32', 0x004ACE18),
    (0x01F, 'REL32', 0x00415CD0),
    (0x02A, 'DIR32', 0x004A7E78),
    (0x03D, 'REL32', 0x00415CB0),
    (0x048, 'DIR32', 0x004A7E78),
    (0x063, 'REL32', 0x00415CB0),
    (0x075, 'REL32', 0x00415CB0),
    (0x08D, 'REL32', 0x00404950),
    (0x0A0, 'REL32', 0x00401540),
    (0x0AD, 'REL32', 0x00401500),
    (0x0B6, 'REL32', 0x00401520),
    (0x0C7, 'REL32', 0x00401520),
    (0x0D4, 'REL32', 0x0041F300),
    (0x0DD, 'REL32', 0x00401500),
    (0x0EA, 'REL32', 0x00401500),
    (0x0F3, 'REL32', 0x00403DE0),
    (0x100, 'REL32', 0x00406670),
    (0x109, 'DIR32', 0x004A7E78),
    (0x123, 'DIR32', 0x0048E3C8),
    (0x14A, 'REL32', 0x00401500),
    (0x159, 'REL32', 0x00401500),
    (0x165, 'DIR32', 0x0048E3C4),
    (0x17D, 'REL32', 0x0040F7D0),
    (0x1A2, 'REL32', 0x00403E50),
    (0x1AE, 'REL32', 0x0041F310),
    (0x1B7, 'REL32', 0x00401500),
    (0x1BE, 'REL32', 0x00403CF0),
    (0x1D2, 'REL32', 0x00415CB0),
    (0x1EB, 'REL32', 0x00415CB0),
    (0x203, 'REL32', 0x00401510),
    (0x20C, 'REL32', 0x00401520),
    (0x227, 'DIR32', 0x0048E314),
    (0x251, 'DIR32', 0x004DC698),
    (0x256, 'REL32', 0x0043E2F0),
    (0x261, 'REL32', 0x0041A270),
    (0x26C, 'REL32', 0x0047B1D4),
    (0x272, 'DIR32', 0x004B36B8),
    (0x2C2, 'REL32', 0x00401510),
    (0x2CE, 'REL32', 0x00401520),
    (0x2E3, 'REL32', 0x00401500),
    (0x2EE, 'REL32', 0x0047B1D4),
    (0x324, 'DIR32', 0x004DC698),
    (0x329, 'REL32', 0x0043E2F0),
    (0x33A, 'DIR32', 0x0048EF00),
    (0x35C, 'REL32', 0x00403E50),
    (0x368, 'REL32', 0x0041F310),
    (0x371, 'REL32', 0x00401500),
    (0x378, 'REL32', 0x00403C90),
    (0x386, 'DIR32', 0x0048EEFC),
    (0x3A8, 'REL32', 0x00403E50),
    (0x3B1, 'REL32', 0x00401500),
    (0x3BD, 'REL32', 0x0041F310),
    (0x3C4, 'REL32', 0x00403C30),
    (0x3CF, 'REL32', 0x0041A2D0),
    (0x3DC, 'REL32', 0x00401500),
    (0x3F5, 'REL32', 0x00403DE0),
    (0x402, 'REL32', 0x00406670),
    (0x418, 'REL32', 0x0040F810),
    (0x425, 'REL32', 0x00435F00),
    (0x42E, 'REL32', 0x0041F4C0),
    (0x437, 'REL32', 0x00401510),
    (0x43E, 'REL32', 0x004014F0),
    (0x457, 'REL32', 0x00401500),
    (0x46D, 'REL32', 0x00401520),
    (0x478, 'REL32', 0x0040F810),
    (0x485, 'REL32', 0x00435F00),
    (0x48E, 'REL32', 0x0041F4C0),
    (0x497, 'REL32', 0x00401510),
    (0x4A0, 'REL32', 0x00401520),
    (0x4AD, 'REL32', 0x00401500),
)


def module(name, filename):
    spec = importlib.util.spec_from_file_location(name, ROOT / 'scripts' / filename)
    result = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(result)
    return result


def require_original(target):
    if len(target) != SIZE or hashlib.sha256(target).hexdigest() != TARGET_SHA256:
        raise ValueError('charge original full-owner SHA/extent changed')


def decode(data):
    import capstone
    decoder = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32)
    decoder.detail = True
    instructions = list(decoder.disasm(data, BASE))
    if (not instructions or sum(i.size for i in instructions) != len(data) or
            instructions[-1].mnemonic != 'ret' or
            any(i.operands for i in instructions if i.mnemonic == 'ret')):
        raise ValueError('charge code is incomplete or changes the plain-ret ABI')
    return instructions


def address_fields(instructions):
    import capstone
    fields = []
    for instruction in instructions:
        offset = instruction.address - BASE
        if instruction.mnemonic.startswith('j'):
            continue
        if instruction.mnemonic == 'call':
            if (len(instruction.operands) != 1 or instruction.imm_size != 4 or
                    instruction.operands[0].type != capstone.x86.X86_OP_IMM):
                raise ValueError('charge has a nonoriginal indirect/short call')
            fields.append((offset + instruction.imm_offset, 'REL32',
                           instruction.operands[0].imm))
            continue
        for operand in instruction.operands:
            if (operand.type == capstone.x86.X86_OP_MEM and instruction.disp_size == 4
                    and 0x400000 <= operand.mem.disp < 0x4E7000):
                fields.append((offset + instruction.disp_offset, 'DIR32', operand.mem.disp))
            elif (operand.type == capstone.x86.X86_OP_IMM and instruction.imm_size == 4
                  and 0x400000 <= operand.imm < 0x4E7000):
                fields.append((offset + instruction.imm_offset, 'DIR32', operand.imm))
    return fields


def source_addend(symbol):
    if symbol == '?g_GameManager@@3UAsciiGameManagerView@@A':
        return 0xE8
    if symbol == '?g_Supervisor@@3VSupervisor@@A':
        return 0x5B8
    return 0


def bind_candidate(raw, rows, target):
    require_original(target)
    if not 0 < len(raw) <= SIZE * 2 or len(rows) != len(TARGET_FIELDS):
        raise ValueError('charge requires the complete 70-field candidate')
    linked = bytearray(raw)
    end = 0
    for row, (original_offset, kind, destination) in zip(rows, TARGET_FIELDS):
        symbol, offset = row['symbol'], row['offset']
        if (symbol not in REFERENTS or row['type'] != kind or
                row['type_id'] != {'DIR32': 6, 'REL32': 20}[kind] or
                type(row['addend']) is not int or
                row['addend'] != source_addend(symbol) or
                type(offset) is not int or offset < end or offset + 4 > len(raw)):
            raise ValueError('charge field symbol/kind/addend/order/range changed')
        effective = REFERENTS[symbol] + row['addend']
        if effective != destination:
            raise ValueError('charge source changes an independent original referent')
        if struct.unpack_from('<I', raw, offset)[0] != row['addend']:
            raise ValueError('charge raw field and actual COFF addend disagree')
        expected = destination - (BASE + original_offset + 4 if kind == 'REL32' else 0)
        if struct.unpack_from('<I', target, original_offset)[0] != expected & 0xFFFFFFFF:
            raise ValueError('charge original operand encoding changed')
        value = effective - (BASE + offset + 4 if kind == 'REL32' else 0)
        struct.pack_into('<I', linked, offset, value & 0xFFFFFFFF)
        end = offset + 4
    return bytes(linked)


def literal_payload(data, row):
    """Read the actual four-byte literal definition, never just its name."""
    if len(data) < 20:
        raise ValueError('truncated charge COFF header')
    machine, count = struct.unpack_from('<HH', data)
    optional = struct.unpack_from('<H', data, 16)[0]
    section = row['symbol_section']
    value = row['symbol_value']
    if (machine != 0x14C or optional != 0 or type(section) is not int or
            not 1 <= section <= count or type(value) is not int or value < 0):
        raise ValueError('charge literal lacks an actual i386 COFF definition')
    header = 20 + (section - 1) * 40
    if header + 40 > len(data):
        raise ValueError('truncated charge COFF section header')
    size, start = struct.unpack_from('<II', data, header + 16)
    if (start < 20 + count * 40 or value + 4 > size or
            start + size > len(data)):
        raise ValueError('charge literal definition leaves its actual section')
    return data[start + value:start + value + 4]


def diagnose(path):
    import capstone
    direct_control_flow = module(
        'charge_complete_graph', 'diagnostic_branch_alignment.py').direct_control_flow
    coff = module('charge_full_coff', 'compare-coff-function.py')
    image = coff.verified_target()
    target = coff.pe_bytes_at(image, BASE, SIZE)
    require_original(target)
    original = decode(target)
    if len(original) != 350 or tuple(address_fields(original)) != TARGET_FIELDS:
        raise ValueError('charge original instruction/operand inventory changed')
    raw, rows = coff.object_function(path, SYMBOL, include_symbol_locations=True)
    linked = bind_candidate(raw, rows, target)
    candidate = decode(linked)
    actual_fields = [(row['offset'], kind, dest)
                     for row, (_, kind, dest) in zip(rows, TARGET_FIELDS)]
    if address_fields(candidate) != actual_fields:
        raise ValueError('charge actual COFF operands do not decode at their claimed positions')
    object_data = Path(path).read_bytes()
    literals = []
    for row in rows:
        if row['symbol'].startswith('__real@'):
            expected = struct.pack('<I', int(row['symbol'][7:], 16))
            if (literal_payload(object_data, row) != expected or
                    coff.pe_bytes_at(image, REFERENTS[row['symbol']], 4) != expected):
                raise ValueError('charge actual source/original literal payload differs')
            literals.append(row['symbol'])
    def calls(instructions):
        return [i.operands[0].imm for i in instructions if i.mnemonic == 'call']
    if len(calls(original)) != 58 or calls(original) != calls(candidate):
        raise ValueError('charge ordered original 58-call inventory changed')
    original_graph = direct_control_flow(original, capstone.x86.X86_OP_IMM)
    candidate_graph = direct_control_flow(candidate, capstone.x86.X86_OP_IMM)
    if len(original_graph) != TARGET_BLOCKS:
        raise ValueError('charge original complete graph changed')
    differences = [i for i, (a, b) in enumerate(zip(target, linked)) if a != b]
    def stack_byte_accesses(instructions):
        return [{'offset': hex(i.address - BASE), 'mnemonic': i.mnemonic,
                 'operands': i.op_str}
                for i in instructions if any(
                    op.type == capstone.x86.X86_OP_MEM and op.size == 1 and
                    op.mem.base == capstone.x86.X86_REG_EBP and op.mem.disp == -1
                    for op in i.operands)]
    return {
        'diagnostic_only': True, 'exactness_credit': 'none',
        'target_bytes': SIZE, 'candidate_bytes': len(raw),
        'target_sha256': TARGET_SHA256,
        'candidate_raw_sha256': hashlib.sha256(raw).hexdigest(),
        'candidate_linked_sha256': hashlib.sha256(linked).hexdigest(),
        'target_instructions': len(original), 'candidate_instructions': len(candidate),
        'original_fields': 70, 'bound_fields': len(rows),
        'ordered_calls': 58, 'ordered_calls_agree': True,
        'target_blocks': len(original_graph), 'candidate_blocks': len(candidate_graph),
        'direct_graphs_agree': original_graph == candidate_graph,
        'overlap_differences': len(differences), 'difference_offsets': differences,
        'missing_target_bytes': max(0, SIZE - len(raw)),
        'extra_candidate_bytes': max(0, len(raw) - SIZE),
        'complete_bytes_equal': target == linked,
        'literal_definitions_checked': len(literals),
        'original_stack_byte_minus_one': stack_byte_accesses(original),
        'candidate_stack_byte_minus_one': stack_byte_accesses(candidate),
        'bindings': rows,
    }


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('object', type=Path)
    args = parser.parse_args()
    try:
        print(json.dumps(diagnose(args.object), indent=2))
        return 0
    except (OSError, ValueError, KeyError, ImportError, struct.error) as error:
        print('charge complete-owner input rejected:', error, file=sys.stderr)
        return 2


if __name__ == '__main__':
    raise SystemExit(main())
