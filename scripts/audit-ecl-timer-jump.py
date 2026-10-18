#!/usr/bin/env python3
"""Original-PE/full-COFF/noncrediting RunEcl opcode-4 timer call audit.

Interpret the actual 29-byte TH09 jump-handler instructions, not a C++
formula or masked similarity window. The original and current compiler must
pass the same raw signed/unsigned instruction-time value by stack argument to
the actual ZunTimer::operator=(int) on active-context +0x08 and publish the
same signed relative bytecode advance. Exact codegen requires a *separate*
complete, cold source/target owner equality check; this never awards credit.
"""

from __future__ import annotations

import argparse
from collections import Counter
import importlib.util
import json
from pathlib import Path
import re
import sys

ROOT = Path(__file__).resolve().parents[1]
OWNER_BASE = 0x004086C0
OPCODE_OFFSET = 0x0277
OPCODE_SIZE = 29
OPCODE_VA = OWNER_BASE + OPCODE_OFFSET
TIMER_ASSIGN = 0x00401500
REDISPATCH_VA = 0x00408750
TARGET_OWNER_SHA = 'e9433504219119eba40ed4d11537782178135effeb6c95d74b6274e0e7f5e7ea'
MASK = 0xFFFFFFFF


# The emulator is deliberately smaller than general i386. Unknown opcodes,
# registers, memory aliases, width changes, calls or branch destinations fail.
REGISTERS = ('eax', 'ebx', 'ecx', 'edi', 'ebp')
MEMORY = {
    'dword ptr [ebx + 0xc]': ('ebx', 0xC),
    'dword ptr [ebx + 0x10]': ('ebx', 0x10),
    'dword ptr [edi + 0x2ce0]': ('edi', 0x2CE0),
    'dword ptr [ebp - 8]': ('ebp', -8),
}


def operand(value: str) -> tuple:
    value = value.strip().lower()
    if value in REGISTERS:
        return ('register', value)
    if value in MEMORY:
        return ('memory',) + MEMORY[value]
    if re.fullmatch(r'(?:0x[0-9a-f]+|[0-9]+)', value):
        return ('immediate', int(value, 0))
    raise ValueError('unreviewed ECL opcode-4 operand: ' + value)


def parsed_instructions(instructions: list) -> list[tuple]:
    """Decode only observed legal register/memory operations and direct flow."""
    if len(instructions) != 8:
        raise ValueError('opcode 4 must contain the eight reviewed instructions')
    result = []
    for ins in instructions:
        mnemonic = ins.mnemonic.lower()
        if mnemonic not in ('mov', 'push', 'add', 'call', 'jmp'):
            raise ValueError('unreviewed ECL jump instruction: ' + mnemonic)
        parts = ins.op_str.lower().split(', ')
        if len(parts) != (2 if mnemonic in ('mov', 'add') else 1):
            raise ValueError('opcode-4 instruction argument count differs')
        ops = tuple(operand(s) for s in parts)
        if mnemonic == 'mov':
            if ops[0][0] not in ('register', 'memory') or ops[1][0] == 'memory' and ops[0][0] == 'memory':
                raise ValueError('unreviewed opcode-4 move destination/width')
            if ops[0][0] == 'memory' and ops[0] != ('memory', 'ebp', -8):
                raise ValueError('opcode-4 writes an unexpected game field')
        if mnemonic == 'add' and ops[0][0] != 'register':
            raise ValueError('opcode-4 alters unexpected memory/flags')
        if mnemonic == 'push' and ops[0][0] not in ('register', 'immediate'):
            raise ValueError('opcode-4 passes an unreviewed value')
        if mnemonic == 'call' and ops != (('immediate', TIMER_ASSIGN),):
            raise ValueError('opcode-4 calls an incorrect timer target')
        if mnemonic == 'jmp' and ops != (('immediate', REDISPATCH_VA),):
            raise ValueError('opcode-4 changes the ECL redispatch target')
        result.append((mnemonic, *ops))
    if [x[0] for x in result].count('call') != 1 or result[-1][0] != 'jmp':
        raise ValueError('opcode-4 call/exit inventory changed')
    return result


def execute(ops: list[tuple], *, time: int, instruction_offset: int,
            context_address: int) -> dict:
    """Execute only real reviewed opcode-4 instructions before/after call."""
    regs = {'eax': 0xAABBAABB, 'ebx': 0x51001000,
            'ecx': 0x3EDC0FF0, 'edi': 0x62001000, 'ebp': 0x73001000}
    old_instruction = regs['ebx']
    mem = {regs['ebx'] + 0xC: time & MASK,
           regs['ebx'] + 0x10: instruction_offset & MASK,
           regs['edi'] + 0x2CE0: context_address & MASK,
           regs['ebp'] - 8: 0xBEEFBEEF}
    records = Counter()
    pushed, call_records, next_destination = [], [], None

    def read(op: tuple) -> int:
        if op[0] == 'register':
            return regs[op[1]]
        if op[0] == 'immediate':
            return op[1]
        if op[0] == 'memory':
            base, offset = op[1:]
            address = (regs[base] + offset) & MASK
            if address not in mem:
                raise ValueError('opcode-4 accessed an unexpected memory address')
            records[('read', address)] += 1
            return mem[address]
        raise ValueError('unknown opcode-4 source')

    def write(op: tuple, value: int) -> None:
        value &= MASK
        if op[0] == 'register':
            regs[op[1]] = value
        elif op == ('memory', 'ebp', -8):
            address = (regs['ebp'] - 8) & MASK
            records[('write', address)] += 1
            mem[address] = value
        else:
            raise ValueError('opcode-4 wrote an unsupported memory field')

    for index, ins in enumerate(ops):
        mnemonic = ins[0]
        if mnemonic == 'mov':
            write(ins[1], read(ins[2]))
        elif mnemonic == 'add':
            write(ins[1], read(ins[1]) + read(ins[2]))
        elif mnemonic == 'push':
            pushed.append(read(ins[1]))
        elif mnemonic == 'call':
            if index != 4 or len(pushed) != 1:
                raise ValueError('opcode-4 timer call has unexpected stack argument')
            call_records.append((ins[1][1], regs['ecx'], pushed.pop()))
            # The real canonical thiscall modifies timer storage but cannot
            # change the protected EBX/EDI/EBP pointer bookkeeping contract.
            regs['eax'] = 0xDEADBEEF
            regs['ecx'] = 0xC0DEC0DE
        elif mnemonic == 'jmp':
            if index != len(ops) - 1:
                raise ValueError('opcode-4 has an early/extra jump')
            next_destination = ins[1][1]
    expected_memory = Counter({
        ('read', old_instruction + 0xC): 1,
        ('read', old_instruction + 0x10): 1,
        ('read', 0x62001000 + 0x2CE0): 1,
        ('write', 0x73001000 - 8): 1,
    })
    if records != expected_memory or pushed or next_destination != REDISPATCH_VA:
        raise ValueError('opcode-4 altered its reviewed read/write/branch footprint')
    if mem[0x73001000 - 8] != ((old_instruction + instruction_offset) & MASK):
        raise ValueError('opcode-4 publishes a different next instruction')
    if len(call_records) != 1:
        raise ValueError('opcode-4 omitted or duplicated timer assignment')
    return {'call': call_records[0], 'next_instruction': mem[0x73001000 - 8],
            'branch': next_destination}


def validate_transitions(original: list[tuple], candidate: list[tuple]) -> int:
    times = [0, 1, -1, 0x7FFFFFFF, 0x80000000, 0x12345678]
    offsets = [0, 0xC, 0x24, -0xC, 0x7000]
    contexts = [0x40001000, 0x50001000, 0x70001000]
    count = 0
    for time in times:
        for relative in offsets:
            for context in contexts:
                expected = {'call': (TIMER_ASSIGN, (context + 8) & MASK, time & MASK),
                            'next_instruction': (0x51001000 + relative) & MASK,
                            'branch': REDISPATCH_VA}
                if (execute(original, time=time, instruction_offset=relative,
                            context_address=context) != expected or
                        execute(candidate, time=time, instruction_offset=relative,
                                context_address=context) != expected):
                    raise ValueError('opcode-4 original/source timer or ECL pointer behavior differs')
                count += 1
    return count


def load_module(name: str, filename: str):
    sp = importlib.util.spec_from_file_location(name, ROOT / 'scripts' / filename)
    module = importlib.util.module_from_spec(sp)
    sp.loader.exec_module(module)
    return module


def audit(object_path: Path) -> dict:
    import capstone
    complete = load_module('runecl_timer_full', 'inspect-ecl-complete.py')
    report = complete.report(object_path)
    if (report['target_owner_sha256'] != TARGET_OWNER_SHA or
            report['physical_bytes'] != 15564 or report['relocations'] != 598 or
            report['field_different_bytes'] != 0 or report['table_different_bytes'] != 0 or
            report['identity_audit_passed'] is not True):
        raise ValueError('opcode-4 audit lacks original-bound whole RunEcl owner')
    coff = load_module('runecl_timer_coff', 'compare-coff-function.py')
    ident = load_module('runecl_timer_identity', 'audit-ecl-callsite-identities.py').audit(object_path)
    if not ident['audit_passed']:
        raise ValueError('opcode-4 has unverified full-owner operand identities')
    raw, rows = coff.object_function(object_path, complete.SYMBOL, include_symbol_locations=True)
    links = {field['candidate_offset'] + 1: field['expected_destination']
             for field in ident['direct_calls']}
    links.update({field['candidate_offset']: field['expected_value']
                  for field in ident['body_data_fields']})
    linked, _ = complete.link_owner(raw, rows, links)
    target = coff.pe_bytes_at(coff.verified_target(), OWNER_BASE, complete.SIZE)
    decoded = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32)
    windows = []
    for blob in (target, linked):
        code = bytes(blob[OPCODE_OFFSET:OPCODE_OFFSET + OPCODE_SIZE])
        instructions = list(decoded.disasm(code, OPCODE_VA))
        if (len(instructions) != 8 or sum(x.size for x in instructions) != OPCODE_SIZE):
            raise ValueError('opcode-4 physical 29-byte / 8-instruction window drifted')
        windows.append(parsed_instructions(instructions))
    cases = validate_transitions(*windows)
    return {'diagnostic_only': True, 'exactness_credit': 'none',
            'full_owner_original_sha256': report['target_owner_sha256'],
            'full_owner_ordinary_differences': report['ordinary_different_bytes'],
            'original_opcode_va': hex(OPCODE_VA), 'opcode_bytes': OPCODE_SIZE,
            'original_instructions': 8, 'candidate_instructions': 8,
            'timer_call_va': hex(TIMER_ASSIGN), 'redispatch_va': hex(REDISPATCH_VA),
            'time_values_tested': 6, 'offsets_tested': 5, 'contexts_tested': 3,
            'behavior_cases': cases, 'timer_and_advance_equal': True}


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('object', type=Path)
    args = parser.parse_args()
    try:
        print(json.dumps(audit(args.object), indent=2))
        return 0
    except (OSError, ValueError, KeyError, ImportError) as err:
        print(json.dumps({'result': 'error', 'message': str(err)}), file=sys.stderr)
        return 1


if __name__ == '__main__':
    raise SystemExit(main())
