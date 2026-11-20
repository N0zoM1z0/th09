#include "AsciiGameManagerView.hpp"
#include "AnmManager.hpp"
#include "PlayerLifecycleView.hpp"

struct PlayerShotDraw1PlayerView;
// Retain the original callback's opaque argument tag for its ABI symbol.
struct PlayerShotDraw1ShotView;

int __fastcall PlayerShotDrawCallbackType1(
    PlayerShotDraw1PlayerView *,
    PlayerShotDraw1ShotView *shot)
{
    // Recover the full constructed PlayerShot allocation instead of
    // maintaining a second sparse view of its VM/history/trail tail.
    PlayerShotCtorView *nativeShot =
        reinterpret_cast<PlayerShotCtorView *>(shot);
    unsigned int alpha = nativeShot->vm.color1Bytes[3];
    int index = 0;

    if (2 * nativeShot->trailSegmentCount46A > 0)
    {
        int alphaAccumulator = 0;
        PlayerConstructedVector3View *point = nativeShot->trailVectors2B0;

        do
        {
            if (*reinterpret_cast<int *>(&point->x) ==
                static_cast<int>(0xC479C000u))
            {
                // The invalid-history sentinel exits this drawn-trail path
                // after restoring the shared VM alpha byte.
                nativeShot->vm.color1Bytes[3] = static_cast<unsigned char>(alpha);
                return 0;
            }

            nativeShot->vm.pos.x =
                g_GameManager.TransformPopupX(point->x);
            nativeShot->vm.pos.y =
                g_GameManager.TransformPopupY(point->y);
            nativeShot->vm.pos.z = 0.0f;

            nativeShot->vm.color1Bytes[3] = static_cast<unsigned char>(
                alpha -
                alphaAccumulator /
                    (2 * nativeShot->trailSegmentCount46A));

            g_AnmManager->Draw2D(
                &nativeShot->vm);

            ++index;
            // The original drawn trail advances through genuine contiguous
        // 0x0C-byte shot history records using an i386 DWORD address,
        // not a guessed independent C++ pointer/array bound.
        point = reinterpret_cast<PlayerConstructedVector3View *>(
            reinterpret_cast<unsigned int>(point) + sizeof(*point));
            alphaAccumulator += alpha;
        }
        while (index < 2 * nativeShot->trailSegmentCount46A);
        nativeShot->vm.color1Bytes[3] = static_cast<unsigned char>(alpha);
        return 0;
    }

    // The zero-trip path has a separate physical restore/return tail in TH09.
    nativeShot->vm.color1Bytes[3] = static_cast<unsigned char>(alpha);
    return 0;
}
