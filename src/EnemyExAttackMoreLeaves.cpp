#include "EnemyEclExInstructions.hpp"
#include "AsciiManager.hpp"
#include "EnemyManager.hpp"
#include "ExAttackController.hpp"
#include <stddef.h>

struct EnemyExAttackContextValuesView
{
    unsigned char unknown00[0x3C];
    float value3C;
    unsigned char unknown40[0x2C];
    float value6C;
    float value70;
};

struct EnemyExAttackMoreView
{
    EnemyManagerView *manager00;
    unsigned char unknown0004[0x2CE0 - 0x0004];
    EnemyExAttackContextValuesView *activeContext2CE0;
    unsigned char unknown2CE4[0x90];
    Float3 position2D74;
};

typedef char EnemyExAttackMoreContextAt2CE0[
    (offsetof(EnemyExAttackMoreView, activeContext2CE0) == 0x2CE0) ? 1 : -1];
typedef char EnemyExAttackMorePositionAt2D74[
    (offsetof(EnemyExAttackMoreView, position2D74) == 0x2D74) ? 1 : -1];

extern ExAttackController *g_ExAttackController;

void __fastcall EnemyEclExInstruction6(
    EnemyView *enemy, EnemyEclExInstructionView *)
{
    EnemyExAttackMoreView *view =
        reinterpret_cast<EnemyExAttackMoreView *>(enemy);
    Float3 position;
    position.x = view->activeContext2CE0->value6C;
    position.y = view->activeContext2CE0->value70;
    position.z = 0.0f;
    g_ExAttackController->Spawn(
        10, &position,
        1 - view->manager00->sideIndex31C, 0);
}

void __fastcall EnemyEclExInstruction7(
    EnemyView *enemy, EnemyEclExInstructionView *)
{
    EnemyExAttackMoreView *view =
        reinterpret_cast<EnemyExAttackMoreView *>(enemy);
    Float3 position;
    position.x = view->activeContext2CE0->value6C;
    position.y = view->activeContext2CE0->value70;
    position.z = view->activeContext2CE0->value3C;
    g_ExAttackController->Spawn(
        11, &position,
        1 - view->manager00->sideIndex31C, 0);
}

void __fastcall EnemyEclExInstruction8(
    EnemyView *enemy, EnemyEclExInstructionView *)
{
    EnemyExAttackMoreView *view =
        reinterpret_cast<EnemyExAttackMoreView *>(enemy);
    Float3 position;
    position.x = view->activeContext2CE0->value6C;
    position.y = view->activeContext2CE0->value70;
    position.z = view->activeContext2CE0->value3C;
    g_ExAttackController->Spawn(
        12, &position,
        1 - view->manager00->sideIndex31C, 0);
}

void __fastcall EnemyEclExInstruction9(
    EnemyView *enemy, EnemyEclExInstructionView *)
{
    EnemyExAttackMoreView *view =
        reinterpret_cast<EnemyExAttackMoreView *>(enemy);
    Float3 parameter;

    parameter.FromAngleMagnitude(1.5707964f, 64.0f);
    parameter += view->position2D74;
    parameter.z = 1.5707964f;
    g_ExAttackController->Spawn(
        22, &view->position2D74, 1 - view->manager00->sideIndex31C,
        reinterpret_cast<int>(&parameter));

    parameter.FromAngleMagnitude(1.2566371f, 64.0f);
    parameter += view->position2D74;
    parameter.z = 1.2566371f;
    g_ExAttackController->Spawn(
        22, &view->position2D74, 1 - view->manager00->sideIndex31C,
        reinterpret_cast<int>(&parameter));

    parameter.FromAngleMagnitude(1.8849556f, 64.0f);
    parameter += view->position2D74;
    parameter.z = 1.8849556f;
    g_ExAttackController->Spawn(
        22, &view->position2D74, 1 - view->manager00->sideIndex31C,
        reinterpret_cast<int>(&parameter));
}
