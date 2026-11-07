"""ScreenEffect and Effect strip callback owners share one native 20-byte vertex."""
from pathlib import Path
import tomllib
import unittest

ROOT = Path(__file__).resolve().parents[1]

class NativeDiffuseXyzrhwVertex(unittest.TestCase):
    def test_physical_d3d_vertex_and_aliased_w_rhw_are_identical(self):
        header = (ROOT/'src/VertexDiffuseXyzrhw.hpp').read_text()
        screen = (ROOT/'src/ScreenEffect.cpp').read_text()
        strips = (ROOT/'src/EffectDrawStrips.cpp').read_text()
        self.assertIn('struct VertexDiffuseXyzrhw', header)
        self.assertIn('VertexDiffuseXyzrhw();', header)
        self.assertIn('union { float w; float rhw; };', header)
        for expression in ('sizeof(VertexDiffuseXyzrhw) == 0x14',
                           'offsetof(VertexDiffuseXyzrhw, pos) == 0',
                           'offsetof(VertexDiffuseXyzrhw, w) == 0x0C',
                           'offsetof(VertexDiffuseXyzrhw, rhw) == 0x0C',
                           'offsetof(VertexDiffuseXyzrhw, diffuse) == 0x10'):
            self.assertIn(expression, header)
        for source in (screen,strips):
            self.assertIn('#include "VertexDiffuseXyzrhw.hpp"', source)
            self.assertNotIn('struct VertexDiffuseXyzrhw\n{', source)
        self.assertIn('vertices[3].w = 1.0f;', screen)
        self.assertIn('vertex->rhw = 1.0f;', strips)

    def test_three_original_bound_owners_retain_complete_extents(self):
        with (ROOT/'config/match-units.toml').open('rb') as stream:
            u=tomllib.load(stream)['units']
        for name,size in (('screen-effect-draw-square',527),
                          ('effect-draw-callback-type27',491),
                          ('effect-draw-callback-type36',506)):
            with self.subTest(name=name):
                self.assertEqual(u[name]['size'],size)
        vertex_ctor = next(x for x in u['screen-effect-draw-square']['relocations']
                           if '??0VertexDiffuseXyzrhw@@' in x['symbol'])
        self.assertEqual((vertex_ctor['offset'],vertex_ctor['target']), (0x18,0x004343D0))

if __name__ == '__main__':unittest.main()
