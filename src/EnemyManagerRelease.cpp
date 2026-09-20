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
