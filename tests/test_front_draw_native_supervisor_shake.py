"""The 3,094-byte exact HUD draw owner consumes the native Supervisor viewport.

These are structural guards; the original PE/COFF comparison is independent.
"""
from pathlib import Path
import unittest

ROOT = Path(__file__).resolve().parents[1]


class FrontDrawNativeSupervisorShake(unittest.TestCase):
    def test_native_screen_shake_without_local_opaque_camera(self):
        src = (ROOT/'src/FrontSideDraw.cpp').read_text()
        supervisor = (ROOT/'src/Supervisor.hpp').read_text()
        self.assertNotIn('struct FrontSideDrawViewportConfiguration', src)
        self.assertIn('offsetof(SupervisorCtorArrayElement, screenShakeOffsetE8)', src)
        for component in ('x', 'y'):
            self.assertIn('g_Supervisor.ctorArray078[frontSide->sideIndexA66C].screenShakeOffsetE8.'+component+' = 0.0f;', src)
        self.assertIn('SupervisorCtorArrayElement ctorArray078[3];', supervisor)
        self.assertIn('offsetof(SupervisorCtorArrayElement, screenShakeOffsetE8) == 0xE8', supervisor)


if __name__ == '__main__':
    unittest.main()
