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
        'target_bytes': 800, 'candidate_bytes': 799,
        'target_sha256': front.TITLE_REPLAY_SAVE_TARGET_SHA256,
        'target_instructions': 245, 'candidate_instructions': 245,
        'original_fields': 41, 'bound_fields': 41,
        'ordered_calls': 4, 'ordered_calls_agree': True,
        'original_blocks': 28, 'candidate_blocks': 28,
        'direct_graphs_agree': True, 'paired_branch_conflicts': [],
        'unpaired_branch_destinations': 5,
        'aligned_instructions': 173,
        'complete_differences': None, 'difference_offsets': None,
        'complete_bytes_equal': False,
    }


class TitleReplaySaveTargetBoundFrontier(TestCase):
    def test_current_799_byte_graph_recovered_but_not_exact(self):
        value = current_report()
        front.guard_title_replay_save(value)
        self.assertEqual(value['exactness_credit'], 'none')
        source = (ROOT/'src/TitleScreenReplaySaveDraw.cpp').read_text()
        self.assertIn('int valid = replay->magic != 0;', source)
        self.assertIn('const char *playTime = valid ? replay->playTime : "--/--/--";', source)
        self.assertIn('const char *playerName = valid ? replay->playerName : "--------";', source)
        self.assertIn('float factor = (float)frame;', source)
        self.assertIn('target.y = scaled.y * factor;', source)
        self.assertIn('target.z = scaled.z * factor;', source)

    def test_strict_source_snapshot_can_narrow_without_locally_granting_credit(self):
        report = current_report()
        candidate = {**report, 'candidate_bytes': 800,
                     'complete_differences': 7,
                     'difference_offsets': [hex(i) for i in range(7)]}
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
            {'candidate_instructions': 244},
            {'aligned_instructions': 172},
            {'paired_branch_conflicts': [['0x1','0x2','0x3']]},
            {'unpaired_branch_destinations': 6},
            {'original_blocks': 27}, {'candidate_blocks': 27},
            {'direct_graphs_agree': False},
            {'ordered_calls_agree': False}, {'ordered_calls': 3},
        )
        for change in cases:
            with self.subTest(change=change), self.assertRaises(RuntimeError):
                front.guard_title_replay_save({**old, **change})

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
