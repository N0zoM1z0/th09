"""Pure-input checks for cross-profile cold exact-sibling regressions.

RunEcl and Title have large near-exact owners. Their strict known-exact
siblings span several real compiler carriers; a good boss-only diagnostic
must never conceal a broken exact function in an alternate carrier.
"""

import copy
import importlib.util
from pathlib import Path
import tomllib
import unittest
from unittest.mock import patch

ROOT = Path(__file__).resolve().parents[1]
loader = importlib.util.spec_from_file_location(
    'boss_exact_carriers', ROOT / 'scripts/check-boss-frontiers.py')
frontier = importlib.util.module_from_spec(loader)
loader.loader.exec_module(frontier)


class ExactCarrierDiscovery(unittest.TestCase):
    @staticmethod
    def units():
        with (ROOT / 'config/match-units.toml').open('rb') as stream:
            return tomllib.load(stream)['units']

    def test_real_ecl_main_and_alternate_profiles_are_all_covered(self):
        groups = frontier.exact_source_carriers(
            'src/EclManager.cpp', frontier.ECL_EXACT_CARRIERS)
        self.assertEqual(len(groups), 7)
        self.assertEqual(sum(map(len, groups.values())), 24)
        self.assertIn('ecl-pop-context', groups['build/matching/EclManager.obj'])
        self.assertIn('exattack-type21-update', groups['build/matching/EclManager.obj'])
        self.assertIn('exattack-type6-initialize',
                      groups['build/matching/EclManagerType6Initialize.obj'])
        self.assertEqual(len(set(sum(groups.values(), []))), 24)

    def test_ecl_private_label_targets_remain_original_across_carriers(self):
        # These entries used different compiler-private $L spellings before
        # the shared Etama side owner became a genuine type. A symbol rename
        # is harmless only if it still resolves to the *same* target VA.
        expected = {
            'ecl-post-update-movement': {
                0x139: 0x00408548,
                0x3C8: 0x004082BD, 0x3CC: 0x004082C8,
                0x3D0: 0x004082D6, 0x3D4: 0x004082E7,
                0x3D8: 0x004082F4, 0x3DC: 0x00408301,
            },
            'ecl-compare-operands': {
                0x14: 0x00407A4C,
                0x34C: 0x00407718, 0x350: 0x00407753,
                0x354: 0x0040779C, 0x358: 0x004077D7,
                0x35C: 0x00407820, 0x360: 0x0040785B,
                0x364: 0x004078A6, 0x368: 0x004078E1,
                0x36C: 0x0040792C, 0x370: 0x00407967,
                0x374: 0x004079B9, 0x378: 0x004079F0,
            },
        }
        for unit, addresses in expected.items():
            fields = {r['offset']: r for r in self.units()[unit]['relocations']
                      if r['symbol'].startswith('$L')}
            self.assertEqual(set(fields), set(addresses))
            for off, target in addresses.items():
                with self.subTest(unit=unit, offset=hex(off)):
                    self.assertEqual(fields[off]['target'], target)
                    self.assertEqual(fields[off]['type'], 'DIR32')

    def test_replay_owner_publication_keeps_all_nine_original_title_labels(self):
        # The genuine shared 0x164-byte ReplayManager view introduces new
        # compiler-private labels but changes zero target bytes. Validate
        # each old retail address rather than accepting symbol-string churn.
        unit = self.units()['title-screen-start-menu']
        self.assertEqual(unit['source'], 'src/TitleScreen.cpp')
        expected = {
            0x3C2: 0x0042A43B,
            0x618: 0x0042A1E9, 0x61C: 0x0042A208,
            0x620: 0x0042A23C, 0x624: 0x0042A286,
            0x628: 0x0042A2D2, 0x62C: 0x0042A293,
            0x630: 0x0042A302, 0x634: 0x0042A380,
        }
        actual = {row['offset']: row['target'] for row in unit['relocations']
                  if row['type']=='DIR32' and row['symbol'].startswith('$L')}
        for off, address in expected.items():
            with self.subTest(offset=hex(off)):
                self.assertEqual(actual[off], address)
        self.assertEqual(set(expected), set(actual))

    def test_real_title_main_and_o2_score_carriers_are_all_covered(self):
        groups = frontier.exact_source_carriers(
            'src/TitleScreen.cpp', frontier.TITLE_EXACT_CARRIERS)
        self.assertEqual(len(groups), 2)
        self.assertEqual(len(groups['build/matching/TitleScreen.obj']), 37)
        self.assertEqual(groups['build/matching/TitleScoreRecord.obj'],
                         ['score-record-insert'])
        self.assertIn('title-screen-setup-thread',
                      groups['build/matching/TitleScreen.obj'])

    def test_real_anm_render_carrier_has_all_fifteen_exact_siblings(self):
        groups = frontier.exact_source_carriers(
            'src/AnmManagerRender.cpp', frontier.ANM_RENDER_EXACT_CARRIERS)
        self.assertEqual(groups.keys(), {'build/matching/AnmManagerRender.obj'})
        units = groups['build/matching/AnmManagerRender.obj']
        self.assertEqual(len(units), 15)
        self.assertEqual(len(set(units)), 15)
        for name in ('anm-manager-translate-rotation', 'anm-manager-draw2d',
                     'anm-manager-draw-camera-facing-quad',
                     'anm-manager-draw-projected-3d-quad',
                     'anm-manager-flush-vertex-buffer',
                     'anm-manager-draw-inner-flipped-x'):
            self.assertIn(name, units)

    def test_real_etama_draw_carrier_has_both_exact_siblings(self):
        groups = frontier.exact_source_carriers(
            'src/EtamaControllerDraw.cpp', frontier.ETAMA_DRAW_EXACT_CARRIERS)
        self.assertEqual(groups, {'build/matching/EtamaControllerDraw.obj': [
            'bullet-draw-single', 'etama-controller-added-callback']})

    def test_partial_anm_or_etama_carrier_inventory_is_rejected(self):
        for source, expected, missing in (
                ('src/AnmManagerRender.cpp', frontier.ANM_RENDER_EXACT_CARRIERS,
                 'anm-manager-draw-inner-flipped-x'),
                ('src/EtamaControllerDraw.cpp', frontier.ETAMA_DRAW_EXACT_CARRIERS,
                 'etama-controller-added-callback')):
            with self.subTest(source=source):
                units = self.units()
                units.pop(missing)
                with self.assertRaisesRegex(RuntimeError,
                                            'exact sibling carrier inventory drifted'):
                    frontier.exact_source_carriers(source, expected, units)

    def test_prebuilt_anm_etama_carriers_verify_every_unit_without_rebuilding(self):
        for source, expected, prebuilt, total in (
                ('src/AnmManagerRender.cpp', frontier.ANM_RENDER_EXACT_CARRIERS,
                 ('build/matching/AnmManagerRender.obj',), 15),
                ('src/EtamaControllerDraw.cpp', frontier.ETAMA_DRAW_EXACT_CARRIERS,
                 ('build/matching/EtamaControllerDraw.obj',), 2)):
            with self.subTest(source=source), patch.object(
                    frontier, 'build') as build, patch.object(
                    frontier, 'canonical', side_effect=lambda name: {'unit': name}) as replay:
                result = frontier.cold_replay_exact_source(
                    source, expected, already_built=prebuilt)
                build.assert_not_called()
                self.assertEqual(len(result), total)
                self.assertEqual(replay.call_count, total)
                self.assertEqual(len({r['unit'] for r in result}), total)

    def test_fail_closed_on_missing_or_retargeted_exact_unit(self):
        for source, expected, missing in (
                ('src/EclManager.cpp', frontier.ECL_EXACT_CARRIERS,
                 'exattack-type21-update'),
                ('src/TitleScreen.cpp', frontier.TITLE_EXACT_CARRIERS,
                 'score-record-insert')):
            with self.subTest(source=source):
                units = self.units()
                units.pop(missing)
                with self.assertRaisesRegex(RuntimeError,
                                            'exact sibling carrier inventory drifted'):
                    frontier.exact_source_carriers(source, expected, units)

    def test_fail_closed_on_mixed_profile_and_duplicate_function(self):
        unit = 'title-screen-setup-thread'
        other = 'title-screen-release'
        for field, bad in (
                ('profile', ['/O2']),
                ('functions', self.units()[other]['functions']),
                ('object', 'build/matching/not-the-original.obj')):
            with self.subTest(field=field):
                units = self.units()
                units[unit] = copy.deepcopy(units[unit])
                units[unit][field] = bad
                with self.assertRaises(RuntimeError):
                    frontier.exact_source_carriers(
                        'src/TitleScreen.cpp', frontier.TITLE_EXACT_CARRIERS,
                        units)

    def test_reject_empty_unreviewed_symbols_and_prebuilt_carriers(self):
        units = self.units()
        units['ecl-pop-context'] = copy.deepcopy(units['ecl-pop-context'])
        units['ecl-pop-context']['functions'] = ['']
        with self.assertRaisesRegex(RuntimeError, 'non-canonical exact carrier'):
            frontier.exact_source_carriers(
                'src/EclManager.cpp', frontier.ECL_EXACT_CARRIERS, units)
        with self.assertRaisesRegex(RuntimeError, 'prebuilt exact carrier'):
            frontier.cold_replay_exact_source(
                'src/EclManager.cpp', frontier.ECL_EXACT_CARRIERS,
                already_built=('build/matching/wrong.obj',))

    def test_cold_builds_every_distinct_carrier_once_and_replays_every_unit(self):
        for source, expected, prebuilt, nbuild, nexact in (
                ('src/EclManager.cpp', frontier.ECL_EXACT_CARRIERS,
                 ('build/matching/EclManager.obj',), 6, 24),
                ('src/TitleScreen.cpp', frontier.TITLE_EXACT_CARRIERS,
                 (), 2, 38)):
            with self.subTest(source=source), patch.object(
                    frontier, 'build') as build, patch.object(
                    frontier, 'canonical', side_effect=lambda unit: {'unit': unit}) as canonical:
                reports = frontier.cold_replay_exact_source(
                    source, expected, already_built=prebuilt)
                self.assertEqual(len(reports), nexact)
                self.assertEqual(build.call_count, nbuild)
                self.assertEqual(canonical.call_count, nexact)
                self.assertEqual(len({r['unit'] for r in reports}), nexact)

    def test_bad_exact_comparison_cannot_be_counted_as_pass(self):
        with patch.object(frontier, 'build'), patch.object(
                frontier, 'canonical', side_effect=RuntimeError('nonexact original')):
            with self.assertRaisesRegex(RuntimeError, 'nonexact original'):
                frontier.cold_replay_exact_source(
                    'src/TitleScreen.cpp', frontier.TITLE_EXACT_CARRIERS)

    def test_title_owner_runs_38_screen_plus_two_music_plus_draw_callback(self):
        with patch.object(frontier, 'build') as build, patch.object(
                frontier, 'canonical', side_effect=lambda unit: {'unit': unit}):
            owners, exact = frontier.run_title(protect_exact=True)
            self.assertEqual(owners, {})
            self.assertEqual(len(exact), 41)
            self.assertEqual(len({r['unit'] for r in exact}), 41)
            self.assertEqual(build.call_count, 4)
            self.assertEqual([r['unit'] for r in exact[-3:]], [
                'title-screen-music-room', 'title-screen-draw-music-room',
                'title-draw-callback'])
        with patch.object(frontier, 'build') as build:
            owners, exact = frontier.run_title(protect_exact=False)
            self.assertEqual((owners, exact), ({}, []))
            build.assert_not_called()


if __name__ == '__main__':
    unittest.main()
