#include "RuntimePolarMath.hpp"
#include "AnmManager.hpp"
#include "AsciiManager.hpp"
#include "ExAttackController.hpp"
#include "ExAttackInterpolation.hpp"
#include "PlayerLifecycleView.hpp"
#include "GameManagerRuntimeView.hpp"
#include "PlayerAngleView.hpp"

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



// The original +0xBC ANM resource and +0x36C collision query are members
// of the same native Player allocation as the other ExAttack receivers.
typedef char ExAttackType1824NativePlayerFields[
    (offsetof(PlayerLifecycleView, collisionQuery36C) == 0x36C &&
     offsetof(PlayerLifecycleView, header24) +
         offsetof(PlayerHeaderStateView, anmFile98) == 0xBC) ? 1 : -1];

// The four record VMs are the already recovered 0x2A4-byte ANM allocation;
// rotation calls use its real original-exact PlayerAnmVmView member.
typedef char ExAttackType1824NativeVmLayout[
    (sizeof(PlayerAnmVmView) == 0x2A4 &&
     sizeof(AnmVm) == sizeof(PlayerAnmVmView) &&
     offsetof(AnmVm, pos2) == 0x288) ? 1 : -1];

extern AsciiGameManagerView g_GameManager;
float __stdcall AddNormalizeAngle(float angle, float delta);

int __fastcall ExAttackUpdateCallbackType18_24(ExAttackRecord *base)
{
    ExAttackType1824RecordView *record =
        reinterpret_cast<ExAttackType1824RecordView *>(base);
    ExAttackType1824UpdateExtra *extra = record->extra34;
    Float3 collisionSize(24.0f, 24.0f, 0.0f);
    Float3 collisionPoint;

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
            g_GameManager.sides[record->side08].player04->header24.anmFile98->ExecuteAnmIdx(
                reinterpret_cast<AnmVm *>(
                    &reinterpret_cast<PlayerAnmVmView *>(record->dynamicData1C)[0]), 9);
            g_GameManager.sides[record->side08].player04->header24.anmFile98->ExecuteAnmIdx(
                reinterpret_cast<AnmVm *>(
                    &reinterpret_cast<PlayerAnmVmView *>(record->dynamicData1C)[1]), 10);
            g_GameManager.sides[record->side08].player04->header24.anmFile98->ExecuteAnmIdx(
                reinterpret_cast<AnmVm *>(
                    &reinterpret_cast<PlayerAnmVmView *>(record->dynamicData1C)[2]), 10);
            g_GameManager.sides[record->side08].player04->header24.anmFile98->ExecuteAnmIdx(
                reinterpret_cast<AnmVm *>(
                    &reinterpret_cast<PlayerAnmVmView *>(record->dynamicData1C)[3]), 10);

            ++extra->state00;
            record->position20 = extra->spawn40;
            record->unknown00 = record->opponentSide04;
            record->timer10 = 0;

            extra->angle4C =
                reinterpret_cast<PlayerAngleView *>(
                    g_GameManager.sides[record->opponentSide04].player04)
                    ->AngleToPoint(
                        reinterpret_cast<PlayerAngleFloat3View *>(&record->position20));
            extra->speed50 = 0.2f;
            extra->reflected54 = 0;

            for (int i = 0; i < 16; ++i)
            {
                extra->trailPositions58[i] = record->position20;
                extra->trailAngles118[i] = record->rotation3C;
            }
        }
        return 0;

    case 1:
        break;

    case 2:
        if (record->timer10 > 20)
            return 1;

    default:
        return 0;
    }

    for (int i = 15; i > 0; --i)
    {
        extra->trailPositions58[i] = extra->trailPositions58[i - 1];
        extra->trailAngles118[i] = extra->trailAngles118[i - 1];
    }

    extra->trailPositions58[0] = record->position20;
    extra->trailAngles118[0] = extra->angle4C;

    reinterpret_cast<PlayerAnmVmView *>(record->dynamicData1C)[0].SetZRotation(extra->angle4C);

    // One sequenced publication keeps all three returned trail values alive
    // through the last VM rotation; each value is consumed by its pos2 copy.
    reinterpret_cast<PlayerAnmVmView *>(record->dynamicData1C)[1].pos2 =
        extra->trailPositions58[1] - record->position20,
    reinterpret_cast<PlayerAnmVmView *>(record->dynamicData1C)[1].SetZRotation(extra->trailAngles118[1]),

    reinterpret_cast<PlayerAnmVmView *>(record->dynamicData1C)[2].pos2 =
        extra->trailPositions58[3] - record->position20,
    reinterpret_cast<PlayerAnmVmView *>(record->dynamicData1C)[2].SetZRotation(extra->trailAngles118[3]),

    reinterpret_cast<PlayerAnmVmView *>(record->dynamicData1C)[3].pos2 =
        extra->trailPositions58[5] - record->position20,
    reinterpret_cast<PlayerAnmVmView *>(record->dynamicData1C)[3].SetZRotation(extra->trailAngles118[5]);

    if (record->timer10 > 20)
    {
        SetPolar2D(reinterpret_cast<Float2 *>(&(extra->motion04)),
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
            reinterpret_cast<PlayerAnmVmView *>(record->dynamicData1C)[0].SetInterrupt(1);
            reinterpret_cast<PlayerAnmVmView *>(record->dynamicData1C)[1].SetInterrupt(1);
            reinterpret_cast<PlayerAnmVmView *>(record->dynamicData1C)[2].SetInterrupt(1);
            reinterpret_cast<PlayerAnmVmView *>(record->dynamicData1C)[3].SetInterrupt(1);
            return 0;
        }

        record->position20 += extra->motion04;

        if (extra->reflected54 == 0)
        {
            if (record->position20.x < -144.0f)
            {
                extra->motion04.x = -extra->motion04.x;
                extra->angle4C = 3.1415927f - extra->angle4C;
                extra->angle4C = AddNormalizeAngle(extra->angle4C, 0.0f);
                extra->reflected54 = 1;
            }

            else if (record->position20.x > 144.0f)
            {
                extra->motion04.x = -extra->motion04.x;
                extra->angle4C = 3.1415927f - extra->angle4C;
                extra->angle4C = AddNormalizeAngle(extra->angle4C, 0.0f);
                extra->reflected54 = 1;
            }
        }

        SetPolar2D(reinterpret_cast<Float2 *>(&(collisionPoint)),
            extra->angle4C, extra->speed50);
        collisionPoint += record->position20;
        g_GameManager.sides[record->opponentSide04]
            .player04->collisionQuery36C.AppendCircleRecord(
                reinterpret_cast<PlayerPositionView *>(&collisionPoint),
                12.0f,
                0);

        if (record->position20.y >= 480.0f)
            return 1;
    }

    return 0;
}
