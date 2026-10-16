"""Target-independent falsifiers for full Type19/21 original address binding.

The cold factory/VC7.1 path separately reads the SHA-pinned Japanese PE and
actual COFF source. A matching field *count* alone never proves the identities.
"""

import hashlib
import importlib.util
from pathlib import Path
import struct
import unittest

ROOT = Path(__file__).resolve().parents[1]
spec = importlib.util.spec_from_file_location(
    'exattack_trail_original_fields', ROOT/'scripts/inspect-exattack-trail-init.py')
trail = importlib.util.module_from_spec(spec)
spec.loader.exec_module(trail)


class OriginalTrailAddressFieldBinding(unittest.TestCase):
    BASE = 0x00440000
    GLOBAL = 0x004A80E8
    CALLEE = 0x00401560

    @classmethod
    def fixture(cls):
        # MOV ECX,[genuine-global]; CALL genuine-callee; RET
        target = (bytes.fromhex('8b0d') + struct.pack('<I', cls.GLOBAL) +
                  bytes.fromhex('e8') +
                  struct.pack('<I', (cls.CALLEE - cls.BASE - 11) & 0xFFFFFFFF) +
                  bytes.fromhex('c3'))
        candidate = bytearray(target)
        candidate[2:6] = bytes(4)
        candidate[7:11] = bytes(4)
        fields = [(2, 'DIR32', cls.GLOBAL), (7, 'REL32', cls.CALLEE)]
        rows = [
            {'offset': 2, 'type': 'DIR32', 'symbol': 'global', 'addend': 0},
            {'offset': 7, 'type': 'REL32', 'symbol': 'callee', 'addend': 0},
        ]
        kwargs = {
            'base': cls.BASE, 'destinations': {'global': cls.GLOBAL, 'callee': cls.CALLEE},
            'original_digest': hashlib.sha256(target).hexdigest(),
            'expected_count': 2,
        }
        return target, bytes(candidate), rows, fields, kwargs

    def test_complete_source_fields_bind_to_original_exactly(self):
        target, candidate, rows, fields, kw = self.fixture()
        self.assertEqual(trail.bind_original_fields(target, candidate, rows, fields, **kw), target)
        self.assertEqual(trail.ORIGINAL_OWNER['19'][:3], (0x44A030, 763, 57))
        self.assertEqual(trail.ORIGINAL_OWNER['21'][:3], (0x44B220, 727, 53))

    def test_rejects_wrong_origin_wrong_symbol_or_wrong_field_order(self):
        target, candidate, rows, fields, kw = self.fixture()
        cases = (
            (rows, fields, {**kw, 'original_digest': '0'*64}),
            (rows, fields, {**kw, 'expected_count': 3}),
            ([{**rows[0], 'symbol': 'callee'}, rows[1]], fields, kw),
            ([{**rows[0], 'offset': 3}, rows[1]], fields, kw),
            ([{**rows[0], 'type': 'REL32'}, rows[1]], fields, kw),
            ([{**rows[0], 'addend': 4}, rows[1]], fields, kw),
            ([rows[1], rows[0]], fields, kw),
            (rows[:1], fields, kw),
            (rows, [(2, 'DIR32', self.CALLEE), fields[1]], kw),
            (rows, [fields[0], (2, 'REL32', self.CALLEE)], kw),
            ([{**rows[0], 'offset': True}, rows[1]], fields, kw),
        )
        for changed_rows, changed_fields, options in cases:
            with self.subTest(rows=changed_rows, fields=changed_fields,
                              digest=options['original_digest']), self.assertRaises(ValueError):
                trail.bind_original_fields(target, candidate, changed_rows,
                                           changed_fields, **options)
        with self.assertRaises(ValueError):
            trail.bind_original_fields(target[:-1], candidate[:-1], rows, fields, **kw)
        with self.assertRaises(ValueError):
            trail.bind_original_fields(target, candidate + b'\x90', rows, fields, **kw)

    def test_original_encoded_instruction_field_cannot_be_forged(self):
        target, candidate, rows, fields, kw = self.fixture()
        altered = bytearray(target)
        altered[7] ^= 1
        changed = {**kw, 'original_digest': hashlib.sha256(altered).hexdigest()}
        with self.assertRaises(ValueError):
            trail.bind_original_fields(bytes(altered), candidate, rows, fields, **changed)

    def test_read_retail_x86_fields_independent_of_candidate(self):
        try:
            import capstone
        except ImportError:
            self.skipTest('Capstone unavailable in target-independent CI')
        target, candidate, rows, fields, kw = self.fixture()
        self.assertEqual(trail.address_operands(target, self.BASE, capstone), fields)
        with self.assertRaises(ValueError):
            trail.address_operands(b'\xff\xd0\xc3', self.BASE, capstone)
        self.assertEqual(trail.address_operands(
            trail.bind_original_fields(target, candidate, rows, fields, **kw),
            self.BASE, capstone), fields)


if __name__ == '__main__':
    unittest.main()
