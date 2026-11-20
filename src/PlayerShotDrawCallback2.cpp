#include "AnmManager.hpp"
#include "PlayerLifecycleView.hpp"

struct PlayerShotDraw2PlayerView;

// Preserve the callback ABI's original opaque parameter tag.
struct PlayerShotDraw2ShotView;
int __fastcall PlayerShotDrawCallbackType2(
    PlayerShotDraw2PlayerView *,
    PlayerShotDraw2ShotView *shot)
{
    // This callback shares the same constructed 0x484-byte shot allocation
    // as the Type1 draw path. Its four samples are the first four native
    // trail vectors, and its scale/color members are embedded in the ANM VM.
    PlayerShotCtorView *nativeShot =
        reinterpret_cast<PlayerShotCtorView *>(shot);
    unsigned int savedRed = nativeShot->vm.color1Bytes[2];
    unsigned int savedGreen = nativeShot->vm.color1Bytes[1];
    unsigned int savedBlue = nativeShot->vm.color1Bytes[0];
    unsigned int savedAlpha = nativeShot->vm.color1Bytes[3];

    int index = 0;
    int alphaAccumulator = 0;
    int greenAccumulator = 0;
    int redAccumulator = 0;
    PlayerConstructedVector3View *point = nativeShot->trailVectors2B0;

    do
    {
        if (*reinterpret_cast<int *>(&point->x) ==
            static_cast<int>(0xC479C000u))
        {
            break;
        }

        float progress = static_cast<float>(index) * 0.33333334f;
        nativeShot->vm.color1Bytes[2] = static_cast<unsigned char>(
            redAccumulator / 4 + 63);
        nativeShot->vm.scale.x = 3.0f - progress;
        nativeShot->vm.color1Bytes[1] = static_cast<unsigned char>(
            greenAccumulator / 4 + 63);
        nativeShot->vm.scale.y = 4.0f - progress;
        nativeShot->vm.color1Bytes[3] = static_cast<unsigned char>(
            alphaAccumulator / 4 + 63);

        g_AnmManager->Draw2D(
            &nativeShot->vm);

        ++index;
        // The original drawn trail advances through genuine contiguous
        // 0x0C-byte shot history records using an i386 DWORD address,
        // not a guessed independent C++ pointer/array bound.
        point = reinterpret_cast<PlayerConstructedVector3View *>(
            reinterpret_cast<unsigned int>(point) + sizeof(*point));
        redAccumulator += savedRed;
        greenAccumulator += savedGreen;
        alphaAccumulator += savedAlpha;
    }
    while (index < 4);

    nativeShot->vm.color1Bytes[2] = static_cast<unsigned char>(savedRed);
    nativeShot->vm.color1Bytes[1] = static_cast<unsigned char>(savedGreen);
    nativeShot->vm.color1Bytes[0] = savedBlue;
    nativeShot->vm.color1Bytes[3] = static_cast<unsigned char>(savedAlpha);
    return 0;
}
