#include "EnemyChildEclBlocks.hpp"
#include "EnemyManager.hpp"
#include "ZunMemory.hpp"

#include <stddef.h>

void __fastcall EnemyCoreReleaseChildEclBlocks(EnemyView *enemyObject)
{

    for (int childIndex = 0; childIndex < 4; childIndex++)
    {
        if (enemyObject->childEclBlocks33D8[childIndex] != 0)
        {
            g_ZunMemory.Free(enemyObject->childEclBlocks33D8[childIndex]);
            enemyObject->childEclBlocks33D8[childIndex] = 0;
        }
    }
}
