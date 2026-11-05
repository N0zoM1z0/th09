#include "AnmManager.hpp"
#include "AsciiGameManagerView.hpp"
#include "ExAttackController.hpp"
#include "ExAttackType3DataView.hpp"
#include "PlayerLifecycleView.hpp"
#include "Supervisor.hpp"

#include <stddef.h>

typedef char ExAttackType3NativeVmPositionAt288[
    (offsetof(AnmVm, pos2) == 0x288) ? 1 : -1];
typedef char ExAttackType3NativeVmInterruptAt1FE[
    (offsetof(AnmVm, pendingInterrupt) == 0x1FE) ? 1 : -1];

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
    for (int i = 31; i != 0; --i)
        extra->history10[i] = extra->history10[i - 1];

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

    PlayerPositionView collisionSize;
    PlayerPositionView *collision = reinterpret_cast<PlayerPositionView *>(
        collisionSize.operator float *());
    collision->x = 12.0f;
    collision->y = 12.0f;
    collision->z = 0.0f;

    g_Supervisor.SelectSide(record->opponentSide04);

    PlayerPositionView current;
    current.x = g_GameManager.InverseTransformPopupX(position->x);
    current.y = g_GameManager.InverseTransformPopupY(record->position20.y);
    current.z = 0.0f;

    PlayerPositionView middle;
    middle.x =
        g_GameManager.InverseTransformPopupX(extra->history10[12].x);
    middle.y =
        g_GameManager.InverseTransformPopupY(extra->history10[12].y);
    // The original sets middle.z after the Y conversion. VC7.1 currently
    // delays this store past the following tail-X load; keep that divergence
    // visible to the complete-owner diagnostic instead of moving it pre-call.
    middle.z = 0.0f;

    PlayerPositionView tail;
    tail.x =
        g_GameManager.InverseTransformPopupX(extra->history10[24].x);
    tail.y =
        g_GameManager.InverseTransformPopupY(extra->history10[24].y);
    tail.z = 0.0f;

    if (reinterpret_cast<PlayerLifecycleView *>(g_GameManager.sides[record->opponentSide04].player)->CheckBulletCollision(
                    &current,
                    &collisionSize,
                    0) == 2 ||
        reinterpret_cast<PlayerLifecycleView *>(g_GameManager.sides[record->opponentSide04].player)->CheckBulletCollision(
                    &middle,
                    &collisionSize,
                    0) == 2 ||
        reinterpret_cast<PlayerLifecycleView *>(g_GameManager.sides[record->opponentSide04].player)->CheckBulletCollision(
                    &tail,
                    &collisionSize,
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

        if (extra->history10[31].x < -32.0f ||
            extra->history10[31].x > 672.0f ||
            extra->history10[31].y < -32.0f ||
            extra->history10[31].y > 512.0f)
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
