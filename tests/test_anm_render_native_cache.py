"""Real ANM cache and real VM alpha in the full original render owner."""
from pathlib import Path
import tomllib
import unittest

ROOT=Path(__file__).resolve().parents[1]


class AnmRenderNativeCache(unittest.TestCase):
    def test_cache_inheritance_and_native_vm_alpha(self):
        cache=(ROOT/'src/AnmManagerRenderCacheView.hpp').read_text()
        render=(ROOT/'src/AnmManagerRender.cpp').read_text()
        vm=(ROOT/'src/AnmVmLifecycle.hpp').read_text()
        self.assertIn('offsetof(AnmManagerRenderCacheView, flushesThisFrame) == 0x18',cache)
        self.assertIn('struct AnmManagerRenderView : AnmManagerQuadCacheView',render)
        quad=(ROOT/'src/AnmManagerQuadCacheView.hpp').read_text()
        self.assertIn('struct AnmManagerQuadCacheView : AnmManagerRenderCacheView',quad)
        self.assertIn('offsetof(AnmManagerRenderView, vertexBufferEndPtr) == 0x2B28E8',render)
        self.assertIn('offsetof(AnmManagerRenderView, vertexBufferStartPtr) == 0x2B28EC',render)
        self.assertNotIn('AnmVmDrawAlphaView',render)
        self.assertEqual(render.count('vm->color1Script.a'),7)
        self.assertIn('offsetof(AnmVmScriptColor, a) == 0x1F3',render)
        self.assertIn('AnmVmScriptColor color1Script',vm)

    def test_original_affected_profile_census(self):
        with (ROOT/'config/match-units.toml').open('rb') as stream:
            units=tomllib.load(stream)['units']
        source_names={'src/AnmManagerRender.cpp','src/AnmManagerPrimitiveDraw.cpp',
                      'src/AnmManagerDraw3D.cpp','src/AnmManagerBackground.cpp',
                      'src/TitleDrawCallback.cpp'}
        selected=[u for u in units.values() if u.get('source') in source_names]
        core=[u for u in selected if u['source']=='src/AnmManagerRender.cpp']
        self.assertEqual((len(core),sum(u.get('compare_size',u['size']) for u in core),
                          sum(len(u.get('relocations',[])) for u in core)),(16,5814,395))
        self.assertEqual((len(selected),sum(u.get('compare_size',u['size']) for u in selected),
                          sum(len(u.get('relocations',[])) for u in selected)),(35,8813,533))


if __name__=='__main__':
    unittest.main()
