"""No-target input-gate regressions for cold large-owner replay orchestration."""

import copy
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
            'direct_graphs_agree': False, 'aligned_instructions': 118,
            'overlap_differences': 569,
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
        self.assertEqual(owners['front_random_setup']['overlap_differences'], 569)
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
                 {'direct_graphs_agree': True}, {'aligned_instructions': 117},
                 {'overlap_differences': 570}, {'missing_target_bytes': 13},
                 {'extra_candidate_bytes': 1}, {'complete_bytes_equal': True},
                 {'exactness_credit': 'accepted'}, {'diagnostic_only': False})
        for change in wrong:
            with self.subTest(change=change), patch.object(frontiers, 'build_nonexact'), patch.object(
                    frontiers, 'parsed', return_value={**self.front_random_evidence(), **change}), self.assertRaises(RuntimeError):
                frontiers.run_front_random(protect_exact=False)

    @staticmethod
    def player_movement_evidence():
        # Pure target-independent shape, not a fabricated exactness receipt.
        return {
            'diagnostic_only': True, 'exactness_credit': 'none',
            'target_code_bytes': 1835, 'target_physical_bytes': 1900,
            'candidate_code_bytes': 1835, 'candidate_physical_bytes': 1900,
            'candidate_post_return_alignment_bytes': 1,
            'candidate_table_bytes': 64, 'target_instructions': 489,
            'candidate_instructions': 489, 'relocations': 66,
            'internal_relocations': 18, 'direct_calls': 23,
            'direct_call_order_agrees': True,
            'candidate_operand_fields_cover_actual_records': True,
            'field_identity_occurrences_agree': True,
            'normalized_aligned_instructions': 433,
            'target_extent_sha256': frontiers.PLAYER_MOVEMENT_TARGET_SHA,
            'full_difference_count': 129,
            'difference_offsets': sorted(frontiers.PLAYER_MOVEMENT_ORIGINAL_RESIDUAL_OFFSETS),
            'missing_target_bytes': 0, 'extra_candidate_bytes': 0,
            'full_byte_agreement': False,
            'target_fields': [{'offset': 0x31D, 'type': 'REL32',
                               'effective_destination': 0x00406790}],
            'actual_resolved_fields': [{'offset': 0x31D, 'type': 'REL32',
                'symbol': frontiers.PLAYER_MOVEMENT_INTERRUPT_SYMBOL, 'addend': 0,
                'destination': 0x00406790, 'effective_destination': 0x00406790,
                'owner_local': False, 'symbol_section': 0}],
        }

    def test_player_movement_cold_full_owner_and_exact_caller(self):
        evidence = self.player_movement_evidence()
        with patch.object(frontiers, 'build_nonexact') as compile_source, patch.object(
                frontiers, 'parsed', return_value=evidence) as inspect, patch.object(
                frontiers, 'build') as build, patch.object(
                frontiers, 'canonical', side_effect=lambda name: {'unit': name, 'result': 'exact'}):
            owners, exact = frontiers.run_player_movement(protect_exact=True)
        compile_source.assert_called_once_with('src/PlayerMovement.cpp',
                                                'build/matching/PlayerMovement.obj',
                                                reference_unit='player-on-update')
        inspect.assert_called_once_with(['scripts/inspect-player-movement.py',
                                         'build/matching/PlayerMovement.obj'])
        self.assertEqual([entry.args[0] for entry in build.call_args_list],
                         ['player-on-update', 'anm-vm-set-interrupt'])
        self.assertEqual(owners['player_movement']['ordinary_differences'], 129)
        self.assertEqual(owners['player_movement']['aligned_instructions'], 433)
        self.assertEqual(exact[0]['unit'], 'player-on-update')
        self.assertEqual(exact[1]['unit'], 'anm-vm-set-interrupt')

    def test_player_movement_original_extent_fields_and_differences_fail_closed(self):
        evidence = self.player_movement_evidence()
        corruptions = (
            {'diagnostic_only': False}, {'exactness_credit': 'accepted'},
            {'target_extent_sha256': '0'*64}, {'target_code_bytes': 1834},
            {'target_physical_bytes': 1899}, {'candidate_code_bytes': 1834},
            {'candidate_physical_bytes': 1901},
            {'candidate_post_return_alignment_bytes': 2},
            {'candidate_table_bytes': 63}, {'target_instructions': 488},
            {'candidate_instructions': 490}, {'relocations': 65},
            {'internal_relocations': 17}, {'direct_calls': 22},
            {'direct_call_order_agrees': False},
            {'candidate_operand_fields_cover_actual_records': False},
            {'field_identity_occurrences_agree': False},
            {'normalized_aligned_instructions': 432},
            {'full_difference_count': 130}, {'full_difference_count': 128},
            {'difference_offsets': list(range(130))},
            {'difference_offsets': [1]*129},
            {'difference_offsets': list(reversed(range(129)))},
            {'difference_offsets': list(range(128)) + [1900]},
            {'difference_offsets': ['0'] + list(range(1,129))},
            {'missing_target_bytes': 1}, {'extra_candidate_bytes': 1},
            {'full_byte_agreement': True},
        )
        for bad in corruptions:
            with self.subTest(bad=bad), self.assertRaises(RuntimeError):
                frontiers.guard_player_movement({**evidence, **bad})

    def test_player_movement_rejects_new_bad_bytes_inside_and_outside_old_spans(self):
        evidence = self.player_movement_evidence()
        known = set(evidence['difference_offsets'])
        self.assertEqual(len(known), 129)
        self.assertEqual(known, frontiers.PLAYER_MOVEMENT_ORIGINAL_RESIDUAL_OFFSETS)
        self.assertTrue(all(x < 1835 for x in known))
        for original, poisoned in ((0x127, 0x126), (0x186, 0x189),
                                   (0x533, 0x532), (0x569, 0x56A),
                                   (0x5B5, 0x5BF), (0x5BE, 0x5BF),
                                   (0x533, 1835), (0x533, 1899)):
            self.assertIn(original, known)
            self.assertNotIn(poisoned, known)
            with self.subTest(poisoned=hex(poisoned)), self.assertRaisesRegex(
                    RuntimeError, '129-byte frontier regressed'):
                moved = sorted((known - {original}) | {poisoned})
                # The same 129 count, all 66 original-bound operands, all
                # 23 calls, and the complete physical extent are unchanged.
                frontiers.guard_player_movement({**evidence,
                                                  'difference_offsets': moved})

    def test_player_movement_future_convergence_is_still_noncrediting(self):
        evidence = self.player_movement_evidence()
        smaller = {**evidence, 'difference_offsets': evidence['difference_offsets'][1:],
                   'full_difference_count': 128}
        self.assertEqual(frontiers.guard_player_movement(smaller)['ordinary_differences'], 128)
        converged = {**evidence, 'difference_offsets': [],
                     'full_difference_count': 0, 'full_byte_agreement': True,
                     'normalized_aligned_instructions': 489}
        self.assertEqual(frontiers.guard_player_movement(converged)['ordinary_differences'], 0)
        self.assertEqual(converged['exactness_credit'], 'none')

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
            'complete_differences': 29,
            'difference_offsets': [hex(v) for v in (
                0x19C, 0x1A3, 0x252, 0x263, 0x26D, 0x272, 0x294,
                0x29A, 0x29C, 0x2A5, 0x2A9, 0x2B7, 0x2BD,
                0x9C0, 0x9C6, 0x9DF, 0x9E2, 0x9EC,
                0x9F3, 0xA1F, 0xA23, 0xA29, 0xA2E, 0xA34, 0xA3A,
                0xA42, 0xA45, 0xA61, 0xA7E)],
        }
        leaf = {
            'diagnostic_only': True, 'exactness_credit': 'none',
            'target_sha256': frontiers.ENEMY_COLLISION_SHA256,
            'target_bytes': 82, 'candidate_bytes': 82,
            'bound_call_fields': 2, 'target_instructions': 30,
            'candidate_instructions': 30, 'complete_bytes_agree': True,
            'ordered_call_addresses': ['0x40f5a0', '0x40f8e0'],
            'ordinary_differing_bytes': 0,
            'difference_offsets': [],
        }
        return main, leaf

    def test_enemy_boss_and_leaf_require_separate_full_oracles(self):
        main, leaf = self.enemy_evidence()
        with patch.object(frontiers, 'build') as build, patch.object(
                frontiers, 'build_nonexact') as compile_leaf, patch.object(
                frontiers, 'parsed', side_effect=[main, leaf]), patch.object(
                frontiers, 'canonical', side_effect=lambda unit: {'unit': unit}), patch.object(
                frontiers, 'cold_replay_exact_source', return_value=[{'unit': name} for name in (
                    'enemy-attached-effect-update', 'float3-divide',
                    'player-collision-append-box', 'enemy-collision-append-box')]) as shared:
            owners, exact = frontiers.run_enemy(protect_exact=True)
        self.assertEqual(owners['enemy_manager_update']['ordinary_differences'], 29)
        self.assertEqual(owners['enemy_collision_append']['ordinary_differences'], 0)
        self.assertTrue(owners['enemy_manager_update']['switch_destinations_agree'])
        self.assertEqual([x['unit'] for x in exact],
                         ['enemy-attached-effect-update', 'float3-divide',
                          'player-collision-append-box', 'enemy-collision-append-box',
                          'enemy-manager-draw-impl',
                          'bullet-spawn-descriptor-constructor',
                          'bullet-pattern-primary',
                          'soundplayer-play-positioned-sound-by-idx',
                          'anm-manager-execute-script',
                          'ascii-boss-marker-set-position',
                          'ascii-boss-marker-set-interrupt',
                          'ascii-manager-set-boss-marker-state'])
        self.assertEqual(build.call_count, 8)
        shared.assert_called_once_with(
            'src/EclManager.cpp', frontiers.ECL_EXACT_CARRIERS,
            already_built=('build/matching/EclManager.obj',))
        compile_leaf.assert_not_called()

    def test_enemy_all_real_callee_carriers_are_cold_replayed_once(self):
        # The true AsciiBossMarkerVm two-function carrier is compiled ONCE;
        # ANM ExecuteScript must keep its separate target-matching /Ob0 TU.
        from pathlib import Path
        import tomllib
        with (ROOT / 'config/match-units.toml').open('rb') as file:
            units = tomllib.load(file)['units']
        cases = frontiers.ENEMY_REAL_CALLEE_EXACT_CARRIERS
        self.assertEqual([len(group) for _, group in cases], [1, 1, 2, 1])
        self.assertEqual(len({units[group[0][0]]['object']
                              for _, group in cases}), 4)
        self.assertIn('/Ob0', units['anm-manager-execute-script']['profile'])
        with patch.object(frontiers, 'build') as build, patch.object(
                frontiers, 'canonical', side_effect=lambda unit: {'unit': unit}):
            result = frontiers.replay_enemy_real_callee_exact()
        self.assertEqual([r['unit'] for r in result], [
            'soundplayer-play-positioned-sound-by-idx',
            'anm-manager-execute-script',
            'ascii-boss-marker-set-position',
            'ascii-boss-marker-set-interrupt',
            'ascii-manager-set-boss-marker-state'])
        self.assertEqual([call.args[0] for call in build.call_args_list], [
            'soundplayer-play-positioned-sound-by-idx',
            'anm-manager-execute-script',
            'ascii-boss-marker-set-position',
            'ascii-manager-set-boss-marker-state'])

    def test_enemy_real_exact_callee_missing_swapped_or_split_rejected(self):
        import tomllib
        with (ROOT / 'config/match-units.toml').open('rb') as file:
            manifest = tomllib.load(file)['units']
        wrongs = (
            ('anm-manager-execute-script', 'target_address', 0x00436F31),
            ('anm-manager-execute-script', 'symbol', '?Faked@@QAEHXZ'),
            ('anm-manager-execute-script', 'source', 'src/Fake.cpp'),
            ('anm-manager-execute-script', 'object', '../outside.obj'),
            ('ascii-boss-marker-set-interrupt', 'object', 'build/matching/Wrong.obj'),
            ('ascii-boss-marker-set-interrupt', 'profile', ['/Ob0']),
            ('soundplayer-play-positioned-sound-by-idx', 'target_address', 0x0043E381),
        )
        for name, field, wrong_value in wrongs:
            bad = copy.deepcopy(manifest)
            bad[name][field] = wrong_value
            with self.subTest(name=name, field=field), patch.object(
                    frontiers.tomllib, 'load', return_value={'units': bad}), patch.object(
                    frontiers, 'build'), patch.object(frontiers, 'canonical'), self.assertRaises(RuntimeError):
                frontiers.replay_enemy_real_callee_exact()
        bad = copy.deepcopy(manifest)
        del bad['ascii-boss-marker-set-position']
        with patch.object(frontiers.tomllib, 'load', return_value={'units': bad}), patch.object(
                frontiers, 'build'), patch.object(frontiers, 'canonical'), self.assertRaises(RuntimeError):
            frontiers.replay_enemy_real_callee_exact()

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
                       {'difference_offsets': sorted(main['difference_offsets'][1:] + ['0x845'], key=lambda x: int(x, 16))},
                       {'complete_bytes_agree': True},
                       {'complete_differences': 0},
                       {'exactness_credit': 'accepted'}):
            with self.subTest(change=change), self.assertRaises(RuntimeError):
                frontiers.guard_enemy_update({**main, **change})
        for change in ({'target_sha256': '0' * 64},
                       {'candidate_bytes': 83}, {'bound_call_fields': 1},
                       {'candidate_instructions': 29},
                       {'difference_offsets': ['0x1c']},
                       {'difference_offsets': ['0x19']},
                       {'difference_offsets': ['0x40']},
                       {'difference_offsets': ['0x1c', '0x1c']},
                       {'ordinary_differing_bytes': 1},
                       {'complete_bytes_agree': False},
                       {'exactness_credit': 'accepted'}):
            with self.subTest(change=change), self.assertRaises(RuntimeError):
                frontiers.guard_enemy_collision({**leaf, **change})

    def test_enemy_preserves_matched_bytes_even_inside_known_bad_register_spans(self):
        baseline, _ = self.enemy_evidence()
        self.assertEqual(len(frontiers.ENEMY_UPDATE_ORIGINAL_RESIDUAL_OFFSETS), 29)
        self.assertEqual({int(s, 16) for s in baseline['difference_offsets']},
                         frontiers.ENEMY_UPDATE_ORIGINAL_RESIDUAL_OFFSETS)
        # The previous three broad windows allow each poisoned offset, even
        # though they were matching code bytes in the verified original.
        for known, introduced in ((0x19C, 0x19D), (0x1A3, 0x1A4),
                                  (0x252, 0x253), (0x294, 0x295),
                                  (0x9C0, 0x9C1), (0xA7E, 0xA7D)):
            self.assertNotIn(introduced,
                             frontiers.ENEMY_UPDATE_ORIGINAL_RESIDUAL_OFFSETS)
            relocated = sorted(
                [int(x, 16) for x in baseline['difference_offsets']
                 if int(x, 16) != known] + [introduced])
            with self.subTest(introduced=hex(introduced)), self.assertRaisesRegex(
                    RuntimeError, 'already matched original physical byte'):
                frontiers.guard_enemy_update({**baseline,
                    'difference_offsets': [hex(x) for x in relocated]})

    def test_enemy_valid_future_improvements_still_grant_no_exact_credit(self):
        main, leaf = self.enemy_evidence()
        improved = {**main, 'difference_offsets': main['difference_offsets'][1:],
                    'complete_differences': main['complete_differences'] - 1}
        self.assertEqual(frontiers.guard_enemy_update(improved)['differences'], 28)
        converged = {**main, 'difference_offsets': [], 'complete_differences': 0,
                     'complete_bytes_agree': True}
        self.assertEqual(frontiers.guard_enemy_update(converged)['differences'], 0)
        self.assertEqual(frontiers.guard_enemy_collision(leaf)['differences'], 0)
        # A noncrediting diagnostic alone never grants new exact credit; the
        # four owned exact functions require canonical comparison in run_enemy.
        self.assertEqual(leaf['exactness_credit'], 'none')

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
            'candidate_z_order': {'middle_y_call_offset': '0x18e',
                                  'middle_z_store_offset': '0x1a2',
                                  'tail_x_load_offset': '0x196',
                                  'classification': 'zero-after-tail-x-load'},
            'ordinary_differences': 15,
            'difference_offsets': [hex(i) for i in
                                   (0x196, 0x197, 0x198, 0x199, 0x19c,
                                    0x19d, 0x19e, 0x19f, 0x1a0, 0x1a2,
                                    0x1a3, 0x1a4, 0x1a5, 0x1a6, 0x1a7)],
            'complete_bytes_equal': False,
        }

    def test_type3_original_z_timing_and_complete_byte_frontier(self):
        baseline = self.type3_update_evidence()
        self.assertEqual(frontiers.guard_exattack_type3_update(baseline)['differences'], 15)
        for key, value in (
                ('target_sha256', '0' * 64), ('candidate_bytes', 781),
                ('candidate_instructions', 239), ('target_fields', 37),
                ('ordered_calls', 23), ('calls_agree', False),
                ('target_blocks', 16), ('candidate_blocks', 16),
                ('direct_graphs_agree', False),
                ('original_z_order', {'classification': 'matches-original-chronology'}),
                ('difference_offsets', baseline['difference_offsets'][:-1] + ['0x1a8']),
                ('difference_offsets', baseline['difference_offsets'][:-1]),
                ('difference_offsets', ['0x196'] * 15),
                ('difference_offsets', ['bad'] + baseline['difference_offsets'][1:]),
                ('difference_offsets', list(reversed(baseline['difference_offsets']))),
                ('difference_offsets', baseline['difference_offsets'] + ['0x1a8']),
                ('ordinary_differences', 14), ('complete_bytes_equal', True),
                ('exactness_credit', 'accepted')):
            with self.subTest(key=key, value=value), self.assertRaises(RuntimeError):
                frontiers.guard_exattack_type3_update({**baseline, key: value})
        for bad_class in ('zero-before-middle-y-call', 'unreviewed-order'):
            with self.subTest(bad_class=bad_class), self.assertRaises(RuntimeError):
                frontiers.guard_exattack_type3_update({**baseline,
                    'candidate_z_order': {**baseline['candidate_z_order'],
                                          'classification': bad_class}})
        old_early = {'middle_y_call_offset': '0x195',
                     'middle_z_store_offset': '0x18e',
                     'tail_x_load_offset': '0x19d',
                     'classification': 'zero-before-middle-y-call'}
        with self.assertRaises(RuntimeError):
            frontiers.guard_exattack_type3_update({**baseline,
                'candidate_z_order': old_early})
        for field, replacement in (
                ('middle_y_call_offset', '0x18f'),
                ('middle_z_store_offset', '0x190'),
                ('tail_x_load_offset', '0x1a9'),
                ('middle_z_store_offset', 'invalid')):
            with self.subTest(field=field, replacement=replacement), \
                 self.assertRaises(RuntimeError):
                frontiers.guard_exattack_type3_update({**baseline,
                    'candidate_z_order': {**baseline['candidate_z_order'],
                                          field: replacement}})


    def test_type3_exact_residual_set_rejects_equal_count_new_bad_bytes(self):
        baseline = self.type3_update_evidence()
        self.assertEqual(len(frontiers.TYPE3_UPDATE_ORIGINAL_RESIDUAL_OFFSETS), 15)
        self.assertEqual({int(s, 16) for s in baseline['difference_offsets']},
                         frontiers.TYPE3_UPDATE_ORIGINAL_RESIDUAL_OFFSETS)
        # Every poisoned position lies WITHIN the previously allowed 18-byte
        # window; the real original PE/COFF proves these three bytes matched.
        for old, bad in ((0x196, 0x19A), (0x19D, 0x19B), (0x1A7, 0x1A1)):
            self.assertNotIn(bad, frontiers.TYPE3_UPDATE_ORIGINAL_RESIDUAL_OFFSETS)
            offsets = sorted((set(frontiers.TYPE3_UPDATE_ORIGINAL_RESIDUAL_OFFSETS)
                              - {old}) | {bad})
            with self.subTest(bad=hex(bad)), self.assertRaisesRegex(
                    RuntimeError, 'already matched original byte'):
                frontiers.guard_exattack_type3_update({**baseline,
                    'difference_offsets': [hex(x) for x in offsets]})

    def test_type3_valid_narrowing_never_credits_exact(self):
        baseline = self.type3_update_evidence()
        narrow = {**baseline, 'ordinary_differences': 14,
                  'difference_offsets': baseline['difference_offsets'][:-1]}
        self.assertEqual(frontiers.guard_exattack_type3_update(narrow)['differences'], 14)
        converged = {**baseline, 'ordinary_differences': 0,
                     'difference_offsets': [], 'complete_bytes_equal': True,
                     'candidate_z_order': dict(frontiers.TYPE3_MIDDLE_Z_ORIGINAL)}
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
        self.assertEqual([r.args[0] for r in build.call_args_list], [
            'ex-attack-initialize-type3',
            'game-manager-transform-popup-x',
            'exattack-type4-initialize',
            'anm-vm-set-interrupt'])
        self.assertEqual(build.call_count, 4)
        self.assertEqual(owners['exattack_type3_update']['ordinary_differences'], 15)
        # Six real exact GameManager/Type4/AnmVm owners add regression checks,
        # NOT new canonical exact credit to the still-unmatched Type3 update.
        self.assertEqual(len(exact), 7)
        self.assertTrue(all(row['unit'] == 'exact' for row in exact))

    @staticmethod
    def exattack_1422_draw_evidence():
        # The real SHA and byte/field/call/jump counts are independently
        # decoded from retail TH09; the fixture remains target-independent.
        return {
            'diagnostic_only': True, 'exactness_credit': 'none',
            'target_sha256': frontiers.EXATTACK_1422_DRAW_SHA256,
            'target_bytes': 959, 'candidate_bytes': 974,
            'target_instructions': 290, 'candidate_instructions': 297,
            'target_fields': 53, 'candidate_fields': 53,
            'target_COM_calls': [dict(slot=slot, reverse_arguments=list(args),
                                     receiver_address=0x004B3108) for slot, args in (
                (0xFC, (2, 4, 0)), (0xFC, (2, 1, 0)),
                (0xFC, (0, 5, 0)), (0xFC, (0, 2, 0)),
                (0xC8, (0, 14)), (0xC8, (6, 20)),
                (0x130, (0x44,)), (0x120, (20, 'dynamic', 31, 3)),
                (0xFC, (4, 4, 0)), (0xFC, (4, 1, 0)),
                (0xFC, (2, 5, 0)), (0xFC, (2, 2, 0)))],
            'candidate_COM_calls': [dict(slot=slot, reverse_arguments=list(args),
                                        receiver_address=0x004B3108) for slot, args in (
                (0xFC, (2, 4, 0)), (0xFC, (2, 1, 0)),
                (0xFC, (0, 5, 0)), (0xFC, (0, 2, 0)),
                (0xC8, (0, 14)), (0xC8, (6, 20)),
                (0x130, (0x44,)), (0x120, (20, 'dynamic', 31, 3)),
                (0xFC, (4, 4, 0)), (0xFC, (4, 1, 0)),
                (0xFC, (2, 5, 0)), (0xFC, (2, 2, 0)))],
            'direct_calls': 20, 'indirect_calls': 12,
            'target_direct_jumps': 9, 'candidate_direct_jumps': 9,
            'field_order_and_destinations_equal': True,
            'ordered_direct_calls_equal': True,
            'overlapping_differences': 905,
            'difference_offsets': [i for i in range(959) if
                frontiers.EXATTACK_1422_DRAW_ORIGINAL_RESIDUAL_MASK[i // 8] &
                (1 << (i % 8))],
            'absent_target_bytes': 0, 'extra_candidate_bytes': 15,
            'first_differences': [hex(x) for x in
                                  (0x5, 0x7, 0x8, 0x9, 0xB, 0xE, 0x10, 0x11,
                                   0x12, 0x13, 0x14, 0x15, 0x16, 0x17,
                                   0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D,
                                   0x1E, 0x1F, 0x20, 0x21)],
            'complete_bytes_equal': False,
        }

    def test_exattack_1422_draw_cold_original_owner_and_three_exact_siblings(self):
        evidence = self.exattack_1422_draw_evidence()
        with patch.object(frontiers, 'build_nonexact') as compile_source, \
             patch.object(frontiers, 'parsed', return_value=evidence) as read, \
             patch.object(frontiers, 'build') as build, \
             patch.object(frontiers, 'canonical', side_effect=lambda unit: {'unit': unit}):
            owners, siblings = frontiers.run_exattack_type1422_draw(protect_exact=True)
        compile_source.assert_called_once_with(
            'src/ExAttackDrawType14Type22.cpp',
            'build/matching/ExAttackDrawType14Type22.obj',
            reference_unit='exattack-type14-init')
        read.assert_called_once_with(
            ['scripts/inspect-exattack-type14-22-draw.py',
             'build/matching/ExAttackDrawType14Type22.obj', '--json'], diagnostic=True)
        self.assertEqual(owners['exattack_type14_22_draw']['target_bytes'], 959)
        self.assertEqual(owners['exattack_type14_22_draw']['extra_candidate_bytes'], 15)
        self.assertEqual(build.call_count, 3)
        self.assertEqual([x['unit'] for x in siblings],
                         list(frontiers.EXATTACK_1422_DRAW_EXACT_FAMILY))

    def test_exattack_1422_draw_wrong_extent_fields_calls_graph_or_credit_fails(self):
        baseline = self.exattack_1422_draw_evidence()
        bad = (
            {'target_sha256': '0'*64}, {'candidate_bytes': 975},
            {'target_instructions': 289}, {'candidate_instructions': 298},
            {'target_fields': 52}, {'candidate_fields': 52},
            {'target_COM_calls': None}, {'candidate_COM_calls': []},
            {'direct_calls': 19}, {'indirect_calls': 11},
            {'target_direct_jumps': 8}, {'candidate_direct_jumps': 10},
            {'field_order_and_destinations_equal': False},
            {'ordered_direct_calls_equal': False},
            {'overlapping_differences': 906},
            {'absent_target_bytes': 1}, {'extra_candidate_bytes': 14},
            {'complete_bytes_equal': True},
            {'first_differences': ['0x3'] + baseline['first_differences'][:-1]},
            {'difference_offsets': baseline['difference_offsets'][1:] + [1000]},
            {'difference_offsets': baseline['difference_offsets'][:-1]},
            {'difference_offsets': list(reversed(baseline['difference_offsets']))},
            {'difference_offsets': baseline['difference_offsets'][:1] * 905},
            {'first_differences': baseline['first_differences'][:-1]},
            {'first_differences': baseline['first_differences'][::-1]},
            {'first_differences': baseline['first_differences'][:-1] + ['0x3c1']},
            {'difference_offsets': [True] + baseline['difference_offsets'][1:]},
            {'difference_offsets': baseline['difference_offsets'][:-1] + [959]},
            {'diagnostic_only': False}, {'exactness_credit': 'accepted'},
        )
        for mutation in bad:
            with self.subTest(mutation=mutation), self.assertRaises(RuntimeError):
                frontiers.guard_exattack_type1422_draw({**baseline, **mutation})

    def test_exattack_1422_draw_rejects_com_argument_or_device_forgery(self):
        import copy
        baseline = self.exattack_1422_draw_evidence()
        for key in ('target_COM_calls', 'candidate_COM_calls'):
            for change in ('slot', 'argument', 'device', 'pointer', 'order'):
                evidence = copy.deepcopy(baseline)
                rows = evidence[key]
                if change == 'slot': rows[0]['slot'] = 0xF8
                elif change == 'argument': rows[0]['reverse_arguments'][0] = 4
                elif change == 'device': rows[0]['receiver_address'] += 4
                elif change == 'pointer': rows[7]['reverse_arguments'][1] = 0
                else: rows[0], rows[1] = rows[1], rows[0]
                with self.subTest(key=key, change=change), self.assertRaises(RuntimeError):
                    frontiers.guard_exattack_type1422_draw(evidence)

    def test_exattack_1422_draw_cannot_exchange_a_real_difference_for_a_matching_byte(self):
        baseline = self.exattack_1422_draw_evidence()
        known = set(baseline['difference_offsets'])
        self.assertEqual(len(known), 905)
        self.assertEqual(959 - len(known), 54)
        first = baseline['first_differences']
        self.assertTrue(first)
        self.assertEqual([int(x, 16) for x in first], baseline['difference_offsets'][:24])
        # Every inserted mismatch was *already equal* in the original owner.
        # The 905 count, all 53 COFF fields, both call kinds and jump count
        # stay unchanged. A count-only guard would incorrectly accept these.
        for bad in (3, 0x6, 0xA, 0x7A, 0xC5, 0x157, 0x208, 0x3B6):
            self.assertNotIn(bad, known)
            moved = sorted((known - {0x5}) | {bad})
            changed = {**baseline, 'difference_offsets': moved,
                       'first_differences': [hex(x) for x in moved[:24]]}
            with self.subTest(bad=hex(bad)), self.assertRaisesRegex(
                    RuntimeError, 'already matching original physical byte'):
                frontiers.guard_exattack_type1422_draw(changed)
        # A genuine reduction of one old difference is permitted as a
        # noncrediting diagnostic, not accepted source exactness.
        narrowed = sorted(known - {0x5})
        improved = {**baseline, 'overlapping_differences': 904,
                    'difference_offsets': narrowed,
                    'first_differences': [hex(x) for x in narrowed[:24]]}
        self.assertEqual(frontiers.guard_exattack_type1422_draw(improved)[
            'overlap_differences'], 904)
        self.assertEqual(improved['exactness_credit'], 'none')

    def test_exattack_1422_draw_future_full_size_convergence_remains_noncrediting(self):
        baseline = self.exattack_1422_draw_evidence()
        true_shape = {**baseline, 'candidate_bytes': 959,
                      'candidate_instructions': 290,
                      'overlapping_differences': 0, 'difference_offsets': [],
                      'first_differences': [],
                      'extra_candidate_bytes': 0, 'complete_bytes_equal': True}
        report = frontiers.guard_exattack_type1422_draw(true_shape)
        self.assertEqual(report['overlap_differences'], 0)
        self.assertEqual(true_shape['exactness_credit'], 'none')

    @staticmethod
    def screen_evidence():
        return {
            'diagnostic_only': True, 'exactness_credit': 'none',
            'target_bytes': 336, 'candidate_bytes': 336,
            'target_sha256': frontiers.SCREEN_SHAKE_ORIGINAL_SHA256,
            'difference_offsets': list(frontiers.SCREEN_SHAKE_RESIDUAL_OFFSETS),
            'reviewed_target_fields': 20, 'bound_relocations': 20,
            'target_instructions': 132, 'candidate_instructions': 132,
            'target_calls': 8, 'candidate_calls': 8, 'ordered_calls_agree': True,
            'target_blocks': 26, 'candidate_blocks': 26,
            'complete_direct_graphs_agree': True,
            'overlapping_differences': 26,
            'absent_target_bytes': 0, 'extra_candidate_bytes': 0,
            'complete_bytes_agree': False,
        }

    def test_screen_shake_cold_carrier_protects_all_eighteen_exact_profiles(self):
        with patch.object(frontiers, 'build') as build, patch.object(
                frontiers, 'parsed', return_value=self.screen_evidence()) as parsed, patch.object(
                frontiers, 'canonical', side_effect=lambda unit: {'unit': unit}) as canonical:
            owners, exact = frontiers.run_screen_shake(protect_exact=True)
        self.assertEqual([a[0][0] for a in build.call_args_list], [
            'screen-effect-calc-shake', 'screen-effect-register-chain',
            'screen-effect-draw-square'])
        parsed.assert_called_once_with(['scripts/inspect-screen-shake-envelope.py',
                                        'build/matching/ScreenEffect.obj', '--json'])
        self.assertEqual(owners['screen_shake_envelope']['ordinary_differences'], 26)
        inventory = frontiers.exact_source_profile_groups(
            'src/ScreenEffect.cpp', frontiers.SCREEN_EFFECT_EXACT_PROFILES)
        expected = sum(inventory.values(), [])
        self.assertEqual(len(expected), 18)
        self.assertEqual(len(set(expected)), 18)
        self.assertEqual([row['unit'] for row in exact], expected)
        self.assertEqual(canonical.call_count, 18)
        self.assertEqual([row['unit'] for row in exact][-2:], [
            'screen-effect-register-chain', 'screen-effect-draw-square'])

    def test_screen_shake_skip_exact_keeps_boss_but_not_profile_builds(self):
        with patch.object(frontiers, 'build') as build, patch.object(
                frontiers, 'parsed', return_value=self.screen_evidence()), patch.object(
                frontiers, 'canonical') as canonical:
            owners, exact = frontiers.run_screen_shake(protect_exact=False)
        self.assertEqual(owners['screen_shake_envelope']['ordinary_differences'], 26)
        self.assertEqual(exact, [])
        build.assert_called_once_with('screen-effect-calc-shake')
        canonical.assert_not_called()

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
            'target_sha256': frontiers.ETAMA_DRAW_ORIGINAL_SHA,
            'target_bytes': 600, 'candidate_bytes': 600,
            'target_instructions': 157, 'candidate_instructions': 160,
            'bound_fields': 27, 'ordered_calls': 13,
            'target_direct_blocks': 17, 'candidate_direct_blocks': 17,
            'complete_direct_graphs_agree': True,
            'overlapping_byte_differences': 268,
            'difference_offsets': [i for lo, hi in frontiers.ETAMA_DRAW_RESIDUAL_RUNS
                                   for i in range(lo, hi)],
            'missing_target_bytes': 0, 'extra_candidate_bytes': 0,
            'complete_bytes_equal': False,
        }

    def test_etama_big_draw_protects_all_four_real_exact_siblings(self):
        with patch.object(frontiers, 'build') as build, patch.object(
                frontiers, 'parsed', return_value=self.etama_evidence()), patch.object(
                frontiers, 'canonical', side_effect=lambda unit: {'unit': unit}):
            owners, siblings = frontiers.run_etama(protect_exact=True)
        self.assertEqual(owners['etama_draw']['overlapping_differences'], 268)
        self.assertEqual([r['unit'] for r in siblings],
                         ['bullet-draw-single', 'etama-controller-added-callback',
                          'etama-controller-update', 'runtime-add-normalize-angle'])
        self.assertEqual(build.call_count, 3)

    def test_etama_original_physically_bound_residual_runs_are_nonoverlapping(self):
        runs = frontiers.ETAMA_DRAW_RESIDUAL_RUNS
        self.assertEqual(len(runs), 32)
        self.assertTrue(all(0 <= lo < hi <= 600 for lo, hi in runs))
        self.assertTrue(all(a[1] < b[0] for a, b in zip(runs, runs[1:])))
        self.assertEqual(sum(hi-lo for lo, hi in runs), 268)
        frontiers.guard_etama_draw(self.etama_evidence())

    def test_etama_original_further_narrowing_and_zero_remain_noncrediting(self):
        report = self.etama_evidence()
        report['difference_offsets'] = report['difference_offsets'][10:]
        report['overlapping_byte_differences'] = len(report['difference_offsets'])
        frontiers.guard_etama_draw(report)
        exact_looking = {**report, 'difference_offsets': [],
                         'overlapping_byte_differences': 0,
                         'candidate_instructions': 157,
                         'complete_bytes_equal': True}
        frontiers.guard_etama_draw(exact_looking)
        self.assertEqual(exact_looking['exactness_credit'], 'none')
        with self.assertRaises(RuntimeError):
            frontiers.guard_etama_draw({**exact_looking, 'exactness_credit': 'accepted'})

    def test_etama_rejects_changed_previously_matching_bytes_and_fake_equality(self):
        evidence = self.etama_evidence()
        offsets = evidence['difference_offsets']
        for bad in (
            {'target_sha256': 'f'*64},
            {'difference_offsets': offsets[:-1]},
            {'difference_offsets': offsets + [offsets[-1]]},
            {'difference_offsets': list(reversed(offsets))},
            {'difference_offsets': offsets[:1] + [0x02A] + offsets[2:]},
            {'difference_offsets': offsets[:1] + [600] + offsets[2:]},
            {'difference_offsets': offsets[:1] + ['41'] + offsets[2:]},
            {'difference_offsets': offsets[:1] + [True] + offsets[2:]},
            {'difference_offsets': offsets[:1] + [-1] + offsets[2:]},
            {'difference_offsets': offsets[:1] + [offsets[0]] + offsets[2:]},
            {'difference_offsets': None},
            {'overlapping_byte_differences': True},
            {'candidate_instructions': 156},
            {'complete_bytes_equal': True},
        ):
            with self.subTest(bad=bad), self.assertRaises(RuntimeError):
                frontiers.guard_etama_draw({**evidence, **bad})

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

    def test_trail_initializers_target_extent_calls_blocks_and_all_bytes_guarded(self):
        for typ, size, ins, fields, calls, blocks, diff, begin, sha in (
                ('19', 763, 207, 57, 27, 9, 6, 0x1BB,
                 'fc7b630cff17c32048de02f169f5c39710727027941aa502db823e357563cf67'),
                ('21', 727, 198, 53, 25, 10, 9, 0x1C2,
                 'b5271179d169d482952d887b03ad08e10a19c74654590e32598551d0a523ca47')):
            evidence = {'diagnostic_only': True, 'exactness_credit': 'none',
                        'target_sha256': sha,
                        'target_bytes': size, 'candidate_bytes': size,
                        'target_instructions': ins, 'candidate_instructions': ins,
                        'normalized_aligned_instructions': ins,
                        'relocations': fields, 'target_calls': calls,
                        'candidate_calls': calls, 'ordered_calls_agree': True,
                        'direct_blocks': [blocks, blocks],
                        'complete_direct_graphs_agree': True,
                        'difference_offsets': [],
                        'complete_differences_if_same_extent': 0,
                        'complete_bytes_agree': True}
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
                        {'normalized_aligned_instructions': ins - 1},
                        {'complete_differences_if_same_extent': False},
                        {'target_sha256': '0' * 64},
                        {'diagnostic_only': False},
                        {'exactness_credit': 'accepted'},
                        {'complete_bytes_agree': False}):
                with self.subTest(typ=typ, bad=bad), self.assertRaises(RuntimeError):
                    frontiers.validate_exattack_trail_initializer(typ,
                                                                  {**evidence, **bad})
            # Restoring any former six/nine-byte residual is a regression.
            with self.subTest(typ=typ, old_gap=True), self.assertRaises(RuntimeError):
                frontiers.validate_exattack_trail_initializer(typ, {
                    **evidence, 'difference_offsets': list(range(begin, begin + diff)),
                    'complete_differences_if_same_extent': diff,
                    'complete_bytes_agree': False})
            # A diagnostic pass still cannot claim canonical acceptance.
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
            'target_sha256': frontiers.NETWORK_PARSER_ORIGINAL_SHA,
            'difference_offsets': sorted(frontiers.NETWORK_PARSER_RESIDUAL_OFFSETS),
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
                    {'target_sha256': '0'*64},
                    {'difference_offsets': list(reversed(sorted(frontiers.NETWORK_PARSER_RESIDUAL_OFFSETS)))},
                    {'difference_offsets': sorted(frontiers.NETWORK_PARSER_RESIDUAL_OFFSETS)[:-1]},
                    {'difference_offsets': sorted(frontiers.NETWORK_PARSER_RESIDUAL_OFFSETS)[:-1] + [0xE3]},
                    {'difference_offsets': [0x88]*76},
                    {'difference_offsets': ['0x88'] + sorted(frontiers.NETWORK_PARSER_RESIDUAL_OFFSETS)[1:]},
                    {'exactness_credit': 'accepted'}, {'diagnostic_only': False}):
            with self.subTest(bad=bad), patch.object(frontiers, 'build'), patch.object(
                    frontiers, 'parsed', return_value={**self.network_parser_evidence(), **bad}), self.assertRaises(RuntimeError):
                frontiers.run_network_config_parser(protect_exact=False)

    def test_network_parser_can_narrow_without_regressing_any_equal_byte(self):
        report = self.network_parser_evidence()
        known = sorted(frontiers.NETWORK_PARSER_RESIDUAL_OFFSETS)
        self.assertEqual(len(known), 76)
        self.assertEqual(known[0], 0x88)
        self.assertEqual(known[-1], 0xE2)
        # One genuine code improvement may fix any of the old 76 positions,
        # but cannot create new mismatches in the other 167 physical bytes.
        narrowed = {**report, 'overlap_differences': 75,
                    'difference_offsets': known[1:]}
        self.assertEqual(frontiers.guard_network_parser_original(
            narrowed)['overlap_differences'], 75)
        self.assertEqual(narrowed['exactness_credit'], 'none')
        candidate = {**report, 'overlap_differences': 0,
                     'difference_offsets': [], 'complete_bytes_equal': True,
                     'candidate_blocks': 33, 'direct_graphs_agree': True}
        frontiers.guard_network_parser_original(candidate)
        self.assertEqual(candidate['exactness_credit'], 'none')
        for now_bad in (0x87, 0xAA, 0xAC, 0xD6, 0xE3, 0xF2):
            with self.subTest(off=now_bad), self.assertRaises(RuntimeError):
                moved = sorted(known[1:] + [now_bad])
                frontiers.guard_network_parser_original(
                    {**report, 'difference_offsets': moved})

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
            'aligned_field_identities': 32,
            'field_change_spans': [],
            'field_offset_deltas': list(
                frontiers.REPLAY_BEGIN_CURRENT_FIELD_OFFSET_DELTAS),
            'complete_overlap_differences': 39, 'missing_target_bytes': 2,
            'target_sha256': frontiers.REPLAY_BEGIN_TARGET_SHA,
            'difference_offsets': sorted(frontiers.REPLAY_BEGIN_ORIGINAL_RESIDUAL_OFFSETS),
            'extra_source_bytes': 0, 'complete_bytes_equal': False,
        }

    def test_save_replay_uses_complete_guard_and_only_two_affected_profiles(self):
        report = {'diagnostic_only': True, 'overlap_differences': 3}
        with patch.object(frontiers, 'build') as build, patch.object(
                frontiers, 'parsed', return_value=report) as parsed, patch.object(
                frontiers, 'canonical', side_effect=lambda unit: {'unit': unit}):
            owners, siblings = frontiers.run_replay_save(protect_exact=True)
        self.assertEqual([call[0][0] for call in build.call_args_list], [
            'replay-manager-begin-recording-stage', 'replay-input-update'])
        parsed.assert_called_once_with(['scripts/inspect-replay-save.py',
                                        'build/matching/ReplayInputNoEh.obj', '--json', '--guard'])
        self.assertEqual(owners['replay_save'], report)
        groups = frontiers.exact_source_profile_groups(
            'src/ReplayInput.cpp', frontiers.REPLAY_INPUT_EXACT_PROFILES)
        self.assertEqual([x['unit'] for x in siblings], sum(groups.values(), []))
        self.assertEqual(len(siblings), 12)

    def test_save_replay_all_scope_reuses_the_just_checked_carriers(self):
        with patch.object(frontiers, 'build') as build, patch.object(
                frontiers, 'parsed', return_value={}) as parsed, patch.object(
                frontiers, 'canonical') as canonical:
            _, siblings = frontiers.run_replay_save(protect_exact=True, already_checked=True)
        build.assert_not_called()
        canonical.assert_not_called()
        self.assertEqual(siblings, [])
        self.assertIn('--guard', parsed.call_args.args[0])

    def test_network_message_builds_only_its_standalone_guarded_owner(self):
        report = {'diagnostic_only': True, 'exactness_credit': 'none',
                  'target_bytes': 808, 'bound_fields': 46}
        with patch.object(frontiers, 'build') as build, patch.object(
                frontiers, 'canonical') as canonical, patch.object(
                frontiers, 'parsed', return_value=report) as parsed:
            owners = frontiers.run_network_message()
        parsed.assert_called_once_with(['scripts/inspect-network-message.py',
                                        '--build', '--guard', '--json'])
        build.assert_not_called()
        canonical.assert_not_called()
        self.assertEqual(owners['network_message'], report)

    def test_replay_playback_cold_pinned_no_eh_and_exact_siblings(self):
        with patch.object(frontiers, 'build') as build, patch.object(
                frontiers, 'parsed', return_value=self.replay_evidence()) as parsed, patch.object(
                frontiers, 'canonical', side_effect=lambda unit: {'unit': unit}):
            owners, siblings = frontiers.run_replay_playback(protect_exact=True)
        self.assertEqual([call[0][0] for call in build.call_args_list], [
            'replay-manager-begin-recording-stage', 'replay-input-update',
            'zun-memory-alloc'])
        parsed.assert_called_once_with(['scripts/inspect-replay-begin-playback.py',
                                        'build/matching/ReplayInputNoEh.obj', '--json'])
        self.assertEqual(owners['replay_begin_playback']['ordinary_differences'], 39)
        self.assertEqual(owners['replay_begin_playback']['aligned_fields'], 32)
        self.assertEqual(owners['replay_begin_playback']['field_offset_deltas'],
                         [0] * 30 + [-1, -2])
        groups = frontiers.exact_source_profile_groups(
            'src/ReplayInput.cpp', frontiers.REPLAY_INPUT_EXACT_PROFILES)
        self.assertEqual([x['unit'] for x in siblings],
                         sum(groups.values(), []) +
                         list(frontiers.REPLAY_REAL_MEMORY_CALLEES))
        self.assertEqual(len(siblings), 15)
        self.assertEqual(len({x['unit'] for x in siblings}), 15)

    def test_replay_skip_exact_keeps_original_boss_without_eh_carrier(self):
        with patch.object(frontiers, 'build') as build, patch.object(
                frontiers, 'parsed', return_value=self.replay_evidence()) as parsed, patch.object(
                frontiers, 'canonical') as canonical:
            owners, siblings = frontiers.run_replay_playback(protect_exact=False)
        self.assertEqual(owners['replay_begin_playback']['ordinary_differences'], 39)
        self.assertEqual(owners['replay_begin_playback']['missing_target_bytes'], 2)
        self.assertEqual(siblings, [])
        build.assert_called_once_with('replay-manager-begin-recording-stage')
        parsed.assert_called_once_with(['scripts/inspect-replay-begin-playback.py',
                                        'build/matching/ReplayInputNoEh.obj', '--json'])
        canonical.assert_not_called()

    def test_replay_playback_rejects_incomplete_original_and_worse_code(self):
        for corruption in ({'target_bytes': 539}, {'candidate_bytes': 537},
                           {'candidate_instructions': 150},
                           {'target_instructions': 151}, {'original_fields': 31},
                           {'bound_fields': 31}, {'ordered_calls': 4},
                           {'ordered_calls_agree': False},
                           {'target_blocks': 11}, {'candidate_blocks': 13},
                           {'complete_direct_graphs_agree': False},
                           {'aligned_field_identities': 30},
                           {'aligned_field_identities': 31},
                           {'field_change_spans': [{'kind': 'insert'}]},
                           {'field_offset_deltas': [0] * 32},
                           {'complete_overlap_differences': 40},
                           {'target_sha256': '0'*64},
                           {'difference_offsets': None},
                           {'difference_offsets': list(reversed(sorted(
                               frontiers.REPLAY_BEGIN_ORIGINAL_RESIDUAL_OFFSETS)))},
                           {'difference_offsets': sorted(
                               frontiers.REPLAY_BEGIN_ORIGINAL_RESIDUAL_OFFSETS)[:-1]},
                           {'missing_target_bytes': 3},
                           {'extra_source_bytes': 1},
                           {'complete_bytes_equal': True}):
            with self.subTest(corruption=corruption), patch.object(frontiers, 'build'), patch.object(
                    frontiers, 'parsed', return_value={**self.replay_evidence(), **corruption}), self.assertRaises(RuntimeError):
                frontiers.run_replay_playback(protect_exact=False)

    def test_replay_begin_byte_guard_rejects_new_mismatch_with_same_count(self):
        expected = set(frontiers.REPLAY_BEGIN_ORIGINAL_RESIDUAL_OFFSETS)
        self.assertEqual(len(expected), 39)
        evidence = self.replay_evidence()
        self.assertEqual(expected, set(evidence['difference_offsets']))
        # The old 49-byte mask would now hide regression into newly matched
        # header/global field order. Every newly agreed physical byte is
        # sacred, even when a proposal keeps the *same* number of residuals.
        for damaged in (0x14B, 0x14C, 0x14D, 0x14E,
                        0x150, 0x151, 0x160, 0x1FA, 0x1FF, 0x1F9):
            self.assertNotIn(damaged, expected)
            changed = sorted((expected - {0x152}) | {damaged})
            with self.subTest(damaged=hex(damaged)), patch.object(
                    frontiers, 'build'), patch.object(frontiers, 'parsed',
                    return_value={**evidence, 'difference_offsets': changed}), self.assertRaisesRegex(
                    RuntimeError, 'already matching original physical byte'):
                frontiers.run_replay_playback(protect_exact=False)
        narrowed = {**evidence, 'complete_overlap_differences': 38,
                    'difference_offsets': sorted(expected - {0x152})}
        with patch.object(frontiers, 'build'), patch.object(frontiers, 'parsed',
                return_value=narrowed):
            result, _ = frontiers.run_replay_playback(protect_exact=False)
        self.assertEqual(result['replay_begin_playback']['ordinary_differences'], 38)
        self.assertEqual(narrowed['exactness_credit'], 'none')

    @staticmethod
    def title_draw_result_evidence():
        return {
            'diagnostic_only': True, 'exactness_credit': 'none',
            'target_bytes': 1939, 'candidate_bytes': 1939,
            'target_sha256': frontiers.TITLE_DRAW_RESULT_SHA256,
            'game_manager_fields': [{
                'offset': 0x3F, 'type': 'DIR32',
                'symbol': '?g_GameManager@@3UAsciiGameManagerView@@A',
                'target': 0x004A7D90, 'addend': 0x11C,
                'effective_target': 0x004A7EAC}],
            'target_instructions': 577, 'candidate_instructions': 577,
            'original_fields': 82, 'bound_fields': 82,
            'ordered_calls': 16, 'ordered_calls_agree': True,
            'original_blocks': 92, 'candidate_blocks': 92,
            'direct_graphs_agree': True,
            'aligned_instructions': 574, 'paired_branch_conflicts': [],
            'unpaired_branch_destinations': 1,
            'complete_differences': 22, 'complete_bytes_equal': False,
            'difference_offsets': [hex(i) for i in (
                0x0E, 0x0F, 0x10, 0x11, 0x12, 0x14, 0x15, 0x16, 0x17,
                0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F, 0x20,
                0x21, 0x22, 0x23, 0x24, 0x39)],
        }

    def test_title_draw_result_whole_owner_byte_frontier(self):
        frontiers.guard_title_draw_result(self.title_draw_result_evidence())
        original = self.title_draw_result_evidence()
        for changes in ({'target_sha256': '0' * 64},
                        {'candidate_bytes': 1938},
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
                        {'difference_offsets': original['difference_offsets'][:-1]},
                        {'difference_offsets': original['difference_offsets'] + ['0x40']},
                        {'difference_offsets': ['0x0e'] * 22},
                        {'difference_offsets': list(reversed(original['difference_offsets']))},
                        {'difference_offsets': ['bad'] + original['difference_offsets'][1:]},
                        {'difference_offsets': original['difference_offsets'][:-1] + ['0x25']},
                        {'difference_offsets': original['difference_offsets'][:-1] + ['0x38']},
                        {'difference_offsets': original['difference_offsets'][:-1] + ['0x3a']},
                        {'difference_offsets': original['difference_offsets'][:-1] + ['0x100']},
                        {'exactness_credit': 'accepted'}):
            with self.subTest(changes=changes), self.assertRaises(RuntimeError):
                frontiers.guard_title_draw_result({**original, **changes})

    def test_title_draw_result_native_global_cannot_be_aliased_or_retargeted(self):
        baseline = self.title_draw_result_evidence()
        original = baseline['game_manager_fields'][0]
        for fields in ([], [original, original],
                       [{**original, 'offset': 0x40}],
                       [{**original, 'type': 'REL32'}],
                       [{**original, 'target': 0x004A7EAC}],
                       [{**original, 'addend': 0}],
                       [{**original, 'effective_target': 0x004A7D90}],
                       [{**original, 'symbol': '?g_GameManager@@3UGameManagerModeView@@A'}],
                       [{**original, 'symbol': '?g_TitleNameTableIndex@@3HA'}]):
            with self.subTest(fields=fields), self.assertRaisesRegex(
                    RuntimeError, 'shared GameManager field'):
                frontiers.guard_title_draw_result({**baseline,
                    'game_manager_fields': fields})

    def test_title_draw_result_protects_two_originally_equal_entry_bytes(self):
        baseline = self.title_draw_result_evidence()
        old = set(map(lambda x: int(x, 16), baseline['difference_offsets']))
        self.assertEqual(old, frontiers.TITLE_DRAW_RESULT_ORIGINAL_RESIDUAL_OFFSETS)
        self.assertEqual(len(old), 22)
        for damaged in (0x13, 0x18):
            self.assertNotIn(damaged, old)
            # Both offsets fell INSIDE the prior allowed first window, so
            # same-count swaps passed the old window-only count bound.
            replaced = sorted((old - {0x0E}) | {damaged})
            with self.subTest(damaged=hex(damaged)), self.assertRaisesRegex(
                    RuntimeError, 'already matched original byte'):
                frontiers.guard_title_draw_result({**baseline,
                    'difference_offsets': [hex(x) for x in replaced]})

    def test_title_draw_result_bound_future_narrowing_not_exact_credit(self):
        baseline = self.title_draw_result_evidence()
        narrowed = {**baseline, 'complete_differences': 21,
                    'difference_offsets': baseline['difference_offsets'][:-1]}
        frontiers.guard_title_draw_result(narrowed)
        # Full owner convergence would still be a noncrediting diagnosis,
        # not a canonical match without the separate function-level replay.
        converged = {**baseline, 'complete_differences': 0,
                     'difference_offsets': [], 'complete_bytes_equal': True}
        frontiers.guard_title_draw_result(converged)
        self.assertEqual(converged['exactness_credit'], 'none')

    @staticmethod
    def camera_evidence():
        return {
            'diagnostic_only': True, 'exactness_credit': 'none',
            'target_bytes': 622, 'candidate_bytes': 622,
            'target_instructions': 201, 'candidate_instructions': 201,
            'original_fields': 40, 'bound_fields': 40,
            'ordered_calls': 11, 'calls_agree': True,
            'original_blocks': 8, 'candidate_blocks': 8,
            'direct_graphs_agree': True, 'overlap_differences': 0,
            'difference_offsets': sorted(frontiers.CAMERA_FACING_RESIDUAL_OFFSETS),
            'target_sha256': frontiers.CAMERA_FACING_TARGET_SHA,
            'extra_candidate_bytes': 0, 'complete_bytes_equal': True,
            'missing_target_bytes': 0,
        }

    def test_camera_replay_uses_render_carrier_and_protects_all_siblings(self):
        from unittest.mock import call
        with patch.object(frontiers, 'build') as build, patch.object(
                frontiers, 'parsed', return_value=self.camera_evidence()) as parsed, patch.object(
                frontiers, 'canonical', side_effect=lambda unit: {'unit': unit}):
            owners, exact = frontiers.run_camera_facing(protect_exact=True)
        self.assertEqual(owners['camera_facing_projection']['overlap_differences'], 0)
        self.assertEqual(owners['camera_facing_projection']['candidate_bytes'], 622)
        self.assertEqual(build.call_args_list,
                         [call('anm-manager-project-camera-facing-quad'),
                          call('anm-manager-project-3d-quad')])
        parsed.assert_called_once_with(['scripts/inspect-anm-camera-facing.py',
                                        'build/matching/AnmManagerRender.obj'])
        expected = frontiers.exact_source_carriers(
            'src/AnmManagerRender.cpp', frontiers.ANM_RENDER_EXACT_CARRIERS)
        all_render_units = expected['build/matching/AnmManagerRender.obj']
        self.assertEqual(len(all_render_units), 16)
        self.assertEqual([e['unit'] for e in exact],
                         all_render_units +
                         ['anm-manager-project-3d-quad'])

    def test_camera_replay_rejects_shortened_owner_wrong_fields_or_regression(self):
        for bad in ({'candidate_bytes': 619}, {'candidate_instructions': 202},
                    {'original_fields': 39}, {'bound_fields': 39},
                    {'ordered_calls': 10}, {'calls_agree': False},
                    {'candidate_blocks': 7}, {'direct_graphs_agree': False},
                    {'overlap_differences': 5}):
            with self.subTest(bad=bad), patch.object(frontiers, 'build'), patch.object(
                    frontiers, 'parsed', return_value={**self.camera_evidence(), **bad}), self.assertRaises(RuntimeError):
                frontiers.run_camera_facing(protect_exact=False)

    def test_camera_closure_protects_every_original_byte(self):
        current = self.camera_evidence()
        self.assertEqual(frontiers.CAMERA_FACING_RESIDUAL_OFFSETS, set())
        self.assertEqual(622 - len(current['difference_offsets']), 622)
        # Former residual positions now have the same strict protection as
        # the first and last bytes of the complete original owner.
        for offset in (0, 0xF6, 0xF7, 0xF9, 0xFA, 621):
            changed = {**current, 'overlap_differences': 1,
                       'difference_offsets': [offset], 'complete_bytes_equal': False}
            with self.subTest(offset=hex(offset)), patch.object(
                    frontiers, 'build'), patch.object(frontiers, 'canonical'), patch.object(
                    frontiers, 'parsed', return_value=changed), self.assertRaises(RuntimeError):
                frontiers.run_camera_facing(protect_exact=False)
        self.assertEqual(current['exactness_credit'], 'none')

    def test_network_zero_field_owner_and_both_exact_consumers(self):
        evidence = {'diagnostic_only': True, 'exactness_credit': 'none',
                    'target_bytes': 213, 'candidate_bytes': 213,
                    'relocation_fields': 0,
                    'target_instructions': 67, 'candidate_instructions': 67,
                    'target_calls': 0, 'candidate_calls': 0,
                    'target_blocks': 9, 'candidate_blocks': 9,
                    'complete_direct_graphs_agree': True,
                    'target_sha256': frontiers.RECEIVED_FRAME_TARGET_SHA256,
                    'differing_offsets': [hex(x) for x in sorted(
                        frontiers.RECEIVED_FRAME_RESIDUAL_OFFSETS)],
                    'complete_bytes_equal': False,
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

    def test_focused_queue_and_menu_cli_verify_target_and_avoid_other_owners(self):
        import io
        import json
        import sys
        for owner in ('queue', 'replaymenu'):
            order = []
            def received(**kwargs):
                self.assertEqual(kwargs, {'protect_exact': True})
                order.append('queue')
                return {'queue': {}}, [{'unit': 'predicted'}, {'unit': 'pop'}]
            def menu():
                order.append('replaymenu')
                return {'replaymenu': {}}
            output = io.StringIO()
            with self.subTest(owner=owner), patch.object(
                    sys, 'argv', ['check-boss-frontiers.py', '--owner', owner]), patch.object(
                    frontiers, 'execute', side_effect=lambda args: order.append(args)), patch.object(
                    frontiers, 'run_received_frames', side_effect=received) as queue, patch.object(
                    frontiers, 'run_title_replay_menu', side_effect=menu) as replaymenu, patch.object(
                    frontiers, 'run_title') as title, patch.object(
                    frontiers, 'run_ecl') as ecl, patch('sys.stdout', output):
                self.assertEqual(frontiers.main(), 0)
            self.assertEqual(order, [['scripts/verify-target.py'], owner])
            self.assertEqual(queue.call_count, owner == 'queue')
            self.assertEqual(replaymenu.call_count, owner == 'replaymenu')
            title.assert_not_called()
            ecl.assert_not_called()
            report = json.loads(output.getvalue())
            self.assertEqual(list(report['owners']), [owner])
            self.assertEqual(len(report['canonical_siblings']), 2 if owner == 'queue' else 0)
            self.assertEqual(report['exactness_credit'], 'none')

    def test_title_protects_all_screen_and_music_exact_units(self):
        checked = []
        with patch.object(frontiers, 'build') as build, patch.object(
                frontiers, 'canonical', side_effect=lambda unit: checked.append(unit) or {'unit': unit}):
            owners, exact = frontiers.run_title(protect_exact=True)
        self.assertEqual(owners, {})
        self.assertEqual(len(exact), 46)
        self.assertEqual(len(set(checked)), 46)
        self.assertIn('title-screen-setup-thread', checked)
        self.assertIn('score-record-insert', checked)
        self.assertEqual([row['unit'] for row in exact][-3:],
                         ['anm-manager-draw-text-left',
                          'anm-manager-load-surface',
                          'anm-manager-set-interrupt-array'])
        self.assertEqual(build.call_count, 9)
        self.assertEqual(checked, [row['unit'] for row in exact])

    def test_target_independent_title_probe_does_not_invent_receipts(self):
        with patch.object(frontiers, 'build') as build, patch.object(
                frontiers, 'canonical') as canonical:
            self.assertEqual(frontiers.run_title(protect_exact=False), ({}, []))
        build.assert_not_called()
        canonical.assert_not_called()
