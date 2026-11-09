"""Exact 1,939-byte ResultDraw uses the full native TitleScreen allocation.

Original code/COFF equality is enforced separately by source-cold canonical
comparison; these source checks prevent duplicate title-prefix models.
"""
from pathlib import Path
import tomllib
import unittest

ROOT=Path(__file__).resolve().parents[1]

class TitleResultDrawFullNativeOwner(unittest.TestCase):
    def test_one_full_title_object_for_result_core_options_and_replay(self):
        native=(ROOT/'src/TitleScreenReplayView.hpp').read_text()
        draw=(ROOT/'src/TitleScreenResultDraw.cpp').read_text()
        core=(ROOT/'src/TitleScreen.cpp').read_text()
        opts=(ROOT/'src/TitleScreenOptions.cpp').read_text()
        self.assertEqual(native.count('struct TitleScreenView {'),1)
        self.assertIn('int DrawResult();',native)
        self.assertIn('int TitleScreenView::DrawResult()',draw)
        self.assertIn('#include "TitleScreenReplayView.hpp"',draw)
        self.assertNotIn('struct TitleScreenView',draw)
        self.assertNotIn('struct TitleScreenView {',core)
        self.assertNotIn('struct TitleScreenView {',opts)
        self.assertIn('sizeof(TitleScreenView) == 0x1B388',native)
        for field,offset in (('keyboardSelection','0x0'),('nameBankIndex','0x8'),
                             ('nameSlotIndex','0xC'),('currentScreenState','0x28'),
                             ('stateTimer','0x2C'),('currentScreen','0x1B228'),
                             ('stateTimer2','0x1B230')):
            with self.subTest(field=field):
                self.assertIn('offsetof(TitleScreenView, '+field+')',draw)
        self.assertIn('const TitleScoreRecordView (*scores)[5] =',draw)
        self.assertIn('g_TitleScoreTable[nameBankIndex]',draw)
        self.assertNotIn('scores = g_TitleScoreTable',draw)
        self.assertNotIn('unknown00030[0x1B228 - 0x30]',draw)

    def test_exact_full_result_draw_and_open_bosses_remain_distinct(self):
        with (ROOT/'config/match-units.toml').open('rb') as fd:
            units=tomllib.load(fd)['units']
        draw=units['title-screen-draw-result']
        self.assertEqual(draw['target_address'],0x00423D16)
        self.assertEqual(draw['size'],1939)
        self.assertEqual(len(draw['relocations']),82)
        self.assertEqual(draw['symbol'],'?DrawResult@TitleScreenView@@QAEHXZ')
        self.assertEqual(units['title-screen-options-update']['size'],2045)
        self.assertEqual(units['title-screen-draw-replay-save']['size'],800)
        self.assertNotIn('title-screen-draw-replay-menu',units)
        self.assertNotIn('title-screen-on-update-result',units)

if __name__=='__main__':unittest.main()
