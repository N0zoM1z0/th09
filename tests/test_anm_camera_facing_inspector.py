"""No-target reviewed-field guards for TH09's 622-byte camera-facing boss."""

import hashlib
import importlib.util
from pathlib import Path
import struct
import unittest
from unittest.mock import patch

try:
    import capstone
except ImportError:  # Public target-independent CI intentionally uses python -S.
    capstone = None

ROOT = Path(__file__).resolve().parents[1]
spec = importlib.util.spec_from_file_location(
    'anm_camera_facing_oracle', ROOT / 'scripts/inspect-anm-camera-facing.py')
camera = importlib.util.module_from_spec(spec)
spec.loader.exec_module(camera)

spec = importlib.util.spec_from_file_location(
    'boss_frontier_script', ROOT / 'scripts/check-boss-frontiers.py')
boss = importlib.util.module_from_spec(spec)
spec.loader.exec_module(boss)


class CameraFacingManifestTests(unittest.TestCase):
    @staticmethod
    def target_and_digest():
        code = bytearray(camera.SIZE)
        for offset, kind, _, address, addend in camera.FIELDS:
            struct.pack_into('<I', code, offset,
                             camera.encoded(kind, address + addend, offset))
        return bytes(code), hashlib.sha256(code).hexdigest()

    @staticmethod
    def sample_rows(shift=0):
        return [{'offset': offset + shift, 'type': kind,
                 'symbol': symbol, 'addend': addend}
                for offset, kind, symbol, _, addend in camera.FIELDS]

    def test_manifest_covers_original_ordered_forty_fields(self):
        self.assertEqual(len(camera.FIELDS), 40)
        self.assertEqual(len(camera.EXPECTED_CALLS), 11)
        self.assertEqual(len({offset for offset, _, _, _, _ in camera.FIELDS}), 40)
        data, digest = self.target_and_digest()
        camera.validate_original(data, sha256=digest)

    def test_full_candidate_fields_link_without_consulting_the_target(self):
        data = camera.bind_candidate(bytes(camera.SIZE), self.sample_rows(shift=1))
        self.assertEqual(len(data), camera.SIZE)
        for row, (_, kind, _, addr, addend) in zip(self.sample_rows(shift=1), camera.FIELDS):
            actual = struct.unpack_from('<I', data, row['offset'])[0]
            self.assertEqual(actual, camera.encoded(kind, addr + addend, row['offset']))
        # Nearby 619-byte nonexact candidates remain diagnosable, not exact.
        self.assertEqual(len(camera.bind_candidate(bytes(619), self.sample_rows())), 619)

    def test_target_hash_and_encoded_identity_are_both_required(self):
        data, digest = self.target_and_digest()
        for offset, *_ in camera.FIELDS:
            broken = bytearray(data)
            broken[offset] ^= 1
            with self.subTest(offset=hex(offset)), self.assertRaises(ValueError):
                camera.validate_original(bytes(broken), sha256=hashlib.sha256(broken).hexdigest())
        with self.assertRaises(ValueError):
            camera.validate_original(data, sha256='0' * 64)
        with self.assertRaises(ValueError):
            camera.validate_original(data[:-1], sha256=digest)

    def test_reject_wrong_candidate_kind_symbol_addend_and_overlap(self):
        baseline = self.sample_rows(shift=1)
        mutation = [
            baseline[:-1], list(reversed(baseline)),
            [dict(baseline[0], type='DIR32')] + baseline[1:],
            [dict(baseline[0], symbol='?newExternal@@3HA')] + baseline[1:],
            [dict(baseline[0], addend=4)] + baseline[1:],
            baseline[:1] + [dict(baseline[1], offset=baseline[0]['offset'] + 1)] + baseline[2:],
            baseline[:-1] + [dict(baseline[-1], offset=camera.SIZE - 2)],
        ]
        for rows in mutation:
            with self.subTest(first_rows=rows[:2]), self.assertRaises(ValueError):
                camera.bind_candidate(bytes(camera.SIZE), rows)
        for size in (camera.SIZE - 53, camera.SIZE + 90):
            with self.assertRaises(ValueError):
                camera.bind_candidate(bytes(size), baseline)

    @unittest.skipIf(capstone is None, 'Capstone unavailable under python -S')
    def test_decoder_rejects_indirect_calls_and_wrong_abi(self):
        for program in (bytes.fromhex('ffd0c20400'),
                        bytes.fromhex('90c3')):
            with self.subTest(binary=program.hex()), self.assertRaises(ValueError):
                camera.decoded_fields(program)


class CameraBossSweepTests(unittest.TestCase):
    @staticmethod
    def evidence():
        return {
            'diagnostic_only': True,
            'exactness_credit': 'none',
            'target_bytes': 622, 'candidate_bytes': 622,
            'target_instructions': 201, 'candidate_instructions': 201,
            'original_fields': 40, 'bound_fields': 40,
            'ordered_calls': 11, 'calls_agree': True,
            'original_blocks': 8, 'candidate_blocks': 8,
            'direct_graphs_agree': True,
            'overlap_differences': 13,
            'difference_offsets': sorted(boss.CAMERA_FACING_RESIDUAL_OFFSETS),
            'target_sha256': boss.CAMERA_FACING_TARGET_SHA,
            'extra_candidate_bytes': 0, 'complete_bytes_equal': False, 'missing_target_bytes': 0,
        }

    def test_sweep_protects_exact_same_and_neighboring_translation_units(self):
        with patch.object(boss, 'build') as build, patch.object(
                boss, 'parsed', return_value=self.evidence()), patch.object(
                boss, 'canonical', side_effect=lambda x: {'unit': x}):
            owners, siblings = boss.run_camera_facing(protect_exact=True)
        self.assertEqual(owners['camera_facing_projection']['missing_target_bytes'], 0)
        self.assertEqual(owners['camera_facing_projection']['overlap_differences'], 13)
        self.assertEqual([x['unit'] for x in siblings],
                         ['anm-manager-translate-rotation', 'anm-manager-draw2d',
                          'anm-manager-draw-camera-facing-quad', 'anm-manager-project-3d-quad'])
        self.assertEqual(build.call_count, 2)

    def test_closed_local_slots_can_improve_without_forging_exactness(self):
        evidence = self.evidence()
        with patch.object(boss, 'build'), patch.object(
                boss, 'parsed', return_value=evidence):
            boss.run_camera_facing(protect_exact=False)
        improved = {**evidence, 'overlap_differences': 12,
                    'difference_offsets': evidence['difference_offsets'][1:]}
        with patch.object(boss, 'build'), patch.object(
                boss, 'parsed', return_value=improved):
            boss.run_camera_facing(protect_exact=False)
        complete = {**evidence, 'overlap_differences': 0,
                    'difference_offsets': [], 'complete_bytes_equal': True}
        with patch.object(boss, 'build'), patch.object(
                boss, 'parsed', return_value=complete):
            boss.run_camera_facing(protect_exact=False)
        self.assertEqual(complete['exactness_credit'], 'none')

    def test_camera_closed_original_copy_window_rejects_moved_differences(self):
        evidence = self.evidence()
        for changed in (
                {'difference_offsets': [0xE8] + evidence['difference_offsets'][1:]},
                {'difference_offsets': list(reversed(evidence['difference_offsets']))},
                {'difference_offsets': [0xE9] * 13},
                {'difference_offsets': evidence['difference_offsets'][:-1]},
                {'difference_offsets': None},
                {'overlap_differences': 14},
                {'overlap_differences': True},
                {'complete_bytes_equal': True},
                {'target_sha256': '0' * 64},
                {'extra_candidate_bytes': 1},
                {'missing_target_bytes': 1},
        ):
            with self.subTest(changed=changed), patch.object(
                    boss, 'build'), patch.object(boss, 'parsed',
                    return_value={**evidence, **changed}), self.assertRaises(RuntimeError):
                boss.run_camera_facing(protect_exact=False)

    def test_bad_original_call_or_graph_inventory_fails_closed(self):
        for changed in ({'original_fields': 39}, {'ordered_calls': 10},
                        {'calls_agree': False}, {'original_blocks': 7},
                        {'direct_graphs_agree': False}):
            with self.subTest(change=changed), patch.object(
                    boss, 'build'), patch.object(boss, 'parsed',
                    return_value={**self.evidence(), **changed}), self.assertRaises(RuntimeError):
                boss.run_camera_facing(protect_exact=False)


if __name__ == '__main__':
    unittest.main()
