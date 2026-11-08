"""16 exact ExAttack reward callbacks borrow the real 0x44-byte Player owner.

Whole pinned VC7.1 original PE/COFF strict comparisons are the byte oracle.
"""
from pathlib import Path
import tomllib
import unittest

ROOT=Path(__file__).resolve().parents[1]

class PlayerNativeOwnerRewardCallbacks(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        with (ROOT/'config/match-units.toml').open('rb') as fd:
            cls.units=tomllib.load(fd)['units']

    def test_callback_code_has_one_real_state_not_fake_prefix(self):
        source=(ROOT/'src/PlayerOwnerRewardCallbacks.cpp').read_text()
        native=(ROOT/'src/PlayerLifecycleView.hpp').read_text()
        reward=(ROOT/'src/PlayerOwnerStateReward.cpp').read_text()
        self.assertNotIn('PlayerOwnerRewardCallbackStateView',source)
        self.assertIn('PlayerRewardCallbackNativeStateAt34',source)
        self.assertEqual(source.count('PlayerOwnerStateView *state = base;'),16)
        self.assertEqual(source.count('state->playerOwner00->sideIndex'),16)
        self.assertNotIn('state->owner00->sideIndex',source)
        self.assertNotIn('state->value30',source)
        self.assertEqual(source.count('state->spawnCounter30'),4)
        self.assertIn('state->spawnCounter30 = oldValue30 - 5;',source)
        self.assertIn('PlayerOwnerStateView *state = this;',reward)
        self.assertIn('state->rewardCallback40(this, position);',reward)
        self.assertIn('PlayerOwnerStateNativeFullMembers',native)
        self.assertIn('offsetof(PlayerOwnerStateView, value34) == 0x34',native)
        self.assertIn('offsetof(PlayerOwnerStateView, spawnCounter30) == 0x30',native)

    def test_complete_original_sixteen_callback_owner_extents(self):
        names=['player-owner-reward-callback-type'+str(i) for i in range(16)]
        items=[self.units[n] for n in names]
        self.assertEqual(len(items),16)
        self.assertEqual(sum(int(u.get('compare_size',u['size'])) for u in items),1604)
        self.assertEqual(sum(len(u['relocations']) for u in items),79)
        self.assertTrue(all(u['source']=='src/PlayerOwnerRewardCallbacks.cpp' for u in items))
        for i,unit in enumerate(items):
            with self.subTest(callback=i):
                self.assertIn('PlayerOwnerRewardCallbackType'+str(i),unit['symbol'])
                self.assertGreater(unit['size'],80)
                self.assertTrue(any('Spawn@ExAttackController@@' in r['symbol']
                                    for r in unit['relocations']))
        self.assertEqual((self.units['player-owner-apply-reward']['size'],
                          self.units['player-owner-apply-reward']['compare_size']),
                         (1519,1540))
        self.assertEqual(self.units['player-update-owner-state']['size'],126)
        self.assertNotIn('player-check-bullet-collision',self.units)

if __name__=='__main__':unittest.main()
