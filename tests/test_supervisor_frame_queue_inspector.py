"""Target-independent tests for zero-field frame-insertion comparison."""
import importlib.util
from pathlib import Path
from unittest import TestCase, main

SOURCE = Path(__file__).resolve().parents[1] / 'scripts/inspect-supervisor-frame-queue.py'
spec = importlib.util.spec_from_file_location('supervisor_frame_inspector', SOURCE)
inspector = importlib.util.module_from_spec(spec)
spec.loader.exec_module(inspector)


class FrameQueueInspectorTests(TestCase):
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


if __name__ == '__main__':
    main()
