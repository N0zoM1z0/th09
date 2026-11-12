#include "ZunTimer.hpp"
#include "PlayerLifecycleView.hpp"

// Preserve the actual callback argument ABI while using the true Player and
// 0x484-byte constructed Shot allocation for every field.
struct PlayerShotUpdate2PlayerView;
struct PlayerShotUpdate2ShotView;
typedef char PlayerShot2NativeShapes[
    (offsetof(PlayerLifecycleView, state3031C) == 0x3031C &&
     offsetof(PlayerState3031CView, timer00) == 0 &&
     offsetof(PlayerState3031CView, shot0C) == 0x0C &&
     offsetof(PlayerShotCtorView, position2A4) == 0x2A4 &&
     offsetof(PlayerShotCtorView, trailVectors2B0) == 0x2B0 &&
     offsetof(PlayerShotCtorView, hitbox430) == 0x430 &&
     offsetof(PlayerShotCtorView, velocity43C) == 0x43C &&
     offsetof(PlayerShotCtorView, field46A) == 0x46A &&
     sizeof(PlayerShotCtorView) == 0x484) ? 1 : -1];

int __fastcall PlayerShotUpdateCallbackType2(
    PlayerShotUpdate2PlayerView *player,
    PlayerShotUpdate2ShotView *shot)
{
    PlayerLifecycleView *lifecycle = reinterpret_cast<PlayerLifecycleView *>(player);
    PlayerShotCtorView *nativeShot = reinterpret_cast<PlayerShotCtorView *>(shot);
    ZunTimer *stateTimer = &lifecycle->state3031C.timer00;

    if (*stateTimer <= 0)
    {
        *stateTimer = 0;
        lifecycle->state3031C.shot0C = 0;
        nativeShot->state462 = 0;
        return 1;
    }

    float velocityZ = nativeShot->velocity43C.z;
    nativeShot->position2A4 = lifecycle->position1B88;
    float newX = velocityZ + nativeShot->position2A4.x;
    nativeShot->position2A4.z = 0.44f;
    nativeShot->position2A4.x = newX;
    nativeShot->hitbox430.y = nativeShot->position2A4.y;
    nativeShot->vm.scale.y = nativeShot->position2A4.y * (1.0f / 14.0f);
    nativeShot->position2A4.y *= 0.5f;

    for (int i = 31; i > 0; --i)
    {
        nativeShot->trailVectors2B0[i] = nativeShot->trailVectors2B0[i - 1];
        nativeShot->trailVectors2B0[i].y -= 1.0f;
    }

    // Original unbounded trail count increments its signed i386 loop word
    // with a wrapping DWORD INC, preserving the comparison below.
    for (int i = 0; i < nativeShot->field46A;
         ++*reinterpret_cast<unsigned int *>(&i))
    {
        // The live even-indexed trail samples reside in the original 0x0C-byte
        // Float3 records at shot +0x2B0; use native i386 byte-address math.
        PlayerConstructedVector3View *point = reinterpret_cast<PlayerConstructedVector3View *>(
            reinterpret_cast<unsigned int>(&nativeShot->trailVectors2B0[0]) +
            // Each paired sample index is multiplied in one real DWORD.
            (static_cast<unsigned int>(i) * 2u) * sizeof(nativeShot->trailVectors2B0[0]));
        if (point->x >= -900.0f)
            lifecycle->CreateRectType4(
                reinterpret_cast<const PlayerRegionPointView *>(point),
                4.0f, 448.0f, 1, 1, 0);
    }

    nativeShot->trailVectors2B0[0] =
        *reinterpret_cast<const PlayerConstructedVector3View *>(&nativeShot->position2A4);
    (*stateTimer)--;
    return 0;
}
