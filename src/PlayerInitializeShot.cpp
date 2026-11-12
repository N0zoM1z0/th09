#include "PlayerShotInitializeView.hpp"
#include "AnmManager.hpp"
#include "SoundPlayer.hpp"

#include <math.h>

void __fastcall PlayerLifecycleView::InitializeShot(
    PlayerShotCtorView *shot,
    PlayerShtDescriptorView *entry)
{
    PlayerLifecycleView &nativePlayer = *this;
    PlayerShotCtorView &nativeShot = *shot;
    PlayerShtDescriptorView &nativeEntry = *entry;
    if (nativeEntry.sourceOptionIndex20 == 0)
        nativeShot.position2A4 = nativePlayer.position1B88;
    else
        reinterpret_cast<Float3 &>(nativeShot.position2A4) =
            nativePlayer.optionStates[nativeEntry.sourceOptionIndex20 - 1].position2A4;

    // The original conversion is retained; its result points at the x
    // member of the genuine shot Float3, not a stand-alone float array.
    Float3 *position = reinterpret_cast<Float3 *>(
        reinterpret_cast<Float3 &>(nativeShot.position2A4).operator float *());
    position->x += nativeEntry.positionOffsetX04;
    float &positionY = position->y;
    positionY += nativeEntry.positionOffsetY08;
    position->z = 0.495f;

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
