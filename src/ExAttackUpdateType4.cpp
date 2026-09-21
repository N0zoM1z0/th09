#include "AnmManager.hpp"
#include "ExAttackController.hpp"
#include "PlayerLifecycleView.hpp"

#include <stddef.h>

struct ExAttackType4UpdateExtra
{
    int state00;
    Float3 parameterVector04;
    Float3 history10[32];
};

typedef char ExAttackType4UpdateExtraSizeIs190[
    (sizeof(ExAttackType4UpdateExtra) == 0x190) ? 1 : -1];

struct ExAttackType4SourceExtra
{
    int state00;
    Float3 motion04;
    Float3 history10[32];
};

struct ExAttackType4SourceRecord
{
    unsigned char unknown00[0x34];
    ExAttackType4SourceExtra *extra34;
};

struct ExAttackType4VmView
{
    unsigned char unknown000[0x288];
    Float3 pos2;
    unsigned char unknown294[0x2A4 - 0x294];

    void SetInterrupt(short interrupt);
};

typedef char ExAttackType4VmViewSizeIs2A4[
    (sizeof(ExAttackType4VmView) == 0x2A4) ? 1 : -1];

struct ExAttackType4UpdateRecord
{
    int unknown00;
    int opponentSide04;
    int side08;
    int active0C;
    ZunTimer timer10;
    ExAttackType4VmView *vms1C;
    Float3 position20;
    int vmCount2C;
    int unknown30;
    ExAttackType4UpdateExtra *extra34;
    ExAttackType4SourceRecord *parameter38;
};

struct ExAttackType4CollisionSizeView
{
    float x;
    float y;
    float z;

    ExAttackType4CollisionSizeView();
};

typedef char ExAttackType4CollisionSizeViewSizeIs0C[
    (sizeof(ExAttackType4CollisionSizeView) == 0x0C) ? 1 : -1];

struct ExAttackType4CircleAppendView
{
    void AppendCircleRecord(
        const PlayerPositionView *position,
        float radius,
        int unknown);
};

struct ExAttackType4PlayerView
{
    unsigned char unknown000[0x36C];
    ExAttackType4CircleAppendView collisionQuery36C;
};

struct ExAttackType4UpdateSideView
{
    unsigned char unknown00[0x04];
    ExAttackType4PlayerView *player04;
    unsigned char unknown08[0x30];
};

struct ExAttackType4UpdateGameManagerView
{
    ExAttackType4UpdateSideView sides[2];
};

extern ExAttackType4UpdateGameManagerView g_GameManager;

int __fastcall ExAttackUpdateCallbackType4(ExAttackRecord *base)
{
    ExAttackType4UpdateRecord *record =
        reinterpret_cast<ExAttackType4UpdateRecord *>(base);
    ExAttackType4UpdateExtra *extra = record->extra34;

    switch (extra->state00)
    {
    case 1:
        if (record->timer10 >= 15)
            return 1;
        break;

    case 0:
    {
        int historyIndex = 31;
        do
        {
            extra->history10[historyIndex] =
                extra->history10[historyIndex - 1];
        }
        while (--historyIndex != 0);

        Float3 *position = &record->position20;
        extra->history10[0] = *position;

        Float3 *history6 = &extra->history10[6];
        record->vms1C[1].pos2 = *history6 - *position;

        Float3 *history12 = &extra->history10[12];
        record->vms1C[2].pos2 = *history12 - *position;

        Float3 *history18 = &extra->history10[18];
        record->vms1C[3].pos2 = *history18 - *position;

        Float3 *history24 = &extra->history10[24];
        record->vms1C[4].pos2 = *history24 - *position;

        ExAttackType4SourceExtra *sourceExtra =
            record->parameter38->extra34;

        ExAttackType4CollisionSizeView collisionSize;
        collisionSize.x = 12.0f;
        collisionSize.y = 12.0f;
        collisionSize.z = 0.0f;

        if (sourceExtra == 0 || sourceExtra->state00 != 0)
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
            *position += extra->parameterVector04;

            g_GameManager.sides[record->opponentSide04]
                .player04->collisionQuery36C.AppendCircleRecord(
                    reinterpret_cast<const PlayerPositionView *>(position),
                    12.0f,
                    0);
            g_GameManager.sides[record->opponentSide04]
                .player04->collisionQuery36C.AppendCircleRecord(
                    reinterpret_cast<const PlayerPositionView *>(history6),
                    12.0f,
                    0);
            g_GameManager.sides[record->opponentSide04]
                .player04->collisionQuery36C.AppendCircleRecord(
                    reinterpret_cast<const PlayerPositionView *>(history12),
                    12.0f,
                    0);
            g_GameManager.sides[record->opponentSide04]
                .player04->collisionQuery36C.AppendCircleRecord(
                    reinterpret_cast<const PlayerPositionView *>(history18),
                    12.0f,
                    0);
            g_GameManager.sides[record->opponentSide04]
                .player04->collisionQuery36C.AppendCircleRecord(
                    reinterpret_cast<const PlayerPositionView *>(history24),
                    12.0f,
                    0);

            const Float3 *sourceTail = &sourceExtra->history10[24];
            if (sourceTail->x < -32.0f ||
                sourceTail->x > 672.0f ||
                sourceTail->y < -32.0f ||
                sourceTail->y > 512.0f)
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
