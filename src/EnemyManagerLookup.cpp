#include "EnemyManager.hpp"

EnemyView *EnemyManagerView::FindActiveEnemyBySideCategory(int sideCategory)
{
    EnemyView *enemy = enemies5758;
    for (int i = 0; i < 128; ++i, ++enemy)
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
