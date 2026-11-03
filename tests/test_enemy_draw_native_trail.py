"""Original-bound native trail ownership in the 1758-byte Enemy draw boss."""
from pathlib import Path
import tomllib
import unittest

ROOT=Path(__file__).resolve().parents[1]


class EnemyDrawNativeTrail(unittest.TestCase):
    def test_exact_draw_uses_single_native_trail_storage(self):
        draw=(ROOT/'src/EnemyManagerDraw.cpp').read_text()
        native=(ROOT/'src/EnemyManager.hpp').read_text()
        self.assertIn('EnemyDrawNativeVertexLayoutAt3E68',draw)
        self.assertIn('unsigned char unknown3384[0x5430 - 0x3384];',draw)
        self.assertNotIn('struct EnemyDrawTrailSampleView',draw)
        self.assertNotIn('EnemyDrawTrailSampleView trailSamples33E8[96];',draw)
        self.assertNotIn('EnemyDrawVertex trailVertices3E68[192];',draw)
        self.assertIn('EnemyTrailSampleView trailSamples33E8[96];',native)
        self.assertIn('EnemyTrailVertexView trailVertices3E68[192];',native)
        self.assertIn('reinterpret_cast<EnemyView *>(enemy)->trailVertices3E68',draw)
        for name in ('trailSamples33E8','trailFlags53A0','trailHistoryLength53A2',
                     'trailSampleStride53A6'):
            with self.subTest(name=name):
                self.assertIn('reinterpret_cast<EnemyView *>(enemy)->'+name,draw)
                self.assertNotIn('enemy->'+name,draw)
        for field in ('position00','rhw0C','diffuse10','u14','v18'):
            self.assertIn('offsetof(EnemyTrailVertexView, '+field+')',draw)

    def test_original_bound_draw_owner_is_not_relabelled(self):
        with (ROOT/'config/match-units.toml').open('rb') as f:
            units=tomllib.load(f)['units']
        self.assertEqual(units['enemy-manager-draw-impl']['size'],1758)
        self.assertEqual(units['enemy-manager-draw-impl']['source'],
                         'src/EnemyManagerDraw.cpp')
        self.assertEqual(units['enemy-manager-draw-impl']['target_address'],0x411670)


    def test_exact_draw_uses_real_native_popup_coordinate_owner(self):
        draw = (ROOT/'src/EnemyManagerDraw.cpp').read_text()
        self.assertIn('EnemyDrawNativeGameManagerCoordinates', draw)
        self.assertNotIn('struct EnemyDrawGameManagerView', draw)
        self.assertNotIn('extern EnemyDrawGameManagerView g_GameManager;', draw)
        with (ROOT/'config/match-units.toml').open('rb') as stream:
            units = tomllib.load(stream)['units']
        fields = units['enemy-manager-draw-impl']['relocations']
        names = {
            '?g_GameManager@@3UAsciiGameManagerView@@A': (0x004A7D90, 8),
            '?TransformPopupX@AsciiGameManagerView@@QAEMM@Z': (0x00401680, 4),
            '?TransformPopupY@AsciiGameManagerView@@QAEMM@Z': (0x004016B0, 4),
        }
        for symbol, (target, count) in names.items():
            with self.subTest(symbol=symbol):
                chosen = [r for r in fields if r['symbol'] == symbol]
                self.assertEqual(len(chosen), count)
                self.assertTrue(all(r['target'] == target for r in chosen))
        for name in ('game-manager-transform-popup-x', 'game-manager-transform-popup-y'):
            self.assertEqual(units[name]['size'], 46)


if __name__=='__main__':
    unittest.main()
