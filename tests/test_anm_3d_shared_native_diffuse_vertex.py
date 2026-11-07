"""ANM 3D projection and draw use the shared exact 20-byte diffuse vertex."""
from pathlib import Path
import tomllib
import unittest

ROOT=Path(__file__).resolve().parents[1]

class Anm3DSharedNativeDiffuseVertex(unittest.TestCase):
    def test_one_actual_xyzrhw_vertex_type_for_projection_and_draw(self):
        vertex=(ROOT/'src/VertexDiffuseXyzrhw.hpp').read_text()
        for name in ('AnmManagerProjection.cpp','AnmManagerDraw3D.cpp'):
            with self.subTest(source=name):
                source=(ROOT/'src'/name).read_text()
                self.assertIn('#include "VertexDiffuseXyzrhw.hpp"',source)
                self.assertNotIn('struct VertexDiffuseXyzrhw',source)
                self.assertIn('sizeof(VertexDiffuseXyzrhw)',source)
        projection=(ROOT/'src/AnmManagerProjection.cpp').read_text()
        self.assertIn('VertexDiffuseXyzrhw untexturedVertices[4];',projection)
        self.assertIn('offsetof(AnmManagerProjectionView,untexturedVertices)==0x12894',projection)
        self.assertIn('sizeof(VertexDiffuseXyzrhw) == 0x14',vertex)

    def test_both_original_bound_large_functions_remain_exact_extent(self):
        with (ROOT/'config/match-units.toml').open('rb') as f:
            units=tomllib.load(f)['units']
        self.assertEqual(units['anm-manager-project-3d-quad']['size'],595)
        self.assertEqual(units['anm-manager-draw-3d']['size'],811)
        # A formerly duplicated 0x14 struct has exactly one concrete owner.
        definitions=[p for p in (ROOT/'src').rglob('*.cpp')
                     if 'struct VertexDiffuseXyzrhw' in p.read_text()]
        self.assertEqual(definitions,[])

if __name__=='__main__':unittest.main()
