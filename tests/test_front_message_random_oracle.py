"""Target-independent corruption cases for original-bound Front weighted RNG."""
import hashlib
import importlib.util
from pathlib import Path
import struct
import unittest

ROOT = Path(__file__).resolve().parents[1]
spec = importlib.util.spec_from_file_location(
    'front_random', ROOT / 'scripts/inspect-front-message-random.py')
front = importlib.util.module_from_spec(spec)
spec.loader.exec_module(front)


class FrontRandomOriginalTests(unittest.TestCase):
    @staticmethod
    def synthetic_original():
        data = bytearray(front.SIZE)
        for off, kind, dest in front.ORIGINAL_FIELDS:
            struct.pack_into('<I', data, off, front.encoded(kind, dest, off))
        return bytes(data)

    @staticmethod
    def matching_source_rows():
        rows = []
        for n, (_, kind, dest) in enumerate(front.ORIGINAL_FIELDS):
            choices = [(symbol, addend)
                       for symbol, (declared_kind, base, addends) in front.REFERENTS.items()
                       for addend in addends
                       if declared_kind == kind and base + addend == dest]
            if len(choices) != 1:
                raise AssertionError('ambiguous original-backed symbol identity')
            symbol, addend = choices[0]
            rows.append({'offset': 8 + 12 * n, 'type': kind,
                         'symbol': symbol, 'addend': addend})
        return rows

    def test_target_manifest_checks_sha256_and_all_15_occupied_operands(self):
        self.assertEqual(len(front.ORIGINAL_FIELDS), 15)
        self.assertEqual(len(front.CALL_ORDER), 6)
        raw = self.synthetic_original()
        front.original_manifest(raw, expected_sha=hashlib.sha256(raw).hexdigest())
        with self.assertRaisesRegex(ValueError, 'SHA-256'):
            front.original_manifest(raw)
        with self.assertRaisesRegex(ValueError, 'SHA-256'):
            front.original_manifest(raw[:-1], expected_sha=hashlib.sha256(raw[:-1]).hexdigest())
        for off, _, _ in front.ORIGINAL_FIELDS:
            modified = bytearray(raw)
            modified[off] ^= 1
            with self.subTest(offset=off), self.assertRaisesRegex(ValueError, 'address field'):
                front.original_manifest(bytes(modified),
                                         expected_sha=hashlib.sha256(modified).hexdigest())

    def test_binds_actual_candidate_offsets_not_original_offsets(self):
        raw = bytes(682)
        rows = self.matching_source_rows()
        linked, bound = front.bind_candidate(raw, rows)
        self.assertEqual(len(linked), len(raw))
        self.assertEqual(len(bound), 15)
        for row, (_, kind, dest) in zip(rows, front.ORIGINAL_FIELDS):
            self.assertEqual(struct.unpack_from('<I', linked, row['offset'])[0],
                             front.encoded(kind, dest, row['offset']))
        changed = [dict(row) for row in rows]
        changed[1]['offset'] += 1
        linked_new, bound_new = front.bind_candidate(raw, changed)
        self.assertNotEqual(bound_new[1][0], bound[1][0])
        self.assertEqual(bound_new[1][2], bound[1][2])
        self.assertNotEqual(linked_new, linked)

    def test_unknown_symbol_wrong_type_addend_and_field_reorder_rejected(self):
        source = self.matching_source_rows()
        broken = [source[:14], source + [source[-1]],
                  [dict(source[0], symbol='?not_reviewed@@3HA')] + source[1:],
                  [dict(source[0], type='REL32')] + source[1:],
                  [dict(source[0], addend=0)] + source[1:],
                  [dict(source[0], offset=-1)] + source[1:],
                  [dict(source[0], offset=681)] + source[1:],
                  source[:2] + [source[3], source[2]] + source[4:],
                  [dict(source[0], offset=source[1]['offset'])] + source[1:]]
        for rows in broken:
            with self.subTest(rows=rows[:2]), self.assertRaises(ValueError):
                front.bind_candidate(bytes(682), rows)
        with self.assertRaisesRegex(ValueError, 'whole body'):
            front.bind_candidate(bytes(599), source)

    def test_original_relocation_calls_order_and_absolutes_are_distinct(self):
        self.assertEqual(front.CALL_ORDER,
                         (0x4048B0, 0x4048B0, 0x403E00, 0x403E00, 0x43BDC0, 0x43BDC0))
        self.assertEqual(front.ORIGINAL_FIELDS[0], (0x11, 'DIR32', 0x4A7DE8))
        self.assertEqual(front.ORIGINAL_FIELDS[-1], (0x2A5, 'REL32', 0x43BDC0))
        self.assertNotEqual(front.encoded('REL32', 0x4048B0, 0x56),
                            front.encoded('REL32', 0x4048B0, 0x9A))
        with self.assertRaises(ValueError):
            front.encoded('PCREL16', 0x4048B0, 0x56)
