#!/usr/bin/env python3
"""Original-PE/COFF proof for three origin-unclassified TH09 VM group constructors.

Compile src/NativeVmGroupConstructors.cpp using pinned VC7.1 /O2 /Ob0 first.
A byte-perfect construction pattern does not establish authored origin.
"""
import argparse
import hashlib
import importlib.util
import json
from pathlib import Path
import struct

ROOT = Path(__file__).resolve().parents[1]
A = '??0AnmVm@@QAE@XZ'
V = '??_H@YGXPAXIHP6EPAX0@Z@Z'
PATTERNS = (
    ('NativeVmSevenPlusOneView', 0x00435D40, 39,
     'f74fc7f11f55152256e04324dea05774c7f440b60dff38a6d9802a9a13be85bb',
     ((2, 'DIR32', A), (20, 'REL32', V), (31, 'REL32', A))),
    ('NativeVmFivePlusOneView', 0x00435D70, 39,
     '616ed6b8c857d1935256aaac9d24eaec9e59c654bb93e86cddfc5e040ca8f3e8',
     ((2, 'DIR32', A), (20, 'REL32', V), (31, 'REL32', A))),
    ('NativeVmThreeView', 0x00435DA0, 28,
     'f043d384874961df1fcf44f6a1108ccf398e2f57d2150e26e5465c5c7f74f393',
     ((2, 'DIR32', A), (20, 'REL32', V))),
)
DESTINATION = {A: 0x00403A10, V: 0x00401470}


def inspect(object_path):
    spec = importlib.util.spec_from_file_location('coff', ROOT/'scripts/compare-coff-function.py')
    coff = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(coff)
    image = coff.verified_target()
    results = []
    for name, address, extent, digest, expected in PATTERNS:
        original = coff.pe_bytes_at(image, address, extent)
        if hashlib.sha256(original).hexdigest() != digest:
            raise ValueError('wrong TH09 original constructor: ' + name)
        source, fields = coff.object_function(
            object_path, '??0%s@@QAE@XZ' % name, include_symbol_locations=True)
        if len(source) != extent:
            raise ValueError('%s: incomplete extent %d/%d' % (name, len(source), extent))
        actual = tuple((f['offset'], f['type'], f['symbol']) for f in fields)
        if actual != expected or any(f['addend'] for f in fields):
            raise ValueError(name + ' actual COFF fields differ: ' + str(actual))
        linked = bytearray(source)
        for offset, kind, symbol in expected:
            target = DESTINATION[symbol]
            encoded = (target if kind == 'DIR32' else
                       target - (address + offset + 4)) & 0xffffffff
            old, = struct.unpack_from('<I', original, offset)
            if old != encoded:
                raise ValueError('%s: original PE operand +%x wrong' % (name, offset))
            struct.pack_into('<I', linked, offset, encoded)
        unequal = [i for i, (a, b) in enumerate(zip(linked, original)) if a != b]
        results.append({'name': name, 'original_va': hex(address), 'physical_bytes': extent,
                        'bound_fields': len(fields), 'difference_offsets': unequal,
                        'complete_equal': not unequal})
    return {'diagnostic_only': True, 'exactness_credit': 'none',
            'origin': 'unclassified', 'constructors': results}


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('object', type=Path)
    args = parser.parse_args()
    try:
        result = inspect(args.object)
    except (ValueError, KeyError, OSError, struct.error) as err:
        print(json.dumps({'error': str(err)}))
        return 2
    print(json.dumps(result, indent=2))
    return 0 if all(x['complete_equal'] for x in result['constructors']) else 1


if __name__ == '__main__':
    raise SystemExit(main())
