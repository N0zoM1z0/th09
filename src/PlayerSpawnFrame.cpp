#include "PlayerSpawnFrameView.hpp"
#include "PlayerShotInitializeView.hpp"

int __fastcall PlayerSpawnFrameView::SpawnShotWhenFrameMatches(
    PlayerShotInitShotView *shot,
    int value,
    PlayerShotInitDescriptorView *entry)
{
    if (value == entry->fireInterval00)
    {
        reinterpret_cast<PlayerLifecycleView *>(this)->InitializeShot(
            reinterpret_cast<PlayerShotCtorView *>(shot),
            reinterpret_cast<PlayerShtDescriptorView *>(entry));
        return 1;
    }
    return 0;
}
