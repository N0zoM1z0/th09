"""TH09 exact ApplyReward uses one native 0x44-byte owner-state allocation.

The whole 1,540-byte original-PE comparison remains the exactness oracle;
these checks guard storage identity and callback/constructor ownership.
"""
from pathlib import Path
import tomllib
import unittest

ROOT=Path(__file__).resolve().parents[1]

class NativeFullPlayerOwnerStateReward(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        with (ROOT/'config/match-units.toml').open('rb') as f:
            cls.units=tomllib.load(f)['units']

    def test_one_actual_full_owner_state_not_duplicated_reward_overlay(self):
        header=(ROOT/'src/PlayerLifecycleView.hpp').read_text()
        reward=(ROOT/'src/PlayerOwnerStateReward.cpp').read_text()
        self.assertEqual(header.count('struct PlayerOwnerStateView\n{'),1)
        self.assertNotIn('struct PlayerOwnerRewardStateView',reward)
        self.assertNotIn('reinterpret_cast<PlayerOwnerRewardStateView *>(this)',reward)
        self.assertIn('PlayerOwnerStateView *state = this;',reward)
        self.assertIn('PlayerOwnerStateNativeFullMembers',header)
        self.assertIn('PlayerOwnerStateView ownerState30410;',header)
        self.assertIn('PlayerLifecycleView *playerOwner00;',header)
        self.assertIn('PlayerOwnerRewardCallback rewardCallback40;',header)
        self.assertIn('state->rewardCallback40(this, position);',reward)
        self.assertIn('state->playerOwner00->sideState->AddScore(5000);',reward)
        self.assertNotIn('state->owner00->',reward)
        self.assertIn('state->timer14 = current + 45;',reward)
        self.assertIn('static_cast<int>(state->timer20)',reward)

    def test_all_0x44_original_native_fields_and_two_constructed_timers(self):
        h=(ROOT/'src/PlayerLifecycleView.hpp').read_text()
        fields=(('owner00','0x00'),('state04','0x04'),('maxState08','0x08'),
                ('value0C','0x0C'),('maxValue10','0x10'),('timer14','0x14'),
                ('timer20','0x20'),('rewardMeter2C','0x2C'),
                ('spawnCounter30','0x30'),('value34','0x34'),
                ('state38','0x38'),('activeFrameCounter3C','0x3C'),
                ('callback40','0x40'))
        for name,offset in fields:
            with self.subTest(member=name):
                self.assertIn('offsetof(PlayerOwnerStateView, '+name+') == '+offset,h)
        self.assertIn('PlayerOwnerStateSizeIs44',h)
        self.assertIn('ZunTimer timer14;',h)
        self.assertIn('ZunTimer timer20;',h)
        self.assertIn('PlayerOwnerRewardCallback',h)
        self.assertEqual(h.count('PlayerOwnerStateView ownerState30410;'),1)
        self.assertIn('offsetof(PlayerLifecycleView, ownerState30410) == 0x30410',h)

    def test_full_original_reward_and_callbacks_not_new_exact_credit(self):
        unit=self.units['player-owner-apply-reward']
        self.assertEqual((unit['target_address'],unit['size'],unit['compare_size'],len(unit['relocations'])),
                         (0x0041D150,1519,1540,91))
        self.assertEqual(unit['symbol'],'?ApplyReward@PlayerOwnerStateView@@QAEXPAUPlayerPositionView@@HHHH@Z')
        self.assertEqual(self.units['front-result-initialize-bonus']['size'],553)
        self.assertEqual(self.units['player-owner-reward-state-advance']['size'],12)
        self.assertEqual(len([r for r in unit['relocations'] if r['symbol'].startswith('$L')]),6)
        # These source-present owners were already exact before this layout change.
        self.assertNotIn('player-check-bullet-collision',self.units)

if __name__=='__main__':unittest.main()
