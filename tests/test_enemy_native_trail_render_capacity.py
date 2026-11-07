"""Shared native 96-sample/192-vertex Enemy trail and four draw buckets.

The real original-bound PE/COFF replay for the 3900-byte calc and 1758-byte
DrawImpl is independently run by the focused owner harness. These tests guard
source-facing storage relationships, not candidate exactness percentages.
"""
from pathlib import Path
import tomllib
import unittest

ROOT=Path(__file__).resolve().parents[1]


class EnemyNativeTrailRenderCapacityTests(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.layout=(ROOT/'src/EnemyNativeTrailRenderCapacity.hpp').read_text()
        cls.native=(ROOT/'src/EnemyManager.hpp').read_text()
        cls.calc=(ROOT/'src/EnemyManagerCore.inl').read_text()
        cls.draw=(ROOT/'src/EnemyManagerDraw.cpp').read_text()

    def test_actual_array_shape_and_all_four_owned_cardinalities(self):
        for name,count in (('drawGroupHeads2AC410',4),('secondaryVms2AC',2),
                           ('trailSamples33E8',96),('trailVertices3E68',192)):
            with self.subTest(name=name):
                self.assertIn(name+f'[{count}]',self.native)
        h=self.layout
        for key,name in (('DrawGroupCount','drawGroupHeads2AC410'),
                         ('SecondaryVmCount','secondaryVms2AC'),
                         ('TrailSampleCount','trailSamples33E8'),
                         ('TrailVertexCount','trailVertices3E68')):
            with self.subTest(key=key):
                self.assertIn(key+' = sizeof(',h)
                self.assertIn('->'+name+')',h)
        self.assertIn('VerticesPerSample = TrailVertexCount / TrailSampleCount',h)
        self.assertIn('EnemyNativeTrailRenderCapacityLayout',h)
        self.assertIn('sizeof(EnemyTrailSampleView) == 0x1C',h)
        self.assertIn('sizeof(EnemyTrailVertexView) == 0x1C',h)
        self.assertIn('offsetof(EnemyView, trailVertices3E68) ==',h)

    def test_two_vertices_per_sample_and_no_separate_history_allocation(self):
        self.assertEqual(192//96,2)
        self.assertEqual(192%96,0)
        self.assertEqual(96*0x1c,0xA80)
        self.assertEqual(192*0x1c,0x1500)
        self.assertEqual(0x33E8+0xA80,0x3E68)
        self.assertEqual(0x3E68+0x1500,0x5368)
        self.assertIn('EnemyNativeTrailRenderCapacity::VerticesPerSample',self.draw)
        self.assertIn('vertexCount += EnemyNativeTrailRenderCapacity::VerticesPerSample',self.draw)
        self.assertIn('vertexCount -= EnemyNativeTrailRenderCapacity::VerticesPerSample',self.draw)
        self.assertEqual(self.draw.count('if (vertexCount > EnemyNativeTrailRenderCapacity::VerticesPerSample)'),2)
        self.assertIn('g_AnmManager->DrawVertices(',self.draw)

    def test_the_open_calc_and_exact_draw_share_one_physical_layout(self):
        for source in (self.calc,self.draw):
            self.assertIn('#include "EnemyNativeTrailRenderCapacity.hpp"',source)
        self.assertIn('EnemyNativeTrailRenderCapacity::DrawGroupCount - 1',self.calc)
        self.assertIn('EnemyNativeTrailRenderCapacity::SecondaryVmCount',self.calc)
        self.assertIn('EnemyNativeTrailRenderCapacity::SecondaryVmCount',self.draw)
        self.assertIn('EnemyNativeTrailRenderCapacity::DrawGroupCount / 2',self.draw)
        self.assertIn('EnemyNativeTrailRenderCapacity::DrawGroupCount);',self.draw)
        self.assertNotIn('vertexCount += 2;',self.draw)
        self.assertNotIn('vertexCount -= 2;',self.draw)

    def test_original_canonical_draw_leaves_stay_distinct_from_open_update(self):
        units=tomllib.loads((ROOT/'config/match-units.toml').read_text())['units']
        owned={n:u for n,u in units.items() if u['source']=='src/EnemyManagerDraw.cpp'}
        self.assertEqual(set(owned),{
            'enemy-manager-draw-impl','enemy-manager-draw-high-prio',
            'enemy-manager-draw-low-prio','enemy-draw-interpolate-wrapped-angle'
        })
        self.assertEqual(owned['enemy-manager-draw-impl']['size'],1758)
        self.assertEqual(sum(x['size'] for x in owned.values()),1922)
        self.assertNotIn('enemy-manager-on-update',units)

if __name__=='__main__':
    unittest.main()
