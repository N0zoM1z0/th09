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
        self.assertIn('typedef EnemyTrailVertexView EnemyDrawVertex;',draw)
        self.assertNotIn('struct EnemyDrawVertex',draw)
        self.assertIn('EnemyDrawVertex *vertices = enemy->trailVertices3E68;',draw)
        self.assertIn('vertices[0].position00',draw)
        self.assertIn('vertices[0].u14',draw)
        self.assertIn('vertices[0].v18',draw)
        self.assertIn('vertices[1].diffuse10',draw)
        self.assertIn('typedef EnemyView EnemyDrawView;', draw)
        self.assertNotIn('struct EnemyDrawView\n{', draw)
        self.assertIn('EnemyDrawViewSizeIs5430', draw)
        self.assertIn('EnemyDrawSecondaryAt2AC', draw)
        self.assertIn('EnemyDrawWorldAt2DD4', draw)
        self.assertNotIn('struct EnemyDrawTrailSampleView',draw)
        self.assertNotIn('EnemyDrawTrailSampleView trailSamples33E8[96];',draw)
        self.assertNotIn('EnemyDrawVertex trailVertices3E68[192];',draw)
        self.assertIn('EnemyTrailSampleView trailSamples33E8[96];',native)
        self.assertIn('EnemyTrailVertexView trailVertices3E68[192];',native)
        self.assertIn('enemy->trailVertices3E68',draw)
        for name in ('trailSamples33E8','trailFlags53A0','trailHistoryLength53A2',
                     'trailSampleStride53A6'):
            with self.subTest(name=name):
                self.assertIn('enemy->'+name,draw)
                self.assertNotIn('reinterpret_cast<EnemyView *>(enemy)->'+name,draw)
        for field in ('position00','rhw0C','diffuse10','u14','v18'):
            self.assertIn('offsetof(EnemyTrailVertexView, '+field+')',draw)

    def test_native_viewport_wrapper_calls_and_original_target_fields(self):
        draw=(ROOT/'src/EnemyManagerDraw.cpp').read_text()
        self.assertIn('#include "Supervisor.hpp"',draw)
        self.assertNotIn('EnemyDrawSupervisorView',draw)
        self.assertEqual(draw.count('g_Supervisor.ConfigureScreenViewport('),2)
        with (ROOT/'config/match-units.toml').open('rb') as f:
            units=tomllib.load(f)['units']
        callee=units['supervisor-configure-screen-viewport']
        self.assertEqual((callee['target_address'],callee['size'],callee['symbol']),
                         (0x403D90,71,'?ConfigureScreenViewport@Supervisor@@QAEXH@Z'))
        for name in ('enemy-manager-draw-high-prio','enemy-manager-draw-low-prio'):
            with self.subTest(owner=name):
                fields=units[name]['relocations']
                self.assertEqual((fields[0]['offset'],fields[0]['symbol'],fields[0]['target']),
                                 (0x0B,'?g_Supervisor@@3VSupervisor@@A',0x4B3100))
                self.assertEqual((fields[1]['offset'],fields[1]['symbol'],fields[1]['target']),
                                 (0x10,'?ConfigureScreenViewport@Supervisor@@QAEXH@Z',0x403D90))

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


    def test_exact_draw_uses_one_native_sprite_record_with_original_uv_fields(self):
        draw=(ROOT/'src/EnemyManagerDraw.cpp').read_text()
        owner=(ROOT/'src/AnmManager.hpp').read_text()
        self.assertIn('EnemyDrawNativeLoadedSpriteAt0x44',draw)
        self.assertNotIn('struct EnemyDrawSpriteView',draw)
        self.assertIn('AnmLoadedSprite *loadedSprite;', (ROOT/'src/AnmVmLifecycle.hpp').read_text())
        for field,offset in [('uvStartX','0x20'),('uvStartY','0x24'),
                             ('uvEndX','0x28'),('uvEndY','0x2C'),
                             ('heightPx','0x30'),('widthPx','0x34')]:
            with self.subTest(field=field):
                self.assertIn('offsetof(AnmLoadedSprite, '+field+') == '+offset,draw)
                self.assertIn('float '+field+';',owner)
        self.assertIn('sprite->uvEndX - sprite->uvStartX',draw)
        self.assertIn('enemy->primaryAnmVm0008.loadedSprite->heightPx * 0.5f;',draw)


if __name__=='__main__':
    unittest.main()
