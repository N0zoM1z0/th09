"""Source-level native Enemy/ECL ExAttack ownership, with separate PE proof."""
from pathlib import Path
import tomllib
import unittest

ROOT=Path(__file__).resolve().parents[1]

class EnemyExAttackNativeStorage(unittest.TestCase):
    def test_six_exact_prefix_wrappers_use_native_enemy_fields(self):
        source=(ROOT/'src/EnemyExAttackLeaves.cpp').read_text()
        native=(ROOT/'src/EnemyManager.hpp').read_text()
        self.assertNotIn('struct EnemyExAttackView',source)
        self.assertNotIn('reinterpret_cast<EnemyExAttackView',source)
        self.assertIn('EnemyExAttackNativePositionAt2D74',source)
        self.assertIn('reinterpret_cast<const Float3 *>(&enemy->position2D74)',source)
        self.assertIn('1 - enemy->manager00->sideIndex31C',source)
        self.assertIn('offsetof(EnemyView, position2D74) == 0x2D74',source)
        self.assertIn('EnemyManagerView *manager00;',native)
        self.assertIn('EnemyFloat3 position2D74;',native)
        with (ROOT/'config/match-units.toml').open('rb') as f:
            units=tomllib.load(f)['units']
        for name in ('enemy-spawn-exattack-type1','enemy-spawn-exattack-type7',
                     'enemy-spawn-exattack-type8','enemy-spawn-exattack-type9',
                     'enemy-spawn-exattack-type23','enemy-spawn-exattack-type24'):
            with self.subTest(name=name):
                self.assertEqual(units[name]['size'],41)
                self.assertEqual(units[name]['source'],'src/EnemyExAttackLeaves.cpp')

if __name__=='__main__':unittest.main()
