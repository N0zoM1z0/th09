"""Exact 946-byte Effect34 draw uses the real 20-byte vertex and 0xEF8 allocation."""
from pathlib import Path
import tomllib
import unittest

ROOT=Path(__file__).resolve().parents[1]

class Effect34SharedNativeDiffuseVertex(unittest.TestCase):
    def test_native_vertex_array_with_original_offsets_and_allocation(self):
        view=(ROOT/'src/EffectType34RuntimeView.hpp').read_text()
        native=(ROOT/'src/VertexDiffuseXyzrhw.hpp').read_text()
        family=(ROOT/'src/EffectType34Family.inl').read_text()
        self.assertIn('#include "VertexDiffuseXyzrhw.hpp"',view)
        self.assertNotIn('struct VertexDiffuseXyzrhw\n{',view)
        self.assertIn('VertexDiffuseXyzrhw fan54[33];',view)
        self.assertIn('VertexDiffuseXyzrhw lines2E8[4][32];',view)
        self.assertIn('sizeof(EffectType34VerticesView)==0xEF8',view)
        self.assertIn('offsetof(EffectType34VerticesView, fan54) == 0x54',view)
        self.assertIn('offsetof(EffectType34VerticesView, lines2E8) == 0x2E8',view)
        self.assertIn('sizeof(VertexDiffuseXyzrhw) == 0x14',native)
        self.assertIn('g_ZunMemory.Alloc(sizeof(EffectType34VerticesView)',family)
        self.assertIn('memset(effect->vertices, 0, sizeof(EffectType34VerticesView));',family)
        self.assertIn('vertex->rhw = 1.0f;',family)

    def test_large_draw_and_initializer_keep_full_original_extents(self):
        with (ROOT/'config/match-units.toml').open('rb') as stream:
            u=tomllib.load(stream)['units']
        for n,size in (('effect-draw-callback-type34',946),
                       ('effect-initialize-callback-type34',178),
                       ('effect-initialize-type29',152)):
            with self.subTest(unit=n):
                self.assertEqual((u[n]['source'],u[n]['size']),('src/EclManager.cpp',size))

if __name__=='__main__': unittest.main()
