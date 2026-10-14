"""Pure input-gate tests for the two original TH09 position callbacks."""

import importlib.util
from pathlib import Path
import struct
import unittest

ROOT = Path(__file__).resolve().parents[1]
spec = importlib.util.spec_from_file_location(
    'player_type48_inspector', ROOT / 'scripts/inspect-player-position-type48.py')
TYPE48 = importlib.util.module_from_spec(spec)
spec.loader.exec_module(TYPE48)


class PlayerType48InspectorTests(unittest.TestCase):
    @staticmethod
    def rows(callback_type, shifted=False):
        return [dict(offset=offset + (1 if shifted and offset >= 0x07D else 0),
                     type=kind, symbol=symbol, addend=0)
                for offset, kind, symbol, _ in TYPE48.expected_fields(callback_type)]

    @staticmethod
    def synthetic_target(callback_type):
        data = bytearray(TYPE48.SIZE)
        base = TYPE48.TYPES[callback_type][0]
        for offset, kind, _, address in TYPE48.expected_fields(callback_type):
            struct.pack_into('<I', data, offset,
                             TYPE48.encoded_value(base, offset, kind, address))
        data[0x73:0x75] = b'\x8b\xd1'
        return bytes(data)

    def test_reviewed_original_fields_on_each_independent_target(self):
        for callback_type in TYPE48.TYPES:
            with self.subTest(callback_type=callback_type):
                TYPE48.validate_target_fields(
                    self.synthetic_target(callback_type), callback_type)

    def test_link_250_byte_candidate_with_real_shifted_fields(self):
        for callback_type in TYPE48.TYPES:
            with self.subTest(callback_type=callback_type):
                raw = bytes(TYPE48.SIZE + 1)
                rows = self.rows(callback_type, shifted=True)
                linked, offsets = TYPE48.bind_candidate(raw, rows, callback_type)
                self.assertEqual(len(linked), TYPE48.SIZE + 1)
                self.assertEqual(len(offsets), 8)
                self.assertEqual(raw, bytes(TYPE48.SIZE + 1))
                base = TYPE48.TYPES[callback_type][0]
                for row, (_, kind, _, address) in zip(
                        rows, TYPE48.expected_fields(callback_type)):
                    self.assertEqual(struct.unpack_from('<I', linked, row['offset'])[0],
                                     TYPE48.encoded_value(base, row['offset'], kind, address))

    def test_249_byte_exact_candidate_can_be_checked_without_shift(self):
        for callback_type in TYPE48.TYPES:
            linked, offsets = TYPE48.bind_candidate(
                bytes(TYPE48.SIZE), self.rows(callback_type), callback_type)
            self.assertEqual(len(linked), TYPE48.SIZE)
            self.assertEqual(offsets, [x[0] for x in TYPE48.expected_fields(callback_type)])

    def test_reject_target_corruption_and_sister_real_misidentification(self):
        for callback_type in TYPE48.TYPES:
            base = self.synthetic_target(callback_type)
            for offset in (0x01B, 0x032, 0x045, 0x054, 0x068, 0x07D, 0x08E, 0x0AC, 0x73):
                corrupted = bytearray(base)
                corrupted[offset] ^= 1
                with self.subTest(kind=callback_type, offset=hex(offset)), self.assertRaises(ValueError):
                    TYPE48.validate_target_fields(corrupted, callback_type)
            other = '8' if callback_type == '4' else '4'
            with self.assertRaises(ValueError):
                TYPE48.validate_target_fields(base, other)
            with self.assertRaises(ValueError):
                TYPE48.validate_target_fields(base[:-1], callback_type)

    def test_reject_missing_aliased_or_fabricated_candidate_fields(self):
        baseline = self.rows('4', shifted=True)
        malformed = [
            baseline[:-1],
            list(reversed(baseline)),
            [dict(baseline[0], type='DIR32')] + baseline[1:],
            [dict(baseline[0], symbol='?Unbound@@3HA')] + baseline[1:],
            [dict(baseline[0], addend=4)] + baseline[1:],
            baseline[:1] + [dict(baseline[1], offset=baseline[0]['offset'] + 1)] + baseline[2:],
            baseline[:-1] + [dict(baseline[-1], offset=TYPE48.SIZE - 2)],
        ]
        for fields in malformed:
            with self.subTest(fields=fields), self.assertRaises(ValueError):
                TYPE48.bind_candidate(bytes(TYPE48.SIZE + 1), fields, '4')
        for size in (TYPE48.SIZE - 1, TYPE48.SIZE + 2):
            with self.assertRaises(ValueError):
                TYPE48.bind_candidate(bytes(size), baseline, '4')


if __name__ == '__main__':
    unittest.main()
