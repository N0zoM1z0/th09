"""Nonexact movement boss snapshots one genuine 0x8E ReplayInputState slot."""
from pathlib import Path
import tomllib
import unittest

ROOT=Path(__file__).resolve().parents[1]

class PlayerMovementNativeInputSnapshot(unittest.TestCase):
    def test_one_real_replay_input_slot_for_direction_and_focus(self):
        src=(ROOT/'src/PlayerMovement.cpp').read_text()
        native=(ROOT/'src/ReplayInputState.hpp').read_text()
        self.assertIn('const int inputSide = player->sideIndex;',src)
        self.assertIn('int inputOffset = inputSide * sizeof(ReplayInputState);',src)
        self.assertIn('ReplayInputState *input = &g_ReplayInputStates[inputSide];',src)
        self.assertIn('inputOffset)->auxiliary2A >= 8;',src)
        self.assertIn('sizeof(ReplayInputState) == 0x8E',native)
        self.assertIn('offsetof(ReplayInputState, auxiliary2A) == 0x2A',native)
        self.assertNotIn('struct PlayerMovementReplayInputState',src)

    def test_nonexact_boss_remains_independent_of_exact_methods(self):
        with (ROOT/'config/match-units.toml').open('rb') as f:u=tomllib.load(f)['units']
        self.assertEqual((u['player-on-update']['size'],u['replay-input-is-held']['size']),(522,18))
        self.assertEqual(u['replay-input-is-held']['target_address'],0x00415CB0)

if __name__=='__main__':unittest.main()
