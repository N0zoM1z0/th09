#include "PlayerShotInitializeView.hpp"
#include "AnmManager.hpp"
#include "SoundPlayer.hpp"

#include <math.h>

void __fastcall PlayerShotInitPlayerView::InitializeShot(
    PlayerShotInitShotView *shot,
    PlayerShotInitDescriptorView *entry)
{
    if (entry->sourceOptionIndex20 == 0)
        shot->position2A4 = position1B88;
    else
        shot->position2A4 =
            optionStates1CEC[entry->sourceOptionIndex20 - 1].position2A4;

    float *position = shot->position2A4.operator float *();
    position[0] += entry->positionOffset04.x;
    float &positionY = position[1];
    positionY += entry->positionOffset04.y;
    position[2] = 0.495f;

    shot->hitbox430.x = entry->hitbox0C.x;
    shot->hitbox430.y = entry->hitbox0C.y;
    shot->hitbox430.z = 1.0f;

    shot->angle450 = entry->angle14;
    shot->speed44C = entry->speed18;
    shot->velocity43C.x = cosf(entry->angle14) * entry->speed18;
    shot->velocity43C.y = sinf(entry->angle14) * entry->speed18;

    shot->timer454 = 0;
    shot->field470 = 0;
    shot->shotType464 = entry->shotType22;
    shot->damage460 = entry->damage1C;
    shot->animationIndex46C = entry->animationIndex24;

    if (entry->soundIndex26 >= 0)
        g_SoundPlayer.PlaySoundPositionedByIdx(
            entry->soundIndex26, position1B88.x);

    anmFileBC->SetAndExecuteScriptIdx(
        reinterpret_cast<AnmVm *>(shot), entry->animationIndex24 + 5);
    shot->tintFlag46E = 0;
}
