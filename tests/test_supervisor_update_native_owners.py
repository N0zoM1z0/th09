"""Native callable and version-definition guards; no private target/compiler."""
import importlib.util
from pathlib import Path
import struct
import unittest

ROOT = Path(__file__).resolve().parents[1]
SPEC = importlib.util.spec_from_file_location(
    "supervisor_update_native_owners", ROOT / "scripts/inspect-supervisor-update.py")
DIAGNOSTIC = importlib.util.module_from_spec(SPEC)
SPEC.loader.exec_module(DIAGNOSTIC)


def object_fixture():
    # Two bounded actual sections: an unchanged eight-byte owner and its literal.
    data = bytearray(114)
    struct.pack_into('<HHIIIHH', data, 0, 0x14C, 2, 0, 0, 0, 0, 0)
    struct.pack_into('<8sIIIIIIHHI', data, 20, b'.text\0\0\0',
                     0, 0, 8, 100, 0, 0, 0, 0, 0x60000020)
    struct.pack_into('<8sIIIIIIHHI', data, 60, b'.rdata\0\0',
                     0, 0, 6, 108, 0, 0, 0, 0, 0x40000040)
    data[100:108] = b'CODEBODY'
    data[108:114] = b'0150a\0'
    row = dict(symbol=DIAGNOSTIC.VERSION_LITERAL_SYMBOL, type='DIR32', addend=0,
               offset=2, symbol_section=2, symbol_value=0, owner_section=1, owner_value=0)
    return data, row


class SupervisorNativeOwnerTests(unittest.TestCase):
    def test_reads_real_definition_and_original_payload(self):
        data, row = object_fixture()
        result = DIAGNOSTIC.check_version_literal(data, [row], b'0150a\0')
        self.assertEqual(result['payload_hex'], '303135306100')
        self.assertEqual(result['size'], 6)
        self.assertEqual(result['target'], 0x0048F1C4)
        self.assertEqual(result['symbol_section'], 2)

    def test_data_only_corruption_fails_although_code_is_identical(self):
        data, row = object_fixture()
        changed = bytearray(data)
        changed[109] = ord('9')
        self.assertEqual(data[100:108], changed[100:108])
        with self.assertRaisesRegex(ValueError, 'payload differs'):
            DIAGNOSTIC.check_version_literal(changed, [row], b'0150a\0')
        with self.assertRaisesRegex(ValueError, 'payload differs'):
            DIAGNOSTIC.check_version_literal(data, [row], b'0150b\0')

    def test_name_or_address_without_definition_is_insufficient(self):
        data, row = object_fixture()
        for changes in ({'symbol_section': 0}, {'symbol_section': 3},
                        {'symbol_value': -1}, {'symbol_value': 1}, {'addend': 1},
                        {'type': 'REL32'}, {'symbol': '?g_VersionString@@3PADA'}):
            with self.subTest(changes=changes), self.assertRaises(ValueError):
                DIAGNOSTIC.check_version_literal(data, [{**row, **changes}], b'0150a\0')
        for rows in ([], [row, row]):
            with self.subTest(rows=rows), self.assertRaises(ValueError):
                DIAGNOSTIC.check_version_literal(data, rows, b'0150a\0')

    def test_section_bounds_attributes_and_relocations_fail_closed(self):
        data, row = object_fixture()
        for offset, fmt, value in ((0, '<H', 0x8664), (16, '<H', 2),
                (76, '<I', 5), (80, '<I', 20), (80, '<I', 114),
                (92, '<H', 1), (96, '<I', 0xC0000040),
                (96, '<I', 0x60000020), (96, '<I', 0x40000080),
                (96, '<I', 0x00000040), (96, '<I', 0x400000C0),
                (96, '<I', 0x60000040), (96, '<I', 0x40000060)):
            altered = bytearray(data)
            struct.pack_into(fmt, altered, offset, value)
            with self.subTest(offset=offset, value=value), self.assertRaises(ValueError):
                DIAGNOSTIC.check_version_literal(altered, [row], b'0150a\0')
        for length in (19, 99, 113):
            with self.subTest(length=length), self.assertRaises(ValueError):
                DIAGNOSTIC.check_version_literal(data[:length], [row], b'0150a\0')

    def test_obsolete_undefined_aliases_are_not_normalized_to_native_owners(self):
        local = dict(offset=4, type='DIR32', symbol='$table', addend=0,
                     symbol_section=1, symbol_value=24, owner_section=1, owner_value=0)
        field = dict(local, offset=12, symbol_section=0, symbol_value=0)
        for old in ('?RegisterStateObject@@YIHH@Z', '?RegisterState9Object@@YIHXZ',
                    '?g_VersionString@@3PADA', '?GameManager_RegisterChain@@YIHXZ'):
            self.assertNotIn(old, DIAGNOSTIC.DESTINATIONS)
            with self.subTest(old=old), self.assertRaisesRegex(ValueError, 'unreviewed external'):
                DIAGNOSTIC.bind(bytes(32), [local, dict(field, symbol=old)])
        self.assertEqual(DIAGNOSTIC.DESTINATIONS['?RegisterTitleScreen@@YIHH@Z'], 0x0042AB13)
        self.assertEqual(DIAGNOSTIC.DESTINATIONS['?RegisterChain@Ending@@SIHXZ'], 0x0040F080)
        self.assertEqual(DIAGNOSTIC.DESTINATIONS['?RegisterChain@GameManagerSetupLayout@@SIHXZ'], 0x0041B9D0)


if __name__ == '__main__':
    unittest.main()
