"""Exact 553-byte FrontResult owner reads real Player and GameManager fields.

Original PE/COFF complete comparisons separately prove machine bytes. This
statically protects the physical 0xA8/0xB0/0xB4/0xB8/0x30418 score fields,
including the otherwise easy-to-lose repeated +0x30418 owner reload.
"""
from pathlib import Path
import tomllib
import unittest

ROOT=Path(__file__).resolve().parents[1]

class FrontResultNativePlayerManager(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        with (ROOT/'config/match-units.toml').open('rb') as file:
            cls.units=tomllib.load(file)['units']

    def test_no_third_front_result_game_manager_or_player_allocation(self):
        source=(ROOT/'src/FrontResultBonus.cpp').read_text()
        player=(ROOT/'src/PlayerLifecycleView.hpp').read_text()
        manager=(ROOT/'src/GameManagerRuntimeView.hpp').read_text()
        for name in ('FrontResultPlayerView','FrontResultSideRuntimeView',
                     'FrontResultSideView','FrontResultGameManagerLayout',
                     'FrontResultGameManager()'):
            with self.subTest(shadow=name):
                self.assertNotIn(name,source)
        self.assertIn('#include "PlayerLifecycleView.hpp"',source)
        self.assertIn('#include "GameManagerRuntimeView.hpp"',source)
        self.assertIn('FrontResultRealNativePlayerScores',source)
        self.assertIn('PlayerLifecycleView *player =',source)
        self.assertIn('PlayerOwnerStateView ownerState30410;',player)
        self.assertIn('int maxState08;',player)
        self.assertIn('AsciiGameManagerSideView sides[2];',manager)
        self.assertIn('int field0FC;',manager)
        self.assertIn('g_GameManager.sides[this->side10964].AddScore(this->bonus11E88[6]);',source)
        self.assertIn('g_GameManager.sides[0].state->phase10 = 9999;',source)
        self.assertIn('g_GameManager.sides[1].shotType20',source)

    def test_exact_original_native_player_score_offsets(self):
        source=(ROOT/'src/FrontResultBonus.cpp').read_text()
        expected=(('state84','0xA8'),('state8C','0xB0'),
                  ('state90','0xB4'),('state94','0xB8'))
        for member,offset in expected:
            with self.subTest(member=member):
                self.assertIn('offsetof(PlayerLifecycleView, header24) + offsetof(PlayerHeaderStateView, '+member+') == '+offset,source)
                self.assertIn('player04->header24.'+member,source)
        self.assertIn('offsetof(PlayerLifecycleView, ownerState30410) + offsetof(PlayerOwnerStateView, maxState08) == 0x30418',source)
        self.assertIn('PlayerOwnerStateSizeIs44', (ROOT/'src/PlayerLifecycleView.hpp').read_text())
        self.assertIn('player->ownerState30410.maxState08 < 1000',source)
        self.assertIn('g_GameManager.sides[side].player04->ownerState30410.maxState08',source)
        self.assertIn('sizeof(AsciiGameManagerSideView) == 0x38',source)
        self.assertIn('offsetof(AsciiGameManagerSideView, state) == 0x1C',source)
        self.assertIn('offsetof(AsciiGameSideRuntimeView, phase10) == 0x10',source)

    def test_complete_original_function_abi_and_real_exact_score_leaf(self):
        u=self.units['front-result-initialize-bonus']
        self.assertEqual((u['target_address'],u['size'],len(u['relocations'])),
                         (0x00415E50,553,27))
        self.assertEqual(u['symbol'],'?InitializeResultBonus@FrontResultBonusView@@QAEXXZ')
        calls=[r for r in u['relocations']
               if r['symbol']=='?AddScore@AsciiGameManagerSideView@@QAEXH@Z']
        self.assertEqual([(r['offset'],r['target'],r['type']) for r in calls],
                         [(0x222,0x0040F860,'REL32')])
        self.assertEqual(self.units['game-manager-side-add-score']['size'],33)
        self.assertEqual(self.units['game-manager-side-add-score']['target_address'],0x0040F860)
        self.assertEqual(self.units['player-owner-apply-reward']['compare_size'],1540)

if __name__=='__main__':unittest.main()
