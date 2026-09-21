#include "AnmManager.hpp"
#include "AsciiManager.hpp"
#include "ExAttackController.hpp"
#include "ExAttackInterpolation.hpp"
#include "PlayerLifecycleView.hpp"

#include <stddef.h>

struct ExAttackType1824UpdateExtra
{
    int state00;
    Float3 motion04;
    Float3 point10;
    Float3 point1C;
    Float3 control28;
    Float3 control34;
    Float3 spawn40;
    float angle4C;
    float speed50;
    int reflected54;
    Float3 trailPositions58[16];
    float trailAngles118[16];
};
typedef char ExAttackType1824UpdateExtraSizeIs158[
    (sizeof(ExAttackType1824UpdateExtra) == 0x158) ? 1 : -1];
typedef char ExAttackType1824TrailPositionsAt58[
    (offsetof(ExAttackType1824UpdateExtra, trailPositions58) == 0x58) ? 1 : -1];
typedef char ExAttackType1824TrailAnglesAt118[
    (offsetof(ExAttackType1824UpdateExtra, trailAngles118) == 0x118) ? 1 : -1];

struct ExAttackType1824RecordView
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
    ExAttackType1824UpdateExtra *extra34;
    int parameter38;
    float rotation3C;
};

struct ExAttackType1824CircleAppendView
{
    PlayerLifecycleView *player00;
    PlayerCollisionQueryRecordView records[128];
    int count1804;

    void AppendCircleRecord(
        const PlayerPositionView *position,
        float radius,
        int unknown);
};
typedef char ExAttackType1824CircleAppendSizeIs1808[
    (sizeof(ExAttackType1824CircleAppendView) == 0x1808) ? 1 : -1];

struct ExAttackType1824PlayerView
{
    unsigned char unknown0000[0xBC];
    AnmLoaded *anmFileBC;
    unsigned char unknown00C0[0x36C - 0xC0];
    ExAttackType1824CircleAppendView collisionQuery36C;
    unsigned char unknown1B74[0x1B88 - 0x1B74];
    PlayerPositionView position1B88;
    unsigned char unknown1B94[0x1CE4 - 0x1B94];
    float multiplier1CE4;
    float multiplier1CE8;

    int CheckBulletCollision(
        PlayerPositionView *position,
        PlayerPositionView *collisionSize,
        Bullet *bullet);
    float AngleToPoint(PlayerPositionView *position);
};

struct ExAttackType1824SideView
{
    unsigned char unknown00[4];
    ExAttackType1824PlayerView *player04;
    unsigned char unknown08[0x30];
};

struct ExAttackType1824GameManagerView
{
    ExAttackType1824SideView sides[2];
};

struct ExAttackType1824VmView
{
    unsigned char unknown000[0x288];
    Float3 pos2;
    unsigned char unknown294[0x10];

    void SetZRotation(float angle);
    void SetInterrupt(short interrupt);
};
typedef char ExAttackType1824VmSizeIs2A4[
    (sizeof(ExAttackType1824VmView) == 0x2A4) ? 1 : -1];

extern ExAttackType1824GameManagerView g_GameManager;
float __stdcall AddNormalizeAngle(float angle, float delta);

int __fastcall ExAttackUpdateCallbackType18_24(ExAttackRecord *base)
{
    ExAttackType1824RecordView *record =
        reinterpret_cast<ExAttackType1824RecordView *>(base);
    ExAttackType1824UpdateExtra *extra = record->extra34;
    Float3 collisionSize(24.0f, 24.0f, 0.0f);

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
            g_GameManager.sides[record->side08].player04->anmFileBC->ExecuteAnmIdx(
                reinterpret_cast<AnmVm *>(
                    &reinterpret_cast<ExAttackType1824VmView *>(record->dynamicData1C)[0]), 9);
            g_GameManager.sides[record->side08].player04->anmFileBC->ExecuteAnmIdx(
                reinterpret_cast<AnmVm *>(
                    &reinterpret_cast<ExAttackType1824VmView *>(record->dynamicData1C)[1]), 10);
            g_GameManager.sides[record->side08].player04->anmFileBC->ExecuteAnmIdx(
                reinterpret_cast<AnmVm *>(
                    &reinterpret_cast<ExAttackType1824VmView *>(record->dynamicData1C)[2]), 10);
            g_GameManager.sides[record->side08].player04->anmFileBC->ExecuteAnmIdx(
                reinterpret_cast<AnmVm *>(
                    &reinterpret_cast<ExAttackType1824VmView *>(record->dynamicData1C)[3]), 10);

            ++extra->state00;
            record->position20 = extra->spawn40;
            record->unknown00 = record->opponentSide04;
            record->timer10 = 0;

            extra->angle4C =
                g_GameManager.sides[record->opponentSide04]
                    .player04->AngleToPoint(
                        reinterpret_cast<PlayerPositionView *>(&record->position20));
            extra->speed50 = 0.2f;
            extra->reflected54 = 0;

            for (int i = 0; i < 16; ++i)
            {
                extra->trailPositions58[i] = record->position20;
                extra->trailAngles118[i] = record->rotation3C;
            }
        }
        return 0;

    case 2:
        if (record->timer10 > 20)
            return 1;
        break;

    case 1:
        break;

    default:
        return 0;
    }

    for (int i = 15; i > 0; --i)
    {
        extra->trailPositions58[i] = extra->trailPositions58[i - 1];
        extra->trailAngles118[i] = extra->trailAngles118[i - 1];
    }

    extra->trailPositions58[0] = record->position20;
    float currentAngle = extra->angle4C;
    extra->trailAngles118[0] = currentAngle;

    reinterpret_cast<ExAttackType1824VmView *>(record->dynamicData1C)[0].SetZRotation(currentAngle);

    Float3 trailDelta1 =
        extra->trailPositions58[1] - record->position20;
    reinterpret_cast<ExAttackType1824VmView *>(record->dynamicData1C)[1].pos2 = trailDelta1;
    reinterpret_cast<ExAttackType1824VmView *>(record->dynamicData1C)[1].SetZRotation(extra->trailAngles118[1]);

    Float3 trailDelta3 =
        extra->trailPositions58[3] - record->position20;
    reinterpret_cast<ExAttackType1824VmView *>(record->dynamicData1C)[2].pos2 = trailDelta3;
    reinterpret_cast<ExAttackType1824VmView *>(record->dynamicData1C)[2].SetZRotation(extra->trailAngles118[3]);

    Float3 trailDelta5 =
        extra->trailPositions58[5] - record->position20;
    reinterpret_cast<ExAttackType1824VmView *>(record->dynamicData1C)[3].pos2 = trailDelta5;
    reinterpret_cast<ExAttackType1824VmView *>(record->dynamicData1C)[3].SetZRotation(extra->trailAngles118[5]);

    if (record->timer10 > 20)
    {
        extra->motion04.FromAngleMagnitude(
            extra->angle4C, extra->speed50);
        extra->speed50 += 0.1f;

        if (g_GameManager.sides[record->opponentSide04]
                .player04->CheckBulletCollision(
                    reinterpret_cast<PlayerPositionView *>(&record->position20),
                    reinterpret_cast<PlayerPositionView *>(&collisionSize),
                    0) == 2)
        {
            extra->state00 = 2;
            record->timer10 = 0;
            reinterpret_cast<ExAttackType1824VmView *>(record->dynamicData1C)[0].SetInterrupt(1);
            reinterpret_cast<ExAttackType1824VmView *>(record->dynamicData1C)[1].SetInterrupt(1);
            reinterpret_cast<ExAttackType1824VmView *>(record->dynamicData1C)[2].SetInterrupt(1);
            reinterpret_cast<ExAttackType1824VmView *>(record->dynamicData1C)[3].SetInterrupt(1);
            return 0;
        }

        record->position20 += extra->motion04;

        if (extra->reflected54 == 0)
        {
            if (record->position20.x < -144.0f)
            {
                extra->motion04.x = -extra->motion04.x;
                extra->angle4C =
                    AddNormalizeAngle(
                        3.1415927f - extra->angle4C, 0.0f);
                extra->reflected54 = 1;
            }

            if (record->position20.x > 144.0f)
            {
                extra->motion04.x = -extra->motion04.x;
                extra->angle4C =
                    AddNormalizeAngle(
                        3.1415927f - extra->angle4C, 0.0f);
                extra->reflected54 = 1;
            }
        }

        float collisionPointStorage[3];
        Float3 *collisionPoint =
            reinterpret_cast<Float3 *>(collisionPointStorage);
        collisionPoint->FromAngleMagnitude(
            extra->angle4C, extra->speed50);
        *collisionPoint += record->position20;
        g_GameManager.sides[record->opponentSide04]
            .player04->collisionQuery36C.AppendCircleRecord(
                reinterpret_cast<PlayerPositionView *>(collisionPoint),
                12.0f,
                0);

        if (record->position20.y >= 480.0f)
            return 1;
    }

    return 0;
}
