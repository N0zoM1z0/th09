"""Exact ANM ReleaseVertexBuffer uses the shared native +0x12890 cell."""
from pathlib import Path
import tomllib
import unittest

ROOT=Path(__file__).resolve().parents[1]

class AnmReleaseVertexBufferSharedCache(unittest.TestCase):
    def test_single_native_buffer_owner_across_render_and_release(self):
        source=(ROOT/'src/AnmManagerVertexBuffer.cpp').read_text()
        cache=(ROOT/'src/AnmManagerQuadCacheView.hpp').read_text()
        self.assertIn('#include "AnmManagerQuadCacheView.hpp"',source)
        self.assertNotIn('struct AnmManagerVertexBufferView',source)
        self.assertIn('offsetof(AnmManagerQuadCacheView, quadVertexBuffer) == 0x12890',source)
        self.assertIn('reinterpret_cast<AnmManagerQuadCacheView *>(this)',source)
        self.assertIn('manager->quadVertexBuffer->Release();',source)
        self.assertIn('manager->quadVertexBuffer = NULL;',source)
        self.assertIn('IDirect3DVertexBuffer8 *quadVertexBuffer;',cache)
        self.assertIn('offsetof(AnmManagerQuadCacheView, quadVertexBuffer) == 0x12890',cache)

    def test_independent_original_bound_leaf_is_31_bytes(self):
        with (ROOT/'config/match-units.toml').open('rb') as f:units=tomllib.load(f)['units']
        unit=units['anm-manager-release-vertex-buffer']
        self.assertEqual((unit['target_address'],unit['size'],len(unit['relocations'])),
                         (0x0042F3D0,31,0))

if __name__=='__main__':unittest.main()
