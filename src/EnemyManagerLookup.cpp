#include "EnemyManager.hpp"
#include "EnemyNativePoolCapacity.hpp"

EnemyView *EnemyManagerView::FindActiveEnemyBySideCategory(int sideCategory)
{
    EnemyView *enemy = enemies5758;
    for (int i = 0; i < EnemyNativePoolCapacity::ActiveSlots; ++i, ++enemy)
    {
        int enemySideCategory = enemy->secondaryFlagBits3380.category.sideCategory10_11;
        if (enemySideCategory == sideCategory &&
            enemy->primaryFlagBits337C.active00 != 0)
        {
            return enemy;
        }
    }
    return NULL;
}
