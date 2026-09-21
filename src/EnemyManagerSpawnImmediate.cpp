#include "EnemyManager.hpp"

EnemyView *EnemyManagerView::SpawnEnemyImmediate(
    int eclSubroutineId,
    EnemyFloat3 *position,
    int argument4,
    int argument5,
    int argument6,
    int *initialVariables)
{
    return this->SpawnEnemy(
        static_cast<short>(eclSubroutineId),
        position,
        argument4,
        static_cast<signed char>(argument5),
        argument6,
        initialVariables,
        1);
}
