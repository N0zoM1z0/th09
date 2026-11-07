"""GameplaySetupThread publishes spawn state to real EnemyView fields.

The still-nonexact 1,689-byte Setup worker is protected separately by
its 172-field/35-call original-bound complete-body diagnostic.
"""
from pathlib import Path
import tomllib
import unittest

ROOT=Path(__file__).resolve().parents[1]

class SetupNativeSpawnedEnemy(unittest.TestCase):
    def test_real_enemy_allocation_not_a_private_335c_prefix(self):
        source=(ROOT/'src/GameManagerSetup.cpp').read_text()
        header=(ROOT/'src/EnemyManager.hpp').read_text()
        self.assertNotIn('struct SpawnedSideObjectView',source)
        self.assertNotIn('static_cast<SpawnedSideObjectView *>',source)
        self.assertIn('SetupNativeSpawnedEnemyFields',source)
        self.assertIn('offsetof(EnemyView, itemDropType335C) == 0x335C',source)
        self.assertIn('offsetof(EnemyView, secondaryFlags3380) == 0x3380',source)
        self.assertIn('EnemyView *object =',source)
        self.assertEqual(source.count('object->itemDropType335C'),2)
        self.assertEqual(source.count('object->secondaryFlags3380'),2)
        self.assertIn('int itemDropType335C;',header)
        self.assertIn('unsigned int secondaryFlags3380;',header)

    def test_original_bound_setup_and_spawn_leaf_metadata(self):
        with (ROOT/'config/match-units.toml').open('rb') as f:u=tomllib.load(f)['units']
        self.assertEqual(u['enemy-manager-spawn-record']['size'],364)
        self.assertEqual(u['enemy-manager-create']['size'],328)
        self.assertEqual(u['game-manager-on-update']['source'],'src/GameManagerSetup.cpp')

if __name__=='__main__': unittest.main()
