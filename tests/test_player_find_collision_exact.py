"""TH09 Player FindCollision same-TU original-ABI and seven-owner exact gates."""

import importlib.util
from pathlib import Path
import tomllib
import unittest
from unittest.mock import patch

ROOT=Path(__file__).resolve().parents[1]
spec=importlib.util.spec_from_file_location('player_collision_boss',ROOT/'scripts/check-boss-frontiers.py')
frontier=importlib.util.module_from_spec(spec)
spec.loader.exec_module(frontier)


class PlayerFindCollisionSourceGates(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.units=tomllib.loads((ROOT/'config/match-units.toml').read_text())['units']
        cls.source=(ROOT/'src/PlayerCollisionQuery.cpp').read_text()
        cls.helpers=(ROOT/'src/PlayerCollisionHelpers.cpp').read_text()

    def test_independent_original_full_function_identity(self):
        owner=self.units['player-find-collision']
        self.assertEqual(owner['target_address'],0x0041D810)
        self.assertEqual(owner['size'],602)
        self.assertEqual(owner['symbol'],
          '?FindCollision@PlayerCollisionQueryStateView@@QAEPAUPlayerCollisionQueryRecordView@@ABUPlayerPositionView@@0M@Z')
        self.assertEqual(owner['source'],'src/PlayerCollisionQuery.cpp')
        self.assertEqual(len(owner['relocations']),9)
        self.assertEqual([r['offset'] for r in owner['relocations']],
                         [0x0F,0x1F,0x83,0xA8,0xF2,0x108,0x120,0x175,0x1C8])
        self.assertEqual(sum(r['type']=='REL32' for r in owner['relocations']),7)
        self.assertEqual(sum(r['type']=='DIR32' for r in owner['relocations']),2)
        self.assertEqual([r['target'] for r in owner['relocations'] if r['type']=='DIR32'],
                         [0x48E314,0x48E314])
        self.assertTrue(all(r.get('addend',0)==0 for r in owner['relocations']))
        self.assertIn('/O2',owner['profile'])
        self.assertIn('/Oy-',owner['profile'])

    def test_exact_callee_body_is_owned_once_in_same_source_object(self):
        main=self.units['player-find-collision']
        callee=self.units['player-calc-item-box-collision']
        self.assertEqual(callee['size'],121)
        self.assertEqual(callee['source'],main['source'])
        self.assertEqual(callee['object'],main['object'])
        self.assertEqual(callee['profile'],main['profile'])
        self.assertEqual(self.source.count('int PlayerLifecycleView::CalcItemBoxCollision('),1)
        self.assertEqual(self.helpers.count('int PlayerLifecycleView::CalcItemBoxCollision('),0)
        self.assertLess(self.source.index('PlayerCollisionQueryStateView::FindCollision('),
                        self.source.index('int PlayerLifecycleView::CalcItemBoxCollision('))
        self.assertEqual(self.source.count('if (player00->CalcItemBoxCollision('),1)
        self.assertEqual(self.source.count('PlayerCollisionQueryRecordView *record = records;'),1)
        self.assertLess(self.source.index('PlayerCollisionQueryRecordView *record = records;'),
                        self.source.index('PlayerPositionView previousPosition = *position;'))
        self.assertEqual(self.source.count('player00->position1B88 = previousPosition;'),4)
        self.assertEqual(self.source.count('player00->hurtboxHalfSize = previousHalfSize;'),4)

    def test_original_box_laser_circle_are_seven_target_calls(self):
        owner=self.units['player-find-collision']
        ref=[r['symbol'] for r in owner['relocations'] if r['type']=='REL32']
        self.assertEqual(ref[-3:],[
            '?CalcItemBoxCollision@PlayerLifecycleView@@QAEHPAUPlayerPositionView@@0@Z',
            '?CalcLaserHitbox@PlayerLifecycleView@@QAEHPAUPlayerPositionView@@00MH@Z',
            '?CalcCircleCollision@PlayerLifecycleView@@QAEHPAUPlayerPositionView@@M@Z',
        ])
        self.assertEqual([r['target'] for r in owner['relocations']][-3:],
                         [0x41BDF0,0x41BFC0,0x41BE70])


class PlayerFindCollisionColdSweep(unittest.TestCase):
    @staticmethod
    def noncrediting_bullet():
        return {
            'diagnostic_only': True, 'exactness_credit': 'none',
            'target_bytes': 862, 'candidate_bytes': 849,
            'target_instructions': 277, 'candidate_instructions': 266,
            'original_fields': 26, 'bound_fields': 26,
            'ordered_calls': 12, 'ordered_calls_agree': True,
            'ordered_fields_agree': True, 'original_blocks': 33,
            'candidate_blocks': 32, 'direct_graphs_agree': False,
            'overlap_differences': 804, 'missing_target_bytes': 13,
            'extra_candidate_bytes': 0, 'complete_bytes_equal': False,
        }

    def test_real_same_tu_and_five_other_exact_helpers_are_rebuilt(self):
        with patch.object(frontier,'build') as build, patch.object(frontier,'canonical',
                    side_effect=lambda unit:{'unit':unit}), patch.object(
                    frontier, 'parsed', return_value=self.noncrediting_bullet()) as parsed:
            owners,exact=frontier.run_player_collision(protect_exact=True)
        self.assertEqual(owners['player_check_bullet_collision']['target_bytes'], 862)
        parsed.assert_called_once_with(['scripts/inspect-player-bullet-collision.py',
                                        'build/matching/PlayerCollisionHelpers.obj', '--json'])
        self.assertEqual(len(exact),7)
        self.assertEqual([x['unit'] for x in exact],[
            'player-find-collision','player-calc-item-box-collision',
            'player-check-graze-collision','player-calc-laser-hitbox',
            'player-calc-item-collection-collision','player-calc-circle-collision',
            'player-find-collision-at-player',
        ])
        self.assertEqual([x.args[0] for x in build.call_args_list],
                        ['player-find-collision',
                         'player-check-graze-collision', 'player-calc-laser-hitbox',
                         'player-calc-item-collection-collision',
                         'player-calc-circle-collision','player-find-collision-at-player'])

    def test_skipping_neighbors_does_not_skip_new_exact_large_owner(self):
        with patch.object(frontier,'build') as build, patch.object(frontier,'canonical',
                          return_value={'unit':'player-find-collision'}) as exact, patch.object(
                          frontier,'parsed',return_value=self.noncrediting_bullet()):
            owners, siblings=frontier.run_player_collision(protect_exact=False)
        self.assertEqual(owners['player_check_bullet_collision']['candidate_bytes'], 849)
        self.assertEqual(siblings,[{'unit':'player-find-collision'}])
        self.assertEqual([call.args[0] for call in build.call_args_list],
                         ['player-find-collision', 'player-check-graze-collision'])
        exact.assert_called_once_with('player-find-collision')


if __name__=='__main__':
    unittest.main()
