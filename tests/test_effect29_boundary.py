"""Pure control-flow cases for the bounded original callback boundary walk."""
import importlib.util
from pathlib import Path
import unittest

ROOT = Path(__file__).resolve().parents[1]
spec = importlib.util.spec_from_file_location(
    'effect29_boundary', ROOT / 'scripts/inspect-effect-type29.py')
audit = importlib.util.module_from_spec(spec)
spec.loader.exec_module(audit)


class CallbackBoundary(unittest.TestCase):
    def test_both_return_paths_and_backedge_exclude_adjacent_bytes(self):
        code = {100: (2, 'jcc', 108), 102: (2, 'call', 500),
                104: (2, 'jcc', 102), 106: (1, 'ret', None),
                108: (1, 'ret', None), 109: (1, 'invalid', None)}
        end, seen = audit.direct_extent(100, code.__getitem__, limit=20)
        self.assertEqual(end, 109)
        self.assertEqual(set(seen), set(code) - {109})
        self.assertNotIn(500, seen)  # A call is not an owned tail.

    def test_unreachable_alignment_is_inside_extent_without_being_walked(self):
        code = {100: (2, 'jmp', 106), 106: (1, 'ret', None)}
        end, seen = audit.direct_extent(100, code.__getitem__, limit=10)
        self.assertEqual((end, set(seen)), (107, {100, 106}))

    def test_indirect_or_out_of_bound_branch_is_rejected(self):
        for destination in (None, 99, 110):
            with self.subTest(destination=destination), self.assertRaises(ValueError):
                audit.direct_extent(100, lambda _: (2, 'jmp', destination), limit=10)

    def test_overlap_and_unrecognized_flow_are_rejected(self):
        code = {100: (2, 'jcc', 103), 102: (2, 'normal', None),
                104: (1, 'ret', None), 103: (1, 'ret', None)}
        with self.assertRaisesRegex(ValueError, 'overlapping'):
            audit.direct_extent(100, code.__getitem__, limit=10)
        for instruction in ((0, 'normal', None), (11, 'ret', None),
                            (1, 'invalid', None)):
            with self.subTest(instruction=instruction), self.assertRaises(ValueError):
                audit.direct_extent(100, lambda _: instruction, limit=10)

    def test_an_infinite_loop_does_not_prove_a_return_extent(self):
        with self.assertRaisesRegex(ValueError, 'no return'):
            audit.direct_extent(100, lambda _: (2, 'jmp', 100), limit=10)


if __name__ == '__main__':
    unittest.main()
