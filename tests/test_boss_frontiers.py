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
    def enemy_evidence():
        main = {
            'diagnostic_only': True, 'exactness_credit': 'none',
            'target_bytes': 3900, 'candidate_bytes': 3900,
            'relocations': 97, 'relocation_fields_agree': 97,
            'target_calls': 69, 'candidate_calls': 69,
            'ordered_calls_agree': True, 'target_direct_blocks': 170,
            'candidate_direct_blocks': 170,
            'reviewed_control_flow_agrees': True,
            'switch_table_destinations_agree': True,
            'complete_differences': 39,
        }
        leaf = {
            'diagnostic_only': True, 'exactness_credit': 'none',
            'target_bytes': 82, 'candidate_bytes': 82,
            'bound_call_fields': 2, 'target_instructions': 30,
            'ordered_call_addresses': ['0x40f5a0', '0x40f8e0'],
            'ordinary_differing_bytes': 31,
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
