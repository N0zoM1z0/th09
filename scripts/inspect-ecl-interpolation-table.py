#!/usr/bin/env python3
"""Verify eight reviewed ECL interpolation slots and both native function bodies.

This proves a bounded data prefix, not an original array declaration, authored
function credit, source attestation, or complete product linkage.
"""
import argparse
import hashlib
import importlib.util
import json
from pathlib import Path
import re
import struct
import tomllib

ROOT = Path(__file__).resolve().parents[1]
BASE, SIZE = 0x004A0AF4, 32
SHA256 = 'e7dc5fcf71f55100a679788e8e51ad25317c1b53e8bc34fcd950eca63fb85b2b'
SYMBOL = '?g_EclInterpolationCallbacks@Th09EclRunControl@@3PAP6IXPAUEnemyView@@PAUTh09EclInterpolationSlotView@@M@ZA'
SLOTS = ((0x00407450, 'ecl-interpolate-linear'),) * 7 + ((0x004074A0, 'ecl-interpolate-hermite'),)
OBJECT = ROOT / 'build/matching/EclHelpers.obj'


def load(name, filename):
    spec = importlib.util.spec_from_file_location(name, ROOT / 'scripts' / filename)
    result = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(result)
    return result


def require(condition, message):
    if not condition:
        raise ValueError(message)


def validate_binding(payload, fields, units, original):
    require(len(payload) == len(original) == SIZE and len(fields) == 8,
            'incomplete interpolation prefix or relocation inventory')
    require(hashlib.sha256(original).hexdigest() == SHA256 and
            struct.unpack('<8I', original) == tuple(x[0] for x in SLOTS),
            'original interpolation slot order or identity changed')
    rows = sorted(fields, key=lambda x: x['offset'])
    linked = bytearray(payload)
    for index, (row, (address, name)) in enumerate(zip(rows, SLOTS)):
        unit = units.get(name, {})
        expected = '?Interpolate' + ('Linear' if index < 7 else 'Hermite')
        require(unit.get('target_address') == address and
                unit.get('source') == 'src/EclHelpers.cpp' and
                unit.get('size') == (74 if index < 7 else 188) and
                re.fullmatch(re.escape(expected) + r'@\?A0x[0-9a-fA-F]+@@YIXPAUEnemyView@@PAUTh09EclInterpolationSlotView@@M@Z',
                             str(unit.get('symbol', ''))) is not None,
                'canonical interpolation owner changed')
        require(row['offset'] == index * 4 and row['type_id'] == 6 and
                row['symbol'] == unit['symbol'] and type(row['addend']) is int and
                row['addend'] == 0 and struct.unpack_from('<I', payload, index*4)[0] == 0 and
                row['symbol_type'] == 0x20 and row['storage'] == 2 and
                type(row['symbol_section']) is int and row['symbol_section'] > 0 and
                type(row['symbol_value']) is int and row['symbol_value'] == 0,
                'interpolation slot is not its actual same-object native function')
        struct.pack_into('<I', linked, index * 4, address)
    require(bytes(linked) == original, 'complete reviewed interpolation prefix differs')
    return rows


def prove_callee(path, unit, reference, coff, image):
    raw, fields = coff.object_function(path, unit['symbol'], include_symbol_locations=True)
    expected = sorted(unit.get('relocations', []), key=lambda x: x['offset'])
    require(len(raw) == unit['size'] and len(fields) == len(expected),
            'native interpolation body extent or relocation inventory changed')
    require(all(x['owner_section'] == reference['symbol_section'] and
                x['owner_value'] == reference['symbol_value'] for x in fields),
            'table reference and actual function definition disagree')
    coff.validate_symbol_destinations(expected)
    coff.validate_native_crt_destinations(expected)
    coff.validate_owner_local_destinations(fields, expected, unit['target_address'], len(raw))
    linked = bytearray(raw)
    for actual, want in zip(fields, expected):
        require((actual['offset'], actual['type'], actual['symbol']) ==
                (want['offset'], want['type'], want['symbol']) and
                actual['addend'] == want.get('addend', 0),
                'native interpolation referent identity changed')
        value = want['target'] + actual['addend']
        if actual['type'] == 'REL32':
            value -= unit['target_address'] + actual['offset'] + 4
        else:
            require(actual['type'] == 'DIR32', 'unsupported native interpolation relocation')
        struct.pack_into('<I', linked, actual['offset'], value & 0xFFFFFFFF)
    require(bytes(linked) == coff.pe_bytes_at(image, unit['target_address'], len(raw)),
            'native interpolation complete body differs from original')
    literals = coff.validate_literal_definitions(path, fields, image, expected)
    return dict(bytes=len(raw), fields=len(fields), complete_original_equal=True,
                literal_definitions=literals)


def inspect(path):
    coff = load('interpolation_coff', 'compare-coff-function.py')
    reader = load('interpolation_data_reader', 'inspect-ecl-ex-instruction-table.py')
    reader.SYMBOL, reader.SIZE = SYMBOL, SIZE
    image = coff.verified_target()
    original = coff.pe_bytes_at(image, BASE, SIZE)
    require(coff.pe_bytes_at(image, BASE + SIZE, 4) == struct.pack('<I', 0x0040C3C0),
            'independently reviewed following ex-instruction table entry changed')
    units = tomllib.loads((ROOT / 'config/match-units.toml').read_text())['units']
    payload, fields = reader.read_definition(path.read_bytes(), coff)
    rows = validate_binding(payload, fields, units, original)
    bodies = {name: prove_callee(path, units[name], rows[index], coff, image)
              for index, (_, name) in ((0, SLOTS[0]), (7, SLOTS[7]))}
    return dict(target_sha256=hashlib.sha256(image).hexdigest(),
                object=str(path), object_sha256=hashlib.sha256(path.read_bytes()).hexdigest(),
                table_symbol=SYMBOL, reviewed_bytes=SIZE, slots=8,
                complete_reviewed_prefix_equal=True, original_prefix_sha256=SHA256,
                native_bodies=bodies,
                original_array_extent='unknown; eight reviewed slots before separate ex-instruction table',
                exactness_credit='none; data prefix and existing native functions',
                source_binding='supplied object only; source/profile binding requires a separate build receipt')


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--object', type=Path, default=OBJECT)
    args = parser.parse_args()
    try:
        print(json.dumps(inspect(args.object), indent=2))
        return 0
    except (OSError, ValueError, KeyError, AssertionError, struct.error) as error:
        print(json.dumps(dict(result='error', message=str(error))))
        return 2


if __name__ == '__main__':
    raise SystemExit(main())
