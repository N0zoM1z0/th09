"""Fail-closed source/target input tests for the separate 1,280-byte Title boss.

Original PE field values/CFG and the full 1,258-byte source are read by the
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


def current():
    return {
        'diagnostic_only': True, 'exactness_credit': 'none',
        'target_sha256': original.ORIGINAL_SHA256,
        'target_bytes': 1280, 'candidate_bytes': 1258,
        'target_instructions': 376, 'candidate_instructions': 368,
        'original_fields': 57, 'bound_fields': 57,
        'original_calls': 13, 'candidate_calls': 13,
        'ordered_calls_agree': True, 'original_blocks': 72,
        'candidate_blocks': 70, 'direct_graphs_agree': False,
        'aligned_instructions': 238, 'overlap_differences': 1187,
        'difference_offsets': sorted(set(range(1258)) -
                                   frontier.TITLE_REPLAY_MENU_MATCHED_OFFSETS),
        'missing_target_bytes': 22, 'extra_candidate_bytes': 0,
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
        self.assertEqual(len(frontier.TITLE_REPLAY_MENU_MATCHED_OFFSETS), 71)
        self.assertEqual(len(report['difference_offsets']), 1187)
        # A lower total or better aligned score cannot pay for losing a byte
        # that already equals the independently linked original.
        for offset in (0, 0x19, 0x1A2, 0x2CF, 0x492):
            with self.subTest(offset=hex(offset)):
                changed = {**report, 'difference_offsets': sorted(
                    report['difference_offsets'][1:] + [offset])}
                self.assertEqual(len(changed['difference_offsets']), 1187)
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
        self.assertEqual(report['title_draw_replay_menu']['overlap_differences'], 1187)
        corrupted = current()
        corrupted['difference_offsets'] = [0] + corrupted['difference_offsets'][1:]
        with patch.object(frontier, 'build_nonexact'), patch.object(
                frontier, 'parsed', return_value=corrupted), self.assertRaises(RuntimeError):
            frontier.run_title_replay_menu()

    def test_narrower_valid_or_exact_bound_source_never_claims_acceptance(self):
        report = current()
        closer = {**report, 'candidate_bytes': 1260,
                  'candidate_instructions': 370, 'candidate_blocks': 71,
                  'aligned_instructions': 250, 'missing_target_bytes': 20,
                  'overlap_differences': 1175,
                  'difference_offsets': report['difference_offsets'][:1175]}
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
            {'target_bytes': 1279}, {'candidate_bytes': 1257},
            {'candidate_bytes': 1281},
            {'target_instructions': 375},
            {'candidate_instructions': 367}, {'candidate_instructions': 377},
            {'original_fields': 56}, {'bound_fields': 56},
            {'original_calls': 12}, {'candidate_calls': 14},
            {'ordered_calls_agree': False}, {'original_blocks': 71},
            {'candidate_blocks': 69}, {'candidate_blocks': 73},
            {'direct_graphs_agree': True}, {'direct_graphs_agree': None},
            {'aligned_instructions': 237},
            {'overlap_differences': 1188},
            {'overlap_differences': '1187'},
            {'difference_offsets': report['difference_offsets'][:-1]},
            {'difference_offsets': list(reversed(report['difference_offsets']))},
            {'difference_offsets': [str(i) for i in report['difference_offsets']]},
            {'difference_offsets': [0] * 1187},
            {'difference_offsets': [*range(1186), 1258]},
            {'difference_offsets': None},
            {'missing_target_bytes': 21}, {'extra_candidate_bytes': 1},
            {'complete_bytes_equal': True},
        )
        for patch in changes:
            with self.subTest(patch=str(patch)[:100]), self.assertRaises(RuntimeError):
                frontier.guard_title_replay_menu({**report, **patch})
