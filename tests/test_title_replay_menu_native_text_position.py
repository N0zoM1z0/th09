"""Preserve the nonexact menu renderer's native ASCII text position view."""
from pathlib import Path
import unittest

ROOT = Path(__file__).resolve().parents[1]


class ReplayMenuTextPosition(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.source = (ROOT / 'src/TitleScreenReplayMenuDraw.cpp').read_text()

    def test_one_checked_sdk_to_float3_view_for_all_text_draws(self):
        s = self.source
        self.assertIn('sizeof(D3DXVECTOR3) == sizeof(Float3)', s)
        self.assertIn('offsetof(D3DXVECTOR3, x) == offsetof(Float3, x)', s)
        self.assertIn('offsetof(D3DXVECTOR3, z) == offsetof(Float3, z)', s)
        self.assertIn('Float3 &textPosition = reinterpret_cast<Float3 &>(position);', s)
        self.assertNotIn('reinterpret_cast<Float3 *>(&position)', s)
        self.assertEqual(s.count('g_AsciiManager.AddFormatText('), 8)
        self.assertEqual(s.count('&textPosition'), 9)  # eight calls plus its declaration
        self.assertIn('D3DXVECTOR3 displacement = target - position;', s)
        self.assertIn('position = position + displacement * (float)frame * 0.1f;', s)

    def test_still_bound_to_1280_byte_nonexact_original_range(self):
        s = self.source
        self.assertIn('const ReplayDataView &replayValue = replays[selectedReplay];', s)
        self.assertIn('i32 selected = selectedReplay;', s)
        self.assertIn('TitleReplayNativeSlots::ReplayCount', s)
        inspect = (ROOT / 'scripts/inspect-title-replay-menu-draw.py').read_text()
        for phrase in ('SIZE = 1280', 'ORIGINAL_FIELDS = 57',
                       'ORIGINAL_CALLS = 13', 'ORIGINAL_BLOCKS = 72'):
            self.assertIn(phrase, inspect)


if __name__ == '__main__':
    unittest.main()
