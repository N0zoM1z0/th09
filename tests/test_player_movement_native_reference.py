"""Keep the nonexact movement boss on the genuine Player movement subobject."""
from pathlib import Path
import unittest

ROOT = Path(__file__).resolve().parents[1]


class MovementNativeReference(unittest.TestCase):
    def test_native_player_movement_reference_and_offset(self):
        source = (ROOT / 'src/PlayerMovement.cpp').read_text()
        native = (ROOT / 'src/PlayerLifecycleView.hpp').read_text()
        self.assertIn('PlayerMovementFields &runtime = player->movementFields1CCC;', source)
        self.assertIn('PlayerMovementNativeFieldsAt1CCC', source)
        self.assertIn('offsetof(PlayerLifecycleView, movementFields1CCC) == 0x1CCC', source)
        self.assertIn('sizeof(PlayerMovementFields) == 0x20', source)
        self.assertIn('PlayerMovementFields movementFields1CCC;', native)
        self.assertIn('runtime.velocity1CCC.x', source)
        self.assertIn('runtime.movementAngle1CD8', source)
        self.assertNotIn('runtime->', source)
        self.assertNotIn('PlayerMovementFields *runtime', source)

    def test_full_movement_owner_still_requires_real_byte_exactness(self):
        guard = (ROOT / 'scripts/check-boss-frontiers.py').read_text()
        self.assertIn('PLAYER_MOVEMENT_CURRENT_RESIDUAL_OFFSETS', guard)
        self.assertIn('not 0 <= count <= 122', guard)
        self.assertIn('target_physical_bytes', guard)
        self.assertIn('candidate_physical_bytes', guard)
        self.assertIn('1900', guard)


if __name__ == '__main__':
    unittest.main()
