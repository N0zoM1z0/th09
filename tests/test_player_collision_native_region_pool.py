"""Guard the real Player collision pool and the still-open byte-exact owner."""
from pathlib import Path
import unittest

ROOT = Path(__file__).resolve().parents[1]


class NativePlayerCollisionPool(unittest.TestCase):
    def test_native_pool_and_lifetime_of_region_type(self):
        source=(ROOT/'src/PlayerCollisionHelpers.cpp').read_text()
        native=(ROOT/'src/PlayerCollisionRegionCreate.hpp').read_text()
        self.assertIn('PlayerCollisionRegionCreateRegionView **slot = activeB100;',source)
        self.assertNotIn('PlayerCollisionRegionCreateView &pool =',source)
        player=(ROOT/'src/PlayerLifecycleView.hpp').read_text()
        self.assertIn('PlayerLifecycleNativeRegionAllocation',player)
        self.assertIn('PlayerCollisionRegionCreateRegionView *activeB100[514];',player)
        self.assertIn('PlayerCollisionRegionCreateRegionView *releasedB908[514];',player)
        self.assertIn('PlayerCollisionRegionCreateRegionView *region = *slot;',source)
        self.assertIn('const int regionType = region->type38;',source)
        self.assertIn('regionType != 1 && regionType != 2 && regionType != 3',source)
        self.assertIn('offsetof(PlayerCollisionRegionCreateView, activeB100) == 0xB100',native)
        self.assertIn('sizeof(PlayerCollisionRegionCreateRegionView) == 0x44',native)

    def test_complete_original_boss_audit_remains_noncrediting(self):
        inspect=(ROOT/'scripts/inspect-player-bullet-collision.py').read_text()
        self.assertIn('862',inspect)
        self.assertIn("'exactness_credit': 'none'",inspect)
        self.assertIn('three ret 12 ABI exit contracts changed',inspect)
        self.assertIn('26',inspect)


if __name__=='__main__':
    unittest.main()
