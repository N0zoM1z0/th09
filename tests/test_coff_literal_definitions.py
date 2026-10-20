"""Local literal contents/provenance cannot be replaced by a matching name."""
import importlib.util
from pathlib import Path
import struct
import unittest

ROOT = Path(__file__).resolve().parents[1]
spec = importlib.util.spec_from_file_location(
    'coff_literals', ROOT/'scripts/coff_literal_definitions.py')
audit = importlib.util.module_from_spec(spec)
spec.loader.exec_module(audit)


class CoffLiteralDefinitionTests(unittest.TestCase):
    def setUp(self):
        self.scalar = '__real@3fa00000'
        self.string = '??_C@_test_format'
        self.payload = struct.pack('<I', 0x3FA00000) + b'row=%d\0'
        self.object = bytearray(80 + len(self.payload))
        struct.pack_into('<HH', self.object, 0, 0x14C, 1)
        struct.pack_into('<II', self.object, 36, len(self.payload), 80)
        struct.pack_into('<I', self.object, 56, 0x40000040)
        self.object[80:] = self.payload
        self.rows = [dict(symbol=self.scalar, symbol_section=1, symbol_value=0,
                          offset=2, addend=0),
                     dict(symbol=self.string, symbol_section=1, symbol_value=4,
                          offset=9, addend=0)]
        self.destinations = {self.scalar: 0x100, self.string: 0x104}
        self.image = bytes(0x100) + self.payload + bytes(256)

    @staticmethod
    def pe_bytes_at(image, address, size):
        if address < 0 or address+size > len(image):
            raise ValueError('outside test image')
        return image[address:address+size]

    def check(self, *, data=None, rows=None, image=None, destinations=None):
        return audit.check_literal_definitions(
            self.object if data is None else data,
            self.rows if rows is None else rows,
            self.image if image is None else image, self,
            self.destinations if destinations is None else destinations)

    def test_repeated_references_count_one_actual_definition(self):
        result = self.check(rows=self.rows + [dict(self.rows[0], offset=20)])
        self.assertEqual(len(result), 2)
        self.assertEqual(result[0]['references'], [2, 20])
        self.assertEqual(result[1]['size'], len(b'row=%d\0'))
        self.assertEqual(result[0]['section'], 1)
        self.assertEqual(result[1]['value'], 4)

    def test_mutated_scalar_string_or_terminator_is_rejected_with_same_rows(self):
        for offset in (80, 84, 80+len(self.payload)-1):
            changed = bytearray(self.object)
            changed[offset] ^= 1
            with self.subTest(offset=offset), self.assertRaisesRegex(
                    ValueError, 'literal definition differs'):
                self.check(data=changed)

    def test_original_scalar_string_and_bounded_terminator_are_independent(self):
        for offset in (0x100, 0x104):
            changed = bytearray(self.image)
            changed[offset] ^= 1
            with self.subTest(offset=offset), self.assertRaises(ValueError):
                self.check(image=changed)
        with self.assertRaisesRegex(ValueError, 'unbounded'):
            self.check(image=self.image[:0x104] + b'x'*256)

    def test_undefined_or_out_of_section_coordinates_are_rejected(self):
        for change in ({'symbol_section': 0}, {'symbol_section': 2},
                       {'symbol_section': True}, {'symbol_value': -1},
                       {'symbol_value': len(self.payload)-3}):
            with self.subTest(change=change), self.assertRaises(ValueError):
                self.check(rows=[{**self.rows[0], **change}])
        for data in (self.object[:19], self.object[:59], self.object[:-1]):
            with self.assertRaises(ValueError):
                self.check(data=data)

    def test_machine_optional_header_section_bounds_and_flags_are_required(self):
        for offset, fmt, value in ((0, '<H', 0x8664), (16, '<H', 4),
                                   (40, '<I', 0), (36, '<I', 3),
                                   (56, '<I', 0xC0000040),
                                   (56, '<I', 0x40000020),
                                   (56, '<I', 0x40)):
            data = bytearray(self.object)
            struct.pack_into(fmt, data, offset, value)
            with self.subTest(offset=offset, value=value), self.assertRaises(ValueError):
                self.check(data=data)

    def test_unknown_literal_width_interior_reference_and_provenance_fail(self):
        for destinations in ({}, {self.scalar: 0x101, self.string: 0x104}):
            with self.assertRaises(ValueError):
                self.check(destinations=destinations)
        rows = [dict(self.rows[0], symbol='__real@3ff0000000000000')]
        with self.assertRaisesRegex(ValueError, 'scalar literal type'):
            self.check(rows=rows, destinations={rows[0]['symbol']: 0x100})
        for row in (dict(self.rows[0], addend=1), dict(self.rows[0], addend=-1),
                    dict(self.rows[1], addend=7), dict(self.rows[0], addend=True)):
            with self.assertRaises(ValueError):
                self.check(rows=[row])
        with self.assertRaises(ValueError):
            self.check(rows=[self.rows[0], dict(self.rows[0], symbol_value=4)])


if __name__ == '__main__':
    unittest.main()
