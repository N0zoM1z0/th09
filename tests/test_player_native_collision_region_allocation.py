"""Player bullet-collision boss and exact producer share native region fields.

The source-facing ownership proof complements, never replaces, the complete
original-bound PE/COFF audits of exact leaves and the still-nonexact boss.
"""
from pathlib import Path
import unittest
import tomllib

ROOT=Path(__file__).resolve().parents[1]

class NativePlayerCollisionRegionAllocation(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.player=(ROOT/'src/PlayerLifecycleView.hpp').read_text()
        cls.producer=(ROOT/'src/PlayerCollisionRegionCreate.hpp').read_text()
        cls.consumer=(ROOT/'src/PlayerCollisionHelpers.cpp').read_text()
        with (ROOT/'config/match-units.toml').open('rb') as f:
            cls.units=tomllib.load(f)['units']

    def test_direct_player_regions_match_original_44_byte_record_array(self):
        p=self.player
        for item in ('#include "PlayerCollisionRegionCreate.hpp"',
                     'PlayerCollisionRegionCreateRegionView collisionRegionB0BC;',
                     'PlayerCollisionRegionCreateRegionView *activeB100[514];',
                     'PlayerCollisionRegionCreateRegionView *releasedB908[514];',
                     'int collisionRegionActiveCountC110;',
                     'int collisionRegionReleasedCountC114;',
                     'PlayerLifecycleNativeRegionAllocation',
                     'offsetof(PlayerCollisionRegionCreateView, activeB100)',
                     'offsetof(PlayerCollisionRegionCreateView, releasedB908)',
                     'sizeof(((PlayerCollisionRegionCreateView *)0)->activeB100)',
                     'sizeof(((PlayerCollisionRegionCreateView *)0)->releasedB908)'):
            with self.subTest(item=item):self.assertIn(item,p)
        self.assertIn('sizeof(PlayerLifecycleView) == 0x30F70',p)
        self.assertIn('PlayerCreateRegionSizeIs44',self.producer)
        self.assertIn('offsetof(PlayerCollisionRegionCreateView, embeddedB0BC) == 0xB0BC',self.producer)

    def test_boss_scans_exact_native_field_without_duplicate_receiver(self):
        body=self.consumer.split('int PlayerLifecycleView::CheckBulletCollision(',1)[1].split(
            '\nint PlayerLifecycleView::CheckGrazeCollision(',1)[0]
        self.assertIn('PlayerCollisionRegionCreateRegionView **slot = activeB100;',body)
        self.assertNotIn('PlayerCollisionRegionCreateView &pool =',body)
        self.assertNotIn('reinterpret_cast<PlayerCollisionRegionCreateView',body)
        self.assertIn('const int regionType = region->type38;',body)
        self.assertIn('if ((*slot)->delay40 > 0)',body)
        self.assertIn('if ((*slot)->type38 == 2)',body)
        self.assertIn('if ((*slot)->type38 == 3)',body)

    def test_scoped_exact_siblings_are_still_independent_from_open_boss(self):
        for name in ('player-allocate-collision-region',
                     'player-find-collision',
                     'player-find-collision-at-player',
                     'player-check-graze-collision',
                     'player-update-before-state',
                     'player-initialize-added-state'):
            with self.subTest(name=name):
                self.assertIn(name,self.units)
                self.assertGreater(self.units[name]['size'],0)
        # The partially reconstructed 862-byte boss itself is *not*
        # canonical exact, even when its source-facing layout is recovered.
        self.assertNotIn('player-check-bullet-collision',self.units)

if __name__=='__main__':unittest.main()
