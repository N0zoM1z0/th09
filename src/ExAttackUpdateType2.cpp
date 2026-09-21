#include "ExAttackController.hpp"
#include "ExAttackInterpolation.hpp"
#include "EnemyManager.hpp"

#include <stddef.h>

struct ExAttackType2Extra
{
    int state00;
    ExAttackVec3 motion04;
    ExAttackVec3 point10;
    ExAttackVec3 point1C;
    ExAttackVec3 point28;
    ExAttackVec3 point34;
    EnemyFloat3 spawnPosition40;
};
typedef char ExAttackType2ExtraSizeIs4C[
    (sizeof(ExAttackType2Extra) == 0x4C) ? 1 : -1];

struct ExAttackType2RecordView
{
    int unknown00;
    int opponentSide04;
    int side08;
    int active0C;
    ZunTimer timer10;
    void *dynamicData1C;
    ExAttackVec3 position20;
    int vmCount2C;
    int unknown30;
    ExAttackType2Extra *extra34;
};
typedef char ExAttackType2TimerAt10[
    (offsetof(ExAttackType2RecordView, timer10) == 0x10) ? 1 : -1];
typedef char ExAttackType2PositionAt20[
    (offsetof(ExAttackType2RecordView, position20) == 0x20) ? 1 : -1];
typedef char ExAttackType2ExtraAt34[
    (offsetof(ExAttackType2RecordView, extra34) == 0x34) ? 1 : -1];

struct ExAttackType2GameSideView
{
    unsigned char unknown00[0x10];
    EnemyManagerView *enemyManager10;
    unsigned char unknown14[0x24];
};
struct ExAttackType2GameManagerView
{
    ExAttackType2GameSideView sides[2];
};
typedef char ExAttackType2GameSideSizeIs38[
    (sizeof(ExAttackType2GameSideView) == 0x38) ? 1 : -1];

extern ExAttackType2GameManagerView g_GameManager;

int __fastcall ExAttackUpdateCallbackType2(ExAttackRecord *base)
{
    ExAttackType2RecordView *record =
        reinterpret_cast<ExAttackType2RecordView *>(base);
    ExAttackType2Extra *extra = record->extra34;

    if (extra->state00 == 0)
    {
        ExAttackInterpolate2D(
            &record->position20.x,
            &extra->point1C.x,
            &extra->point10.x,
            &extra->point34.x,
            &extra->point28.x,
            record->timer10,
            90.0f);

        if (record->timer10 > 90)
        {
            int initialVariables[30];
            initialVariables[8] = 0xBFC90FDB;
            g_GameManager.sides[record->opponentSide04].enemyManager10->
                SpawnEnemyUsingOpposingEcl(
                    8,
                    &extra->spawnPosition40,
                    20,
                    -2,
                    1000,
                    initialVariables);
            return 1;
        }
    }
    return 0;
}
