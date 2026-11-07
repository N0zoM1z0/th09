#include "EnemyManager.hpp"
#include "EnemyNativePoolCapacity.hpp"

#include <string.h>

EnemyView *EnemyManagerView::SpawnEnemy(
    short eclSubroutineId,
    EnemyFloat3 *position,
    int life,
    signed char defeatTokenType,
    int score,
    int *initialVariables,
    int useOpposingEclManager)
{
    EnemyManagerView *manager = this;
    EnemyView *enemy = &manager->enemies5758[0];
    int enemyIndex;

    for (enemyIndex = 0; enemyIndex < EnemyNativePoolCapacity::ActiveSlots; ++enemyIndex, ++enemy)
    {
        if ((enemy->flags337C & 1) != 0)
            continue;

        *enemy = manager->spawnTemplate328;
        enemy->sequenceIndex2E58 = enemyIndex;

        if (life >= 0)
            enemy->life2E48 = life;

        enemy->position2D74 = *position;

        if (useOpposingEclManager == 0)
        {
            manager->primaryEclManager000.InitializeSubroutine(
                &enemy->mainContext07F4,
                eclSubroutineId);
            memcpy(
                &enemy->mainContext07F4.intVariables1C[0],
                initialVariables,
                0x78);
            if (manager->primaryEclManager000.RunEcl(
                    enemy) == -1)
            {
                enemy->flags337C &= ~1U;
                enemyIndex = EnemyNativePoolCapacity::FailureSentinelIndex;
                break;
            }
        }
        else
        {
            manager->opposingEclManager188.InitializeSubroutine(
                &enemy->mainContext07F4,
                eclSubroutineId);
            memcpy(
                &enemy->mainContext07F4.intVariables1C[0],
                initialVariables,
                0x78);
            if (manager->opposingEclManager188.RunEcl(
                    enemy) == -1)
            {
                enemy->flags337C &= ~1U;
                enemyIndex = EnemyNativePoolCapacity::FailureSentinelIndex;
                break;
            }
        }

        enemy->displayColor2E70 = enemy->primaryAnmVm0008.color1;
        enemy->defeatTokenType335C = defeatTokenType;

        if (life >= 0)
            enemy->life2E48 = life;
        if (score >= 0)
            enemy->score2E54 = score;

        enemy->maxLife2E4C = enemy->life2E48;
        enemy->phaseStartingLife2E50 = enemy->life2E48;
        break;
    }

    manager->spawnFailure2AC42C = enemyIndex == EnemyNativePoolCapacity::FailureSentinelIndex;
    return enemy;
}
