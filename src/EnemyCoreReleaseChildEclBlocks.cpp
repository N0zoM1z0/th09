#include "EnemyChildEclBlocks.hpp"
#include "ZunMemory.hpp"

#include <stddef.h>

struct EnemyChildEclBlocksView
{
    unsigned char unknown000[0x33D8];
    void *childEclBlocks33D8[4];
};

typedef char EnemyCoreReleaseChildrenAt33D8[
    (offsetof(EnemyChildEclBlocksView, childEclBlocks33D8) == 0x33D8) ? 1 : -1];

void __fastcall EnemyCoreReleaseChildEclBlocks(EnemyView *enemyObject)
{
    EnemyChildEclBlocksView *enemy =
        reinterpret_cast<EnemyChildEclBlocksView *>(enemyObject);
    for (int childIndex = 0; childIndex < 4; childIndex++)
    {
        if (enemy->childEclBlocks33D8[childIndex] != 0)
        {
            g_ZunMemory.Free(enemy->childEclBlocks33D8[childIndex]);
            enemy->childEclBlocks33D8[childIndex] = 0;
        }
    }
}
