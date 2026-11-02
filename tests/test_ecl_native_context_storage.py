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

    def test_post_ecl_movement_shares_enemy_update_core_record(self):
        core=(ROOT/'src/EclRunState.inl').read_text()
        post=(ROOT/'src/EclPostRuntime.inl').read_text()
        self.assertIn('typedef EnemyCoreView EnemyPostEclRuntimeView;',post)
        self.assertNotIn('struct EnemyPostEclRuntimeView\n{',post)
        for field in ('movementTimer2E28','movementDuration2E34',
                      'pendingShotInstruction3088','shootIntervalTimer30B8',
                      'movementDelta2E10','radialVelocity2E00',
                      'anmDirection3386','moveLeftAnm3390'):
            self.assertIn(field,core)
            self.assertIn(field,post)
        self.assertIn('sizeof(EnemyStateView) == 0x5430',core)

    def test_post_ecl_anm_vm_and_script_slots_are_native_enemy_fields(self):
        native=(ROOT/'src/EnemyManager.hpp').read_text()
        post=(ROOT/'src/EclPostRuntime.inl').read_text()
        self.assertIn('EnemyViewPrimaryAnmVmAt008',native)
        self.assertIn('AnmVm primaryAnmVm0008;',native)
        self.assertIn('EnemyViewSharedMovementAndPostAnmSlots',native)
        for member in ('idleInitialAnm338A','idleFromLeftAnm338C',
                       'idleFromRightAnm338E','moveLeftAnm3390','moveRightAnm3392'):
            self.assertIn('thisEnemy->'+member,post)
            self.assertIn(member,native)
        self.assertIn('&thisEnemy->primaryAnmVm0008',post)

    def test_bullet_and_laser_opcode_family_uses_real_enemy_descriptors(self):
        manager = (ROOT/'src/EnemyManager.hpp').read_text()
        bullet = (ROOT/'src/EclRunBullet.inl').read_text()
        post = (ROOT/'src/EclPostRuntime.inl').read_text()
        self.assertIn('typedef ::EnemyView EnemyBulletView;', bullet)
        self.assertNotIn('struct EnemyBulletView\n{', bullet)
        self.assertIn('EnemyViewBulletAndLaserDescriptors', manager)
        self.assertIn('BulletSpawnDescriptor bulletDescriptor2E74;', manager)
        self.assertIn('BulletSpawnDescriptor laserDescriptor30C4;', manager)
        self.assertIn('ZunTimer shootIntervalTimer30B8;', manager)
        self.assertIn('pendingShotInstruction3088[0x2C]', manager)
        self.assertIn('Th09EclRunBullet::View(enemy)->bulletDescriptor2E74', bullet)
        self.assertIn('reinterpret_cast<const Float3 &>(view->worldPosition2DD4)', post)

    def test_no_duplicate_enemy_resolver_owner(self):
        native=(ROOT/'src/EnemyManager.hpp').read_text()
        resolver=(ROOT/'src/EclOperands.cpp').read_text()
        self.assertIn('typedef EnemyView EclOperandEnemyView;',resolver)
        self.assertNotIn('struct EclOperandEnemyView\n{',resolver)
        self.assertIn('EnemyViewNativeEclRewardAndDamageFields',native)
        self.assertIn('int itemDropType335C;',native)
        self.assertIn('unsigned int defeatTokenType335C;',native)
        self.assertIn('unsigned char bossSlot336B;',native)
        self.assertIn('unsigned int secondaryFlags3380;',native)
        self.assertIn('unsigned int rewardFlags3380;',native)
        self.assertIn('int lastDamage33AC;',native)
        self.assertIn('int lifeCallbackThresholds33B0[4];',native)
        self.assertIn('reinterpret_cast<const EclOperandFloat3View &>',resolver)
        self.assertIn('enemy->secondaryFlags3380',resolver)

    def test_exact_resolvers_use_native_enemy_and_player_receivers(self):
        resolver=(ROOT/'src/EclOperands.cpp').read_text()
        self.assertNotIn('TH09_ECL_OPERAND_ENEMY(',resolver)
        self.assertNotIn('reinterpret_cast<EclOperandEnemyView *>',resolver)
        self.assertIn('enemy->secondaryFlags3380',resolver)
        self.assertIn('this->lifeCallbackThresholds33B0[0]',resolver)
        self.assertIn('((owner)->activeContext2CE0)',resolver)
        self.assertIn('((owner)->manager00->sideState320->player04)',resolver)

    def test_bullet_laser_family_shares_native_enemy_descriptors(self):
        native=(ROOT/'src/EnemyManager.hpp').read_text()
        bullet=(ROOT/'src/EclRunBullet.inl').read_text()
        post=(ROOT/'src/EclPostRuntime.inl').read_text()
        self.assertIn('#include "BulletManager.hpp"',native)
        self.assertIn('EnemyViewBulletAndLaserDescriptors',native)
        self.assertIn('BulletSpawnDescriptor bulletDescriptor2E74;',native)
        self.assertIn('BulletSpawnDescriptor laserDescriptor30C4;',native)
        self.assertIn('pendingShotInstruction3088[0x2C]',native)
        self.assertIn('ZunTimer shootIntervalTimer30B8;',native)
        self.assertIn('typedef ::EnemyView EnemyBulletView;',bullet)
        self.assertNotIn('struct EnemyBulletView\n{',bullet)
        self.assertIn('return enemy;',bullet)
        self.assertIn('reinterpret_cast<const Float3 &>(view->worldPosition2DD4)',post)

    def test_movement_opcode_reads_direct_native_flags(self):
        movement=(ROOT/'src/EclRunMovement.inl').read_text()
        self.assertIn('#define TH09_EXPR_MOVEMENT_VIEW(owner) ((owner))',movement)
        self.assertIn('(&(owner)->primaryFlags337C)',movement)
        self.assertIn('(&(owner)->secondaryFlags3380)',movement)
        self.assertNotIn('reinterpret_cast<Th09EclRunMovement::EnemyMovementView *>(',movement)

    def test_trail_and_attached_effect_region_is_native_enemy_storage(self):
        native=(ROOT/'src/EnemyManager.hpp').read_text()
        core=(ROOT/'src/EclRunState.inl').read_text()
        draw=(ROOT/'src/EnemyManagerDraw.cpp').read_text()
        self.assertIn('EnemyViewNativeTrailAndAttachedEffectOwnership',native)
        self.assertIn('EnemyTrailSampleView trailSamples33E8[96];',native)
        self.assertIn('EnemyTrailVertexView trailVertices3E68[192];',native)
        self.assertIn('typedef EnemyTrailSampleView EnemyCoreTrailSampleView;',core)
        self.assertIn('typedef EnemyTrailVertexView EnemyCoreTrailVertexView;',core)
        self.assertIn('EnemyCoreTrailSampleView trailSamples33E8[96];',core)
        self.assertIn('EnemyCoreTrailVertexView trailVertices3E68[192];',core)
        self.assertIn('enemy->trailVertices3E68',core)
        self.assertIn('&enemy->primaryAnmVm0008',core)
        for f,offset in (('trailSamples33E8','0x33E8'),
                         ('trailVertices3E68','0x3E68'),
                         ('trailFlags53A0','0x53A0'),
                         ('trailHistoryLength53A2','0x53A2'),
                         ('trailCollisionLength53A4','0x53A4'),
                         ('trailSampleStride53A6','0x53A6'),
                         ('damageReductionTimer53A8','0x53A8'),
                         ('attachedEffectState53B4','0x53B4'),
                         ('phaseEndSeconds5420','0x5420'),
                         ('specialAttackTimer5424','0x5424')):
            with self.subTest(field=f):
                self.assertIn('offsetof(EnemyView, '+f+') == '+offset,native)
        for f in ('attachedEffectState53B4','trailFlags53A0',
                  'trailHistoryLength53A2','trailCollisionLength53A4',
                  'trailSampleStride53A6','trailVertices3E68'):
            with self.subTest(source_field=f):
                self.assertIn('enemy->'+f,core)
                self.assertNotIn('Th09EclRunState::View(enemy)->'+f,core)
        self.assertIn('trailVertices3E68',draw)

    def test_ecl_life_rank_and_drop_fields_have_native_enemy_ownership(self):
        native=(ROOT/'src/EnemyManager.hpp').read_text()
        state=(ROOT/'src/EclRunState.inl').read_text()
        self.assertIn('EnemyViewNativeEclLifeRankAndDrops',native)
        for field,offset in (('bulletRankSpeedLow2E38','0x2E38'),
                             ('bulletRankCount1Low2E40','0x2E40'),
                             ('bulletRankCount2High2E46','0x2E46'),
                             ('maxLife2E4C','0x2E4C'),
                             ('phaseStartingLife2E50','0x2E50'),
                             ('primaryDamageAccumulator2E5C','0x2E5C'),
                             ('secondaryDamageAccumulator2E60','0x2E60'),
                             ('itemDropCount3360','0x3360'),
                             ('itemDropExtraCount3364','0x3364')):
            with self.subTest(field=field):
                self.assertIn('offsetof(EnemyView, '+field+') == '+offset,native)
        for field in ('life2E48','maxLife2E4C','phaseStartingLife2E50',
                      'bulletRankSpeedLow2E38','bulletRankSpeedHigh2E3C',
                      'bulletRankCount1Low2E40','bulletRankCount1High2E42',
                      'bulletRankCount2Low2E44','bulletRankCount2High2E46',
                      'lifeCallbackThresholds33B0','itemDropCount3360',
                      'itemDropExtraCount3364'):
            with self.subTest(source_field=field):
                self.assertIn('enemy->'+field,state)
                self.assertNotIn('Th09EclRunState::View(enemy)->'+field,state)
        # EclOperands' independently exact +0x2E6C current-frame accessor
        # must survive until the complete timer class can be proved.
        self.assertIn('ZunTimer bossTimer2E64;',native)
        self.assertIn('EnemyViewNativeBossTimerAt2E64',native)
        resolver=(ROOT/'src/EclOperands.cpp').read_text()
        self.assertIn('offsetof(EclOperandEnemyView, bossTimer2E64) + offsetof(ZunTimer, current)',resolver)
        self.assertIn('enemy->bossTimer2E64.current',resolver)
        self.assertIn('this->bossTimer2E64.current',resolver)
        self.assertIn('enemy->bossTimer2E64 =',state)
        self.assertNotIn('bossTimerCurrent2E6C',native)
        self.assertNotIn('Th09EclRunState::View(enemy)->bossTimer2E64',state)

    def test_ecl_callback_and_child_context_fields_share_native_enemy(self):
        native=(ROOT/'src/EnemyManager.hpp').read_text()
        core=(ROOT/'src/EclRunState.inl').read_text()
        interpreter=(ROOT/'src/EclManager.cpp').read_text()
        self.assertIn('EnemyViewNativeEclCallbacksAndChildren',native)
        for field,offset in (('lifeCallbackSubIds33C0','0x33C0'),
                             ('timerCallbackThreshold33D0','0x33D0'),
                             ('timerCallbackSubId33D4','0x33D4'),
                             ('childEclBlocks33D8','0x33D8')):
            with self.subTest(field=field):
                self.assertIn('offsetof(EnemyView, '+field+') == '+offset,native)
                self.assertIn(field,core)
        self.assertIn('void *childEclBlocks33D8[4];',native)
        self.assertIn('enemy->lifeCallbackSubIds33C0[stateIndex]',core)
        self.assertIn('enemy->timerCallbackThreshold33D0',core)
        self.assertIn('enemy->timerCallbackSubId33D4',core)
        self.assertIn('enemy->childEclBlocks33D8',core)
        self.assertIn('enemy->childEclBlocks33D8',interpreter)
        self.assertNotIn('enemyState->childEclBlocks33D8',interpreter)
        self.assertNotIn('view->childEclBlocks33D8',interpreter)

    def test_ecl_call_depth_and_subroutines_are_native_enemy_members(self):
        native=(ROOT/'src/EnemyManager.hpp').read_text()
        core=(ROOT/'src/EclRunState.inl').read_text()
        interpreter=(ROOT/'src/EclManager.cpp').read_text()
        self.assertIn('EnemyViewNativeEclCallDepthAndSubroutines',native)
        for field,off in (('mainCallDepth2D28','0x2D28'),
                          ('activeCallDepth2D2A','0x2D2A'),
                          ('deathCallbackSubId2D2E','0x2D2E'),
                          ('subroutineSlots2D30','0x2D30'),
                          ('pendingSubroutineId2D70','0x2D70')):
            with self.subTest(field=field):
                self.assertIn(field,native)
                self.assertIn('offsetof(EnemyView, '+field+') == '+off,native)
                self.assertIn(field,core)
        self.assertIn('short subroutineSlots2D30[32];',native)
        self.assertIn('enemy->activeCallDepth2D2A',interpreter)
        self.assertIn('enemy->subroutineSlots2D30[',interpreter)
        self.assertIn('enemy->pendingSubroutineId2D70',interpreter)
        self.assertNotIn('enemyState->mainCallDepth2D28',interpreter)
        self.assertIn('enemy->deathCallbackSubId2D2E',core)

    def test_native_main_context_and_sixteen_entry_call_stack(self):
        native=(ROOT/'src/EnemyManager.hpp').read_text()
        interpreter=(ROOT/'src/EclManager.cpp').read_text()
        core=(ROOT/'src/EclRunState.inl').read_text()
        protocol=(ROOT/'src/EclOpcodes.hpp').read_text()
        self.assertIn('#include "EclOpcodes.hpp"',native)
        self.assertIn('EnemyViewMainContextAndStackAt07F4',native)
        self.assertIn('Th09EclContextView mainContext07F4;',native)
        self.assertIn('Th09EclContextView mainCallStack0A20[TH09_ECL_MAIN_CALL_STACK_CAPACITY];',native)
        self.assertIn('TH09_ECL_MAIN_CALL_STACK_CAPACITY = 16',protocol)
        self.assertIn('sizeof(Th09EclContextView) == 0x22C',protocol)
        self.assertIn('enemy->mainCallStack0A20',interpreter)
        self.assertIn('&enemy->mainContext07F4',interpreter)
        self.assertIn('mainContext07F4',core)
        self.assertNotIn('enemyState->mainCallStack0A20',interpreter)

    def test_main_and_secondary_enemy_anm_vms_share_one_native_owner(self):
        native=(ROOT/'src/EnemyManager.hpp').read_text()
        movement=(ROOT/'src/EclRunMovement.inl').read_text()
        interpreter=(ROOT/'src/EclManager.cpp').read_text()
        core=(ROOT/'src/EclRunState.inl').read_text()
        self.assertIn('EnemyViewSecondaryAnmVmsAt2AC',native)
        self.assertIn('AnmVm primaryAnmVm0008;',native)
        self.assertIn('AnmVm secondaryVms2AC[2];',native)
        self.assertIn('AnmVm secondaryVms2AC[2];',core)
        self.assertIn('&enemy->primaryAnmVm0008',movement)
        self.assertIn('(&(owner)->primaryAnmVm0008)',movement)
        self.assertIn('&enemy->secondaryVms2AC[',interpreter)
        self.assertIn('enemy->secondaryVms2AC[',interpreter)
        self.assertNotIn('reinterpret_cast<unsigned char *>(enemy) + 0x2AC',interpreter)

    def test_movement_family_owns_one_native_enemy_geometry(self):
        native=(ROOT/'src/EnemyManager.hpp').read_text()
        movement=(ROOT/'src/EclRunMovement.inl').read_text()
        self.assertIn('typedef ::EnemyView EnemyMovementView;',movement)
        self.assertNotIn('struct EnemyMovementView\n{', movement)
        self.assertIn('typedef ::EnemyFloat2 EnemyFloat2;',movement)
        self.assertIn('EnemyViewMovementGeometryAt2DBC',native)
        self.assertIn('EnemyViewMovementScriptsAndBoundsAt3386',native)
        self.assertIn('movementTimer2E28[0x0C]',native)
        self.assertIn('EnemyFloat2 movementLowerBounds3398;',native)
        self.assertIn('EnemyFloat2 movementUpperBounds33A0;',native)
        self.assertIn('unsigned int primaryFlags337C;',native)
        self.assertIn('unsigned int secondaryFlags3380;',native)

    def test_movement_and_boss_timer_operands_use_native_enemy_fields(self):
        native=(ROOT/'src/EnemyManager.hpp').read_text()
        resolver=(ROOT/'src/EclOperands.cpp').read_text()
        self.assertIn('EnemyViewNativeMovementOperands',native)
        for name in ('floatVariables2DB0','movementAngle2DE0',
                     'angularVelocity2DE4','orbitAngle2DE8',
                     'orbitAngularVelocity2DEC','speed2DF4',
                     'acceleration2DF8','orbitRadius2DFC',
                     'floatVariables2E10','movementOrigin2E1C',
                     'operandValue2E54'):
            with self.subTest(field=name):
                self.assertIn(name, native)
                self.assertIn('->'+name, resolver)
                self.assertNotIn(f'TH09_ECL_OPERAND_ENEMY(enemy)->{name}',resolver)
                self.assertNotIn(f'TH09_ECL_OPERAND_ENEMY(this)->{name}',resolver)

    def test_boss_timer_shared_with_exact_ecl_operands(self):
        native=(ROOT/'src/EnemyManager.hpp').read_text()
        resolver=(ROOT/'src/EclOperands.cpp').read_text()
        state=(ROOT/'src/EclRunState.inl').read_text()
        self.assertIn('ZunTimer bossTimer2E64;',native)
        self.assertIn('EnemyViewNativeBossTimerAt2E64',native)
        self.assertIn('offsetof(EnemyView, bossTimer2E64) + offsetof(ZunTimer, current) == 0x2E6C',native)
        self.assertEqual(resolver.count('bossTimer2E64.current'),3)
        self.assertIn('enemy->bossTimer2E64 =',state)
        self.assertNotIn('bossTimerCurrent2E6C',resolver)

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
