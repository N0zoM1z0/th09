"""Independent input gates for source-present TH09 Title OnUpdateResult."""

import hashlib
import importlib.util
from pathlib import Path
import struct
import unittest

try:
    import capstone
except ImportError:  # python -S public CI deliberately excludes site packages.
    capstone = None

ROOT = Path(__file__).resolve().parents[1]
spec = importlib.util.spec_from_file_location(
    'title_result_update_inspector', ROOT / 'scripts/inspect-title-update-result.py')
result = importlib.util.module_from_spec(spec)
spec.loader.exec_module(result)


class TitleResultUpdateGates(unittest.TestCase):
    @staticmethod
    def synthetic():
        data = bytearray(result.SIZE)
        for off, kind, _, address in result.FIELDS:
            struct.pack_into('<I', data, off, result.encode(kind, address, off))
        return bytes(data), hashlib.sha256(data).hexdigest()

    @staticmethod
    def rows(shift=0):
        return [dict(offset=off + shift, type=kind, symbol=symbol, addend=0)
                for off, kind, symbol, _ in result.FIELDS]

    def test_original_has_34_fields_and_nine_calls(self):
        self.assertEqual(len(result.FIELDS), 34)
        self.assertEqual(len(result.CALLS), 9)
        self.assertEqual([off for off, _, _, _ in result.FIELDS],
                         sorted(off for off, _, _, _ in result.FIELDS))
        data, digest = self.synthetic()
        result.validate_original(data, target_hash=digest)

    def test_original_hash_and_each_field_encoding_are_independently_required(self):
        data, digest = self.synthetic()
        for off, _, _, _ in result.FIELDS:
            bad = bytearray(data)
            bad[off] ^= 0x80
            with self.subTest(off=hex(off)), self.assertRaises(ValueError):
                result.validate_original(bytes(bad), target_hash=hashlib.sha256(bad).hexdigest())
        with self.assertRaises(ValueError):
            result.validate_original(data, target_hash='0' * 64)
        with self.assertRaises(ValueError):
            result.validate_original(data[:-1], target_hash=digest)

    def test_source_field_payload_uses_each_real_candidate_offset(self):
        for shift in (0, 1):
            rows = self.rows(shift=shift)
            linked = result.bind_candidate(bytes(result.SIZE), rows)
            self.assertEqual(len(linked), result.SIZE)
            for row, (_, kind, _, target) in zip(rows, result.FIELDS):
                self.assertEqual(struct.unpack_from('<I', linked, row['offset'])[0],
                                 result.encode(kind, target, row['offset']))
        self.assertEqual(len(result.bind_candidate(bytes(483), self.rows())), 483)

    def test_wrong_or_fabricated_source_fields_fail_closed(self):
        rows = self.rows(shift=1)
        tests = [rows[:-1], list(reversed(rows)),
                 [dict(rows[0], type='REL32')] + rows[1:],
                 [dict(rows[0], symbol='?injectedUnknown@@3HA')] + rows[1:],
                 [dict(rows[0], addend=4)] + rows[1:],
                 rows[:1] + [dict(rows[1], offset=rows[0]['offset'] + 1)] + rows[2:],
                 rows[:-1] + [dict(rows[-1], offset=result.SIZE - 2)]]
        for candidate in tests:
            with self.subTest(candidate=candidate[:2]), self.assertRaises(ValueError):
                result.bind_candidate(bytes(result.SIZE), candidate)
        for extent in (449, 521):
            with self.assertRaises(ValueError):
                result.bind_candidate(bytes(extent), rows)

    @unittest.skipIf(capstone is None, 'Capstone unavailable under python -S')
    def test_decoder_distinguishes_jump_displacements_from_external_referents(self):
        # mov eax,[absolute], call next, jnz next, ret
        code = bytes.fromhex('a1ac1d4a00e8000000007500c3')
        ins, fields, calls = result.decoded_fields(code)
        self.assertEqual(sum(i.size for i in ins), len(code))
        self.assertEqual(fields, [(1, 'DIR32', 0x4A1DAC),
                                  (6, 'REL32', result.BASE + 10)])
        self.assertEqual(calls, [result.BASE + 10])

    @unittest.skipIf(capstone is None, 'Capstone unavailable under python -S')
    def test_wrong_abi_or_indirect_call_does_not_pass(self):
        for code in (bytes.fromhex('ffd0c3'), bytes.fromhex('c21000')):
            with self.assertRaises(ValueError):
                result.decoded_fields(code)


if __name__ == '__main__':
    unittest.main()
