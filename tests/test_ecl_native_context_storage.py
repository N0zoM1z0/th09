"""Original-bound shared ECL context/laser storage contracts.

The complete source-cold comparison is run by the focused RunEcl and Enemy
boss gates, and all four EclOperands canonical units. These assertions catch
private layout reintroduction before expensive compiler replay.
"""
from pathlib import Path
import tomllib
import unittest

ROOT = Path(__file__).resolve().parents[1]


class NativeEclContextStorage(unittest.TestCase):
    def test_interpreter_and_resolver_use_one_complete_context(self):
        context = (ROOT/'src/EclOpcodes.hpp').read_text()
        operands = (ROOT/'src/EclOperands.cpp').read_text()
        self.assertIn('typedef Th09EclContextView EclOperandContextView;', operands)
        self.assertNotIn('struct EclOperandContextView\n{', operands)
        self.assertIn('Th09EclContextNativeVariableFields', context)
        for field, count in (('intVariables1C',8), ('floatVariables3C',8),
                             ('intVariables5C',4), ('floatVariables6C',2),
                             ('intVariables74',4), ('floatVariables84',4)):
            self.assertIn(f'{field}[{count}];', context)
            self.assertIn(f'offsetof(Th09EclContextView, {field})', context)
        self.assertIn('(sizeof(Th09EclContextView) == 0x22C)', context)

    def test_laser_condition_and_spawn_share_justified_offsets(self):
        remote = (ROOT/'src/EclRunRemote.inl').read_text()
        bullet = (ROOT/'src/EclRunBullet.inl').read_text()
        enemy = (ROOT/'src/EnemyManager.hpp').read_text()
        state = (ROOT/'src/EclRunState.inl').read_text()
        self.assertIn('return ActiveContext(enemy)->intVariables1C;', remote)
        self.assertIn('return ActiveContext(enemy)->intVariables5C[2];', bullet)
        self.assertIn('EnemyViewLaserSlotsAt32D8', enemy)
        self.assertIn('EnemyViewSelectedLaserAt3358', enemy)
        self.assertIn('EnemyEclNativeLaserSlotsAt32D8', state)
        self.assertIn('enemy->laserSlots32D8[laserIndex]', bullet)

    def test_native_subroutine_copy_uses_same_context_fields(self):
        manager = (ROOT/'src/EclManager.cpp').read_text()
        state = (ROOT/'src/EclRunState.inl').read_text()
        self.assertIn('&view->activeContext2CE0->intVariables74[0]', manager)
        self.assertIn('offsetof(Th09EclContextView, intVariables1C)', state)
        self.assertNotIn('view->activeContext2CE0->unknown01C', manager)

    def test_four_exact_operand_owners_remain_registered(self):
        with (ROOT/'config/match-units.toml').open('rb') as f:
            units=tomllib.load(f)['units']
        expected={
            'ecl-resolve-int': (1980,165),
            'ecl-resolve-int-lvalue': (726,39),
            'ecl-resolve-float-lvalue': (968,49),
            'ecl-resolve-float': (2044,121),
        }
        for name,(physical,fields) in expected.items():
            with self.subTest(name=name):
                item=units[name]
                self.assertEqual(item['source'],'src/EclOperands.cpp')
                self.assertEqual(item['object'],'build/matching/EclOperands.obj')
                self.assertEqual(item.get('compare_size',item['size']),physical)
                self.assertEqual(len(item['relocations']),fields)


if __name__=='__main__':
    unittest.main()
