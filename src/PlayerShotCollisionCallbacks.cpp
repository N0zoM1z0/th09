#include "PlayerLifecycleView.hpp"

// Preserve the original callback ABI tags; the player is the full native
// PlayerLifecycleView and the position argument remains opaque here.
struct PlayerShotCallbackPlayerView;
struct PlayerShotCallbackPositionView;
int __fastcall PlayerShotCollisionCallbackType1(
    PlayerShotCallbackPlayerView *player,
    void *,
    PlayerShotCallbackPositionView *)
{
    PlayerLifecycleView *nativePlayer =
        reinterpret_cast<PlayerLifecycleView *>(player);
    ++nativePlayer->shotCollisionCount30354;
    return 0;
}

int __fastcall PlayerShotCollisionCallbackType2(
    PlayerShotCallbackPlayerView *,
    void *,
    PlayerShotCallbackPositionView *)
{
    return 1;
}
