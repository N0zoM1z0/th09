"""Original-bound 2803-byte key-config boss calls the exact SoundPlayer leaf."""
from pathlib import Path
import tomllib
import unittest

ROOT=Path(__file__).resolve().parents[1]

class TitleKeyConfigNativeSoundPlayer(unittest.TestCase):
    def test_no_local_sound_player_shadow(self):
        source=(ROOT/'src/TitleScreenKeyConfig.cpp').read_text()
        native=(ROOT/'src/SoundPlayer.hpp').read_text()
        self.assertIn('#include "SoundPlayer.hpp"',source)
        self.assertNotIn('struct TitleKeySoundView',source)
        self.assertNotIn('extern TitleKeySoundView g_SoundPlayer;',source)
        self.assertIn('g_SoundPlayer.PlaySoundByIdx(12, 0);',source)
        self.assertIn('extern SoundPlayer g_SoundPlayer;',native)
        self.assertIn('void PlaySoundByIdx(int index, int pan);',native)

    def test_four_true_original_symbol_referents(self):
        with (ROOT/'config/match-units.toml').open('rb') as f:u=tomllib.load(f)['units']
        boss=u['title-screen-update-key-config']
        sound=u['soundplayer-play-sound-by-idx']
        fields={r['offset']:r for r in boss['relocations']}
        self.assertEqual((boss['size'],len(fields)),(2803,118))
        self.assertEqual((sound['size'],sound['target_address']),(140,0x0043E2F0))
        for offset in (0x6DE,0x95D):
            r=fields[offset]
            self.assertEqual((r['symbol'],r['target'],r['type']),
                             ('?g_SoundPlayer@@3VSoundPlayer@@A',0x004DC698,'DIR32'))
        for offset in (0x6E5,0x964):
            r=fields[offset]
            self.assertEqual((r['symbol'],r['target'],r['type']),
                             (sound['symbol'],sound['target_address'],'REL32'))
        self.assertEqual(u['title-screen-swap-duplicate-character-index']['size'],106)

if __name__=='__main__':unittest.main()
