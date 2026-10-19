#pragma once

// Compile with EclManager's visible ordinary Float3::FromAngleMagnitude.
// The original retains its receiver in ECX for the following vector addition.

#include "AnmManager.hpp"
#include "ExAttackController.hpp"
#include "ExAttackInterpolation.hpp"
#include "ExAttackType19ExtraView.hpp"

#include <stddef.h>

struct ExAttackType19UpdateVmView
{
    unsigned char unknown000[0x1F8];
    unsigned int flags1F8;
};

struct ExAttackType19UpdatePlayerView
{
    unsigned char unknown0000[0xBC];
    AnmLoaded *anmFileBC;
    unsigned char unknown00C0[0x1B88 - 0xC0];
    Float3 position1B88;
    unsigned char unknown1B94[0x1CE4 - 0x1B94];
    float multiplier1CE4;
    float multiplier1CE8;
};

struct ExAttackType19UpdateSideView
{
    unsigned char unknown00[0x04];
    ExAttackType19UpdatePlayerView *player04;
    unsigned char unknown08[0x30];
};

struct ExAttackType19UpdateGameManagerView
{
    ExAttackType19UpdateSideView sides[2];
};

#include "AsciiGameManagerView.hpp"



inline ExAttackType19UpdatePlayerView *& GetType19PlayerSlot(ExAttackType19UpdateRecord *owner)
{ return reinterpret_cast<ExAttackType19UpdateGameManagerView &>(g_GameManager).sides[owner->opponentSide04].player04; }
inline ExAttackType19UpdatePlayerView *GetType19PlayerValue(ExAttackType19UpdateRecord *owner)
{ return reinterpret_cast<ExAttackType19UpdateGameManagerView &>(g_GameManager).sides[owner->opponentSide04].player04; }

int __fastcall ExAttackUpdateCallbackType19(ExAttackRecord *base)
{
    ExAttackType19UpdateExtra *extra = reinterpret_cast<ExAttackType19UpdateRecord *>(base)->extra34;

    switch (extra->state00)
    {
    case 0:
    {
    ExAttackType19UpdateRecord *record = reinterpret_cast<ExAttackType19UpdateRecord *>(base);
        ExAttackInterpolate2D(
            &record->position20.x,
            &extra->point1C.x,
            &extra->point10.x,
            &extra->control34.x,
            &extra->control28.x,
            record->timer10,
            90.0f);

        if (record->timer10 > 90)
        {
            ++extra->state00;
            record->position20 = extra->spawn40;
            record->unknown00 = record->opponentSide04;
            record->timer10 = 0;

            reinterpret_cast<ExAttackType19UpdateGameManagerView &>(g_GameManager).sides[record->side08]
                .player04->anmFileBC->ExecuteAnmIdx(
                    reinterpret_cast<AnmVm *>(record->dynamicData1C),
                    9);

            reinterpret_cast<ExAttackType19UpdateVmView *>(
                record->dynamicData1C)->flags1F8 &= ~2u;
        }
        break;

    }
    case 1:
    {
        ExAttackType19UpdateVertex *vertex = &extra->vertices4C[0];
        float angle = -3.1415927f;

        vertex->u += extra->drift67C;
        if (vertex->u < 0.0f)
        {
            ExAttackType19UpdateVertex *wrap = &extra->vertices4C[0];
            int wrapRemaining = 11;
            do
            {
                wrap[0].u += 1.0f;
                wrap[1].u += 1.0f;
                wrap[2].u += 1.0f;
                wrap += 3;
            }
            while (--wrapRemaining != 0);
        }

        vertex->v += extra->drift67C;
        if (vertex->v < 0.0f)
        {
            ExAttackType19UpdateVertex *wrap = &extra->vertices4C[0];
            int wrapRemaining = 11;
            do
            {
                wrap[0].v += 1.0f;
                wrap[1].v += 1.0f;
                wrap[2].v += 1.0f;
                wrap += 3;
            }
            while (--wrapRemaining != 0);
        }

        ++vertex;
        // Magnitudes and angular steps belong to separate 32-element arrays.
        int sample = 0;

        do
        {
            vertex->u += extra->drift67C;
            if (vertex->u < 0.0f)
            {
                ExAttackType19UpdateVertex *wrap = &extra->vertices4C[0];
                int wrapRemaining = 11;
                do
                {
                    wrap[0].u += 1.0f;
                    wrap[1].u += 1.0f;
                    wrap[2].u += 1.0f;
                    wrap += 3;
                }
                while (--wrapRemaining != 0);
            }

            vertex->v += extra->drift67C;
            if (vertex->v < 0.0f)
            {
                ExAttackType19UpdateVertex *wrap = &extra->vertices4C[0];
                int wrapRemaining = 11;
                do
                {
                    wrap[0].v += 1.0f;
                    wrap[1].v += 1.0f;
                    wrap[2].v += 1.0f;
                    wrap += 3;
                }
                while (--wrapRemaining != 0);
            }

            extra->magnitudes578[sample] += extra->angularSteps5FC[sample];

            extra->history3E8[sample + 1].FromAngleMagnitude(
                angle, extra->magnitudes578[sample]);
            extra->history3E8[sample + 1] += extra->spawn40;

            ++vertex;
            ++sample;
            angle += 0.2026834f;
        }
        while (sample != 32);

    ExAttackType19UpdateRecord *record = reinterpret_cast<ExAttackType19UpdateRecord *>(base);
        if (record->timer10 < 300 && record->timer10 > 20)
        {
            ExAttackType19UpdatePlayerView *player = GetType19PlayerValue(record);
            ExAttackType19UpdatePlayerView *&slot = GetType19PlayerSlot(record);
            Float3 delta = player->position1B88 - record->position20;

            if (delta.x * delta.x + delta.y * delta.y <= 4096.0f)
            {
                slot->multiplier1CE4 *= 0.40000001f;
                reinterpret_cast<ExAttackType19UpdateGameManagerView &>(g_GameManager).sides[record->opponentSide04]
                    .player04->multiplier1CE8 *= 0.40000001f;
            }
        }

        int result = 1;
        unsigned char flags =
            reinterpret_cast<unsigned char *>(record->dynamicData1C)[0x1F8];
        if ((flags & result) == 0)
            return result;
        break;
    }

    case 2:
    {
    ExAttackType19UpdateRecord *record = reinterpret_cast<ExAttackType19UpdateRecord *>(base);
        if (record->timer10 > 20)
            return 1;
        break;

    }
    default:
        break;
    }

    return 0;
}
