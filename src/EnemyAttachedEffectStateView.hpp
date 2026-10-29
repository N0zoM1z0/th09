#pragma once

#include <stddef.h>

struct Effect;

// Shared source projection for the Enemy update, ECL and release consumers.
// The original C++ storage declaration and nominal type remain unknown.
struct EnemyAttachedEffectStateView
{
    Effect *effects00[24];
    int count60;
    float distance64;
};

typedef char EnemyAttachedEffectStateCountAt60[
    (offsetof(EnemyAttachedEffectStateView, count60) == 0x60) ? 1 : -1];
typedef char EnemyAttachedEffectStateDistanceAt64[
    (offsetof(EnemyAttachedEffectStateView, distance64) == 0x64) ? 1 : -1];
typedef char EnemyAttachedEffectStateSizeIs68[
    (sizeof(EnemyAttachedEffectStateView) == 0x68) ? 1 : -1];
