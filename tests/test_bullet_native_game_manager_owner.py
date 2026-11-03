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


    def test_native_side_storage_and_original_bullet_player_receiver(self):
        header = (ROOT/'src/BulletManager.hpp').read_text()
        source = (ROOT/'src/BulletManager.cpp').read_text()
        laser = (ROOT/'src/BulletLaserSpawn.cpp').read_text()
        self.assertIn('#include "GameManagerRuntimeView.hpp"', header)
        self.assertIn('typedef AsciiGameManagerSideView BulletSideStateView;', header)
        self.assertNotIn('struct BulletSideStateView\n{', header)
        self.assertIn('BulletSideStateSizeIs38', header)
        self.assertIn('BulletSideEffectManagerAt0C', header)
        self.assertIn('controller->sideState = &g_GameManager.sides[sideIndex];', source)
        self.assertIn('controller->opponentSideState = &g_GameManager.sides[1 - sideIndex];', source)
        self.assertIn('reinterpret_cast<BulletPlayerView *>(this->sideState->player04)', source)
        self.assertIn('reinterpret_cast<BulletPlayerView *>(this->sideState->player04)', laser)
        self.assertNotIn('struct BulletGameManagerView', source)
        with (ROOT/'config/match-units.toml').open('rb') as f:
            units = tomllib.load(f)['units']
        for name, address, logical, physical in (
                ('etama-controller-update',0x004146F0,2195,2216),
                ('etama-spawn-single-bullet',0x00412960,1893,1932),
                ('etama-spawn-laser-pattern',0x00413290,450,450)):
            with self.subTest(unit=name):
                u=units[name]
                self.assertEqual((u['target_address'],u['size'],u.get('compare_size',u['size'])),
                                 (address,logical,physical))


if __name__ == '__main__':
    unittest.main()
