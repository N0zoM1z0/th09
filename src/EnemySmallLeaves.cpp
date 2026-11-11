#include "EnemyManager.hpp"

int EnemyEclManagerView::GetSubroutineCount()
{
    return rawFile00->subroutineCount;
}

// Retail treats the variable-length ECL subroutine table as physical
// pointer words and scales the signed lookup index modulo 2^32.
void *EnemyEclManagerView::GetSubroutine(int index)
{
    return *reinterpret_cast<void *const *>(
        reinterpret_cast<unsigned int>(&rawFile00->resolvedSubroutines[0]) +
        static_cast<unsigned int>(index) * sizeof(rawFile00->resolvedSubroutines[0]));
}

void EnemyView::ResetBulletRankInfluence()
{
    bulletRankSpeedLow2E38 = -0.5f;
    bulletRankSpeedHigh2E3C = 0.5f;
    bulletRankCount1Low2E40 = 0;
    bulletRankCount1High2E42 = 0;
    bulletRankCount2Low2E44 = 0;
    bulletRankCount2High2E46 = 0;
}
