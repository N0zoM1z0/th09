"""Exact 572-byte Player AddedState uses the same native movement and shots."""
from pathlib import Path
import tomllib
import unittest

ROOT=Path(__file__).resolve().parents[1]

class PlayerAddedNativeShotsMovement(unittest.TestCase):
    def test_actual_player_movement_fields_and_shots_are_shared(self):
        added=(ROOT/'src/PlayerAddedState.cpp').read_text()
        native=(ROOT/'src/PlayerLifecycleView.hpp').read_text()
        movement=(ROOT/'src/PlayerMovement.cpp').read_text()
        self.assertNotIn('struct PlayerAddedShotView',added)
        self.assertNotIn('PlayerAddedShotView shotsC11C[128]',added)
        self.assertIn('unsigned char unknownC11C[0x3031C - 0xC11C];',added)
        self.assertIn('offsetof(PlayerLifecycleView, shots) == 0xC11C',added)
        self.assertIn('this->shots[i].state462 = 0;',added)
        self.assertIn('PlayerShotCtorView shots[128];',native)
        self.assertIn('unsigned char unknown1CD8[0x1CEC - 0x1CD8];',added)
        for name in ('speedMultiplier1CDC','speedMultiplier1CE0',
                     'speedMultiplier1CE4','speedMultiplier1CE8'):
            with self.subTest(name=name):
                self.assertIn('this->movementFields1CCC.'+name+' = 1.0f;',added)
                self.assertIn('runtime->'+name,movement)
        self.assertIn('this->movementFields1CCC.movementAngle1CD8 = -1.57079637f;',added)
        self.assertIn('PlayerMovementFields movementFields1CCC;',native)

    def test_header_initial_state_side_mode_and_focus_share_native_player(self):
        added=(ROOT/'src/PlayerAddedState.cpp').read_text()
        native=(ROOT/'src/PlayerLifecycleView.hpp').read_text()
        self.assertIn('this->header24.state7C = initialState;',added)
        self.assertIn('this->header24.state80 = initialState;',added)
        self.assertIn('this->header24.state84 = g_PlayerAddedSideTable[this->sideIndex];',added)
        self.assertIn('this->header24.state84 = 1;',added)
        self.assertIn('this->focusEffect364 = NULL;',added)
        self.assertIn('this->flags1B80 = addedFlags;',added)
        self.assertIn('this->position1B88.operator float *()',added)
        for member in ('state7C','state80','state84'):
            self.assertIn('int '+member+';',native)
        self.assertIn('Effect *focusEffect364;',native)

    def test_original_sized_exact_owners_remain_one_translation_unit(self):
        with (ROOT/'config/match-units.toml').open('rb') as f:
            units=tomllib.load(f)['units']
        for name,size in (('player-initialize-added-state',552),
                          ('player-added-collision-reset',23),
                          ('player-added-header-reset',29),
                          ('player-added-owner-reset',41)):
            with self.subTest(name=name):
                self.assertEqual(units[name]['size'],size)
                self.assertEqual(units[name]['source'],'src/PlayerAddedState.cpp')
                if name=='player-initialize-added-state':
                    self.assertEqual(units[name]['compare_size'],572)

if __name__=='__main__':unittest.main()
