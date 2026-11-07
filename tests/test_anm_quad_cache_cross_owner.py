"""One native ANM quad cache tail is shared by primitive, projection and Render."""
from pathlib import Path
import tomllib
import unittest

ROOT=Path(__file__).resolve().parents[1]

class AnmQuadCacheCrossOwner(unittest.TestCase):
    def test_one_quad_cache_view_across_four_exact_source_carriers(self):
        shared=(ROOT/'src/AnmManagerQuadCacheView.hpp').read_text()
        self.assertIn('sizeof(AnmManagerQuadCacheView) == 0x128E8',shared)
        for name in ('AnmManagerPrimitiveDraw.cpp','AnmManagerDraw3D.cpp',
                     'AnmManagerProjection.cpp','AnmManagerRender.cpp'):
            source=(ROOT/'src'/name).read_text()
            with self.subTest(name=name):
                self.assertIn('#include "AnmManagerQuadCacheView.hpp"',source)
                self.assertNotIn('AnmLoadedSprite *currentSprite;\n    IDirect3DVertexBuffer8 *quadVertexBuffer;',source)
        primitive=(ROOT/'src/AnmManagerPrimitiveDraw.cpp').read_text()
        draw3d=(ROOT/'src/AnmManagerDraw3D.cpp').read_text()
        projection=(ROOT/'src/AnmManagerProjection.cpp').read_text()
        render=(ROOT/'src/AnmManagerRender.cpp').read_text()
        self.assertIn('typedef AnmManagerQuadCacheView AnmManagerPrimitiveView;',primitive)
        self.assertIn('typedef AnmManagerQuadCacheView AnmManagerDraw3DView;',draw3d)
        self.assertIn('typedef AnmManagerQuadCacheView AnmManagerProjectionView;',projection)
        self.assertIn('struct AnmManagerRenderView : AnmManagerQuadCacheView',render)
        self.assertIn('anm->untexturedVector[0]',projection)
        self.assertNotIn('anm->untexturedVertices[',projection)
        self.assertIn('offsetof(AnmManagerProjectionView,cachedWorldMatrix12024)==0x12024',projection)
        self.assertIn('offsetof(AnmManagerRenderView, vertexBufferEndPtr) == 0x2B28E8',render)

    def test_all_thirty_original_exact_function_bounds_intact(self):
        with (ROOT/'config/match-units.toml').open('rb') as f:u=tomllib.load(f)['units']
        sources=('src/AnmManagerPrimitiveDraw.cpp','src/AnmManagerDraw3D.cpp',
                 'src/AnmManagerProjection.cpp','src/AnmManagerRender.cpp')
        rows=[(n,v) for n,v in u.items() if v['source'] in sources]
        self.assertEqual(len(rows),30)
        self.assertEqual(sum(v['size'] for _,v in rows),8983)
        for name,size in (('anm-manager-project-3d-quad',595),
                          ('anm-manager-draw-3d',811),
                          ('anm-manager-flush-vertex-buffer',137)):
            self.assertEqual(u[name]['size'],size)

if __name__=='__main__':unittest.main()
