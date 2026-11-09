#pragma once

// Target-observed Player damage projection; original class identity is unproved.
#include <stddef.h>

struct PlayerDamageFloat3
{
    float x;
    float y;
    float z;
    operator float *();
};

typedef char PlayerDamageFloat3SizeIs0C[
    (sizeof(PlayerDamageFloat3) == 0x0C) ? 1 : -1];
typedef char PlayerDamageFloat3XAt00[
    (offsetof(PlayerDamageFloat3, x) == 0x00) ? 1 : -1];
typedef char PlayerDamageFloat3ZAt08[
    (offsetof(PlayerDamageFloat3, z) == 0x08) ? 1 : -1];

// The original 0x41FCD0 API retains its observed three-float argument ABI.
// The constructed Player, Shot and collision-region objects now come from
// PlayerLifecycleView.hpp, never independently allocated here.
struct PlayerLifecycleView;
struct PlayerShotCtorView;
typedef int (__fastcall *PlayerDamageShotCollisionCallback)(
    PlayerLifecycleView *player,
    PlayerShotCtorView *shot,
    PlayerDamageFloat3 *targetPosition);

void __fastcall PlayerDamageRotate(
    PlayerDamageFloat3 *out,
    PlayerDamageFloat3 *point,
    float angle);
