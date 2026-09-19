#include <stddef.h>

struct PlayerSpawnShotsView;
struct PlayerSpawnShotsShotView;
struct PlayerSpawnShotsDescriptorView;

typedef int (__fastcall *PlayerSpawnCallback)(
    PlayerSpawnShotsView *player,
    PlayerSpawnShotsShotView *shot,
    int value,
    PlayerSpawnShotsDescriptorView *entry);

struct PlayerSpawnShotsDescriptorView
{
    short scheduleValue00;
    unsigned char unknown02[0x26];
    PlayerSpawnCallback spawnCallback28;
    void *updateCallback2C;
    void *drawCallback30;
    void *collisionCallback34;
};

typedef char PlayerSpawnShotsDescriptorSizeIs38[
    (sizeof(PlayerSpawnShotsDescriptorView) == 0x38) ? 1 : -1];

struct PlayerSpawnShotsPowerLevelView
{
    PlayerSpawnShotsDescriptorView *descriptors00;
    int unknown04;
};

struct PlayerSpawnShotsShtFileView
{
    unsigned char unknown000[0x42C];
    PlayerSpawnShotsPowerLevelView shotPowerLevels42C[1];
};

struct PlayerSpawnShotsShotView
{
    unsigned char unknown000[0x1F8];
    unsigned int vmFlags1F8;
    unsigned char unknown1FC[0x462 - 0x1FC];
    short state462;
    unsigned char unknown464[0x474 - 0x464];
    void *updateCallback474;
    void *drawCallback478;
    void *collisionCallback47C;
    PlayerSpawnShotsDescriptorView *descriptor480;
};

typedef char PlayerSpawnShotsShotSizeIs484[
    (sizeof(PlayerSpawnShotsShotView) == 0x484) ? 1 : -1];

struct PlayerSpawnShotsView
{
    unsigned char unknown00000[0xC11C];
    PlayerSpawnShotsShotView shotsC11C[128];
    unsigned char unknown3031C[0x30338 - 0x3031C];
    PlayerSpawnShotsShtFileView *primaryShtFile30338;

    int __fastcall SpawnShotWhenFrameMatches(
        PlayerSpawnShotsShotView *shot,
        int value,
        PlayerSpawnShotsDescriptorView *entry);

    void __fastcall SpawnShots(int value, int powerLevel);
};

typedef char PlayerSpawnShotsSlotsAtC11C[
    (offsetof(PlayerSpawnShotsView, shotsC11C) == 0xC11C) ? 1 : -1];
typedef char PlayerSpawnShotsShtAt30338[
    (offsetof(PlayerSpawnShotsView, primaryShtFile30338) == 0x30338) ? 1 : -1];

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
