"""Protect the verified source record snapshot and unresolved replay-menu boss."""
from pathlib import Path
import unittest

ROOT = Path(__file__).resolve().parents[1]


class TitleReplayMenuNativeRecord(unittest.TestCase):
    def test_borrow_selected_record_before_snapshot(self):
        src = (ROOT/'src/TitleScreenReplayMenuDraw.cpp').read_text()
        selected = 'const ReplayDataView &replayValue = replays[selectedReplay];'
        snapshot = 'i32 selected = selectedReplay;'
        self.assertEqual(src.count(selected), 1)
        self.assertEqual(src.count(snapshot), 1)
        self.assertLess(src.index(selected), src.index(snapshot))
        self.assertIn('replayValue.frameStart[0][i]', src)
        self.assertIn('replayValue.frameStart[1][i]', src)
        self.assertIn('replayValue.playTime', src)
        self.assertIn('replayValue.playerName', src)
        self.assertNotIn('replay->', src)

    def test_preserve_distinct_mode_accesses_and_original_guard(self):
        src = (ROOT/'src/TitleScreenReplayMenuDraw.cpp').read_text()
        self.assertEqual(src.count('replays[selected].value1E4'), 3)
        inspect = (ROOT/'scripts/inspect-title-replay-menu-draw.py').read_text()
        self.assertIn('SIZE = 1280', inspect)
        self.assertIn('ORIGINAL_FIELDS = 57', inspect)
        self.assertIn('ORIGINAL_CALLS = 13', inspect)
        self.assertIn('ORIGINAL_BLOCKS = 72', inspect)


if __name__ == '__main__':
    unittest.main()
