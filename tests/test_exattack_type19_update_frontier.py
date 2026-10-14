"""Full-size TH09 Type19 update boss plus exact consumer regression gates."""

import importlib.util
from pathlib import Path
import unittest
from unittest.mock import patch

ROOT = Path(__file__).resolve().parents[1]
spec = importlib.util.spec_from_file_location(
    'type19_update_boss', ROOT / 'scripts/check-boss-frontiers.py')
boss = importlib.util.module_from_spec(spec)
spec.loader.exec_module(boss)


class Type19UpdateColdOwnerGates(unittest.TestCase):
    @staticmethod
    def evidence():
        return {
            'diagnostic_only': True, 'exactness_credit': 'none',
            'target_bytes': 821, 'candidate_bytes': 821,
            'target_instructions': 254, 'candidate_instructions': 254,
            'target_fields': 35, 'candidate_fields': 35,
            'ordered_fields_agree': False,
            'distinct_player_slot_fields': 4,
            'target_calls': 11, 'candidate_calls': 11,
            'target_direct_blocks': 30, 'candidate_direct_blocks': 30,
            'complete_direct_graphs_agree': True,
            'overlapping_differences': 198,
        }

    def test_same_original_player_extra_and_exact_trail_draw(self):
        with patch.object(boss, 'build_nonexact') as build_source, patch.object(
                boss, 'build') as build_exact, patch.object(
                boss, 'parsed', return_value=self.evidence()), patch.object(
                boss, 'canonical', side_effect=lambda unit: {'unit': unit}):
            frontier, exact = boss.run_exattack_type19_update(protect_exact=True)
        self.assertEqual(frontier['exattack_type19_update']['ordinary_differences'], 198)
        self.assertFalse(frontier['exattack_type19_update']['ordered_fields_agree'])
        self.assertEqual([item['unit'] for item in exact], ['exattack-type19-21-draw'])
        build_source.assert_called_once()
        build_exact.assert_called_once_with('exattack-type19-21-draw')

    def test_retain_nonexact_observations_without_claiming_acceptance(self):
        with patch.object(boss, 'build_nonexact'), patch.object(
                boss, 'parsed', return_value=self.evidence()):
            frontier, exact = boss.run_exattack_type19_update(protect_exact=False)
        self.assertEqual(exact, [])
        self.assertEqual(frontier['exattack_type19_update']['bound_fields'], 35)
        self.assertEqual(frontier['exattack_type19_update']['ordered_calls'], 11)

    def test_independent_original_fields_player_slots_calls_and_graph_are_required(self):
        for changed in ({'target_bytes': 820}, {'candidate_fields': 34},
                        {'target_fields': 34}, {'target_instructions': 253},
                        {'candidate_instructions': 255},
                        {'target_calls': 10}, {'candidate_calls': 12},
                        {'target_direct_blocks': 29},
                        {'candidate_direct_blocks': 31},
                        {'distinct_player_slot_fields': 3},
                        {'complete_direct_graphs_agree': False},
                        {'diagnostic_only': False},
                        {'exactness_credit': 'accepted'}):
            with self.subTest(changed=changed), patch.object(
                    boss, 'build_nonexact'), patch.object(
                    boss, 'parsed', return_value={**self.evidence(), **changed}), self.assertRaises(RuntimeError):
                boss.run_exattack_type19_update(protect_exact=False)


if __name__ == '__main__':
    unittest.main()
