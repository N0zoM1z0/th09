#include "PlayerShotInitializeView.hpp"
#include "ZunTimer.hpp"
#include "PlayerLifecycleView.hpp"

// This callback's +0x3031C timer/shot record is the same genuine
// PlayerState3031CView constructed by Player at 0x41EE20. It is not
// a second source-only 0x1C-byte callback state allocation.

typedef char PlayerShotSpawn1RealNativeState[
    (sizeof(PlayerState3031CView) == 0x1C &&
     offsetof(PlayerLifecycleView, state3031C) == 0x3031C &&
     offsetof(PlayerState3031CView, timer00) == 0x00 &&
     offsetof(PlayerState3031CView, shot0C) == 0x0C &&
     sizeof(PlayerShotCtorView) == 0x484 &&
     offsetof(PlayerShotCtorView, sourceOption468) == 0x468 &&
     offsetof(PlayerShotCtorView, field46A) == 0x46A &&
     offsetof(PlayerShotCtorView, trailVectors2B0) == 0x2B0 &&
     offsetof(PlayerShtDescriptorView, positionOffsetX04) == 0x04 &&
     offsetof(PlayerShtDescriptorView, positionOffsetY08) == 0x08 &&
     offsetof(PlayerShtDescriptorView, sourceOptionIndex20) == 0x20) ? 1 : -1];

int __fastcall PlayerShotSpawnCallbackType1(
    PlayerLifecycleView *player,
    PlayerShotCtorView *shot,
    int value,
    PlayerShtDescriptorView *entry)
{
    if (value != entry->fireInterval)
        return 0;

    player->state3031C.timer00 = 30;
    player->state3031C.shot0C = shot;

    shot->sourceOption468 = entry->sourceOptionIndex20;
    shot->velocity43C.z = entry->positionOffsetX04;
    shot->auxiliary448 = entry->positionOffsetY08;
    shot->field46A = 15;

    reinterpret_cast<PlayerShotInitPlayerView *>(player)->InitializeShot(
            reinterpret_cast<PlayerShotInitShotView *>(shot),
            reinterpret_cast<PlayerShotInitDescriptorView *>(entry));
    shot->shotType464 = 2;

    for (int i = 31; i >= 0; --i)
        shot->trailVectors2B0[i].x = -999.0f;
    shot->position2A4.x = -999.0f;

    return 1;
}
