"""Original native 0xA0 side-phase lifetime and direct GameManager reads.

These source guards are accompanied by separate current-source pinned
VC7.1/PE comparisons of full gameplay setup and exact callback siblings.
"""
from pathlib import Path
import unittest

ROOT=Path(__file__).resolve().parents[1]

class DirectSidePhaseOwner(unittest.TestCase):
    def test_setup_uses_actual_typed_side_state_across_lifetimes(self):
        src=(ROOT/'src/GameManagerSetup.cpp').read_text()
        shared=(ROOT/'src/GameManagerRuntimeView.hpp').read_text()
        self.assertIn('typedef AsciiGameSideRuntimeView SetupStateBuffer;',shared)
        self.assertNotIn('reinterpret_cast<SetupStateBuffer *>(',src)
        # Local aliases and direct global member accesses are equivalent
        # source forms; their physical accesses are checked by the Oracle.
        self.assertIn('sizeof(SetupStateBuffer) == 0xA0',src)
        self.assertIn('state->phase10++',src)

if __name__=='__main__':
    unittest.main()
