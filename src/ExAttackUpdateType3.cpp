// SPDX-License-Identifier: GPL-3.0-only
// Adapted from Cirno-Labs/th09 src/pl02.cpp UpdateExAttackType3,
// commit d7653152becabb8c86f2f43d49015ecc11bd0652.
// Original MIT portions retain their grant in LICENSE; see THIRD_PARTY_NOTICES.md.
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
    ExAttackType3UpdateRecord *attack = reinterpret_cast<ExAttackType3UpdateRecord *>(base);
    ExAttackType3UpdateExtra *data = (ExAttackType3UpdateExtra *)attack->extra34;
    PlayerPositionView pos0;
    PlayerPositionView pos1;
    PlayerPositionView pos2;
    int i;

    switch (data->state00)
    {
    case 0:
    {
        for (i = 0x1f; i > 0; i--)
        {
            data->history10[i] = data->history10[i - 1];
        }
        data->history10[0] = attack->position20;
        attack->vms1C[1].pos2 = data->history10[6] - attack->position20;
        attack->vms1C[2].pos2 = data->history10[12] - attack->position20;
        attack->vms1C[3].pos2 = data->history10[18] - attack->position20;
        attack->vms1C[4].pos2 = data->history10[24] - attack->position20;
        PlayerPositionView size;
        float *s = size;
        s[0] = 12.0f;
        s[1] = 12.0f;
        s[2] = 0.0f;
        g_Supervisor.SelectSide(attack->opponentSide04);
        pos0.x = g_GameManager.InverseTransformPopupX(attack->position20.x);
        pos0.y = g_GameManager.InverseTransformPopupY(attack->position20.y);
        pos0.z = 0.0f;
        pos1.x = g_GameManager.InverseTransformPopupX(data->history10[12].x);
        pos1.y = g_GameManager.InverseTransformPopupY(data->history10[12].y);
        pos1.z = 0.0f;
        pos2.x = g_GameManager.InverseTransformPopupX(data->history10[24].x);
        pos2.y = g_GameManager.InverseTransformPopupY(data->history10[24].y);
        pos2.z = 0.0f;
        if (reinterpret_cast<PlayerLifecycleView *>(g_GameManager.sides[attack->opponentSide04].player)->CheckBulletCollision(&pos0, &size, NULL) == 2 ||
            reinterpret_cast<PlayerLifecycleView *>(g_GameManager.sides[attack->opponentSide04].player)->CheckBulletCollision(&pos1, &size, NULL) == 2 ||
            reinterpret_cast<PlayerLifecycleView *>(g_GameManager.sides[attack->opponentSide04].player)->CheckBulletCollision(&pos2, &size, NULL) == 2)
        {
            attack->timer10 = 0;
            data->state00 = 1;
            attack->vms1C[0].SetInterrupt(1);
            attack->vms1C[1].SetInterrupt(1);
            attack->vms1C[2].SetInterrupt(1);
            attack->vms1C[3].SetInterrupt(1);
            attack->vms1C[4].SetInterrupt(1);
            break;
        }
        if (attack->timer10 >= 30)
        {
            attack->position20 += data->motion04;
            if (data->history10[0x1f].x < -32.0f || data->history10[0x1f].x > 672.0f || data->history10[0x1f].y < -32.0f ||
                data->history10[0x1f].y > 512.0f)
            {
                return 1;
            }
        }
        break;
    }
    case 1:
        if (attack->timer10 >= 15)
        {
            return 1;
        }
        break;
    }
    return 0;
}
