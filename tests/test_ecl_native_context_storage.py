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

    def test_enemy_operand_banks_use_real_native_enemy_fields(self):
        enemy = (ROOT/'src/EnemyManager.hpp').read_text()
        resolver = (ROOT/'src/EclOperands.cpp').read_text()
        state = (ROOT/'src/EclRunState.inl').read_text()
        self.assertIn('EnemyViewNativeOperandBanks',enemy)
        self.assertIn('int intVariables2CE8[8];',enemy)
        self.assertIn('float floatVariables2D08[8];',enemy)
        self.assertIn('EnemyEclNativeOperandBanks',state)
        self.assertIn('enemy->intVariables2CE8[0]',resolver)
        self.assertIn('this->floatVariables2D08[7]',resolver)
        self.assertNotIn('TH09_ECL_OPERAND_ENEMY(enemy)->intVariables2CE8',resolver)
        self.assertNotIn('TH09_ECL_OPERAND_ENEMY(this)->floatVariables2D08',resolver)

    def test_remote_slot_uses_real_enemy_manager_array(self):
        remote=(ROOT/'src/EclRunRemote.inl').read_text()
        manager=(ROOT/'src/EnemyManager.hpp').read_text()
        self.assertIn('EnemyView *remoteSlots2AC388[9];',manager)
        self.assertIn('((owner)->manager00->remoteSlots2AC388[(index)])',remote)
        self.assertIn('return enemy->manager00->remoteSlots2AC388[slot];',remote)
        self.assertNotIn('ENEMY_MANAGER_REMOTE_TABLE',remote)
        self.assertNotIn('struct EnemyRemoteOffset',remote)
        self.assertNotIn('reinterpret_cast<EnemyView **>',remote)
        self.assertIn('TH09_ECL_REMOTE_SLOT(enemy, remoteIndex)',remote)

    def test_native_subroutine_copy_uses_same_context_fields(self):
        manager = (ROOT/'src/EclManager.cpp').read_text()
        state = (ROOT/'src/EclRunState.inl').read_text()
        self.assertIn('&view->activeContext2CE0->intVariables74[0]', manager)
        self.assertIn('offsetof(Th09EclContextView, intVariables1C)', state)
        self.assertNotIn('view->activeContext2CE0->unknown01C', manager)

    def test_opposing_player_initial_state_and_position_use_native_view(self):
        player=(ROOT/'src/EnemyManager.hpp').read_text()
        resolver=(ROOT/'src/EclOperands.cpp').read_text()
        added=(ROOT/'src/PlayerAddedState.cpp').read_text()
        self.assertIn('EnemyRewardPlayerNativeInitialStateAtA0',player)
        self.assertIn('int initialStateA0[2];',player)
        self.assertIn('typedef EnemyRewardPlayerView EclOperandPlayerView;',resolver)
        self.assertNotIn('struct EclOperandPlayerView\n{',resolver)
        self.assertIn('initialStateA0[0]',resolver)
        self.assertIn('initialStateA0[1]',resolver)
        self.assertIn('position1B88',resolver)
        self.assertIn('player->initialStateA0[0] = initialState;',added)
        self.assertIn('player->initialStateA0[1] = initialState;',added)

    def test_manager_timeline_is_one_native_owner_for_interpreter_and_leaves(self):
        native=(ROOT/'src/EnemyManager.hpp').read_text()
        operand=(ROOT/'src/EclOperands.cpp').read_text()
        interpreter=(ROOT/'src/EclManager.cpp').read_text()
        self.assertIn('typedef EnemyManagerView EclOperandManagerView;', operand)
        self.assertNotIn('struct EclOperandManagerView\n{', operand)
        self.assertIn('EnemyEclTimelineNativeSharedStorage', native)
        self.assertIn('EnemyEclManagerNativeSharedAt168', native)
        self.assertIn('int sharedInts160[4];', native)
        self.assertIn('float sharedFloats170[4];', native)
        self.assertIn('timelineState08.sharedInts160[', operand)
        self.assertIn('timelineState08.sharedFloats170[', operand)
        self.assertIn('selectedManager000->timelineState08.sharedInts160[0]', interpreter)
        self.assertIn('(sizeof(EnemyEclTimelineStateView) == 0x180)', native)

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
