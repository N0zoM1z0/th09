#pragma once
#include "PlayerStateEffectView.hpp"
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

// The exact SpawnShots thiscall sees the same native Player allocation.
// Its callback-specific shot and descriptor *views* remain local until their
// independent callback signatures can be reconstructed.
struct PlayerSpawnShotsView : PlayerStateEffectView
{
    void __fastcall SpawnShots(int value, int powerLevel);
};

typedef char PlayerSpawnShotsNativeStorage[
    (sizeof(PlayerSpawnShotsView) == sizeof(PlayerLifecycleView) &&
     offsetof(PlayerSpawnShotsView, shots) == 0xC11C &&
     offsetof(PlayerSpawnShotsView, primaryShtFile) == 0x30338 &&
     offsetof(PlayerSpawnShotsView, header24) +
     offsetof(PlayerHeaderStateView, anmFile98) == 0xBC) ? 1 : -1];
