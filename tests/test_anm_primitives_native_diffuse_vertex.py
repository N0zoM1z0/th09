"""All exact ANM primitive leaves and ScreenEffect use one D3D diffuse vertex."""
from pathlib import Path
import tomllib
import unittest

ROOT=Path(__file__).resolve().parents[1]

class AnmPrimitivesNativeDiffuseVertex(unittest.TestCase):
    def test_one_native_xyzrhw_diffuse_type(self):
        primitive=(ROOT/'src/AnmManagerPrimitiveDraw.cpp').read_text()
        shared=(ROOT/'src/VertexDiffuseXyzrhw.hpp').read_text()
        self.assertIn('#include "VertexDiffuseXyzrhw.hpp"',primitive)
        self.assertNotIn('struct VertexDiffuseXyzrhw\n{',primitive)
        self.assertIn('sizeof(VertexDiffuseXyzrhw) == 0x14',primitive)
        self.assertIn('union { float w; float rhw; };',shared)
        self.assertIn('offsetof(VertexDiffuseXyzrhw, diffuse) == 0x10',shared)
        self.assertIn('int AnmManager::DrawTriangleStrip(',primitive)
        self.assertIn('int AnmManager::DrawTriangleFan(',primitive)
        self.assertIn('sizeof(VertexDiffuseXyzrhw)',primitive)

    def test_all_twelve_bound_exact_owners_preserve_full_extents(self):
        with (ROOT/'config/match-units.toml').open('rb') as stream:
            units=tomllib.load(stream)['units']
        local={n:v for n,v in units.items()
               if v['source']=='src/AnmManagerPrimitiveDraw.cpp'}
        self.assertEqual(len(local),12)
        self.assertEqual(sum(x['size'] for x in local.values()),1763)
        for name,size in (('anm-manager-draw-triangle-strip',318),
                          ('anm-manager-draw-triangle-fan',318),
                          ('anm-manager-setup-vertex-buffer',472)):
            self.assertEqual(local[name]['size'],size)

if __name__=='__main__':unittest.main()
