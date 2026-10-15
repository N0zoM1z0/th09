"""Target-independent tests for the TH09 PauseMenu cold owner frontier.

Fixtures are non-crediting shapes, never accepted oracle receipts. An actual
source/profile/PE-bound cold build is required by --owner pause.
"""

from collections import Counter
import importlib.util
from pathlib import Path
import unittest
from unittest.mock import patch

ROOT = Path(__file__).resolve().parents[1]
SPEC = importlib.util.spec_from_file_location(
    'pause_boss_guard', ROOT / 'scripts/check-boss-frontiers.py')
frontier = importlib.util.module_from_spec(SPEC)
SPEC.loader.exec_module(frontier)


class PauseMenuFrontierTests(unittest.TestCase):
    @staticmethod
    def baseline():
        calls = [name for name, count in frontier.PAUSE_CALL_CENSUS.items()
                 for _ in range(count)]
        assert len(calls) == 46
        source_table = [
            '0x434813', '0x434883', '0x4349e0', '0x434b08',
            '0x434b4b', '0x434c41', '0x434b4b', '0x434c41',
            '0x434d51', '0x434d69',
        ]
        return {
            'diagnostic_only': True, 'exactness_credit': 'none',
            'target_owner_sha256': frontier.PAUSE_SHA256,
            'physical_bytes': 1732, 'overlap_differences': 1453,
            'missing_bytes': 44, 'excess_bytes': 0, 'complete_equal': False,
            'fields': [{} for _ in range(107)],
            'differences': [
                {'offset': i, 'candidate': 1, 'target': 2}
                for i in range(1453)
            ],
            'target': {
                'code_bytes': 1734, 'alignment_hex': '8bff',
                'table_offset': 1736, 'instructions': 449,
                'fields': [(0, 'DIR32')] * 60 + [(0, 'REL32')] * 46,
                'table_destinations': list(frontier.PAUSE_TARGET_TABLE),
                'ordered_direct_calls': calls,
                'internal_branches': [None] * 52,
            },
            'candidate': {
                'code_bytes': 1692, 'alignment_hex': '',
                'table_offset': 1692, 'instructions': 454,
                'fields': [(0, 'DIR32')] * 107,
                'table_destinations': source_table,
                'ordered_direct_calls': list(reversed(calls)),
                'internal_branches': [None] * 54,
            },
        }

    def test_current_candidate_is_noncrediting(self):
        e = self.baseline()
        r = frontier.guard_pause_menu(e)
        self.assertEqual((r['target_bytes'], r['candidate_bytes']), (1776, 1732))
        self.assertEqual((r['overlap_differences'], r['missing_target_bytes']),
                         (1453, 44))
        self.assertEqual((r['calls'], r['switch_entries']), (46, 10))
        self.assertEqual(e['exactness_credit'], 'none')

    def test_one_cold_compile_and_all_exact_siblings(self):
        e = self.baseline()
        with patch.object(frontier, 'build_nonexact') as compile_owner, patch.object(
                frontier, 'parsed', return_value=e) as inspect, patch.object(
                frontier, 'canonical', side_effect=lambda name: {'unit': name, 'result': 'exact'}) as canonical:
            owners, exact = frontier.run_pause(protect_exact=True)
        compile_owner.assert_called_once_with(
            'src/AsciiManagerMenu.cpp', 'build/matching/AsciiManagerMenu.obj',
            reference_unit='ascii-manager-on-update')
        inspect.assert_called_once_with([
            'scripts/inspect-pause-menu.py', '--json',
            'build/matching/AsciiManagerMenu.obj'])
        self.assertEqual([x['unit'] for x in exact], list(frontier.PAUSE_EXACT_SIBLINGS))
        self.assertEqual(canonical.call_count, 11)
        self.assertEqual(owners['pause_menu_update']['overlap_differences'], 1453)

    def test_forged_original_physical_body_and_source_boundaries_rejected(self):
        e = self.baseline()
        invalid = [
            {'diagnostic_only': False}, {'exactness_credit': 'accepted'},
            {'target_owner_sha256': '0' * 64}, {'physical_bytes': 1731},
            {'physical_bytes': 1777}, {'missing_bytes': 43},
            {'excess_bytes': 1}, {'overlap_differences': 1454},
            {'overlap_differences': 1452}, {'complete_equal': True},
            {'target': {**e['target'], 'code_bytes': 1733}},
            {'target': {**e['target'], 'alignment_hex': '9090'}},
            {'target': {**e['target'], 'table_offset': 1734}},
            {'target': {**e['target'], 'instructions': 448}},
            {'target': {**e['target'], 'fields': e['target']['fields'][:-1]}},
            {'target': {**e['target'], 'table_destinations':
                        e['target']['table_destinations'][:-1] + ['0x434d98']}},
            {'target': {**e['target'], 'ordered_direct_calls':
                        e['target']['ordered_direct_calls'][:-1]}},
            {'target': {**e['target'], 'internal_branches': [None] * 51}},
            {'candidate': {**e['candidate'], 'code_bytes': 1691}},
            {'candidate': {**e['candidate'], 'table_offset': 1691}},
            {'candidate': {**e['candidate'], 'alignment_hex': 'ff'}},
            {'candidate': {**e['candidate'], 'instructions': 455}},
            {'candidate': {**e['candidate'], 'fields': e['candidate']['fields'][:-2]}},
            {'candidate': {**e['candidate'], 'table_destinations':
                           e['candidate']['table_destinations'][:-1]}},
            {'candidate': {**e['candidate'], 'table_destinations':
                           e['candidate']['table_destinations'][:-1] +
                           [e['candidate']['table_destinations'][0]]}},
            {'candidate': {**e['candidate'], 'ordered_direct_calls':
                           e['candidate']['ordered_direct_calls'][:-1]}},
            {'candidate': {**e['candidate'], 'internal_branches': [None] * 55}},
            {'fields': e['fields'][:-2]},
            {'differences': e['differences'][:-1]},
            {'differences': e['differences'][:-1] +
                            [{'offset': 1451, 'candidate': 1, 'target': 2}]},
            {'differences': e['differences'][:-1] +
                            [{'offset': 1732, 'candidate': 1, 'target': 2}]},
            {'differences': e['differences'][:-1] +
                            [{'offset': 1452, 'candidate': 2, 'target': 2}]},
        ]
        for change in invalid:
            with self.subTest(change=str(change)[:110]):
                with self.assertRaises(RuntimeError):
                    frontier.guard_pause_menu({**e, **change})

    def test_future_full_convergence_still_requires_separate_factory_acceptance(self):
        e = self.baseline()
        better = {**e, 'overlap_differences': 1452,
                  'differences': e['differences'][:-1]}
        self.assertEqual(frontier.guard_pause_menu(better)['overlap_differences'], 1452)
        future_target = {**e['target']}
        future_source = {**e['candidate'], 'code_bytes': 1734,
                         'alignment_hex': '8bff', 'table_offset': 1736,
                         'instructions': 449,
                         'fields': [(0, 'DIR32')] * 106,
                         'table_destinations': list(frontier.PAUSE_TARGET_TABLE),
                         'internal_branches': [None] * 52}
        full = {**e, 'physical_bytes': 1776, 'missing_bytes': 0,
                'overlap_differences': 0, 'differences': [],
                'fields': [{} for _ in range(106)],
                'complete_equal': True,
                'target': future_target, 'candidate': future_source}
        self.assertEqual(frontier.guard_pause_menu(full)['overlap_differences'], 0)
        self.assertEqual(full['exactness_credit'], 'none')
        with self.assertRaises(RuntimeError):
            frontier.guard_pause_menu({**full, 'exactness_credit': 'accepted'})


if __name__ == '__main__':
    unittest.main()
