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

    void SpawnExAttackType10();
    void SpawnExAttackType11();
    void SpawnExAttackType12();
    void SpawnExAttackType22Burst();
};

typedef char EnemyExAttackMoreContextAt2CE0[
    (offsetof(EnemyExAttackMoreView, activeContext2CE0) == 0x2CE0) ? 1 : -1];
typedef char EnemyExAttackMorePositionAt2D74[
    (offsetof(EnemyExAttackMoreView, position2D74) == 0x2D74) ? 1 : -1];

extern ExAttackController *g_ExAttackController;

void EnemyExAttackMoreView::SpawnExAttackType10()
{
    Float3 position;
    position.x = activeContext2CE0->value6C;
    position.y = activeContext2CE0->value70;
    position.z = 0.0f;
    g_ExAttackController->Spawn(
        10, &position,
        1 - manager00->sideIndex31C, 0);
}

void EnemyExAttackMoreView::SpawnExAttackType11()
{
    Float3 position;
    position.x = activeContext2CE0->value6C;
    position.y = activeContext2CE0->value70;
    position.z = activeContext2CE0->value3C;
    g_ExAttackController->Spawn(
        11, &position,
        1 - manager00->sideIndex31C, 0);
}

void EnemyExAttackMoreView::SpawnExAttackType12()
{
    Float3 position;
    position.x = activeContext2CE0->value6C;
    position.y = activeContext2CE0->value70;
    position.z = activeContext2CE0->value3C;
    g_ExAttackController->Spawn(
        12, &position,
        1 - manager00->sideIndex31C, 0);
}

void EnemyExAttackMoreView::SpawnExAttackType22Burst()
{
    Float3 parameter;

    parameter.FromAngleMagnitude(1.5707964f, 64.0f);
    parameter += position2D74;
    parameter.z = 1.5707964f;
    g_ExAttackController->Spawn(
        22, &position2D74, 1 - manager00->sideIndex31C,
        reinterpret_cast<int>(&parameter));

    parameter.FromAngleMagnitude(1.2566371f, 64.0f);
    parameter += position2D74;
    parameter.z = 1.2566371f;
    g_ExAttackController->Spawn(
        22, &position2D74, 1 - manager00->sideIndex31C,
        reinterpret_cast<int>(&parameter));

    parameter.FromAngleMagnitude(1.8849556f, 64.0f);
    parameter += position2D74;
    parameter.z = 1.8849556f;
    g_ExAttackController->Spawn(
        22, &position2D74, 1 - manager00->sideIndex31C,
        reinterpret_cast<int>(&parameter));
}
