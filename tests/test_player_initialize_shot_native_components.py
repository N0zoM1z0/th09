"""Original-exact 309-byte shot initializer operates on real Player/Shot/SHT.

The pinned original PE/COFF source-cold comparator separately validates
all bytes and fields. These assertions protect owner identity/offsets.
"""
from pathlib import Path
import tomllib
import unittest

ROOT=Path(__file__).resolve().parents[1]

class NativePlayerShotInitializerComponents(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        with (ROOT/'config/match-units.toml').open('rb') as f:
            cls.units=tomllib.load(f)['units']

    def test_full_original_owner_and_real_sht_descriptors(self):
        src=(ROOT/'src/PlayerInitializeShot.cpp').read_text()
        header=(ROOT/'src/PlayerLifecycleView.hpp').read_text()
        self.assertIn('PlayerLifecycleView &nativePlayer =',src)
        self.assertIn('PlayerShotCtorView &nativeShot =',src)
        self.assertIn('PlayerShtDescriptorView &nativeEntry =',src)
        self.assertIn('nativePlayer.position1B88;',src)
        self.assertIn('nativePlayer.optionStates[nativeEntry.sourceOptionIndex20 - 1].position2A4;',src)
        self.assertIn('nativePlayer.header24.anmFile98->SetAndExecuteScriptIdx(',src)
        self.assertIn('nativeShot.timer454 = 0;',src)
        self.assertIn('nativeShot.damage460 = nativeEntry.damage1C;',src)
        self.assertIn('nativeShot.field470 = 0;',src)
        self.assertIn('nativeShot.animationIndex46C = nativeEntry.animationIndex24;',src)
        self.assertIn('nativeEntry.soundIndex26, nativePlayer.position1B88.x',src)
        self.assertNotIn('shot->damage460',src)
        self.assertNotIn('entry->damage1C',src)
        self.assertIn('PlayerShtNativeShotInitFullFields',header)
        self.assertIn('PlayerShotNativeInitializationFields',header)
        self.assertIn('PlayerShtDescriptorSizeIs38',header)
        self.assertIn('PlayerShotCtorSizeIs484',header)
        self.assertIn('PlayerLifecyclePositionAt1B88',header)
        for typename,field,offset in (
            ('PlayerShtDescriptorView','hitboxX0C','0x0C'),
            ('PlayerShtDescriptorView','hitboxY10','0x10'),
            ('PlayerShtDescriptorView','damage1C','0x1C'),
            ('PlayerShtDescriptorView','shotType22','0x22'),
            ('PlayerShtDescriptorView','animationIndex24','0x24'),
            ('PlayerShtDescriptorView','soundIndex26','0x26'),
            ('PlayerShotCtorView','damage460','0x460'),
            ('PlayerShotCtorView','field470','0x470')):
            with self.subTest(field=field):
                self.assertIn('offsetof('+typename+', '+field+') == '+offset,header)

    def test_original_seven_original_call_fields_complete_309_bytes(self):
        unit=self.units['player-initialize-shot']
        self.assertEqual((unit['target_address'],unit['size'],len(unit['relocations'])),
                         (0x0041F350,309,7))
        calls=[(r['offset'],r['target']) for r in unit['relocations']]
        self.assertEqual(calls,[(0x4B,0x004343D0),(0x99,0x00401060),
                                (0xAB,0x00401070),(0xC2,0x00401500),
                                (0x106,0x004DC698),(0x10B,0x0043E380),
                                (0x122,0x00403E00)])
        self.assertEqual(self.units['player-constructor']['size'],243)
        self.assertEqual(self.units['player-shot-spawn-callback-type1']['size'],151)
        self.assertEqual(self.units['player-shot-spawn-callback-type3']['size'],86)
        # The public InitializeShot method still has its independently
        # exact legacy structural declaration pending cross-TU ABI migration.
        self.assertIn('@PlayerShotInitPlayerView@@',unit['symbol'])

if __name__=='__main__':unittest.main()
