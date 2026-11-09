"""ApplyReward uses the actual GameManager and Supervisor global objects.

The 1,540-byte physical original PE/COFF comparison validates the full reward
state machine and its table; source tests guard these native C++ identities.
"""
from pathlib import Path
import tomllib
import unittest

ROOT=Path(__file__).resolve().parents[1]
GAME='?g_GameManager@@3UAsciiGameManagerView@@A'
SUPER='?g_Supervisor@@3VSupervisor@@A'

class PlayerOwnerRewardNativeManagers(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        with (ROOT/'config/match-units.toml').open('rb') as fd:
            cls.units=tomllib.load(fd)['units']

    def test_native_global_ownership_across_complete_reward_logic(self):
        source=(ROOT/'src/PlayerOwnerStateReward.cpp').read_text()
        self.assertIn('#include "GameManagerRuntimeView.hpp"',source)
        self.assertIn('#include "Supervisor.hpp"',source)
        self.assertNotIn('struct PlayerRewardSupervisorView',source)
        self.assertNotIn('struct PlayerRewardGameManagerView',source)
        self.assertNotIn('g_PlayerSupervisorRuntime',source)
        self.assertNotIn('g_PlayerGameManagerRuntime',source)
        self.assertEqual(source.count('g_Supervisor.SelectSide('),4)
        self.assertEqual(source.count('g_GameManager.TransformPopupX('),2)
        self.assertEqual(source.count('g_GameManager.TransformPopupY('),2)
        self.assertIn('state->rewardMeter2C',source)
        self.assertIn('state->spawnCounter30',source)
        self.assertEqual(source.count('SpawnEffectWithVelocity('),2)
        self.assertIn('state->playerOwner00->sideIndex',source)
        self.assertIn('PlayerOwnerStateView *state = this;',source)
        self.assertNotIn('PlayerOwnerRewardStateView',source)
        self.assertIn('g_GameManager.frontFactoryA8->GetTransitionBlockFlag()',source)
        self.assertNotIn('PlayerRewardSharedRuntimeView',source)

    def test_real_original_addresses_and_exact_callee_providers(self):
        owner=self.units['player-owner-apply-reward']
        self.assertEqual((owner['target_address'],owner['size'],owner['compare_size'],len(owner['relocations'])),
                         (0x0041D150,1519,1540,91))
        fields=owner['relocations']
        game_globals=[r for r in fields if r['symbol']==GAME]
        super_globals=[r for r in fields if r['symbol']==SUPER]
        self.assertEqual([r['offset'] for r in game_globals],
                         [0x0C,0x279,0x29C,0x3F4,0x417])
        self.assertEqual([(r['target'],r.get('addend',0),r['type'])
                          for r in game_globals if r['offset']==0x0C],
                         [(0x004A7D90,0xA8,'DIR32')])
        self.assertEqual([(r['target'],r['symbol']) for r in fields
                          if r['offset']==0x14],
                         [(0x0040D4F0,
                           '?GetTransitionBlockFlag@FrontFactoryConstructView@@QAEHXZ')])
        self.assertEqual([r['offset'] for r in super_globals], [0x2D5,0x343,0x450,0x4BC])
        self.assertTrue(all(r['target']==0x004A7D90 for r in game_globals))
        self.assertTrue(all(r['target']==0x004B3100 for r in super_globals))
        for name,target,offsets in (
            ('?TransformPopupX@AsciiGameManagerView@@QAEMM@Z',0x00401680,[0x27E,0x3F9]),
            ('?TransformPopupY@AsciiGameManagerView@@QAEMM@Z',0x004016B0,[0x2A4,0x41F]),
            ('?SelectSide@Supervisor@@QAEXH@Z',0x00401440,[0x2DA,0x348,0x455,0x4C1])):
            with self.subTest(name=name):
                calls=[r for r in fields if r['symbol']==name]
                self.assertEqual([r['offset'] for r in calls],offsets)
                self.assertTrue(all(r['target']==target and r['type']=='REL32'
                                    for r in calls))
        for unit,target in (('game-manager-transform-popup-x',0x00401680),
                            ('game-manager-transform-popup-y',0x004016B0),
                            ('supervisor-select-side',0x00401440)):
            self.assertEqual(self.units[unit]['target_address'],target)
        private=[r for r in fields if r['symbol'].startswith('$L')]
        self.assertEqual(len(private),6)
        self.assertTrue(any(r['offset']>=1519 for r in private))

    def test_independent_original_reward_inspector_uses_native_symbols(self):
        inspector=(ROOT/'scripts/inspect-player-owner-reward.py').read_text()
        self.assertIn(GAME,inspector)
        self.assertIn(SUPER,inspector)
        self.assertIn('?TransformPopupX@AsciiGameManagerView@@QAEMM@Z',inspector)
        self.assertIn('?SelectSide@Supervisor@@QAEXH@Z',inspector)
        self.assertNotIn('PlayerRewardSupervisorView',inspector)
        self.assertNotIn('PlayerRewardGameManagerView',inspector)

if __name__=='__main__':unittest.main()
