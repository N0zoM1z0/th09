"""TH09's full movement boss shares one true ReplayInputState array.

Native source assertions augment, but do not replace, the complete original
PE/COFF 1,900-byte movement and exact caller/leaf replay checks.
"""
from pathlib import Path
import unittest
import tomllib

ROOT=Path(__file__).resolve().parents[1]

class PlayerMovementNativeFocusAllocation(unittest.TestCase):
    def test_typed_focus_read_uses_same_side_as_direction(self):
        source=(ROOT/'src/PlayerMovement.cpp').read_text()
        native=(ROOT/'src/ReplayInputState.hpp').read_text()
        self.assertIn('const int inputSide = player->sideIndex;',source)
        self.assertIn('ReplayInputState *input = &g_ReplayInputStates[inputSide];',source)
        self.assertIn('g_ReplayInputStates[inputSide].auxiliary2A >= 8;',source)
        self.assertIn('g_GameConfiguration->sideFocusMode[player->sideIndex] == 1',source)
        self.assertIn('input->IsHeld(4) != 0;',source)
        self.assertNotIn('reinterpret_cast<unsigned char *>(g_ReplayInputStates)',source)
        self.assertNotIn('reinterpret_cast<ReplayInputState *>',source)
        self.assertNotIn('inputOffset',source)
        self.assertIn('extern ReplayInputState g_ReplayInputStates[3];',native)
        self.assertIn('ReplayInputStateSizeIs8E',native)
        self.assertIn('ReplayInputAuxAt2A',native)

    def test_native_movement_and_exact_callees_still_independent(self):
        with (ROOT/'config/match-units.toml').open('rb') as stream:
            units=tomllib.load(stream)['units']
        self.assertEqual(units['replay-input-is-held']['size'],18)
        self.assertEqual(units['replay-input-is-held']['target_address'],0x00415CB0)
        self.assertEqual(units['player-on-update']['size'],522)
        self.assertEqual(units['player-on-update']['target_address'],0x0041E900)
        self.assertEqual(units['anm-vm-set-interrupt']['size'],18)
        self.assertEqual(units['anm-vm-set-interrupt']['target_address'],0x00406790)

if __name__=='__main__':
    unittest.main()
