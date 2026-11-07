#include "PlayerLifecycleView.hpp"
#include "PlayerSpawnFrameView.hpp"

// The SHT format stores spawn callbacks as encoded function-pointer words.
// Narrow the native descriptor's void * only at the actual callback site.
typedef int (__fastcall *PlayerSpawnCallback)(
    PlayerLifecycleView *player,
    PlayerShotCtorView *shot,
    int value,
    PlayerShtDescriptorView *entry);

void __fastcall PlayerLifecycleView::SpawnShots(int value, int powerLevel)
{
    // The loader's native SHT power-level descriptor array and Player's
    // native shot records are the same objects used by update/draw.
    PlayerShtDescriptorView *entry = primaryShtFile->shotPowerLevels[powerLevel].descriptors;
    PlayerShotCtorView *shot = shots;
    int result;

    // The scanned 0x484-byte slots are the Player's actual 128-shot array;
    // do not give the shot producer an unrelated capacity constant.
    for (int i = 0; i < static_cast<int>(sizeof(shots) / sizeof(shots[0])); ++i, ++shot)
    {
        if (shot->state462 != 0)
            continue;

processEntry:
        if (entry->spawnCallback != 0)
            result = reinterpret_cast<PlayerSpawnCallback>(entry->spawnCallback)(this, shot, value, entry);
        else
            result = reinterpret_cast<PlayerSpawnFrameView *>(this)
                ->SpawnShotWhenFrameMatches(
                    reinterpret_cast<PlayerShotInitShotView *>(shot), value,
                    reinterpret_cast<PlayerShotInitDescriptorView *>(entry));

        if (result == 1)
        {
            shot->vm.flagsWord |= 0x2000u;
            shot->state462 = 1;
            shot->descriptor480 = entry;
            shot->updateCallback474 = reinterpret_cast<PlayerShotUpdateCallback>(entry->updateCallback);
            shot->drawCallback478 = reinterpret_cast<PlayerShotDrawCallback>(entry->drawCallback);
            shot->collisionCallback47C = entry->collisionCallback;
        }

        ++entry;
        if (entry->fireInterval < 0)
            return;
        if (result == 0)
            goto processEntry;
    }
}
