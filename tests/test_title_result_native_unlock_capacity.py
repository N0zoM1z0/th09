"""The original TH09 Title result browser uses one native PLST cursor extent.

Full PE/COFF cold replays, not source textual assertions, decide exactness.
This guards the real 16-byte unlock record versus the 14-character subset.
"""
from pathlib import Path
import tomllib
import unittest

ROOT = Path(__file__).resolve().parents[1]

class TitleResultNativeUnlockCapacity(unittest.TestCase):
    def test_full_cursor_extent_is_native_unlock_row_and_character_order(self):
        source = (ROOT/'src/TitleScreen.cpp').read_text()
        stats = (ROOT/'src/PlayStats.hpp').read_text()
        start = source.index('int TitleScreenView::OnUpdateResult()')
        end = source.index('\nint TitleScreenView::OnUpdateReplayMenu()',start)
        boss = source[start:end]
        self.assertIn('TitleResultNativeFullCharacterCapacity',source)
        self.assertIn('extern char g_TitleResultCharacterOrder[16];',source)
        self.assertIn('sizeof(((PlayStatsRecordView *)0)->characterUnlocked[0])',source)
        self.assertIn('unsigned char characterUnlocked[3][16];',stats)
        self.assertIn('enum { FullCharacterSlots = sizeof(g_PlayStatsRecord.characterUnlocked[0]) };',boss)
        self.assertIn('for (i32 i = 0; i < FullCharacterSlots; i++)',boss)
        self.assertIn('keyboardSelection += FullCharacterSlots;',boss)
        self.assertIn('keyboardSelection >= FullCharacterSlots',boss)
        self.assertIn('keyboardSelection -= FullCharacterSlots;',boss)
        self.assertIn('SetCharacterCursorInactive(g_TitleResultCharacterOrder[0], 60, FullCharacterSlots, 2);',boss)
        self.assertIn('SetCharacterCursorInactive(*characterSlot, 60, FullCharacterSlots, 2);',boss)
        # The separate game-observed 14 normal unlock characters are not
        # silently promoted to 16 when restoring a physical array bound.
        self.assertIn('for (i32 i = 0; i <= 13; i++)',boss)
        self.assertNotIn('keyboardSelection += 16;',boss)

    def test_exact_record_producer_stays_independent_of_open_title_boss(self):
        with (ROOT/'config/match-units.toml').open('rb') as f:
            units=tomllib.load(f)['units']
        self.assertIn('title-screen-setup-thread',units)
        self.assertIn('title-screen-start-menu',units)
        self.assertIn('title-screen-draw-result',units)
        self.assertTrue('score-record-insert' in units)
        self.assertEqual(units['title-screen-setup-thread']['source'],'src/TitleScreen.cpp')
        self.assertEqual(units['title-screen-start-menu']['size'],0x618)
        self.assertEqual(units['title-screen-start-menu']['compare_size'],0x638)
        self.assertEqual(units['score-record-insert']['source'],'src/TitleScreen.cpp')
        # The currently nonexact 485-byte ResultUpdate remains a separate
        # non-crediting complete owner, not a fabricated exact manifest unit.
        self.assertNotIn('title-screen-on-update-result',units)

if __name__=='__main__':
    unittest.main()
