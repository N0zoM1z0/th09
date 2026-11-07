"""Exact ANM primitive owners share real 0x1288C..0x128E8 cache tail."""
from pathlib import Path
import tomllib
import unittest

ROOT=Path(__file__).resolve().parents[1]

class AnmQuadCacheNativePrimitive(unittest.TestCase):
    def test_one_shared_native_quad_cache_record(self):
        shared=(ROOT/'src/AnmManagerQuadCacheView.hpp').read_text()
        src=(ROOT/'src/AnmManagerPrimitiveDraw.cpp').read_text()
        self.assertIn('struct AnmManagerQuadCacheView : AnmManagerRenderCacheView',shared)
        self.assertIn('struct VertexTex1Xyz',shared)
        for member,offset in (('currentSprite','0x1288C'),
                              ('quadVertexBuffer','0x12890'),
                              ('untexturedVector','0x12894'),
                              ('spritesToDraw','0x128E4')):
            self.assertIn(f'offsetof(AnmManagerQuadCacheView, {member}) == {offset}',shared)
        self.assertIn('sizeof(AnmManagerQuadCacheView) == 0x128E8',shared)
        self.assertIn('#include "AnmManagerQuadCacheView.hpp"',src)
        self.assertIn('typedef AnmManagerQuadCacheView AnmManagerPrimitiveView;',src)
        self.assertNotIn('struct AnmManagerPrimitiveView :',src)
        self.assertNotIn('struct VertexTex1Xyz\n{',src)

    def test_all_original_12_primitive_exact_owners(self):
        with (ROOT/'config/match-units.toml').open('rb') as f:u=tomllib.load(f)['units']
        rows=[(n,v) for n,v in u.items() if v['source']=='src/AnmManagerPrimitiveDraw.cpp']
        self.assertEqual(len(rows),12)
        self.assertEqual(sum(v['size'] for _,v in rows),1763)

if __name__=='__main__':unittest.main()
