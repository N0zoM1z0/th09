"""TH09 exact ResultDraw/ReplaySave share the native six-by-sixteen keyboard.

Complete original PE/COFF equality remains the function oracle; this test
checks the C++ source relationship and prevents a second guessed grid.
"""
from pathlib import Path
import tomllib
import unittest

ROOT=Path(__file__).resolve().parents[1]

class TitleResultDrawNativeKeyboard(unittest.TestCase):
    def test_result_and_replay_save_use_one_real_keyboard(self):
        draw=(ROOT/'src/TitleScreenResultDraw.cpp').read_text()
        save=(ROOT/'src/TitleScreenReplaySaveDraw.cpp').read_text()
        layout=(ROOT/'src/TitleNativeNameKeyboard.hpp').read_text()
        self.assertIn('#include "TitleNativeNameKeyboard.hpp"',draw)
        self.assertIn('#include "TitleNativeNameKeyboard.hpp"',save)
        for line in ('row < TitleNativeNameKeyboard::Rows',
                     'col < TitleNativeNameKeyboard::Columns',
                     'row * TitleNativeNameKeyboard::Columns + col',
                     'row == TitleNativeNameKeyboard::LastRow',
                     'col == TitleNativeNameKeyboard::SpaceColumn',
                     'col == TitleNativeNameKeyboard::ConfirmColumn'):
            with self.subTest(line=line):
                self.assertIn(line,draw)
        self.assertNotIn('row * 16 + col',draw)
        self.assertNotIn('row < 6',draw)
        self.assertNotIn('col < 16',draw)
        self.assertIn('KeyCount = Columns * Rows',layout)
        self.assertIn('GapKey = KeyCount - 3',layout)
        self.assertIn('SpaceKey = KeyCount - 2',layout)
        self.assertIn('ConfirmKey = KeyCount - 1',layout)
        self.assertIn('TitleNativeNameKeyboardLayout',layout)
        self.assertIn('TitleNativeNameKeyboard::Columns',save)

    def test_existing_original_complete_function_ids_not_promoted(self):
        with (ROOT/'config/match-units.toml').open('rb') as f:
            u=tomllib.load(f)['units']
        result=u['title-screen-draw-result']
        save=u['title-screen-draw-replay-save']
        self.assertEqual((result['target_address'],result['size'],len(result['relocations'])),
                         (0x00423D16,1939,82))
        self.assertEqual((save['target_address'],save['size'],len(save['relocations'])),
                         (0x004239F6,800,41))
        self.assertEqual(result['symbol'],'?DrawResult@TitleScreenView@@QAEHXZ')
        self.assertNotIn('title-screen-draw-replay-menu',u)
        self.assertNotIn('title-screen-on-update-result',u)

if __name__=='__main__':
    unittest.main()
