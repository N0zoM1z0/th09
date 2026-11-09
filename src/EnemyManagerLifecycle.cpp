#include "EnemyManager.hpp"

#include "Chain.hpp"
#include "ZunMemory.hpp"

#include <new>

extern Chain g_Chain;

EnemyManagerView *__fastcall EnemyManagerView::Create(int sideIndex)
{
    EnemyManagerView *enemyManager = static_cast<EnemyManagerView *>(
        g_ZunMemory.AddToRegistry(new EnemyManagerView, sizeof(EnemyManagerView),
                                  const_cast<char *>("EnemyCtrlInf")));

    enemyManager->Initialize();
    enemyManager->sideState320 = reinterpret_cast<EnemySideStateView *>(
        &g_GameManager.sides[sideIndex]);
    enemyManager->sideIndex31C = sideIndex;
    enemyManager->opposingSideState324 = reinterpret_cast<EnemySideStateView *>(
        &g_GameManager.sides[1 - sideIndex]);

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

EnemyEclTimelineStateView::EnemyEclTimelineStateView()
{
}

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

    EnemyView *enemy = &this->spawnTemplate328;
    memset(enemy, 0, sizeof(*enemy));
    enemy->manager00 = this;
    for (i = 0; i < 2; ++i)
        enemy->secondaryVms2AC[i].scriptIndex = -1;
    for (i = 0; i < 96; ++i)
        enemy->trailSamples33E8[i].position00.x = -999.0f;

    enemy->flags337C |= 0x00000001u;
    enemy->bossTimer2E64 = 0;

    enemy->flags337C &= ~0x00200000u;
    unsigned int movementFlags = enemy->flags337C;

    reinterpret_cast<Float3 &>(enemy->hitboxDimensions2DBC) = Float3(24.0f, 24.0f, 24.0f);
    reinterpret_cast<Float3 &>(enemy->velocity2D8C) = Float3(0.0f, 0.0f, 0.0f);
    *reinterpret_cast<int *>(&enemy->angularVelocity2DE4) = 0;
    *reinterpret_cast<int *>(&enemy->movementAngle2DE0) = 0;
    *reinterpret_cast<int *>(&enemy->acceleration2DF8) = 0;
    *reinterpret_cast<int *>(&enemy->speed2DF4) = 0;

    movementFlags &= ~0x0000C602u;
    enemy->flags337C = movementFlags;
    enemy->activeCallDepth2D2A = 0;
    enemy->life2E48 = 1;
    enemy->score2E54 = 100;
    enemy->deathEffectVariant3368 = 0;
    enemy->deathEffectVariant3369 = 0;
    enemy->deathEffectVariant336A = 0;
    enemy->shootIntervalFrames30B4 = 0;
    enemy->shootIntervalTimer30B8 = 0;
    reinterpret_cast<Float3 &>(enemy->shootOffset2E04) = Float3(0.0f, 0.0f, 0.0f);

    unsigned int animationFlags =
        enemy->flags337C & ~0x000F0090u;
    enemy->moveLeftAnm3390 = -1;
    enemy->moveRightAnm3392 = -1;
    enemy->idleInitialAnm338A = -1;
    enemy->deathCallbackSubId2D2E = -1;
    enemy->attachedEffectState53B4.count60 = 0;
    enemy->pendingSubroutineId2D70 = -1;
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
    enemy->bulletDescriptor2E74.spawnSound = 7;
    enemy->bulletDescriptor2E74.transformSound = 25;
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
            int currentStage = g_GameManager.valueF4;
            if (currentStage == 0 || currentStage == 3 || currentStage == 7)
            {
                enemyManager->enemyAnm2AC420 =
                    manager->PreloadAnm(9, "enemy1.anm");
                if (enemyManager->enemyAnm2AC420 == NULL)
                    return -1;
            }
            else if (currentStage == 12 || currentStage == 15)
            {
                enemyManager->enemyAnm2AC420 =
                    manager->PreloadAnm(9, "enemy13.anm");
                if (enemyManager->enemyAnm2AC420 == NULL)
                    return -1;
            }
            else
            {
                enemyManager->enemyAnm2AC420 =
                    manager->PreloadAnm(9, "enemy.anm");
                if (enemyManager->enemyAnm2AC420 == NULL)
                    return -1;
            }
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
