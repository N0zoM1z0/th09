#pragma once

#include "EnemyManager.hpp"
#include <stddef.h>

struct CardAttackSpawnPosition
{
    float x;
    float y;
    float z;
};

// Original methods dispatch on the same native 0x2AC450-byte EnemyManager
// allocation; this source role retains its distinct original C++ method
// identities and does not add any data or virtual methods.
struct EnemyManagerCardAttackSpawnView : EnemyManagerView
{
    void *SpawnEnemyRecord(
        int subroutine,
        CardAttackSpawnPosition *position,
        int life,
        char defeatTokenType,
        int score,
        int mirrorMovementX,
        int useOpposingEclManager);

    void *SpawnFromCardAttack(
        int subroutine,
        CardAttackSpawnPosition *position,
        int life,
        int defeatTokenType,
        int score,
        int mirrorMovementX);
};

typedef char EnemyCardAttackSpawnNativeReceiver[
    (sizeof(EnemyManagerCardAttackSpawnView) == sizeof(EnemyManagerView) &&
     offsetof(EnemyManagerCardAttackSpawnView, spawnTemplate328) == 0x328 &&
     offsetof(EnemyManagerCardAttackSpawnView, enemies5758) == 0x5758 &&
     offsetof(EnemyManagerCardAttackSpawnView, spawnFailure2AC42C) == 0x2AC42C)
        ? 1 : -1];
