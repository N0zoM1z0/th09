#include "PlayerShotInitializeView.hpp"
#include "AnmManager.hpp"
#include "SoundPlayer.hpp"

#include <math.h>

void __fastcall PlayerShotInitPlayerView::InitializeShot(
    PlayerShotInitShotView *shot,
    PlayerShotInitDescriptorView *entry)
{
    PlayerLifecycleView &nativePlayer = *reinterpret_cast<PlayerLifecycleView *>(this);
    PlayerShotCtorView &nativeShot = *reinterpret_cast<PlayerShotCtorView *>(shot);
    PlayerShtDescriptorView &nativeEntry =
        *reinterpret_cast<PlayerShtDescriptorView *>(entry);
    if (nativeEntry.sourceOptionIndex20 == 0)
        nativeShot.position2A4 = nativePlayer.position1B88;
    else
        reinterpret_cast<Float3 &>(nativeShot.position2A4) =
            nativePlayer.optionStates[nativeEntry.sourceOptionIndex20 - 1].position2A4;

    float *position = reinterpret_cast<Float3 &>(nativeShot.position2A4).operator float *();
    position[0] += nativeEntry.positionOffsetX04;
    float &positionY = position[1];
    positionY += nativeEntry.positionOffsetY08;
    position[2] = 0.495f;

    nativeShot.hitbox430.x = nativeEntry.hitboxX0C;
    nativeShot.hitbox430.y = nativeEntry.hitboxY10;
    nativeShot.hitbox430.z = 1.0f;

    nativeShot.angle450 = nativeEntry.angle14;
    nativeShot.speed44C = nativeEntry.speed18;
    nativeShot.velocity43C.x = cosf(nativeEntry.angle14) * nativeEntry.speed18;
    nativeShot.velocity43C.y = sinf(nativeEntry.angle14) * nativeEntry.speed18;

    nativeShot.timer454 = 0;
    nativeShot.field470 = 0;
    nativeShot.shotType464 = nativeEntry.shotType22;
    nativeShot.damage460 = nativeEntry.damage1C;
    nativeShot.animationIndex46C = nativeEntry.animationIndex24;

    if (nativeEntry.soundIndex26 >= 0)
        g_SoundPlayer.PlaySoundPositionedByIdx(
            nativeEntry.soundIndex26, nativePlayer.position1B88.x);

    nativePlayer.header24.anmFile98->SetAndExecuteScriptIdx(
        reinterpret_cast<AnmVm *>(shot), nativeEntry.animationIndex24 + 5);
    nativeShot.tintFlag46E = 0;
}
