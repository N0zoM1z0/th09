"""CardAttack and PlayerDeath share the original GameManager +0xB0 value receiver."""
from pathlib import Path
import tomllib
import unittest

ROOT=Path(__file__).resolve().parents[1]
SYMBOL='?SubtractSaturating@GameManagerValueCCView@@QAEXH@Z'

class NativeValueCC(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        with (ROOT/'config/match-units.toml').open('rb') as f:
            cls.units=tomllib.load(f)['units']

    def test_real_root_and_one_callee(self):
        header=(ROOT/'src/GameManagerValueCC.hpp').read_text()
        source=(ROOT/'src/GameManagerValueCC.cpp').read_text()
        self.assertIn('offsetof(AsciiGameManagerView, valueB0) == 0xB0',header)
        self.assertIn('offsetof(AsciiGameManagerView, valueCC) == 0xCC',header)
        self.assertIn('sizeof(GameManagerValueCCView) == 0x20',header)
        self.assertIn('value1C -= amount;',source)
        self.assertIn('if (value1C < 0)',source)
        leaf=self.units['game-manager-valuecc-subtract-saturating']
        self.assertEqual((leaf['symbol'],leaf['target_address'],leaf['size']),
                         (SYMBOL,0x00403E30,27))

    def test_both_large_exact_callers(self):
        cases=(('card-attack-start',0x2B,0x33,923),
               ('player-enter-death-state',0x2B4,0x2B9,753))
        for unit_name, root_off, call_off, size in cases:
            with self.subTest(unit=unit_name):
                u=self.units[unit_name]
                self.assertEqual(u['size'],size)
                refs={r['offset']:r for r in u['relocations']}
                root=refs[root_off]
                self.assertEqual((root['symbol'],root['target'],root.get('addend',0),root['type']),
                                 ('?g_GameManager@@3UAsciiGameManagerView@@A',0x004A7D90,0xB0,'DIR32'))
                call=refs[call_off]
                self.assertEqual((call['symbol'],call['target'],call['type']),
                                 (SYMBOL,0x00403E30,'REL32'))

if __name__=='__main__': unittest.main()
