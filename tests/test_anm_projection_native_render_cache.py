"""Exact ANM projection/3D/2D owners share physical cached world matrix."""
from pathlib import Path
import tomllib
import unittest

ROOT=Path(__file__).resolve().parents[1]

class AnmProjectionNativeRenderCache(unittest.TestCase):
    def test_one_shared_0x12024_matrix_and_0x12894_vertices(self):
        cache=(ROOT/'src/AnmManagerRenderCacheView.hpp').read_text()
        projection=(ROOT/'src/AnmManagerProjection.cpp').read_text()
        self.assertIn('D3DMATRIX cachedWorldMatrix12024;',cache)
        self.assertIn('offsetof(AnmManagerRenderCacheView, cachedWorldMatrix12024) == 0x12024',cache)
        self.assertIn('unsigned char unknown000024[0x12880 - 0x24];',cache)
        self.assertIn('typedef AnmManagerQuadCacheView AnmManagerProjectionView;',projection)
        shared=(ROOT/'src/AnmManagerQuadCacheView.hpp').read_text()
        self.assertIn('struct AnmManagerQuadCacheView : AnmManagerRenderCacheView',shared)
        self.assertIn('AnmLoadedSprite *currentSprite;',shared)
        self.assertIn('IDirect3DVertexBuffer8 *quadVertexBuffer;',shared)
        self.assertIn('VertexTex1Xyz untexturedVector[4];',shared)
        self.assertIn('offsetof(AnmManagerProjectionView,untexturedVector)==0x12894',projection)
        self.assertIn('offsetof(AnmManagerProjectionView,cachedWorldMatrix12024)==0x12024',projection)
        self.assertIn('anm->cachedWorldMatrix12024=worldTransformMatrix;',projection)
        self.assertNotIn('unknown00000[0x12024]',projection)

    def test_complete_original_render_owner_inventory_stable(self):
        with (ROOT/'config/match-units.toml').open('rb') as f:units=tomllib.load(f)['units']
        sources=('src/AnmManagerProjection.cpp','src/AnmManagerDraw3D.cpp',
                 'src/AnmManagerPrimitiveDraw.cpp','src/AnmManagerRender.cpp')
        owners={n:x for n,x in units.items() if x['source'] in sources}
        self.assertEqual(len(owners),30)
        self.assertEqual(owners['anm-manager-project-3d-quad']['size'],595)
        self.assertEqual(owners['anm-manager-draw-3d']['size'],811)
        self.assertEqual(sum(x['size'] for x in owners.values()),8983)

if __name__=='__main__':unittest.main()
