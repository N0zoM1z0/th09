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
        self.assertEqual(len(groups), 2)
        self.assertEqual(sum(map(len, groups.values())), 80)
        self.assertIn('ecl-pop-context', groups['build/matching/EclManager.obj'])
        self.assertIn('effect-polar-set-from-angle-axes', groups['build/matching/EclManager.obj'])
        self.assertIn('effect-initialize-callback-ring', groups['build/matching/EclManager.obj'])
        self.assertIn('effect-initialize-oriented-offset', groups['build/matching/EclManager.obj'])
        self.assertIn('effect-draw-callback-ring', groups['build/matching/EclManager-Type11.obj'])
        self.assertIn('effect-draw-callback-type34', groups['build/matching/EclManager.obj'])
        self.assertIn('exattack-type21-update', groups['build/matching/EclManager.obj'])
        self.assertIn('exattack-type19-init', groups['build/matching/EclManager.obj'])
        self.assertIn('exattack-type19-update', groups['build/matching/EclManager.obj'])
        self.assertIn('exattack-type21-init', groups['build/matching/EclManager.obj'])
        self.assertIn('player-position-callback-type4', groups['build/matching/EclManager.obj'])
        self.assertIn('player-position-callback-type8', groups['build/matching/EclManager.obj'])
        self.assertIn('exattack-type6-initialize',
                      groups['build/matching/EclManager.obj'])
        self.assertEqual(len(set(sum(groups.values(), []))), 80)

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

    def test_real_anm_render_ob0_carrier_covers_all_sixteen_owners(self):
        groups = frontier.exact_source_carriers(
            'src/AnmManagerRender.cpp', frontier.ANM_RENDER_EXACT_CARRIERS)
        self.assertEqual(groups.keys(), {'build/matching/AnmManagerRender.obj'})
        self.assertIn('anm-manager-project-camera-facing-quad',
                      groups['build/matching/AnmManagerRender.obj'])
        units = groups['build/matching/AnmManagerRender.obj']
        self.assertEqual(len(units), 16)
        self.assertEqual(len(set(units)), 16)
        for name in ('anm-manager-translate-rotation', 'anm-manager-draw2d',
                     'anm-manager-draw-camera-facing-quad',
                     'anm-manager-draw-projected-3d-quad',
                     'anm-manager-flush-vertex-buffer',
                     'anm-manager-draw-inner-flipped-x'):
            self.assertIn(name, units)

    def test_screen_effect_all_eighteen_actual_source_profiles_are_reviewed(self):
        expected = frontier.SCREEN_EFFECT_EXACT_PROFILES
        self.assertEqual({ref: spec[0] for ref, spec in expected.items()}, {
            'screen-effect-calc-shake': 16,
            'screen-effect-register-chain': 1,
            'screen-effect-draw-square': 1})
        self.assertEqual({ref: spec[1] for ref, spec in expected.items()}, {
            'screen-effect-calc-shake': 'build/matching/ScreenEffect.obj',
            'screen-effect-register-chain': 'build/matching/ScreenEffectRegister.obj',
            'screen-effect-draw-square': 'build/matching/ScreenEffectDrawSquare.obj'})
        groups = frontier.exact_source_profile_groups(
            'src/ScreenEffect.cpp', expected)
        self.assertEqual(set(groups), set(expected))
        self.assertEqual([len(v) for v in groups.values()], [16, 1, 1])
        self.assertIn('screen-effect-calc-shake', groups['screen-effect-calc-shake'])
        self.assertIn('screen-effect-draw-arcade-pulse',
                      groups['screen-effect-calc-shake'])
        self.assertIn('screen-effect-constructor', groups['screen-effect-calc-shake'])
        self.assertEqual(groups['screen-effect-register-chain'],
                         ['screen-effect-register-chain'])
        self.assertEqual(groups['screen-effect-draw-square'],
                         ['screen-effect-draw-square'])
        all_units = sum(groups.values(), [])
        self.assertEqual(len(set(all_units)), 18)
        units = self.units()
        profiles = [units[ref]['profile'] for ref in groups]
        self.assertEqual(len({tuple(flags) for flags in profiles}), 3)
        self.assertNotIn('/EHsc', units['screen-effect-register-chain']['profile'])
        self.assertIn('/Ob0', units['screen-effect-draw-square']['profile'])
        self.assertNotIn('/Ob0', units['screen-effect-calc-shake']['profile'])

    def test_screen_effect_missing_mixed_or_unreviewed_profile_fails_closed(self):
        original = self.units()
        for missing in ('screen-effect-delete', 'screen-effect-register-chain',
                        'screen-effect-draw-square'):
            with self.subTest(missing=missing):
                units = copy.deepcopy(original)
                del units[missing]
                with self.assertRaises(RuntimeError):
                    frontier.exact_source_profile_groups(
                        'src/ScreenEffect.cpp',
                        frontier.SCREEN_EFFECT_EXACT_PROFILES, units)
        for key, field, value in (
            ('screen-effect-delete', 'profile', ['/O2']),
            ('screen-effect-register-chain', 'profile', ['/O2']),
            ('screen-effect-draw-square', 'object',
             'build/matching/OtherUnreviewed.obj'),
            ('screen-effect-draw-square', 'profile', []),
            ('screen-effect-clear', 'functions',
             original['screen-effect-calc-shake']['functions']),
        ):
            with self.subTest(key=key, field=field):
                units = copy.deepcopy(original)
                units[key][field] = value
                with self.assertRaises(RuntimeError):
                    frontier.exact_source_profile_groups(
                        'src/ScreenEffect.cpp',
                        frontier.SCREEN_EFFECT_EXACT_PROFILES, units)
        refs_default = frontier.SCREEN_EFFECT_EXACT_PROFILES
        for refs in (
                {'screen-effect-calc-shake': refs_default['screen-effect-calc-shake']},
                {**refs_default,
                 'screen-effect-calc-shake':
                     (17, *refs_default['screen-effect-calc-shake'][1:])},
                {**refs_default,
                 'screen-effect-clear': refs_default['screen-effect-calc-shake']},
                {**refs_default,
                 'screen-effect-calc-shake':
                     (16, 'build/matching/Forged.obj',
                      refs_default['screen-effect-calc-shake'][2])},
        ):
            with self.subTest(refs=refs), self.assertRaises(RuntimeError):
                frontier.exact_source_profile_groups(
                    'src/ScreenEffect.cpp', refs)

    def test_screen_effect_profiles_build_once_each_and_verify_all_originals(self):
        for prebuilt, builds in (
                (('screen-effect-calc-shake',),
                 ['screen-effect-register-chain', 'screen-effect-draw-square']),
                ((), ['screen-effect-calc-shake', 'screen-effect-register-chain',
                      'screen-effect-draw-square'])):
            with self.subTest(prebuilt=prebuilt), patch.object(
                    frontier, 'build') as build, patch.object(
                    frontier, 'canonical', side_effect=lambda n: {'unit': n}) as check:
                result = frontier.cold_replay_exact_profiles(
                    'src/ScreenEffect.cpp', frontier.SCREEN_EFFECT_EXACT_PROFILES,
                    already_built=prebuilt)
                self.assertEqual(len(result), 18)
                self.assertEqual(len(set(r['unit'] for r in result)), 18)
                self.assertEqual([a[0][0] for a in build.call_args_list], builds)
                self.assertEqual(check.call_count, 18)
        with self.assertRaisesRegex(RuntimeError, 'already-built exact profile'):
            frontier.cold_replay_exact_profiles(
                'src/ScreenEffect.cpp', frontier.SCREEN_EFFECT_EXACT_PROFILES,
                already_built=('not-a-real-profile',))
        with patch.object(frontier, 'build'), patch.object(
                frontier, 'canonical', side_effect=RuntimeError('original bytes differ')):
            with self.assertRaisesRegex(RuntimeError, 'original bytes differ'):
                frontier.cold_replay_exact_profiles(
                    'src/ScreenEffect.cpp', frontier.SCREEN_EFFECT_EXACT_PROFILES)

    def test_replay_big_playback_owner_guards_all_twelve_true_source_profiles(self):
        contracts = frontier.REPLAY_INPUT_EXACT_PROFILES
        self.assertEqual({name: item[0] for name, item in contracts.items()}, {
            'replay-manager-begin-recording-stage': 2,
            'replay-input-update': 10,
        })
        groups = frontier.exact_source_profile_groups(
            'src/ReplayInput.cpp', contracts)
        self.assertEqual(set(groups), set(contracts))
        self.assertEqual(groups['replay-manager-begin-recording-stage'], [
            'replay-manager-begin-recording-stage', 'replay-manager-create'])
        self.assertEqual(len(groups['replay-input-update']), 10)
        self.assertIn('replay-manager-release', groups['replay-input-update'])
        self.assertIn('replay-manager-record-input', groups['replay-input-update'])
        self.assertIn('replay-manager-playback-input', groups['replay-input-update'])
        self.assertIn('replay-input-state-constructor', groups['replay-input-update'])
        self.assertEqual(len(set(sum(groups.values(), []))), 12)
        u = self.units()
        for ref, (n, obj, flags) in contracts.items():
            self.assertEqual(u[ref]['source'], 'src/ReplayInput.cpp')
            self.assertEqual(u[ref]['object'], obj)
            self.assertEqual(u[ref]['profile'], list(flags))
        self.assertNotIn('/EHsc', contracts['replay-manager-begin-recording-stage'][2])
        self.assertIn('/EHsc', contracts['replay-input-update'][2])

    def test_replay_profile_missing_unit_and_stale_flags_fail_closed(self):
        originals = self.units()
        for missing in ('replay-manager-create', 'replay-manager-release',
                        'replay-manager-playback-input', 'replay-input-state-constructor'):
            with self.subTest(missing=missing):
                units = copy.deepcopy(originals)
                del units[missing]
                with self.assertRaises(RuntimeError):
                    frontier.exact_source_profile_groups(
                        'src/ReplayInput.cpp', frontier.REPLAY_INPUT_EXACT_PROFILES,
                        units)
        for name, field, bad in (
                ('replay-manager-begin-recording-stage', 'profile', ['/EHsc']),
                ('replay-input-update', 'profile', ['/O1']),
                ('replay-manager-release', 'profile', ['/O2']),
                ('replay-manager-create', 'object', 'build/matching/Wrong.obj'),
                ('replay-manager-record-input', 'functions',
                 originals['replay-manager-playback-input']['functions']),
        ):
            with self.subTest(unit=name, field=field):
                units = copy.deepcopy(originals)
                units[name][field] = bad
                with self.assertRaises(RuntimeError):
                    frontier.exact_source_profile_groups(
                        'src/ReplayInput.cpp', frontier.REPLAY_INPUT_EXACT_PROFILES,
                        units)
        contract = frontier.REPLAY_INPUT_EXACT_PROFILES
        for incorrect in (
                {**contract,
                 'replay-input-update': (9, *contract['replay-input-update'][1:])},
                {'replay-manager-begin-recording-stage':
                     contract['replay-manager-begin-recording-stage']},
                {**contract,
                 'replay-input-update':
                    (10,'build/matching/Wrong.obj',contract['replay-input-update'][2])}):
            with self.subTest(contract=incorrect), self.assertRaises(RuntimeError):
                frontier.exact_source_profile_groups(
                    'src/ReplayInput.cpp', incorrect)

    def test_replay_cold_uses_original_no_eh_and_eh_carriers_once(self):
        for prebuilt, expected_builds in (
                (('replay-manager-begin-recording-stage',),
                 ['replay-input-update']),
                ((), ['replay-manager-begin-recording-stage',
                      'replay-input-update'])):
            with self.subTest(prebuilt=prebuilt), patch.object(
                    frontier, 'build') as build, patch.object(
                    frontier, 'canonical', side_effect=lambda name: {'unit': name}) as canonical:
                result = frontier.cold_replay_exact_profiles(
                    'src/ReplayInput.cpp', frontier.REPLAY_INPUT_EXACT_PROFILES,
                    already_built=prebuilt)
                self.assertEqual(len(result), 12)
                self.assertEqual(len({item['unit'] for item in result}), 12)
                self.assertEqual([a[0][0] for a in build.call_args_list], expected_builds)
                self.assertEqual(canonical.call_count, 12)
        with self.assertRaisesRegex(RuntimeError, 'already-built exact profile'):
            frontier.cold_replay_exact_profiles(
                'src/ReplayInput.cpp', frontier.REPLAY_INPUT_EXACT_PROFILES,
                already_built=('not-a-real-replay-profile',))
        with patch.object(frontier, 'build'), patch.object(
                frontier, 'canonical', side_effect=RuntimeError('bad replay PE bytes')):
            with self.assertRaisesRegex(RuntimeError, 'bad replay PE bytes'):
                frontier.cold_replay_exact_profiles(
                    'src/ReplayInput.cpp', frontier.REPLAY_INPUT_EXACT_PROFILES)

    def test_effect_big_resource_callback_protects_all_fifteen_exact_members(self):
        groups = frontier.exact_source_carriers(
            'src/EffectManager.cpp', frontier.EFFECT_MANAGER_EXACT_CARRIERS)
        self.assertEqual(groups.keys(), {'build/matching/EffectManager.obj'})
        names = groups['build/matching/EffectManager.obj']
        self.assertEqual(len(names), 15)
        self.assertEqual(len(set(names)), 15)
        for name in ('effect-manager-update', 'effect-manager-draw',
                     'effect-manager-draw-shared', 'effect-manager-create',
                     'effect-manager-initialize-effect', 'effect-update-from-vm',
                     'effect-manager-spawn-effect',
                     'effect-manager-spawn-effect-with-velocity',
                     'effect-manager-spawn-effect-fixed-slot'):
            self.assertIn(name, names)

    def test_effect_source_inventory_rejects_unreviewed_changes(self):
        original = self.units()
        for missing in ('effect-manager-release',
                        'effect-manager-spawn-effect-fixed-slot',
                        'effect-manager-update'):
            with self.subTest(missing=missing):
                units = copy.deepcopy(original)
                units.pop(missing)
                with self.assertRaisesRegex(RuntimeError,
                                            'exact sibling carrier inventory drifted'):
                    frontier.exact_source_carriers(
                        'src/EffectManager.cpp',
                        frontier.EFFECT_MANAGER_EXACT_CARRIERS, units)
        for unit_name, field, value in (
                ('effect-manager-update', 'profile', ['/O1']),
                ('effect-manager-draw', 'object', 'build/matching/Other.obj'),
                ('effect-manager-initialize-effect', 'functions',
                 original['effect-manager-create']['functions'])):
            with self.subTest(unit=unit_name, field=field):
                units = copy.deepcopy(original)
                units[unit_name][field] = value
                with self.assertRaises(RuntimeError):
                    frontier.exact_source_carriers(
                        'src/EffectManager.cpp',
                        frontier.EFFECT_MANAGER_EXACT_CARRIERS, units)

    def test_effect_prebuilt_carrier_replays_all_fifteen_without_second_build(self):
        with patch.object(frontier, 'build') as build, patch.object(
                frontier, 'canonical', side_effect=lambda name: {'unit': name}) as canonical:
            exact = frontier.cold_replay_exact_source(
                'src/EffectManager.cpp', frontier.EFFECT_MANAGER_EXACT_CARRIERS,
                already_built=('build/matching/EffectManager.obj',))
        build.assert_not_called()
        self.assertEqual(len(exact), 15)
        self.assertEqual(len({item['unit'] for item in exact}), 15)
        self.assertEqual(canonical.call_count, 15)
        with patch.object(frontier, 'build'), patch.object(
                frontier, 'canonical', side_effect=RuntimeError('Effect original mismatch')):
            with self.assertRaisesRegex(RuntimeError, 'Effect original mismatch'):
                frontier.cold_replay_exact_source(
                    'src/EffectManager.cpp', frontier.EFFECT_MANAGER_EXACT_CARRIERS)

    def test_effect_owner_builds_one_real_object_and_checks_all_fifteen(self):
        from unittest.mock import patch
        evidence = {
            'diagnostic_only': True, 'exactness_credit': 'none',
            'target_sha256': frontier.EFFECT_ADDED_ORIGINAL_SHA,
            'target_bytes': 51, 'candidate_bytes': 51,
            'target_instructions': 19, 'candidate_instructions': 19,
            'target_relocations': 4, 'bound_relocations': 4,
            'original_blocks': 3, 'candidate_blocks': 3,
            'complete_direct_graphs_agree': True,
            'ordered_calls_match': True,
            'differing_bytes': 8,
            'difference_offsets': [f'0x{x:x}' for x in (*range(3, 10), 13)],
            'complete_bytes_equal': False,
        }
        with patch.object(frontier, 'build') as build, patch.object(
                frontier, 'parsed', return_value=evidence) as parsed, patch.object(
                frontier, 'canonical', side_effect=lambda unit: {'unit': unit}) as canonical:
            owners, siblings = frontier.run_effect(protect_exact=True)
        self.assertEqual([call.args[0] for call in build.call_args_list],
                         ['effect-manager-update', 'anm-loaded-set-and-execute-script-idx'])
        parsed.assert_called_once_with([
            'scripts/inspect-effect-added.py', 'build/matching/EffectManager.obj', '--json'],
            diagnostic=True)
        self.assertEqual(owners['effect_added_callback']['ordinary_differences'], 8)
        groups = frontier.exact_source_carriers(
            'src/EffectManager.cpp', frontier.EFFECT_MANAGER_EXACT_CARRIERS)
        self.assertEqual([r['unit'] for r in siblings],
                         groups['build/matching/EffectManager.obj'] +
                         ['anm-loaded-set-and-execute-script-idx'])
        self.assertEqual(canonical.call_count, 16)
        with patch.object(frontier, 'build') as build, patch.object(
                frontier, 'parsed', return_value=evidence), patch.object(
                frontier, 'canonical') as canonical:
            owners, siblings = frontier.run_effect(protect_exact=False)
        self.assertEqual(owners['effect_added_callback']['ordinary_differences'], 8)
        self.assertEqual(siblings, [])
        build.assert_called_once_with('effect-manager-update')
        canonical.assert_not_called()

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
                 ('build/matching/AnmManagerRender.obj',), 16),
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
                 ('build/matching/EclManager.obj',), 1, 80),
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

    def test_title_owner_runs_38_screen_two_music_draw_and_two_real_audio_callees(self):
        with patch.object(frontier, 'build') as build, patch.object(
                frontier, 'canonical', side_effect=lambda unit: {'unit': unit}):
            owners, exact = frontier.run_title(protect_exact=True)
            self.assertEqual(owners, {})
            self.assertEqual(len(exact), 46)
            self.assertEqual(len({r['unit'] for r in exact}), 46)
            self.assertEqual(build.call_count, 9)
            self.assertEqual([r['unit'] for r in exact[-3:]], [
                'anm-manager-draw-text-left', 'anm-manager-load-surface',
                'anm-manager-set-interrupt-array'])
        # In --owner all RunEcl has already rechecked PlaySoundByIdx.
        # Only QueueCommand is a newly distinct Title-side exact regression.
        with patch.object(frontier, 'build') as build, patch.object(
                frontier, 'canonical', side_effect=lambda unit: {'unit': unit}):
            owners, exact = frontier.run_title(protect_exact=True,
                                               play_sound_prechecked=True)
            self.assertEqual(owners, {})
            self.assertEqual(len(exact), 45)
            self.assertEqual(len({r['unit'] for r in exact}), 45)
            self.assertEqual(build.call_count, 8)
            self.assertNotIn('soundplayer-play-sound-by-idx',
                             [r['unit'] for r in exact])
            self.assertEqual(exact[-1]['unit'], 'anm-manager-set-interrupt-array')
        with patch.object(frontier, 'build') as build:
            owners, exact = frontier.run_title(protect_exact=False)
            self.assertEqual((owners, exact), ({}, []))
            build.assert_not_called()


if __name__ == '__main__':
    unittest.main()
