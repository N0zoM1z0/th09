"""The exact 811-byte ANM Draw3D uses genuine Supervisor device storage."""
from pathlib import Path
import tomllib
import unittest

ROOT=Path(__file__).resolve().parents[1]

class AnmDraw3DNativeSupervisorDevice(unittest.TestCase):
    def test_real_device_member_not_a_private_8byte_prefix(self):
        source=(ROOT/'src/AnmManagerDraw3D.cpp').read_text()
        native=(ROOT/'src/Supervisor.hpp').read_text()
        self.assertNotIn('struct SupervisorDraw3DView',source)
        self.assertNotIn('SupervisorDraw3DView *',source)
        self.assertIn('offsetof(Supervisor, d3dDevice08) == 0x08',source)
        self.assertIn('Supervisor *supervisor = &g_Supervisor;',source)
        self.assertIn('supervisor->d3dDevice08->SetTransform(',source)
        self.assertIn('supervisor->d3dDevice08->SetStreamSource(',source)
        self.assertIn('supervisor->d3dDevice08->DrawPrimitive(',source)
        self.assertIn('IDirect3DDevice8 *d3dDevice08;',native)

    def test_exact_original_bound_render_method(self):
        with (ROOT/'config/match-units.toml').open('rb') as stream:
            u=tomllib.load(stream)['units']
        unit=u['anm-manager-draw-3d']
        self.assertEqual(unit['size'],811)
        self.assertEqual(unit['source'],'src/AnmManagerDraw3D.cpp')
        self.assertEqual(unit['target_address'],0x0043B1A0)

if __name__=='__main__':unittest.main()
