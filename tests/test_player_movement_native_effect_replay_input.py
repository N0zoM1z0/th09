"""Full 1900-byte Player movement owner uses one native focus pair/input index."""
from pathlib import Path
import tomllib
import unittest

ROOT=Path(__file__).resolve().parents[1]

class PlayerMovementNativeFocusInput(unittest.TestCase):
    def test_native_focus_effect_pair_has_no_trivial_shadow_accessors(self):
        source=(ROOT/'src/PlayerMovement.cpp').read_text()
        header=(ROOT/'src/PlayerLifecycleView.hpp').read_text()
        self.assertNotIn('PlayerMovementFocusEffect(',source)
        self.assertNotIn('PlayerMovementSecondaryEffect(',source)
        self.assertIn('player->focusEffect364 =',source)
        self.assertIn('player->secondaryEffect368 =',source)
        self.assertIn('if (player->focusEffect364 == NULL)',source)
        self.assertIn('if (player->focusEffect364 != NULL)',source)
        self.assertIn('player->secondaryEffect368->active = 0;',source)
        self.assertIn('Effect *focusEffect364;',header)
        self.assertIn('Effect *secondaryEffect368;',header)

    def test_real_replay_input_array_and_second_independent_focus_read(self):
        source=(ROOT/'src/PlayerMovement.cpp').read_text()
        input_header=(ROOT/'src/ReplayInputState.hpp').read_text()
        self.assertIn('ReplayInputState *input = &g_ReplayInputStates[player->sideIndex];',source)
        self.assertIn('int inputOffset = player->sideIndex * sizeof(ReplayInputState);',source)
        self.assertIn('reinterpret_cast<unsigned char *>(g_ReplayInputStates) +',source)
        self.assertIn('inputOffset)->auxiliary2A >= 8;',source)
        self.assertIn('extern ReplayInputState g_ReplayInputStates[3];',input_header)
        self.assertIn('ReplayInputStateSizeIs8E',input_header)
        self.assertIn('ReplayInputAuxAt2A',input_header)
        with (ROOT/'config/match-units.toml').open('rb') as f:
            u=tomllib.load(f)['units']
        self.assertEqual(u['player-on-update']['target_address'],0x0041E900)
        self.assertEqual(u['anm-vm-set-interrupt']['target_address'],0x00406790)
        self.assertEqual(u['effect-manager-spawn-effect-fixed-slot']['target_address'],0x0040CD70)

if __name__=='__main__':unittest.main()
