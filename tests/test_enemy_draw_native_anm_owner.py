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
        self.assertIn('AnmLoadedSprite *loadedSprite;', (ROOT/'src/AnmVmLifecycle.hpp').read_text())
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

    def test_native_vm_fields_and_four_set_z_rotation_calls(self):
        src = (ROOT/'src/EnemyManagerDraw.cpp').read_text()
        self.assertIn('typedef AnmVm EnemyDrawVmView;', src)
        self.assertNotIn('struct EnemyDrawVmView', src)
        self.assertIn('EnemyDrawVmSizeIs2A4', src)
        self.assertIn('EnemyDrawVmNativeBaseState',src)
        self.assertIn('offsetof(AnmVmBase, flagsWord) == 0x1F8',src)
        for native_slot in ('scriptIndex','type','pos','pos2','scale',
                            'color1','uvScrollPos','loadedSprite'):
            with self.subTest(native_slot=native_slot):
                self.assertTrue(('->'+native_slot) in src or ('.'+native_slot) in src)
        with (ROOT/'config/match-units.toml').open('rb') as f:
            units=tomllib.load(f)['units']
        old='?SetZRotation@EnemyDrawVmView@@QAEXM@Z'
        native='?SetZRotation@AnmVm@@QAEXM@Z'
        sites=[f for f in units['enemy-manager-draw-impl']['relocations']
               if 'SetZRotation' in f['symbol']]
        self.assertEqual([f['offset'] for f in sites],[0x58,0xFD,0x1DA,0x607])
        self.assertTrue(all(f['symbol']==native and f['target']==0x40F840 for f in sites))
        self.assertNotIn(old,src)
        self.assertTrue(any(f['symbol']==native and f['target']==0x40F840
                            for f in units['bullet-draw-single']['relocations']))

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
