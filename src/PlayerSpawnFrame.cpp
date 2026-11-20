#include "PlayerSpawnFrameView.hpp"
#include "PlayerShotInitializeView.hpp"

int __fastcall PlayerSpawnFrameView::SpawnShotWhenFrameMatches(
    PlayerShotInitShotView *shot,
    int value,
    PlayerShotInitDescriptorView *entry)
{
    PlayerShotCtorView *nativeShot =
        reinterpret_cast<PlayerShotCtorView *>(shot);
    PlayerShtDescriptorView *nativeEntry =
        reinterpret_cast<PlayerShtDescriptorView *>(entry);
    if (value == nativeEntry->fireInterval)
    {
        reinterpret_cast<PlayerLifecycleView *>(this)->InitializeShot(
            nativeShot, nativeEntry);
        return 1;
    }
    return 0;
}
