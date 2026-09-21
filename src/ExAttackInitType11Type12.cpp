#include "AnmManager.hpp"
#include "AsciiGameManagerView.hpp"
#include "ExAttackController.hpp"
#include "RngRuntimeLeaves.hpp"
#include "Supervisor.hpp"

struct ExAttackType1112Extra
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
typedef char ExAttackType1112ExtraSizeIs5C[
    (sizeof(ExAttackType1112Extra) == 0x5C) ? 1 : -1];

struct ExAttackType1112RecordView
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
    ExAttackType1112Extra *extra34;
};

struct ExAttackType1112AnmOwner
{
    unsigned char unknown000[0x2D4];
    AnmLoaded *anm2D4;
};

struct ExAttackType1112SideView
{
    unsigned char unknown00[0x0C];
    ExAttackType1112AnmOwner *anmOwner0C;
    unsigned char unknown10[0x28];
};

struct ExAttackType1112SideLookupView
{
    int unknown00;
    int opponentSide04;
    int side08;
    ExAttackType1112SideView *GetSideState();
};

struct ExAttackType1112AllocateView
{
    unsigned char bytes[0x38];
    void AllocateDynamicData(int vmCount, int extraBytes);
};

extern RngRuntimeView g_ReplayRng;

int __fastcall ExAttackInitializeCallbackType11And12(ExAttackRecord *base)
{
    ExAttackType1112RecordView *record =
        reinterpret_cast<ExAttackType1112RecordView *>(base);

    reinterpret_cast<ExAttackType1112AllocateView *>(record)
        ->AllocateDynamicData(1, sizeof(ExAttackType1112Extra));

    reinterpret_cast<ExAttackType1112SideLookupView *>(record)
        ->GetSideState()->anmOwner0C->anm2D4->ExecuteAnmIdx(
            reinterpret_cast<AnmVm *>(record->dynamicData1C), 37);

    ExAttackType1112Extra *extra = record->extra34;

    g_ReplayRng.GetRandomF32InRange(-3.1415927f);

    extra->motion14.z = 0.0f;
    extra->motion14.FromAngleMagnitude(record->position20.z, 2.0f);
    extra->angle08 = record->position20.z;
    extra->state00 = 0;

    record->unknown00 = 2;
    record->timer10 = 90;
    record->position20.z = 0.0f;

    extra->control38.FromAngleMagnitude(
        g_ReplayRng.GetRandomF32SignedInRange(3.1415927f),
        g_ReplayRng.GetRandomF32InRange(192.0f));
    extra->control44.FromAngleMagnitude(
        g_ReplayRng.GetRandomF32SignedInRange(3.1415927f),
        g_ReplayRng.GetRandomF32InRange(192.0f));

    extra->point20.x = g_GameManager.TransformPopupX(record->position20.x);
    extra->point20.y = g_GameManager.TransformPopupY(record->position20.y);
    extra->point20.z = 0.0f;

    g_Supervisor.SelectSide(record->opponentSide04);

    extra->spawn50.x = record->position20.x;
    extra->spawn50.y = record->position20.y;
    extra->spawn50.z = 0.0f;

    extra->point2C.x = g_GameManager.TransformPopupX(record->position20.x);
    extra->point2C.y = g_GameManager.TransformPopupY(record->position20.y);
    extra->point2C.z = 0.0f;

    g_Supervisor.SelectSide(record->side08);

    extra->angle0C =
        g_ReplayRng.GetRandomF32SignedInRange(3.1415927f);

    return 0;
}
