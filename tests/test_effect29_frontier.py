"""Reject incomplete or relocated evidence for the maintained Type29 frontier."""
from copy import deepcopy
import importlib.util
from pathlib import Path
from unittest import TestCase

ROOT = Path(__file__).resolve().parents[1]
spec = importlib.util.spec_from_file_location(
    'effect29_frontier_tests', ROOT / 'scripts/check-boss-frontiers.py')
boss = importlib.util.module_from_spec(spec)
spec.loader.exec_module(boss)


def baseline():
    offsets = [i for a, b in boss.EFFECT29_DRAW_RESIDUAL_RUNS for i in range(a, b)]
    calls = [['direct', address] for address in boss.EFFECT29_DRAW_CALLS]
    return {
        'diagnostic_only': True, 'exactness_credit': 'none',
        'maintained_source_present': True, 'object_source_attested': False,
        'original': {
            'target_address': 0x448750, 'target_bytes': 2220,
            'target_sha256': boss.EFFECT29_DRAW_SHA256,
            'table_pointer_address': 0x4A0E14,
            'prefix_sha256': '318a2c5a7e469179735911b94d8837b8b96651b2c89153ea2a15023ec20942b8',
            'reachable_instructions': 672, 'physical_instructions': 675,
            'returns': [0x448FFB], 'following_CC_bytes': 4,
        },
        'comparison': {
            'diagnostic_only': True, 'exactness_credit': 'none',
            'target_bytes': 2220, 'target_address': 0x448750,
            'target_sha256': boss.EFFECT29_DRAW_SHA256,
            'candidate_bytes': 2205, 'missing_bytes': 15, 'extra_bytes': 0,
            'target_instructions': 675, 'candidate_instructions': 670,
            'fields': 129, 'field_referent_order_agrees': True,
            'ordered_calls_agree': True,
            'target_calls': calls, 'candidate_calls': deepcopy(calls),
            'target_blocks': 41, 'candidate_blocks': 41,
            'overlapping_differences': 1170, 'difference_offsets': offsets,
            'complete_bytes_agree': False,
        },
    }


class Type29Frontier(TestCase):
    def test_boundary_and_source_claims_are_independent(self):
        boss.guard_effect_type29_draw(baseline())
        for key, value in [('maintained_source_present', False),
                           ('object_source_attested', True),
                           ('exactness_credit', 'exact')]:
            result = baseline()
            result[key] = value
            with self.subTest(key=key), self.assertRaises(RuntimeError):
                boss.guard_effect_type29_draw(result)
        for key, value in [('target_sha256', 'bad'), ('target_bytes', 2205),
                           ('following_CC_bytes', 0), ('table_pointer_address', 0x4A0E10),
                           ('reachable_instructions', 675), ('returns', [0x448FF7])]:
            result = baseline()
            result['original'][key] = value
            with self.subTest(key=key), self.assertRaises(RuntimeError):
                boss.guard_effect_type29_draw(result)

    def test_same_count_exchange_cannot_damage_an_old_equal_byte(self):
        result = baseline()
        row = result['comparison']
        row['difference_offsets'][0] = 0  # Entry PUSH EBP was already equal.
        row['difference_offsets'].sort()
        with self.assertRaisesRegex(RuntimeError, 'already equal'):
            boss.guard_effect_type29_draw(result)

    def test_ordered_call_identity_cannot_be_replaced_by_equal_counts(self):
        result = baseline()
        result['comparison']['candidate_calls'][0][1] += 4
        with self.assertRaisesRegex(RuntimeError, 'call trace'):
            boss.guard_effect_type29_draw(result)
        result = baseline()
        result['comparison']['field_referent_order_agrees'] = False
        with self.assertRaisesRegex(RuntimeError, 'fields/calls'):
            boss.guard_effect_type29_draw(result)

    def test_missing_tail_is_never_hidden_by_overlap_equality(self):
        for key, value in [('candidate_bytes', 2204), ('candidate_bytes', 2221),
                           ('missing_bytes', 0), ('extra_bytes', 1), ('fields', 128)]:
            result = baseline()
            result['comparison'][key] = value
            with self.subTest(key=key, value=value), self.assertRaises(RuntimeError):
                boss.guard_effect_type29_draw(result)
        result = baseline()
        result['comparison']['complete_bytes_agree'] = True
        with self.assertRaisesRegex(RuntimeError, 'contradictory'):
            boss.guard_effect_type29_draw(result)

    def test_duplicate_unsorted_or_inconsistent_offsets_are_rejected(self):
        for mutate in (
            lambda row: row['difference_offsets'].reverse(),
            lambda row: row['difference_offsets'].append(row['difference_offsets'][-1]),
            lambda row: row['difference_offsets'].__setitem__(0, True),
            lambda row: row.__setitem__('overlapping_differences', 1169),
        ):
            result = baseline()
            mutate(result['comparison'])
            with self.assertRaises(RuntimeError):
                boss.guard_effect_type29_draw(result)

    def test_partial_recovery_does_not_grant_exact_credit(self):
        result = baseline()
        result['comparison']['difference_offsets'].pop()
        result['comparison']['overlapping_differences'] -= 1
        summary = boss.guard_effect_type29_draw(result)
        self.assertEqual(summary['ordinary_differences'], 1169)
        self.assertEqual(summary['missing_bytes'], 15)
        self.assertEqual(summary['exactness_credit'], 'none')

    def test_complete_equality_needs_all_complete_owner_evidence(self):
        result = baseline()
        row = result['comparison']
        row.update(candidate_bytes=2220, missing_bytes=0, difference_offsets=[],
                   overlapping_differences=0, complete_bytes_agree=True)
        with self.assertRaisesRegex(RuntimeError, 'incomplete equality'):
            boss.guard_effect_type29_draw(result)
        row.update(resolved_sha256=boss.EFFECT29_DRAW_SHA256,
                   candidate_instructions=675, all_original_fields_agree=True,
                   direct_graphs_agree=True)
        self.assertEqual(boss.guard_effect_type29_draw(result)['exactness_credit'], 'none')

    def test_malformed_nested_evidence_is_rejected(self):
        for key in ('original', 'comparison'):
            result = baseline()
            result[key] = []
            with self.subTest(key=key), self.assertRaisesRegex(RuntimeError, 'malformed'):
                boss.guard_effect_type29_draw(result)
