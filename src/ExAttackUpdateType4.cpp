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

// Type4 uses genuine 0x0C-byte trail positions, 0x2A4-byte VM records
// and 0x38-byte opponent side slots. Preserve original live reads while
// forming native Win32 DWORD addresses without a second bounded view.
#define TH09_EX4_HISTORY(extra, i) \
    (*reinterpret_cast<Float3 *>(reinterpret_cast<unsigned int>(&(extra)->history10[0]) + \
        static_cast<unsigned int>(i) * sizeof((extra)->history10[0])))
#define TH09_EX4_PREVIOUS(extra, i) \
    (*reinterpret_cast<Float3 *>(reinterpret_cast<unsigned int>(&(extra)->history10[0]) + \
        static_cast<unsigned int>(i) * sizeof((extra)->history10[0]) - sizeof((extra)->history10[0])))
#define TH09_EX4_VM(record, i) \
    (*reinterpret_cast<AnmVm *>(reinterpret_cast<unsigned int>((record)->vms1C) + \
        static_cast<unsigned int>(i) * sizeof(AnmVm)))
#define TH09_EX4_SIDE(side) \
    (*reinterpret_cast<AsciiGameManagerSideView *>(reinterpret_cast<unsigned int>(&g_GameManager.sides[0]) + \
        static_cast<unsigned int>(side) * sizeof(g_GameManager.sides[0])))

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
            TH09_EX4_HISTORY(extra, historyIndex) =
                TH09_EX4_PREVIOUS(extra, historyIndex);
        }
        while (--historyIndex != 0);

        Float3 *position = &record->position20;
        extra->history10[0] = *position;

        Float3 *history6 = &extra->history10[6];
        TH09_EX4_VM(record, 1).pos2 = *history6 - *position;

        Float3 *history12 = &extra->history10[12];
        TH09_EX4_VM(record, 2).pos2 = *history12 - *position;

        Float3 *history18 = &extra->history10[18];
        TH09_EX4_VM(record, 3).pos2 = *history18 - *position;

        Float3 *history24 = &extra->history10[24];
        TH09_EX4_VM(record, 4).pos2 = *history24 - *position;

        ExAttackType4SourceExtra *sourceExtra =
            record->parameter38->extra34;

        PlayerPositionView collisionSize;
        // Retain the exact conversion's call and write through the actual
        // three-float PlayerPositionView, not its x-float subobject.
        PlayerPositionView *collision = reinterpret_cast<PlayerPositionView *>(
            collisionSize.operator float *());
        collision->x = 12.0f;
        collision->y = 12.0f;
        collision->z = 0.0f;

        if (sourceExtra == 0 || sourceExtra->state00 != 0)
        {
            record->timer10 = 0;
            extra->state00 = 1;
            TH09_EX4_VM(record, 0).SetInterrupt(1);
            TH09_EX4_VM(record, 1).SetInterrupt(1);
            TH09_EX4_VM(record, 2).SetInterrupt(1);
            TH09_EX4_VM(record, 3).SetInterrupt(1);
            TH09_EX4_VM(record, 4).SetInterrupt(1);
        }
        else if (record->timer10 >= 30)
        {
            *position += extra->motion04;

            TH09_EX4_SIDE(record->opponentSide04)
                .player04->collisionQuery36C.AppendCircleRecord(
                    reinterpret_cast<const PlayerPositionView *>(position),
                    12.0f,
                    0);
            TH09_EX4_SIDE(record->opponentSide04)
                .player04->collisionQuery36C.AppendCircleRecord(
                    reinterpret_cast<const PlayerPositionView *>(history6),
                    12.0f,
                    0);
            TH09_EX4_SIDE(record->opponentSide04)
                .player04->collisionQuery36C.AppendCircleRecord(
                    reinterpret_cast<const PlayerPositionView *>(history12),
                    12.0f,
                    0);
            TH09_EX4_SIDE(record->opponentSide04)
                .player04->collisionQuery36C.AppendCircleRecord(
                    reinterpret_cast<const PlayerPositionView *>(history18),
                    12.0f,
                    0);
            TH09_EX4_SIDE(record->opponentSide04)
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

#undef TH09_EX4_HISTORY
#undef TH09_EX4_PREVIOUS
#undef TH09_EX4_VM
#undef TH09_EX4_SIDE
