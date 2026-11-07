"""Shared native D3D8 viewport storage in the exact initialization owner.

These source invariants complement the separate whole-function PE/COFF replay;
no source-only assertion grants exactness credit.
"""
from pathlib import Path
import tomllib
import unittest

ROOT = Path(__file__).resolve().parents[1]


class D3DInitNativeSupervisorViewports(unittest.TestCase):
    def test_native_viewports_and_minimal_flag_receiver(self):
        source = (ROOT / 'src/GameWindowInitD3D.cpp').read_text()
        header = (ROOT / 'src/Supervisor.hpp').read_text()
        self.assertIn('SupervisorCtorArrayElement (&viewports)[3] = g_Supervisor.ctorArray078;', source)
        self.assertNotIn('struct D3DInitViewportConfiguration', source)
        self.assertNotIn('supervisor->viewports078', source)
        self.assertNotIn('SupervisorCtorArrayElement viewports078[3];', source)
        for i in range(3):
            self.assertIn(f'viewports[{i}].cameraPosition00', source)
            self.assertIn(f'viewports[{i}].viewportCC.Width', source)
            self.assertIn(f'viewports[{i}].viewportModeE4', source)
        self.assertIn('unsigned int flags5D4;', source)
        self.assertIn('offsetof(SupervisorD3DInitView, flags5D4) == 0x5D4', source)
        self.assertIn('supervisor->flags5D4', source)
        self.assertIn('SupervisorCtorArrayElement ctorArray078[3];', header)
        self.assertIn('sizeof(SupervisorCtorArrayElement) == 0xF0', header)

    def test_complete_exact_owner_manifest_remains_separate(self):
        with (ROOT / 'config/match-units.toml').open('rb') as f:
            units = tomllib.load(f)['units']
        for name, extent in (
            ('initialize-d3d-rendering', 1561),
            ('append-d3d-capability-support-line', 74),
            ('format-d3d-capabilities', 809),
        ):
            with self.subTest(owner=name):
                self.assertEqual(units[name]['size'], extent)
                self.assertEqual(units[name]['source'], 'src/GameWindowInitD3D.cpp')
                self.assertTrue(units[name]['object'].startswith('build/matching/'))


if __name__ == '__main__':
    unittest.main()
