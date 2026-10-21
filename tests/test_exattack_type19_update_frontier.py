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
            'literal_definitions_checked': 5,
            'literal_definitions_original_equal': True,
            'ordered_fields_agree': True,
            'distinct_player_slot_fields': 4,
            'target_calls': 11, 'candidate_calls': 11,
            'target_direct_blocks': 30, 'candidate_direct_blocks': 30,
            'complete_direct_graphs_agree': True,
            'target_sha256': boss.TYPE19_UPDATE_TARGET_SHA256,
            'overlapping_differences': 0,
            'difference_offsets': [],
            'complete_bytes_agree': True,
            'missing_target_bytes': 0, 'extra_candidate_bytes': 0,
        }

    def test_same_original_player_extra_and_exact_trail_draw(self):
        with patch.object(boss, 'build') as build_source, patch.object(
                boss, 'cold_replay_exact_source', return_value=[
                    {'unit': 'float3-from-angle-magnitude'}]) as replay_ecl, patch.object(
                boss, 'parsed', return_value=self.evidence()), patch.object(
                boss, 'canonical', side_effect=lambda unit: {'unit': unit}):
            frontier, exact = boss.run_exattack_type19_update(protect_exact=True)
        self.assertEqual(frontier['exattack_type19_update']['ordinary_differences'], 0)
        self.assertTrue(frontier['exattack_type19_update']['ordered_fields_agree'])
        self.assertEqual([item['unit'] for item in exact], [
            'float3-from-angle-magnitude', 'exattack-type19-21-draw'])
        self.assertEqual([call.args[0] for call in build_source.call_args_list], [
            'ecl-pop-context', 'exattack-type19-21-draw'])
        replay_ecl.assert_called_once_with(
            'src/EclManager.cpp', boss.ECL_EXACT_CARRIERS,
            already_built=('build/matching/EclManager.obj',))

    def test_retain_nonexact_observations_without_claiming_acceptance(self):
        with patch.object(boss, 'build'), patch.object(
                boss, 'parsed', return_value=self.evidence()):
            frontier, exact = boss.run_exattack_type19_update(protect_exact=False)
        self.assertEqual(exact, [])
        self.assertEqual(frontier['exattack_type19_update']['bound_fields'], 35)
        self.assertEqual(frontier['exattack_type19_update']['ordered_calls'], 11)

    def test_independent_original_fields_player_slots_calls_and_graph_are_required(self):
        for changed in ({'target_bytes': 820}, {'candidate_bytes': 820},
                        {'candidate_fields': 34}, {'target_sha256': '0' * 64},
                        {'target_fields': 34}, {'target_instructions': 253},
                        {'candidate_instructions': 255},
                        {'target_calls': 10}, {'candidate_calls': 12},
                        {'target_direct_blocks': 29},
                        {'candidate_direct_blocks': 31},
                        {'distinct_player_slot_fields': 3},
                        {'ordered_fields_agree': False},
                        {'missing_target_bytes': 1}, {'extra_candidate_bytes': 1},
                        {'complete_direct_graphs_agree': False},
                        {'diagnostic_only': False},
                        {'exactness_credit': 'accepted'}):
            with self.subTest(changed=changed), patch.object(
                    boss, 'build'), patch.object(
                    boss, 'parsed', return_value={**self.evidence(), **changed}), self.assertRaises(RuntimeError):
                boss.run_exattack_type19_update(protect_exact=False)

    def test_all_821_original_bytes_stay_protected(self):
        evidence = self.evidence()
        for offset in range(821):
            with self.subTest(offset=hex(offset)), self.assertRaisesRegex(
                    RuntimeError, 'canonical 821-byte'):
                boss.guard_exattack_type19_update({
                    **evidence, 'difference_offsets': [hex(offset)],
                    'overlapping_differences': 1, 'complete_bytes_agree': False})

    def test_former_near_match_is_rejected_even_without_sibling_replay(self):
        old = {**self.evidence(), 'complete_bytes_agree': False,
               'overlapping_differences': 44,
               'difference_offsets': [hex(i) for i in sorted(
                   boss.TYPE19_UPDATE_ORIGINAL_RESIDUAL_OFFSETS)]}
        with patch.object(boss, 'build'), patch.object(
                boss, 'parsed', return_value=old), self.assertRaisesRegex(
                RuntimeError, 'canonical 821-byte'):
            boss.run_exattack_type19_update(protect_exact=False)

    def test_literal_proofs_and_noncrediting_status_are_required(self):
        evidence = self.evidence()
        checked = boss.guard_exattack_type19_update(evidence)
        self.assertEqual(checked['differences'], 0)
        self.assertEqual(evidence['exactness_credit'], 'none')
        for changed in ({'literal_definitions_checked': 4},
                        {'literal_definitions_checked': 0},
                        {'literal_definitions_original_equal': False}):
            with self.subTest(changed=changed), self.assertRaisesRegex(
                    RuntimeError, 'owner drifted'):
                boss.guard_exattack_type19_update({**evidence, **changed})

    def test_prechecked_ecl_reuses_cold_parent_and_profile_receipts(self):
        with patch.object(boss, 'build') as build_source, patch.object(
                boss, 'cold_replay_exact_source') as replay_ecl, patch.object(
                boss, 'parsed', return_value=self.evidence()), patch.object(
                boss, 'canonical', side_effect=lambda unit: {'unit': unit}):
            _, exact = boss.run_exattack_type19_update(
                protect_exact=True, shared_ecl_prechecked=True)
        replay_ecl.assert_not_called()
        build_source.assert_called_once_with('exattack-type19-21-draw')
        self.assertEqual([item['unit'] for item in exact], ['exattack-type19-21-draw'])


if __name__ == '__main__':
    unittest.main()
