#include "EnemyManager.hpp"

#include "Chain.hpp"
#include "ZunMemory.hpp"

#include <new>

extern Chain g_Chain;
extern unsigned char g_GameManager[];

EnemyManagerView *__fastcall EnemyManagerView::Create(int sideIndex)
{
    EnemyManagerView *enemyManager = static_cast<EnemyManagerView *>(
        g_ZunMemory.AddToRegistry(new EnemyManagerView, sizeof(EnemyManagerView),
                                  const_cast<char *>("EnemyCtrlInf")));

    enemyManager->Initialize();
    enemyManager->sideState320 = reinterpret_cast<EnemySideStateView *>(
        g_GameManager + sideIndex * sizeof(EnemySideStateView));
    enemyManager->sideIndex31C = sideIndex;
    enemyManager->opposingSideState324 = reinterpret_cast<EnemySideStateView *>(
        g_GameManager + sizeof(EnemySideStateView) -
        sideIndex * sizeof(EnemySideStateView));

    enemyManager->calcChain310 =
        g_Chain.CreateElem(reinterpret_cast<ChainCallback>(EnemyManagerView::OnUpdate));
    enemyManager->calcChain310->arg = enemyManager;
    enemyManager->calcChain310->addedCallback =
        reinterpret_cast<ChainLifetimeCallback>(EnemyManagerView::AddedCallback);
    if (g_Chain.AddToCalcChain(enemyManager->calcChain310, sideIndex + 10) != 0)
        return NULL;

    enemyManager->drawHighChain314 =
        g_Chain.CreateElem(reinterpret_cast<ChainCallback>(EnemyManagerView::OnDrawHighPrio));
    enemyManager->drawHighChain314->arg = enemyManager;
    g_Chain.AddToDrawChain(enemyManager->drawHighChain314, sideIndex + 10);

    enemyManager->drawLowChain318 =
        g_Chain.CreateElem(reinterpret_cast<ChainCallback>(EnemyManagerView::OnDrawLowPrio));
    enemyManager->drawLowChain318->arg = enemyManager;
    g_Chain.AddToDrawChain(enemyManager->drawLowChain318, sideIndex + 17);

    return enemyManager;
}

#include "AnmManager.hpp"
#include "AsciiManager.hpp"
#include "ZunTimer.hpp"

#include <string.h>

struct EnemyLifecycleVmView
{
    unsigned char unknown000[0x21A];
    short scriptIndex21A;
    unsigned char unknown21C[0x2A4 - 0x21C];
};
typedef char EnemyLifecycleVmSizeIs2A4[
    (sizeof(EnemyLifecycleVmView) == 0x2A4) ? 1 : -1];

struct EnemyLifecycleTrailSampleView
{
    Float3 position00;
    unsigned char unknown0C[0x10];
};
typedef char EnemyLifecycleTrailSizeIs1C[
    (sizeof(EnemyLifecycleTrailSampleView) == 0x1C) ? 1 : -1];

struct EnemyLifecycleInitView
{
    EnemyManagerView *manager00;
    unsigned char unknown004[0x2AC - 0x004];
    EnemyLifecycleVmView secondaryVms2AC[2];
    unsigned char unknown7F4[0x2D2A - 0x7F4];
    short activeEclCallStackDepth2D2A;
    unsigned char unknown2D2C[0x02];
    short deathCallbackSubId2D2E;
    unsigned char unknown2D30[0x2D70 - 0x2D30];
    short pendingEclSubId2D70;
    unsigned char unknown2D72[0x2D8C - 0x2D72];
    Float3 velocity2D8C;
    unsigned char unknown2D98[0x2DBC - 0x2D98];
    Float3 hitboxDimensions2DBC;
    unsigned char unknown2DC8[0x2DE0 - 0x2DC8];
    float movementAngle2DE0;
    float angularVelocity2DE4;
    unsigned char unknown2DE8[0x2DF4 - 0x2DE8];
    float speed2DF4;
    float acceleration2DF8;
    unsigned char unknown2DFC[0x2E04 - 0x2DFC];
    Float3 shootOffset2E04;
    unsigned char unknown2E10[0x2E38 - 0x2E10];
    float bulletRankSpeedLow2E38;
    float bulletRankSpeedHigh2E3C;
    unsigned char unknown2E40[0x2E48 - 0x2E40];
    int life2E48;
    unsigned char unknown2E4C[0x2E54 - 0x2E4C];
    int score2E54;
    unsigned char unknown2E58[0x2E64 - 0x2E58];
    ZunTimer bossTimer2E64;
    unsigned char unknown2E70[0x3074 - 0x2E70];
    int bulletSpawnSound3074;
    int bulletTransformSound3078;
    unsigned char unknown307C[0x30B4 - 0x307C];
    int shootIntervalFrames30B4;
    ZunTimer shootIntervalTimer30B8;
    unsigned char unknown30C4[0x3358 - 0x30C4];
    int selectedLaserSlot3358;
    unsigned char unknown335C[0x3368 - 0x335C];
    signed char deathEffectVariant3368;
    unsigned char deathEffectVariant3369;
    unsigned char deathEffectVariant336A;
    unsigned char unknown336B;
    unsigned char damageFlashTimer336C;
    unsigned char unknown336D[0x337C - 0x336D];
    unsigned int flags337C;
    unsigned char unknown3380[0x338A - 0x3380];
    short idleAnmScript338A;
    unsigned char unknown338C[0x3390 - 0x338C];
    short moveLeftAnmScript3390;
    short moveRightAnmScript3392;
    unsigned char unknown3394[0x33A8 - 0x3394];
    float minimumPlayerDistanceSquared33A8;
    unsigned char unknown33AC[0x33B0 - 0x33AC];
    int lifeCallbackThresholds33B0[4];
    unsigned char unknown33C0[0x33D0 - 0x33C0];
    int timerCallbackThreshold33D0;
    unsigned char unknown33D4[0x33E8 - 0x33D4];
    EnemyLifecycleTrailSampleView trailSamples33E8[96];
    unsigned char unknown3E68[0x5414 - 0x3E68];
    int attachedEffectCount5414;
    unsigned char unknown5418[0x5430 - 0x5418];
};
typedef char EnemyLifecycleInitSizeIs5430[
    (sizeof(EnemyLifecycleInitView) == 0x5430) ? 1 : -1];
typedef char EnemyLifecycleInitSecondary0At2AC[
    (offsetof(EnemyLifecycleInitView, secondaryVms2AC) == 0x2AC) ? 1 : -1];
typedef char EnemyLifecycleInitFlagsAt337C[
    (offsetof(EnemyLifecycleInitView, flags337C) == 0x337C) ? 1 : -1];
typedef char EnemyLifecycleInitTrailAt33E8[
    (offsetof(EnemyLifecycleInitView, trailSamples33E8) == 0x33E8) ? 1 : -1];

struct EnemyLifecycleAnmLoadedView
{
    unsigned char unknown00[4];
    void *rawData04;
};

struct EnemyLifecycleCharacterResources
{
    char *eclPath;
    void *unknown04;
    void *unknown08;
    void *unknown0C;
    void *unknown10;
    void *unknown14;
};
typedef char EnemyLifecycleCharacterResourcesSizeIs18[
    (sizeof(EnemyLifecycleCharacterResources) == 0x18) ? 1 : -1];

struct EnemyLifecycleRngView
{
    unsigned int GetRandomU32InRange(unsigned int maximum);
};

extern EnemyLifecycleCharacterResources g_EnemyCharacterResources[];
extern EnemyLifecycleRngView g_Rng;
extern unsigned char g_EnemySubroutineSchedule[256];

EnemyEclManagerView::EnemyEclManagerView()
{
}

EnemyScheduleRuntimeView::EnemyScheduleRuntimeView()
{
}

EnemyManagerView::EnemyManagerView()
{
    Initialize();
}

void EnemyManagerView::Initialize()
{
    int i;

    memset(this, 0, sizeof(*this));
    for (i = 0; i < 4; ++i)
        this->timelineEventSlots2AC430[i] = -1;

    EnemyLifecycleInitView *enemy =
        reinterpret_cast<EnemyLifecycleInitView *>(&this->spawnTemplate328);
    memset(enemy, 0, sizeof(*enemy));
    enemy->manager00 = this;
    for (i = 0; i < 2; ++i)
        enemy->secondaryVms2AC[i].scriptIndex21A = -1;
    for (i = 0; i < 96; ++i)
        enemy->trailSamples33E8[i].position00.x = -999.0f;

    enemy->flags337C |= 0x00000001u;
    enemy->bossTimer2E64 = 0;

    enemy->flags337C &= ~0x00200000u;
    unsigned int movementFlags = enemy->flags337C;

    enemy->hitboxDimensions2DBC = Float3(24.0f, 24.0f, 24.0f);
    enemy->velocity2D8C = Float3(0.0f, 0.0f, 0.0f);
    *reinterpret_cast<int *>(&enemy->angularVelocity2DE4) = 0;
    *reinterpret_cast<int *>(&enemy->movementAngle2DE0) = 0;
    *reinterpret_cast<int *>(&enemy->acceleration2DF8) = 0;
    *reinterpret_cast<int *>(&enemy->speed2DF4) = 0;

    movementFlags &= ~0x0000C602u;
    enemy->flags337C = movementFlags;
    enemy->activeEclCallStackDepth2D2A = 0;
    enemy->life2E48 = 1;
    enemy->score2E54 = 100;
    enemy->deathEffectVariant3368 = 0;
    enemy->deathEffectVariant3369 = 0;
    enemy->deathEffectVariant336A = 0;
    enemy->shootIntervalFrames30B4 = 0;
    enemy->shootIntervalTimer30B8 = 0;
    enemy->shootOffset2E04 = Float3(0.0f, 0.0f, 0.0f);

    unsigned int animationFlags =
        enemy->flags337C & ~0x000F0090u;
    enemy->moveLeftAnmScript3390 = -1;
    enemy->moveRightAnmScript3392 = -1;
    enemy->idleAnmScript338A = -1;
    enemy->deathCallbackSubId2D2E = -1;
    enemy->attachedEffectCount5414 = 0;
    enemy->pendingEclSubId2D70 = -1;
    animationFlags |= 0x0000004Cu;
    enemy->flags337C = animationFlags;
    for (i = 0; i < 4; ++i)
        enemy->lifeCallbackThresholds33B0[i] = -1;
    enemy->timerCallbackThreshold33D0 = -1;
    enemy->selectedLaserSlot3358 = 0;
    enemy->damageFlashTimer336C = 0;
    enemy->flags337C &= ~0x00400000u;

    enemy->bulletRankSpeedLow2E38 = -0.15f;
    enemy->bulletRankSpeedHigh2E3C = 0.15f;
    enemy->bulletSpawnSound3074 = 7;
    enemy->bulletTransformSound3078 = 25;
    enemy->minimumPlayerDistanceSquared33A8 = 1024.0f;
}

int __fastcall EnemyManagerView::AddedCallback(EnemyManagerView *enemyManager)
{
    {
        AnmManager *manager = g_AnmManager;
        EnemyLifecycleAnmLoadedView *loaded =
            reinterpret_cast<EnemyLifecycleAnmLoadedView *>(manager->GetAnm(9));

        if (loaded->rawData04 == NULL)
        {
            int currentStage = *reinterpret_cast<int *>(g_GameManager + 0xF4);
            if (currentStage == 0 || currentStage == 3 || currentStage == 7)
                enemyManager->enemyAnm2AC420 =
                    manager->PreloadAnm(9, "enemy1.anm");
            else if (currentStage == 12 || currentStage == 15)
                enemyManager->enemyAnm2AC420 =
                    manager->PreloadAnm(9, "enemy13.anm");
            else
                enemyManager->enemyAnm2AC420 =
                    manager->PreloadAnm(9, "enemy.anm");

            if (enemyManager->enemyAnm2AC420 == NULL)
                return -1;
        }
        else
        {
            enemyManager->enemyAnm2AC420 = reinterpret_cast<AnmLoaded *>(loaded);
        }
    }

    enemyManager->opposingEnemyAnm2AC424 =
        g_AnmManager->GetAnm(6 - enemyManager->sideIndex31C);

    EnemyEclManagerView *primary = &enemyManager->primaryEclManager000;
    memset(primary, 0, sizeof(*primary));
    if (primary->Load(const_cast<char *>("enemy.ecl")) != 0)
        return -1;

    if (enemyManager->opposingEclManager188.Load(
            g_EnemyCharacterResources[
                enemyManager->opposingSideState324->characterIndex20].eclPath) != 0)
        return -1;

    if (enemyManager->sideIndex31C == 0)
    {
        int index;
        for (index = 0; index < 256; index++)
        {
            g_EnemySubroutineSchedule[index] = static_cast<unsigned char>(
                g_Rng.GetRandomU32InRange(
                    enemyManager->primaryEclManager000.GetSubroutineCount()));
            g_EnemySubroutineSchedule[index] |= static_cast<unsigned char>(
                g_Rng.GetRandomU32InRange(2) << 7);
        }
    }

    enemyManager->selfPointer2AC3FC = enemyManager;
    unsigned char scheduleEntry =
        g_EnemySubroutineSchedule[enemyManager->scheduleIndex2AC3D4++];
    enemyManager->scheduledSubroutine2AC400 =
        enemyManager->primaryEclManager000.GetSubroutine(scheduleEntry & 0x7F);
    enemyManager->scheduleRuntime2AC3E0.timer0C = 200;
    enemyManager->specialAttackThreshold2AC44C =
        enemyManager->sideState320->characterIndex20 == 5 ? 200 : 160;
    return 0;
}
