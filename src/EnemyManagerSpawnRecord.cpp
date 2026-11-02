#include "EnemyManagerCardAttackSpawn.hpp"
#include "EnemyManager.hpp"

void *EnemyManagerCardAttackSpawnView::SpawnEnemyRecord(
    int subroutine,
    CardAttackSpawnPosition *position,
    int life,
    char defeatTokenType,
    int score,
    int mirrorMovementX,
    int useOpposingEclManager)
{
    EnemyManagerView *manager = reinterpret_cast<EnemyManagerView *>(this);
    int enemyIndex;
    EnemyView *enemy;

    enemy = &manager->enemies5758[0];
    for (enemyIndex = 0; enemyIndex < 128; ++enemyIndex, ++enemy)
    {
        if ((enemy->flags337C & 1) != 0)
            continue;

        *enemy = manager->spawnTemplate328;
        enemy->sequenceIndex2E58 = enemyIndex;
        enemy->primaryFlagBits337C.mirrorMovementX15 = mirrorMovementX;

        if (life >= 0)
            enemy->life2E48 = life;

        enemy->position2D74 = *reinterpret_cast<EnemyFloat3 *>(position);
        if (mirrorMovementX)
            enemy->position2D74.x = -enemy->position2D74.x;

        if (useOpposingEclManager == 0)
        {
            manager->primaryEclManager000.InitializeSubroutine(
                &enemy->mainContext07F4,
                subroutine);
            if (manager->primaryEclManager000.RunEcl(
                    enemy) == -1)
            {
                enemy->flags337C &= ~1U;
                enemyIndex = 128;
                break;
            }
        }
        else
        {
            manager->opposingEclManager188.InitializeSubroutine(
                &enemy->mainContext07F4,
                subroutine);
            if (manager->opposingEclManager188.RunEcl(
                    enemy) == -1)
            {
                enemy->flags337C &= ~1U;
                enemyIndex = 128;
                break;
            }
        }

        enemy->displayColor2E70 = enemy->primaryAnmVm0008.color1;
        enemy->defeatTokenType335C = defeatTokenType;
        if (score >= 0)
            enemy->score2E54 = score;
        enemy->maxLife2E4C = enemy->life2E48;
        enemy->phaseStartingLife2E50 = enemy->life2E48;
        break;
    }

    manager->spawnFailure2AC42C = enemyIndex == 128;
    return enemy;
}
