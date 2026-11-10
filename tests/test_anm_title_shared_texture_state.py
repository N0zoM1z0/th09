"""Keep the Title texture owner bound to the exact ANM background cache."""
from pathlib import Path
import tomllib
import unittest

ROOT = Path(__file__).resolve().parents[1]

class NativeAnmTitleTextureState(unittest.TestCase):

    def test_reviewed_original_carrier_extents(self):
        with (ROOT / 'config/match-units.toml').open('rb') as f:
            units = tomllib.load(f)['units']
        names = [n for n,v in units.items() if v.get('source') in
                 ('src/AnmManagerBackground.cpp','src/TitleDrawCallback.cpp')]
        self.assertEqual(len(names),6)
        self.assertEqual(sum(units[n].get('compare_size',units[n]['size']) for n in names),425)
        self.assertEqual(sum(len(units[n]['relocations']) for n in names),22)
        self.assertEqual(units['title-draw-callback']['target_address'],0x00424898)

if __name__ == '__main__':
    unittest.main()
