"""Exact 151-byte Type1 spawn uses actual 0x484-byte Player shot subobject.

Source-shape test complements complete original PE/COFF, not substitutes it.
"""
from pathlib import Path
import tomllib
import unittest
ROOT=Path(__file__).resolve().parents[1]

class NativeType1ShotOwner(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        with (ROOT/'config/match-units.toml').open('rb') as f:
            cls.units=tomllib.load(f)['units']

    def test_one_native_shot_with_real_constructed_trail(self):
        src=(ROOT/'src/PlayerShotSpawnCallback1.cpp').read_text()
        native=(ROOT/'src/PlayerLifecycleView.hpp').read_text()
        self.assertIn('PlayerLifecycleView *player,',src)
        self.assertIn('PlayerShotCtorView *shot,',src)
        self.assertNotIn('struct PlayerShotSpawn1ShotView',src)
        self.assertNotIn('struct PlayerShotSpawn1Vec3',src)
        self.assertIn('PlayerShotSpawn1RealNativeState',src)
        self.assertIn('player->state3031C.shot0C = shot;',src)
        self.assertIn('shot->trailVectors2B0[i].x = -999.0f;',src)
        self.assertIn('shot->position2A4.x = -999.0f;',src)
        self.assertIn('shot->sourceOption468 = entry->sourceOption20;',src)
        self.assertIn('shot->field46A = 15;',src)
        self.assertIn('shot->shotType464 = 2;',src)
        self.assertIn('PlayerShotNativeSourceAndField',native)
        for name,offset in (('sourceOption468','0x468'),('field46A','0x46A'),
                            ('animationIndex46C','0x46C')):
            with self.subTest(field=name):
                self.assertIn('offsetof(PlayerShotCtorView, '+name+') == '+offset,native)
        self.assertIn('PlayerShotCtorSizeIs484',native)
        self.assertIn('PlayerConstructedVector3View trailVectors2B0[32];',native)
        self.assertIn('PlayerState3031CView state3031C;',native)

    def test_original_151_byte_entire_callback_and_callees(self):
        s=self.units['player-shot-spawn-callback-type1']
        self.assertEqual((s['target_address'],s['size'],len(s['relocations'])),
                         (0x00441EF0,151,2))
        self.assertEqual(s['symbol'],'?PlayerShotSpawnCallbackType1@@YIHPAUPlayerLifecycleView@@PAUPlayerShotCtorView@@HPAUPlayerShotSpawn1DescriptorView@@@Z')
        self.assertEqual([(r['offset'],r['target']) for r in s['relocations']],
                         [(0x27,0x00401500),(0x5D,0x0041F350)])
        self.assertEqual(self.units['player-state3031c-constructor']['size'],12)
        self.assertEqual(self.units['player-constructor']['size'],243)

if __name__=='__main__':unittest.main()
