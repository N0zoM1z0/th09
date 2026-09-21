#include "AnmManager.hpp"
#include "ExAttackController.hpp"
#include "ExAttackInterpolation.hpp"

#include <stddef.h>

struct ExAttackType19UpdateVertex
{
    Float3 pos;
    float rhw;
    unsigned long diffuse;
    float u;
    float v;
};
typedef char ExAttackType19UpdateVertexSizeIs1C[
    (sizeof(ExAttackType19UpdateVertex) == 0x1C) ? 1 : -1];

struct ExAttackType19UpdateExtra
{
    int state00;
    Float3 motion04;
    Float3 point10;
    Float3 point1C;
    Float3 control28;
    Float3 control34;
    Float3 spawn40;
    ExAttackType19UpdateVertex vertices4C[33];
    Float3 history3E8[32];
    unsigned char unknown568[0x578 - 0x568];
    float magnitudes578[32];
    float unknown5F8;
    float angularSteps5FC[32];
    float drift67C;
    float drift680;
    int drawPending684;
};

typedef char ExAttackType19UpdateExtraSizeIs688[
    (sizeof(ExAttackType19UpdateExtra) == 0x688) ? 1 : -1];
typedef char ExAttackType19UpdateVerticesAt4C[
    (offsetof(ExAttackType19UpdateExtra, vertices4C) == 0x4C) ? 1 : -1];
typedef char ExAttackType19UpdateHistoryAt3E8[
    (offsetof(ExAttackType19UpdateExtra, history3E8) == 0x3E8) ? 1 : -1];
typedef char ExAttackType19UpdateMagnitudesAt578[
    (offsetof(ExAttackType19UpdateExtra, magnitudes578) == 0x578) ? 1 : -1];
typedef char ExAttackType19UpdateStepsAt5FC[
    (offsetof(ExAttackType19UpdateExtra, angularSteps5FC) == 0x5FC) ? 1 : -1];

struct ExAttackType19UpdateRecord
{
    int unknown00;
    int opponentSide04;
    int side08;
    int active0C;
    ZunTimer timer10;
    void *dynamicData1C;
    Float3 position20;
    int vmCount2C;
    int unknown30;
    ExAttackType19UpdateExtra *extra34;
};

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

extern ExAttackType19UpdateGameManagerView g_GameManager;

int __fastcall ExAttackUpdateCallbackType19(ExAttackRecord *base)
{
    ExAttackType19UpdateRecord *record =
        reinterpret_cast<ExAttackType19UpdateRecord *>(base);
    ExAttackType19UpdateExtra *extra = record->extra34;

    switch (extra->state00)
    {
    case 0:
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

            g_GameManager.sides[record->side08]
                .player04->anmFileBC->ExecuteAnmIdx(
                    reinterpret_cast<AnmVm *>(record->dynamicData1C),
                    9);

            reinterpret_cast<ExAttackType19UpdateVmView *>(
                record->dynamicData1C)->flags1F8 &= ~2u;
        }
        break;

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
        Float3 *history = &extra->history3E8[1];
        float *magnitude = &extra->magnitudes578[0];
        int remaining = 32;

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

            *magnitude += magnitude[33];

            history->FromAngleMagnitude(angle, *magnitude);
            *history += extra->spawn40;

            ++vertex;
            ++history;
            ++magnitude;
            angle += 0.2026834f;
        }
        while (--remaining != 0);

        if (record->timer10 < 300 && record->timer10 > 20)
        {
            ExAttackType19UpdateSideView *side =
                &g_GameManager.sides[record->opponentSide04];
            Float3 delta =
                side->player04->position1B88 - record->position20;

            if (delta.x * delta.x + delta.y * delta.y <= 4096.0f)
            {
                side->player04->multiplier1CE4 *= 0.40000001f;
                g_GameManager.sides[record->opponentSide04]
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
        if (record->timer10 > 20)
            return 1;
        break;

    default:
        break;
    }

    return 0;
}
