"""Source-level native Enemy/ECL ExAttack ownership, with separate PE proof."""
from pathlib import Path
import tomllib
import unittest

ROOT=Path(__file__).resolve().parents[1]

class EnemyExAttackNativeStorage(unittest.TestCase):
    def test_four_exact_context_and_burst_callbacks_share_native_ecl_context(self):
        src=(ROOT/'src/EnemyExAttackMoreLeaves.cpp').read_text()
        ecl=(ROOT/'src/EclOpcodes.hpp').read_text()
        native=(ROOT/'src/EnemyManager.hpp').read_text()
        self.assertIn('EnemyExAttackMoreNativeContextFields',src)
        self.assertNotIn('struct EnemyExAttackMoreView',src)
        self.assertNotIn('struct EnemyExAttackContextValuesView',src)
        self.assertIn('enemy->activeContext2CE0->floatVariables6C[0]',src)
        self.assertIn('enemy->activeContext2CE0->floatVariables6C[1]',src)
        self.assertIn('enemy->activeContext2CE0->floatVariables3C[0]',src)
        self.assertIn('reinterpret_cast<const Float3 &>(enemy->position2D74)',src)
        self.assertIn('reinterpret_cast<const Float3 *>(&enemy->position2D74)',src)
        self.assertIn('enemy->manager00->sideIndex31C',src)
        self.assertIn('offsetof(Th09EclContextView, floatVariables6C) == 0x6C',src)
        self.assertIn('float floatVariables6C[2];',ecl)
        self.assertIn('Th09EclContextView *activeContext2CE0;',native)
        with (ROOT/'config/match-units.toml').open('rb') as f:
            units=tomllib.load(f)['units']
        for name,size in [('enemy-spawn-exattack-type10',70),
                          ('enemy-spawn-exattack-type11',69),
                          ('enemy-spawn-exattack-type12',69),
                          ('enemy-spawn-exattack-type22-burst',226)]:
            with self.subTest(unit=name):
                self.assertEqual(units[name]['size'],size)
                self.assertEqual(units[name]['source'],'src/EnemyExAttackMoreLeaves.cpp')

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
