#include "PlayerSpawnShotsView.hpp"

void __fastcall PlayerSpawnShotsView::SpawnShots(int value, int powerLevel)
{
    PlayerSpawnShotsDescriptorView *entry =
        primaryShtFile30338->shotPowerLevels42C[powerLevel].descriptors00;
    PlayerSpawnShotsShotView *shot = shotsC11C;
    int result;

    for (int i = 0; i < 128; ++i, ++shot)
    {
        if (shot->state462 != 0)
            continue;

processEntry:
        if (entry->spawnCallback28 != 0)
            result = entry->spawnCallback28(this, shot, value, entry);
        else
            result = SpawnShotWhenFrameMatches(shot, value, entry);

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
