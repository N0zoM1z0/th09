#include "PlayerSpawnShotsView.hpp"
#include "PlayerSpawnFrameView.hpp"

void __fastcall PlayerLifecycleView::SpawnShots(int value, int powerLevel)
{
    PlayerSpawnShotsDescriptorView *entry =
        reinterpret_cast<PlayerSpawnShotsShtFileView *>(primaryShtFile)->shotPowerLevels42C[powerLevel].descriptors00;
    PlayerSpawnShotsShotView *shot = reinterpret_cast<PlayerSpawnShotsShotView *>(shots);
    int result;

    // The scanned 0x484-byte slots are the Player's actual 128-shot array;
    // do not give the shot producer an unrelated capacity constant.
    for (int i = 0; i < static_cast<int>(sizeof(shots) / sizeof(shots[0])); ++i, ++shot)
    {
        if (shot->state462 != 0)
            continue;

processEntry:
        if (entry->spawnCallback28 != 0)
            result = entry->spawnCallback28(this, shot, value, entry);
        else
            result = reinterpret_cast<PlayerSpawnFrameView *>(this)
                ->SpawnShotWhenFrameMatches(
                    reinterpret_cast<PlayerShotInitShotView *>(shot), value,
                    reinterpret_cast<PlayerShotInitDescriptorView *>(entry));

        if (result == 1)
        {
            shot->vmFlags1F8 |= 0x2000u;
            shot->state462 = 1;
            shot->descriptor480 = entry;
            shot->updateCallback474 = entry->updateCallback2C;
            shot->drawCallback478 = entry->drawCallback30;
            shot->collisionCallback47C = entry->collisionCallback34;
        }

        ++entry;
        if (entry->scheduleValue00 < 0)
            return;
        if (result == 0)
            goto processEntry;
    }
}
