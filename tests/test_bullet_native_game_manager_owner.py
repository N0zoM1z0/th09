"""Original TH09 BulletManager GameManager ABI after removal of private owner."""
from pathlib import Path
import tomllib
import unittest

ROOT = Path(__file__).resolve().parents[1]


class BulletNativeGameManagerOwner(unittest.TestCase):
    def test_large_etama_update_and_register_use_real_global_and_side_slots(self):
        source = (ROOT/'src/BulletManager.cpp').read_text()
        self.assertIn('#include "AsciiGameManagerView.hpp"', source)
        self.assertIn('BulletNativeGameManagerOwner', source)
        self.assertNotIn('struct BulletGameManagerView', source)
        self.assertNotIn('extern BulletGameManagerView g_GameManager;', source)
        self.assertIn('offsetof(AsciiGameManagerView, flags) == 0x134', source)
        self.assertIn('sizeof(AsciiGameManagerSideView) == 0x38', source)
        self.assertIn('&g_GameManager.sides[sideIndex]', source)
        self.assertIn('&g_GameManager.sides[1 - sideIndex]', source)
        with (ROOT/'config/match-units.toml').open('rb') as f:
            units = tomllib.load(f)['units']
        native_global = '?g_GameManager@@3UAsciiGameManagerView@@A'
        native_method = '?IsWithinPlayfield@AsciiGameManagerView@@QAEHMMMM@Z'
        self.assertEqual((units['game-manager-is-within-playfield']['target_address'],
                          units['game-manager-is-within-playfield']['size'],
                          units['game-manager-is-within-playfield']['symbol']),
                         (0x0041A6EB, 116, native_method))
        expected = {
            'bullet-update-boundary-bounce': [(native_global, 0x004A7D90),
                                              (native_method, 0x0041A6EB)],
            'etama-controller-register': [(native_global, 0x004A7D90)]*2,
            'etama-controller-update': [(native_global, 0x004A7D90),
                                        (native_global, 0x004A7D90),
                                        (native_method, 0x0041A6EB)],
        }
        for name, names in expected.items():
            with self.subTest(unit=name):
                self.assertEqual([(r['symbol'],r['target']) for r in units[name]['relocations']
                                  if 'GameManager' in r['symbol']], names)


if __name__ == '__main__':
    unittest.main()
