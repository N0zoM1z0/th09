#include "AnmManager.hpp"
#include "SoundPlayer.hpp"
#include "AsciiManager.hpp"
#include "ExAttackController.hpp"
#include "ExAttackType1422ExtraView.hpp"
#include "ExAttackInterpolation.hpp"
#include "PlayerLifecycleView.hpp"
#include "GameManagerRuntimeView.hpp"
#include "RngRuntimeLeaves.hpp"

#include <stddef.h>


struct ExAttackType1422VmView
{
    unsigned char unknown000[0x1F0];
    int field1F0;
    unsigned char unknown1F4[4];
    unsigned int flags1F8;
    unsigned char unknown1FC[0x220 - 0x1FC];
    int field220;
};



extern AsciiGameManagerView g_GameManager;
extern RngRuntimeView g_ReplayRng;

int __fastcall ExAttackUpdateCallbackType14_22(ExAttackRecord *base)
{
    ExAttackType1422Record *record =
        reinterpret_cast<ExAttackType1422Record *>(base);
    ExAttackType1422Extra *extra = record->extra34;
    Float3 collisionSize(24.0f, 24.0f, 0.0f);

    switch (extra->state00)
    {
    case 0:
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
            g_GameManager.sides[record->side08]
                .player04->header24.anmFile98->ExecuteAnmIdx(
                    reinterpret_cast<AnmVm *>(record->dynamicData1C),
                    9);
            ++extra->state00;
            record->position20 = extra->spawn40;
            record->unknown00 = record->opponentSide04;
            record->timer10 = 0;
        }
        break;

    case 1:
    {
        PlayerPositionView delta;
        delta = g_GameManager.sides[record->opponentSide04]
                    .player04->position1B88 -
                *reinterpret_cast<PlayerPositionView *>(&record->position20);

        record->position20 += extra->motion04;

        if (g_GameManager.sides[record->opponentSide04]
                .player04->CheckBulletCollision(
                    reinterpret_cast<PlayerPositionView *>(&record->position20),
                    reinterpret_cast<PlayerPositionView *>(&collisionSize),
                    0) == 2 ||
            record->position20.y > 480.0f)
        {
            return 1;
        }

        if (delta.x * delta.x + delta.y * delta.y < 9216.0f)
        {
            g_SoundPlayer.PlaySoundByIdx(
                16,
                record->opponentSide04 != 0 ? 500 : -500);

            reinterpret_cast<ExAttackType1422VmView *>(
                record->dynamicData1C)->flags1F8 &= ~1u;
            reinterpret_cast<ExAttackType1422VmView *>(
                record->dynamicData1C)->field220 = 0;
            reinterpret_cast<ExAttackType1422VmView *>(
                record->dynamicData1C)->field1F0 = -1;

            ++extra->state00;
            record->timer10 = 0;
        }
        break;
    }

    case 2:
    {
        float t = record->timer10;
        float remaining = 1.0f - t * 0.020833334f;
        extra->value50 =
            (1.0f - remaining * remaining) * 64.0f;

        if (record->timer10 > 1)
        {
            if (g_GameManager.sides[record->opponentSide04]
                    .player04->CheckBulletCollision(
                        reinterpret_cast<PlayerPositionView *>(&record->position20),
                        reinterpret_cast<PlayerPositionView *>(&collisionSize),
                        0) == 2)
            {
                ++extra->state00;
                record->timer10 = 0;
            }
            else
            {
                g_GameManager.sides[record->opponentSide04]
                    .player04->collisionQuery36C.AppendCircleRecord(
                        reinterpret_cast<PlayerPositionView *>(&record->position20),
                        extra->value50 * 0.80000001f,
                        0);
            }
            if (record->timer10 >= 30)
            {
                ++extra->state00;
                record->timer10 = 0;
            }
        }
        break;
    }

    case 3:
    {
        float angle = -3.1415927f;
        for (int group = 0; group < 4; ++group)
        {
            for (int i = 0; i < 32; ++i)
            {
                if (angle >= 3.1415927f)
                    angle -= 6.2831855f;

                extra->fanGroups2E8[group][0].pos.z = 0.0f;
                extra->randomD38[group][i] =
                    g_ReplayRng.GetRandomF32SignedInRange(
                        record->timer10);
                angle += 0.2026834f;
            }

            angle += 0.050670851f;
        }

        if (record->timer10 >= 30)
            return 1;
        break;
    }

    default:
        break;
    }

    return 0;
}
