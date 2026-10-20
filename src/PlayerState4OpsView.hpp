#pragma once
#include <stddef.h>

struct PlayerPositionView;

// Four-byte Player state prefix, not the complete original class.
// The setter at 0x403C20 writes this first word; methods retain their
// existing thiscall receiver and declaration-only ABI.
struct PlayerState4OpsView
{
    int updateState00;

    void AddRespawnResource(float value);
    void SetUpdateState(int state);
    void SpawnTransitionA(
        PlayerPositionView *position, float x, float y, int kind, int zero);
    void SpawnTransitionB(
        PlayerPositionView *position, float x, float y,
        int subtype, int kind, int zero);
};
typedef char PlayerStateOpsStateAt00[
    (offsetof(PlayerState4OpsView, updateState00) == 0) ? 1 : -1];
