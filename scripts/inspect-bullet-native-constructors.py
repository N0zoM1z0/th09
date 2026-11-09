#!/usr/bin/env python3
"""Verify natural Bullet, Laser, sprites and implicit transform-state ctors.

Pinned VC7.1 /O2 /Ob0; checked against full original TH09 v1.50a PE.
The implicit state-ctor body aliases a separately reviewed 12-byte owner;
source equivalence is not proof of unique historic authored origin.
"""
import argparse
import hashlib
import importlib.util
import json
from pathlib import Path
import struct

ROOT = Path(__file__).resolve().parents[1]
A = '??0AnmVm@@QAE@XZ'
B = '??0BulletTypeSprites@@QAE@XZ'
T = '??0ZunTimer@@QAE@XZ'
E = '??0BulletExState@@QAE@XZ'
V = '??_H@YGXPAXIHP6EPAX0@Z@Z'
ADDRESSES = {
    A: 0x00403A10, B: 0x00414FA0, T: 0x00403390,
    E: 0x0041EE20, V: 0x00401470,
}
OWNERS = (
    ('BulletTypeSprites', 0x00414FA0, 56,
     'd1461bea02ce48ba45e0330698b5477549d5b1752503bcc141e18bb32a4a115a',
     ((4, 'REL32', A), (15, 'REL32', A), (26, 'REL32', A),
      (37, 'REL32', A), (48, 'REL32', A))),
    ('BulletExState', 0x0041EE20, 12,
     'a300c33ce483d875f36319a11934637be842d7d8fbd9850c6e3fb06266ec88db',
     ((4, 'REL32', T),)),
    ('Bullet', 0x00414FE0, 58,
     'e446adc9bbd9c04b124e97dc5e047dca2d69c38560a00466a161c7abd82bbee2',
     ((7, 'REL32', B), (18, 'REL32', T), (29, 'REL32', T),
      (34, 'DIR32', E), (50, 'REL32', V))),
    ('Laser', 0x00415020, 34,
     '28870b19994ca91b40a7ddb20d9f64f30efbd499fd6c5f6f4de18aa3ed9ec131',
     ((4, 'REL32', A), (15, 'REL32', A), (26, 'REL32', T))),
)


def inspect(obj):
    spec = importlib.util.spec_from_file_location(
        'native_coff', ROOT / 'scripts/compare-coff-function.py')
    coff = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(coff)
    image = coff.verified_target()
    rows = []
    for name, original_va, size, expected_sha, originals in OWNERS:
        original = coff.pe_bytes_at(image, original_va, size)
        sha = hashlib.sha256(original).hexdigest()
        if expected_sha is not None and sha != expected_sha:
            raise ValueError(name + ' original full-body hash mismatch')
        # The implicit BulletExState COMDAT lacks an auxiliary function
        # size record. The independent original extent is required here;
        # the COFF parser still verifies its actual code-section and length.
        raw, fields = coff.object_function(
            obj, '??0%s@@QAE@XZ' % name,
            expected_size=size, include_symbol_locations=True)
        if len(raw) != size:
            raise ValueError('%s code size differs: %d/%d' % (name, len(raw), size))
        actual = tuple((f['offset'], f['type'], f['symbol']) for f in fields)
        if actual != originals or any(f['addend'] != 0 for f in fields):
            raise ValueError(name + ' actual original-bound COFF fields drifted')
        if name == 'Bullet':
            # The real parent must call SAME-OBJECT constructor definitions,
            # not merely relocations to compatible numeric external targets.
            for row in fields:
                if row['symbol'] in (B, E) and not (
                        row['symbol_section'] > 0 and
                        row['owner_section'] > 0 and
                        row['symbol_section'] != row['owner_section'] and
                        row['symbol_value'] == 0):
                    raise ValueError('Bullet nested constructor lacks same-object definition')
        linked = bytearray(raw)
        for off, kind, symbol in originals:
            destination = ADDRESSES[symbol]
            encoded = (destination if kind == 'DIR32' else
                       destination - (original_va + off + 4)) & 0xffffffff
            target_word, = struct.unpack_from('<I', original, off)
            if target_word != encoded:
                raise ValueError('%s original PE field +%x is not the reviewed callee' %
                                 (name, off))
            struct.pack_into('<I', linked, off, encoded)
        differences = [i for i, (left, right) in enumerate(zip(linked, original))
                       if left != right]
        rows.append(dict(function=name, target=hex(original_va),
                         physical_bytes=size, original_sha256=sha,
                         relocation_fields=len(fields),
                         difference_offsets=differences,
                         complete_original_equal=not differences))
    return dict(diagnostic_only=True, exactness_credit='none; origin unclassified',
                original_japanese_game='TH09 v1.50a', constructors=rows)


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('object', type=Path)
    args = parser.parse_args()
    try:
        result = inspect(args.object)
    except (ValueError, KeyError, OSError, struct.error) as error:
        print(json.dumps({'error': str(error)}))
        return 2
    print(json.dumps(result, indent=2))
    return 0 if all(x['complete_original_equal'] for x in result['constructors']) else 1


if __name__ == '__main__':
    raise SystemExit(main())
