"""No-target input-gate regressions for cold large-owner replay orchestration."""

import importlib.util
from pathlib import Path
from unittest import TestCase
from unittest.mock import patch

ROOT = Path(__file__).resolve().parents[1]
sp = importlib.util.spec_from_file_location('check_boss_frontiers',
                                            ROOT / 'scripts/check-boss-frontiers.py')
frontiers = importlib.util.module_from_spec(sp)
sp.loader.exec_module(frontiers)


class BossFrontierChecks(TestCase):
    def test_noncrediting_never_uses_claimed_acceptance(self):
        document = {'diagnostic_only': True, 'exactness_credit': 'none',
                    'target_bytes': 600, 'bound_fields': 27}
        self.assertIs(frontiers.noncrediting(document, name='test',
                                            target_size=600, fields=27), document)
        for change in ({'diagnostic_only': False}, {'exactness_credit': 'accepted'},
                       {'exactness_credit': ''}, {'target_bytes': 594},
                       {'bound_fields': 26}):
            with self.subTest(change=change), self.assertRaises(RuntimeError):
                frontiers.noncrediting({**document, **change}, name='test',
                                       target_size=600, fields=27)

    def test_exact_physical_extent_may_exceed_logical_authored_extent(self):
        for logical, physical in ((215, 232), (2195, 2216), (147, 147)):
            complete = {'result': 'exact', 'size': logical, 'matched_bytes': logical,
                        'compared_size': physical, 'matched_compared_bytes': physical,
                        'relocations': [{}, {}]}
            with self.subTest(logical=logical, physical=physical), patch.object(
                    frontiers, 'parsed', return_value=complete):
                result = frontiers.canonical('sample')
                self.assertEqual((result['logical_bytes'], result['physical_bytes']),
                                 (logical, physical))
                self.assertEqual(result['result'], 'exact')

    def test_reject_partial_or_missing_physical_match(self):
        full = {'result': 'exact', 'size': 215, 'matched_bytes': 215,
                'compared_size': 232, 'matched_compared_bytes': 232}
        for field, value in (('result', 'near'), ('matched_bytes', 214),
                             ('compared_size', 214), ('matched_compared_bytes', 231)):
            with self.subTest(field=field), patch.object(
                    frontiers, 'parsed', return_value={**full, field: value}), self.assertRaises(RuntimeError):
                frontiers.canonical('sample')

    def test_malformed_and_rejected_diagnostic_documents_fail_closed(self):
        for output in ('not-json', '[]', '{"error": "bad owner"}',
                       '{"result": "error", "message": "unknown target"}'):
            with self.subTest(output=output), patch.object(
                    frontiers, 'execute', return_value=output), self.assertRaises(RuntimeError):
                frontiers.parsed(['scripts/diagnostic.py'], diagnostic=True)

    @staticmethod
    def front_random_evidence():
        return {
            'diagnostic_only': True, 'exactness_credit': 'none',
            'target_bytes': 693, 'candidate_bytes': 681,
            'target_instructions': 206, 'candidate_instructions': 201,
            'original_fields': 15, 'bound_fields': 15,
            'ordered_calls': 6, 'ordered_calls_agree': True,
            'target_blocks': 40, 'candidate_blocks': 41,
            'direct_graphs_agree': False, 'aligned_instructions': 117,
            'overlap_differences': 572,
            'missing_target_bytes': 12, 'extra_candidate_bytes': 0,
            'complete_bytes_equal': False,
        }

    def test_front_random_genuine_cold_carrier_and_exact_caller(self):
        with patch.object(frontiers, 'build_nonexact') as compile_boss, patch.object(
                frontiers, 'build') as build, patch.object(
                frontiers, 'parsed', return_value=self.front_random_evidence()) as oracle, patch.object(
                frontiers, 'canonical', side_effect=lambda unit: {'unit': unit}):
            owners, exact = frontiers.run_front_random(protect_exact=True)
        compile_boss.assert_called_once_with('src/FrontMessageRandomSetup.cpp',
            'build/matching/FrontMessageRandomSetup.obj', reference_unit='front-message-setup')
        oracle.assert_called_once_with(['scripts/inspect-front-message-random.py',
            'build/matching/FrontMessageRandomSetup.obj', '--json'])
        self.assertEqual(owners['front_random_setup']['overlap_differences'], 572)
        self.assertEqual(owners['front_random_setup']['target_blocks'], 40)
        self.assertEqual(owners['front_random_setup']['candidate_blocks'], 41)
        self.assertEqual([x['unit'] for x in exact],
            ['front-calc-callback', 'front-message-setup',
             'front-message-load-for-side-count', 'front-message-initialize-runtime'])
        self.assertEqual(build.call_count, 4)

    def test_front_random_wrong_original_operand_or_codegen_fails_closed(self):
        wrong = ({'target_bytes': 692}, {'candidate_bytes': 680},
                 {'target_instructions': 205}, {'candidate_instructions': 200},
                 {'original_fields': 14}, {'bound_fields': 14},
                 {'ordered_calls': 5}, {'ordered_calls_agree': False},
                 {'target_blocks': 39}, {'candidate_blocks': 42},
                 {'direct_graphs_agree': True}, {'aligned_instructions': 116},
                 {'overlap_differences': 573}, {'missing_target_bytes': 13},
                 {'extra_candidate_bytes': 1}, {'complete_bytes_equal': True},
                 {'exactness_credit': 'accepted'}, {'diagnostic_only': False})
        for change in wrong:
            with self.subTest(change=change), patch.object(frontiers, 'build_nonexact'), patch.object(
                    frontiers, 'parsed', return_value={**self.front_random_evidence(), **change}), self.assertRaises(RuntimeError):
                frontiers.run_front_random(protect_exact=False)

    @staticmethod
    def player_boss_evidence():
        return {
            'diagnostic_only': True, 'exactness_credit': 'none',
            'target_bytes': 862, 'candidate_bytes': 849,
            'target_instructions': 277, 'candidate_instructions': 266,
            'original_fields': 26, 'bound_fields': 26,
            'ordered_calls': 12, 'ordered_calls_agree': True,
            'ordered_fields_agree': True,
            'original_blocks': 33, 'candidate_blocks': 32,
            'direct_graphs_agree': False,
            'overlap_differences': 804, 'missing_target_bytes': 13,
            'extra_candidate_bytes': 0, 'complete_bytes_equal': False,
        }

    def test_player_collision_boss_is_cold_bound_with_seven_exact_siblings(self):
        with patch.object(frontiers, 'build') as build, patch.object(
                frontiers, 'parsed', return_value=self.player_boss_evidence()) as oracle, patch.object(
                frontiers, 'canonical', side_effect=lambda unit: {'unit': unit}):
            owners, exact = frontiers.run_player_collision(protect_exact=True)
        self.assertEqual(owners['player_check_bullet_collision']['target_bytes'], 862)
        self.assertEqual(owners['player_check_bullet_collision']['overlap_differences'], 804)
        oracle.assert_called_once_with(['scripts/inspect-player-bullet-collision.py',
                                       'build/matching/PlayerCollisionHelpers.obj', '--json'])
        self.assertEqual([x['unit'] for x in exact],
                         ['player-find-collision', 'player-calc-item-box-collision',
                          'player-check-graze-collision', 'player-calc-laser-hitbox',
                          'player-calc-item-collection-collision', 'player-calc-circle-collision',
                          'player-find-collision-at-player'])
        self.assertEqual(build.call_count, 6)

    def test_player_collision_rejects_missing_original_fields_or_worse_layout(self):
        for change in ({'target_bytes': 861}, {'candidate_bytes': 848},
                       {'target_instructions': 276}, {'candidate_instructions': 265},
                       {'original_fields': 25}, {'bound_fields': 25},
                       {'ordered_calls': 11}, {'ordered_calls_agree': False},
                       {'ordered_fields_agree': False}, {'original_blocks': 32},
                       {'candidate_blocks': 31}, {'direct_graphs_agree': True},
                       {'overlap_differences': 805}, {'missing_target_bytes': 14},
                       {'extra_candidate_bytes': 1}, {'complete_bytes_equal': True},
                       {'diagnostic_only': False}, {'exactness_credit': 'accepted'}):
            with self.subTest(change=change), patch.object(frontiers, 'build'), patch.object(
                    frontiers, 'canonical', return_value={'unit': 'exact'}), patch.object(
                    frontiers, 'parsed', return_value={**self.player_boss_evidence(), **change}), self.assertRaises(RuntimeError):
                frontiers.run_player_collision(protect_exact=False)

    @staticmethod
    def enemy_evidence():
        main = {
            'diagnostic_only': True, 'exactness_credit': 'none',
            'target_sha256': frontiers.ENEMY_UPDATE_SHA256,
            'target_bytes': 3900, 'candidate_bytes': 3900,
            'code_bytes': 3883,
            'target_instructions': 1026, 'candidate_instructions': 1026,
            'complete_bytes_agree': False,
            'relocations': 97, 'relocation_fields_agree': 97,
            'target_calls': 69, 'candidate_calls': 69,
            'ordered_calls_agree': True, 'target_direct_blocks': 170,
            'candidate_direct_blocks': 170,
            'reviewed_control_flow_agrees': True,
            'switch_table_destinations_agree': True,
            'complete_differences': 39,
            'difference_offsets': [hex(v) for v in (
                0x19C, 0x1A3, 0x252, 0x263, 0x26D, 0x272, 0x294,
                0x29A, 0x29C, 0x2A5, 0x2A9, 0x2B7, 0x2BD, 0x845,
                0x846, 0x847, 0x849, 0x84A, 0x84C, 0x84D, 0x84E,
                0x84F, 0x850, 0x9C0, 0x9C6, 0x9DF, 0x9E2, 0x9EC,
                0x9F3, 0xA1F, 0xA23, 0xA29, 0xA2E, 0xA34, 0xA3A,
                0xA42, 0xA45, 0xA61, 0xA7E)],
        }
        leaf = {
            'diagnostic_only': True, 'exactness_credit': 'none',
            'target_sha256': frontiers.ENEMY_COLLISION_SHA256,
            'target_bytes': 82, 'candidate_bytes': 82,
            'bound_call_fields': 2, 'target_instructions': 30,
            'candidate_instructions': 30, 'complete_bytes_agree': False,
            'ordered_call_addresses': ['0x40f5a0', '0x40f8e0'],
            'ordinary_differing_bytes': 31,
            'difference_offsets': [hex(v) for v in (
                0x1C, 0x1D, 0x1E, 0x20, 0x21, 0x23, 0x24,
                0x25, 0x26, 0x27, 0x28, 0x29, 0x2A, 0x2B,
                0x2C, 0x2D, 0x2E, 0x2F, 0x30, 0x31, 0x32,
                0x33, 0x34, 0x35, 0x37, 0x38, 0x39, 0x3A,
                0x3B, 0x3D, 0x3E)],
        }
        return main, leaf

    def test_enemy_boss_and_leaf_require_separate_full_oracles(self):
        main, leaf = self.enemy_evidence()
        with patch.object(frontiers, 'build') as build, patch.object(
                frontiers, 'build_nonexact') as compile_leaf, patch.object(
                frontiers, 'parsed', side_effect=[main, leaf]), patch.object(
                frontiers, 'canonical', side_effect=lambda unit: {'unit': unit}):
            owners, exact = frontiers.run_enemy(protect_exact=True)
        self.assertEqual(owners['enemy_manager_update']['ordinary_differences'], 39)
        self.assertEqual(owners['enemy_collision_append']['ordinary_differences'], 31)
        self.assertTrue(owners['enemy_manager_update']['switch_destinations_agree'])
        self.assertEqual([x['unit'] for x in exact],
                         ['enemy-attached-effect-update', 'enemy-manager-draw-impl'])
        self.assertEqual(build.call_count, 2)
        compile_leaf.assert_called_once()

    def test_enemy_boss_input_rejects_false_original_call_identity(self):
        main, leaf = self.enemy_evidence()
        for corruption in ({'relocation_fields_agree': 96}, {'target_calls': 68},
                           {'candidate_bytes': 3899}, {'candidate_instructions': 1025},
                           {'complete_differences': 40}, {'complete_bytes_agree': True},
                           {'reviewed_control_flow_agrees': False},
                           {'switch_table_destinations_agree': False}):
            with self.subTest(corruption=corruption), patch.object(
                    frontiers, 'build'), patch.object(frontiers, 'parsed',
                    return_value={**main, **corruption}), self.assertRaises(RuntimeError):
                frontiers.run_enemy(protect_exact=False)
        with patch.object(frontiers, 'build'), patch.object(
                frontiers, 'build_nonexact'), patch.object(
                frontiers, 'parsed', side_effect=[main, {
                    **leaf, 'ordered_call_addresses': ['0x40f8e0', '0x40f5a0']
                }]), self.assertRaises(RuntimeError):
            frontiers.run_enemy(protect_exact=False)

    def test_enemy_original_digest_windows_and_count_fail_closed(self):
        main, leaf = self.enemy_evidence()
        for change in ({'target_sha256': '0' * 64},
                       {'code_bytes': 3882}, {'candidate_instructions': 1025},
                       {'relocation_fields_agree': 96},
                       {'difference_offsets': main['difference_offsets'][:-1]},
                       {'difference_offsets': main['difference_offsets'][:-1] + ['0x1AB']},
                       {'difference_offsets': ['0x1A3', '0x19C'] + main['difference_offsets'][2:]},
                       {'difference_offsets': ['0x19C'] * 39},
                       {'difference_offsets': ['garbage'] + main['difference_offsets'][1:]},
                       {'difference_offsets': ['0x19C', '0x1B0'] + main['difference_offsets'][2:]},
                       {'difference_offsets': ['0x19C', '0x19D', '0x1A3'] + main['difference_offsets'][2:-1]},
                       {'complete_bytes_agree': True},
                       {'complete_differences': 0},
                       {'exactness_credit': 'accepted'}):
            with self.subTest(change=change), self.assertRaises(RuntimeError):
                frontiers.guard_enemy_update({**main, **change})
        for change in ({'target_sha256': '0' * 64},
                       {'candidate_bytes': 83}, {'bound_call_fields': 1},
                       {'candidate_instructions': 29},
                       {'difference_offsets': leaf['difference_offsets'][:-1]},
                       {'difference_offsets': ['0x19'] + leaf['difference_offsets'][1:]},
                       {'difference_offsets': ['0x40'] + leaf['difference_offsets'][1:]},
                       {'difference_offsets': ['0x1c'] * 31},
                       {'ordinary_differing_bytes': 32},
                       {'complete_bytes_agree': True},
                       {'exactness_credit': 'accepted'}):
            with self.subTest(change=change), self.assertRaises(RuntimeError):
                frontiers.guard_enemy_collision({**leaf, **change})

    def test_enemy_valid_future_improvements_still_grant_no_exact_credit(self):
        main, leaf = self.enemy_evidence()
        for result, guard, key in (
                (main, frontiers.guard_enemy_update, 'complete_differences'),
                (leaf, frontiers.guard_enemy_collision, 'ordinary_differing_bytes')):
            improved = {**result, 'difference_offsets': result['difference_offsets'][1:],
                        key: result[key] - 1}
            self.assertEqual(guard(improved)['differences'], result[key] - 1)
            converged = {**result, 'difference_offsets': [], key: 0,
                         'complete_bytes_agree': True}
            self.assertEqual(guard(converged)['differences'], 0)
            self.assertEqual(converged['exactness_credit'], 'none')

    @staticmethod
    def type3_update_evidence():
        return {
            'diagnostic_only': True, 'exactness_credit': 'none',
            'target_sha256': frontiers.TYPE3_UPDATE_SHA256,
            'target_bytes': 780, 'candidate_bytes': 780,
            'target_instructions': 240, 'candidate_instructions': 240,
            'target_fields': 38, 'bound_fields': 38,
            'ordered_calls': 24, 'calls_agree': True,
            'target_blocks': 17, 'candidate_blocks': 17,
            'direct_graphs_agree': True,
            'original_z_order': dict(frontiers.TYPE3_MIDDLE_Z_ORIGINAL),
            'candidate_z_order': {'middle_y_call_offset': '0x195',
                                  'middle_z_store_offset': '0x18e',
                                  'tail_x_load_offset': '0x19d',
                                  'classification': 'zero-before-middle-y-call'},
            'ordinary_differences': 14,
            'difference_offsets': [hex(i) for i in range(0x18e, 0x19d)
                                   if i != 0x195],
            'complete_bytes_equal': False,
        }

    def test_type3_original_z_timing_and_complete_byte_frontier(self):
        baseline = self.type3_update_evidence()
        self.assertEqual(frontiers.guard_exattack_type3_update(baseline)['differences'], 14)
        for key, value in (
                ('target_sha256', '0' * 64), ('candidate_bytes', 781),
                ('candidate_instructions', 239), ('target_fields', 37),
                ('ordered_calls', 23), ('calls_agree', False),
                ('target_blocks', 16), ('candidate_blocks', 16),
                ('direct_graphs_agree', False),
                ('original_z_order', {'classification': 'matches-original-chronology'}),
                ('difference_offsets', baseline['difference_offsets'][:-1] + ['0x19d']),
                ('difference_offsets', baseline['difference_offsets'][:-1]),
                ('difference_offsets', ['0x18e'] * 14),
                ('difference_offsets', ['bad'] + baseline['difference_offsets'][1:]),
                ('difference_offsets', list(reversed(baseline['difference_offsets']))),
                ('difference_offsets', baseline['difference_offsets'] + ['0x1a0']),
                ('ordinary_differences', 13), ('complete_bytes_equal', True),
                ('exactness_credit', 'accepted')):
            with self.subTest(key=key, value=value), self.assertRaises(RuntimeError):
                frontiers.guard_exattack_type3_update({**baseline, key: value})
        for bad_class in ('zero-after-tail-x-load', 'unreviewed-order'):
            with self.subTest(bad_class=bad_class), self.assertRaises(RuntimeError):
                frontiers.guard_exattack_type3_update({**baseline,
                    'candidate_z_order': {**baseline['candidate_z_order'],
                                          'classification': bad_class}})

    def test_type3_valid_narrowing_never_credits_exact(self):
        baseline = self.type3_update_evidence()
        narrow = {**baseline, 'ordinary_differences': 13,
                  'difference_offsets': baseline['difference_offsets'][:-1]}
        self.assertEqual(frontiers.guard_exattack_type3_update(narrow)['differences'], 13)
        converged = {**baseline, 'ordinary_differences': 0,
                     'difference_offsets': [], 'complete_bytes_equal': True,
                     'candidate_z_order': {**baseline['candidate_z_order'],
                        'classification': 'matches-original-chronology'}}
        self.assertEqual(frontiers.guard_exattack_type3_update(converged)['differences'], 0)
        self.assertEqual(converged['exactness_credit'], 'none')
        with self.assertRaises(RuntimeError):
            frontiers.guard_exattack_type3_update({**converged,
                'candidate_z_order': baseline['candidate_z_order']})

    def test_type3_update_cold_true_tu_and_exact_initializer(self):
        baseline = self.type3_update_evidence()
        with patch.object(frontiers, 'build_nonexact') as compile_source, \
             patch.object(frontiers, 'parsed', return_value=baseline) as read, \
             patch.object(frontiers, 'build') as build, \
             patch.object(frontiers, 'canonical', return_value={'unit': 'exact'}):
            owners, exact = frontiers.run_exattack_type3(protect_exact=True)
        compile_source.assert_called_once_with('src/ExAttackUpdateType3.cpp',
            'build/matching/ExAttackUpdateType3.obj',
            reference_unit='ex-attack-initialize-type3')
        read.assert_called_once_with(['scripts/inspect-exattack-type3-schedule.py',
                                      'build/matching/ExAttackUpdateType3.obj'])
        build.assert_called_once_with('ex-attack-initialize-type3')
        self.assertEqual(owners['exattack_type3_update']['ordinary_differences'], 14)
        self.assertEqual([x['unit'] for x in exact], ['exact'])

    @staticmethod
    def screen_evidence():
        return {
            'diagnostic_only': True, 'exactness_credit': 'none',
            'target_bytes': 336, 'candidate_bytes': 336,
            'reviewed_target_fields': 20, 'bound_relocations': 20,
            'target_instructions': 132, 'candidate_instructions': 132,
            'target_calls': 8, 'candidate_calls': 8, 'ordered_calls_agree': True,
            'target_blocks': 26, 'candidate_blocks': 26,
            'complete_direct_graphs_agree': True,
            'overlapping_differences': 28,
            'absent_target_bytes': 0, 'extra_candidate_bytes': 0,
            'complete_bytes_agree': False,
        }

    def test_screen_shake_cold_carrier_protects_six_exact_siblings(self):
        with patch.object(frontiers, 'build') as build, patch.object(
                frontiers, 'parsed', return_value=self.screen_evidence()) as parsed, patch.object(
                frontiers, 'canonical', side_effect=lambda unit: {'unit': unit}):
            owners, exact = frontiers.run_screen_shake(protect_exact=True)
        build.assert_called_once_with('screen-effect-calc-shake')
        parsed.assert_called_once_with(['scripts/inspect-screen-shake-envelope.py',
                                        'build/matching/ScreenEffect.obj', '--json'])
        self.assertEqual(owners['screen_shake_envelope']['ordinary_differences'], 28)
        self.assertEqual([x['unit'] for x in exact],
                         ['screen-effect-calc-shake', 'screen-effect-calc-fade-in',
                          'screen-effect-calc-fade-out', 'screen-effect-calc-fade-hold',
                          'screen-effect-calc-arcade-pulse', 'screen-effect-initialize-timer'])

    def test_screen_shake_bad_extent_fields_calls_or_regression_fail_closed(self):
        for bad in ({'candidate_bytes': 334}, {'target_instructions': 131},
                    {'candidate_instructions': 133}, {'bound_relocations': 19},
                    {'reviewed_target_fields': 19}, {'target_calls': 7},
                    {'candidate_calls': 7}, {'ordered_calls_agree': False},
                    {'target_blocks': 25}, {'candidate_blocks': 27},
                    {'complete_direct_graphs_agree': False},
                    {'overlapping_differences': 29}, {'absent_target_bytes': 1},
                    {'extra_candidate_bytes': 1}, {'complete_bytes_agree': True}):
            with self.subTest(bad=bad), patch.object(frontiers, 'build'), patch.object(
                    frontiers, 'parsed', return_value={**self.screen_evidence(), **bad}), self.assertRaises(RuntimeError):
                frontiers.run_screen_shake(protect_exact=False)

    @staticmethod
    def etama_evidence():
        return {
            'diagnostic_only': True, 'exactness_credit': 'none',
            'target_bytes': 600, 'candidate_bytes': 600,
            'target_instructions': 157, 'candidate_instructions': 160,
            'bound_fields': 27, 'ordered_calls': 13,
            'target_direct_blocks': 17, 'candidate_direct_blocks': 17,
            'complete_direct_graphs_agree': True,
            'overlapping_byte_differences': 268,
            'missing_target_bytes': 0, 'extra_candidate_bytes': 0,
            'complete_bytes_equal': False,
        }

    def test_etama_big_draw_protects_two_real_exact_siblings(self):
        with patch.object(frontiers, 'build') as build, patch.object(
                frontiers, 'parsed', return_value=self.etama_evidence()), patch.object(
                frontiers, 'canonical', side_effect=lambda unit: {'unit': unit}):
            owners, siblings = frontiers.run_etama(protect_exact=True)
        self.assertEqual(owners['etama_draw']['overlapping_differences'], 268)
        self.assertEqual([r['unit'] for r in siblings],
                         ['bullet-draw-single', 'etama-controller-update'])
        self.assertEqual(build.call_count, 2)

    def test_etama_original_graph_register_schedule_regressions_fail(self):
        for bad in ({'candidate_bytes': 594}, {'target_instructions': 156},
                    {'candidate_instructions': 161}, {'ordered_calls': 12},
                    {'target_direct_blocks': 16}, {'candidate_direct_blocks': 16},
                    {'complete_direct_graphs_agree': False},
                    {'overlapping_byte_differences': 269},
                    {'missing_target_bytes': 1}, {'extra_candidate_bytes': 1},
                    {'complete_bytes_equal': True}):
            with self.subTest(bad=bad), patch.object(frontiers, 'build'), patch.object(
                    frontiers, 'parsed', return_value={**self.etama_evidence(), **bad}), self.assertRaises(RuntimeError):
                frontiers.run_etama(protect_exact=False)

    def test_trail_initializers_target_extent_calls_blocks_and_gap_guarded(self):
        for typ, size, ins, fields, calls, blocks, diff, begin, sha in (
                ('19', 763, 207, 57, 27, 9, 6, 0x1BB,
                 'fc7b630cff17c32048de02f169f5c39710727027941aa502db823e357563cf67'),
                ('21', 727, 198, 53, 25, 10, 9, 0x1C2,
                 'b5271179d169d482952d887b03ad08e10a19c74654590e32598551d0a523ca47')):
            evidence = {'diagnostic_only': True, 'exactness_credit': 'none',
                        'target_sha256': sha,
                        'target_bytes': size, 'candidate_bytes': size,
                        'target_instructions': ins, 'candidate_instructions': ins,
                        'normalized_aligned_instructions': ins - 1,
                        'relocations': fields, 'target_calls': calls,
                        'candidate_calls': calls, 'ordered_calls_agree': True,
                        'direct_blocks': [blocks, blocks],
                        'complete_direct_graphs_agree': True,
                        'difference_offsets': list(range(begin, begin + diff)),
                        'complete_differences_if_same_extent': diff,
                        'complete_bytes_agree': False}
            frontiers.validate_exattack_trail_initializer(typ, evidence)
            for bad in ({'candidate_bytes': size + 1},
                        {'candidate_instructions': ins - 1},
                        {'relocations': fields - 1},
                        {'target_calls': calls - 1},
                        {'candidate_calls': calls - 1},
                        {'ordered_calls_agree': False},
                        {'direct_blocks': [blocks, blocks - 1]},
                        {'complete_direct_graphs_agree': False},
                        {'complete_differences_if_same_extent': diff + 1},
                        {'difference_offsets': list(range(begin-1, begin+diff-1))},
                        {'difference_offsets': [begin] * diff},
                        {'difference_offsets': list(reversed(range(begin, begin+diff)))},
                        {'difference_offsets': list(range(begin, begin+diff-1))},
                        {'difference_offsets': list(range(begin+1, begin+diff+1))},
                        {'normalized_aligned_instructions': ins - 2},
                        {'target_sha256': '0' * 64},
                        {'diagnostic_only': False},
                        {'exactness_credit': 'accepted'},
                        {'complete_bytes_agree': True}):
                with self.subTest(typ=typ, bad=bad), self.assertRaises(RuntimeError):
                    frontiers.validate_exattack_trail_initializer(typ,
                                                                  {**evidence, **bad})
            # Even a hypothetical zero-residual diagnostic is NOT accepted exact.
            # It must nevertheless remain valid for a future real source fix.
            exact_looking = {**evidence, 'difference_offsets': [],
                             'complete_differences_if_same_extent': 0,
                             'normalized_aligned_instructions': ins,
                             'complete_bytes_agree': True}
            frontiers.validate_exattack_trail_initializer(typ, exact_looking)
            with self.subTest(typ=typ), self.assertRaises(RuntimeError):
                frontiers.validate_exattack_trail_initializer(
                    typ, {**exact_looking, 'exactness_credit': 'accepted'})
        with self.assertRaises(RuntimeError):
            frontiers.validate_exattack_trail_initializer('0', {})

    @staticmethod
    def network_parser_evidence():
        return {
            'diagnostic_only': True, 'exactness_credit': 'none',
            'target_bytes': 243, 'candidate_bytes': 243,
            'target_instructions': 116, 'candidate_instructions': 116,
            'original_fields': 1, 'bound_fields': 1,
            'ordered_calls': 1, 'ordered_calls_agree': True,
            'target_blocks': 33, 'candidate_blocks': 34,
            'direct_graphs_agree': False, 'overlap_differences': 76,
            'missing_target_bytes': 0, 'extra_candidate_bytes': 0,
            'complete_bytes_equal': False,
        }

    def test_network_parser_uses_true_679_byte_exact_caller_carrier(self):
        with patch.object(frontiers, 'build') as build, patch.object(
                frontiers, 'parsed', return_value=self.network_parser_evidence()) as read, patch.object(
                frontiers, 'canonical', side_effect=lambda unit: {'unit': unit}):
            owners, exact = frontiers.run_network_config_parser(protect_exact=True)
        build.assert_called_once_with('supervisor-network-config')
        read.assert_called_once_with(['scripts/inspect-network-config-parser.py',
                                     'build/matching/SupervisorNetworkConfig.obj', '--json'])
        self.assertEqual(owners['network_config_parser']['overlap_differences'], 76)
        self.assertEqual(owners['network_config_parser']['target_blocks'], 33)
        self.assertEqual(owners['network_config_parser']['candidate_blocks'], 34)
        self.assertEqual([item['unit'] for item in exact], ['supervisor-network-config'])

    def test_network_parser_rejects_wrong_extent_operand_graph_or_false_exact(self):
        for bad in ({'target_bytes': 242}, {'candidate_bytes': 244},
                    {'target_instructions': 115}, {'candidate_instructions': 115},
                    {'original_fields': 0}, {'bound_fields': 0},
                    {'ordered_calls': 0}, {'ordered_calls_agree': False},
                    {'target_blocks': 32}, {'candidate_blocks': 35},
                    {'direct_graphs_agree': True},
                    {'overlap_differences': 77}, {'missing_target_bytes': 1},
                    {'extra_candidate_bytes': 1}, {'complete_bytes_equal': True},
                    {'exactness_credit': 'accepted'}, {'diagnostic_only': False}):
            with self.subTest(bad=bad), patch.object(frontiers, 'build'), patch.object(
                    frontiers, 'parsed', return_value={**self.network_parser_evidence(), **bad}), self.assertRaises(RuntimeError):
                frontiers.run_network_config_parser(protect_exact=False)

    @staticmethod
    def replay_evidence():
        return {
            'diagnostic_only': True, 'exactness_credit': 'none',
            'target_bytes': 540, 'candidate_bytes': 538,
            'original_fields': 32, 'bound_fields': 32,
            'target_instructions': 152, 'candidate_instructions': 151,
            'ordered_calls': 5, 'ordered_calls_agree': True,
            'target_blocks': 12, 'candidate_blocks': 12,
            'complete_direct_graphs_agree': True,
            'aligned_field_identities': 31,
            'complete_overlap_differences': 49, 'missing_target_bytes': 2,
            'extra_source_bytes': 0, 'complete_bytes_equal': False,
        }

    def test_replay_playback_cold_pinned_no_eh_and_exact_siblings(self):
        with patch.object(frontiers, 'build') as build, patch.object(
                frontiers, 'parsed', return_value=self.replay_evidence()) as parsed, patch.object(
                frontiers, 'canonical', side_effect=lambda unit: {'unit': unit}):
            owners, siblings = frontiers.run_replay_playback(protect_exact=True)
        build.assert_called_once_with('replay-manager-begin-recording-stage')
        parsed.assert_called_once_with(['scripts/inspect-replay-begin-playback.py',
                                        'build/matching/ReplayInputNoEh.obj', '--json'])
        self.assertEqual(owners['replay_begin_playback']['ordinary_differences'], 49)
        self.assertEqual(owners['replay_begin_playback']['aligned_fields'], 31)
        self.assertEqual([x['unit'] for x in siblings],
                         ['replay-manager-begin-recording-stage', 'replay-manager-create'])

    def test_replay_playback_rejects_incomplete_original_and_worse_code(self):
        for corruption in ({'target_bytes': 539}, {'candidate_bytes': 537},
                           {'candidate_instructions': 150},
                           {'target_instructions': 151}, {'original_fields': 31},
                           {'bound_fields': 31}, {'ordered_calls': 4},
                           {'ordered_calls_agree': False},
                           {'target_blocks': 11}, {'candidate_blocks': 13},
                           {'complete_direct_graphs_agree': False},
                           {'aligned_field_identities': 30},
                           {'complete_overlap_differences': 50},
                           {'missing_target_bytes': 3},
                           {'extra_source_bytes': 1},
                           {'complete_bytes_equal': True}):
            with self.subTest(corruption=corruption), patch.object(frontiers, 'build'), patch.object(
                    frontiers, 'parsed', return_value={**self.replay_evidence(), **corruption}), self.assertRaises(RuntimeError):
                frontiers.run_replay_playback(protect_exact=False)

    @staticmethod
    def title_draw_result_evidence():
        return {
            'diagnostic_only': True, 'exactness_credit': 'none',
            'target_bytes': 1939, 'candidate_bytes': 1939,
            'target_instructions': 577, 'candidate_instructions': 577,
            'original_fields': 82, 'bound_fields': 82,
            'ordered_calls': 16, 'ordered_calls_agree': True,
            'original_blocks': 92, 'candidate_blocks': 92,
            'direct_graphs_agree': True,
            'aligned_instructions': 574, 'paired_branch_conflicts': [],
            'unpaired_branch_destinations': 1,
            'complete_differences': 22, 'complete_bytes_equal': False,
        }

    def test_title_draw_result_whole_owner_byte_frontier(self):
        frontiers.guard_title_draw_result(self.title_draw_result_evidence())
        for changes in ({'candidate_bytes': 1938},
                        {'candidate_instructions': 576},
                        {'target_instructions': 576},
                        {'bound_fields': 81}, {'original_fields': 81},
                        {'ordered_calls': 15}, {'ordered_calls_agree': False},
                        {'original_blocks': 91}, {'candidate_blocks': 91},
                        {'direct_graphs_agree': False},
                        {'aligned_instructions': 573},
                        {'paired_branch_conflicts': [{'branch': 4}]},
                        {'unpaired_branch_destinations': 2},
                        {'complete_differences': 23},
                        {'complete_bytes_equal': True},
                        {'exactness_credit': 'accepted'}):
            with self.subTest(changes=changes), self.assertRaises(RuntimeError):
                frontiers.guard_title_draw_result({**self.title_draw_result_evidence(), **changes})

    @staticmethod
    def camera_evidence():
        return {
            'diagnostic_only': True, 'exactness_credit': 'none',
            'target_bytes': 622, 'candidate_bytes': 622,
            'target_instructions': 201, 'candidate_instructions': 201,
            'original_fields': 40, 'bound_fields': 40,
            'ordered_calls': 11, 'calls_agree': True,
            'original_blocks': 8, 'candidate_blocks': 8,
            'direct_graphs_agree': True, 'overlap_differences': 24,
            'missing_target_bytes': 0,
        }

    def test_camera_replay_uses_render_carrier_and_protects_all_siblings(self):
        from unittest.mock import call
        with patch.object(frontiers, 'build') as build, patch.object(
                frontiers, 'parsed', return_value=self.camera_evidence()) as parsed, patch.object(
                frontiers, 'canonical', side_effect=lambda unit: {'unit': unit}):
            owners, exact = frontiers.run_camera_facing(protect_exact=True)
        self.assertEqual(owners['camera_facing_projection']['overlap_differences'], 24)
        self.assertEqual(owners['camera_facing_projection']['candidate_bytes'], 622)
        self.assertEqual(build.call_args_list,
                         [call('anm-manager-translate-rotation'),
                          call('anm-manager-project-3d-quad')])
        parsed.assert_called_once_with(['scripts/inspect-anm-camera-facing.py',
                                        'build/matching/AnmManagerRender.obj'])
        self.assertEqual([e['unit'] for e in exact],
                         ['anm-manager-translate-rotation', 'anm-manager-draw2d',
                          'anm-manager-draw-camera-facing-quad', 'anm-manager-project-3d-quad'])

    def test_camera_replay_rejects_shortened_owner_wrong_fields_or_regression(self):
        for bad in ({'candidate_bytes': 619}, {'candidate_instructions': 202},
                    {'original_fields': 39}, {'bound_fields': 39},
                    {'ordered_calls': 10}, {'calls_agree': False},
                    {'candidate_blocks': 7}, {'direct_graphs_agree': False},
                    {'overlap_differences': 25}):
            with self.subTest(bad=bad), patch.object(frontiers, 'build'), patch.object(
                    frontiers, 'parsed', return_value={**self.camera_evidence(), **bad}), self.assertRaises(RuntimeError):
                frontiers.run_camera_facing(protect_exact=False)

    def test_network_zero_field_owner_and_both_exact_consumers(self):
        evidence = {'diagnostic_only': True, 'exactness_credit': 'none',
                    'target_bytes': 213, 'candidate_bytes': 213,
                    'relocation_fields': 0,
                    'target_instructions': 67, 'candidate_instructions': 67,
                    'target_calls': 0, 'candidate_calls': 0,
                    'target_blocks': 9, 'candidate_blocks': 9,
                    'complete_direct_graphs_agree': True,
                    'complete_differences': 35}
        with patch.object(frontiers, 'build_nonexact') as nonexact, patch.object(
                frontiers, 'build') as build, patch.object(frontiers, 'parsed',
                return_value=evidence), patch.object(frontiers, 'canonical',
                side_effect=lambda unit: {'unit': unit}):
            owners, exact = frontiers.run_received_frames(protect_exact=True)
        self.assertEqual(owners['supervisor_received_frame']['ordinary_differences'], 35)
        self.assertEqual([e['unit'] for e in exact],
                         ['supervisor-frame-queue-insert-predicted',
                          'supervisor-frame-queue-pop-frame'])
        nonexact.assert_called_once()
        self.assertEqual(build.call_count, 2)
        for incorrect in ({'relocation_fields': 1}, {'target_calls': 1},
                          {'target_instructions': 66}, {'candidate_instructions': 68},
                          {'complete_direct_graphs_agree': False}):
            with self.subTest(incorrect=incorrect), patch.object(frontiers, 'build_nonexact'), patch.object(
                    frontiers, 'parsed', return_value={**evidence, **incorrect}), self.assertRaises(RuntimeError):
                frontiers.run_received_frames(protect_exact=False)

    def test_title_protects_four_independent_exact_units(self):
        checked = []
        with patch.object(frontiers, 'build') as build, patch.object(
                frontiers, 'canonical', side_effect=lambda unit: checked.append(unit) or {'unit': unit}):
            owners, exact = frontiers.run_title(protect_exact=True)
        self.assertEqual(owners, {})
        self.assertEqual([row['unit'] for row in exact],
                         ['title-screen-setup-thread', 'title-screen-music-room',
                          'title-screen-draw-music-room', 'title-draw-callback'])
        self.assertEqual(build.call_count, 3)
        self.assertEqual(checked, [row['unit'] for row in exact])

    def test_target_independent_title_probe_does_not_invent_receipts(self):
        with patch.object(frontiers, 'build') as build, patch.object(
                frontiers, 'canonical') as canonical:
            self.assertEqual(frontiers.run_title(protect_exact=False), ({}, []))
        build.assert_not_called()
        canonical.assert_not_called()
