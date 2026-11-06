"""Guard the actual Player collision owner used by Etama's box/laser appends."""
from pathlib import Path
import tomllib
import unittest

ROOT = Path(__file__).resolve().parents[1]


class BulletNativePlayerCollision(unittest.TestCase):
    def test_real_player_collision_query_replaces_four_raw_views(self):
        source = (ROOT/'src/BulletManager.cpp').read_text()
        native = (ROOT/'src/PlayerLifecycleView.hpp').read_text()
        self.assertIn('#include "PlayerLifecycleView.hpp"', source)
        self.assertIn('BulletNativePlayerCollisionQueryAt36C', source)
        self.assertIn('offsetof(PlayerLifecycleView, collisionQuery36C) == 0x36C', source)
        self.assertIn('PlayerCollisionQueryStateView collisionQuery36C;', native)
        self.assertIn('void AppendBoxRecord(', native)
        self.assertIn('void AppendLaserRecord(', native)
        self.assertNotIn('struct PlayerCollisionQueryStateView\n{', source)
        self.assertNotIn('reinterpret_cast<unsigned char *>(controller->sideState->player04) + 0x36C', source)
        self.assertEqual(source.count('controller->sideState->player04->collisionQuery36C.'), 4)

    def test_complete_unchanged_boss_and_spawn_referent_counts(self):
        with (ROOT/'config/match-units.toml').open('rb') as f:
            units = tomllib.load(f)['units']
        for name, expected in (
            ('etama-controller-update', (0x00413530, 2216, 88)),
            ('etama-spawn-single-bullet', (0x00412960, 1932, 47))):
            with self.subTest(unit=name):
                u = units[name]
                self.assertEqual(u['source'], 'src/BulletManager.cpp')
                self.assertEqual(u['target_address'], expected[0])
                self.assertEqual(u.get('compare_size', u['size']), expected[1])
                self.assertEqual(len(u['relocations']), expected[2])


if __name__ == '__main__':
    unittest.main()
