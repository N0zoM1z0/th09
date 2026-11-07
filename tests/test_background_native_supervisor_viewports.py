"""Protect TH09 Background's original-bound native Supervisor camera access.

These source checks complement, rather than replace, complete VC7.1 PE/COFF
comparison of the stage-script and same-TU timer/interpolation functions.
"""
import tomllib
from pathlib import Path
import unittest

ROOT = Path(__file__).resolve().parents[1]


class BackgroundNativeSupervisorViewports(unittest.TestCase):
    def test_stage_camera_uses_shared_supervisor_without_shadow(self):
        source = (ROOT/'src/BackgroundStageScript.cpp').read_text()
        supervisor = (ROOT/'src/Supervisor.hpp').read_text()
        self.assertIn('#include "Supervisor.hpp"', source)
        self.assertNotIn('struct BackgroundStageSupervisorView', source)
        self.assertNotIn('#define g_BackgroundStageViewports', source)
        self.assertNotIn('g_BackgroundStageViewports[', source)
        self.assertIn('BackgroundStageNativeViewportLayout', source)
        self.assertIn('SupervisorCtorArrayElement ctorArray078[3];', supervisor)
        for field in ('cameraPosition00', 'cameraLookAtOffset0C', 'cameraUp18',
                      'cameraForward24', 'cameraPositionOffset3C', 'fieldOfView48'):
            self.assertIn('g_Supervisor.ctorArray078[background->viewportIndex00C]', source)
            self.assertIn('offsetof(SupervisorCtorArrayElement, '+field+')', source)

    def test_every_stage_camera_global_field_binds_native_retail_owner(self):
        with (ROOT/'config/match-units.toml').open('rb') as file:
            units = tomllib.load(file)['units']
        run = units['background-run-stage-script']
        self.assertEqual((run['size'],run['compare_size']),(2472,2652))
        globals_ = [field for field in run['relocations']
                    if field['symbol'].startswith('?g_Supervisor@@')]
        self.assertEqual(len(globals_),19)
        for field in globals_:
            self.assertEqual(field['symbol'],'?g_Supervisor@@3VSupervisor@@A')
            self.assertEqual(field['target'],0x004B3100)
            self.assertGreaterEqual(field.get('addend',0),0x78)
        self.assertEqual(units['background-interpolate-camera-vector']['compare_size'],448)
        self.assertEqual(units['background-interpolate-camera-vector']['size'],423)


if __name__=='__main__':
    unittest.main()
