"""Source-identity guard for Player movement's genuine side Effect manager.

Original-byte proof belongs to the focused Player movement and exact lifecycle
owners; this test prevents a second private 0x38-byte side-state definition.
"""
from pathlib import Path
import tomllib
import unittest

ROOT=Path(__file__).resolve().parents[1]

class PlayerMovementNativeSideEffect(unittest.TestCase):
    def test_side_effect_manager_consumed_from_native_lifecycle_record(self):
        header=(ROOT/'src/PlayerLifecycleView.hpp').read_text()
        source=(ROOT/'src/PlayerMovement.cpp').read_text()
        self.assertIn('struct EffectManager;',header)
        self.assertIn('EffectManager *effectManager0C;',header)
        self.assertIn('PlayerSideEffectAt0C',header)
        self.assertIn('(offsetof(PlayerSideStateView, effectManager0C) == 0x0C)',header)
        self.assertIn('PlayerSideStateSizeIs38',header)
        self.assertIn('typedef PlayerSideStateView PlayerMovementSideView;',source)
        self.assertNotIn('struct PlayerMovementSideView',source)
        self.assertIn('player->sideState->effectManager0C->SpawnEffectInFixedSlot(',source)
        self.assertIn('g_PlayerFocusEffectIds[player->sideState->shotType20]',source)
        self.assertNotIn('PlayerMovementSide(player)->',source)

    def test_player_movement_calls_genuine_effect_manager_spawn(self):
        source=(ROOT/'src/PlayerMovement.cpp').read_text()
        for token in ('PlayerMovementFocusEffect(player) =',
                      'PlayerMovementSecondaryEffect(player) =',
                      'effectManager0C->SpawnEffectInFixedSlot('):
            self.assertIn(token,source)
        with (ROOT/'config/match-units.toml').open('rb') as f:
            u=tomllib.load(f)['units']
        self.assertEqual(u['effect-manager-spawn-effect-fixed-slot']['target_address'],0x0040CD70)
        self.assertEqual(u['player-constructor']['target_address'],0x0041F070)
        self.assertEqual(u['player-on-update']['target_address'],0x0041E900)

if __name__=='__main__':
    unittest.main()
