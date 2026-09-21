#include "AnmManager.hpp"
#include "AsciiManager.hpp"
#include "ExAttackController.hpp"
#include "ExAttackInterpolation.hpp"
#include "PlayerLifecycleView.hpp"
#include "RngRuntimeLeaves.hpp"

#include <stddef.h>

struct ExAttackType1422Vertex
{
    Float3 pos;
    float rhw;
    unsigned long diffuse;
};
typedef char ExAttackType1422VertexSizeIs14[
    (sizeof(ExAttackType1422Vertex) == 0x14) ? 1 : -1];

struct ExAttackType1422Extra
{
    int state00;
    Float3 motion04;
    Float3 point10;
    Float3 point1C;
    Float3 point28;
    Float3 point34;
    Float3 spawn40;
    unsigned char unknown4C[4];
    float value50;
    ExAttackType1422Vertex centerFan54[33];
    ExAttackType1422Vertex fanGroups2E8[4][33];
    float randomD38[4][33];
};
typedef char ExAttackType1422ExtraSizeIsF48[
    (sizeof(ExAttackType1422Extra) == 0xF48) ? 1 : -1];
typedef char ExAttackType1422CenterAt54[
    (offsetof(ExAttackType1422Extra, centerFan54) == 0x54) ? 1 : -1];
typedef char ExAttackType1422GroupsAt2E8[
    (offsetof(ExAttackType1422Extra, fanGroups2E8) == 0x2E8) ? 1 : -1];
typedef char ExAttackType1422RandomAtD38[
    (offsetof(ExAttackType1422Extra, randomD38) == 0xD38) ? 1 : -1];

struct ExAttackType1422Record
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
    ExAttackType1422Extra *extra34;
};

struct ExAttackType1422VmView
{
    unsigned char unknown000[0x1F0];
    int field1F0;
    unsigned char unknown1F4[4];
    unsigned int flags1F8;
    unsigned char unknown1FC[0x220 - 0x1FC];
    int field220;
};

struct ExAttackType1422CircleAppendView
{
    PlayerLifecycleView *player00;
    PlayerCollisionQueryRecordView records[128];
    int count1804;

    void AppendCircleRecord(
        const PlayerPositionView *position,
        float radius,
        int unknown);
};
typedef char ExAttackType1422CircleAppendSizeIs1808[
    (sizeof(ExAttackType1422CircleAppendView) == 0x1808) ? 1 : -1];

struct ExAttackType1422PlayerView
{
    unsigned char unknown0000[0xBC];
    AnmLoaded *anmFileBC;
    unsigned char unknown00C0[0x36C - 0xC0];
    ExAttackType1422CircleAppendView collisionQuery36C;
    unsigned char unknown1B74[0x1B88 - 0x1B74];
    PlayerPositionView position1B88;

    int CheckBulletCollision(
        PlayerPositionView *position,
        PlayerPositionView *collisionSize,
        Bullet *bullet);
};

struct ExAttackType1422SideView
{
    unsigned char unknown00[4];
    ExAttackType1422PlayerView *player04;
    unsigned char unknown08[0x30];
};

struct ExAttackType1422GameManagerView
{
    ExAttackType1422SideView sides[2];
};

struct ExAttackType1422SoundPlayerView
{
    void PlaySoundByIdx(int soundIndex, int pan);
};

extern ExAttackType1422GameManagerView g_GameManager;
extern ExAttackType1422SoundPlayerView g_SoundPlayer;
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
                .player04->anmFileBC->ExecuteAnmIdx(
                    reinterpret_cast<AnmVm *>(record->dynamicData1C),
                    9);
            ++extra->state00;
            record->position20 = extra->spawn40;
            record->unknown00 = record->opponentSide04;
            record->timer10 = 0;
        }
        return 0;

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

        if (delta.x * delta.x + delta.y * delta.y >= 9216.0f)
            return 0;

        g_SoundPlayer.PlaySoundByIdx(
            16,
            record->opponentSide04 != 0 ? 500 : -500);

        ExAttackType1422VmView *vm =
            reinterpret_cast<ExAttackType1422VmView *>(
                record->dynamicData1C);
        vm->flags1F8 &= ~1u;
        vm->field220 = 0;
        vm->field1F0 = -1;

        ++extra->state00;
        record->timer10 = 0;
        return 0;
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
        }

        if (record->timer10 >= 30)
        {
            ++extra->state00;
            record->timer10 = 0;
        }
        return 0;
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
        return 0;
    }

    default:
        return 0;
    }
}
