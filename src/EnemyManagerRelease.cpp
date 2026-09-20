#include "EnemyManager.hpp"

#include "AnmManager.hpp"
#include "Chain.hpp"

#include <stdlib.h>

extern Chain g_Chain;

struct EnemyCoreView;
extern void EnemyCoreReleaseChildEclBlocks(EnemyCoreView *enemy);

static int EnemyManagerReleaseCore(EnemyManagerView *manager)
{
    EnemyView *enemy = &manager->enemies5758[0];
    int remaining = 128;
    do
    {
        EnemyCoreReleaseChildEclBlocks(
            reinterpret_cast<EnemyCoreView *>(enemy));
        ++enemy;
        --remaining;
    } while (remaining != 0);

    g_AnmManager->ReleaseAnm(9);
    manager->primaryEclManager000.ReleaseRawFile();
    manager->opposingEclManager188.ReleaseRawFile();
    return 0;
}

void __fastcall ReleaseSubsystem4(void *object)
{
    EnemyManagerView *manager = static_cast<EnemyManagerView *>(object);
    if (manager != 0)
    {
        EnemyManagerReleaseCore(manager);
        g_Chain.Cut(manager->calcChain310);
        g_Chain.Cut(manager->drawHighChain314);
        g_Chain.Cut(manager->drawLowChain318);
        free(manager);
    }
}

struct EnemyLifecycleRngView
{
    unsigned int GetRandomU32InRange(unsigned int maximum);
};

extern EnemyLifecycleRngView g_Rng;
extern unsigned char g_EnemySubroutineSchedule[256];

void EnemyManagerView::ResetForGameplayCleanup()
{
    EnemyView *enemy = &enemies5758[0];
    int remaining = 128;
    do
    {
        EnemyCoreReleaseChildEclBlocks(
            reinterpret_cast<EnemyCoreView *>(enemy));
        enemy->flags337C &= ~1u;
        ++enemy;
        --remaining;
    } while (remaining != 0);

    if (sideIndex31C == 0)
    {
        for (int index = 0; index < 256; index++)
        {
            g_EnemySubroutineSchedule[index] = static_cast<unsigned char>(
                g_Rng.GetRandomU32InRange(
                    primaryEclManager000.GetSubroutineCount()));
            g_EnemySubroutineSchedule[index] |= static_cast<unsigned char>(
                g_Rng.GetRandomU32InRange(2) << 7);
        }
    }

    scheduleIndex2AC3D4 = 0;
    scheduledSubroutine2AC400 =
        primaryEclManager000.GetSubroutine(g_EnemySubroutineSchedule[0] & 0x7F);
    scheduleRuntime2AC3E0.timer0C = 200;
    scheduleRuntime2AC3E0.timer00 = 0;
}
