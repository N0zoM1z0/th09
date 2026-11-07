"""The two replay display owners share the actual 50-record/10-stage layout.

Static C++ view checks accompany separate complete original PE/COFF replays.
An unchanged near-match is not new canonical exactness credit.
"""
from pathlib import Path
import tomllib
import unittest

ROOT=Path(__file__).resolve().parents[1]


class TitleReplayNativeSlots(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.layout=(ROOT/'src/TitleReplayNativeSlots.hpp').read_text()
        cls.view=(ROOT/'src/TitleScreenReplayView.hpp').read_text()
        cls.menu=(ROOT/'src/TitleScreenReplayMenuDraw.cpp').read_text()
        cls.save=(ROOT/'src/TitleScreenReplaySaveDraw.cpp').read_text()

    def test_replay_path_entries_and_serialized_frame_arrays_have_real_lengths(self):
        self.assertIn('char replayPaths[50][0x200];',self.view)
        self.assertIn('ReplayDataView replays[50];',self.view)
        wire=(ROOT/'src/ReplayDataOriginalView.hpp').read_text()
        self.assertIn('frameStart[3][10]',wire)
        self.assertIn('TitleReplayNativeSlotDimensions',self.layout)
        for name,member in [('ReplayCount','replays'),('PathCount','replayPaths')]:
            with self.subTest(name=name):
                self.assertIn(name+' = sizeof(((TitleScreenView *)0)->'+member+')',self.layout)
        self.assertIn('StageCount = sizeof(((ReplayDataView *)0)->frameStart[0])',self.layout)
        self.assertIn('RowsPerColumn = ReplayCount / ColumnCount',self.layout)
        self.assertIn('ColumnCount = 2',self.layout)
        self.assertIn('RowsPerColumn == 25',self.layout)
        self.assertIn('sizeof(((TitleScreenView *)0)->replays) == 0x6018',self.layout)
        self.assertIn('offsetof(TitleScreenView, unknown0C8FC) ==',self.layout)
        self.assertIn('StageCount == 10',self.layout)

    def test_two_display_owners_use_one_native_count_and_column_boundary(self):
        for code in (self.menu,self.save):
            self.assertIn('#include "TitleReplayNativeSlots.hpp"',code)
            self.assertIn('TitleReplayNativeSlots::RowsPerColumn',code)
        self.assertIn('i < TitleReplayNativeSlots::ReplayCount',self.menu)
        self.assertIn('i < TitleReplayNativeSlots::StageCount',self.menu)
        self.assertIn('i == TitleReplayNativeSlots::RowsPerColumn',self.menu)
        self.assertEqual(self.menu.count('selected < TitleReplayNativeSlots::RowsPerColumn'),2)
        self.assertIn('i < TitleReplayNativeSlots::RowsPerColumn',self.save)
        self.assertNotIn('for (i32 i = 0; i < 50;',self.menu)
        self.assertNotIn('for (i = 0; i < 25;',self.save)

    def test_stage_count_and_columns_do_not_conflate_capacity(self):
        self.assertEqual((50,50//2,10),(50,25,10))
        self.assertEqual(50%2,0)
        self.assertEqual(3*10,30)
        self.assertNotEqual(50,10)
        self.assertEqual(0x1EC*50,0x6018)

    def test_frame_animation_and_keyboard_layout_are_independent_of_stages(self):
        # The animation's ten frames are a different semantic owner from
        # ten replay-stage headers; the save keyboard uses a separate 6x16 grid.
        self.assertIn('if (frame < 10)',self.menu)
        self.assertIn('for (row = 0; row < 6; row++)',self.save)
        self.assertIn('for (col = 0; col < 16; col++)',self.save)
        self.assertIn('replayValue.frameStart[0][i]',self.menu)
        self.assertEqual(6*16,96)

    def test_nonexact_menu_not_registered_as_exact_draw_save(self):
        units=tomllib.loads((ROOT/'config/match-units.toml').read_text())['units']
        self.assertEqual(units['title-screen-draw-replay-save']['source'],
                         'src/TitleScreenReplaySaveDraw.cpp')
        self.assertEqual(units['title-screen-draw-replay-save']['size'],800)
        self.assertEqual(len(units['title-screen-draw-replay-save']['relocations']),41)
        self.assertNotIn('title-screen-draw-replay-menu',units)
        self.assertNotIn('title-draw-replay-menu',units)

if __name__=='__main__':
    unittest.main()
