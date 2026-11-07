"""The exact SpawnShots and Effect35 leaves consume native Player members."""
from pathlib import Path
import tomllib
import unittest

ROOT=Path(__file__).resolve().parents[1]

class PlayerSpawnShotsNativeOwner(unittest.TestCase):
    def test_one_native_player_storage_and_callback_specific_views(self):
        header=(ROOT/'src/PlayerLifecycleView.hpp').read_text()
        source=(ROOT/'src/PlayerSpawnShots.cpp').read_text()
        callback=(ROOT/'src/EffectType35.cpp').read_text()
        self.assertIn('void __fastcall SpawnShots(int value, int powerLevel);',header)
        self.assertIn('PlayerLifecycleSizeIs30F70',header)
        self.assertIn('PlayerLifecycleShotsAtC11C',header)
        self.assertIn('PlayerLifecyclePrimaryShtAt30338',header)
        self.assertIn('PlayerHeaderNativeAnmAt98',header)
        self.assertIn('PlayerLifecycleHeaderAt24',header)
        self.assertNotIn('unknown00000[0xBC]',header)
        self.assertNotIn('shotsC11C[128]',header)
        self.assertFalse((ROOT/'src/PlayerSpawnShotsView.hpp').exists())
        self.assertIn('PlayerShotCtorView *shot = shots;',source)
        self.assertIn('PlayerShtDescriptorView *entry = primaryShtFile->shotPowerLevels[powerLevel].descriptors;',source)
        self.assertIn('->header24.anmFile98->SetAndExecuteScriptIdx',callback)

    def test_independent_leaf_original_extents(self):
        with (ROOT/'config/match-units.toml').open('rb') as f:u=tomllib.load(f)['units']
        for unit,size in (('player-spawn-shots',190),
                          ('effect-initialize-type35',72),
                          ('effect-update-type35',38)):
            with self.subTest(unit=unit):
                self.assertEqual(u[unit]['size'],size)

if __name__=='__main__':unittest.main()
