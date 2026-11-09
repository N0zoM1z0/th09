"""One actual 0x38-byte GameManager side owns exact AddScore and callers.

Complete original PE/COFF source-cold comparisons, not these source tests,
prove the 33-byte leaf and entire reward/result function and jump tables.
"""
from pathlib import Path
import tomllib
import unittest

ROOT = Path(__file__).resolve().parents[1]
CALLEE = '?AddScore@AsciiGameManagerSideView@@QAEXH@Z'

class NativeGameManagerSideScoreOwner(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        with (ROOT/'config/match-units.toml').open('rb') as file:
            cls.units = tomllib.load(file)['units']

    def test_native_score_storage_and_no_shadow_score_side(self):
        header = (ROOT/'src/GameManagerSideState.hpp').read_text()
        native = (ROOT/'src/GameManagerRuntimeView.hpp').read_text()
        method = (ROOT/'src/GameManagerSideScore.cpp').read_text()
        self.assertFalse((ROOT/'src/GameManagerSideScoreView.hpp').exists())
        self.assertIn('void AddScore(int score);', header)
        self.assertIn('struct AsciiGameManagerSideView', header)
        self.assertIn('sizeof(AsciiGameManagerSideView) == 0x38', header)
        self.assertIn('AsciiGameSideRuntimeView *state;',header)
        self.assertIn('union { unsigned int value08; int score08; };', native)
        self.assertIn('AsciiGameSideNativeScoreWordAt08',native)
        self.assertIn('offsetof(AsciiGameSideRuntimeView, score08) == 0x08', native)
        self.assertIn('offsetof(AsciiGameManagerSideView, state) == 0x1C',native)
        self.assertIn('void AsciiGameManagerSideView::AddScore(int score)',method)
        self.assertIn('this->state->score08 += score / 10;',method)
        self.assertNotIn('GameManagerSideStateView',method)

    def test_original_exact_score_leaf_and_all_source_callers(self):
        leaf = self.units['game-manager-side-add-score']
        self.assertEqual((leaf['target_address'],leaf['size'],len(leaf['relocations'])),
                         (0x0040F860,33,0))
        self.assertEqual(leaf['symbol'],CALLEE)
        calls = {
            'player-owner-apply-reward':(1519,[0x8D]),
            'front-result-initialize-bonus':(553,[0x222]),
        }
        for unit,(size,offsets) in calls.items():
            with self.subTest(unit=unit):
                spec = self.units[unit]
                self.assertEqual(spec['size'],size)
                refs = [r for r in spec['relocations'] if r['symbol']==CALLEE]
                self.assertEqual([r['offset'] for r in refs],offsets)
                self.assertTrue(all(r['type']=='REL32' and r['target']==0x0040F860
                                    for r in refs))
        reward = self.units['player-owner-apply-reward']
        self.assertEqual(reward['compare_size'],1540)
        self.assertEqual(len(reward['relocations']),91)
        timer=[r for r in reward['relocations'] if r['offset']==0x5D6]
        self.assertEqual([(r['symbol'],r['target']) for r in timer],
                         [('??BZunTimer@@QAEHXZ',0x00435F00)])
        self.assertEqual(self.units['zun-timer-current-value-alias']['target_address'],0x00435F00)

    def test_enemy_player_and_front_result_use_actual_side(self):
        enemy=(ROOT/'src/EnemyManagerCore.inl').read_text()
        player=(ROOT/'src/PlayerCollisionHelpers.cpp').read_text()
        reward=(ROOT/'src/PlayerOwnerStateReward.cpp').read_text()
        front=(ROOT/'src/FrontResultBonus.cpp').read_text()
        self.assertEqual(enemy.count('reinterpret_cast<AsciiGameManagerSideView *>('),2)
        self.assertEqual(enemy.count('manager->sideState320)->AddScore(enemy->score2E54);'),2)
        self.assertIn('this->sideState->AddScore(50);',player)
        self.assertIn('state->playerOwner00->sideState->AddScore(5000);',reward)
        self.assertIn('g_GameManager.sides[0].runtime->resultTransition34',
                      (ROOT/'src/FrontCalcCallback.cpp').read_text())
        self.assertIn('static_cast<int>(state->timer20)',reward)
        self.assertNotIn('PlayerRewardSideScoreView',reward)
        self.assertNotIn('PlayerRewardTimerCurrentView',reward)
        self.assertIn('AsciiGameManagerSideView',front)
        for file in ('EnemyManagerCore.inl','PlayerCollisionHelpers.cpp','PlayerOwnerStateReward.cpp'):
            self.assertNotIn('GameManagerSideScoreView', (ROOT/'src'/file).read_text())
        for name in ('functions.csv','implemented.csv','matches.csv','reccmp-functions.csv'):
            text=(ROOT/'config'/name).read_text()
            self.assertIn('AsciiGameManagerSideView::AddScore',text)
            self.assertNotIn('GameManagerSideStateView::AddScore',text)

if __name__=='__main__':unittest.main()
