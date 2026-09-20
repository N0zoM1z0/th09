#include "ZunMemory.hpp"

#include <stddef.h>

struct EnemyCoreView
{
    unsigned char unknown000[0x33D8];
    void *childEclBlocks33D8[4];
};

typedef char EnemyCoreReleaseChildrenAt33D8[
    (offsetof(EnemyCoreView, childEclBlocks33D8) == 0x33D8) ? 1 : -1];

void EnemyCoreReleaseChildEclBlocks(EnemyCoreView *enemy)
{
    for (int childIndex = 0; childIndex < 4; childIndex++)
    {
        if (enemy->childEclBlocks33D8[childIndex] != 0)
        {
            g_ZunMemory.Free(enemy->childEclBlocks33D8[childIndex]);
            enemy->childEclBlocks33D8[childIndex] = 0;
        }
    }
}
