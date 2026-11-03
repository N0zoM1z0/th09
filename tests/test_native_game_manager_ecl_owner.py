"""ECL opcode 148, exact GameManager reset and timeline share native fields."""
from pathlib import Path
import tomllib
import unittest

ROOT = Path(__file__).resolve().parents[1]


class NativeGameManagerEclOwner(unittest.TestCase):
    def test_opcode148_and_reset_use_same_native_value380(self):
        native = (ROOT/'src/GameManagerRuntimeView.hpp').read_text()
        ecl = (ROOT/'src/EclRunState.inl').read_text()
        reset = (ROOT/'src/GameManagerSetup.cpp').read_text()
        self.assertIn('int value380;', native)
        self.assertIn('AsciiGameManagerNativeEclStorage', native)
        self.assertIn('offsetof(AsciiGameManagerView, value380) == 0x380', native)
        self.assertIn('sizeof(AsciiGameManagerView) == 0x390', native)
        self.assertIn('g_GameManager.value380 += 1800;', ecl)
        self.assertNotIn('GameManagerEclStateView', ecl)
        self.assertIn('reinterpret_cast<AsciiGameManagerView *>(manager)->value380 = 0;',reset)
        self.assertNotIn('reinterpret_cast<GameManagerEclStateView *>(manager)',reset)
        self.assertIn('offsetof(AsciiGameManagerView, instructionMask120)',reset)

    def test_unified_mask_word_still_has_an_unmodified_low_byte(self):
        native = (ROOT/'src/GameManagerRuntimeView.hpp').read_text()
        self.assertIn('union { unsigned int instructionMask120; unsigned char instructionMaskLowByte120; };', native)
        self.assertIn('offsetof(AsciiGameManagerView, instructionMaskLowByte120) == 0x120', native)
        with (ROOT/'config/match-units.toml').open('rb') as f:
            u=tomllib.load(f)['units']
        self.assertEqual((u['game-manager-reset-runtime']['size'],
                          u['game-manager-reset-runtime']['target_address']),
                         (100,0x0041A7C3))


if __name__ == '__main__':
    unittest.main()
