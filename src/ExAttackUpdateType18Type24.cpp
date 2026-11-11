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
// The older 101-byte angle helper's receiver/operand projections share
// the same X/Y coordinates as the genuine Player and 12-byte Float3.
typedef char ExAttackType1824NativeAngleStorage[
    (offsetof(PlayerLifecycleView, position1B88) ==
         offsetof(PlayerAngleView, position1B88) &&
     sizeof(PlayerAngleFloat3View) == sizeof(PlayerPositionView) &&
     offsetof(PlayerAngleFloat3View, x) == offsetof(PlayerPositionView, x) &&
     offsetof(PlayerAngleFloat3View, y) == offsetof(PlayerPositionView, y)) ? 1 : -1];

// The four record VMs are the already recovered 0x2A4-byte ANM allocation;
// rotation calls use its real original-exact PlayerAnmVmView member.
typedef char ExAttackType1824NativeVmLayout[
    (sizeof(PlayerAnmVmView) == 0x2A4 &&
     sizeof(AnmVm) == sizeof(PlayerAnmVmView) &&
     offsetof(AnmVm, pos2) == 0x288) ? 1 : -1];

extern AsciiGameManagerView g_GameManager;
float __stdcall AddNormalizeAngle(float angle, float delta);

// Both ExAttack sides are the actual GameManager 0x38-byte side records.
// Native i386 uses the signed side DWORD scaled modulo 2^32, without a gate.
static __forceinline AsciiGameManagerSideView &NativeExAttackSide(int index)
{
    return *reinterpret_cast<AsciiGameManagerSideView *>(
        reinterpret_cast<unsigned int>(&g_GameManager.sides[0]) +
        static_cast<unsigned int>(index) * sizeof(g_GameManager.sides[0]));
}



// The live attack allocation owns 0x0C-byte trail positions, 0x04-byte
// trail angles and 0x2A4-byte ANM VMs. Retain native i386 DWORD address
// words instead of relying on bounded C++ indexing, with no new guards.
#define TH09_EX_TRAIL_POS(owner, idx) \
    (*reinterpret_cast<Float3 *>( \
        reinterpret_cast<unsigned int>(&(owner)->trailPositions58[0]) + \
        static_cast<unsigned int>(idx) * sizeof((owner)->trailPositions58[0])))
#define TH09_EX_TRAIL_PREV(owner, idx) \
    (*reinterpret_cast<Float3 *>( \
        reinterpret_cast<unsigned int>(&(owner)->trailPositions58[0]) + \
        static_cast<unsigned int>(idx) * sizeof((owner)->trailPositions58[0]) - \
        sizeof((owner)->trailPositions58[0])))
#define TH09_EX_TRAIL_ANGLE(owner, idx) \
    (*reinterpret_cast<float *>( \
        reinterpret_cast<unsigned int>(&(owner)->trailAngles118[0]) + \
        static_cast<unsigned int>(idx) * sizeof((owner)->trailAngles118[0])))
#define TH09_EX_NATIVE_VM(record, idx) \
    (*reinterpret_cast<PlayerAnmVmView *>( \
        reinterpret_cast<unsigned int>((record)->dynamicData1C) + \
        static_cast<unsigned int>(idx) * sizeof(PlayerAnmVmView)))

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
            NativeExAttackSide(record->side08).player04->header24.anmFile98->ExecuteAnmIdx(
                reinterpret_cast<AnmVm *>(
                    &TH09_EX_NATIVE_VM(record, 0)), 9);
            NativeExAttackSide(record->side08).player04->header24.anmFile98->ExecuteAnmIdx(
                reinterpret_cast<AnmVm *>(
                    &TH09_EX_NATIVE_VM(record, 1)), 10);
            NativeExAttackSide(record->side08).player04->header24.anmFile98->ExecuteAnmIdx(
                reinterpret_cast<AnmVm *>(
                    &TH09_EX_NATIVE_VM(record, 2)), 10);
            NativeExAttackSide(record->side08).player04->header24.anmFile98->ExecuteAnmIdx(
                reinterpret_cast<AnmVm *>(
                    &TH09_EX_NATIVE_VM(record, 3)), 10);

            extra->state00 = static_cast<int>(
                static_cast<unsigned int>(extra->state00) + 1u);
            record->position20 = extra->spawn40;
            record->unknown00 = record->opponentSide04;
            record->timer10 = 0;

            extra->angle4C =
                reinterpret_cast<PlayerAngleView *>(
                    NativeExAttackSide(record->opponentSide04).player04)
                    ->AngleToPoint(
                        reinterpret_cast<PlayerAngleFloat3View *>(&record->position20));
            extra->speed50 = 0.2f;
            extra->reflected54 = 0;

            for (int i = 0; i < 16; ++i)
            {
                TH09_EX_TRAIL_POS(extra, i) = record->position20;
                TH09_EX_TRAIL_ANGLE(extra, i) = record->rotation3C;
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
        TH09_EX_TRAIL_POS(extra, i) = TH09_EX_TRAIL_PREV(extra, i);
        TH09_EX_TRAIL_ANGLE(extra, i) = TH09_EX_TRAIL_ANGLE(
            extra, static_cast<int>(static_cast<unsigned int>(i) - 1u));
    }

    extra->trailPositions58[0] = record->position20;
    extra->trailAngles118[0] = extra->angle4C;

    TH09_EX_NATIVE_VM(record, 0).SetZRotation(extra->angle4C);

    // One sequenced publication keeps all three returned trail values alive
    // through the last VM rotation; each value is consumed by its pos2 copy.
    TH09_EX_NATIVE_VM(record, 1).pos2 =
        extra->trailPositions58[1] - record->position20,
    TH09_EX_NATIVE_VM(record, 1).SetZRotation(extra->trailAngles118[1]),

    TH09_EX_NATIVE_VM(record, 2).pos2 =
        extra->trailPositions58[3] - record->position20,
    TH09_EX_NATIVE_VM(record, 2).SetZRotation(extra->trailAngles118[3]),

    TH09_EX_NATIVE_VM(record, 3).pos2 =
        extra->trailPositions58[5] - record->position20,
    TH09_EX_NATIVE_VM(record, 3).SetZRotation(extra->trailAngles118[5]);

    if (record->timer10 > 20)
    {
        SetPolar2D(reinterpret_cast<Float2 *>(&(extra->motion04)),
            extra->angle4C, extra->speed50);
        extra->speed50 += 0.1f;

        if (NativeExAttackSide(record->opponentSide04)
                .player04->CheckBulletCollision(
                    reinterpret_cast<PlayerPositionView *>(&record->position20),
                    reinterpret_cast<PlayerPositionView *>(&collisionSize),
                    0) == 2)
        {
            extra->state00 = 2;
            record->timer10 = 0;
            TH09_EX_NATIVE_VM(record, 0).SetInterrupt(1);
            TH09_EX_NATIVE_VM(record, 1).SetInterrupt(1);
            TH09_EX_NATIVE_VM(record, 2).SetInterrupt(1);
            TH09_EX_NATIVE_VM(record, 3).SetInterrupt(1);
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
        NativeExAttackSide(record->opponentSide04)
            .player04->collisionQuery36C.AppendCircleRecord(
                reinterpret_cast<PlayerPositionView *>(&collisionPoint),
                12.0f,
                0);

        if (record->position20.y >= 480.0f)
            return 1;
    }

    return 0;
}

#undef TH09_EX_TRAIL_POS
#undef TH09_EX_TRAIL_PREV
#undef TH09_EX_TRAIL_ANGLE
#undef TH09_EX_NATIVE_VM
