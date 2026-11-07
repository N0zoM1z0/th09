"""Keep the FrontSide HUD scores on the original shared side-runtime storage."""
from pathlib import Path
import tomllib
import unittest

ROOT = Path(__file__).resolve().parents[1]

class FrontSideNativeSideRuntime(unittest.TestCase):
    def test_all_score_reads_use_real_game_manager_side_runtime(self):
        source = (ROOT/'src/FrontSide.cpp').read_text()
        native = (ROOT/'src/GameManagerRuntimeView.hpp').read_text()
        self.assertIn('#define TH09_GAME_MANAGER_TYPES_ONLY', source)
        self.assertIn('#include "GameManagerRuntimeView.hpp"', source)
        self.assertIn('#undef TH09_GAME_MANAGER_TYPES_ONLY', source)
        self.assertNotIn('struct FrontSideSideRuntimeView', source)
        self.assertIn('FrontSideNativeRuntimeLayout', source)
        self.assertIn('offsetof(FrontSideSideStateView, runtime) == 0x1C', source)
        self.assertIn('sizeof(AsciiGameSideRuntimeView) == 0xA0', source)
        self.assertIn('offsetof(AsciiGameSideRuntimeView, counter14) == 0x14', source)
        self.assertIn('#define FRONT_SIDE_RUNTIME(owner) (FRONT_SIDE_STATE(owner)->runtime)', source)
        self.assertIn('.runtime->value00', source)
        self.assertNotIn('->runtime1C', source)
        self.assertIn('struct AsciiGameSideRuntimeView', native)

    def test_original_game_manager_method_abi_remains_separate(self):
        source = (ROOT/'src/FrontSide.cpp').read_text()
        self.assertIn('struct FrontSideGameManagerView', source)
        with (ROOT/'config/match-units.toml').open('rb') as f:
            units = tomllib.load(f)['units']
        self.assertEqual(units['front-side-on-update']['size'],2381)
        self.assertEqual(units['front-side-on-update']['source'],'src/FrontSide.cpp')

if __name__ == '__main__':
    unittest.main()
