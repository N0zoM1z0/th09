"""Six per-viewport frame offsets belong to the three native ctor-array elements.

The previous SupervisorUpdateFrameOffsetsView was a redundant flat view over
the same original 0xF0 array elements. Complete Supervisor boss and sibling
PE/COFF replays independently verify the unchanged instruction/field graph.
"""
from pathlib import Path
import unittest

ROOT=Path(__file__).resolve().parents[1]


class SupervisorCtorFrameOffsets(unittest.TestCase):
    def test_three_native_ctor_elements_own_six_frame_offsets(self):
        h=(ROOT/'src/Supervisor.hpp').read_text()
        s=(ROOT/'src/SupervisorUpdate.cpp').read_text()
        self.assertIn('float frameOffsetX;',h)
        self.assertIn('float frameOffsetY;',h)
        self.assertIn('unknown000[0xE8]',h)
        self.assertIn('SupervisorCtorFrameOffsetsAtE8',h)
        self.assertIn('sizeof(SupervisorCtorArrayElement) == 0xF0',h)
        self.assertNotIn('struct SupervisorUpdateFrameOffsetsView',s)
        self.assertNotIn('reinterpret_cast<SupervisorUpdateFrameOffsetsView',s)
        for index in (0,1,2):
            for axis in ('X','Y'):
                name=f'frameOffset{axis}'
                self.assertIn(f'ctorArray078[{index}].{name}',s)
        self.assertIn('SupervisorUpdateNativeFrameOffsets',s)

    def test_original_large_supervisor_and_its_exact_siblings_remain_frozen(self):
        import tomllib
        with (ROOT/'config/match-units.toml').open('rb') as f:
            u=tomllib.load(f)['units']
        self.assertEqual((u['supervisor-constructor']['size'],
                          len(u['supervisor-constructor']['relocations'])),(62,3))
        self.assertEqual((u['supervisor-startup-thread']['size'],
                          len(u['supervisor-startup-thread']['relocations'])),(882,106))
        self.assertEqual((u['supervisor-deleted-callback']['size'],
                          len(u['supervisor-deleted-callback']['relocations'])),(405,44))


if __name__=='__main__':
    unittest.main()
