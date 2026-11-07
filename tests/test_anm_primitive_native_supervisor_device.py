"""Original ANM primitive render owners use real Supervisor D3D8 device."""
from pathlib import Path
import tomllib
import unittest

ROOT=Path(__file__).resolve().parents[1]

class AnmPrimitiveNativeSupervisorDevice(unittest.TestCase):
    def test_all_device_calls_share_true_supervisor_owner(self):
        code=(ROOT/'src/AnmManagerPrimitiveDraw.cpp').read_text()
        owner=(ROOT/'src/Supervisor.hpp').read_text()
        self.assertNotIn('struct SupervisorPrimitiveView',code)
        self.assertNotIn('SupervisorPrimitiveView *',code)
        self.assertIn('offsetof(Supervisor, d3dDevice08) == 0x08',code)
        self.assertIn('static __inline Supervisor *PrimitiveSupervisor()',code)
        self.assertIn('return &g_Supervisor;',code)
        self.assertIn('Supervisor *supervisor = PrimitiveSupervisor();',code)
        self.assertIn('supervisor->d3dDevice08->DrawPrimitiveUP(',code)
        self.assertIn('PrimitiveSupervisor()->d3dDevice08->CreateVertexBuffer(',code)
        self.assertIn('IDirect3DDevice8 *d3dDevice08;',owner)

    def test_full_original_primitive_owner_inventory(self):
        with (ROOT/'config/match-units.toml').open('rb') as f:u=tomllib.load(f)['units']
        sources=[(n,v) for n,v in u.items() if v['source']=='src/AnmManagerPrimitiveDraw.cpp']
        self.assertEqual(len(sources),12)
        self.assertEqual(sum(v['size'] for n,v in sources),1763)
        for name,size in (('anm-manager-draw-triangle-strip',318),
                          ('anm-manager-draw-triangle-fan',318),
                          ('anm-manager-setup-vertex-buffer',472),
                          ('anm-manager-draw-vertices',238)):
            self.assertEqual(u[name]['size'],size)

if __name__=='__main__':unittest.main()
