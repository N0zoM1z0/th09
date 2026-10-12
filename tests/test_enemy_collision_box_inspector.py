"""Target-independent negative controls for the collision-box leaf Oracle."""

import importlib.util
from pathlib import Path
import struct
import unittest

ROOT = Path(__file__).resolve().parents[1]
spec = importlib.util.spec_from_file_location(
    'enemy_box_oracle', ROOT / 'scripts/inspect-enemy-collision-box.py')
BOX = importlib.util.module_from_spec(spec)
spec.loader.exec_module(BOX)


class EnemyCollisionBoxInspectorTests(unittest.TestCase):
    def field(self, offset, symbol, **overrides):
        row = dict(offset=offset, type='REL32', symbol=symbol, addend=0)
        row.update(overrides)
        return row

    def fields(self):
        return [self.field(0x16, list(BOX.CALL_TARGETS)[0]),
                self.field(0x47, list(BOX.CALL_TARGETS)[1])]

    def test_relocation_binds_only_independent_exact_calls(self):
        source = bytes(BOX.SIZE)
        result = BOX.bind_candidate(source, self.fields())
        self.assertEqual(len(result), BOX.SIZE)
        for offset, dest in ((0x16, 0x40f5a0), (0x47, 0x40f8e0)):
            self.assertEqual(struct.unpack_from('<i', result, offset)[0],
                             dest - (BOX.BASE + offset + 4))
        self.assertEqual(source, bytes(BOX.SIZE))

    def test_wrong_call_owner_or_signature_fails(self):
        for fields in (
            self.fields()[:1],
            [self.field(0x16, 'wrong'), self.fields()[1]],
            [self.field(0x16, list(BOX.CALL_TARGETS)[0], addend=1),
             self.fields()[1]],
            [self.field(0x16, list(BOX.CALL_TARGETS)[0], type='DIR32'),
             self.fields()[1]],
            list(reversed(self.fields())),
        ):
            with self.subTest(fields=fields), self.assertRaises(ValueError):
                BOX.bind_candidate(bytes(BOX.SIZE), fields)

    def test_unowned_or_overlapping_rel32_fails(self):
        fields = self.fields()
        for bad_offset in (-1, BOX.SIZE - 3, 0x17):
            copied = [dict(fields[0]), dict(fields[1])]
            copied[1]['offset'] = bad_offset
            with self.subTest(offset=bad_offset), self.assertRaises(ValueError):
                BOX.bind_candidate(bytes(BOX.SIZE), copied)

    @unittest.skipUnless(importlib.util.find_spec('capstone') is not None,
                         'Capstone is intentionally unavailable under CI python -S')
    def test_machine_decode_fail_closed_and_stable_abi(self):
        with self.assertRaises(ValueError):
            BOX.decode_owner(b'\x90\x90')
        with self.assertRaises(ValueError):
            BOX.decode_owner(b'\xff\xd0\xc2\x08\x00')  # indirect call
        with self.assertRaises(ValueError):
            BOX.decode_owner(b'\xe8\x00\x00\x00\x00\xc3')  # wrong ret


if __name__ == '__main__':
    unittest.main()
