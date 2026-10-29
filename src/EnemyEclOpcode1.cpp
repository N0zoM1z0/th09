#include "EnemyEclExInstructions.hpp"
#include "SoundPlayer.hpp"
#include "EnemyManager.hpp"

struct EnemyEclOpcode1Flags
{
    unsigned int bit0 : 1;
    unsigned int remaining : 31;
};

struct EnemyEclOpcode1SideView
{
    unsigned char unknown00[0x34];
    EnemyEclOpcode1Flags flags34;
};

void __fastcall EnemyEclExInstruction1(
    EnemyView *enemy, EnemyEclExInstructionView *instruction)
{
    reinterpret_cast<EnemyEclOpcode1SideView *>(
        enemy->manager00->sideState320)->flags34.bit0 =
            instruction->value10;

    reinterpret_cast<EnemyEclOpcode1SideView *>(
        enemy->manager00->opposingSideState324)->flags34.bit0 =
            instruction->value10;

    if (instruction->value10 != 0)
        g_SoundPlayer.PlaySoundByIdx(0x34, 0);
}
