#!/usr/bin/env python3
"""Source-cold, NON-CREDITING opcode-155 flag-semantics cross-check.

Decode the REAL 45-byte original and independently PE/COFF-linked RunEcl
candidate. Interpret only the fully reviewed flag-update instruction prefix;
reject unknown instructions or extra writes rather than assuming that a
byte-near source implements the original timeout-spell bit transition.
Full-owner field/code/table checks remain the responsibility of the existing
RunEcl inspector. An equivalent transition is never codegen exactness credit.
"""

from __future__ import annotations

import argparse
import importlib.util
import json
from pathlib import Path
import re
import sys

ROOT = Path(__file__).resolve().parents[1]
OWNER_BASE = 0x004086C0
OPCODE_OFFSET = 0x30B0
OPCODE_SIZE = 45
FLAGS_OFFSET = 0x337C
FLAG_MASK = 0x01000000
PREFIX_WRITE = 'dword ptr [edi + 0x337c]'


def operand(value: str) -> tuple:
    """Restrict our emulator to precisely the observed i386 operand family."""
    value = value.strip().lower()
    if value in ('eax', 'ecx', 'esi', 'cl'):
        return ('register', value)
    if re.fullmatch(r'0x[0-9a-f]+|[0-9]+', value):
        return ('immediate', int(value, 0))
    memory = re.fullmatch(r'(byte|dword) ptr \[(ebx|edi) \+ 0x([0-9a-f]+)\]', value)
    if memory:
        size, register, offset = memory.groups()
        location = (register, int(offset, 16))
        if (size, location) not in (('byte', ('ebx', 0xC)),
                                    ('dword', ('edi', FLAGS_OFFSET))):
            raise ValueError('opcode 155 uses an unreviewed memory operand: ' + value)
        return ('memory', size, location)
    raise ValueError('opcode 155 uses an unreviewed operand: ' + value)


def prefix(instructions: list) -> list[tuple]:
    """Extract all instructions up to the single real flag store, inclusively."""
    decoded = []
    flag_writes = 0
    for instruction in instructions:
        mnemonic = instruction.mnemonic.lower()
        if mnemonic not in ('mov', 'movzx', 'shl', 'xor', 'and'):
            raise ValueError('unknown opcode 155 flag-update instruction: ' + mnemonic)
        parts = instruction.op_str.lower().split(', ')
        if len(parts) != 2:
            raise ValueError('unknown opcode 155 operand count')
        dst, src = map(operand, parts)
        # Capstone normally rejects invalid mixed-width opcodes already, but
        # the pure-input tests also exercise this guard without a decoder.
        reg32 = lambda op: op[0] == 'register' and op[1] != 'cl'
        byte_read = lambda op: op == ('register', 'cl') or (op[0] == 'memory' and op[1] == 'byte')
        dword_read = lambda op: reg32(op) or op[0] == 'immediate' or (op[0] == 'memory' and op[1] == 'dword')
        if mnemonic == 'movzx':
            valid = reg32(dst) and byte_read(src)
        elif mnemonic == 'mov':
            valid = ((dst == ('register', 'cl') and (byte_read(src) or src[0] == 'immediate')) or
                     (reg32(dst) and dword_read(src)) or
                     (dst == ('memory', 'dword', ('edi', FLAGS_OFFSET)) and dword_read(src)))
        elif mnemonic == 'shl':
            valid = reg32(dst) and src[0] == 'immediate' and 0 <= src[1] < 32
        else:  # xor, and
            valid = reg32(dst) and (reg32(src) or src[0] == 'immediate')
        if not valid:
            raise ValueError('unknown opcode 155 operand widths')
        if dst[0] == 'memory':
            if dst != ('memory', 'dword', ('edi', FLAGS_OFFSET)) or mnemonic != 'mov':
                raise ValueError('opcode 155 writes outside its original flags word')
            flag_writes += 1
        decoded.append((mnemonic, dst, src))
        if flag_writes:
            break
    if flag_writes != 1 or not 4 <= len(decoded) <= 12:
        raise ValueError('opcode 155 lacks one complete reviewed flag store')
    return decoded


def emulate(instructions: list[tuple], previous: int, wire_byte: int) -> int:
    """Execute *decoded* x86 operations, not a C++-formula stand-in."""
    registers = {'eax': 0xA1B2C3D4, 'ecx': 0x76543210, 'esi': 0xF0E1D2C3}
    flags = previous & 0xFFFFFFFF
    def read(op: tuple) -> int:
        if op[0] == 'register':
            return registers['ecx'] & 0xFF if op[1] == 'cl' else registers[op[1]]
        if op[0] == 'immediate':
            return op[1]
        if op[0] == 'memory':
            return wire_byte if op[1] == 'byte' else flags
        raise ValueError('invalid opcode 155 operand')
    def write(op: tuple, value: int) -> None:
        nonlocal flags
        value &= 0xFFFFFFFF
        if op[0] == 'register':
            if op[1] == 'cl':
                registers['ecx'] = (registers['ecx'] & 0xFFFFFF00) | (value & 0xFF)
            else:
                registers[op[1]] = value
        elif op == ('memory', 'dword', ('edi', FLAGS_OFFSET)):
            flags = value
        else:
            raise ValueError('illegal opcode 155 memory/constant write')
    for mnemonic, dst, src in instructions:
        value = read(src)
        if mnemonic == 'movzx':
            if src != ('memory', 'byte', ('ebx', 0xC)) and src != ('register', 'cl'):
                raise ValueError('unknown movzx byte source')
            if dst[0] != 'register' or dst[1] == 'cl':
                raise ValueError('unknown movzx destination')
            write(dst, value & 0xFF)
        elif mnemonic == 'mov':
            write(dst, value)
        elif mnemonic == 'shl':
            write(dst, read(dst) << value)
        elif mnemonic == 'xor':
            write(dst, read(dst) ^ value)
        elif mnemonic == 'and':
            write(dst, read(dst) & value)
    return flags


def validate_transition(original: list[tuple], candidate: list[tuple]) -> int:
    """Exhaust the original wire byte and representative full 32-bit flags."""
    flags = (0, FLAG_MASK, ~FLAG_MASK & 0xFFFFFFFF, 0xFFFFFFFF,
             0x12345678, 0x89ABCDEF, 0x00FFFFFF, 0xFEFFFFFF,
             0x7F000000, 0x80000000, 0x01020304, 0xA5A5A5A5)
    cases = 0
    for value in range(256):
        for before in flags:
            # Independently reconstructed from TH09 bit-24 XOR/AND operands:
            # only bit 24 can change and its new value is (value & 1).
            expected = before ^ ((before ^ ((value << 24) & 0xFFFFFFFF)) & FLAG_MASK)
            actual_target = emulate(original, before, value)
            actual_candidate = emulate(candidate, before, value)
            if actual_target != expected or actual_candidate != actual_target:
                raise ValueError('opcode 155 original/candidate flag behavior differs for '
                                 f'wire={value:#x}, flags={before:#x}: target='
                                 f'{actual_target:#x}, candidate={actual_candidate:#x}, '
                                 f'expected={expected:#x}')
            cases += 1
    return cases


def audit(object_path: Path) -> dict:
    import capstone
    spec = importlib.util.spec_from_file_location('timeout_complete', ROOT/'scripts/inspect-ecl-complete.py')
    complete = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(complete)
    report = complete.report(object_path)
    if (report['target_owner_sha256'] !=
            'e9433504219119eba40ed4d11537782178135effeb6c95d74b6274e0e7f5e7ea'
            or report['physical_bytes'] != 15564 or report['relocations'] != 598 or
            report['field_different_bytes'] != 0 or report['table_different_bytes'] != 0):
        raise ValueError('opcode 155 requires the original full RunEcl PE/COFF owner')
    # Get the bytes independently, with the very same identity-checked PE/COFF
    # binding used by the full-owner inspector. No target-byte fitting is done.
    coff = complete.load_module('timeout_coff', 'compare-coff-function.py')
    identities = complete.load_module('timeout_ident', 'audit-ecl-callsite-identities.py').audit(object_path)
    if not identities['audit_passed']:
        raise ValueError('opcode 155 original operand identities are not attested')
    raw, rows = coff.object_function(object_path, complete.SYMBOL, include_symbol_locations=True)
    fields = {row['candidate_offset'] + 1: row['expected_destination']
              for row in identities['direct_calls']}
    fields.update({row['candidate_offset']: row['expected_value']
                   for row in identities['body_data_fields']})
    candidate, _ = complete.link_owner(raw, rows, fields)
    target = coff.pe_bytes_at(coff.verified_target(), OWNER_BASE, complete.SIZE)
    decoder = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32)
    starts = OWNER_BASE + OPCODE_OFFSET
    target_ins = list(decoder.disasm(bytes(target[OPCODE_OFFSET:OPCODE_OFFSET + OPCODE_SIZE]), starts))
    candidate_ins = list(decoder.disasm(bytes(candidate[OPCODE_OFFSET:OPCODE_OFFSET + OPCODE_SIZE]), starts))
    if (len(target_ins) != 12 or len(candidate_ins) != 12 or
            sum(ins.size for ins in target_ins) != OPCODE_SIZE or
            sum(ins.size for ins in candidate_ins) != OPCODE_SIZE):
        raise ValueError('opcode 155 physical window/12-instruction boundaries changed')
    target_prefix = prefix(target_ins)
    candidate_prefix = prefix(candidate_ins)
    if (target_prefix[-1][0] != 'mov' or candidate_prefix[-1][0] != 'mov' or
            sum(ins.size for ins in target_ins[:len(target_prefix)]) != 31 or
            sum(ins.size for ins in candidate_ins[:len(candidate_prefix)]) != 31):
        raise ValueError('opcode 155 complete original flag-store boundary changed')
    cases = validate_transition(target_prefix, candidate_prefix)
    return {'diagnostic_only': True, 'exactness_credit': 'none',
            'original_opcode_va': hex(starts), 'opcode_bytes': OPCODE_SIZE,
            'original_instructions': len(target_ins), 'candidate_instructions': len(candidate_ins),
            'original_flag_prefix_instructions': len(target_prefix),
            'candidate_flag_prefix_instructions': len(candidate_prefix),
            'flags_tested': 12, 'wire_values_tested': 256,
            'behavior_cases': cases, 'flag_transition_equal': True,
            'full_owner_original_sha256': report['target_owner_sha256'],
            'full_owner_ordinary_differences': report['ordinary_different_bytes']}


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('object', type=Path)
    args = parser.parse_args()
    try:
        print(json.dumps(audit(args.object), indent=2))
        return 0
    except (OSError, KeyError, ValueError, ImportError) as error:
        print(json.dumps({'result':'error', 'message':str(error)}), file=sys.stderr)
        return 1


if __name__ == '__main__':
    raise SystemExit(main())
