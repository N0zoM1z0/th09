"""Pure negative/identity tests; the actual target and VC7.1 are optional."""

import importlib.util
from pathlib import Path
import struct
import unittest

ROOT = Path(__file__).resolve().parents[1]
spec = importlib.util.spec_from_file_location(
    'ecl_biased', ROOT / 'scripts/inspect-ecl-biased-movement.py')
mod = importlib.util.module_from_spec(spec)
spec.loader.exec_module(mod)


def rows():
    # Full reviewed ordered field inventory with target offsets. This tests
    # synthetically valid bytes; it is NOT a fabricated exact game object.
    return [{'offset':off,'type':kind,'symbol':symbol,'addend':0}
            for off,kind,symbol in mod.TARGET_OPERANDS]


class MoveRandomBiasedOracleTests(unittest.TestCase):
    def test_manifest_independent_bounds_and_exact_order(self):
        mod.validate_manifest()
        self.assertEqual(len(mod.TARGET_OPERANDS),28)
        self.assertEqual(mod.TARGET_OPERANDS[20][0],0x130)
        self.assertEqual(mod.TARGET_OPERANDS[-1][0],0x1fa)

    def test_valid_516_byte_link_uses_original_identities(self):
        code, fields = mod.bind_candidate(bytes(mod.SIZE), rows())
        self.assertEqual(len(code),516)
        self.assertEqual(len(fields),28)
        for (off,kind,symbol),(actual_off, actual_kind, dest) in zip(mod.TARGET_OPERANDS,fields):
            self.assertEqual((off,kind,mod.REFERENTS[symbol]), (actual_off,actual_kind,dest))
            self.assertEqual(struct.unpack_from('<I',code,off)[0],mod.encoded(kind,off,dest))
        self.assertEqual(code[0x134],0)

    def test_valid_517_byte_link_never_masks_extra_byte(self):
        code, fields = mod.bind_candidate(bytes(mod.SIZE+1), rows())
        self.assertEqual(len(code),mod.SIZE+1)
        self.assertEqual(len(fields),28)

    def test_missing_duplicate_reordered_or_wrong_symbol_rejected(self):
        base=rows()
        edits=[base[:-1],base+[base[-1]],
               base[:8]+[base[9],base[8]]+base[10:],
               [dict(base[0],symbol='?invented@Fake@@QAEXXZ')]+base[1:],
               [dict(base[0],type='REL32')]+base[1:],
               [dict(base[0],addend=4)]+base[1:]]
        for variant in edits:
            with self.subTest(variant=variant[:2]):
                with self.assertRaises(ValueError):
                    mod.bind_candidate(bytes(mod.SIZE+1),variant)

    def test_overlaps_out_of_bounds_and_wrong_extent_rejected(self):
        base=rows()
        for bad_offset in (-1,mod.SIZE-3,base[1]['offset']):
            bad=[dict(base[0],offset=bad_offset)]+base[1:]
            with self.subTest(offset=bad_offset),self.assertRaises(ValueError):
                mod.bind_candidate(bytes(mod.SIZE+1),bad)
        bad=base[:]
        bad[1]=dict(bad[1],offset=bad[0]['offset']+2)
        with self.assertRaises(ValueError):
            mod.bind_candidate(bytes(mod.SIZE+1),bad)
        for size in (mod.SIZE-1,mod.SIZE+2):
            with self.assertRaises(ValueError):
                mod.bind_candidate(bytes(size),base)

    def test_relative_and_absolute_encoding_keep_distinct_roles(self):
        off=0x102
        address=0x00407200
        self.assertEqual(mod.encoded('DIR32',off,address),address)
        self.assertEqual(mod.encoded('REL32',off,address),
                         (address-(mod.BASE+off+4))&0xffffffff)
        with self.assertRaises(ValueError):mod.encoded('INVALID',off,address)


if __name__=='__main__':
    unittest.main()
