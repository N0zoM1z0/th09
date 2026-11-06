"""Wrong source/target scalar definitions must not pass the movement Oracle."""
import importlib.util
from pathlib import Path
import struct
import unittest

ROOT = Path(__file__).resolve().parents[1]
spec = importlib.util.spec_from_file_location(
    'movement_literal_test', ROOT / 'scripts/inspect-player-movement.py'
)
MOVEMENT = importlib.util.module_from_spec(spec)
spec.loader.exec_module(MOVEMENT)


def object_with_literal(payload=bytes(4), flags=0x40000040):
    data = bytearray(60) + payload
    struct.pack_into('<HH', data, 0, 0x14C, 1)
    struct.pack_into('<II', data, 36, len(payload), 60)
    struct.pack_into('<I', data, 56, flags)
    return bytes(data)


def reference():
    return dict(symbol='__real@00000000', offset=0x123, type='DIR32', addend=0,
                symbol_section=1, symbol_value=0)


class OriginalScalar:
    def pe_bytes_at(self, image, address, size):
        if address != 0x48E314 or size != 4:
            raise AssertionError('wrong original scalar identity')
        return image


class PlayerMovementLiteralDefinitions(unittest.TestCase):
    def check(self, data, fields=None, original=bytes(4)):
        return MOVEMENT.validate_movement_literals(
            data, [reference()] if fields is None else fields,
            original, OriginalScalar()
        )

    def test_defined_read_only_zero_is_checked_against_original(self):
        definitions = self.check(object_with_literal())
        self.assertEqual(len(definitions), 1)
        self.assertEqual(definitions[0]['target'], 0x48E314)
        self.assertEqual(definitions[0]['size'], 4)
        self.assertEqual(definitions[0]['references'], [0x123])

    def test_matching_name_does_not_hide_changed_source_value(self):
        with self.assertRaisesRegex(ValueError, 'source/original literal definition differs'):
            self.check(object_with_literal(struct.pack('<f', 0.5)))

    def test_undefined_writable_or_truncated_source_is_rejected(self):
        cases = [
            (object_with_literal(), {**reference(), 'symbol_section': 0}),
            (object_with_literal(flags=0xC0000040), reference()),
            (object_with_literal(bytes(3)), reference()),
        ]
        for data, row in cases:
            with self.subTest(data=data, row=row), self.assertRaises(ValueError):
                self.check(data, [row])

    def test_required_movement_reference_cannot_be_omitted(self):
        with self.assertRaisesRegex(ValueError, 'required zero literal'):
            self.check(object_with_literal(), [])

    def test_original_zero_identity_is_independent_of_source_payload(self):
        with self.assertRaisesRegex(ValueError, 'name/original scalar value differs'):
            self.check(object_with_literal(), original=struct.pack('<f', 0.5))


if __name__ == '__main__':
    unittest.main()
