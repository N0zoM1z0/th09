"""Catalog completeness needs original table evidence, including interior entries."""
import hashlib
import importlib.util
from pathlib import Path
import struct
import unittest

ROOT = Path(__file__).resolve().parents[1]
SPEC = importlib.util.spec_from_file_location('callback_coverage', ROOT/'scripts/report-callback-coverage.py')
AUDIT = importlib.util.module_from_spec(SPEC)
SPEC.loader.exec_module(AUDIT)


class CallbackCoverageTests(unittest.TestCase):
    def fixture(self):
        data = struct.pack('<6I', 35, 0, 1, 0x401000, 0x401020, 0)
        data += struct.pack('<6I', 36, 0, 1, 0x401000, 0x401004, 0)
        return data

    def decode(self, data, digest=None):
        return AUDIT.decode_prefix(data, 0x4A0B48, 2, 24, 12,
            ('initialize', 'update', 'draw'), digest or hashlib.sha256(data).hexdigest(),
            [(0x401000, 0x402000)])

    def test_shared_callback_keeps_each_original_slot_and_nulls_are_skipped(self):
        slots = self.decode(self.fixture())
        self.assertEqual(len(slots), 4)
        self.assertEqual([r['pointer_address'] for r in slots],
                         [0x4A0B54, 0x4A0B58, 0x4A0B6C, 0x4A0B70])
        self.assertEqual([r['role'] for r in slots], ['initialize', 'update']*2)

    def test_wrong_digest_extent_and_rodata_callbacks_are_rejected(self):
        data = self.fixture()
        for bad, digest in ((data, '0'*64), (data[:-1], None),
                            (data + bytes(24), None),
                            (data[:12] + struct.pack('<I', 0x48E500) + data[16:], None)):
            with self.subTest(size=len(bad)), self.assertRaises(ValueError):
                self.decode(bad, digest)
        with self.assertRaises(ValueError):
            AUDIT.decode_prefix(data, 0x4A0B48, 2, 24, 8,
                ('initialize', 'update', 'draw'), hashlib.sha256(data).hexdigest(),
                [(0x401000, 0x402000)])

    def test_interior_pointer_is_distinct_from_an_uncatalogued_entry(self):
        functions = [{'address': '0x00401000', 'span_end': '0x0040100F',
                      'proposed_name': 'ObservedOwner', 'status': 'unclassified'}]
        rows = AUDIT.classify_entries(self.decode(self.fixture()), functions)
        self.assertEqual([r['catalog_state'] for r in rows],
                         ['listed-entry', 'interior-entry-needs-review', 'missing-entry'])
        self.assertEqual(len(rows[0]['slots']), 2)
        self.assertEqual(rows[1]['containing_catalog_entries'], ['0x00401000'])
        self.assertEqual(rows[0]['status'], 'unclassified')
        self.assertNotIn('exactness_credit', rows[0])
        with self.assertRaises(ValueError):
            AUDIT.classify_entries(self.decode(self.fixture()), functions*2)

    def test_pe_section_flags_separate_executable_and_data_ranges(self):
        data = bytearray(0x300)
        struct.pack_into('<I', data, 0x3C, 0x80)
        data[0x80:0x84] = b'PE\0\0'
        struct.pack_into('<HHIIIHH', data, 0x84, 0x14C, 2, 0, 0, 0, 0xE0, 0)
        struct.pack_into('<H', data, 0x98, 0x10B)
        struct.pack_into('<I', data, 0x98+28, 0x400000)
        for n, (rva, flags) in enumerate(((0x1000, 0x60000020), (0x2000, 0x40000040))):
            struct.pack_into('<8sIIIIIIHHI', data, 0x178+40*n,
                b'.text' if n == 0 else b'.rdata', 0x100, rva, 0x200, 0, 0, 0, 0, 0, flags)
        self.assertEqual(AUDIT.executable_intervals(data), [(0x401000, 0x401200)])
        struct.pack_into('<H', data, 0x84, 0x8664)
        with self.assertRaises(ValueError):
            AUDIT.executable_intervals(data)


if __name__ == '__main__':
    unittest.main()
