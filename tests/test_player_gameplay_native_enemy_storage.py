"""Keep the Player selector on the actual shared Enemy owner layouts.

Original byte and reloc equivalence still needs the focused compiled match
units; these source guards catch accidental reintroduction of raw side casts.
"""
from pathlib import Path
import tomllib
import unittest

ROOT = Path(__file__).resolve().parents[1]


class PlayerSelectorNativeEnemyStorage(unittest.TestCase):
    def test_selector_consumes_actual_enemy_manager_and_enemy_records(self):
        gameplay = (ROOT/'src/PlayerGameplayState.cpp').read_text()
        side = (ROOT/'src/GameManagerSideState.hpp').read_text()
        enemy = (ROOT/'src/EnemyManager.hpp').read_text()
        player = (ROOT/'src/PlayerLifecycleView.hpp').read_text()

        self.assertIn('#include "EnemyManager.hpp"', gameplay)
        self.assertIn('PlayerSelectorNativeEnemyOwnerFields', gameplay)
        self.assertIn('PlayerSelectorNativeEnemyXYZWindow', gameplay)
        for token in ('enemyManager10', 'rewardEnemyCount2AC3B8',
                      'activeEnemyCount2AC3AC', 'priorityEnemy2AC444',
                      'firstActiveEnemy2AC448', 'secondaryFlags3380',
                      'position2D74', 'targetOverride30F64'):
            with self.subTest(member=token):
                self.assertIn(token, gameplay)
        for offset in ('0x2AC3AC', '0x2AC3B8', '0x2AC444', '0x2AC448',
                       '0x2D74', '0x3380', '0x30F64'):
            self.assertNotIn(' + '+offset, gameplay)
        self.assertIn('EnemyManagerView *enemyManager10;', side)
        self.assertIn('EnemyView *priorityEnemy2AC444;', enemy)
        self.assertIn('EnemyView *firstActiveEnemy2AC448;', enemy)
        self.assertIn('PlayerPositionView targetOverride30F64;', player)
        # +0x3044C and +0xC110 still lack a uniquely proven owner type.
        self.assertIn(' + 0x3044C', gameplay)
        self.assertIn(' + 0xC110', gameplay)

    def test_source_label_churn_retains_every_original_table_destination(self):
        with (ROOT/'config/match-units.toml').open('rb') as f:
            units = tomllib.load(f)['units']
        boss, leaf = (units[name] for name in
                      ('player-update-selector-state', 'player-resolve-pattern-offset'))
        self.assertEqual((boss['target_address'],boss['size'],len(boss['relocations'])),
                         (0x4049A0, 3238, 136))
        self.assertEqual((leaf['target_address'],leaf['size'],leaf['compare_size']),
                         (0x404710, 346, 412))
        original_targets = (0x40486C, 0x40488C, 0x404767, 0x404777,
                            0x404755, 0x404745, 0x404787, 0x4047AB,
                            0x404797, 0x4047BD, 0x4047F1, 0x4047FE,
                            0x4047E2, 0x4047D5, 0x40480B, 0x404829,
                            0x404818, 0x404838)
        self.assertEqual(tuple(r['target'] for r in leaf['relocations']),
                         original_targets)
        self.assertTrue(all(r['symbol'].startswith('$L') and
                            r['type']=='DIR32' for r in leaf['relocations']))


if __name__ == '__main__':
    unittest.main()
