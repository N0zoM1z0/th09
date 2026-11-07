"""The box/laser/circle append owners share a native 128-record query pool.

The physical COFF comparisons remain the exactness authority; this tests
only the source-facing allocation and three producer consumers.
"""
from pathlib import Path
import tomllib
import unittest

ROOT = Path(__file__).resolve().parents[1]


class PlayerCollisionQueryNativeCapacity(unittest.TestCase):
    def test_real_layout_contains_128_records_of_0x30_before_count(self):
        native = (ROOT/'src/PlayerLifecycleView.hpp').read_text()
        derived = (ROOT/'src/PlayerCollisionQueryCapacity.hpp').read_text()
        self.assertIn('PlayerCollisionQueryRecordView records[128]',native)
        self.assertIn('int count1804',native)
        self.assertIn('sizeof(((PlayerCollisionQueryStateView *)0)->records)',derived)
        self.assertIn('sizeof(((PlayerCollisionQueryStateView *)0)->records[0])',derived)
        self.assertIn('sizeof(PlayerCollisionQueryRecordView) == 0x30',derived)
        self.assertIn('offsetof(PlayerCollisionQueryStateView, count1804)',derived)
        self.assertIn('sizeof(PlayerCollisionQueryStateView) == 0x1808',derived)

    def test_all_three_append_owners_derive_same_signed_count_limit(self):
        files = ('EnemyManagerCore.inl','PlayerCollisionAppend.cpp',
                 'PlayerCollisionAppendCircle.cpp')
        for name in files:
            with self.subTest(source=name):
                text = (ROOT/'src'/name).read_text()
                self.assertIn('#include "PlayerCollisionQueryCapacity.hpp"',text)
                self.assertIn('>= PlayerCollisionQueryCapacity::RecordCount',text)
        circle = (ROOT/'src/PlayerCollisionAppendCircle.cpp').read_text()
        self.assertIn('PlayerCollisionQueryCapacity::RecordCount]',circle)
        self.assertIn('sizeof(PlayerCollisionCircleAppendView) == sizeof(PlayerCollisionQueryStateView)',circle)

    def test_three_complete_original_owner_units_stay_separately_registered(self):
        with (ROOT/'config/match-units.toml').open('rb') as stream:
            units=tomllib.load(stream)['units']
        targets=(('player-collision-append-box','src/EclManager.cpp',205),
                 ('player-collision-append-laser-record','src/PlayerCollisionAppend.cpp',245),
                 ('player-collision-append-circle','src/PlayerCollisionAppendCircle.cpp',95))
        for name, source, size in targets:
            with self.subTest(unit=name):
                unit=units[name]
                self.assertEqual(unit['source'],source)
                self.assertEqual(unit['size'],size)


if __name__ == '__main__':
    unittest.main()
