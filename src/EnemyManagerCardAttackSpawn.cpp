#include "EnemyManagerCardAttackSpawn.hpp"

void *EnemyManagerCardAttackSpawnView::SpawnFromCardAttack(
    int subroutine,
    CardAttackSpawnPosition *position,
    int life,
    int defeatTokenType,
    int score,
    int mirrorMovementX)
{
    return SpawnEnemyRecord(
        subroutine,
        position,
        life,
        static_cast<char>(defeatTokenType),
        score,
        mirrorMovementX,
        1);
}
