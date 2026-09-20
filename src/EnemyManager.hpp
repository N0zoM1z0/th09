#pragma once

#include "EffectManager.hpp"
#include "ZunTimer.hpp"

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

class ChainElem;
class AnmLoaded;
struct EnemyManagerView;

struct EnemySideStateView
{
    unsigned char unknown00[0x04];
    EnemyRewardPlayerView *player04;
    unsigned char unknown08[0x04];
    EffectManager *effectManager0C;
    EnemyManagerView *enemyManager10;
    unsigned char unknown14[0x0C];
    int characterIndex20;
    unsigned char unknown24[0x14];
};
typedef char EnemySideStateSizeIs38[
    (sizeof(EnemySideStateView) == 0x38) ? 1 : -1];
typedef char EnemySideStateCharacterAt20[
    (offsetof(EnemySideStateView, characterIndex20) == 0x20) ? 1 : -1];

struct EnemyEclTimelineStateView
{
    unsigned char storage000[0x180];

    EnemyEclTimelineStateView();
};
typedef char EnemyEclTimelineStateViewSizeIs180[
    (sizeof(EnemyEclTimelineStateView) == 0x180) ? 1 : -1];

struct EnemyEclManagerView
{
    void *rawFile00;
    void *subroutineTable04;
    EnemyEclTimelineStateView timelineState08;

    EnemyEclManagerView();
    int Load(char *path);
    int GetSubroutineCount();
    void *GetSubroutine(int index);
    void ReleaseRawFile();
};
typedef char EnemyEclManagerViewSizeIs188[
    (sizeof(EnemyEclManagerView) == 0x188) ? 1 : -1];
typedef char EnemyEclManagerTimelineAt08[
    (offsetof(EnemyEclManagerView, timelineState08) == 0x08) ? 1 : -1];

struct EnemyScheduleRuntimeView
{
    ZunTimer timer00;
    ZunTimer timer0C;

    EnemyScheduleRuntimeView();
};
typedef char EnemyScheduleRuntimeViewSizeIs18[
    (sizeof(EnemyScheduleRuntimeView) == 0x18) ? 1 : -1];
typedef char EnemyScheduleRuntimeTimer0C[
    (offsetof(EnemyScheduleRuntimeView, timer0C) == 0x0C) ? 1 : -1];

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

    EnemyView();
    float ResolveFloat(float rawValue);
    void UpdateMovement();
    void UpdateShotAndAnm();
    void HandleDeathRewards(int hitKind);
    void ResetBulletRankInfluence();
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
    EnemyEclManagerView primaryEclManager000;
    EnemyEclManagerView opposingEclManager188;
    ChainElem *calcChain310;
    ChainElem *drawHighChain314;
    ChainElem *drawLowChain318;
    int sideIndex31C;
    EnemySideStateView *sideState320;
    EnemySideStateView *opposingSideState324;
    EnemyView spawnTemplate328;
    // The constructor vector-constructs 129 records here; the runtime update
    // loop processes 128.  The role of the extra constructed record remains unknown.
    EnemyView enemies5758[129];
    unsigned char unknown2AC388[0x2AC3AC - 0x2AC388];
    int activeEnemyCount2AC3AC;
    int normalEnemyCount2AC3B0;
    int specialEnemyCount2AC3B4;
    int rewardEnemyCount2AC3B8;
    unsigned char unknown2AC3BC[0x0C];
    ZunTimer sideTimer2AC3C8;
    unsigned char scheduleIndex2AC3D4;
    unsigned char unknown2AC3D5[0x0B];
    EnemyScheduleRuntimeView scheduleRuntime2AC3E0;
    int unknown2AC3F8;
    EnemyManagerView *selfPointer2AC3FC;
    void *scheduledSubroutine2AC400;
    ZunTimer frameTimer2AC404;
    EnemyView *drawGroupHeads2AC410[4];
    AnmLoaded *enemyAnm2AC420;
    AnmLoaded *opposingEnemyAnm2AC424;
    unsigned char unknown2AC428[0x08];
    int timelineEventSlots2AC430[4];
    unsigned char unknown2AC440[0x04];
    EnemyView *priorityEnemy2AC444;
    EnemyView *firstActiveEnemy2AC448;
    int specialAttackThreshold2AC44C;

    void ResetForGameplayCleanup();

    EnemyManagerView();
    void Initialize();
    EnemyView *FindActiveEnemyBySideCategory(int sideCategory);
    static EnemyManagerView *__fastcall Create(int sideIndex);
    static int __fastcall AddedCallback(EnemyManagerView *enemyManager);
    static int __fastcall OnDrawHighPrio(EnemyManagerView *enemyManager);
    static int __fastcall OnDrawLowPrio(EnemyManagerView *enemyManager);

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
typedef char EnemyManagerPrimaryEclAt000[
    (offsetof(EnemyManagerView, primaryEclManager000) == 0x000) ? 1 : -1];
typedef char EnemyManagerOpposingEclAt188[
    (offsetof(EnemyManagerView, opposingEclManager188) == 0x188) ? 1 : -1];
typedef char EnemyManagerScheduleIndexAt2AC3D4[
    (offsetof(EnemyManagerView, scheduleIndex2AC3D4) == 0x2AC3D4) ? 1 : -1];
typedef char EnemyManagerScheduleRuntimeAt2AC3E0[
    (offsetof(EnemyManagerView, scheduleRuntime2AC3E0) == 0x2AC3E0) ? 1 : -1];
typedef char EnemyManagerScheduleTimerAt2AC3EC[
    (offsetof(EnemyManagerView, scheduleRuntime2AC3E0) +
         offsetof(EnemyScheduleRuntimeView, timer0C) ==
     0x2AC3EC) ? 1 : -1];
typedef char EnemyManagerEnemyAnmAt2AC420[
    (offsetof(EnemyManagerView, enemyAnm2AC420) == 0x2AC420) ? 1 : -1];
typedef char EnemyManagerTimelineSlotsAt2AC430[
    (offsetof(EnemyManagerView, timelineEventSlots2AC430) == 0x2AC430) ? 1 : -1];
typedef char EnemyManagerSpecialThresholdAt2AC44C[
    (offsetof(EnemyManagerView, specialAttackThreshold2AC44C) == 0x2AC44C) ? 1 : -1];
typedef char EnemyManagerCalcChainAt310[
    (offsetof(EnemyManagerView, calcChain310) == 0x310) ? 1 : -1];
typedef char EnemyManagerDrawHighChainAt314[
    (offsetof(EnemyManagerView, drawHighChain314) == 0x314) ? 1 : -1];
typedef char EnemyManagerDrawLowChainAt318[
    (offsetof(EnemyManagerView, drawLowChain318) == 0x318) ? 1 : -1];
typedef char EnemyManagerSideAt31C[
    (offsetof(EnemyManagerView, sideIndex31C) == 0x31C) ? 1 : -1];
typedef char EnemyManagerTemplateAt328[
    (offsetof(EnemyManagerView, spawnTemplate328) == 0x328) ? 1 : -1];
typedef char EnemyManagerEnemiesAt5758[
    (offsetof(EnemyManagerView, enemies5758) == 0x5758) ? 1 : -1];
typedef char EnemyManagerRewardCountAt2AC3B8[
    (offsetof(EnemyManagerView, rewardEnemyCount2AC3B8) == 0x2AC3B8) ? 1 : -1];
