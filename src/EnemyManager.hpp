#pragma once

#include "EffectManager.hpp"

#include <stddef.h>

// Target-facing views for the TH09 enemy/ECL owner.  Names stop at behavior
// observed in th09.exe; opaque ranges deliberately do not import TH08 layout.

struct EnemyFloat3
{
    float x;
    float y;
    float z;
};
typedef char EnemyFloat3SizeIs0C[(sizeof(EnemyFloat3) == 0x0C) ? 1 : -1];

struct EnemyRewardAttackStateView
{
    unsigned char unknown00[0x38];
    int rewardLevel38;
    unsigned char unknown3C[0x08];

    void ResetForEnemyReward();
    void QueueEnemyReward(
        EnemyFloat3 *position,
        int interval,
        int spread,
        int count,
        int value);
};
typedef char EnemyRewardAttackStateSizeIs44[
    (sizeof(EnemyRewardAttackStateView) == 0x44) ? 1 : -1];

struct EnemyRewardPlayerView
{
    unsigned char unknown00000[0x30388];
    float accumulatedDefeatValue30388;
    unsigned char unknown3038C[0x30410 - 0x3038C];
    EnemyRewardAttackStateView rewardAttack30410;

    void AccumulateDefeatValue(float amount);
    void SpawnDefeatToken(unsigned int tokenType, EnemyFloat3 *position);
};
typedef char EnemyRewardPlayerAttackAt30410[
    (offsetof(EnemyRewardPlayerView, rewardAttack30410) == 0x30410) ? 1 : -1];
typedef char EnemyRewardPlayerLevelAt30448[
    (offsetof(EnemyRewardPlayerView, rewardAttack30410) +
         offsetof(EnemyRewardAttackStateView, rewardLevel38) ==
     0x30448) ? 1 : -1];

struct EnemyManagerView;

struct EnemySideStateView
{
    unsigned char unknown00[0x04];
    EnemyRewardPlayerView *player04;
    unsigned char unknown08[0x04];
    EffectManager *effectManager0C;
    EnemyManagerView *enemyManager10;
    unsigned char unknown14[0x24];
};
typedef char EnemySideStateSizeIs38[
    (sizeof(EnemySideStateView) == 0x38) ? 1 : -1];

struct EnemyView
{
    EnemyManagerView *manager00;
    unsigned char unknown0004[0x2D74 - 0x0004];
    EnemyFloat3 position2D74;
    EnemyFloat3 positionOffset2D80;
    unsigned char unknown2D8C[0x2DD4 - 0x2D8C];
    EnemyFloat3 worldPosition2DD4;
    unsigned char unknown2DE0[0x2E58 - 0x2DE0];
    int sequenceIndex2E58;
    unsigned char unknown2E5C[0x335C - 0x2E5C];
    unsigned int defeatTokenType335C;
    unsigned char unknown3360[0x3368 - 0x3360];
    signed char deathEffectVariant3368;
    unsigned char unknown3369[0x337C - 0x3369];
    unsigned int flags337C;
    unsigned int rewardFlags3380;
    unsigned char unknown3384[0x5430 - 0x3384];

    void HandleDeathRewards(int hitKind);
};
typedef char EnemyViewSizeIs5430[(sizeof(EnemyView) == 0x5430) ? 1 : -1];
typedef char EnemyViewPositionAt2D74[
    (offsetof(EnemyView, position2D74) == 0x2D74) ? 1 : -1];
typedef char EnemyViewWorldPositionAt2DD4[
    (offsetof(EnemyView, worldPosition2DD4) == 0x2DD4) ? 1 : -1];
typedef char EnemyViewRewardFlagsAt3380[
    (offsetof(EnemyView, rewardFlags3380) == 0x3380) ? 1 : -1];

struct EnemyManagerView
{
    unsigned char unknown000000[0x31C];
    int sideIndex31C;
    EnemySideStateView *sideState320;
    EnemySideStateView *opposingSideState324;
    EnemyView spawnTemplate328;
    EnemyView enemies5758[128];
    unsigned char unknown2A6F58[0x2AC3B8 - 0x2A6F58];
    int rewardEnemyCount2AC3B8;
    unsigned char unknown2AC3BC[0x2AC450 - 0x2AC3BC];

    EnemyView *SpawnEnemy(
        short eclSubroutineId,
        EnemyFloat3 *position,
        int argument4,
        signed char argument5,
        int argument6,
        int *initialVariables,
        int runImmediately);
    static int __fastcall OnUpdate(EnemyManagerView *enemyManager);
};
typedef char EnemyManagerViewSizeIs2AC450[
    (sizeof(EnemyManagerView) == 0x2AC450) ? 1 : -1];
typedef char EnemyManagerSideAt31C[
    (offsetof(EnemyManagerView, sideIndex31C) == 0x31C) ? 1 : -1];
typedef char EnemyManagerTemplateAt328[
    (offsetof(EnemyManagerView, spawnTemplate328) == 0x328) ? 1 : -1];
typedef char EnemyManagerEnemiesAt5758[
    (offsetof(EnemyManagerView, enemies5758) == 0x5758) ? 1 : -1];
typedef char EnemyManagerRewardCountAt2AC3B8[
    (offsetof(EnemyManagerView, rewardEnemyCount2AC3B8) == 0x2AC3B8) ? 1 : -1];
