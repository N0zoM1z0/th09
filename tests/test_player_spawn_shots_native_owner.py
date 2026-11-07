"""The exact SpawnShots and Effect35 leaves consume native Player members."""
from pathlib import Path
import tomllib
import unittest

ROOT=Path(__file__).resolve().parents[1]

class PlayerSpawnShotsNativeOwner(unittest.TestCase):
    def test_one_native_player_storage_and_callback_specific_views(self):
        header=(ROOT/'src/PlayerSpawnShotsView.hpp').read_text()
        source=(ROOT/'src/PlayerSpawnShots.cpp').read_text()
        callback=(ROOT/'src/EffectType35.cpp').read_text()
        self.assertIn('struct PlayerSpawnShotsView : PlayerStateEffectView',header)
        self.assertIn('sizeof(PlayerSpawnShotsView) == sizeof(PlayerLifecycleView)',header)
        self.assertIn('offsetof(PlayerSpawnShotsView, shots) == 0xC11C',header)
        self.assertIn('offsetof(PlayerSpawnShotsView, primaryShtFile) == 0x30338',header)
        self.assertIn('offsetof(PlayerHeaderStateView, anmFile98) == 0xBC',header)
        self.assertNotIn('unknown00000[0xBC]',header)
        self.assertNotIn('shotsC11C[128]',header)
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
