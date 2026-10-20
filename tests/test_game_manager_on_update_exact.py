"""Target-independent source/manifest and two-profile GameManager regressions."""

import importlib.util
from pathlib import Path
import tomllib
import unittest
from unittest.mock import patch

ROOT = Path(__file__).resolve().parents[1]
spec = importlib.util.spec_from_file_location('th09_boss',
       ROOT / 'scripts/check-boss-frontiers.py')
boss = importlib.util.module_from_spec(spec)
spec.loader.exec_module(boss)


class GameManagerExactSource(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        with (ROOT / 'config/match-units.toml').open('rb') as src:
            cls.units = tomllib.load(src)['units']
        cls.unit = cls.units['game-manager-on-update']
        cls.source = (ROOT / 'src/GameManagerSetup.cpp').read_text()

    def test_cleanup_manifest_keeps_original_extent_native_calls_and_os_profile(self):
        unit = self.units['game-manager-cleanup-gameplay-state']
        self.assertEqual((unit['source'], unit['object'], unit['profile']),
                         (self.unit['source'], self.unit['object'], self.unit['profile']))
        self.assertEqual((unit['target_address'], unit['size'], unit.get('compare_size', 425)),
                         (0x41B5C6, 425, 425))
        self.assertEqual(unit['symbol'], '?CleanupGameplayState@GameManagerSetupLayout@@SIXXZ')
        rows = unit['relocations']
        self.assertEqual(len(rows), 43)
        self.assertEqual([r['offset'] for r in rows], sorted({r['offset'] for r in rows}))
        self.assertEqual([r['target'] for r in rows if r['type'] == 'REL32'],
                         [0x41A7C3, 0x4343E0, 0x4181E0, 0x4156F0, 0x4011C0, 0x41EBC0,
                          0x412470, 0x40C990, 0x411FD0, 0x403D80, 0x41A230, 0x40C990])

    def test_complete_full_target_relocation_inventory(self):
        unit = self.unit
        self.assertEqual(unit['target_address'], 0x0041AA5F)
        self.assertEqual(unit['size'], 1230)
        self.assertEqual(unit['source'], 'src/GameManagerSetup.cpp')
        self.assertEqual(unit['symbol'], '?OnUpdate@GameManagerSetupLayout@@SIHPAU1@@Z')
        rows = unit['relocations']
        self.assertEqual(len(rows), 87)
        offsets = [r['offset'] for r in rows]
        self.assertEqual(len(offsets), len(set(offsets)))
        self.assertEqual(offsets, sorted(offsets))
        self.assertEqual(sum(r['type']=='REL32' for r in rows), 16)
        self.assertEqual(sum(r['type']=='DIR32' for r in rows), 71)
        self.assertEqual(unit['profile'], self.units['game-manager-advance-timed-state']['profile'])
        self.assertIn('/Os', unit['profile'])
        self.assertIn('/Oy-', unit['profile'])

    def test_original_three_disjoint_input_state_regions(self):
        rows = self.unit['relocations']
        symbol = '?g_ReplayInputStates@@3PAUReplayInputState@@A'
        input_fields = [r for r in rows if r['symbol']==symbol]
        self.assertEqual(len(input_fields), 17)  # two later independent pause-input reads
        self.assertTrue(all(r['target']==0x004ACE18 for r in input_fields))
        # The third input state is source, not either destination; the
        # independently exact constructor specifies 0x8e bytes per element.
        offsets = [r.get('addend', 0) for r in input_fields]
        self.assertEqual(offsets[:5], [284,286,288,290,292])
        self.assertEqual(offsets[5:10], [0,2,4,6,8])
        self.assertEqual(offsets[10:15], [142,144,146,148,150])
        self.assertEqual(offsets[15:], [290,290])
        self.assertFalse(set(offsets[:5]) & set(offsets[5:15]))

    def test_five_named_true_member_references_and_two_complete_publications(self):
        part = self.source.split('int GameManagerSetupLayout::OnUpdate(',1)[1].split(
            '\nint GameManagerSetupLayout::OnDraw(',1)[0]
        fields = ('currentInput','word02','repeatOutput','word06','word08')
        for i,field in enumerate(fields):
            self.assertIn(f'const unsigned short &w{i} = g_ReplayInputStates[2].{field};',part)
            self.assertEqual(part.count(f'g_ReplayInputStates[0].{field} = w{i};'),1)
            self.assertEqual(part.count(f'g_ReplayInputStates[1].{field} = w{i};'),1)
        self.assertNotIn('const InputSnapshot snapshot',part)
        self.assertIn('g_GameManager.playtimeFrames++;',part)

    def test_two_distinct_real_tu_profiles_are_never_confused(self):
        fast = self.unit['profile']
        plain = self.units['supervisor-setup-loading-vms-and-capture']['profile']
        self.assertIn('/Os',fast)
        self.assertNotIn('/Os',plain)
        self.assertEqual(self.unit['object'], self.units[
            'supervisor-setup-loading-vms-and-capture']['object'])


class GameManagerColdRegression(unittest.TestCase):
    @staticmethod
    def evidence():
        # Non-exact worker remains explicitly noncrediting and distinct from
        # the ten canonical full-owner exact GameManager callbacks.
        return {
            'diagnostic_only': True, 'exactness_credit': 'none',
            'target_bytes': 1689, 'candidate_bytes': 1689,
            'target_instructions': 413, 'candidate_instructions': 416,
            'original_fields': 172, 'bound_fields': 171,
            'ordered_calls': 35, 'candidate_calls': 35,
            'ordered_calls_agree': True,
            'original_blocks': 78, 'candidate_blocks': 77,
            'direct_graphs_agree': False,
            'aligned_field_identities': 157,
            'field_change_spans': 15,
            'original_only_field_referents': [dict(type=t, address=a, count=n)
                for t,a,n in boss.GAMEPLAY_SETUP_TARGET_ONLY_REFERENTS],
            'source_only_field_referents': [dict(type=t, address=a, count=n)
                for t,a,n in boss.GAMEPLAY_SETUP_SOURCE_ONLY_REFERENTS],
            'aligned_instructions': 394,
            'target_sha256': boss.GAMEPLAY_SETUP_TARGET_SHA,
            'difference_offsets': [i for i in range(1689) if
                boss.GAMEPLAY_SETUP_RESIDUAL_MASK[i // 8] & (1 << (i % 8))],
            'overlap_differences': 911,
            'complete_bytes_equal': False,
        }

    def test_gameplay_setup_exact_172_vs_171_original_referents(self):
        example = self.evidence()
        self.assertEqual(boss.GAMEPLAY_SETUP_TARGET_ONLY_REFERENTS, (
            ('DIR32', 0x4A7D90, 3), ('DIR32', 0x4A7DC4, 1),
            ('DIR32', 0x4A7E34, 1)))
        self.assertEqual(boss.GAMEPLAY_SETUP_SOURCE_ONLY_REFERENTS, (
            ('DIR32', 0x4A7DB8, 1), ('DIR32', 0x4A7E28, 1),
            ('DIR32', 0x4A7E44, 2)))
        boss.guard_gameplay_setup_field_referents(example)
        # A real improvement may reconcile a target-only and a candidate-only
        # memory referent without changing total field counts or calls.
        improved = {**example,
            'original_only_field_referents': [dict(type='DIR32',address=0x4A7D90,count=2),
                *example['original_only_field_referents'][1:]],
            'source_only_field_referents': [*example['source_only_field_referents'][:-1],
                dict(type='DIR32',address=0x4A7E44,count=1)]}
        boss.guard_gameplay_setup_field_referents(improved)
        for change in (
            {'original_only_field_referents': []},
            {'source_only_field_referents': []},
            {'original_only_field_referents': None},
            {'original_only_field_referents': example['original_only_field_referents'][::-1]},
            {'original_only_field_referents': example['original_only_field_referents'] +
             [dict(type='DIR32',address=0x4A7DCC,count=1)]},
            {'source_only_field_referents': example['source_only_field_referents'][:-1] +
             [dict(type='DIR32',address=0x4A7E44,count=3)]},
            {'source_only_field_referents': example['source_only_field_referents'][:-1] +
             [dict(type='REL32',address=0x4A7E44,count=2)]},
            {'source_only_field_referents': [dict(type='DIR32',address='0x4A7DB8',count=1)] +
             example['source_only_field_referents'][1:]},
        ):
            with self.subTest(change=change), self.assertRaises(RuntimeError):
                boss.guard_gameplay_setup_field_referents({**example, **change})

    @staticmethod
    def cleanup_evidence():
        return {'diagnostic_only': True,
         'exactness_credit': 'none',
         'target_bytes': 425,
         'candidate_bytes': 425,
         'target_instructions': 117,
         'candidate_instructions': 117,
         'original_fields': 43,
         'bound_fields': 43,
         'ordered_calls': 12,
         'ordered_calls_agree': True,
         'original_blocks': 24,
         'candidate_blocks': 24,
         'direct_graphs_agree': True,
         'aligned_field_identities': 43,
         'field_change_spans': 0,
         'aligned_instructions': 117,
         'overlap_differences': 0,
         'difference_offsets': [],
         'missing_target_bytes': 0,
         'extra_candidate_bytes': 0,
         'original_limit_publication': {'default_reads': [361],
                                        'all_writes': [162, 377, 384],
                                        'normal_clamp_branch': [382]},
         'candidate_limit_publication': {'default_reads': [361],
                                         'all_writes': [162, 377, 384],
                                         'normal_clamp_branch': [382]},
         'mode2_lower_bound_branch_present': True,
         'original_limit_double_store': True,
         'complete_bytes_equal': True,
         'target_sha256': 'e31f3c176b3b735008612b07823982a646248cbae4fb8ac826e2f2cff2d6a798',
         'source_raw_sha256': '4c4f1d42b1d0f651287714bf4fda111e2351e09450ee378319f9c1442b9af62d',
         'source_linked_sha256': 'e31f3c176b3b735008612b07823982a646248cbae4fb8ac826e2f2cff2d6a798'}

    def test_cleanup_zero_byte_gate_remains_noncrediting_and_rejects_old_frontiers(self):
        row = self.cleanup_evidence()
        self.assertIs(boss.guard_game_manager_cleanup(row), row)
        self.assertEqual(row['exactness_credit'], 'none')
        for stale in (dict(candidate_bytes=410, bound_fields=42, overlap_differences=210),
                      dict(candidate_bytes=422, bound_fields=43, missing_target_bytes=3),
                      dict(source_linked_sha256='0'*64), dict(overlap_differences=False)):
            with self.subTest(stale=stale), self.assertRaises(RuntimeError):
                boss.guard_game_manager_cleanup({**row, **stale})

    def test_all_fourteen_original_bound_functions_replayed_across_five_builds(self):
        with patch.object(boss, 'build') as build, patch.object(
                boss, 'parsed', side_effect=[self.evidence(), self.cleanup_evidence()]), patch.object(
                boss, 'canonical', side_effect=lambda unit: {'unit':unit}) as canonical:
            owners, exact = boss.run_game_manager(protect_exact=True)
        self.assertEqual(owners['gameplay_setup_thread']['bound_fields'],171)
        self.assertEqual(owners['game_manager_cleanup']['candidate_bytes'],425)
        self.assertFalse(owners['gameplay_setup_thread']['direct_graphs_agree'])
        self.assertEqual(len(exact),14)
        self.assertEqual(exact[0]['unit'],'game-manager-on-update')
        self.assertIn('front-message-initialize-runtime', [a['unit'] for a in exact])
        self.assertEqual([row['unit'] for row in exact[-4:]],
                         ['supervisor-setup-loading-vms-and-capture',
                          'front-message-initialize-runtime',
                          'replay-manager-release', 'replay-manager-create'])
        self.assertEqual(build.call_count,5)
        self.assertEqual([args[0][0] for args in build.call_args_list],
                         ['game-manager-on-update',
                          'supervisor-setup-loading-vms-and-capture',
                          'front-message-initialize-runtime',
                          'replay-manager-release', 'replay-manager-create'])
        self.assertEqual(canonical.call_count,14)

    def test_full_all_mode_does_not_double_count_either_replay_callee(self):
        # The full suite now runs both original ReplayInput VC7.1 profiles
        # before GameManager, including both exact Create and exact Release.
        # Full --owner all checks the Front exact member in the prior front
        # sweep. Focused gamemgr builds it alongside the Replay callees.
        with patch.object(boss, 'build') as build, patch.object(
                boss, 'parsed', side_effect=[self.evidence(), self.cleanup_evidence()]), patch.object(
                boss, 'canonical', side_effect=lambda name: {'unit': name}) as canonical:
            owners, exact = boss.run_game_manager(
                protect_exact=True, replay_callees_prechecked=True)
        self.assertEqual(owners['gameplay_setup_thread']['ordinary_differences'], 911)
        self.assertEqual(len(exact), 11)
        self.assertEqual(exact[-1]['unit'],
                         'supervisor-setup-loading-vms-and-capture')
        self.assertNotIn('replay-manager-create', [r['unit'] for r in exact])
        self.assertNotIn('replay-manager-release', [r['unit'] for r in exact])
        self.assertNotIn('front-message-initialize-runtime', [r['unit'] for r in exact])
        self.assertEqual([a[0][0] for a in build.call_args_list], [
            'game-manager-on-update',
            'supervisor-setup-loading-vms-and-capture'])
        self.assertEqual(canonical.call_count, 11)

    def test_skip_siblings_never_drops_the_new_large_boss_or_nonexact_worker(self):
        with patch.object(boss, 'build') as build, patch.object(
                boss, 'parsed', side_effect=[self.evidence(), self.cleanup_evidence()]), patch.object(
                boss, 'canonical',side_effect=lambda name: {'unit':name}):
            owners, exact = boss.run_game_manager(protect_exact=False)
        self.assertEqual([x['unit'] for x in exact], ['game-manager-on-update', 'game-manager-cleanup-gameplay-state'])
        self.assertEqual(owners['gameplay_setup_thread']['original_fields'],172)
        self.assertTrue(owners['game_manager_cleanup']['mode2_lower_bound_branch_present'])
        build.assert_called_once_with('game-manager-on-update')

    def test_missing_original_operand_wrong_tu_or_calls_fail_closed(self):
        for change in (
                {'target_bytes':1688}, {'candidate_bytes':1812},
                {'target_instructions':412}, {'original_fields':171},
                {'bound_fields':172}, {'ordered_calls':34},
                {'candidate_calls':34}, {'ordered_calls_agree':False},
                {'original_blocks':77}, {'candidate_blocks':78},
                {'aligned_field_identities':154},
                {'diagnostic_only':False}, {'exactness_credit':'accepted'}):
            with self.subTest(change=change), patch.object(
                    boss,'build'), patch.object(
                    boss,'canonical',return_value={'unit':'game-manager-on-update'}), patch.object(
                    boss,'parsed',side_effect=[{**self.evidence(),**change},self.cleanup_evidence()]), self.assertRaises(RuntimeError):
                boss.run_game_manager(protect_exact=False)

    def test_cleanup_source_changed_or_fake_original_fields_fail_closed(self):
        for change in ({'candidate_bytes':399}, {'candidate_bytes':406}, {'candidate_bytes':422},
                       {'target_bytes':424}, {'target_instructions':116},
                       {'candidate_instructions':113}, {'candidate_instructions':114}, {'original_fields':42},
                       {'bound_fields':40}, {'bound_fields':41}, {'bound_fields':42}, {'ordered_calls':11},
                       {'ordered_calls_agree':False}, {'original_blocks':25},
                       {'candidate_blocks':25}, {'mode2_lower_bound_branch_present':False},
                       {'original_limit_double_store':False},
                       {'original_limit_publication':{'default_reads':[0x169],
                            'all_writes':[0x0A2,0x180,0x179],
                            'normal_clamp_branch':[0x17E]}},
                       {'candidate_limit_publication':None},
                       {'candidate_limit_publication':{}},
                       {'candidate_limit_publication':{'default_reads':['0xc8'],
                            'all_writes':[0xA2,0x16A,0x171],
                            'normal_clamp_branch':[0x16F]}},
                       {'candidate_limit_publication':{'default_reads':[0xC8],
                            'all_writes':[0xA2,0x16A,0x171],
                            'normal_clamp_branch':[0x16A]}},
                       {'candidate_limit_publication':{'default_reads':[0xC8],
                            'all_writes':[0xA2,0x171],
                            'normal_clamp_branch':[0x16F]}},
                       {'candidate_limit_publication':{'default_reads':[0xC8],
                            'all_writes':[0xA2,0x171,0x16A],
                            'normal_clamp_branch':[0x16F]}},
                       {'aligned_field_identities':40}, {'field_change_spans':4},
                       {'aligned_instructions':94}, {'overlap_differences':225},
                       {'direct_graphs_agree':False}, {'exactness_credit':'accepted'},
                       {'overlap_differences':1}, {'overlap_differences':False},
                       {'difference_offsets':[424]}, {'missing_target_bytes':1},
                       {'source_linked_sha256':'0'*64}, {'complete_bytes_equal':False}):
            with self.subTest(change=change), patch.object(boss,'build'), patch.object(
                    boss,'canonical',return_value={'unit':'game-manager-on-update'}), patch.object(
                    boss,'parsed',side_effect=[self.evidence(),
                        {**self.cleanup_evidence(),**change}]), self.assertRaises(RuntimeError):
                boss.run_game_manager(protect_exact=False)


if __name__ == '__main__':
    unittest.main()
