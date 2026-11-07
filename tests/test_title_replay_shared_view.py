"""Protect the one Title replay layout shared by both original-bound draws.

The code/COFF comparisons remain in their standalone owner inspectors. These
source tests assert storage aliasing and prevent restoring two divergent views.
"""
from pathlib import Path
import unittest

ROOT = Path(__file__).resolve().parents[1]


class TitleReplaySharedViewTests(unittest.TestCase):
    def test_replay_menu_and_save_renderers_include_one_layout(self):
        header = (ROOT / 'src/TitleScreenReplayView.hpp').read_text()
        self.assertEqual(header.count('struct TitleScreenView {'), 1)
        self.assertEqual(header.count('ReplayDataView replays[50]'), 1)
        self.assertIn('union { i32 replaySaveState; i32 currentScreenState; };', header)
        self.assertIn('sizeof(((TitleScreenView *)0)->replays[0]) == 0x1EC', header)
        self.assertIn('sizeof(((TitleScreenView *)0)->replayPaths[0]) == 0x200', header)
        self.assertIn('offsetof(TitleScreenView, stateTimer2) == 0x1B230', header)
        self.assertIn('sizeof(TitleScreenView) == 0x1B388', header)
        self.assertIn('TitleReplayNativeFullOwnerExtent',header)
        self.assertIn('TitleConfigSnapshotView configSnapshot;',header)
        self.assertNotIn('unknown0C8FC',header)

        for filename in ('TitleScreenReplayMenuDraw.cpp', 'TitleScreenReplaySaveDraw.cpp'):
            with self.subTest(source=filename):
                source = (ROOT / 'src' / filename).read_text()
                self.assertIn('#include "TitleScreenReplayView.hpp"', source)
                self.assertNotIn('struct TitleScreenView\n{', source)
                self.assertNotIn('ReplayDataView replays[50]', source)

    def test_all_replay_member_accesses_remain_typed(self):
        header = (ROOT / 'src/TitleScreenReplayView.hpp').read_text()
        menu = (ROOT / 'src/TitleScreenReplayMenuDraw.cpp').read_text()
        save = (ROOT / 'src/TitleScreenReplaySaveDraw.cpp').read_text()
        for member in ('replayNameCursor', 'replaySaveState', 'currentScreenState',
                       'replayPaths', 'replayName', 'replays', 'selectedReplay',
                       'stateTimer2'):
            self.assertIn(member, header)
        self.assertIn('replays[selectedReplay]', menu)
        self.assertIn('replayPaths[selected]', menu)
        self.assertIn('this->replays[i]', save)
        self.assertIn('this->replayName', save)


if __name__ == '__main__':
    unittest.main()
