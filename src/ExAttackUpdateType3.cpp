#include "AnmManager.hpp"
#include "ExAttackController.hpp"
#include "PlayerLifecycleView.hpp"
#include "Supervisor.hpp"

#include <stddef.h>

struct ExAttackType3UpdateExtra
{
    int state00;
    Float3 motion04;
    Float3 history10[32];
};

typedef char ExAttackType3UpdateExtraSizeIs190[
    (sizeof(ExAttackType3UpdateExtra) == 0x190) ? 1 : -1];
typedef char ExAttackType3UpdateHistoryAt10[
    (offsetof(ExAttackType3UpdateExtra, history10) == 0x10) ? 1 : -1];

struct ExAttackType3VmView
{
    unsigned char unknown000[0x288];
    Float3 pos2;
    unsigned char unknown294[0x2A4 - 0x294];

    void SetInterrupt(short interrupt);
};

typedef char ExAttackType3VmViewSizeIs2A4[
    (sizeof(ExAttackType3VmView) == 0x2A4) ? 1 : -1];

struct ExAttackType3UpdateRecord
{
    int unknown00;
    int opponentSide04;
    int side08;
    int active0C;
    ZunTimer timer10;
    ExAttackType3VmView *vms1C;
    Float3 position20;
    int vmCount2C;
    int unknown30;
    ExAttackType3UpdateExtra *extra34;
};

struct ExAttackType3CollisionSizeView
{
    float x;
    float y;
    float z;

    ExAttackType3CollisionSizeView();
};

typedef char ExAttackType3CollisionSizeViewSizeIs0C[
    (sizeof(ExAttackType3CollisionSizeView) == 0x0C) ? 1 : -1];

struct ExAttackType3UpdateSideView
{
    unsigned char unknown00[0x04];
    PlayerLifecycleView *player04;
    unsigned char unknown08[0x30];
};

struct ExAttackType3UpdateGameManagerView
{
    ExAttackType3UpdateSideView sides[2];

    float InverseTransformPopupX(float value);
    float InverseTransformPopupY(float value);
};

extern ExAttackType3UpdateGameManagerView g_GameManager;

int __fastcall ExAttackUpdateCallbackType3(ExAttackRecord *base)
{
    ExAttackType3UpdateRecord *record =
        reinterpret_cast<ExAttackType3UpdateRecord *>(base);
    ExAttackType3UpdateExtra *extra = record->extra34;

    switch (extra->state00)
    {
    case 1:
        if (record->timer10 >= 15)
            return 1;
        break;

    case 0:
    {
    Float3 *history = &extra->history10[31];
    int remaining = 31;
    do
    {
        *history = history[-1];
        --history;
    }
    while (--remaining != 0);

    Float3 *position = &record->position20;
    extra->history10[0] = *position;

    record->vms1C[1].pos2 =
        extra->history10[6] - *position;
    record->vms1C[2].pos2 =
        extra->history10[12] - *position;
    record->vms1C[3].pos2 =
        extra->history10[18] - *position;
    record->vms1C[4].pos2 =
        extra->history10[24] - *position;

    ExAttackType3CollisionSizeView collisionSize;
    collisionSize.x = 12.0f;
    collisionSize.y = 12.0f;
    collisionSize.z = 0.0f;

    g_Supervisor.SelectSide(record->opponentSide04);

    float currentStorage[3];
    PlayerPositionView *current =
        reinterpret_cast<PlayerPositionView *>(currentStorage);
    current->x = g_GameManager.InverseTransformPopupX(position->x);
    current->y = g_GameManager.InverseTransformPopupY(position->y);
    current->z = 0.0f;

    float middleStorage[3];
    PlayerPositionView *middle =
        reinterpret_cast<PlayerPositionView *>(middleStorage);
    middle->x =
        g_GameManager.InverseTransformPopupX(extra->history10[12].x);
    middle->y =
        g_GameManager.InverseTransformPopupY(extra->history10[12].y);
    middle->z = 0.0f;

    float tailStorage[3];
    PlayerPositionView *tail =
        reinterpret_cast<PlayerPositionView *>(tailStorage);
    tail->x =
        g_GameManager.InverseTransformPopupX(extra->history10[24].x);
    tail->y =
        g_GameManager.InverseTransformPopupY(extra->history10[24].y);
    tail->z = 0.0f;

    PlayerLifecycleView *player =
        g_GameManager.sides[record->opponentSide04].player04;

    if (player->CheckBulletCollision(
            current,
            reinterpret_cast<PlayerPositionView *>(&collisionSize),
            0) == 2 ||
        g_GameManager.sides[record->opponentSide04]
                .player04->CheckBulletCollision(
                    middle,
                    reinterpret_cast<PlayerPositionView *>(&collisionSize),
                    0) == 2 ||
        g_GameManager.sides[record->opponentSide04]
                .player04->CheckBulletCollision(
                    tail,
                    reinterpret_cast<PlayerPositionView *>(&collisionSize),
                    0) == 2)
    {
        record->timer10 = 0;
        extra->state00 = 1;
        record->vms1C[0].SetInterrupt(1);
        record->vms1C[1].SetInterrupt(1);
        record->vms1C[2].SetInterrupt(1);
        record->vms1C[3].SetInterrupt(1);
        record->vms1C[4].SetInterrupt(1);
    }
    else if (record->timer10 >= 30)
    {
        *position += extra->motion04;

        const Float3 *oldest = &extra->history10[31];
        if (oldest->x < -32.0f ||
            oldest->x > 672.0f ||
            oldest->y < -32.0f ||
            oldest->y > 512.0f)
        {
            return 1;
        }
    }
    break;
    }

    default:
        break;
    }

    return 0;
}
