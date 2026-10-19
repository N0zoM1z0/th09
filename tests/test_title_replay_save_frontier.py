"""Pure-input, non-crediting guards for the TH09 800-byte replay-save boss.

The actual Japanese PE, decoded 41 address fields and VC7.1 compiled bytes
are checked in the cold title boss sweep; no synthetic input earns exactness.
"""

import importlib.util
from pathlib import Path
from unittest import TestCase

ROOT = Path(__file__).resolve().parents[1]
spec = importlib.util.spec_from_file_location(
    'title_replay_save_frontier', ROOT/'scripts/check-boss-frontiers.py')
front = importlib.util.module_from_spec(spec)
spec.loader.exec_module(front)


def current_report():
    return {
        'diagnostic_only': True, 'exactness_credit': 'none',
        'target_bytes': 800, 'candidate_bytes': 800,
        'target_sha256': front.TITLE_REPLAY_SAVE_TARGET_SHA256,
        'target_instructions': 245, 'candidate_instructions': 245,
        'original_fields': 41, 'bound_fields': 41,
        'ordered_calls': 4, 'ordered_calls_agree': True,
        'original_blocks': 28, 'candidate_blocks': 28,
        'direct_graphs_agree': True, 'paired_branch_conflicts': [],
        'unpaired_branch_destinations': 5,
        'aligned_instructions': 184,
        'complete_differences': 96,
        'difference_offsets': [hex(x) for x in sorted(front.TITLE_REPLAY_SAVE_RESIDUAL_OFFSETS)],
        'complete_bytes_equal': False,
    }


class TitleReplaySaveTargetBoundFrontier(TestCase):
    def test_current_complete_graph_and_extent_do_not_grant_exactness(self):
        value = current_report()
        front.guard_title_replay_save(value)
        self.assertEqual(value['exactness_credit'], 'none')
        self.assertFalse(value['complete_bytes_equal'])

    def test_strict_source_snapshot_can_narrow_without_locally_granting_credit(self):
        report = current_report()
        candidate = {**report, 'candidate_bytes': 800,
                     'complete_differences': 7,
                     'difference_offsets': report['difference_offsets'][:7]}
        front.guard_title_replay_save(candidate)
        self.assertEqual(candidate['exactness_credit'], 'none')
        no_local_credit = {**candidate, 'complete_differences': 0,
                           'difference_offsets': [], 'complete_bytes_equal': True}
        front.guard_title_replay_save(no_local_credit)
        self.assertEqual(no_local_credit['exactness_credit'], 'none')

    def test_reject_baseline_graph_and_byte_regressions(self):
        old = current_report()
        cases = (
            {'candidate_bytes': 793, 'candidate_instructions': 241,
             'aligned_instructions': 164, 'direct_graphs_agree': False},
            {'candidate_bytes': 800, 'candidate_instructions': 242},
            {'candidate_bytes': 798},
            {'candidate_bytes': 799},
            {'candidate_instructions': 244},
            {'aligned_instructions': 183},
            {'paired_branch_conflicts': [['0x1','0x2','0x3']]},
            {'unpaired_branch_destinations': 6},
            {'original_blocks': 27}, {'candidate_blocks': 27},
            {'direct_graphs_agree': False},
            {'ordered_calls_agree': False}, {'ordered_calls': 3},
        )
        for change in cases:
            with self.subTest(change=change), self.assertRaises(RuntimeError):
                front.guard_title_replay_save({**old, **change})

    def test_each_of_the_704_equal_original_bytes_is_protected(self):
        old = current_report()
        baseline = set(front.TITLE_REPLAY_SAVE_RESIDUAL_OFFSETS)
        self.assertEqual(len(baseline), 96)
        for position in sorted(set(range(800)) - baseline):
            differences = sorted((baseline - {min(baseline)}) | {position})
            with self.subTest(position=hex(position)), self.assertRaises(RuntimeError):
                front.guard_title_replay_save({
                    **old, 'difference_offsets': [hex(x) for x in differences]})

    def test_duplicate_unordered_and_out_of_owner_residuals_are_rejected(self):
        old = current_report()
        offsets = old['difference_offsets']
        cases = (
            [offsets[0], offsets[0], *offsets[2:]],
            list(reversed(offsets)),
            [*offsets[:-1], '0x320'],
            [*offsets[:-1], '-0x1'],
            [*offsets[:-1], 'unknown'],
        )
        for offsets in cases:
            with self.subTest(offsets=offsets[:2]), self.assertRaises(RuntimeError):
                front.guard_title_replay_save({**old, 'difference_offsets': offsets})

    def test_reject_unreviewed_target_and_forged_acceptance(self):
        old = current_report()
        cases = (
            {'target_sha256': '0'*64}, {'target_bytes': 799},
            {'original_fields': 40}, {'bound_fields': 40},
            {'target_instructions': 244},
            {'diagnostic_only': False},
            {'exactness_credit': 'accepted'},
            {'complete_bytes_equal': True},
            {'complete_differences': []},
            {'difference_offsets': []},
            {'candidate_bytes': 800, 'complete_differences': 5,
             'difference_offsets': ['0x1']},
        )
        for change in cases:
            with self.subTest(change=str(change)[:70]), self.assertRaises(RuntimeError):
                front.guard_title_replay_save({**old, **change})
