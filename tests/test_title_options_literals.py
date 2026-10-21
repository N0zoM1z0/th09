"""Options must read its actual COFF string, even when code/fields agree."""
import importlib.util
from pathlib import Path
import struct
import unittest

ROOT = Path(__file__).resolve().parents[1]
spec = importlib.util.spec_from_file_location(
    'options_literals', ROOT / 'scripts/inspect-title-options.py')
audit = importlib.util.module_from_spec(spec)
spec.loader.exec_module(audit)


class OptionsLiteralDefinitions(unittest.TestCase):
    def setUp(self):
        self.symbol = '??_C@_06EAGAPDIF@SetVol?$AA@'
        self.payload = b'SetVol\0'
        self.object = bytearray(80 + len(self.payload))
        struct.pack_into('<HH', self.object, 0, 0x14C, 1)
        struct.pack_into('<II', self.object, 36, len(self.payload), 80)
        struct.pack_into('<I', self.object, 56, 0x40000040)
        self.object[80:] = self.payload
        self.rows = [dict(symbol=self.symbol, symbol_section=1, symbol_value=0,
                          offset=20, addend=0)]

    def pe_bytes_at(self, image, address, size):
        self.assertEqual(address, audit.DESTINATIONS[self.symbol])
        return self.payload[:size]

    def check(self, data):
        return audit.check_local_definitions(data, self.rows, b'fixture image', self)

    def test_one_actual_definition_covers_repeated_source_references(self):
        self.rows.append(dict(self.rows[0], offset=40))
        definitions = self.check(self.object)
        self.assertEqual(len(definitions), 1)
        self.assertEqual(definitions[0]['size'], 7)
        self.assertEqual(definitions[0]['references'], [20, 40])

    def test_unchanged_fields_cannot_hide_string_terminator_or_provenance_corruption(self):
        for offset, mask in ((80, 1), (86, 1), (59, 0x80)):
            changed = bytearray(self.object)
            changed[offset] ^= mask
            with self.subTest(offset=offset), self.assertRaises(ValueError):
                self.check(changed)


if __name__ == '__main__':
    unittest.main()
