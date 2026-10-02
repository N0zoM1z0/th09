#!/usr/bin/env python3
"""Target-bound boundary diagnostic for the previously uncensused slot zero.

This checks the bounded owner and its immediate neighbors, not census
completeness, original source ownership, runtime behavior or codegen exactness.
Use the separate canonical match unit for compiler acceptance.
"""
import argparse
import capstone
import csv
import hashlib
import importlib.util
import json
from pathlib import Path
import struct

ROOT = Path(__file__).resolve().parents[1]
START, SIZE, TABLE = 0x4412E0, 101, 0x4A19D0


def require(condition, message):
    if not condition:
        raise ValueError(message)


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--instructions', action='store_true')
    args = parser.parse_args()
    spec = importlib.util.spec_from_file_location(
        'slot0_coff', ROOT / 'scripts/compare-coff-function.py')
    coff = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(coff)
    pe = coff.verified_target()
    decoder = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32)
    decoder.detail = True

    def decode(address, size):
        blob = coff.pe_bytes_at(pe, address, size)
        instructions = list(decoder.disasm(blob, address))
        require(sum(i.size for i in instructions) == size, 'incomplete owner decoding')
        positions = {i.address for i in instructions}
        for i in instructions:
            if i.mnemonic.startswith('j'):
                require(i.operands[0].type == capstone.x86.X86_OP_IMM
                        and i.operands[0].imm in positions,
                        'unreviewed external or indirect owner branch')
        require(instructions[-1].mnemonic == 'ret', 'owner does not end at return')
        return blob, instructions

    # Neighbor extents are independently tracked; do not inherit their ABI.
    decode(0x441100, 475)
    decode(0x441350, 75)
    require(coff.pe_bytes_at(pe, 0x4412DB, 5) == b'\xCC' * 5,
            'predecessor separator differs')
    require(coff.pe_bytes_at(pe, START + SIZE, 11) == b'\xCC' * 11,
            'following separator differs')
    blob, instructions = decode(START, SIZE)
    returns = [i for i in instructions if i.mnemonic == 'ret']
    require(len(returns) == 1 and not returns[0].operands,
            'unexpected return cleanup')
    calls = [i for i in instructions if i.mnemonic == 'call']
    require(len(calls) == 1 and calls[0].operands[0].type == capstone.x86.X86_OP_IMM
            and calls[0].operands[0].imm == 0x4151F0, 'unexpected call boundary')
    with (ROOT / 'config/functions.csv').open(newline='', encoding='utf-8') as stream:
        rows = list(csv.DictReader(stream))
    by_address = {int(r['address'], 0): r for r in rows}
    require(all(a in by_address and int(by_address[a]['size']) == n
                for a, n in [(0x441100, 475), (0x441350, 75)]),
            'neighbor ledger extents differ')
    overlaps = [r for r in rows if int(r['address'], 0) <= START + SIZE - 1
                and int(r['span_end'], 0) >= START]
    require(not overlaps or (len(overlaps) == 1
            and int(overlaps[0]['address'], 0) == START
            and int(overlaps[0]['size']) == SIZE), 'conflicting ledger ownership')
    entries = struct.unpack('<16I', coff.pe_bytes_at(pe, TABLE, 64))
    require(entries[0] == START and len(set(entries)) == 16, 'unexpected callback table')
    require(all(a in by_address and by_address[a]['owner'] == 'authored'
                for a in entries[1:]), 'other callback slots lack reviewed authored entries')
    needle = struct.pack('<I', START)
    occurrences = [offset for offset in range(len(pe)) if pe.startswith(needle, offset)]
    require(len(occurrences) == 1, 'additional raw pointer occurrence needs review')
    print(json.dumps({
        'diagnostic_only': True, 'exactness_credit': 'none',
        'census_completeness': 'not evaluated',
        'address': hex(START), 'code_bytes': SIZE,
        'instructions': len(instructions), 'stack_arguments': 0,
        'target_body_sha256': hashlib.sha256(blob).hexdigest(),
        'table': hex(TABLE), 'table_entries': [hex(a) for a in entries],
        'branches': [{'address': hex(i.address), 'kind': i.mnemonic,
                      'destination': hex(i.operands[0].imm)}
                     for i in instructions if i.mnemonic.startswith('j')],
        'single_call': hex(calls[0].operands[0].imm),
        'unowned_separator_bytes': [5, 11],
        'tracked_slot0': bool(overlaps),
        'raw_pointer_file_offsets': [hex(a) for a in occurrences],
        'limitations': 'Raw pointer scan is not an exhaustive runtime consumer audit; '
                       'IDA function creation and original TU ownership are independent.',
    }, indent=2))
    if args.instructions:
        for i in instructions:
            print(hex(i.address), i.mnemonic, i.op_str)


if __name__ == '__main__':
    main()
