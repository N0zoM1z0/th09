#include "EnemyManager.hpp"
#include "ExAttackController.hpp"
#include <stddef.h>

struct EnemyExAttackView
{
    EnemyManagerView *manager00;
    unsigned char unknown0004[0x2D74 - 0x0004];
    Float3 position2D74;

    void SpawnExAttackType1();
    void SpawnExAttackType7();
    void SpawnExAttackType8();
    void SpawnExAttackType9();
    void SpawnExAttackType23();
    void SpawnExAttackType24();
};

typedef char EnemyExAttackPositionAt2D74[
    (offsetof(EnemyExAttackView, position2D74) == 0x2D74) ? 1 : -1];

extern ExAttackController *g_ExAttackController;

#define DEFINE_EXATTACK_WRAPPER(name, typeValue) \
void EnemyExAttackView::name() \
{ \
    g_ExAttackController->Spawn( \
        typeValue, &position2D74, 1 - manager00->sideIndex31C, 0); \
}

DEFINE_EXATTACK_WRAPPER(SpawnExAttackType1, 1)
DEFINE_EXATTACK_WRAPPER(SpawnExAttackType7, 7)
DEFINE_EXATTACK_WRAPPER(SpawnExAttackType8, 8)
DEFINE_EXATTACK_WRAPPER(SpawnExAttackType9, 9)
DEFINE_EXATTACK_WRAPPER(SpawnExAttackType23, 0x17)
DEFINE_EXATTACK_WRAPPER(SpawnExAttackType24, 0x18)
