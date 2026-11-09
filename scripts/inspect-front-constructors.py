#!/usr/bin/env python3
"""Compare the two still-origin-unclassified FrontSide constructors to TH09 PE.

Compile src/FrontSideCtor.cpp with the pinned VC7.1 /O2 /Ob0 profile first.
Exact code generation here does not classify implicit versus authored origin.
"""
import argparse
import hashlib
import importlib.util
import json
from pathlib import Path
import struct

ROOT = Path(__file__).resolve().parents[1]
SYMBOL_TARGETS = {
    '??0AnmVm@@QAE@XZ': 0x00403A10,
    '??_H@YGXPAXIHP6EPAX0@Z@Z': 0x00401470,
    '??0FrontSideAuxView@@QAE@XZ': 0x0041A470,
    '??0ZunTimer@@QAE@XZ': 0x00403390,
}
# Exact field identities, including the whole 307-byte owner's 26 relocations.
A = '??0AnmVm@@QAE@XZ'
V = '??_H@YGXPAXIHP6EPAX0@Z@Z'
U = '??0FrontSideAuxView@@QAE@XZ'
T = '??0ZunTimer@@QAE@XZ'
FRONT = (
    ('FrontSideAuxView', '??0FrontSideAuxView@@QAE@XZ',
     0x0041A470, 28,
     '58b0dbf3366457ffd4b28deb249665c054216f3b93bdb53f63c004a7c7ba65fd',
     ((0x02, 'DIR32', A), (0x14, 'REL32', V))),
    ('FrontSide', '??0FrontSide@@QAE@XZ',
     0x0041A490, 307,
     'a0395864baef40177c8f5c1d561e335fce4ecfc802d3c37b7d4217502f187b0f',
     (
         (0x02, 'DIR32', A), (0x11, 'REL32', V),
         (0x16, 'DIR32', A), (0x29, 'REL32', V),
         (0x2E, 'DIR32', A), (0x41, 'REL32', V),
         (0x4C, 'REL32', A),
         (0x51, 'DIR32', A), (0x64, 'REL32', V),
         (0x6F, 'REL32', A),
         (0x74, 'DIR32', A), (0x87, 'REL32', V),
         (0x8C, 'DIR32', A), (0x9F, 'REL32', V),
         (0xA4, 'DIR32', A), (0xB7, 'REL32', V),
         (0xC2, 'REL32', A),
         (0xC7, 'DIR32', A), (0xDA, 'REL32', V),
         (0xDF, 'DIR32', A), (0xF2, 'REL32', V),
         (0xF7, 'DIR32', A), (0x10A, 'REL32', V),
         (0x115, 'REL32', U),
         (0x120, 'REL32', T), (0x12B, 'REL32', T),
     )),
)


def inspect(obj):
    spec = importlib.util.spec_from_file_location(
        'original_pe_coff', ROOT / 'scripts/compare-coff-function.py')
    module = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(module)
    original_image = module.verified_target()
    rows = []
    for label, symbol, addr, size, digest, expected_fields in FRONT:
        target = module.pe_bytes_at(original_image, addr, size)
        if hashlib.sha256(target).hexdigest() != digest:
            raise ValueError('original target changed: ' + label)
        raw, fields = module.object_function(obj, symbol, include_symbol_locations=True)
        if len(raw) != size:
            raise ValueError('%s code extent is %d, expected %d' % (label, len(raw), size))
        observed = tuple(
            (f['offset'], f['type'], f['symbol']) for f in fields)
        if observed != expected_fields or any(f['addend'] != 0 for f in fields):
            raise ValueError(label + ' COFF relocation identity drift: ' + repr(observed))
        linked = bytearray(raw)
        for off, kind, sym in expected_fields:
            dest = SYMBOL_TARGETS[sym]
            encoded = dest if kind == 'DIR32' else (dest - (addr + off + 4)) & 0xffffffff
            stored, = struct.unpack_from('<I', target, off)
            if encoded != stored:
                raise ValueError('%s original relocation target differs at +%x' % (label, off))
            struct.pack_into('<I', linked, off, encoded)
        differences = [i for i, (x, y) in enumerate(zip(linked, target)) if x != y]
        rows.append({
            'function': label, 'address': hex(addr),
            'original_bytes': size, 'candidate_bytes': len(raw),
            'bound_relocations': len(fields), 'difference_offsets': differences,
            'complete_original_pe_equal': not differences,
        })
    return {'diagnostic_only': True, 'origin': 'unclassified',
            'exactness_credit': 'none', 'constructors': rows}


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('object', type=Path)
    args = parser.parse_args()
    try:
        result = inspect(args.object)
    except (KeyError, ValueError, OSError, struct.error) as exc:
        print(json.dumps({'error': str(exc)}))
        return 2
    print(json.dumps(result, indent=2))
    return 0 if all(x['complete_original_pe_equal'] for x in result['constructors']) else 1


if __name__ == '__main__':
    raise SystemExit(main())
