"""Target-independent tests for zero-field frame-insertion comparison."""
import importlib.util
import hashlib
from pathlib import Path
from unittest import TestCase, main

SOURCE = Path(__file__).resolve().parents[1] / 'scripts/inspect-supervisor-frame-queue.py'
spec = importlib.util.spec_from_file_location('supervisor_frame_inspector', SOURCE)
inspector = importlib.util.module_from_spec(spec)
spec.loader.exec_module(inspector)
spec = importlib.util.spec_from_file_location(
    'supervisor_frame_frontier', SOURCE.with_name('check-boss-frontiers.py'))
frontier = importlib.util.module_from_spec(spec)
spec.loader.exec_module(frontier)


def current_frontier():
    # Actual cold original-bound residuals, independent of the guard's set.
    offsets = [*range(0x1C, 0x29), *range(0x2A, 0x30),
               *range(0x31, 0x3E), 0xBC, 0xBD, 0xBE]
    return {
        'diagnostic_only': True, 'exactness_credit': 'none',
        'target_sha256': inspector.TARGET_SHA256,
        'target_bytes': 213, 'candidate_bytes': 213,
        'relocation_fields': 0,
        'target_instructions': 67, 'candidate_instructions': 67,
        'target_calls': 0, 'candidate_calls': 0,
        'target_blocks': 9, 'candidate_blocks': 9,
        'complete_direct_graphs_agree': True,
        'complete_differences': 35,
        'differing_offsets': [hex(x) for x in offsets],
        'complete_bytes_equal': False,
    }


class FrameQueueInspectorTests(TestCase):
    def test_original_complete_extent_and_digest_both_are_required(self):
        target = bytes(range(inspector.SIZE))
        inspector.require_original_identity(target, hashlib.sha256(target).hexdigest())
        with self.assertRaisesRegex(ValueError, 'identity/extent'):
            inspector.require_original_identity(target, '0' * 64)
        with self.assertRaisesRegex(ValueError, 'identity/extent'):
            inspector.require_original_identity(target[:-1], hashlib.sha256(target[:-1]).hexdigest())
        modified = bytearray(target)
        modified[-1] ^= 0x80
        with self.assertRaisesRegex(ValueError, 'identity/extent'):
            inspector.require_original_identity(bytes(modified), hashlib.sha256(target).hexdigest())

    def test_complete_zero_field_owner_is_accepted_for_inspection(self):
        inspector.require_owner(bytes(inspector.SIZE), [])

    def test_short_candidate_never_receives_a_comparison(self):
        with self.assertRaisesRegex(ValueError, 'complete'):
            inspector.require_owner(bytes(inspector.SIZE - 2), [])

    def test_long_candidate_never_receives_a_comparison(self):
        with self.assertRaisesRegex(ValueError, 'complete'):
            inspector.require_owner(bytes(inspector.SIZE + 1), [])

    def test_unexpected_relocation_is_rejected(self):
        with self.assertRaisesRegex(ValueError, 'unexpected COFF relocation'):
            inspector.require_owner(bytes(inspector.SIZE), [dict(offset=0x42)])

    def test_all_physical_differences_are_reported_including_last_byte(self):
        original = bytes(inspector.SIZE)
        changed = bytearray(original)
        changed[0] = 0x53
        changed[0x19] = 0x37
        changed[-1] = 0xC2
        self.assertEqual(inspector.differing_offsets(changed, original),
                         [0, 0x19, inspector.SIZE - 1])

    def test_comparison_does_not_drop_truncated_target_or_candidate(self):
        with self.assertRaisesRegex(ValueError, 'incomplete'):
            inspector.differing_offsets(bytes(inspector.SIZE - 1), bytes(inspector.SIZE))
        with self.assertRaisesRegex(ValueError, 'incomplete'):
            inspector.differing_offsets(bytes(inspector.SIZE), bytes(inspector.SIZE + 1))


class FrameQueueFrontierTests(TestCase):
    def test_current_and_narrower_valid_diagnostics_remain_noncrediting(self):
        current = current_frontier()
        self.assertEqual(frontier.RECEIVED_FRAME_TARGET_SHA256, inspector.TARGET_SHA256)
        frontier.guard_received_frames(current)
        frontier.guard_received_frames({**current, 'complete_differences': 34,
                                       'differing_offsets': current['differing_offsets'][1:]})
        exact = {**current, 'complete_differences': 0,
                 'differing_offsets': [], 'complete_bytes_equal': True}
        frontier.guard_received_frames(exact)
        self.assertEqual(exact['exactness_credit'], 'none')

    def test_equal_difference_count_cannot_damage_matching_bytes(self):
        current = current_frontier()
        for offset in (0, 0x29, 0x30, 0xBF, 212):
            changed = {**current, 'differing_offsets': [hex(x) for x in sorted(
                [int(x, 16) for x in current['differing_offsets'][1:]] + [offset])]}
            with self.subTest(offset=hex(offset)), self.assertRaisesRegex(
                    RuntimeError, 'already matched'):
                frontier.guard_received_frames(changed)

    def test_incomplete_forged_and_malformed_evidence_is_rejected(self):
        current = current_frontier()
        changes = (
            {'diagnostic_only': False}, {'exactness_credit': 'accepted'},
            {'target_sha256': '0' * 64}, {'candidate_bytes': 212},
            {'target_bytes': 212}, {'relocation_fields': 1},
            {'complete_direct_graphs_agree': 1}, {'candidate_calls': 1},
            {'target_instructions': 66}, {'candidate_blocks': 8},
            {'complete_differences': True}, {'complete_differences': 34},
            {'complete_bytes_equal': True}, {'differing_offsets': None},
            {'differing_offsets': [0x1C] * 35},
            {'differing_offsets': ['0x1C'] * 35},
            {'differing_offsets': list(reversed(current['differing_offsets']))},
        )
        for change in changes:
            with self.subTest(change=change), self.assertRaises(RuntimeError):
                frontier.guard_received_frames({**current, **change})


if __name__ == '__main__':
    main()
