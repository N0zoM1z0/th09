"""No-target input guards for the complete TH09 1,689-byte gameplay worker.

These pure-input tests do not claim original-byte acceptance. The required
actual Japanese PE/COFF check runs separately in check-boss-frontiers.py.
"""

import importlib.util
from pathlib import Path
from unittest import TestCase

ROOT = Path(__file__).resolve().parents[1]
spec = importlib.util.spec_from_file_location(
    'gameplay_setup_frontier', ROOT / 'scripts/check-boss-frontiers.py')
front = importlib.util.module_from_spec(spec)
spec.loader.exec_module(front)


def baseline():
    differences = [i for i in range(1689)
                   if front.GAMEPLAY_SETUP_RESIDUAL_MASK[i // 8] & (1 << (i % 8))]
    return {
        'diagnostic_only': True, 'exactness_credit': 'none',
        'target_sha256': front.GAMEPLAY_SETUP_TARGET_SHA,
        'target_bytes': 1689, 'candidate_bytes': 1689,
        'target_instructions': 413, 'candidate_instructions': 416,
        'original_fields': 172, 'bound_fields': 171,
        'ordered_calls': 35, 'candidate_calls': 35,
        'ordered_calls_agree': True,
        'original_blocks': 78, 'candidate_blocks': 77,
        'direct_graphs_agree': False,
        'aligned_field_identities': 157, 'field_change_spans': 15,
        'aligned_instructions': 394,
        'overlap_differences': len(differences),
        'difference_offsets': differences, 'complete_bytes_equal': False,
    }


class GameplaySetupOriginalBoundFrontier(TestCase):
    def test_complete_candidate_mask_is_911_different_bytes(self):
        report = baseline()
        self.assertEqual(len(front.GAMEPLAY_SETUP_RESIDUAL_MASK), 212)
        self.assertEqual(report['overlap_differences'], 911)
        self.assertNotIn(0x62B, report['difference_offsets'])
        self.assertNotIn(0x635, report['difference_offsets'])
        self.assertIn(0x62C, report['difference_offsets'])
        front.guard_gameplay_setup_thread(report)

    def test_genuinely_narrower_full_extent_still_receives_no_local_exact_credit(self):
        report = baseline()
        narrower = {**report,
                    'difference_offsets': report['difference_offsets'][:-10],
                    'overlap_differences': report['overlap_differences'] - 10}
        front.guard_gameplay_setup_thread(narrower)
        self.assertEqual(narrower['exactness_credit'], 'none')
        apparent_zero = {**report, 'difference_offsets': [],
                         'overlap_differences': 0, 'complete_bytes_equal': True}
        front.guard_gameplay_setup_thread(apparent_zero)
        self.assertEqual(apparent_zero['exactness_credit'], 'none')

    def test_reject_any_newly_different_byte_anywhere_in_original_owner(self):
        report = baseline()
        allowed = report['difference_offsets']
        outside = [i for i in range(1689) if i not in set(allowed)]
        for off in (0, 0x62B, 0x635, outside[-1]):
            with self.subTest(offset=hex(off)), self.assertRaises(RuntimeError):
                candidate = sorted(allowed[:-1] + [off])
                front.guard_gameplay_setup_thread({**report,
                    'difference_offsets': candidate})

    def test_reject_malformed_byte_inventory_and_forged_acceptance(self):
        report = baseline()
        differences = report['difference_offsets']
        invalid = (
            {'difference_offsets': None},
            {'difference_offsets': differences[:-1]},
            {'difference_offsets': list(reversed(differences))},
            {'difference_offsets': [differences[0]] * len(differences)},
            {'difference_offsets': ['0'] + differences[1:]},
            {'difference_offsets': differences[:-1] + [1689]},
            {'difference_offsets': differences[:-1] + [-1]},
            {'difference_offsets': differences[:-1] + [True]},
            {'overlap_differences': 912},
            {'overlap_differences': '911'},
            {'complete_bytes_equal': True},
            {'exactness_credit': 'accepted'},
            {'diagnostic_only': False},
        )
        for change in invalid:
            with self.subTest(change=str(change)[:60]), self.assertRaises(RuntimeError):
                front.guard_gameplay_setup_thread({**report, **change})

    def test_reject_original_target_and_field_call_graph_regressions(self):
        report = baseline()
        invalid = (
            {'target_sha256': '0' * 64},
            {'target_bytes': 1688}, {'candidate_bytes': 1690},
            {'target_instructions': 412}, {'candidate_instructions': 415},
            {'original_fields': 171}, {'bound_fields': 170},
            {'ordered_calls': 34}, {'candidate_calls': 34},
            {'ordered_calls_agree': False},
            {'original_blocks': 77}, {'candidate_blocks': 78},
            {'direct_graphs_agree': True},
            {'aligned_field_identities': 156},
            {'field_change_spans': 16},
            {'aligned_instructions': 393},
        )
        for change in invalid:
            with self.subTest(change=change), self.assertRaises(RuntimeError):
                front.guard_gameplay_setup_thread({**report, **change})
