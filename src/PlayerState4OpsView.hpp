#pragma once
#include <stddef.h>

struct PlayerPositionView;

// Four-byte source-facing Player prefix, not a separate native Player class.
// The exact 12-byte state setter at 0x403C20 now belongs to the true
// PlayerLifecycleView. Only the separately bound transition method ABI
// remains on this zero-extra-storage facade.
struct PlayerState4OpsView
{
    int updateState00;

    void SpawnTransitionA(
        PlayerPositionView *position, float x, float y, int kind, int zero);
    void SpawnTransitionB(
        PlayerPositionView *position, float x, float y,
        int subtype, int kind, int zero);
};
typedef char PlayerStateOpsStateAt00[
    (offsetof(PlayerState4OpsView, updateState00) == 0) ? 1 : -1];
