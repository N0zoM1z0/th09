"""Real Player owns the 162-byte respawn reward leaf and boss callsites.

Full pinned VC7.1 PE/COFF comparison, not static checks, proves byte equality.
"""
from pathlib import Path
import tomllib
import unittest

ROOT=Path(__file__).resolve().parents[1]
NAME='?AddRespawnResource@PlayerLifecycleView@@QAEXM@Z'

class PlayerNativeRespawnResourceOwner(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        with (ROOT/'config/match-units.toml').open('rb') as f:
            cls.units=tomllib.load(f)['units']

    def test_one_actual_player_method_not_four_byte_state_prefix(self):
        native=(ROOT/'src/PlayerLifecycleView.hpp').read_text()
        old=(ROOT/'src/PlayerState4OpsView.hpp').read_text()
        source=(ROOT/'src/PlayerRuntime.cpp').read_text()
        collision=(ROOT/'src/PlayerCollisionHelpers.cpp').read_text()
        self.assertIn('void AddRespawnResource(float value);',native)
        self.assertNotIn('void AddRespawnResource(float value);',old)
        self.assertIn('void PlayerLifecycleView::AddRespawnResource(',source)
        self.assertIn('player->AddRespawnResource(respawnValue);',source)
        self.assertIn('AddRespawnResource(400.0f);',source)
        self.assertIn('this->AddRespawnResource(0.2f);',collision)
        self.assertNotIn('PlayerState4OpsView',collision)
        self.assertIn('PlayerSideStateView *sideState = player->sideState;',source)
        self.assertIn('player->scalar30388 += value;',source)
        self.assertIn('player->scalar30388 = 400.0f;',source)

    def test_exact_real_resource_leaf_and_two_exact_original_callers(self):
        provider=self.units['player-add-respawn-resource']
        self.assertEqual(provider['size'],162)
        self.assertEqual(provider['target_address'],0x0041BC90)
        self.assertEqual(provider['symbol'],NAME)
        self.assertEqual(len(provider['relocations']),5)
        expected=(('player-check-state-4',0x13F),
                  ('player-update-before-state',0xCC))
        for unit,offset in expected:
            with self.subTest(unit=unit):
                calls=[r for r in self.units[unit]['relocations']
                       if r['symbol']==NAME]
                self.assertEqual(len(calls),1)
                self.assertEqual(calls[0]['offset'],offset)
                self.assertEqual(calls[0]['type'],'REL32')
                self.assertEqual(calls[0]['target'],0x0041BC90)
        self.assertEqual(self.units['player-check-state-4']['size'],400)
        self.assertEqual(self.units['player-update-before-state']['size'],814)
        self.assertEqual(self.units['player-update-before-state']['compare_size'],852)

    def test_original_bullet_boss_diagnostic_and_function_inventory(self):
        script=(ROOT/'scripts/inspect-player-bullet-collision.py').read_text()
        self.assertIn(NAME,script)
        self.assertIn('0x0041BC90',script)
        for tracker in ('functions.csv','implemented.csv','matches.csv','reccmp-functions.csv'):
            with self.subTest(tracker=tracker):
                s=(ROOT/'config'/tracker).read_text()
                self.assertIn('PlayerLifecycleView::AddRespawnResource',s)
                self.assertNotIn('PlayerState4OpsView::AddRespawnResource',s)

if __name__=='__main__':unittest.main()
