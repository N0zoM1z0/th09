"""Keep ECL wire operand 0x2739 on the existing native GameManager field.

The four compiled whole-owner comparisons validate actual original bytes;
these source checks prevent future raw-pointer alias regressions.
"""
import tomllib
import unittest
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]


class EclNativeGameManagerValueB4(unittest.TestCase):
    def test_all_three_operand_forms_use_the_same_shared_native_member(self):
        source = (ROOT / 'src/EclOperands.cpp').read_text()
        native = (ROOT / 'src/GameManagerRuntimeView.hpp').read_text()
        self.assertIn('EclOperandNativeGameManagerB4', source)
        self.assertIn('offsetof(AsciiGameManagerView, valueB4) == 0x00B4', source)
        self.assertIn('union { int valueB4; int limitB4; };', native)
        self.assertEqual(source.count('case 0x2739:'), 3)
        self.assertIn('case 0x2739: return g_GameManager.valueB4;', source)
        self.assertIn('case 0x2739: return &g_GameManager.valueB4;', source)
        self.assertIn('case 0x2739: return static_cast<float>(g_GameManager.valueB4);', source)
        self.assertNotIn('TH09_ECL_OPERAND_GAME_MANAGER_B4', source)
        self.assertNotIn('reinterpret_cast<unsigned char *>(&g_GameManager) + 0x00B4', source)

    def test_whole_owner_byte_and_field_extent_remains_reviewed(self):
        with (ROOT/'config/match-units.toml').open('rb') as f:
            units = tomllib.load(f)['units']
        counts = (
            ('ecl-resolve-int', 0x405760, 1572, 1980, 165),
            ('ecl-resolve-int-lvalue', 0x405f20, 491, 726, 39),
            ('ecl-resolve-float-lvalue', 0x406210, 702, 968, 49),
            ('ecl-resolve-float', 0x4068a0, 1633, 2044, 121),
        )
        for name, address, size, compare_size, fields in counts:
            with self.subTest(name=name):
                unit = units[name]
                self.assertEqual(unit['target_address'], address)
                self.assertEqual(unit['size'], size)
                self.assertEqual(unit['compare_size'], compare_size)
                self.assertEqual(len(unit['relocations']), fields)
                self.assertTrue(all(r['target'] > 0 and r['symbol'] for r in unit['relocations']))


if __name__ == '__main__':
    unittest.main()
