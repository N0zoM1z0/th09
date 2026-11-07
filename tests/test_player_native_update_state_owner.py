"""Native 12-byte Player state setter and exact Enemy/selector callers.

The pinned VC7.1 PE/COFF original function comparisons are a separate oracle.
"""
from pathlib import Path
import tomllib
import unittest

ROOT=Path(__file__).resolve().parents[1]
CALL='?SetUpdateState@PlayerLifecycleView@@QAEXH@Z'

class PlayerNativeUpdateStateOwner(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        with (ROOT/'config/match-units.toml').open('rb') as f:
            cls.units=tomllib.load(f)['units']

    def test_one_real_player_setter_and_preserved_transition_interface(self):
        native=(ROOT/'src/PlayerLifecycleView.hpp').read_text()
        old=(ROOT/'src/PlayerState4OpsView.hpp').read_text()
        implementation=(ROOT/'src/PlayerSetUpdateState.cpp').read_text()
        self.assertIn('void SetUpdateState(int state);',native)
        self.assertNotIn('void SetUpdateState(int state);',old)
        self.assertIn('void SpawnTransitionA(',old)
        self.assertIn('void SpawnTransitionB(',old)
        self.assertIn('void PlayerLifecycleView::SetUpdateState(int state)',implementation)
        self.assertIn('updateState00 = state;',implementation)
        self.assertIn('#include "PlayerLifecycleView.hpp"',implementation)
        self.assertNotIn('PlayerState4OpsView',implementation)

    def test_real_player_receiver_across_state3_selector_enemy_boss(self):
        player=(ROOT/'src/PlayerRuntime.cpp').read_text()
        enemy=(ROOT/'src/EnemyManagerCore.inl').read_text()
        selector=(ROOT/'src/PlayerGameplayState.cpp').read_text()
        state3=(ROOT/'src/PlayerState3EffectSetup.cpp').read_text()
        self.assertIn('player->SetUpdateState(3);',player)
        self.assertIn('player->SetUpdateState(0);',player)
        self.assertIn('phasePlayer->SetUpdateState(3);',selector)
        self.assertNotIn('PlayerState4OpsView',selector)
        self.assertEqual(state3.count('SetUpdateState(3);'),3)
        self.assertNotIn('PlayerState4OpsView',state3)
        self.assertIn('reinterpret_cast<PlayerLifecycleView *>(manager->sideState320->player04)->SetUpdateState(3);',enemy)
        self.assertNotIn('PlayerState4OpsView',enemy)
        self.assertIn('PlayerState4OpsView *ops',player)

    def test_independently_exact_source_and_original_reloc_sites(self):
        leaf=self.units['player-set-update-state']
        self.assertEqual((leaf['symbol'],leaf['size'],leaf['target_address'],len(leaf['relocations'])),
                         (CALL,12,0x00403C20,0))
        callers={
            'player-check-state-4':0x148,
            'player-transition-special':0xE5,
            'player-update-common':0x80,
            'player-state3-effect13':0x35,
            'player-state3-effect14':0x35,
            'player-state3-effect15':0x3B,
            'player-update-selector-state':0x129,
        }
        for unit,offset in callers.items():
            with self.subTest(unit=unit):
                rows=[r for r in self.units[unit]['relocations'] if r['symbol']==CALL]
                self.assertEqual(len(rows),1)
                self.assertEqual((rows[0]['offset'],rows[0]['type'],rows[0]['target'],rows[0].get('addend',0)),
                                 (offset,'REL32',0x00403C20,0))
        self.assertEqual((self.units['player-update-selector-state']['size'],len(self.units['player-update-selector-state']['relocations'])),(3238,136))
        resolver=self.units['player-resolve-pattern-offset']
        self.assertEqual((resolver['size'],resolver['compare_size'],len(resolver['relocations'])),(346,412,18))
        inspector=(ROOT/'scripts/inspect-enemy-manager-update.py').read_text()
        self.assertIn(CALL,inspector)
        self.assertIn('0x00403C20',inspector)

    def test_function_inventories_name_native_receiver_not_prefix(self):
        for file in ('functions.csv','implemented.csv','matches.csv','reccmp-functions.csv'):
            with self.subTest(file=file):
                txt=(ROOT/'config'/file).read_text()
                self.assertIn('PlayerLifecycleView::SetUpdateState',txt)
                self.assertNotIn('PlayerState4OpsView::SetUpdateState',txt)

if __name__=='__main__':unittest.main()
