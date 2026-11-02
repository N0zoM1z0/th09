#include "EnemyEclExInstructions.hpp"
#include "AsciiManager.hpp"
#include "EnemyManager.hpp"
#include "ExAttackController.hpp"
#include <stddef.h>

typedef char EnemyExAttackMoreNativeContextFields[
    (offsetof(EnemyView, activeContext2CE0) == 0x2CE0 &&
     offsetof(Th09EclContextView, floatVariables3C) == 0x3C &&
     offsetof(Th09EclContextView, floatVariables6C) == 0x6C &&
     offsetof(EnemyView, position2D74) == 0x2D74 &&
     offsetof(EnemyView, manager00) == 0x00) ? 1 : -1];

extern ExAttackController *g_ExAttackController;

void __fastcall EnemyEclExInstruction6(
    EnemyView *enemy, EnemyEclExInstructionView *)
{

    Float3 position;
    position.x = enemy->activeContext2CE0->floatVariables6C[0];
    position.y = enemy->activeContext2CE0->floatVariables6C[1];
    position.z = 0.0f;
    g_ExAttackController->Spawn(
        10, &position,
        1 - enemy->manager00->sideIndex31C, 0);
}

void __fastcall EnemyEclExInstruction7(
    EnemyView *enemy, EnemyEclExInstructionView *)
{

    Float3 position;
    position.x = enemy->activeContext2CE0->floatVariables6C[0];
    position.y = enemy->activeContext2CE0->floatVariables6C[1];
    position.z = enemy->activeContext2CE0->floatVariables3C[0];
    g_ExAttackController->Spawn(
        11, &position,
        1 - enemy->manager00->sideIndex31C, 0);
}

void __fastcall EnemyEclExInstruction8(
    EnemyView *enemy, EnemyEclExInstructionView *)
{

    Float3 position;
    position.x = enemy->activeContext2CE0->floatVariables6C[0];
    position.y = enemy->activeContext2CE0->floatVariables6C[1];
    position.z = enemy->activeContext2CE0->floatVariables3C[0];
    g_ExAttackController->Spawn(
        12, &position,
        1 - enemy->manager00->sideIndex31C, 0);
}

void __fastcall EnemyEclExInstruction9(
    EnemyView *enemy, EnemyEclExInstructionView *)
{

    Float3 parameter;

    parameter.FromAngleMagnitude(1.5707964f, 64.0f);
    parameter += reinterpret_cast<const Float3 &>(enemy->position2D74);
    parameter.z = 1.5707964f;
    g_ExAttackController->Spawn(
        22, reinterpret_cast<const Float3 *>(&enemy->position2D74), 1 - enemy->manager00->sideIndex31C,
        reinterpret_cast<int>(&parameter));

    parameter.FromAngleMagnitude(1.2566371f, 64.0f);
    parameter += reinterpret_cast<const Float3 &>(enemy->position2D74);
    parameter.z = 1.2566371f;
    g_ExAttackController->Spawn(
        22, reinterpret_cast<const Float3 *>(&enemy->position2D74), 1 - enemy->manager00->sideIndex31C,
        reinterpret_cast<int>(&parameter));

    parameter.FromAngleMagnitude(1.8849556f, 64.0f);
    parameter += reinterpret_cast<const Float3 &>(enemy->position2D74);
    parameter.z = 1.8849556f;
    g_ExAttackController->Spawn(
        22, reinterpret_cast<const Float3 *>(&enemy->position2D74), 1 - enemy->manager00->sideIndex31C,
        reinterpret_cast<int>(&parameter));
}
