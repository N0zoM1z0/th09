"""The exact 2045-byte Title options boss stores native GameConfiguration."""
from pathlib import Path
import tomllib
import unittest

ROOT=Path(__file__).resolve().parents[1]

class TitleOptionsNativeConfigSnapshot(unittest.TestCase):
    def test_one_native_serialized_configuration(self):
        source=(ROOT/'src/TitleScreenOptions.cpp').read_text()
        native=(ROOT/'src/GameConfiguration.hpp').read_text()
        self.assertIn('typedef GameConfiguration TitleConfigSnapshotView;',source)
        self.assertIn('sizeof(TitleConfigSnapshotView) == sizeof(GameConfiguration)',source)
        self.assertIn('sizeof(TitleConfigSnapshotView) == 0xCC',source)
        for field,offset in (('valueAD','0xAD'),('windowed','0xB1')):
            self.assertIn('offsetof(TitleConfigSnapshotView, '+field+') == '+offset,source)
            self.assertIn('unsigned char '+field+';',native)
        self.assertNotIn('struct TitleConfigSnapshotView {',source)
        self.assertIn('TitleConfigSnapshotView configSnapshot;',source)
        self.assertIn('this->configSnapshot.valueAD',source)
        self.assertIn('this->configSnapshot.windowed',source)

    def test_large_original_owner_and_sound_leaf(self):
        with (ROOT/'config/match-units.toml').open('rb') as f:u=tomllib.load(f)['units']
        self.assertEqual((u['title-screen-options-update']['size'],
                          len(u['title-screen-options-update']['relocations'])),(2045,135))
        self.assertEqual(u['title-screen-play-menu-sound']['size'],34)

if __name__=='__main__':unittest.main()
