"""Original-bound Player movement option storage and lifecycle constructor.

The 0x2F4 option record's timer/callback members are verified structurally
here; current-source VC7.1 byte/COFF proof runs in the focused movement
and player-constructor checks, not in these pure source tests.
"""

from pathlib import Path
import tomllib
import unittest

ROOT = Path(__file__).resolve().parents[1]


class PlayerMovementNativeOptions(unittest.TestCase):
    def test_one_typed_option_record_preserves_known_timer_callback_offsets(self):
        header = (ROOT/'src/PlayerLifecycleView.hpp').read_text()
        source = (ROOT/'src/PlayerMovement.cpp').read_text()
        self.assertIn('struct PlayerOptionStateCtorView', header)
        self.assertIn('ZunTimer timer2E0;', header)
        self.assertIn('PlayerOptionUpdateCallback updateCallback2EC;', header)
        self.assertIn('PlayerOptionStateCtorSizeIs2F4', header)
        self.assertIn('offsetof(PlayerOptionStateCtorView, timer2E0) == 0x2E0', header)
        self.assertIn('offsetof(PlayerOptionStateCtorView, updateCallback2EC) == 0x2EC', header)
        self.assertIn('PlayerOptionStateCtorView optionStates[4];', header)
        self.assertIn('typedef PlayerOptionStateCtorView PlayerMovementOptionView;', source)
        self.assertNotIn('struct PlayerMovementOptionView', source)
        self.assertIn('PlayerMovementOptionView *option = player->optionStates;', source)
        self.assertNotIn('reinterpret_cast<unsigned char *>(player) + 0x1CEC', source)

    def test_native_callback_and_timer_are_consumed_by_full_boss(self):
        source=(ROOT/'src/PlayerMovement.cpp').read_text()
        self.assertIn('option->updateCallback2EC(player, option);', source)
        self.assertIn('option->timer2E0++;', source)
        # The exact four live options are now sized from the real Player
        # allocation. The separate 16-record history also uses its native
        # last index, rather than another unconnected literal.
        self.assertIn('OptionCount = sizeof(player->optionStates)',source)
        self.assertIn('for (int optionCount = OptionCount;', source)
        self.assertIn('PositionHistoryLast = sizeof(player->positions1BA0)',source)
        self.assertIn('for (int historyIndex = PositionHistoryLast;',source)
        self.assertIn('PlayerConstructedVector3View positions1BA0[16];',
                      (ROOT/'src/PlayerLifecycleView.hpp').read_text())
        with (ROOT/'config/match-units.toml').open('rb') as f:
            u=tomllib.load(f)['units']
        self.assertEqual((u['player-constructor']['target_address'],
                          u['player-constructor']['size']),
                         (0x0041F070, 243))
        self.assertEqual((u['player-on-update']['target_address'],
                          u['anm-vm-set-interrupt']['target_address']),
                         (0x0041E900, 0x00406790))


if __name__ == '__main__':
    unittest.main()
