#!/usr/bin/env python3
"""Original-PE/COFF proof for the native AsciiManager construction chain.

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
E = '??0NativeVmEmptyRecord60View@@QAE@XZ'
S = '??0NativeVmSevenPlusOneView@@QAE@XZ'
F = '??0NativeVmFivePlusOneView@@QAE@XZ'
H = '??0NativeVmThreeView@@QAE@XZ'
R = '??0NativeVmTimerRecord38View@@QAE@XZ'
T = '??0ZunTimer@@QAE@XZ'
C = '??0AsciiManagerString@@QAE@XZ'
P = '??0AsciiManagerPopup@@QAE@XZ'
M = '??0PauseMenu@@QAE@XZ'
N = '??0AsciiMenuState5@@QAE@XZ'
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
    ('NativeVmEmptyRecord60View', 0x004343D0, 3,
     '58367ffa2a0179375018fa0f5c26da24391e42ebe0ed8fdda35a21fc7bdc396f', ()),
    ('NativeVmTimerRecord38View', 0x00435AF0, 15,
     '8d8690a189eac94b8dd6afc1220e24118d844d997c60326466f5be8d6573172c',
     ((7, 'REL32', T),)),
    ('NativeVmCompositeOwnerView', 0x00435DC0, 227,
     '4b5240359f68c78419f4a132962cd878a579a382d1a93a4878b390b2897bc2c0',
     (
         (4, 'REL32', A), (15, 'REL32', A),
         (26, 'REL32', A), (37, 'REL32', A),
         (48, 'REL32', A), (59, 'REL32', A),
         (70, 'REL32', A), (81, 'REL32', A),
         (92, 'REL32', A), (97, 'DIR32', A),
         (116, 'REL32', V), (121, 'DIR32', C),
         (140, 'REL32', V), (151, 'REL32', M),
         (162, 'REL32', N), (173, 'REL32', H),
         (184, 'REL32', A), (189, 'DIR32', P),
         (208, 'REL32', V), (219, 'REL32', A),
     )),
)
DESTINATION = {
    A: 0x00403A10, V: 0x00401470, E: 0x004343D0,
    S: 0x00435D40, F: 0x00435D70, H: 0x00435DA0,
    R: 0x00435AF0, T: 0x00403390,
    C: 0x004343D0, P: 0x00435AF0,
    M: 0x00435D40, N: 0x00435D70,
}

# Authentic current AsciiManager subobject types are a second independent
# compiler witness for four original constructor bodies; several are folded
# aliases rather than unique physical functions.
ASCII_TYPES = (
    ('AsciiManagerString', 0x004343D0, 3,
     '58367ffa2a0179375018fa0f5c26da24391e42ebe0ed8fdda35a21fc7bdc396f', ()),
    ('AsciiManagerPopup', 0x00435AF0, 15,
     '8d8690a189eac94b8dd6afc1220e24118d844d997c60326466f5be8d6573172c',
     ((7, 'REL32', T),)),
    ('PauseMenu', 0x00435D40, 39,
     'f74fc7f11f55152256e04324dea05774c7f440b60dff38a6d9802a9a13be85bb',
     ((2, 'DIR32', A), (20, 'REL32', V), (31, 'REL32', A))),
    ('AsciiMenuState5', 0x00435D70, 39,
     '616ed6b8c857d1935256aaac9d24eaec9e59c654bb93e86cddfc5e040ca8f3e8',
     ((2, 'DIR32', A), (20, 'REL32', V), (31, 'REL32', A))),
)


def check_ascii_global_startup(stub):
    # Original CRT initializer at 0x48D950: mov ecx, &g_AsciiManager;
    # jmp original 227-byte constructor. Verify this owner is not invented.
    if (len(stub) != 10 or stub[0] != 0xB9 or stub[5] != 0xE9 or
            struct.unpack_from('<I', stub, 1)[0] != 0x004CE458 or
            0x0048D95A + struct.unpack_from('<i', stub, 6)[0] != 0x00435DC0):
        raise ValueError('original global AsciiManager construction root differs')
    return {'startup_va': '0x48d950', 'global_object_va': '0x4ce458',
            'constructor_va': '0x435dc0'}


def inspect(object_path, patterns=PATTERNS):
    spec = importlib.util.spec_from_file_location('coff', ROOT/'scripts/compare-coff-function.py')
    coff = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(coff)
    image = coff.verified_target()
    global_root = check_ascii_global_startup(coff.pe_bytes_at(image, 0x0048D950, 10))
    results = []
    for name, address, extent, digest, expected in patterns:
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
            'origin': 'unclassified', 'original_global_root': global_root,
            'constructors': results}


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('object', type=Path)
    parser.add_argument('--ascii-object', type=Path,
                        help='also compare four authentic AsciiManager member constructors')
    args = parser.parse_args()
    try:
        result = inspect(args.object)
        if args.ascii_object is not None:
            result['constructors'].extend(
                inspect(args.ascii_object, ASCII_TYPES)['constructors'])
    except (ValueError, KeyError, OSError, struct.error) as err:
        print(json.dumps({'error': str(err)}))
        return 2
    print(json.dumps(result, indent=2))
    return 0 if all(x['complete_equal'] for x in result['constructors']) else 1


if __name__ == '__main__':
    raise SystemExit(main())
