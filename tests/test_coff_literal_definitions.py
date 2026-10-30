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
        rows = [dict(self.rows[0], symbol='__real@3ff000000000')]
        with self.assertRaisesRegex(ValueError, 'scalar literal type'):
            self.check(rows=rows, destinations={rows[0]['symbol']: 0x100})
        for row in (dict(self.rows[0], addend=1), dict(self.rows[0], addend=-1),
                    dict(self.rows[1], addend=7), dict(self.rows[0], addend=True)):
            with self.assertRaises(ValueError):
                self.check(rows=[row])
        with self.assertRaises(ValueError):
            self.check(rows=[self.rows[0], dict(self.rows[0], symbol_value=4)])

    def test_double_requires_all_eight_source_and_original_bytes(self):
        symbol = '__real@3f91111111111111'
        payload = struct.pack('<Q', 0x3F91111111111111)
        data = bytearray(self.object)
        data[80:88] = payload
        image = self.image[:0x100] + payload + bytes(256)
        row = dict(self.rows[0], symbol=symbol)
        result = self.check(data=data, image=image, rows=[row],
                            destinations={symbol: 0x100})
        self.assertEqual(result[0]['size'], 8)
        for offset in range(8):
            changed = bytearray(data)
            changed[80+offset] ^= 1
            with self.subTest(source_byte=offset), self.assertRaisesRegex(
                    ValueError, 'literal definition differs'):
                self.check(data=changed, image=image, rows=[row],
                           destinations={symbol: 0x100})
            changed = bytearray(image)
            changed[0x100+offset] ^= 1
            with self.subTest(target_byte=offset), self.assertRaisesRegex(
                    ValueError, 'scalar value differs'):
                self.check(data=data, image=changed, rows=[row],
                           destinations={symbol: 0x100})

    def test_double_cannot_use_four_byte_section_or_interior_reference(self):
        symbol = '__real@0000000000000000'
        data = bytearray(self.object)
        data[80:88] = bytes(8)
        image = self.image[:0x100] + bytes(264)
        row = dict(self.rows[0], symbol=symbol)
        short = bytearray(data)
        struct.pack_into('<I', short, 36, 4)
        for invalid_data, invalid_row in ((short, row),
                                          (data, dict(row, addend=4))):
            with self.assertRaises(ValueError):
                self.check(data=invalid_data, image=image, rows=[invalid_row],
                           destinations={symbol: 0x100})

    def test_wide_string_checks_past_embedded_zero_bytes_and_terminator(self):
        symbol = '??_C@_1test'
        payload = 'AB'.encode('utf-16le') + bytes(2)
        data = bytearray(self.object)
        data[80:80+len(payload)] = payload
        image = self.image[:0x100] + payload + bytes(256)
        row = dict(self.rows[0], symbol=symbol)
        result = self.check(data=data, image=image, rows=[row],
                            destinations={symbol: 0x100})
        self.assertEqual(result[0]['size'], 6)
        for offset in range(len(payload)):
            changed = bytearray(data)
            changed[80+offset] ^= 1
            with self.subTest(offset=offset), self.assertRaises(ValueError):
                self.check(data=changed, image=image, rows=[row],
                           destinations={symbol: 0x100})

    def test_standard_match_unit_checker_rejects_same_named_changed_payload(self):
        import tempfile
        spec = importlib.util.spec_from_file_location(
            'literal_match_unit', ROOT/'scripts/compare-coff-function.py')
        compare = importlib.util.module_from_spec(spec)
        spec.loader.exec_module(compare)
        compare.pe_bytes_at = self.pe_bytes_at
        expected = [dict(offset=row['offset'], target=self.destinations[row['symbol']])
                    for row in self.rows]
        with tempfile.TemporaryDirectory() as folder:
            obj = Path(folder)/'literal.obj'
            obj.write_bytes(self.object)
            result = compare.validate_literal_definitions(
                obj, self.rows, self.image, expected)
            self.assertEqual(len(result), 2)
            changed = bytearray(self.object)
            changed[85] ^= 1
            obj.write_bytes(changed)
            with self.assertRaisesRegex(ValueError, 'literal definition differs'):
                compare.validate_literal_definitions(obj, self.rows, self.image, expected)

    def test_exact_instruction_and_relocation_replay_requires_literal_payload(self):
        import json
        import tempfile
        from unittest import mock
        spec = importlib.util.spec_from_file_location(
            'literal_complete_unit', ROOT/'scripts/compare-coff-function.py')
        compare = importlib.util.module_from_spec(spec)
        spec.loader.exec_module(compare)
        code = bytearray(16)
        original = bytearray(code)
        for row in self.rows:
            struct.pack_into('<I', original, row['offset'],
                             self.destinations[row['symbol']])
        image = bytearray(0x300)
        image[0x100:0x100+len(self.payload)] = self.payload
        image[0x200:0x210] = original
        rows = [dict(row, type='DIR32', type_id=6, owner_section=2,
                     owner_value=0) for row in self.rows]
        with tempfile.TemporaryDirectory() as folder:
            root = Path(folder)
            (root/'scripts').mkdir()
            (root/'scripts/coff_literal_definitions.py').write_bytes(
                (ROOT/'scripts/coff_literal_definitions.py').read_bytes())
            obj = root/'literal.obj'
            obj.write_bytes(self.object)
            manifest = root/'units.toml'
            fields = ',\n'.join(
                '{offset='+str(row['offset'])+', type="DIR32", symbol='+
                json.dumps(row['symbol'])+', target='+
                str(self.destinations[row['symbol']])+'}' for row in self.rows)
            manifest.write_text('[units.literal]\nsymbol="test"\n'
                                'object="literal.obj"\nsize=16\n'
                                'target_address=512\nrelocations=['+fields+']\n')
            with mock.patch.multiple(
                    compare, ROOT=root, UNITS_MANIFEST=manifest,
                    verified_target=lambda: bytes(image),
                    pe_bytes_at=self.pe_bytes_at,
                    object_function=lambda *a, **k: (bytearray(code), rows)):
                self.assertEqual(compare.compare_unit('literal')['result'], 'exact')
                changed = bytearray(self.object)
                changed[85] ^= 1
                obj.write_bytes(changed)
                with self.assertRaisesRegex(ValueError, 'literal definition differs'):
                    compare.compare_unit('literal')


if __name__ == '__main__':
    unittest.main()
