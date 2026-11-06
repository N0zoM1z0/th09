"""Keep the Title texture owner bound to the exact ANM background cache."""
from pathlib import Path
import tomllib
import unittest

ROOT = Path(__file__).resolve().parents[1]


class NativeAnmTitleTextureState(unittest.TestCase):
    def test_one_declared_cache_for_title_and_background(self):
        view = (ROOT / 'src/AnmManagerBackgroundView.hpp').read_text()
        background = (ROOT / 'src/AnmManagerBackground.cpp').read_text()
        title = (ROOT / 'src/TitleDrawCallback.cpp').read_text()
        self.assertIn('struct AnmManagerBackgroundView', view)
        self.assertIn('offsetof(AnmManagerBackgroundView, currentTexture) == 0x12880', view)
        self.assertIn('offsetof(AnmManagerBackgroundView, cameraMode) == 0x12888', view)
        self.assertIn('#include "AnmManagerBackgroundView.hpp"', background)
        self.assertIn('#include "AnmManagerBackgroundView.hpp"', title)
        self.assertNotIn('struct AnmManagerBackgroundView\n{', background)
        self.assertIn('->currentTexture = NULL;', title)
        self.assertNotIn('reinterpret_cast<unsigned char *>(g_AnmManager) + 0x12880', title)

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
