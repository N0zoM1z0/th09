"""PE-anchored, noncrediting 516/517-byte ECL movement-branch guards.

All fixtures are synthetic and intentionally *not* Factory acceptance. The
source-cold EclManager carrier is checked by --owner runecl separately.
"""

import copy
import importlib.util
from pathlib import Path
import unittest

ROOT = Path(__file__).resolve().parents[1]
spec = importlib.util.spec_from_file_location(
    'biased_owner_gate', ROOT / 'scripts/check-boss-frontiers.py')
frontier = importlib.util.module_from_spec(spec)
spec.loader.exec_module(frontier)


class BiasedOriginalByteFrontier(unittest.TestCase):
    @staticmethod
    def baseline():
        residual = sorted(frontier.BIASED_MOVEMENT_RESIDUAL_OFFSETS)
        return {
            'diagnostic_only': True, 'exactness_credit': 'none',
            'target_bytes': 516, 'candidate_bytes': 517,
            'target_sha256': frontier.BIASED_MOVEMENT_TARGET_SHA,
            'target_instructions': 145, 'candidate_instructions': 145,
            'bound_fields': 28, 'ordered_calls': 14,
            'target_direct_blocks': 24, 'candidate_direct_blocks': 24,
            'complete_direct_graphs_agree': True,
            'overlapping_differences': len(residual),
            'difference_offsets': [f'0x{x:03X}' for x in residual],
            'extra_candidate_bytes': 1, 'complete_bytes_equal': False,
            'normalized_instruction_changes': [
                {'change': 'replace',
                 'target': [[0x134, '8d4804'], [0x137, 'd819']],
                 'candidate': [[0x134, '8bc8'], [0x136, 'd85904']]},
                {'change': 'replace',
                 'target': [[0x164, 'd819']],
                 'candidate': [[0x164, 'd85904']]},
            ],
        }

    def test_independently_reviewed_full_residual_set(self):
        runs = frontier.BIASED_MOVEMENT_RESIDUAL_RUNS
        self.assertEqual(len(runs), 14)
        self.assertTrue(all(0 <= a < b <= 516 for a, b in runs))
        self.assertTrue(all(x[1] < y[0] for x, y in zip(runs, runs[1:])))
        self.assertEqual(len(frontier.BIASED_MOVEMENT_RESIDUAL_OFFSETS), 148)
        self.assertEqual(516 - 148, 368)
        self.assertEqual(frontier.BIASED_MOVEMENT_RESIDUAL_OFFSETS,
                         {i for low, high in runs for i in range(low, high)})
        result = frontier.guard_biased_movement(self.baseline())
        self.assertEqual(result['overlapping_differences'], 148)
        self.assertEqual(result['extra_candidate_bytes'], 1)

    def test_equal_count_swaps_damage_matching_original_bytes_fail_closed(self):
        initial = self.baseline()
        for damaged in (0, 0x133, 0x139, 0x164, 0x194, 0x1A6,
                        0x1B7, 0x1C2, 0x1E8, 0x1F1, 0x1FD, 0x204, 0x203+1):
            self.assertNotIn(damaged, frontier.BIASED_MOVEMENT_RESIDUAL_OFFSETS)
            with self.subTest(damaged=hex(damaged)):
                bad = copy.deepcopy(initial)
                residual = sorted(
                    (frontier.BIASED_MOVEMENT_RESIDUAL_OFFSETS - {0x134}) |
                    {damaged})
                bad['difference_offsets'] = [f'0x{x:03X}' for x in residual]
                self.assertEqual(len(residual), 148)
                with self.assertRaisesRegex(RuntimeError,
                                            'already matched original byte'):
                    frontier.guard_biased_movement(bad)

    def test_zero_and_narrowing_are_diagnostic_not_canonical_exact(self):
        initial = self.baseline()
        narrowed = {**initial,
                    'overlapping_differences': 147,
                    'difference_offsets': initial['difference_offsets'][1:]}
        frontier.guard_biased_movement(narrowed)
        full = {**initial, 'candidate_bytes': 516,
                'overlapping_differences': 0, 'difference_offsets': [],
                'extra_candidate_bytes': 0, 'complete_bytes_equal': True,
                'normalized_instruction_changes': []}
        frontier.guard_biased_movement(full)
        self.assertEqual(full['exactness_credit'], 'none')
        with self.assertRaisesRegex(RuntimeError, 'did not produce'):
            frontier.guard_biased_movement({**full, 'exactness_credit': 'accepted'})

    def test_wrong_extent_calls_fields_graph_sha_or_false_equality_rejected(self):
        initial = self.baseline()
        for changed in ({'target_bytes': 517}, {'candidate_bytes': 515},
                        {'candidate_bytes': 518}, {'bound_fields': 27},
                        {'target_instructions': 144},
                        {'candidate_instructions': 146},
                        {'ordered_calls': 13},
                        {'target_direct_blocks': 23},
                        {'candidate_direct_blocks': 23},
                        {'complete_direct_graphs_agree': False},
                        {'target_sha256': '0' * 64},
                        {'overlapping_differences': 149},
                        {'overlapping_differences': 147},
                        {'overlapping_differences': True},
                        {'extra_candidate_bytes': 0},
                        {'complete_bytes_equal': True},
                        {'diagnostic_only': False}):
            with self.subTest(changed=changed), self.assertRaises(RuntimeError):
                frontier.guard_biased_movement({**initial, **changed})

    def test_malformed_duplicate_or_unsorted_differences_fail_closed(self):
        initial = self.baseline()
        edits = (None,
                 list(reversed(initial['difference_offsets'])),
                 initial['difference_offsets'][:-1] + [initial['difference_offsets'][-2]],
                 initial['difference_offsets'][:-1] + ['bad'],
                 initial['difference_offsets'][:-1] + [0x203])
        for changed in edits:
            with self.subTest(value=str(changed)[:45]), self.assertRaises(RuntimeError):
                frontier.guard_biased_movement({**initial, 'difference_offsets': changed})

    def test_normalized_instruction_window_cannot_hide_other_changes(self):
        initial = self.baseline()
        bad_variants = (
            {'normalized_instruction_changes': None},
            {'normalized_instruction_changes': [{}]},
            {'normalized_instruction_changes': [
                *initial['normalized_instruction_changes'],
                initial['normalized_instruction_changes'][0]]},
            {'normalized_instruction_changes': [
                {**initial['normalized_instruction_changes'][0],
                 'candidate': [[0x133, '8bc8']]},
                initial['normalized_instruction_changes'][1]]},
            {'normalized_instruction_changes': [
                {**initial['normalized_instruction_changes'][0],
                 'target': [[0x165, '8bc8']]},
                initial['normalized_instruction_changes'][1]]},
            {'normalized_instruction_changes': [
                {**initial['normalized_instruction_changes'][0],
                 'change': 'insert'},
                initial['normalized_instruction_changes'][1]]},
        )
        for changed in bad_variants:
            with self.subTest(value=str(changed)[:70]), self.assertRaises(RuntimeError):
                frontier.guard_biased_movement({**initial, **changed})


if __name__ == '__main__':
    unittest.main()
