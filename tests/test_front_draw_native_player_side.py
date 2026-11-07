"""FrontSide's complete exact draw owner reads real Side/Player state."""
from pathlib import Path
import unittest

ROOT = Path(__file__).resolve().parents[1]

class FrontDrawNativeSidePlayer(unittest.TestCase):
    def test_player_position_and_side_record_have_one_real_owner(self):
        src = (ROOT/'src/FrontSideDraw.cpp').read_text()
        header = (ROOT/'src/FrontSide.hpp').read_text()
        self.assertIn('FrontSideDrawNativeSideAndPlayer', src)
        self.assertNotIn('struct FrontSideDrawPlayerView', src)
        self.assertNotIn('struct FrontSideDrawSideView', src)
        self.assertNotIn('FRONT_SIDE_DRAW_SIDE', src)
        self.assertIn('offsetof(PlayerSideStateView, player04) == 0x04', src)
        self.assertIn('offsetof(PlayerLifecycleView, position1B88) == 0x1B88', src)
        self.assertIn('PlayerSideStateView *singleSide = frontSide->sideStateA670;', src)
        self.assertIn('frontSide->sideStateA670->player04->position1B88.x', src)
        self.assertIn('frontSide->sideStateA670->player04->position1B88.y', src)
        self.assertIn('PlayerSideStateView *sideStateA670;', header)

if __name__ == '__main__':
    unittest.main()
