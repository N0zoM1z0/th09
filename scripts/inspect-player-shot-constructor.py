#!/usr/bin/env python3
"""Original-bound, non-crediting PE/COFF proof of Player shot special member.

A source-equivalent empty C++ constructor can be generated in the PlayerRuntime
translation unit under pinned VC7.1. The 44-byte original may be an explicit
source constructor or implicitly compiler-generated; its authored origin
remains unclassified, independent of code-generation exactness.
"""
import argparse
import hashlib
import importlib.util
import json
from pathlib import Path
import struct

ROOT = Path(__file__).resolve().parents[1]
BASE = 0x0041F030
SIZE = 44
ORIGINAL_SHA256 = 'df4c963a7df2a31c0e3c526668b0d0d0ee1ec89f4d2c3177df9364b392de307e'
SYMBOL = '??0PlayerShotCtorView@@QAE@XZ'
FIELDS = (
    (4, 'REL32', '??0PlayerAnmVmView@@QAE@XZ', 0x00403A10),
    (9, 'DIR32', '??0PlayerConstructedVector3View@@QAE@XZ', 0x004343D0),
    (25, 'REL32', '??_H@YGXPAXIHP6EPAX0@Z@Z', 0x00401470),
    (36, 'REL32', '??0ZunTimer@@QAE@XZ', 0x00403390),
)


def inspect(obj: Path) -> dict:
    spec = importlib.util.spec_from_file_location('coff', ROOT/'scripts/compare-coff-function.py')
    if spec is None or spec.loader is None:
        raise ValueError('canonical original PE/COFF inspector is missing')
    coff = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(coff)
    original = coff.pe_bytes_at(coff.verified_target(), BASE, SIZE)
    if hashlib.sha256(original).hexdigest() != ORIGINAL_SHA256:
        raise ValueError('original Japanese 1.50a shot ctor SHA256/extent disagree')
    raw, rows = coff.object_function(obj, SYMBOL, include_symbol_locations=True)
    if len(raw) != SIZE or len(rows) != len(FIELDS):
        raise ValueError(f'non-original shot constructor extent/relocations: {len(raw)} bytes, {len(rows)} fields')
    linked = bytearray(raw)
    for row, (offset, kind, symbol, destination) in zip(rows, FIELDS):
        if (row['offset'], row['type'], row['symbol'], row['addend']) != (offset, kind, symbol, 0):
            raise ValueError(f'non-original shot special-member field: {row}')
        original_word, = struct.unpack_from('<I', original, offset)
        expected = (destination-(BASE+offset+4) if kind == 'REL32' else destination) & 0xFFFFFFFF
        if original_word != expected:
            raise ValueError(f'original TH09 PE operand @ +0x{offset:X} is not the reviewed constructor/callee')
        struct.pack_into('<I', linked, offset, expected)
    return {
        'owner_address': hex(BASE),
        'original_physical_bytes': SIZE,
        'candidate_physical_bytes': len(raw),
        'relocations': len(rows),
        'original_sha256': ORIGINAL_SHA256,
        'candidate_linked_sha256': hashlib.sha256(linked).hexdigest(),
        'complete_original_bytes_equal': linked == original,
        'status': 'unclassified special member; non-crediting source equivalence',
    }


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('object', type=Path)
    parser.add_argument('--json', action='store_true')
    args = parser.parse_args()
    try:
        result = inspect(args.object)
    except (OSError, ValueError, KeyError, IndexError, struct.error) as err:
        print(json.dumps({'error': str(err)}))
        return 2
    if args.json:
        print(json.dumps(result, indent=2))
    else:
        print(f"Shot constructor {BASE:#x}: {SIZE} bytes, {len(FIELDS)} real original fields; "
              f"exact={result['complete_original_bytes_equal']} (unclassified/no authored credit)")
    return 0 if result['complete_original_bytes_equal'] else 1


if __name__ == '__main__':
    raise SystemExit(main())
