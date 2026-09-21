#pragma once

struct CardAttackSpawnPosition
{
    float x;
    float y;
    float z;
};

struct EnemyManagerCardAttackSpawnView
{
    void *SpawnEnemyRecord(
        short subroutine,
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
