"""Target-independent guarantees for detailed RunEcl handler reports."""
import importlib.util
from pathlib import Path
import unittest

SOURCE = Path(__file__).resolve().parents[1] / 'scripts/ecl_shape_differences.py'
spec = importlib.util.spec_from_file_location('shape_differences', SOURCE)
module = importlib.util.module_from_spec(spec)
spec.loader.exec_module(module)


class EclShapeDifferencesTests(unittest.TestCase):
    def test_reordered_instructions_keep_real_offsets(self):
        candidate = [(0x30B0, b'\x8b\x43'), (0x30B2, b'\x33\xc0'),
                     (0x30B4, b'\x83\xe0\x01')]
        target = [(0x30B0, b'\x8b\x43'), (0x30B2, b'\x83\xe0\x01'),
                  (0x30B5, b'\x33\xc0')]
        changes = module.changed_instructions(candidate, target)
        self.assertTrue(changes)
        self.assertEqual([item['change'] for item in changes], ['insert', 'delete'])
        self.assertEqual(changes[0]['target'], [(0x30B2, '83e001')])
        self.assertEqual(changes[1]['candidate'], [(0x30B4, '83e001')])
        # The aligned 33C0 stays equal and should not be printed as a change.
        self.assertFalse(any(data == '33c0' for item in changes
                             for _, data in item['target'] + item['candidate']))

    def test_same_normalized_stream_with_shifted_offsets_is_not_a_change(self):
        self.assertEqual(module.changed_instructions(
            [(2, b'\xe8\0\0\0\0'), (7, b'\xc3')],
            [(3, b'\xe8\0\0\0\0'), (8, b'\xc3')]), [])

    def test_unmatched_insert_and_delete_are_not_silently_dropped(self):
        a = [(0, b'\x90'), (1, b'\xc3')]
        b = [(0, b'\xc3')]
        forward = module.changed_instructions(a, b)
        backward = module.changed_instructions(b, a)
        self.assertEqual([row['change'] for row in forward], ['delete'])
        self.assertEqual([row['change'] for row in backward], ['insert'])
        self.assertEqual(forward[0]['candidate'], [(0, '90')])
        self.assertEqual(backward[0]['target'], [(0, '90')])

    def test_scalar_literal_is_not_normalized_away(self):
        self.assertEqual(module.changed_instructions(
            [(0, b'\x25\x00\x00\x00\x40')],
            [(0, b'\x25\x00\x00\x00\x01')])[0]['change'], 'replace')


if __name__ == '__main__':
    unittest.main()
