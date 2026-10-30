#pragma once
#include <stddef.h>

class AnmLoaded;

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
    unsigned char unknown00000[0xBC];
    AnmLoaded *anmFileBC;
    unsigned char unknown000C0[0xC11C - 0xC0];
    PlayerSpawnShotsShotView shotsC11C[128];
    unsigned char unknown3031C[0x30338 - 0x3031C];
    PlayerSpawnShotsShtFileView *primaryShtFile30338;

    void __fastcall SpawnShots(int value, int powerLevel);
};

typedef char PlayerSpawnShotsSlotsAtC11C[
    (offsetof(PlayerSpawnShotsView, shotsC11C) == 0xC11C) ? 1 : -1];
typedef char PlayerSpawnShotsShtAt30338[
    (offsetof(PlayerSpawnShotsView, primaryShtFile30338) == 0x30338) ? 1 : -1];

typedef char PlayerSpawnShotsAnmAtBC[(offsetof(PlayerSpawnShotsView, anmFileBC) == 0xBC) ? 1 : -1];
