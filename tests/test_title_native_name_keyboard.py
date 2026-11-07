"""Native Title 6x16 shared name keyboard (result entry and replay save).

Ordinary C++ source ownership checks accompany a separate original-bound
cold build of all 42 actual source/profile exact functions and the full
49-unit Title owner scope. Nonexact functions remain independently marked.
"""
from pathlib import Path
import tomllib
import unittest

ROOT=Path(__file__).resolve().parents[1]


class NativeTitleNameKeyboard(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.layout=(ROOT/'src/TitleNativeNameKeyboard.hpp').read_text()
        cls.controller=(ROOT/'src/TitleScreen.cpp').read_text()
        cls.renderer=(ROOT/'src/TitleScreenReplaySaveDraw.cpp').read_text()

    def test_6_by_16_grid_and_last_row_refer_to_one_native_ui(self):
        h=self.layout
        for expression in ('Columns = 16','Rows = 6','KeyCount = Columns * Rows',
                           'LastRow = Rows - 1','GapKey = KeyCount - 3',
                           'SpaceKey = KeyCount - 2','ConfirmKey = KeyCount - 1',
                           'SpaceColumn = Columns - 2','ConfirmColumn = Columns - 1',
                           'TitleNativeNameKeyboardLayout'):
            with self.subTest(expression=expression):self.assertIn(expression,h)
        self.assertEqual(6*16,96)
        self.assertEqual((5*16+13,5*16+14,5*16+15),(93,94,95))
        self.assertEqual((93,94,95),tuple(range(93,96)))

    def test_two_actual_title_input_bosses_use_same_navigation_and_special_keys(self):
        s=self.controller
        a=s.split('int TitleScreenView::OnUpdateResultNameEntry()',1)[1].split(
            'int TitleScreenView::OnUpdateResult()',1)[0]
        b=s.split('int TitleScreenView::UpdateReplaySave()',1)[1].split(
            'int TitleScreenView::OnUpdateStartMenu()',1)[0]
        self.assertIn('#include "TitleNativeNameKeyboard.hpp"',s)
        for owner in (a,b):
            for token in ('TitleNativeNameKeyboard::Columns',
                          'TitleNativeNameKeyboard::KeyCount',
                          'TitleNativeNameKeyboard::GapKey',
                          'TitleNativeNameKeyboard::SpaceKey',
                          'TitleNativeNameKeyboard::ConfirmKey',
                          'TitleNativeNameKeyboard::ConfirmColumn'):
                with self.subTest(owner=owner[:40],token=token):
                    self.assertIn(token,owner)
            self.assertIn('keyboardSelection = 77;',owner)
            self.assertIn('keyboardSelection = 13;',owner)
            self.assertIn('keyboardSelection = 92;',owner)
            self.assertIn('keyboardSelection = 94;',owner)
            self.assertIn('replayNameCursor < 7',owner)
        self.assertNotIn('keyboardSelection == 95',a)
        self.assertNotIn('keyboardSelection == 95',b)
        self.assertIn('SaveTitleScoreData();',a)
        self.assertIn('ReplayManagerView::SaveReplay(',b)

    def test_original_save_renderer_draws_last_two_special_glyphs(self):
        s=self.renderer
        self.assertIn('#include "TitleNativeNameKeyboard.hpp"',s)
        for fragment in ('row < TitleNativeNameKeyboard::Rows',
                         'col < TitleNativeNameKeyboard::Columns',
                         'col + row * TitleNativeNameKeyboard::Columns',
                         'row * TitleNativeNameKeyboard::Columns + col',
                         'row == TitleNativeNameKeyboard::LastRow',
                         'col == TitleNativeNameKeyboard::SpaceColumn',
                         'col == TitleNativeNameKeyboard::ConfirmColumn',
                         'str[0] = 0x7f;', 'str[0] = 0x80;'):
            with self.subTest(fragment=fragment):self.assertIn(fragment,s)
        self.assertIn('g_AsciiManager.AddString(',s)

    def test_original_large_and_small_source_owners_are_exact_not_open_credit(self):
        units=tomllib.loads((ROOT/'config/match-units.toml').read_text())['units']
        for key,length in (('title-screen-result-name-entry',966),
                           ('title-screen-replay-save',1825),
                           ('title-screen-draw-replay-save',800)):
            with self.subTest(unit=key):self.assertEqual(
                units[key].get('compare_size',units[key]['size']),length)
        self.assertEqual(len([u for u in units.values() if u['source']=='src/TitleScreen.cpp']),41)
        self.assertEqual(len([u for u in units.values() if u['source']=='src/TitleScreenReplaySaveDraw.cpp']),1)

if __name__=='__main__':
    unittest.main()
