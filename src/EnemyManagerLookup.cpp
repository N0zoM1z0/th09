#include "EnemyManager.hpp"
#include "EnemyNativePoolCapacity.hpp"

EnemyView *EnemyManagerView::FindActiveEnemyBySideCategory(int sideCategory)
{
    EnemyView *enemy = enemies5758;
    for (int i = 0; i < EnemyNativePoolCapacity::ActiveSlots; ++i, ++enemy)
    {
        int enemySideCategory = (enemy->rewardFlags3380 >> 10) & 3;
        if (enemySideCategory == sideCategory &&
            (enemy->flags337C & 1) != 0)
        {
            return enemy;
        }
    }
    return NULL;
}
