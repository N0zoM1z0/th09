"""Fail-closed tests for the exact original-bound Effect 34 draw owner."""
from copy import deepcopy
import csv
import tomllib
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
 'candidate_bytes': 946,
 'target_instructions': 286,
 'candidate_instructions': 286,
 'fields': 53,
 'candidate_address_field_count': 53,
 'candidate_fields_cover_instructions': True,
 'literal_definitions_checked': 4,
 'ordered_calls_agree': True,
 'field_referent_order_agrees': True,
 'COM_immediate_arguments_agree': True,
 'target_blocks': 16,
 'candidate_blocks': 16,
 'missing_bytes': 0,
 'extra_bytes': 0,
 'overlapping_differences': 0,
 'ordinal_graphs_agree': True,
 'complete_bytes_agree': True,
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
    result['difference_offsets'] = []
    result['resolved_sha256'] = boss.EFFECT34_DRAW_SHA256
    result['all_original_fields_agree'] = True
    result['direct_graphs_agree'] = True
    return result


class Effect34FrontierTests(TestCase):
    def test_promoted_canonical_unit_preserves_real_53_field_owner(self):
        """No private PE or toolchain: pin the reviewed manifest/ledger contract."""
        root = SOURCE.parent.parent
        with (root / 'config/match-units.toml').open('rb') as handle:
            units = tomllib.load(handle)['units']
        draw = units['effect-draw-callback-type34']
        ecl = units['ecl-pop-context']
        self.assertEqual(draw['source'], 'src/EclManager.cpp')
        self.assertEqual(draw['object'], ecl['object'])
        self.assertEqual(draw['profile'], ecl['profile'])
        self.assertEqual(draw['functions'], ['EffectDrawCallbackType34'])
        self.assertEqual(draw['symbol'], '?EffectDrawCallbackType34@@YIHPAUEffect@@@Z')
        self.assertEqual((draw['target_address'], draw['size']), (0x444AF0, 946))
        rows = draw['relocations']
        self.assertEqual(len(rows), 53)
        offsets = [r['offset'] for r in rows]
        self.assertEqual(offsets, sorted(offsets))
        self.assertEqual(len(offsets), len(set(offsets)))
        for i, row in enumerate(rows):
            with self.subTest(i=i, offset=hex(row['offset'])):
                self.assertIn(row['type'], ('DIR32', 'REL32'))
                self.assertTrue(0 <= row['offset'] <= draw['size'] - 4)
                self.assertTrue(row['symbol'])
                self.assertTrue(0x400000 <= row['target'] < 0x500000)
                if i + 1 < len(rows):
                    self.assertLessEqual(row['offset'] + 4, rows[i + 1]['offset'])
        # Retain distinct genuine imported referents, not a count of 53 fake fields.
        self.assertEqual((rows[0]['offset'], rows[0]['symbol'], rows[0]['target']),
                         (0x26, '?AddNormalizeAngle@@YIMMM@Z', 0x42AED0))
        self.assertEqual((rows[-1]['offset'], rows[-1]['symbol'],
                          rows[-1]['target'], rows[-1]['addend']),
                         (0x38C, '?g_Supervisor@@3VSupervisor@@A', 0x4B3100, 8))
        floats = [r for r in rows if r['symbol'].startswith('__real@')]
        self.assertEqual(len(floats), 7)
        self.assertEqual(len({r['symbol'] for r in floats}), 4)

        with (root / 'config/matches.csv').open(newline='') as handle:
            matching = [r for r in csv.DictReader(handle)
                        if r['address'].lower() == '0x00444af0']
        self.assertEqual(len(matching), 1)
        match = matching[0]
        self.assertEqual((match['unit'], match['status'], match['size'],
                          match['match_percent']),
                         ('effect-draw-callback-type34', 'matching', '946', '100.00'))
        with (root / 'config/functions.csv').open(newline='') as handle:
            annotated = [r for r in csv.DictReader(handle)
                         if r['address'].lower() == '0x00444af0']
        self.assertEqual(len(annotated), 1)
        self.assertEqual((annotated[0]['status'], annotated[0]['match_percent'],
                          annotated[0]['owner'], annotated[0]['source_file']),
                         ('matching', '100.00', 'authored', 'src/EclManager.cpp'))

    def test_changed_sdk_argument_is_rejected_with_unchanged_counts(self):
        result = baseline()
        boss.guard_effect_type34_draw(result)
        result['candidate_COM_arguments'][0]['reverse_stack_arguments'][0] = 3
        with self.assertRaisesRegex(RuntimeError, 'call trace'):
            boss.guard_effect_type34_draw(result)

    def test_any_new_byte_difference_is_rejected(self):
        for offset in (0x00A, 0x01F9, 0x0200, 0x0247, 0x03B1):
            with self.subTest(offset=hex(offset)):
                result = baseline()
                result.update(difference_offsets=[offset], overlapping_differences=1,
                              complete_bytes_agree=False)
                with self.assertRaisesRegex(RuntimeError, 'byte equality'):
                    boss.guard_effect_type34_draw(result)

    def test_old_extent_or_changed_block_graph_cannot_inherit_new_frontier(self):
        for change in ({'candidate_bytes': 959}, {'candidate_instructions': 291},
                       {'candidate_blocks': 15}, {'extra_bytes': 13},
                       {'ordinal_graphs_agree': False}, {'ordinal_graphs_agree': None}):
            with self.subTest(change=change), self.assertRaises(RuntimeError):
                boss.guard_effect_type34_draw({**baseline(), **change})

    def test_exact_result_requires_full_bytes_fields_and_graph(self):
        result = baseline()
        boss.guard_effect_type34_draw(result)
        for change in ({'resolved_sha256': '0'*64},
                       {'all_original_fields_agree': False},
                       {'direct_graphs_agree': False},
                       {'complete_bytes_agree': False},
                       {'overlapping_differences': 1},
                       {'difference_offsets': [0x1f9]}):
            with self.subTest(change=change), self.assertRaisesRegex(RuntimeError, 'byte equality'):
                boss.guard_effect_type34_draw({**result, **change})

    def test_reordered_native_calls_are_rejected_with_unchanged_counts(self):
        result = baseline()
        result['candidate_calls'][:2] = reversed(result['candidate_calls'][:2])
        with self.assertRaisesRegex(RuntimeError, 'call trace'):
            boss.guard_effect_type34_draw(result)

    def test_unchecked_literal_or_operand_definitions_fail_closed(self):
        for change in ({'literal_definitions_checked': 3},
                       {'candidate_address_field_count': 52},
                       {'candidate_fields_cover_instructions': False},
                       {'literal_definitions_checked': None}):
            with self.subTest(change=change), self.assertRaises(RuntimeError):
                boss.guard_effect_type34_draw({**baseline(), **change})

    def test_unreviewed_prebuilt_carrier_fails_before_build_or_decode(self):
        with patch.object(boss, 'build') as build, patch.object(boss, 'parsed') as parsed:
            with self.assertRaisesRegex(RuntimeError, 'prebuilt carrier'):
                boss.run_effect_type34(protect_exact=True, already_built=('wrong.obj',))
        build.assert_not_called()
        parsed.assert_not_called()
