#include "EnemyManager.hpp"

EnemyView *EnemyManagerView::SpawnEnemyUsingOpposingEcl(
    int eclSubroutineId,
    EnemyFloat3 *position,
    int life,
    int defeatTokenType,
    int score,
    int *initialVariables)
{
    return this->SpawnEnemy(
        static_cast<short>(eclSubroutineId),
        position,
        life,
        static_cast<signed char>(defeatTokenType),
        score,
        initialVariables,
        1);
}
