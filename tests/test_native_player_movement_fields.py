"""The 1,900-byte Player movement boss reads a single native 0x20 field group.

This source/layout test complements the full pinned VC7.1 original-bound
boss and constructor replays; it grants no exactness to movement itself.
"""
from pathlib import Path
import tomllib
import unittest

ROOT = Path(__file__).resolve().parents[1]


class NativePlayerMovementFields(unittest.TestCase):
    def test_velocity_angle_and_all_four_multiplier_fields_have_one_owner(self):
        header=(ROOT/'src/PlayerLifecycleView.hpp').read_text()
        movement=(ROOT/'src/PlayerMovement.cpp').read_text()
        self.assertIn('struct PlayerMovementFields',header)
        for field in ('PlayerPositionView velocity1CCC;', 'float movementAngle1CD8;',
                      'float speedMultiplier1CDC;', 'float speedMultiplier1CE0;',
                      'float speedMultiplier1CE4;', 'float speedMultiplier1CE8;'):
            with self.subTest(field=field):
                self.assertIn(field,header)
        self.assertIn('PlayerMovementFieldsSize20',header)
        self.assertIn('PlayerMovementAngleAt0C',header)
        self.assertIn('PlayerMovementScaleLastAt1C',header)
        self.assertIn('PlayerMovementFields movementFields1CCC;',header)
        self.assertIn('PlayerLifecycleMovementAt1CCC',header)
        self.assertIn('PlayerMovementFields &runtime = player->movementFields1CCC;',movement)
        self.assertNotIn('struct PlayerMovementFields',movement)
        self.assertNotIn('reinterpret_cast<unsigned char *>(player) + 0x1CCC',movement)

    def test_independent_added_state_and_gameplay_consumers_confirm_multiplier_roles(self):
        added=(ROOT/'src/PlayerAddedState.cpp').read_text()
        gameplay=(ROOT/'src/PlayerGameplayState.cpp').read_text()
        for field in ('speedMultiplier1CE0', 'speedMultiplier1CE4', 'speedMultiplier1CE8'):
            self.assertIn(field,added)
            self.assertIn(field,gameplay)
        with (ROOT/'config/match-units.toml').open('rb') as stream:
            units=tomllib.load(stream)['units']
        self.assertEqual((units['player-constructor']['target_address'],
                          units['player-constructor']['size']), (0x0041F070, 243))
        self.assertEqual(units['player-on-update']['target_address'], 0x0041E900)


if __name__ == '__main__':
    unittest.main()
