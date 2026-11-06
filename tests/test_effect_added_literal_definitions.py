"""The Effect owner must validate its actual literal, not only its symbol."""
import importlib.util
from pathlib import Path
import struct
from types import SimpleNamespace
import unittest

ROOT = Path(__file__).resolve().parents[1]
spec = importlib.util.spec_from_file_location(
    'effect_added_literal_gate', ROOT/'scripts/inspect-effect-added.py')
audit = importlib.util.module_from_spec(spec)
spec.loader.exec_module(audit)
SYMBOL = '??_C@_09NMANGJFI@etama?4anm?$AA@'


def fixture(payload=b'etama.anm\0', flags=0x40000040):
    header = struct.pack('<HHIIIHH', 0x14C, 1, 0, 0, 0, 0, 0)
    section = bytearray(40)
    section[:8] = b'.rdata\0\0'
    struct.pack_into('<II', section, 16, len(payload), 60)
    struct.pack_into('<I', section, 36, flags)
    row = dict(symbol=SYMBOL, symbol_section=1, symbol_value=0,
               offset=27, addend=0)
    return header + section + payload, row


def original(payload=b'etama.anm\0'):
    def pe_bytes_at(image, address, size):
        if address != 0x0048E488:
            raise ValueError('wrong original literal address')
        return payload.ljust(256, b'\0')[:size]
    return SimpleNamespace(pe_bytes_at=pe_bytes_at)


class EffectAddedLiteralDefinitions(unittest.TestCase):
    def test_actual_read_only_definition_matches_original(self):
        data, row = fixture()
        definitions = audit.check_literals(data, [row], b'', original())
        self.assertEqual(len(definitions), 1)
        self.assertEqual(definitions[0]['size'], 10)
        self.assertEqual(definitions[0]['target'], 0x0048E488)

    def test_correct_name_cannot_hide_changed_bytes_or_terminator(self):
        for payload in (b'Xtama.anm\0', b'etama.anmX', b'etama.anm'):
            data, row = fixture(payload)
            with self.subTest(payload=payload), self.assertRaises(ValueError):
                audit.check_literals(data, [row], b'', original())

    def test_missing_external_or_writable_definition_is_rejected(self):
        data, row = fixture()
        for rows in ([], [dict(row, symbol_section=0)]):
            with self.subTest(rows=rows), self.assertRaises(ValueError):
                audit.check_literals(data, rows, b'', original())
        writable, row = fixture(flags=0xC0000040)
        with self.assertRaises(ValueError):
            audit.check_literals(writable, [row], b'', original())

    def test_original_literal_identity_is_independent(self):
        data, row = fixture(b'Xtama.anm\0')
        with self.assertRaisesRegex(ValueError, 'original TH09 ANM filename'):
            audit.check_literals(data, [row], b'', original(b'Xtama.anm\0'))


if __name__ == '__main__':
    unittest.main()
