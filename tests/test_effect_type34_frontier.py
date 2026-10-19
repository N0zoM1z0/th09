"""Fail-closed tests for the original-bound Effect 34 draw frontier."""
from copy import deepcopy
import importlib.util
from pathlib import Path
import sys
from unittest import TestCase
from unittest.mock import patch

SOURCE = Path(__file__).resolve().parents[1] / 'scripts/check-boss-frontiers.py'
spec = importlib.util.spec_from_file_location('effect34_boss_tests', SOURCE)
boss = importlib.util.module_from_spec(spec)
spec.loader.exec_module(boss)

BASE = {'role': 'draw',
 'diagnostic_only': True,
 'exactness_credit': 'none; complete non-crediting original/COFF diagnostic',
 'target_sha256': 'd32652c2e08ffbd02a103e379724cde773716f964bbb962e9e4cbd12f58c88a2',
 'target_bytes': 946,
 'candidate_bytes': 959,
 'target_instructions': 286,
 'candidate_instructions': 291,
 'fields': 53,
 'ordered_calls_agree': True,
 'field_referent_order_agrees': True,
 'COM_immediate_arguments_agree': True,
 'target_blocks': 16,
 'candidate_blocks': 15,
 'missing_bytes': 0,
 'extra_bytes': 13,
 'overlapping_differences': 593,
 'target_calls': [['direct', 4370128],
                  ['direct', 4415232],
                  ['direct', 4200064],
                  ['direct', 4200112],
                  ['direct', 4462736],
                  ['direct', 4216624],
                  ['direct', 4200064],
                  ['direct', 4200112],
                  ['direct', 4437936],
                  ['direct', 4429472],
                  ['indirect-slot', 252],
                  ['indirect-slot', 252],
                  ['indirect-slot', 252],
                  ['indirect-slot', 252],
                  ['indirect-slot', 200],
                  ['indirect-slot', 200],
                  ['direct', 4462736],
                  ['direct', 4216624],
                  ['direct', 4200064],
                  ['direct', 4200112],
                  ['indirect-slot', 304],
                  ['indirect-slot', 288],
                  ['direct', 4252544],
                  ['direct', 4252560],
                  ['direct', 4199920],
                  ['direct', 4283648],
                  ['direct', 4252528],
                  ['direct', 4283664],
                  ['indirect-slot', 252],
                  ['indirect-slot', 252],
                  ['indirect-slot', 252],
                  ['indirect-slot', 252]],
 'target_COM_arguments': [{'slot': 252, 'reverse_stack_arguments': [2, 4, 0, None]},
                          {'slot': 252, 'reverse_stack_arguments': [2, 1, 0, None]},
                          {'slot': 252, 'reverse_stack_arguments': [0, 5, 0, None]},
                          {'slot': 252, 'reverse_stack_arguments': [0, 2, 0, None]},
                          {'slot': 200, 'reverse_stack_arguments': [0, 14, None]},
                          {'slot': 200, 'reverse_stack_arguments': [6, 20, None]},
                          {'slot': 304, 'reverse_stack_arguments': [68, None]},
                          {'slot': 288, 'reverse_stack_arguments': [20, None, 31, 3, None]},
                          {'slot': 252, 'reverse_stack_arguments': [4, 4, 0, None]},
                          {'slot': 252, 'reverse_stack_arguments': [4, 1, 0, None]},
                          {'slot': 252, 'reverse_stack_arguments': [2, 5, 0, None]},
                          {'slot': 252, 'reverse_stack_arguments': [2, 2, 0, None]}]}


def baseline():
    result = deepcopy(BASE)
    result['candidate_calls'] = deepcopy(result['target_calls'])
    result['candidate_COM_arguments'] = deepcopy(result['target_COM_arguments'])
    result['difference_offsets'] = [i for a, b in boss.EFFECT34_DRAW_RESIDUAL_RUNS
                                    for i in range(a, b)]
    return result


class Effect34FrontierTests(TestCase):
    def test_changed_sdk_argument_is_rejected_with_unchanged_counts(self):
        result = baseline()
        boss.guard_effect_type34_draw(result)
        result['candidate_COM_arguments'][0]['reverse_stack_arguments'][0] = 3
        with self.assertRaisesRegex(RuntimeError, 'call trace'):
            boss.guard_effect_type34_draw(result)

    def test_same_count_byte_exchange_cannot_damage_an_old_match(self):
        result = baseline()
        old = set(result['difference_offsets'])
        result['difference_offsets'][-1] = next(i for i in range(946) if i not in old)
        with self.assertRaisesRegex(RuntimeError, 'already equal'):
            boss.guard_effect_type34_draw(result)

    def test_reordered_native_calls_are_rejected_with_unchanged_counts(self):
        result = baseline()
        result['candidate_calls'][:2] = reversed(result['candidate_calls'][:2])
        with self.assertRaisesRegex(RuntimeError, 'call trace'):
            boss.guard_effect_type34_draw(result)

    def test_unreviewed_prebuilt_carrier_fails_before_build_or_decode(self):
        with patch.object(boss, 'build') as build, patch.object(boss, 'parsed') as parsed:
            with self.assertRaisesRegex(RuntimeError, 'prebuilt carrier'):
                boss.run_effect_type34(protect_exact=True, already_built=('wrong.obj',))
        build.assert_not_called()
        parsed.assert_not_called()
