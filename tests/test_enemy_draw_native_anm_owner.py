"""Enemy draw's original ANM callfields are genuine native methods."""
from pathlib import Path
import tomllib
import unittest

ROOT=Path(__file__).resolve().parents[1]


class EnemyDrawNativeAnm(unittest.TestCase):
    def test_original_sprite_uv_and_dimensions_use_native_loaded_record(self):
        source=(ROOT/'src/EnemyManagerDraw.cpp').read_text()
        self.assertNotIn('struct EnemyDrawSpriteView',source)
        self.assertIn('EnemyDrawNativeLoadedSpriteAt0x44',source)
        self.assertIn('AnmLoadedSprite *loadedSprite224;',source)
        self.assertIn('AnmLoadedSprite *sprite =',source)
        for member,offset in (('uvStartX','0x20'),('uvStartY','0x24'),
                              ('uvEndX','0x28'),('uvEndY','0x2C'),
                              ('heightPx','0x30'),('widthPx','0x34')):
            with self.subTest(member=member):
                self.assertIn('offsetof(AnmLoadedSprite, '+member+') == '+offset,source)
        self.assertIn('sprite->uvEndX - sprite->uvStartX',source)
        self.assertIn('->heightPx * 0.5f',source)
        with (ROOT/'config/match-units.toml').open('rb') as f:
            units=tomllib.load(f)['units']
        self.assertEqual((units['anm-loaded-set-sprite']['target_address'],
                          units['anm-loaded-set-sprite']['size']),(0x436AC0,189))

    def test_large_render_owner_has_exact_native_anm_call_targets(self):
        source=(ROOT/'src/EnemyManagerDraw.cpp').read_text()
        self.assertIn('#include "AnmManager.hpp"',source)
        self.assertNotIn('struct EnemyDrawAnmManagerView',source)
        self.assertNotIn('extern EnemyDrawAnmManagerView *g_AnmManager;',source)
        self.assertEqual(source.count('g_AnmManager->Draw2D('),4)
        self.assertEqual(source.count('g_AnmManager->DrawVertices('),1)
        self.assertIn('reinterpret_cast<AnmVm *>(&enemy->vm008)',source)
        self.assertIn('reinterpret_cast<VertexTex1DiffuseXyzrhw *>(',source)
        with (ROOT/'config/match-units.toml').open('rb') as f:
            u=tomllib.load(f)['units']
        owner=u['enemy-manager-draw-impl']
        self.assertEqual((owner['target_address'],owner['size'],len(owner['relocations'])),
                         (0x411670,1758,53))
        expected={
            '?g_AnmManager@@3PAVAnmManager@@A':(5,0x004DC550),
            '?Draw2D@AnmManager@@QAEHPAUAnmVm@@@Z':(4,0x0043AB50),
            '?DrawVertices@AnmManager@@QAEHPAUAnmVm@@PAUVertexTex1DiffuseXyzrhw@@H@Z':(1,0x0043B4D0),
        }
        for symbol,(count,target) in expected.items():
            with self.subTest(symbol=symbol):
                fields=[r for r in owner['relocations'] if r['symbol']==symbol]
                self.assertEqual(len(fields),count)
                self.assertTrue(all(r['target']==target for r in fields))
        self.assertEqual((u['anm-manager-draw2d']['symbol'],u['anm-manager-draw2d']['size']),
                         ('?Draw2D@AnmManager@@QAEHPAUAnmVm@@@Z',491))
        self.assertEqual((u['anm-manager-draw-vertices']['symbol'],u['anm-manager-draw-vertices']['size']),
                         ('?DrawVertices@AnmManager@@QAEHPAUAnmVm@@PAUVertexTex1DiffuseXyzrhw@@H@Z',238))

if __name__=='__main__': unittest.main()
