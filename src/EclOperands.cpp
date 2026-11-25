#include "EclRunControl.inl"
#include "EclOperandIds.hpp"
#include "AsciiGameManagerView.hpp"
#include "AsciiManager.hpp"
#include "PlayerAngleView.hpp"

struct D3DXVECTOR3;
float __fastcall D3DXVec3Length(const D3DXVECTOR3 *value);

#include <stddef.h>

namespace
{

// The four exact ECL operand resolvers read the very same 0x22C
// interpreter context copied by RunEcl, not an invented context prefix.
typedef Th09EclContextView EclOperandContextView;
typedef char EclOperandContextInt1C[
    (offsetof(EclOperandContextView, intVariables1C) == 0x1C) ? 1 : -1];
typedef char EclOperandContextFloat3C[
    (offsetof(EclOperandContextView, floatVariables3C) == 0x3C) ? 1 : -1];
typedef char EclOperandContextInt5C[
    (offsetof(EclOperandContextView, intVariables5C) == 0x5C) ? 1 : -1];
typedef char EclOperandContextFloat6C[
    (offsetof(EclOperandContextView, floatVariables6C) == 0x6C) ? 1 : -1];
typedef char EclOperandContextInt74[
    (offsetof(EclOperandContextView, intVariables74) == 0x74) ? 1 : -1];
typedef char EclOperandContextFloat84[
    (offsetof(EclOperandContextView, floatVariables84) == 0x84) ? 1 : -1];

typedef Float3 EclOperandFloat3View;
typedef char EclOperandFloat3SizeIs0C[
    (sizeof(EclOperandFloat3View) == 0x0C) ? 1 : -1];

// One actual TH09 Player allocation supplies the main and opposing side;
// exact resolver inputs at +0xA0/+0xA4 were also observed in AddedState.
typedef EnemyRewardPlayerView EclOperandPlayerView;
typedef char EclOperandPlayerValueA0[
    (offsetof(EclOperandPlayerView, initialStateA0) == 0x00A0) ? 1 : -1];
typedef char EclOperandPlayerPositionAt1B88[
    (offsetof(EclOperandPlayerView, position1B88) == 0x1B88) ? 1 : -1];

// The exact operand resolver's +0x168/+0x178 values are the actual ECL
// timeline subobject of its native 0x2AC450 EnemyManager, not a second
// 0x328-byte manager prefix copied into this source translation unit.
typedef EnemyManagerView EclOperandManagerView;
typedef char EclOperandManagerSharedIntsAt168[
    (offsetof(EclOperandManagerView, primaryEclManager000) +
     offsetof(EnemyEclManagerView, timelineState08) +
     offsetof(EnemyEclTimelineStateView, sharedInts160) == 0x0168) ? 1 : -1];
typedef char EclOperandManagerSharedFloatsAt178[
    (offsetof(EclOperandManagerView, primaryEclManager000) +
     offsetof(EnemyEclManagerView, timelineState08) +
     offsetof(EnemyEclTimelineStateView, sharedFloats170) == 0x0178) ? 1 : -1];
typedef char EclOperandManagerSideAt320[
    (offsetof(EclOperandManagerView, sideState320) == 0x0320) ? 1 : -1];

// All known operands in this source map to the actual EnemyView object.
// Do not maintain a competing +0x33C0 Enemy view in the resolver TU.
typedef EnemyView EclOperandEnemyView;
typedef char EclOperandEnemyActiveContextAt2CE0[
    (offsetof(EclOperandEnemyView, activeContext2CE0) == 0x2CE0) ? 1 : -1];
typedef char EclOperandEnemyIntsAt2CE8[
    (offsetof(EclOperandEnemyView, intVariables2CE8) == 0x2CE8) ? 1 : -1];
typedef char EclOperandEnemyFloatsAt2D08[
    (offsetof(EclOperandEnemyView, floatVariables2D08) == 0x2D08) ? 1 : -1];
typedef char EclOperandEnemyPositionAt2D74[
    (offsetof(EclOperandEnemyView, position2D74) == 0x2D74) ? 1 : -1];
typedef char EclOperandEnemyWorldAt2DD4[
    (offsetof(EclOperandEnemyView, worldPosition2DD4) == 0x2DD4) ? 1 : -1];
typedef char EclOperandEnemyMovementAt2DE0[
    (offsetof(EclOperandEnemyView, movementAngle2DE0) == 0x2DE0) ? 1 : -1];
typedef char EclOperandEnemyLifeAt2E48[
    (offsetof(EclOperandEnemyView, life2E48) == 0x2E48) ? 1 : -1];
typedef char EclOperandEnemyTimerCurrentAt2E6C[
    (offsetof(EclOperandEnemyView, bossTimer2E64) + offsetof(ZunTimer, current) == 0x2E6C) ? 1 : -1];
typedef char EclOperandEnemyBossSlotAt336B[
    (offsetof(EclOperandEnemyView, bossSlot336B) == 0x336B) ? 1 : -1];
typedef char EclOperandEnemyFlagsAt3380[
    (offsetof(EclOperandEnemyView, secondaryFlags3380) == 0x3380) ? 1 : -1];
typedef char EclOperandEnemyLastDamageAt33AC[
    (offsetof(EclOperandEnemyView, lastDamage33AC) == 0x33AC) ? 1 : -1];
typedef char EclOperandEnemyLifeThresholdsAt33B0[
    (offsetof(EclOperandEnemyView, lifeCallbackThresholds33B0) == 0x33B0) ? 1 : -1];

// The owner and both side Players are the same real ECL and Player
// allocations used by the interpreter; access their members directly.



// Opcode 0x2739 reads the native GameManager's +0xB4 signed word.
typedef char EclOperandNativeGameManagerB4[
    (offsetof(AsciiGameManagerView, valueB4) == 0x00B4) ? 1 : -1];

} // namespace

namespace Th09EclRunControl
{

int __fastcall ResolveInt(EnemyView *enemy, int rawValue)
{
    switch (rawValue)
    {
    case TH09_ECL_OPERAND_LOCAL_INT_0: return enemy->activeContext2CE0->intVariables1C[0];
    case TH09_ECL_OPERAND_LOCAL_INT_1: return enemy->activeContext2CE0->intVariables1C[1];
    case TH09_ECL_OPERAND_LOCAL_INT_2: return enemy->activeContext2CE0->intVariables1C[2];
    case TH09_ECL_OPERAND_LOCAL_INT_3: return enemy->activeContext2CE0->intVariables1C[3];
    case TH09_ECL_OPERAND_LOCAL_INT_4: return enemy->activeContext2CE0->intVariables1C[4];
    case TH09_ECL_OPERAND_LOCAL_INT_5: return enemy->activeContext2CE0->intVariables1C[5];
    case TH09_ECL_OPERAND_LOCAL_INT_6: return enemy->activeContext2CE0->intVariables1C[6];
    case TH09_ECL_OPERAND_LOCAL_INT_7: return enemy->activeContext2CE0->intVariables1C[7];

    case TH09_ECL_OPERAND_ENEMY_INT_0: return enemy->intVariables2CE8[0];
    case TH09_ECL_OPERAND_ENEMY_INT_1: return enemy->intVariables2CE8[1];
    case TH09_ECL_OPERAND_ENEMY_INT_2: return enemy->intVariables2CE8[2];
    case TH09_ECL_OPERAND_ENEMY_INT_3: return enemy->intVariables2CE8[3];
    case TH09_ECL_OPERAND_ENEMY_INT_4: return enemy->intVariables2CE8[4];
    case TH09_ECL_OPERAND_ENEMY_INT_5: return enemy->intVariables2CE8[5];
    case TH09_ECL_OPERAND_ENEMY_INT_6: return enemy->intVariables2CE8[6];
    case TH09_ECL_OPERAND_ENEMY_INT_7: return enemy->intVariables2CE8[7];

    case TH09_ECL_OPERAND_CALL_INT_0: return enemy->activeContext2CE0->intVariables74[0];
    case TH09_ECL_OPERAND_CALL_INT_1: return enemy->activeContext2CE0->intVariables74[1];
    case TH09_ECL_OPERAND_CALL_INT_2: return enemy->activeContext2CE0->intVariables74[2];
    case TH09_ECL_OPERAND_CALL_INT_3: return enemy->activeContext2CE0->intVariables74[3];

    case TH09_ECL_OPERAND_EXTRA_INT_0: return enemy->activeContext2CE0->intVariables5C[0];
    case TH09_ECL_OPERAND_EXTRA_INT_1: return enemy->activeContext2CE0->intVariables5C[1];
    case TH09_ECL_OPERAND_EXTRA_INT_2: return enemy->activeContext2CE0->intVariables5C[2];
    case TH09_ECL_OPERAND_EXTRA_INT_3: return enemy->activeContext2CE0->intVariables5C[3];

    case TH09_ECL_OPERAND_RANDOM_NONNEGATIVE_INT: return static_cast<int>(g_ReplayRng.GetRandomU32() & 0x7FFFFFFF);
    case TH09_ECL_OPERAND_RANDOM_UNIT_FLOAT: return static_cast<int>(g_ReplayRng.GetRandomF32());
    case TH09_ECL_OPERAND_RANDOM_RAW_INT: return static_cast<int>(g_ReplayRng.GetRandomU32());
    case TH09_ECL_OPERAND_RANDOM_SIGNED_UNIT_FLOAT: return static_cast<int>(g_ReplayRng.GetRandomF32Signed());

    case TH09_ECL_OPERAND_DIFFICULTY: return g_GameManager.difficulty;
    case TH09_ECL_OPERAND_GAME_MANAGER_VALUE_B4: return g_GameManager.valueB4;
    case TH09_ECL_OPERAND_BOSS_TIMER: return enemy->bossTimer2E64.current;
    case TH09_ECL_OPERAND_LIFE: return enemy->life2E48;
    case TH09_ECL_OPERAND_CHARACTER_INDEX: return enemy->manager00->sideState320->characterIndex20;

    case TH09_ECL_OPERAND_EXTRA_FLOAT_0: return static_cast<int>(enemy->activeContext2CE0->floatVariables6C[0]);
    case TH09_ECL_OPERAND_EXTRA_FLOAT_1: return static_cast<int>(enemy->activeContext2CE0->floatVariables6C[1]);

    case TH09_ECL_OPERAND_LOCAL_FLOAT_0: return static_cast<int>(enemy->activeContext2CE0->floatVariables3C[0]);
    case TH09_ECL_OPERAND_LOCAL_FLOAT_1: return static_cast<int>(enemy->activeContext2CE0->floatVariables3C[1]);
    case TH09_ECL_OPERAND_LOCAL_FLOAT_2: return static_cast<int>(enemy->activeContext2CE0->floatVariables3C[2]);
    case TH09_ECL_OPERAND_LOCAL_FLOAT_3: return static_cast<int>(enemy->activeContext2CE0->floatVariables3C[3]);
    case TH09_ECL_OPERAND_LOCAL_FLOAT_4: return static_cast<int>(enemy->activeContext2CE0->floatVariables3C[4]);
    case TH09_ECL_OPERAND_LOCAL_FLOAT_5: return static_cast<int>(enemy->activeContext2CE0->floatVariables3C[5]);
    case TH09_ECL_OPERAND_LOCAL_FLOAT_6: return static_cast<int>(enemy->activeContext2CE0->floatVariables3C[6]);
    case TH09_ECL_OPERAND_LOCAL_FLOAT_7: return static_cast<int>(enemy->activeContext2CE0->floatVariables3C[7]);

    case TH09_ECL_OPERAND_ENEMY_FLOAT_0: return static_cast<int>(enemy->floatVariables2D08[0]);
    case TH09_ECL_OPERAND_ENEMY_FLOAT_1: return static_cast<int>(enemy->floatVariables2D08[1]);
    case TH09_ECL_OPERAND_ENEMY_FLOAT_2: return static_cast<int>(enemy->floatVariables2D08[2]);
    case TH09_ECL_OPERAND_ENEMY_FLOAT_3: return static_cast<int>(enemy->floatVariables2D08[3]);
    case TH09_ECL_OPERAND_ENEMY_FLOAT_4: return static_cast<int>(enemy->floatVariables2D08[4]);
    case TH09_ECL_OPERAND_ENEMY_FLOAT_5: return static_cast<int>(enemy->floatVariables2D08[5]);
    case TH09_ECL_OPERAND_ENEMY_FLOAT_6: return static_cast<int>(enemy->floatVariables2D08[6]);
    case TH09_ECL_OPERAND_ENEMY_FLOAT_7: return static_cast<int>(enemy->floatVariables2D08[7]);

    case TH09_ECL_OPERAND_CALL_FLOAT_0: return static_cast<int>(enemy->activeContext2CE0->floatVariables84[0]);
    case TH09_ECL_OPERAND_CALL_FLOAT_1: return static_cast<int>(enemy->activeContext2CE0->floatVariables84[1]);
    case TH09_ECL_OPERAND_CALL_FLOAT_2: return static_cast<int>(enemy->activeContext2CE0->floatVariables84[2]);
    case TH09_ECL_OPERAND_CALL_FLOAT_3: return static_cast<int>(enemy->activeContext2CE0->floatVariables84[3]);

    case TH09_ECL_OPERAND_SHARED_INT_0: return enemy->manager00->primaryEclManager000.timelineState08.sharedInts160[0];
    case TH09_ECL_OPERAND_SHARED_INT_1: return enemy->manager00->primaryEclManager000.timelineState08.sharedInts160[1];
    case TH09_ECL_OPERAND_SHARED_INT_2: return enemy->manager00->primaryEclManager000.timelineState08.sharedInts160[2];
    case TH09_ECL_OPERAND_SHARED_INT_3: return enemy->manager00->primaryEclManager000.timelineState08.sharedInts160[3];
    case TH09_ECL_OPERAND_SHARED_FLOAT_0: return static_cast<int>(enemy->manager00->primaryEclManager000.timelineState08.sharedFloats170[0]);
    case TH09_ECL_OPERAND_SHARED_FLOAT_1: return static_cast<int>(enemy->manager00->primaryEclManager000.timelineState08.sharedFloats170[1]);
    case TH09_ECL_OPERAND_SHARED_FLOAT_2: return static_cast<int>(enemy->manager00->primaryEclManager000.timelineState08.sharedFloats170[2]);
    case TH09_ECL_OPERAND_SHARED_FLOAT_3: return static_cast<int>(enemy->manager00->primaryEclManager000.timelineState08.sharedFloats170[3]);

    case TH09_ECL_OPERAND_ENEMY_POSITION_X: return static_cast<int>(enemy->worldPosition2DD4.x);
    case TH09_ECL_OPERAND_ENEMY_POSITION_Y: return static_cast<int>(enemy->worldPosition2DD4.y);
    case TH09_ECL_OPERAND_ENEMY_POSITION_Z: return static_cast<int>(enemy->worldPosition2DD4.z);
    case TH09_ECL_OPERAND_PLAYER_POSITION_X: return static_cast<int>(enemy->manager00->sideState320->rewardPlayer04->position1B88.x);
    case TH09_ECL_OPERAND_PLAYER_POSITION_Y: return static_cast<int>(enemy->manager00->sideState320->rewardPlayer04->position1B88.y);
    case TH09_ECL_OPERAND_PLAYER_POSITION_Z: return static_cast<int>(enemy->manager00->sideState320->rewardPlayer04->position1B88.z);

    case TH09_ECL_OPERAND_MOVEMENT_ORIGIN_X: return static_cast<int>(enemy->movementOrigin2E1C.x);
    case TH09_ECL_OPERAND_MOVEMENT_ORIGIN_Y: return static_cast<int>(enemy->movementOrigin2E1C.y);
    case TH09_ECL_OPERAND_MOVEMENT_ORIGIN_Z: return static_cast<int>(enemy->movementOrigin2E1C.z);

    case TH09_ECL_OPERAND_ENEMY_FLOAT_2DB0_0: return static_cast<int>(enemy->floatVariables2DB0[0]);
    case TH09_ECL_OPERAND_ENEMY_FLOAT_2DB0_1: return static_cast<int>(enemy->floatVariables2DB0[1]);
    case TH09_ECL_OPERAND_ENEMY_FLOAT_2DB0_2: return static_cast<int>(enemy->floatVariables2DB0[2]);

    case TH09_ECL_OPERAND_LIFE_CALLBACK_THRESHOLD_0: return enemy->lifeCallbackThresholds33B0[0];
    case TH09_ECL_OPERAND_LIFE_CALLBACK_THRESHOLD_1: return enemy->lifeCallbackThresholds33B0[1];
    case TH09_ECL_OPERAND_LIFE_CALLBACK_THRESHOLD_2: return enemy->lifeCallbackThresholds33B0[2];
    case TH09_ECL_OPERAND_LIFE_CALLBACK_THRESHOLD_3: return enemy->lifeCallbackThresholds33B0[3];

    case TH09_ECL_OPERAND_MOVEMENT_ANGLE: return static_cast<int>(enemy->movementAngle2DE0);
    case TH09_ECL_OPERAND_ANGULAR_VELOCITY: return static_cast<int>(enemy->angularVelocity2DE4);
    case TH09_ECL_OPERAND_SPEED: return static_cast<int>(enemy->speed2DF4);
    case TH09_ECL_OPERAND_ACCELERATION: return static_cast<int>(enemy->acceleration2DF8);
    case TH09_ECL_OPERAND_ORBIT_RADIUS: return static_cast<int>(enemy->orbitRadius2DFC);
    case TH09_ECL_OPERAND_ORBIT_ANGLE: return static_cast<int>(enemy->orbitAngle2DE8);
    case TH09_ECL_OPERAND_ORBIT_ANGULAR_VELOCITY: return static_cast<int>(enemy->orbitAngularVelocity2DEC);

    case TH09_ECL_OPERAND_LAST_DAMAGE: return enemy->lastDamage33AC;
    case TH09_ECL_OPERAND_BOSS_SLOT: return enemy->bossSlot336B;
    case TH09_ECL_OPERAND_ITEM_DROP_TYPE: return enemy->itemDropType335C;
    case TH09_ECL_OPERAND_ENEMY_VALUE_2E54: return enemy->operandValue2E54;

    case TH09_ECL_OPERAND_OPPOSING_PLAYER_INITIAL_STATE:
        return enemy->secondaryFlagBits3380.category.sideCategory10_11 < 3
                   ? enemy->manager00->opposingSideState324->rewardPlayer04->initialStateA0[0]
                   : enemy->manager00->opposingSideState324->rewardPlayer04->initialStateA0[1];

    case TH09_ECL_OPERAND_ANGLE_TO_PLAYER:
        return static_cast<int>(reinterpret_cast<PlayerAngleView *>(enemy->manager00->sideState320->rewardPlayer04)->AngleToPoint(
            reinterpret_cast<PlayerAngleFloat3View *>(&enemy->worldPosition2DD4)));

    case TH09_ECL_OPERAND_DISTANCE_TO_PLAYER:
    {
        EclOperandFloat3View delta = enemy->manager00->sideState320->rewardPlayer04->position1B88 - reinterpret_cast<const EclOperandFloat3View &>(enemy->worldPosition2DD4);
        return static_cast<int>(D3DXVec3Length(reinterpret_cast<const D3DXVECTOR3 *>(&delta)));
    }

    default: return rawValue;
    }
}

int *__fastcall ResolveIntLValue(
    EnemyView *enemy,
    int *rawValue,
    unsigned short parameterMask,
    int operandIndex)
{
    if (operandIndex >= 0 && !(parameterMask & (1U << operandIndex)))
        return rawValue;

    switch (*rawValue)
    {
    case TH09_ECL_OPERAND_LOCAL_INT_0: return &enemy->activeContext2CE0->intVariables1C[0];
    case TH09_ECL_OPERAND_LOCAL_INT_1: return &enemy->activeContext2CE0->intVariables1C[1];
    case TH09_ECL_OPERAND_LOCAL_INT_2: return &enemy->activeContext2CE0->intVariables1C[2];
    case TH09_ECL_OPERAND_LOCAL_INT_3: return &enemy->activeContext2CE0->intVariables1C[3];
    case TH09_ECL_OPERAND_LOCAL_INT_4: return &enemy->activeContext2CE0->intVariables1C[4];
    case TH09_ECL_OPERAND_LOCAL_INT_5: return &enemy->activeContext2CE0->intVariables1C[5];
    case TH09_ECL_OPERAND_LOCAL_INT_6: return &enemy->activeContext2CE0->intVariables1C[6];
    case TH09_ECL_OPERAND_LOCAL_INT_7: return &enemy->activeContext2CE0->intVariables1C[7];

    case TH09_ECL_OPERAND_ENEMY_INT_0: return &enemy->intVariables2CE8[0];
    case TH09_ECL_OPERAND_ENEMY_INT_1: return &enemy->intVariables2CE8[1];
    case TH09_ECL_OPERAND_ENEMY_INT_2: return &enemy->intVariables2CE8[2];
    case TH09_ECL_OPERAND_ENEMY_INT_3: return &enemy->intVariables2CE8[3];
    case TH09_ECL_OPERAND_ENEMY_INT_4: return &enemy->intVariables2CE8[4];
    case TH09_ECL_OPERAND_ENEMY_INT_5: return &enemy->intVariables2CE8[5];
    case TH09_ECL_OPERAND_ENEMY_INT_6: return &enemy->intVariables2CE8[6];
    case TH09_ECL_OPERAND_ENEMY_INT_7: return &enemy->intVariables2CE8[7];

    case TH09_ECL_OPERAND_CALL_INT_0: return &enemy->activeContext2CE0->intVariables74[0];
    case TH09_ECL_OPERAND_CALL_INT_1: return &enemy->activeContext2CE0->intVariables74[1];
    case TH09_ECL_OPERAND_CALL_INT_2: return &enemy->activeContext2CE0->intVariables74[2];
    case TH09_ECL_OPERAND_CALL_INT_3: return &enemy->activeContext2CE0->intVariables74[3];

    case TH09_ECL_OPERAND_EXTRA_INT_0: return &enemy->activeContext2CE0->intVariables5C[0];
    case TH09_ECL_OPERAND_EXTRA_INT_1: return &enemy->activeContext2CE0->intVariables5C[1];
    case TH09_ECL_OPERAND_EXTRA_INT_2: return &enemy->activeContext2CE0->intVariables5C[2];
    case TH09_ECL_OPERAND_EXTRA_INT_3: return &enemy->activeContext2CE0->intVariables5C[3];

    case TH09_ECL_OPERAND_DIFFICULTY: return &g_GameManager.difficulty;
    case TH09_ECL_OPERAND_GAME_MANAGER_VALUE_B4: return &g_GameManager.valueB4;
    case TH09_ECL_OPERAND_BOSS_TIMER: return &enemy->bossTimer2E64.current;
    case TH09_ECL_OPERAND_LIFE: return &enemy->life2E48;
    case TH09_ECL_OPERAND_ITEM_DROP_TYPE: return &enemy->itemDropType335C;
    case TH09_ECL_OPERAND_ENEMY_VALUE_2E54: return &enemy->operandValue2E54;

    case TH09_ECL_OPERAND_SHARED_INT_0: return &enemy->manager00->primaryEclManager000.timelineState08.sharedInts160[0];
    case TH09_ECL_OPERAND_SHARED_INT_1: return &enemy->manager00->primaryEclManager000.timelineState08.sharedInts160[1];
    case TH09_ECL_OPERAND_SHARED_INT_2: return &enemy->manager00->primaryEclManager000.timelineState08.sharedInts160[2];
    case TH09_ECL_OPERAND_SHARED_INT_3: return &enemy->manager00->primaryEclManager000.timelineState08.sharedInts160[3];

    default: return rawValue;
    }
}

float *__fastcall ResolveFloatLValue(
    EnemyView *enemy,
    float *rawValue,
    unsigned short parameterMask,
    int operandIndex)
{
    if (operandIndex >= 0 && !(parameterMask & (1U << operandIndex)))
        return rawValue;

    switch (static_cast<int>(*rawValue))
    {
    case TH09_ECL_OPERAND_LOCAL_FLOAT_0: return &enemy->activeContext2CE0->floatVariables3C[0];
    case TH09_ECL_OPERAND_LOCAL_FLOAT_1: return &enemy->activeContext2CE0->floatVariables3C[1];
    case TH09_ECL_OPERAND_LOCAL_FLOAT_2: return &enemy->activeContext2CE0->floatVariables3C[2];
    case TH09_ECL_OPERAND_LOCAL_FLOAT_3: return &enemy->activeContext2CE0->floatVariables3C[3];
    case TH09_ECL_OPERAND_LOCAL_FLOAT_4: return &enemy->activeContext2CE0->floatVariables3C[4];
    case TH09_ECL_OPERAND_LOCAL_FLOAT_5: return &enemy->activeContext2CE0->floatVariables3C[5];
    case TH09_ECL_OPERAND_LOCAL_FLOAT_6: return &enemy->activeContext2CE0->floatVariables3C[6];
    case TH09_ECL_OPERAND_LOCAL_FLOAT_7: return &enemy->activeContext2CE0->floatVariables3C[7];

    case TH09_ECL_OPERAND_ENEMY_FLOAT_0: return &enemy->floatVariables2D08[0];
    case TH09_ECL_OPERAND_ENEMY_FLOAT_1: return &enemy->floatVariables2D08[1];
    case TH09_ECL_OPERAND_ENEMY_FLOAT_2: return &enemy->floatVariables2D08[2];
    case TH09_ECL_OPERAND_ENEMY_FLOAT_3: return &enemy->floatVariables2D08[3];
    case TH09_ECL_OPERAND_ENEMY_FLOAT_4: return &enemy->floatVariables2D08[4];
    case TH09_ECL_OPERAND_ENEMY_FLOAT_5: return &enemy->floatVariables2D08[5];
    case TH09_ECL_OPERAND_ENEMY_FLOAT_6: return &enemy->floatVariables2D08[6];
    case TH09_ECL_OPERAND_ENEMY_FLOAT_7: return &enemy->floatVariables2D08[7];

    case TH09_ECL_OPERAND_CALL_FLOAT_0: return &enemy->activeContext2CE0->floatVariables84[0];
    case TH09_ECL_OPERAND_CALL_FLOAT_1: return &enemy->activeContext2CE0->floatVariables84[1];
    case TH09_ECL_OPERAND_CALL_FLOAT_2: return &enemy->activeContext2CE0->floatVariables84[2];
    case TH09_ECL_OPERAND_CALL_FLOAT_3: return &enemy->activeContext2CE0->floatVariables84[3];

    case TH09_ECL_OPERAND_ENEMY_POSITION_X: return &enemy->position2D74.x;
    case TH09_ECL_OPERAND_ENEMY_POSITION_Y: return &enemy->position2D74.y;
    case TH09_ECL_OPERAND_ENEMY_POSITION_Z: return &enemy->position2D74.z;
    case TH09_ECL_OPERAND_PLAYER_POSITION_X: return &enemy->manager00->sideState320->rewardPlayer04->position1B88.x;
    case TH09_ECL_OPERAND_PLAYER_POSITION_Y: return &enemy->manager00->sideState320->rewardPlayer04->position1B88.y;
    case TH09_ECL_OPERAND_PLAYER_POSITION_Z: return &enemy->manager00->sideState320->rewardPlayer04->position1B88.z;

    case TH09_ECL_OPERAND_EXTRA_FLOAT_0: return &enemy->activeContext2CE0->floatVariables6C[0];
    case TH09_ECL_OPERAND_EXTRA_FLOAT_1: return &enemy->activeContext2CE0->floatVariables6C[1];

    case TH09_ECL_OPERAND_SHARED_FLOAT_0: return &enemy->manager00->primaryEclManager000.timelineState08.sharedFloats170[0];
    case TH09_ECL_OPERAND_SHARED_FLOAT_1: return &enemy->manager00->primaryEclManager000.timelineState08.sharedFloats170[1];
    case TH09_ECL_OPERAND_SHARED_FLOAT_2: return &enemy->manager00->primaryEclManager000.timelineState08.sharedFloats170[2];
    case TH09_ECL_OPERAND_SHARED_FLOAT_3: return &enemy->manager00->primaryEclManager000.timelineState08.sharedFloats170[3];

    case TH09_ECL_OPERAND_MOVEMENT_ORIGIN_X: return &enemy->movementOrigin2E1C.x;
    case TH09_ECL_OPERAND_MOVEMENT_ORIGIN_Y: return &enemy->movementOrigin2E1C.y;
    case TH09_ECL_OPERAND_MOVEMENT_ORIGIN_Z: return &enemy->movementOrigin2E1C.z;

    case TH09_ECL_OPERAND_ENEMY_FLOAT_2E10_0: return &enemy->floatVariables2E10[0];
    case TH09_ECL_OPERAND_ENEMY_FLOAT_2E10_1: return &enemy->floatVariables2E10[1];
    case TH09_ECL_OPERAND_ENEMY_FLOAT_2E10_2: return &enemy->floatVariables2E10[2];

    case TH09_ECL_OPERAND_MOVEMENT_ANGLE: return &enemy->movementAngle2DE0;
    case TH09_ECL_OPERAND_ANGULAR_VELOCITY: return &enemy->angularVelocity2DE4;
    case TH09_ECL_OPERAND_SPEED: return &enemy->speed2DF4;
    case TH09_ECL_OPERAND_ACCELERATION: return &enemy->acceleration2DF8;
    case TH09_ECL_OPERAND_ORBIT_RADIUS: return &enemy->orbitRadius2DFC;

    case TH09_ECL_OPERAND_ORBIT_ANGLE: return &enemy->orbitAngle2DE8;
    case TH09_ECL_OPERAND_ORBIT_ANGULAR_VELOCITY: return &enemy->orbitAngularVelocity2DEC;

    default: return rawValue;
    }
}

} // namespace Th09EclRunControl

float EnemyView::ResolveFloat(float rawValue)
{
    switch (static_cast<int>(rawValue))
    {
    case TH09_ECL_OPERAND_LOCAL_INT_0: return static_cast<float>(this->activeContext2CE0->intVariables1C[0]);
    case TH09_ECL_OPERAND_LOCAL_INT_1: return static_cast<float>(this->activeContext2CE0->intVariables1C[1]);
    case TH09_ECL_OPERAND_LOCAL_INT_2: return static_cast<float>(this->activeContext2CE0->intVariables1C[2]);
    case TH09_ECL_OPERAND_LOCAL_INT_3: return static_cast<float>(this->activeContext2CE0->intVariables1C[3]);
    case TH09_ECL_OPERAND_LOCAL_INT_4: return static_cast<float>(this->activeContext2CE0->intVariables1C[4]);
    case TH09_ECL_OPERAND_LOCAL_INT_5: return static_cast<float>(this->activeContext2CE0->intVariables1C[5]);
    case TH09_ECL_OPERAND_LOCAL_INT_6: return static_cast<float>(this->activeContext2CE0->intVariables1C[6]);
    case TH09_ECL_OPERAND_LOCAL_INT_7: return static_cast<float>(this->activeContext2CE0->intVariables1C[7]);

    case TH09_ECL_OPERAND_ENEMY_INT_0: return static_cast<float>(this->intVariables2CE8[0]);
    case TH09_ECL_OPERAND_ENEMY_INT_1: return static_cast<float>(this->intVariables2CE8[1]);
    case TH09_ECL_OPERAND_ENEMY_INT_2: return static_cast<float>(this->intVariables2CE8[2]);
    case TH09_ECL_OPERAND_ENEMY_INT_3: return static_cast<float>(this->intVariables2CE8[3]);
    case TH09_ECL_OPERAND_ENEMY_INT_4: return static_cast<float>(this->intVariables2CE8[4]);
    case TH09_ECL_OPERAND_ENEMY_INT_5: return static_cast<float>(this->intVariables2CE8[5]);
    case TH09_ECL_OPERAND_ENEMY_INT_6: return static_cast<float>(this->intVariables2CE8[6]);
    case TH09_ECL_OPERAND_ENEMY_INT_7: return static_cast<float>(this->intVariables2CE8[7]);

    case TH09_ECL_OPERAND_CALL_INT_0: return static_cast<float>(this->activeContext2CE0->intVariables74[0]);
    case TH09_ECL_OPERAND_CALL_INT_1: return static_cast<float>(this->activeContext2CE0->intVariables74[1]);
    case TH09_ECL_OPERAND_CALL_INT_2: return static_cast<float>(this->activeContext2CE0->intVariables74[2]);
    case TH09_ECL_OPERAND_CALL_INT_3: return static_cast<float>(this->activeContext2CE0->intVariables74[3]);

    case TH09_ECL_OPERAND_EXTRA_INT_0: return static_cast<float>(this->activeContext2CE0->intVariables5C[0]);
    case TH09_ECL_OPERAND_EXTRA_INT_1: return static_cast<float>(this->activeContext2CE0->intVariables5C[1]);
    case TH09_ECL_OPERAND_EXTRA_INT_2: return static_cast<float>(this->activeContext2CE0->intVariables5C[2]);
    case TH09_ECL_OPERAND_EXTRA_INT_3: return static_cast<float>(this->activeContext2CE0->intVariables5C[3]);

    case TH09_ECL_OPERAND_RANDOM_NONNEGATIVE_INT: return static_cast<float>(Th09EclRunControl::g_ReplayRng.GetRandomU32() & 0x7FFFFFFF);
    case TH09_ECL_OPERAND_RANDOM_UNIT_FLOAT: return Th09EclRunControl::g_ReplayRng.GetRandomF32();
    case TH09_ECL_OPERAND_RANDOM_RAW_INT: return static_cast<float>(static_cast<int>(Th09EclRunControl::g_ReplayRng.GetRandomU32()));
    case TH09_ECL_OPERAND_RANDOM_SIGNED_UNIT_FLOAT: return Th09EclRunControl::g_ReplayRng.GetRandomF32Signed();
    case TH09_ECL_OPERAND_RANDOM_ANGLE: return Th09EclRunControl::g_ReplayRng.GetRandomF32InRange(6.2831855f) - 3.1415927f;

    case TH09_ECL_OPERAND_DIFFICULTY: return static_cast<float>(g_GameManager.difficulty);
    case TH09_ECL_OPERAND_GAME_MANAGER_VALUE_B4: return static_cast<float>(g_GameManager.valueB4);
    case TH09_ECL_OPERAND_BOSS_TIMER: return static_cast<float>(this->bossTimer2E64.current);
    case TH09_ECL_OPERAND_LIFE: return static_cast<float>(this->life2E48);
    case TH09_ECL_OPERAND_CHARACTER_INDEX: return static_cast<float>(this->manager00->sideState320->characterIndex20);
    case TH09_ECL_OPERAND_ITEM_DROP_TYPE: return static_cast<float>(this->itemDropType335C);
    case TH09_ECL_OPERAND_ENEMY_VALUE_2E54: return static_cast<float>(this->operandValue2E54);

    case TH09_ECL_OPERAND_SHARED_INT_0: return static_cast<float>(this->manager00->primaryEclManager000.timelineState08.sharedInts160[0]);
    case TH09_ECL_OPERAND_SHARED_INT_1: return static_cast<float>(this->manager00->primaryEclManager000.timelineState08.sharedInts160[1]);
    case TH09_ECL_OPERAND_SHARED_INT_2: return static_cast<float>(this->manager00->primaryEclManager000.timelineState08.sharedInts160[2]);
    case TH09_ECL_OPERAND_SHARED_INT_3: return static_cast<float>(this->manager00->primaryEclManager000.timelineState08.sharedInts160[3]);
    case TH09_ECL_OPERAND_SHARED_FLOAT_0: return this->manager00->primaryEclManager000.timelineState08.sharedFloats170[0];
    case TH09_ECL_OPERAND_SHARED_FLOAT_1: return this->manager00->primaryEclManager000.timelineState08.sharedFloats170[1];
    case TH09_ECL_OPERAND_SHARED_FLOAT_2: return this->manager00->primaryEclManager000.timelineState08.sharedFloats170[2];
    case TH09_ECL_OPERAND_SHARED_FLOAT_3: return this->manager00->primaryEclManager000.timelineState08.sharedFloats170[3];

    case TH09_ECL_OPERAND_LOCAL_FLOAT_0: return this->activeContext2CE0->floatVariables3C[0];
    case TH09_ECL_OPERAND_LOCAL_FLOAT_1: return this->activeContext2CE0->floatVariables3C[1];
    case TH09_ECL_OPERAND_LOCAL_FLOAT_2: return this->activeContext2CE0->floatVariables3C[2];
    case TH09_ECL_OPERAND_LOCAL_FLOAT_3: return this->activeContext2CE0->floatVariables3C[3];
    case TH09_ECL_OPERAND_LOCAL_FLOAT_4: return this->activeContext2CE0->floatVariables3C[4];
    case TH09_ECL_OPERAND_LOCAL_FLOAT_5: return this->activeContext2CE0->floatVariables3C[5];
    case TH09_ECL_OPERAND_LOCAL_FLOAT_6: return this->activeContext2CE0->floatVariables3C[6];
    case TH09_ECL_OPERAND_LOCAL_FLOAT_7: return this->activeContext2CE0->floatVariables3C[7];

    case TH09_ECL_OPERAND_ENEMY_FLOAT_0: return this->floatVariables2D08[0];
    case TH09_ECL_OPERAND_ENEMY_FLOAT_1: return this->floatVariables2D08[1];
    case TH09_ECL_OPERAND_ENEMY_FLOAT_2: return this->floatVariables2D08[2];
    case TH09_ECL_OPERAND_ENEMY_FLOAT_3: return this->floatVariables2D08[3];
    case TH09_ECL_OPERAND_ENEMY_FLOAT_4: return this->floatVariables2D08[4];
    case TH09_ECL_OPERAND_ENEMY_FLOAT_5: return this->floatVariables2D08[5];
    case TH09_ECL_OPERAND_ENEMY_FLOAT_6: return this->floatVariables2D08[6];
    case TH09_ECL_OPERAND_ENEMY_FLOAT_7: return this->floatVariables2D08[7];

    case TH09_ECL_OPERAND_CALL_FLOAT_0: return this->activeContext2CE0->floatVariables84[0];
    case TH09_ECL_OPERAND_CALL_FLOAT_1: return this->activeContext2CE0->floatVariables84[1];
    case TH09_ECL_OPERAND_CALL_FLOAT_2: return this->activeContext2CE0->floatVariables84[2];
    case TH09_ECL_OPERAND_CALL_FLOAT_3: return this->activeContext2CE0->floatVariables84[3];

    case TH09_ECL_OPERAND_ENEMY_POSITION_X: return this->worldPosition2DD4.x;
    case TH09_ECL_OPERAND_ENEMY_POSITION_Y: return this->worldPosition2DD4.y;
    case TH09_ECL_OPERAND_ENEMY_POSITION_Z: return this->worldPosition2DD4.z;
    case TH09_ECL_OPERAND_PLAYER_POSITION_X: return this->manager00->sideState320->rewardPlayer04->position1B88.x;
    case TH09_ECL_OPERAND_PLAYER_POSITION_Y: return this->manager00->sideState320->rewardPlayer04->position1B88.y;
    case TH09_ECL_OPERAND_PLAYER_POSITION_Z: return this->manager00->sideState320->rewardPlayer04->position1B88.z;

    case TH09_ECL_OPERAND_EXTRA_FLOAT_0: return this->activeContext2CE0->floatVariables6C[0];
    case TH09_ECL_OPERAND_EXTRA_FLOAT_1: return this->activeContext2CE0->floatVariables6C[1];

    case TH09_ECL_OPERAND_MOVEMENT_ORIGIN_X: return this->movementOrigin2E1C.x;
    case TH09_ECL_OPERAND_MOVEMENT_ORIGIN_Y: return this->movementOrigin2E1C.y;
    case TH09_ECL_OPERAND_MOVEMENT_ORIGIN_Z: return this->movementOrigin2E1C.z;

    case TH09_ECL_OPERAND_ENEMY_FLOAT_2E10_0: return this->floatVariables2E10[0];
    case TH09_ECL_OPERAND_ENEMY_FLOAT_2E10_1: return this->floatVariables2E10[1];
    case TH09_ECL_OPERAND_ENEMY_FLOAT_2E10_2: return this->floatVariables2E10[2];

    case TH09_ECL_OPERAND_ENEMY_FLOAT_2DB0_0: return this->floatVariables2DB0[0];
    case TH09_ECL_OPERAND_ENEMY_FLOAT_2DB0_1: return this->floatVariables2DB0[1];
    case TH09_ECL_OPERAND_ENEMY_FLOAT_2DB0_2: return this->floatVariables2DB0[2];

    case TH09_ECL_OPERAND_LIFE_CALLBACK_THRESHOLD_0: return static_cast<float>(this->lifeCallbackThresholds33B0[0]);
    case TH09_ECL_OPERAND_LIFE_CALLBACK_THRESHOLD_1: return static_cast<float>(this->lifeCallbackThresholds33B0[1]);
    case TH09_ECL_OPERAND_LIFE_CALLBACK_THRESHOLD_2: return static_cast<float>(this->lifeCallbackThresholds33B0[2]);
    case TH09_ECL_OPERAND_LIFE_CALLBACK_THRESHOLD_3: return static_cast<float>(this->lifeCallbackThresholds33B0[3]);

    case TH09_ECL_OPERAND_ANGLE_TO_PLAYER:
        return reinterpret_cast<PlayerAngleView *>(this->manager00->sideState320->rewardPlayer04)->AngleToPoint(
            reinterpret_cast<PlayerAngleFloat3View *>(&this->worldPosition2DD4));

    case TH09_ECL_OPERAND_MOVEMENT_ANGLE: return this->movementAngle2DE0;
    case TH09_ECL_OPERAND_ANGULAR_VELOCITY: return this->angularVelocity2DE4;
    case TH09_ECL_OPERAND_SPEED: return this->speed2DF4;
    case TH09_ECL_OPERAND_ACCELERATION: return this->acceleration2DF8;
    case TH09_ECL_OPERAND_ORBIT_RADIUS: return this->orbitRadius2DFC;
    case TH09_ECL_OPERAND_ORBIT_ANGLE: return this->orbitAngle2DE8;
    case TH09_ECL_OPERAND_ORBIT_ANGULAR_VELOCITY: return this->orbitAngularVelocity2DEC;

    case TH09_ECL_OPERAND_OPPOSING_PLAYER_INITIAL_STATE:
    {
        if (this->secondaryFlagBits3380.category.sideCategory10_11 < 3)
        {
            return static_cast<float>(
                this->manager00->opposingSideState324->rewardPlayer04->initialStateA0[0]);
        }
        return static_cast<float>(
            this->manager00->opposingSideState324->rewardPlayer04->initialStateA0[1]);
    }

    case TH09_ECL_OPERAND_BOSS_SLOT: return static_cast<float>(this->bossSlot336B);
    case TH09_ECL_OPERAND_LAST_DAMAGE: return static_cast<float>(this->lastDamage33AC);

    case TH09_ECL_OPERAND_DISTANCE_TO_PLAYER:
    {
        EclOperandFloat3View delta = this->manager00->sideState320->rewardPlayer04->position1B88 - reinterpret_cast<const EclOperandFloat3View &>(this->worldPosition2DD4);
        return D3DXVec3Length(reinterpret_cast<const D3DXVECTOR3 *>(&delta));
    }

    default: return rawValue;
    }
}

#undef TH09_ECL_OPERAND_OPPOSING_PLAYER
#undef TH09_ECL_OPERAND_PLAYER
#undef TH09_ECL_OPERAND_CONTEXT
