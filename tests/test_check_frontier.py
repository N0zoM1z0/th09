"""Target-independent checks for scripts/check-frontier.py and its config."""
import importlib.util
from pathlib import Path
import tomllib
import unittest

ROOT = Path(__file__).resolve().parents[1]
spec = importlib.util.spec_from_file_location('check_frontier', ROOT / 'scripts/check-frontier.py')
frontier = importlib.util.module_from_spec(spec)
spec.loader.exec_module(frontier)


class CheckFrontierTests(unittest.TestCase):
    def test_offset_ranges_round_trip(self):
        offsets = {0, 1, 2, 7, 0x10, 0x11, 0x200}
        text = frontier.format_offsets(offsets)
        self.assertEqual(text, ['0-2', '7', '10-11', '200'])
        self.assertEqual(frontier.parse_offsets(text), offsets)
        self.assertEqual(frontier.format_offsets(set()), [])

    def test_config_entries_are_complete_and_not_canonical_exact(self):
        config = tomllib.loads((ROOT / 'config/frontiers.toml').read_text())['frontier']
        units = tomllib.loads((ROOT / 'config/match-units.toml').read_text())['units']
        exact_symbols = {unit['symbol'] for unit in units.values()}
        for name, entry in config.items():
            with self.subTest(name=name):
                self.assertTrue((ROOT / entry['source']).is_file())
                self.assertIn('/I', entry['profile'])
                self.assertEqual(len(entry['target_sha256']), 64)
                self.assertNotIn(entry['symbol'], exact_symbols)
                residual = frontier.parse_offsets(entry['residual'])
                self.assertTrue(residual, 'an empty residual belongs in match-units.toml')
                self.assertLessEqual(max(residual), max(entry['size'], entry['candidate_size']))


if __name__ == '__main__':
    unittest.main()
