"""Fail-closed source/target input tests for the separate 1,280-byte Title boss.

Original PE field values/CFG and the full 1,280-byte source with29 residual bytes are read by the
pinned standalone inspector; none of these no-target tests grants exactness.
"""
import importlib.util
from pathlib import Path
from unittest import TestCase

ROOT = Path(__file__).resolve().parents[1]
spec = importlib.util.spec_from_file_location(
    'replay_menu_frontier', ROOT / 'scripts/check-boss-frontiers.py')
frontier = importlib.util.module_from_spec(spec)
spec.loader.exec_module(frontier)
spec = importlib.util.spec_from_file_location(
    'replay_menu_original_oracle', ROOT / 'scripts/inspect-title-replay-menu-draw.py')
original = importlib.util.module_from_spec(spec)
spec.loader.exec_module(original)



# Independently recorded complete native comparison, not derived from the guard.
CURRENT_DIFFERENCES = (
    0x185, 0x18B, 0x193, 0x19D, 0x19E, 0x1A0, 0x1A1,
    0x1B6, 0x1BC, 0x1D3, 0x24A, 0x25A, 0x25C, 0x271,
    0x272, 0x29D, 0x2A3, 0x2C6, 0x2D7, 0x2F0, 0x2F5,
    0x2FD, 0x304, 0x308, 0x316, 0x319, 0x31C, 0x31E, 0x321,
)


def current():
    return {
        'diagnostic_only': True, 'exactness_credit': 'none',
        'target_sha256': original.ORIGINAL_SHA256,
        'target_bytes': 1280, 'candidate_bytes': 1280,
        'target_instructions': 376, 'candidate_instructions': 376,
        'original_fields': 57, 'bound_fields': 57,
        'original_calls': 13, 'candidate_calls': 13,
        'ordered_calls_agree': True, 'original_blocks': 72,
        'candidate_blocks': 72, 'direct_graphs_agree': True,
        'aligned_instructions': 350, 'overlap_differences': 29,
        'difference_offsets': list(CURRENT_DIFFERENCES),
        'missing_target_bytes': 0, 'extra_candidate_bytes': 0,
        'complete_bytes_equal': False,
    }


class TitleReplayMenuOriginalFrontierTests(TestCase):
    def test_current_physical_and_owned_original_identity_is_not_exact(self):
        report = current()
        self.assertEqual(original.ORIGINAL_SHA256, frontier.TITLE_REPLAY_MENU_TARGET_SHA256)
        self.assertEqual((original.BASE, original.SIZE), (0x004234F6, 1280))
        self.assertEqual((original.ORIGINAL_FIELDS, original.ORIGINAL_CALLS,
                          original.ORIGINAL_BLOCKS), (57, 13, 72))
        self.assertIsNone(frontier.guard_title_replay_menu(report))
        self.assertEqual(report['exactness_credit'], 'none')
        cpp = (ROOT / 'src/TitleScreenReplayMenuDraw.cpp').read_text()
        self.assertIn('#include "ReplayDataOriginalView.hpp"', cpp)
        self.assertIn('ReplayDataView replays[50]', cpp)

    def test_reviewed_independently_original_addresses_not_candidate_guesses(self):
        symbols = {
            '?g_AsciiManager@@3VAsciiManager@@A': 0x004CE458,
            '?g_ReplayCharacterNames@@3PAPBDA': 0x004A1CF8,
            '?g_ReplayDifficultyNames@@3PAPBDA': 0x004A1D3C,
            '?AddFormatText@AsciiManager@@QAAXPAUFloat3@@PBDZZ': 0x00434330,
            '_sprintf': 0x0047C47B,
            '_strlen': 0x0047C3F0,
        }
        for symbol, physical in symbols.items():
            with self.subTest(symbol=symbol):
                self.assertEqual(original.DESTINATIONS[symbol], physical)

    def test_same_count_exchange_cannot_damage_a_matching_physical_byte(self):
        report = current()
        self.assertEqual(len(frontier.TITLE_REPLAY_MENU_MATCHED_OFFSETS), 1251)
        self.assertEqual(len(report['difference_offsets']), 29)
        # A lower total or better aligned score cannot pay for losing a byte
        # that already equals the independently linked original.
        for offset in sorted(set(range(1280)) - set(CURRENT_DIFFERENCES)):
            with self.subTest(offset=hex(offset)):
                changed = {**report, 'difference_offsets': sorted(
                    report['difference_offsets'][1:] + [offset])}
                self.assertEqual(len(changed['difference_offsets']), 29)
                with self.assertRaisesRegex(RuntimeError, 'already matched'):
                    frontier.guard_title_replay_menu(changed)

    def test_standalone_menu_rebuilds_its_only_source_and_checks_the_guard(self):
        from unittest.mock import patch
        with patch.object(frontier, 'build_nonexact') as build, patch.object(
                frontier, 'parsed', return_value=current()) as inspect:
            report = frontier.run_title_replay_menu()
        build.assert_called_once_with(
            'src/TitleScreenReplayMenuDraw.cpp',
            'build/matching/TitleScreenReplayMenuDraw.obj',
            reference_unit='title-screen-setup-thread')
        inspect.assert_called_once_with([
            'scripts/inspect-title-replay-menu-draw.py',
            'build/matching/TitleScreenReplayMenuDraw.obj', '--json'])
        self.assertEqual(report['title_draw_replay_menu']['overlap_differences'], 29)
        corrupted = current()
        corrupted['difference_offsets'] = [0] + corrupted['difference_offsets'][1:]
        with patch.object(frontier, 'build_nonexact'), patch.object(
                frontier, 'parsed', return_value=corrupted), self.assertRaises(RuntimeError):
            frontier.run_title_replay_menu()

    def test_fewer_residuals_or_exact_bound_source_never_claims_acceptance(self):
        report = current()
        closer = {**report, 'aligned_instructions': 355,
                  'overlap_differences': 20,
                  'difference_offsets': report['difference_offsets'][:20]}
        frontier.guard_title_replay_menu(closer)
        self.assertEqual(closer['exactness_credit'], 'none')
        exact = {**report, 'candidate_bytes': 1280,
                 'candidate_instructions': 376, 'candidate_blocks': 72,
                 'direct_graphs_agree': True, 'aligned_instructions': 376,
                 'missing_target_bytes': 0, 'overlap_differences': 0,
                 'difference_offsets': [], 'complete_bytes_equal': True}
        frontier.guard_title_replay_menu(exact)
        self.assertEqual(exact['exactness_credit'], 'none')

    def test_reject_missing_wrong_unordered_or_forged_original_evidence(self):
        report = current()
        changes = (
            {'diagnostic_only': False}, {'exactness_credit': 'accepted'},
            {'target_sha256': '0' * 64},
            {'target_bytes': 1279}, {'candidate_bytes': 1279},
            {'candidate_bytes': 1281},
            {'target_instructions': 375},
            {'candidate_instructions': 375}, {'candidate_instructions': 377},
            {'original_fields': 56}, {'bound_fields': 56},
            {'original_calls': 12}, {'candidate_calls': 14},
            {'ordered_calls_agree': False}, {'original_blocks': 71},
            {'candidate_blocks': 71}, {'candidate_blocks': 73},
            {'direct_graphs_agree': False}, {'direct_graphs_agree': None},
            {'aligned_instructions': 349},
            {'overlap_differences': 30},
            {'overlap_differences': '29'},
            {'difference_offsets': report['difference_offsets'][:-1]},
            {'difference_offsets': list(reversed(report['difference_offsets']))},
            {'difference_offsets': [str(i) for i in report['difference_offsets']]},
            {'difference_offsets': [0] * 29},
            {'difference_offsets': [*range(28), 1280]},
            {'difference_offsets': None},
            {'missing_target_bytes': 1}, {'extra_candidate_bytes': 1},
            {'complete_bytes_equal': True},
        )
        for patch in changes:
            with self.subTest(patch=str(patch)[:100]), self.assertRaises(RuntimeError):
                frontier.guard_title_replay_menu({**report, **patch})

    def test_legacy_missing_geometry_and_nonmatching_cfg_are_rejected(self):
        legacy = {**current(), 'candidate_bytes': 1258,
                  'candidate_instructions': 368, 'candidate_blocks': 70,
                  'direct_graphs_agree': False, 'missing_target_bytes': 22}
        with self.assertRaises(RuntimeError):
            frontier.guard_title_replay_menu(legacy)
        with self.assertRaises(RuntimeError):
            frontier.guard_title_replay_menu({**current(), 'direct_graphs_agree': False})

    def test_actual_residual_set_pins_complete_comparison(self):
        self.assertEqual(set(CURRENT_DIFFERENCES),
                         frontier.TITLE_REPLAY_MENU_RESIDUAL_OFFSETS)
        frontier.guard_title_replay_menu(current())
        self.assertEqual(len(CURRENT_DIFFERENCES), 29)
