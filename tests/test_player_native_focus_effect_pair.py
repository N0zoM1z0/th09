"""Original Player focus-effect pair consumed by the movement and Enemy bosses.

Pure source/layout contract; full original 1.50a owner comparisons are run by
focused movement, added-state and Enemy oracle commands.
"""
from pathlib import Path
import tomllib
import unittest

ROOT=Path(__file__).resolve().parents[1]

class NativePlayerFocusEffects(unittest.TestCase):
    def test_two_effect_pointers_share_player_storage_boundary(self):
        header=(ROOT/'src/PlayerLifecycleView.hpp').read_text()
        movement=(ROOT/'src/PlayerMovement.cpp').read_text()
        enemy=(ROOT/'src/EnemyManager.hpp').read_text()
        shared=(ROOT/'src/EnemyCorePlayerView.hpp').read_text()
        self.assertIn('Effect *focusEffect364;',header)
        self.assertIn('Effect *secondaryEffect368;',header)
        self.assertIn('PlayerLifecycleFocusEffectAt364',header)
        for field,offset in (('focusEffect364','0x364'),('secondaryEffect368','0x368'),('collisionQuery36C','0x36C')):
            self.assertIn('offsetof(PlayerLifecycleView, '+field+') == '+offset,header)
        self.assertNotIn('unknown0364[0x08]',header)
        self.assertIn('return player->focusEffect364;',movement)
        self.assertIn('return player->secondaryEffect368;',movement)
        self.assertNotIn('reinterpret_cast<unsigned char *>(player) + 0x364',movement)
        self.assertNotIn('reinterpret_cast<unsigned char *>(player) + 0x368',movement)
        # Independent Enemy calc observes the identical physical focus pointer.
        self.assertIn('Effect *focusEffect364;',enemy)
        self.assertIn('EnemyRewardPlayerFocusAt364',enemy)
        self.assertIn('typedef EnemyRewardPlayerView EnemyCorePlayerView;',shared)

    def test_effect_allocation_is_real_and_original_callee_is_preserved(self):
        source=(ROOT/'src/PlayerMovement.cpp').read_text()
        self.assertIn('PlayerMovementFocusEffect(player) =',source)
        self.assertIn('PlayerMovementSecondaryEffect(player) =',source)
        self.assertIn('SpawnEffectInFixedSlot(',source)
        with (ROOT/'config/match-units.toml').open('rb') as f:
            u=tomllib.load(f)['units']
        self.assertEqual(u['effect-manager-spawn-effect-fixed-slot']['target_address'],0x0040CD70)
        self.assertEqual(u['player-on-update']['target_address'],0x0041E900)

if __name__=='__main__':
    unittest.main()
