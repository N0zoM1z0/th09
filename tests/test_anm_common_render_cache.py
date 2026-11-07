"""One original ANM allocation, four source carriers, exact native cache."""
from pathlib import Path
import tomllib
import unittest

ROOT=Path(__file__).resolve().parents[1]


class AnmCommonRenderCache(unittest.TestCase):
    def test_one_native_cache_view_with_distinct_real_draw_tails(self):
        cache=(ROOT/'src/AnmManagerRenderCacheView.hpp').read_text()
        primitive=(ROOT/'src/AnmManagerPrimitiveDraw.cpp').read_text()
        draw=(ROOT/'src/AnmManagerDraw3D.cpp').read_text()
        background=(ROOT/'src/AnmManagerBackgroundView.hpp').read_text()
        self.assertIn('#include "Float3.hpp"',cache)
        self.assertIn('struct AnmManagerRenderCacheView',cache)
        for name,off in [('screenShakeOffset','0x1C'),('currentTexture','0x12880'),
                         ('currentBlendMode','0x12884'),('currentColorOp','0x12885'),
                         ('currentVertexShader','0x12886'),('disableZWrite','0x12887'),
                         ('cameraMode','0x12888')]:
            with self.subTest(name=name):
                self.assertIn('offsetof(AnmManagerRenderCacheView, '+name+') == '+off,cache)
        self.assertIn('sizeof(AnmManagerRenderCacheView) == 0x1288C',cache)
        self.assertIn('struct AnmManagerPrimitiveView : AnmManagerRenderCacheView',primitive)
        self.assertIn('struct AnmManagerDraw3DView : AnmManagerRenderCacheView',draw)
        self.assertIn('typedef AnmManagerRenderCacheView AnmManagerBackgroundView',background)
        self.assertNotIn('struct AnmManagerBackgroundView\n{',background)

    def test_complete_focused_owner_census(self):
        with (ROOT/'config/match-units.toml').open('rb') as f:
            units=tomllib.load(f)['units']
        relevant={'src/AnmManagerPrimitiveDraw.cpp','src/AnmManagerDraw3D.cpp',
                  'src/AnmManagerBackground.cpp','src/TitleDrawCallback.cpp'}
        selected=[u for u in units.values() if u.get('source') in relevant]
        self.assertEqual(len(selected),19)
        self.assertEqual(sum(u.get('compare_size',u['size']) for u in selected),2999)
        self.assertEqual(sum(len(u.get('relocations',[])) for u in selected),138)


if __name__=='__main__':
    unittest.main()
