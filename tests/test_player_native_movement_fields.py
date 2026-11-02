"""Target-observed Player movement state in the true lifecycle record.

The 0x20-byte block is written by the big movement boss and independently
consumed by Player AddedState and other gameplay owners. Its actual binary
identity is checked by the focused original-PE/VC7.1 owner builds.
"""
from pathlib import Path
import tomllib
import unittest

ROOT=Path(__file__).resolve().parents[1]

class PlayerNativeMovementFields(unittest.TestCase):
    def test_single_typed_runtime_replaces_raw_0x1ccc_storage(self):
        header=(ROOT/'src/PlayerLifecycleView.hpp').read_text()
        source=(ROOT/'src/PlayerMovement.cpp').read_text()
        self.assertIn('struct PlayerMovementFields',header)
        self.assertNotIn('struct PlayerMovementFields',source)
        self.assertIn('PlayerMovementFields movementFields1CCC;',header)
        self.assertNotIn('unknown1CCC[0x1CEC - 0x1CCC]',header)
        self.assertIn('return &player->movementFields1CCC;',source)
        for member,offset in (('velocity1CCC','0x1CCC'),
                              ('movementAngle1CD8','0x1CD8'),
                              ('speedMultiplier1CDC','0x1CDC'),
                              ('speedMultiplier1CE0','0x1CE0'),
                              ('speedMultiplier1CE4','0x1CE4'),
                              ('speedMultiplier1CE8','0x1CE8')):
            with self.subTest(member=member):
                self.assertIn(member,header)
                self.assertIn('offsetof(PlayerMovementFields, '+member+')',header)
                self.assertIn('== '+offset,header)
        self.assertIn('PlayerMovementFieldsSize20',header)
        self.assertIn('PlayerLifecycleOptionsAt1CEC',header)

    def test_other_gameplay_owners_independently_consume_movement_scalars(self):
        added=(ROOT/'src/PlayerAddedState.cpp').read_text()
        gameplay=(ROOT/'src/PlayerGameplayState.cpp').read_text()
        source=(ROOT/'src/PlayerMovement.cpp').read_text()
        for name in ('speedMultiplier1CDC','speedMultiplier1CE0',
                     'speedMultiplier1CE4','speedMultiplier1CE8'):
            self.assertIn(name,added)
            self.assertIn(name,gameplay)
            self.assertIn(name,source)
        with (ROOT/'config/match-units.toml').open('rb') as f:
            units=tomllib.load(f)['units']
        self.assertEqual(units['player-constructor']['target_address'],0x0041F070)
        self.assertEqual(units['player-on-update']['target_address'],0x0041E900)

if __name__=='__main__':
    unittest.main()
