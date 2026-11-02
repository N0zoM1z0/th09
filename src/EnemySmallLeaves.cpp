#include "EnemyManager.hpp"

int EnemyEclManagerView::GetSubroutineCount()
{
    return rawFile00->subroutineCount;
}

void *EnemyEclManagerView::GetSubroutine(int index)
{
    return rawFile00->resolvedSubroutines[index];
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
