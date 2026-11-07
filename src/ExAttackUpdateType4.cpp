#include "AnmManager.hpp"
#include "ExAttackController.hpp"
#include "ExAttackHistory32ExtraView.hpp"
#include "PlayerLifecycleView.hpp"
#include "GameManagerRuntimeView.hpp"

#include <stddef.h>

struct ExAttackType4SourceRecord
{
    unsigned char unknown00[0x34];
    ExAttackType4SourceExtra *extra34;
};

// Both the +0x288 position and the actual interrupt method belong to
// the already reconstructed native 0x2A4-byte AnmVm, not a local VM shadow.
typedef char ExAttackType4NativeVmLayout[
    (sizeof(AnmVm) == 0x2A4 && offsetof(AnmVm, pos2) == 0x288) ? 1 : -1];

struct ExAttackType4UpdateRecord
{
    int unknown00;
    int opponentSide04;
    int side08;
    int active0C;
    ZunTimer timer10;
    AnmVm *vms1C;
    Float3 position20;
    int vmCount2C;
    int unknown30;
    ExAttackType4UpdateExtra *extra34;
    ExAttackType4SourceRecord *parameter38;
};



// Player's real collision query is embedded at +0x36C; the query's own
// exact append method is shared with the other ExAttack callbacks.
typedef char ExAttackType4NativePlayerQueryLayout[
    (offsetof(PlayerLifecycleView, collisionQuery36C) == 0x36C &&
     sizeof(PlayerCollisionQueryStateView) == 0x1808) ? 1 : -1];

extern AsciiGameManagerView g_GameManager;

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

        PlayerPositionView collisionSize;
        float *collision = collisionSize.operator float *();
        collision[0] = 12.0f;
        collision[1] = 12.0f;
        collision[2] = 0.0f;

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
            *position += extra->motion04;

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
