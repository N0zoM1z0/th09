"""Fail-closed physical-byte residual checks for TH09 Effect's real ANM leaf.

Pure-input validation of the original-bound guard; this never grants local
exactness. The actual PE/COFF binding runs in inspect-effect-added.py.
"""

import importlib.util
from pathlib import Path
import unittest

ROOT = Path(__file__).resolve().parents[1]
spec = importlib.util.spec_from_file_location(
    'effect_added_boss_gate', ROOT / 'scripts/check-boss-frontiers.py')
gate = importlib.util.module_from_spec(spec)
spec.loader.exec_module(gate)


class EffectAddedOriginalResidualGate(unittest.TestCase):
    @staticmethod
    def baseline():
        return {
            'diagnostic_only': True, 'exactness_credit': 'none',
            'target_sha256': gate.EFFECT_ADDED_ORIGINAL_SHA,
            'target_bytes': 51, 'candidate_bytes': 51,
            'target_instructions': 19, 'candidate_instructions': 19,
            'target_relocations': 4, 'bound_relocations': 4,
            'original_blocks': 3, 'candidate_blocks': 3,
            'complete_direct_graphs_agree': True,
            'ordered_calls_match': True,
            'differing_bytes': 8,
            'difference_offsets': [f'0x{i:x}' for i in range(3, 10)] + ['0xd'],
            'complete_bytes_equal': False,
        }

    def test_full_51_byte_owner_all_current_residuals_only(self):
        observed = self.baseline()
        self.assertEqual(len(gate.EFFECT_ADDED_ORIGINAL_SHA), 64)
        self.assertEqual(gate.EFFECT_ADDED_RESIDUAL_WINDOWS,
                         ((3, 10, 7), (13, 14, 1)))
        self.assertEqual(gate.guard_effect_added(observed)['differences'], 8)

    def test_genuine_byte_narrowing_and_zero_remain_diagnostic_only(self):
        observed = self.baseline()
        narrower = {**observed, 'differing_bytes': 7,
                    'difference_offsets': observed['difference_offsets'][:-1]}
        self.assertEqual(gate.guard_effect_added(narrower)['differences'], 7)
        zero = {**observed, 'differing_bytes': 0,
                'difference_offsets': [], 'complete_bytes_equal': True}
        self.assertEqual(gate.guard_effect_added(zero)['differences'], 0)
        self.assertEqual(zero['exactness_credit'], 'none')

    def test_old_global_load_and_unrelated_function_byte_cannot_regress(self):
        source = self.baseline()
        for offset in (0, 1, 2, 10, 11, 12, 14, 32, 50):
            with self.subTest(offset=offset), self.assertRaises(RuntimeError):
                wrong = sorted(source['difference_offsets'][:-1] + [hex(offset)],
                               key=lambda value: int(value, 16))
                gate.guard_effect_added({**source, 'difference_offsets': wrong})

    def test_malformed_duplicate_or_fabricated_acceptance_is_rejected(self):
        source = self.baseline()
        cases = (
            {'differing_bytes': 9},
            {'differing_bytes': -1},
            {'differing_bytes': '8'},
            {'difference_offsets': source['difference_offsets'][:-1]},
            {'difference_offsets': list(reversed(source['difference_offsets']))},
            {'difference_offsets': ['0x3'] * 8},
            {'difference_offsets': [3] + source['difference_offsets'][1:]},
            {'difference_offsets': source['difference_offsets'][:-1] + ['bad']},
            {'complete_bytes_equal': True},
            {'exactness_credit': 'accepted'},
            {'diagnostic_only': False},
        )
        for change in cases:
            with self.subTest(change=str(change)[:110]), self.assertRaises(RuntimeError):
                gate.guard_effect_added({**source, **change})

    def test_field_call_target_and_graph_contract_fail_closed(self):
        source = self.baseline()
        cases = (
            {'target_sha256': '0' * 64},
            {'target_bytes': 50}, {'candidate_bytes': 50},
            {'target_instructions': 18}, {'candidate_instructions': 20},
            {'target_relocations': 3}, {'bound_relocations': 3},
            {'original_blocks': 2}, {'candidate_blocks': 4},
            {'complete_direct_graphs_agree': False},
            {'ordered_calls_match': False},
        )
        for change in cases:
            with self.subTest(change=change), self.assertRaises(RuntimeError):
                gate.guard_effect_added({**source, **change})


if __name__ == '__main__':
    unittest.main()
