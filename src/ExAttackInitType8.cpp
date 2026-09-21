#include "AnmManager.hpp"
#include "ExAttackController.hpp"
#include "RngRuntimeLeaves.hpp"
#include "Supervisor.hpp"
#include <math.h>

Float3 *Float3::FromAngleMagnitude(float angle, float magnitude)
{
    x = cosf(angle) * magnitude;
    y = sinf(angle) * magnitude;
    return this;
}

struct ExAttackType8Extra
{
    int state00;
    float unknown04;
    float angle08;
    float angle0C;
    float unknown10;
    Float3 motion14;
    Float3 point20;
    Float3 point2C;
    Float3 control38;
    Float3 control44;
    Float3 spawn50;
};
typedef char ExAttackType8ExtraSizeIs5C[
    (sizeof(ExAttackType8Extra) == 0x5C) ? 1 : -1];

struct ExAttackType8RecordView
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
    ExAttackType8Extra *extra34;
};

struct ExAttackType8AnmOwner
{
    unsigned char unknown000[0x2D4];
    AnmLoaded *anm2D4;
};

struct ExAttackType8PlayerView
{
    unsigned char unknown000[0x1B88];
    Float3 position1B88;
};

struct ExAttackType8SideView
{
    unsigned char unknown00[0x04];
    ExAttackType8PlayerView *player04;
    unsigned char unknown08[0x04];
    ExAttackType8AnmOwner *anmOwner0C;
    unsigned char unknown10[0x28];
};

struct ExAttackType8GameManagerView
{
    ExAttackType8SideView sides[2];

    float TransformPopupX(float value);
    float TransformPopupY(float value);
};

struct ExAttackType8SideLookupView
{
    int unknown00;
    int opponentSide04;
    int side08;
    ExAttackType8SideView *GetSideState();
};

struct ExAttackType8AllocateView
{
    unsigned char bytes[0x38];
    void AllocateDynamicData(int vmCount, int extraBytes);
};

extern ExAttackType8GameManagerView g_GameManager;
extern RngRuntimeView g_ReplayRng;

int __fastcall ExAttackInitializeCallbackType8(ExAttackRecord *base)
{
    ExAttackType8RecordView *record =
        reinterpret_cast<ExAttackType8RecordView *>(base);

    reinterpret_cast<ExAttackType8AllocateView *>(record)
        ->AllocateDynamicData(1, sizeof(ExAttackType8Extra));

    reinterpret_cast<ExAttackType8SideLookupView *>(record)
        ->GetSideState()->anmOwner0C->anm2D4->ExecuteAnmIdx(
            reinterpret_cast<AnmVm *>(record->dynamicData1C), 37);

    ExAttackType8Extra *extra = record->extra34;

    float angle = g_ReplayRng.GetRandomF32InRange(-3.1415927f);
    extra->motion14.z = 0.0f;
    extra->motion14.FromAngleMagnitude(angle, 2.0f);
    extra->angle08 = angle;
    extra->state00 = 0;

    record->unknown00 = 2;
    record->timer10 = 90;

    extra->control38.FromAngleMagnitude(
        g_ReplayRng.GetRandomF32SignedInRange(3.1415927f),
        g_ReplayRng.GetRandomF32InRange(192.0f));
    extra->control44.FromAngleMagnitude(
        g_ReplayRng.GetRandomF32SignedInRange(3.1415927f),
        g_ReplayRng.GetRandomF32InRange(192.0f));

    extra->point20.x = g_GameManager.TransformPopupX(record->position20.x);
    extra->point20.y = g_GameManager.TransformPopupY(record->position20.y);
    extra->point20.z = 0.0f;

    extra->spawn50.x = 144.0f;
    extra->angle08 = 3.1415927f;
    extra->spawn50.y = 192.0f;
    extra->spawn50.z = 0.0f;

    g_Supervisor.SelectSide(record->opponentSide04);
    extra->point2C.x = g_GameManager.TransformPopupX(extra->spawn50.x);
    extra->point2C.y = g_GameManager.TransformPopupY(extra->spawn50.y);
    extra->point2C.z = 0.0f;
    g_Supervisor.SelectSide(record->side08);

    extra->angle0C =
        g_ReplayRng.GetRandomF32SignedInRange(3.1415927f);

    return 0;
}
int __fastcall ExAttackInitializeCallbackType7(ExAttackRecord *base)
{
    ExAttackType8RecordView *record =
        reinterpret_cast<ExAttackType8RecordView *>(base);

    reinterpret_cast<ExAttackType8AllocateView *>(record)
        ->AllocateDynamicData(1, sizeof(ExAttackType8Extra));

    reinterpret_cast<ExAttackType8SideLookupView *>(record)
        ->GetSideState()->anmOwner0C->anm2D4->ExecuteAnmIdx(
            reinterpret_cast<AnmVm *>(record->dynamicData1C), 37);

    ExAttackType8Extra *extra = record->extra34;

    float angle = g_ReplayRng.GetRandomF32InRange(-3.1415927f);
    extra->motion14.z = 0.0f;
    extra->motion14.FromAngleMagnitude(angle, 2.0f);
    extra->state00 = 0;

    record->unknown00 = 2;
    record->timer10 = 90;

    extra->control38.FromAngleMagnitude(
        g_ReplayRng.GetRandomF32SignedInRange(3.1415927f),
        g_ReplayRng.GetRandomF32InRange(192.0f));
    extra->control44.FromAngleMagnitude(
        g_ReplayRng.GetRandomF32SignedInRange(3.1415927f),
        g_ReplayRng.GetRandomF32InRange(192.0f));

    extra->point20.x = g_GameManager.TransformPopupX(record->position20.x);
    extra->point20.y = g_GameManager.TransformPopupY(record->position20.y);
    extra->point20.z = 0.0f;

    if (g_ReplayRng.GetRandomU32InRange(2) != 0)
    {
        extra->spawn50.x = -144.0f;
        extra->angle08 = 0.0f;
    }
    else
    {
        extra->spawn50.x = 144.0f;
        extra->angle08 = 3.1415927f;
    }
    extra->spawn50.y = 224.0f;
    extra->spawn50.z = 0.0f;

    g_Supervisor.SelectSide(record->opponentSide04);
    extra->point2C.x = g_GameManager.TransformPopupX(extra->spawn50.x);
    extra->point2C.y = g_GameManager.TransformPopupY(extra->spawn50.y);
    extra->point2C.z = 0.0f;
    g_Supervisor.SelectSide(record->side08);

    extra->angle0C =
        g_ReplayRng.GetRandomF32SignedInRange(3.1415927f);

    return 0;
}
float __stdcall AddNormalizeAngle(float angle, float delta);

int __fastcall ExAttackInitializeCallbackType6(ExAttackRecord *base)
{
    ExAttackType8RecordView *record =
        reinterpret_cast<ExAttackType8RecordView *>(base);

    reinterpret_cast<ExAttackType8AllocateView *>(record)
        ->AllocateDynamicData(1, sizeof(ExAttackType8Extra));

    reinterpret_cast<ExAttackType8SideLookupView *>(record)
        ->GetSideState()->anmOwner0C->anm2D4->ExecuteAnmIdx(
            reinterpret_cast<AnmVm *>(record->dynamicData1C), 37);

    ExAttackType8Extra *extra = record->extra34;

    float angle = g_ReplayRng.GetRandomF32InRange(-3.1415927f);
    extra->motion14.z = 0.0f;
    extra->motion14.FromAngleMagnitude(angle, 2.0f);
    extra->state00 = 0;
    record->unknown00 = 2;

    extra->control38.FromAngleMagnitude(
        g_ReplayRng.GetRandomF32SignedInRange(3.1415927f),
        g_ReplayRng.GetRandomF32InRange(192.0f));
    extra->control44.FromAngleMagnitude(
        g_ReplayRng.GetRandomF32SignedInRange(3.1415927f),
        g_ReplayRng.GetRandomF32InRange(192.0f));

    extra->point20.x = g_GameManager.TransformPopupX(record->position20.x);
    extra->point20.y = g_GameManager.TransformPopupY(record->position20.y);
    extra->point20.z = 0.0f;

    float spawnAngle;
    if (g_ReplayRng.GetRandomU32InRange(2) != 0)
    {
        extra->spawn50.x = -144.0f;
        spawnAngle =
            g_ReplayRng.GetRandomF32SignedInRange(0.52359879f);
    }
    else
    {
        extra->spawn50.x = 144.0f;
        spawnAngle = AddNormalizeAngle(
            3.1415927f,
            g_ReplayRng.GetRandomF32SignedInRange(0.52359879f));
    }
    extra->angle08 = spawnAngle;

    extra->unknown10 =
        g_ReplayRng.GetRandomF32SignedInRange(0.01308997f);
    extra->spawn50.y = g_ReplayRng.GetRandomF32InRange(320.0f);
    extra->spawn50.z = 0.0f;

    g_Supervisor.SelectSide(record->opponentSide04);
    extra->point2C.x = g_GameManager.TransformPopupX(extra->spawn50.x);
    extra->point2C.y = g_GameManager.TransformPopupY(extra->spawn50.y);
    extra->point2C.z = 0.0f;
    g_Supervisor.SelectSide(record->side08);

    return 0;
}


int __fastcall ExAttackInitializeCallbackType9(ExAttackRecord *base)
{
    ExAttackType8RecordView *record =
        reinterpret_cast<ExAttackType8RecordView *>(base);

    reinterpret_cast<ExAttackType8AllocateView *>(record)
        ->AllocateDynamicData(1, sizeof(ExAttackType8Extra));

    reinterpret_cast<ExAttackType8SideLookupView *>(record)
        ->GetSideState()->anmOwner0C->anm2D4->ExecuteAnmIdx(
            reinterpret_cast<AnmVm *>(record->dynamicData1C), 37);

    ExAttackType8Extra *extra = record->extra34;

    float angle = g_ReplayRng.GetRandomF32InRange(-3.1415927f);
    extra->motion14.z = 0.0f;
    extra->motion14.FromAngleMagnitude(angle, 2.0f);
    extra->angle08 = angle;
    extra->state00 = 0;

    record->unknown00 = 2;
    record->timer10 = 90;

    extra->control38.FromAngleMagnitude(
        g_ReplayRng.GetRandomF32SignedInRange(3.1415927f),
        g_ReplayRng.GetRandomF32InRange(192.0f));
    extra->control44.FromAngleMagnitude(
        g_ReplayRng.GetRandomF32SignedInRange(3.1415927f),
        g_ReplayRng.GetRandomF32InRange(192.0f));

    extra->point20.x = g_GameManager.TransformPopupX(record->position20.x);
    extra->point20.y = g_GameManager.TransformPopupY(record->position20.y);
    extra->point20.z = 0.0f;

    extra->spawn50.x = -144.0f;
    extra->angle08 = 0.0f;
    extra->spawn50.y = 192.0f;
    extra->spawn50.z = 0.0f;

    g_Supervisor.SelectSide(record->opponentSide04);
    extra->point2C.x = g_GameManager.TransformPopupX(extra->spawn50.x);
    extra->point2C.y = g_GameManager.TransformPopupY(extra->spawn50.y);
    extra->point2C.z = 0.0f;
    g_Supervisor.SelectSide(record->side08);

    extra->angle0C =
        g_ReplayRng.GetRandomF32SignedInRange(3.1415927f);

    return 0;
}
