#pragma once

struct PlayerShotInitShotView;
struct PlayerShotInitDescriptorView;

struct PlayerSpawnFrameView
{
    int __fastcall SpawnShotWhenFrameMatches(
        PlayerShotInitShotView *shot,
        int value,
        PlayerShotInitDescriptorView *entry);
};
