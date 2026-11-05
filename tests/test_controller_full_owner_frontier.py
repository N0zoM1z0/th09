"""Fail-closed, target-independent TH09 complete Controller owner boundaries."""
from copy import deepcopy
import importlib.util
from pathlib import Path
from unittest import TestCase
from unittest.mock import patch

ROOT = Path(__file__).resolve().parents[1]
spec = importlib.util.spec_from_file_location(
    'controller_full_owner_frontier', ROOT/'scripts/check-boss-frontiers.py')
front = importlib.util.module_from_spec(spec)
spec.loader.exec_module(front)


def baseline():
    offsets = sorted(front.CONTROLLER_INPUT_RESIDUAL_OFFSETS)
    return dict(diagnostic_only=True, exactness_credit='none',
                target_bytes=808, candidate_bytes=810,
                target_sha256=front.CONTROLLER_INPUT_SHA256,
                target_instructions=246, candidate_instructions=247,
                target_fields=32, bound_candidate_fields=32,
                native_global_fields_checked=9, controller_slot_fields=4,
                prefix_674_equal=True, ordered_direct_calls_agree=True,
                target_direct_branches=8, candidate_direct_branches=8,
                complete_direct_branch_graphs_agree=True,
                overlapping_differences=len(offsets), difference_offsets=offsets,
                missing_bytes=0, excess_bytes=2, complete_exact=False)


class ControllerFullOwnerGate(TestCase):
    def test_real_original_mask_records_all_724_matching_physical_bytes(self):
        self.assertEqual(len(front.CONTROLLER_INPUT_RESIDUAL_MASK), 17)
        self.assertEqual(len(front.CONTROLLER_INPUT_RESIDUAL_OFFSETS), 84)
        self.assertEqual(len(set(range(808)) - front.CONTROLLER_INPUT_RESIDUAL_OFFSETS), 724)
        self.assertTrue(all(i >= 674 for i in front.CONTROLLER_INPUT_RESIDUAL_OFFSETS))
        self.assertEqual(front.guard_controller_input(baseline()), baseline())

    def test_new_byte_damage_at_same_count_must_fail_closed(self):
        row = baseline()
        original = row['difference_offsets']
        for new in (0, 1, 673, 674, 690, 700, 807):
            if new in original:
                continue
            damaged = deepcopy(row)
            damaged['difference_offsets'] = sorted([*original[:-1], new])
            self.assertEqual(len(damaged['difference_offsets']), len(original))
            with self.subTest(new=new), self.assertRaisesRegex(RuntimeError, 'frontier regressed'):
                front.guard_controller_input(damaged)

    def test_reloc_abi_graph_and_original_digests_must_remain_pinned(self):
        row = baseline()
        for change in ({'target_sha256':'0'*64}, {'candidate_bytes': 809},
                       {'candidate_bytes': 808}, {'target_instructions': 247},
                       {'candidate_instructions': 248}, {'target_fields': 31},
                       {'bound_candidate_fields': 31}, {'controller_slot_fields': 3},
                       {'native_global_fields_checked': 8},
                       {'ordered_direct_calls_agree': False},
                       {'target_direct_branches': 7}, {'candidate_direct_branches': 9},
                       {'complete_direct_branch_graphs_agree': False},
                       {'prefix_674_equal': False}, {'missing_bytes': 1},
                       {'excess_bytes': 0}, {'complete_exact': True},
                       {'difference_offsets': sorted(row['difference_offsets'][:-1] +
                                                      [row['difference_offsets'][-2]])},
                       {'exactness_credit': 'accepted'}):
            with self.subTest(change=change), self.assertRaises(RuntimeError):
                front.guard_controller_input({**row, **change})

    def test_future_real_narrowing_does_not_grant_partial_credit(self):
        row = baseline()
        row['difference_offsets'] = row['difference_offsets'][:-1]
        row['overlapping_differences'] -= 1
        front.guard_controller_input(row)
        self.assertEqual(row['exactness_credit'], 'none')
        row = dict(row, candidate_bytes=808, candidate_instructions=246,
                   excess_bytes=0, difference_offsets=[], overlapping_differences=0,
                   complete_exact=True)
        front.guard_controller_input(row)
        self.assertEqual(row['exactness_credit'], 'none')

    def test_scoped_builder_replays_only_actual_two_exact_siblings(self):
        with patch.object(front,'build') as build, patch.object(
                front,'parsed',return_value=baseline()) as parsed, patch.object(
                front,'canonical',side_effect=lambda x: {'unit':x}) as exact:
            owners, witnesses=front.run_controller_input(protect_exact=True)
        build.assert_called_once_with('controller-set-button-directinput')
        parsed.assert_called_once_with(['scripts/inspect-controller-input.py',
                                       'build/matching/ControllerInput.obj','--json'],
                                      diagnostic=True)
        self.assertEqual([w['unit'] for w in witnesses],
                         ['controller-set-button-directinput',
                          'controller-set-button-joystick',
                          'controller-get-controller-input'])
        self.assertEqual(exact.call_count,3)
        self.assertEqual(owners['controller_input']['unlike_bytes'],84)
        self.assertEqual(owners['controller_input']['matched_original_bytes'],724)
        self.assertEqual(owners['controller_input']['exactness_credit'],'none')
        with patch.object(front,'build') as build, patch.object(
                front,'parsed',return_value=baseline()) as parsed, patch.object(
                front,'canonical') as exact:
            owners, witnesses=front.run_controller_input(protect_exact=False)
        self.assertEqual(witnesses,[])
        exact.assert_not_called()
        build.assert_called_once()
