#include "EnemyEclExInstructions.hpp"
#include "SoundPlayer.hpp"
#include "EnemyManager.hpp"

void __fastcall EnemyEclExInstruction1(
    EnemyView *enemy, EnemyEclExInstructionView *instruction)
{
    enemy->manager00->sideState320->flagBits34.bit0 =
            instruction->value10;

    enemy->manager00->opposingSideState324->flagBits34.bit0 =
            instruction->value10;

    if (instruction->value10 != 0)
        g_SoundPlayer.PlaySoundByIdx(SOUND_TIME_STOP, 0);
}
