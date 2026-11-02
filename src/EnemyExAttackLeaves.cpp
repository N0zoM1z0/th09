#include "EnemyEclExInstructions.hpp"
#include "EnemyManager.hpp"
#include "ExAttackController.hpp"
#include <stddef.h>

typedef char EnemyExAttackNativePositionAt2D74[
    (offsetof(EnemyView, position2D74) == 0x2D74 &&
     offsetof(EnemyView, manager00) == 0x00 &&
     sizeof(((EnemyView *)0)->position2D74) == sizeof(Float3)) ? 1 : -1];

extern ExAttackController *g_ExAttackController;

#define DEFINE_EXATTACK_WRAPPER(name, typeValue) \
void __fastcall name(EnemyView *enemy, EnemyEclExInstructionView *) \
{ \
    g_ExAttackController->Spawn( \
        typeValue, reinterpret_cast<const Float3 *>(&enemy->position2D74), \
        1 - enemy->manager00->sideIndex31C, 0); \
}

DEFINE_EXATTACK_WRAPPER(EnemyEclExInstruction0, 1)
DEFINE_EXATTACK_WRAPPER(EnemyEclExInstruction3, 7)
DEFINE_EXATTACK_WRAPPER(EnemyEclExInstruction5, 8)
DEFINE_EXATTACK_WRAPPER(EnemyEclExInstruction4, 9)
DEFINE_EXATTACK_WRAPPER(EnemyEclExInstruction10, 0x17)
DEFINE_EXATTACK_WRAPPER(EnemyEclExInstruction11, 0x18)

// The reviewed twelve-callable prefix; no following zero/padding is invented.
EnemyEclExInstruction g_EnemyEclExInstructions[12] =
{
    EnemyEclExInstruction0,
    EnemyEclExInstruction1,
    EnemyEclExInstruction2,
    EnemyEclExInstruction3,
    EnemyEclExInstruction4,
    EnemyEclExInstruction5,
    EnemyEclExInstruction6,
    EnemyEclExInstruction7,
    EnemyEclExInstruction8,
    EnemyEclExInstruction9,
    EnemyEclExInstruction10,
    EnemyEclExInstruction11
};
