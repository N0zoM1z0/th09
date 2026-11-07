"""Real Player owns exact 12-byte state setter across ECL/Enemy/selector.

PE/COFF whole-function source-cold replays decide exactness. These assertions
protect the true object receiver and independently original-bound callees.
"""
from pathlib import Path
import tomllib
import unittest

ROOT=Path(__file__).resolve().parents[1]
SYMBOL='?SetUpdateState@PlayerLifecycleView@@QAEXH@Z'

class PlayerNativeStateSetterAcrossBosses(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        with (ROOT/'config/match-units.toml').open('rb') as f:
            cls.units=tomllib.load(f)['units']

    def test_true_player_method_not_extra_state_prefix(self):
        native=(ROOT/'src/PlayerLifecycleView.hpp').read_text()
        state=(ROOT/'src/PlayerState4OpsView.hpp').read_text()
        setter=(ROOT/'src/PlayerSetUpdateState.cpp').read_text()
        self.assertIn('void SetUpdateState(int state);',native)
        self.assertNotIn('void SetUpdateState(int state);',state)
        self.assertIn('void PlayerLifecycleView::SetUpdateState(int state)',setter)
        self.assertIn('updateState00 = state;',setter)
        self.assertIn('PlayerNative', native)
        self.assertEqual(self.units['player-set-update-state']['symbol'],SYMBOL)
        self.assertEqual(self.units['player-set-update-state']['size'],12)
        self.assertEqual(self.units['player-set-update-state']['target_address'],0x00403C20)
        self.assertEqual(len(self.units['player-set-update-state']['relocations']),0)

    def test_exact_selector_state3_and_runtime_share_same_native_player(self):
        selector=(ROOT/'src/PlayerGameplayState.cpp').read_text()
        runtime=(ROOT/'src/PlayerRuntime.cpp').read_text()
        state3=(ROOT/'src/PlayerState3EffectSetup.cpp').read_text()
        self.assertIn('phasePlayer->SetUpdateState(3);',selector)
        self.assertNotIn('PlayerState4OpsView',selector)
        self.assertEqual(state3.count('SetUpdateState(3);'),3)
        self.assertNotIn('PlayerState4OpsView',state3)
        self.assertIn('player->SetUpdateState(3);',runtime)
        self.assertIn('player->SetUpdateState(0);',runtime)
        expected={'player-check-state-4':0x148,
                  'player-transition-special':0xE5,
                  'player-update-common':0x80,
                  'player-state3-effect13':0x35,
                  'player-state3-effect14':0x35,
                  'player-state3-effect15':0x3B,
                  'player-update-selector-state':0x129}
        for unit,offset in expected.items():
            with self.subTest(unit=unit):
                rows=[r for r in self.units[unit]['relocations'] if r['symbol']==SYMBOL]
                self.assertEqual([(r['offset'],r['target'],r['type']) for r in rows],
                                 [(offset,0x00403C20,'REL32')])
        self.assertEqual((self.units['player-update-selector-state']['size'],
                          len(self.units['player-update-selector-state']['relocations'])),(3238,136))

    def test_enemy_big_boss_calls_native_method_at_original_va(self):
        source=(ROOT/'src/EnemyManagerCore.inl').read_text()
        checker=(ROOT/'scripts/inspect-enemy-manager-update.py').read_text()
        self.assertIn('reinterpret_cast<PlayerLifecycleView *>(manager->sideState320->player04)->SetUpdateState(3);',source)
        self.assertNotIn('PlayerState4OpsView',source)
        self.assertIn(SYMBOL,checker)
        self.assertIn('0x00403C20',checker)
        self.assertIn('(0xC06,',checker)
        for tracker in ('functions.csv','implemented.csv','matches.csv','reccmp-functions.csv'):
            with self.subTest(tracker=tracker):
                text=(ROOT/'config'/tracker).read_text()
                self.assertIn('PlayerLifecycleView::SetUpdateState',text)
                self.assertNotIn('PlayerState4OpsView::SetUpdateState',text)

if __name__=='__main__':unittest.main()
