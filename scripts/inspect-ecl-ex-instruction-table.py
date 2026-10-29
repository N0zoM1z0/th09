#!/usr/bin/env python3
"""Compare the twelve reviewed callable table slots, without code exactness credit.

The next original zero DWORD has unresolved ownership. This verifies a bounded
48-byte prefix and its native source identities, not an original array extent,
unique nominal ABI spelling, native product link or handler-body exactness.
"""
import argparse
import hashlib
import importlib.util
import json
from pathlib import Path
import struct
import subprocess
import os
import tomllib

ROOT = Path(__file__).resolve().parents[1]
BASE = 0x4A0B14
SIZE = 48
SYMBOL = '?g_EnemyEclExInstructions@@3PAP6IXPAUEnemyView@@PAUEnemyEclExInstructionView@@@ZA'
SLOTS = (
    (0x40C3C0, 'enemy-spawn-exattack-type1'),
    (0x40C3F0, 'enemy-ecl-opcode1'),
    (0x40C530, 'enemy-ecl-opcode2'),
    (0x40C440, 'enemy-spawn-exattack-type7'),
    (0x40C4A0, 'enemy-spawn-exattack-type9'),
    (0x40C470, 'enemy-spawn-exattack-type8'),
    (0x40C770, 'enemy-spawn-exattack-type10'),
    (0x40C7C0, 'enemy-spawn-exattack-type11'),
    (0x40C810, 'enemy-spawn-exattack-type12'),
    (0x40C860, 'enemy-spawn-exattack-type22-burst'),
    (0x40C4D0, 'enemy-spawn-exattack-type23'),
    (0x40C500, 'enemy-spawn-exattack-type24'),
)
OBJECT = 'build/matching/EnemyExAttackLeaves.obj'


def require(condition, message):
    if not condition:
        raise ValueError(message)


def callback_symbol(index):
    return '?EnemyEclExInstruction%d@@YIXPAUEnemyView@@PAUEnemyEclExInstructionView@@@Z' % index


def read_definition(data, coff):
    """Read the actual initialized data definition and all native COFF fields."""
    require(len(data) >= 20, 'truncated table COFF header')
    machine, count, _, start, nsyms, optional, _ = struct.unpack_from('<HHIIIHH', data)
    require(machine == 0x14C and optional == 0 and count > 0 and
            20 + 40*count <= len(data) and start >= 20 + 40*count and
            start + 18*nsyms + 4 <= len(data), 'invalid table COFF layout')
    strings_start = start + 18*nsyms
    strings_size = struct.unpack_from('<I', data, strings_start)[0]
    require(4 <= strings_size <= len(data) - strings_start, 'truncated table string data')
    strings = data[strings_start:strings_start+strings_size]
    symbols = {}; index = 0
    while index < nsyms:
        name, value, section, kind, storage, aux = struct.unpack_from('<8sIhHBB', data, start+18*index)
        require(index + aux < nsyms, 'truncated table auxiliary symbols')
        symbols[index] = dict(symbol=coff.coff_name(name, strings), value=value,
                              section=section, kind=kind, storage=storage)
        index += 1 + aux
    definitions = [s for s in symbols.values() if s['symbol'] == SYMBOL and s['section'] > 0]
    require(len(definitions) == 1, 'expected one defined native ECL table')
    symbol = definitions[0]
    require(symbol['storage'] == 2 and symbol['kind'] == 0 and
            1 <= symbol['section'] <= count, 'native table is not external data')
    _, _, _, size, raw, reloc, _, nr, _, flags = struct.unpack_from(
        '<8sIIIIIIHHI', data, 20+40*(symbol['section']-1))
    require(flags & 0x40 and flags & 0x40000000 and flags & 0x80000000 and
            not flags & 0x20 and raw >= 20 + 40*count and
            raw + size <= start and symbol['value'] + SIZE == size and
            reloc >= raw + size and reloc + nr*10 <= start,
            'native table lacks its complete initialized writable data extent')
    payload = data[raw+symbol['value']:raw+size]
    fields = []
    for i in range(nr):
        offset, target, kind = struct.unpack_from('<IIH', data, reloc+10*i)
        offset -= symbol['value']
        require(0 <= offset <= SIZE-4 and target in symbols,
                'table field leaves the data definition')
        target_symbol = symbols[target]
        fields.append(dict(offset=offset, type_id=kind, symbol=target_symbol['symbol'],
                           addend=struct.unpack_from('<I', payload, offset)[0],
                           symbol_section=target_symbol['section'],
                           symbol_value=target_symbol['value'],
                           symbol_type=target_symbol['kind'], storage=target_symbol['storage']))
    return payload, fields


def validate_binding(payload, fields, units, original):
    require(len(payload) == len(original) == SIZE and len(fields) == 12,
            'incomplete ECL callback prefix or relocation census')
    require(struct.unpack('<12I', original) == tuple(address for address, _ in SLOTS),
            'original ECL callback order/destinations changed')
    rows = sorted(fields, key=lambda row: row['offset'])
    result = []; linked = bytearray(payload)
    for index, (row, (address, name)) in enumerate(zip(rows, SLOTS)):
        unit = units.get(name, {})
        expected = callback_symbol(index)
        require(unit.get('target_address') == address and unit.get('symbol') == expected,
                'canonical ECL callback identity differs: '+name)
        require(row['offset'] == 4*index and row['type_id'] == 6 and
                row['symbol'] == expected and type(row['addend']) is int and row['addend'] == 0 and
                struct.unpack_from('<I', payload, 4*index)[0] == 0 and
                row['storage'] == 2 and row['symbol_type'] == 0x20 and
                type(row['symbol_section']) is int and row['symbol_section'] >= 0 and
                type(row['symbol_value']) is int and row['symbol_value'] >= 0 and
                (row['symbol_section'] > 0 or row['symbol_value'] == 0),
                'ECL callback slot is not its native function reference')
        struct.pack_into('<I', linked, 4*index, address)
        result.append(dict(slot=index, unit=name, symbol=expected, target=address,
                           definition='same-object' if row['symbol_section'] else 'external'))
    require(bytes(linked) == original, 'complete reviewed ECL prefix differs')
    return dict(reviewed_bytes=SIZE, slots=result, complete_reviewed_prefix_equal=True,
                original_prefix_sha256=hashlib.sha256(original).hexdigest(),
                exactness_credit='none; data prefix, not authored function credit',
                original_array_extent='unknown; next zero DWORD ownership unresolved')


def inspect(path):
    spec = importlib.util.spec_from_file_location('table_coff', ROOT/'scripts/compare-coff-function.py')
    coff = importlib.util.module_from_spec(spec); spec.loader.exec_module(coff)
    image = coff.verified_target()
    units = tomllib.loads((ROOT/'config/match-units.toml').read_text())['units']
    payload, fields = read_definition(Path(path).read_bytes(), coff)
    result = validate_binding(payload, fields, units, coff.pe_bytes_at(image, BASE, SIZE))
    result.update(object=str(path), object_sha256=hashlib.sha256(Path(path).read_bytes()).hexdigest(),
                  target_sha256=hashlib.sha256(image).hexdigest(), table_symbol=SYMBOL,
                  source_binding='Existing object only; no source-to-object attestation unless separately built/bound.')
    return result


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--object', type=Path, default=ROOT/OBJECT)
    parser.add_argument('--build', action='store_true', help='compile only the table owner carrier')
    args = parser.parse_args()
    if args.build:
        require(args.object.resolve() == (ROOT/OBJECT).resolve(), 'build output must be canonical')
        units = tomllib.loads((ROOT/'config/match-units.toml').read_text())['units']
        command = ['taskset', '-c', str(min(os.sched_getaffinity(0))), 'nice', '-n', '10',
                   'timeout', '--kill-after=10s', '180s', 'python3', '-B',
                   str(ROOT/'scripts/build-match-unit.py'), '--unit', 'enemy-spawn-exattack-type1']
        subprocess.run(command, cwd=ROOT, check=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    print(json.dumps(inspect(args.object), indent=2))


if __name__ == '__main__':
    main()
