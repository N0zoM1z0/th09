#include "AnmManager.hpp"
#include "AsciiGameManagerView.hpp"
#include "ExAttackController.hpp"
#include "RngRuntimeLeaves.hpp"
#include "Supervisor.hpp"

struct ExAttackType16Extra
{
    int state00;
    Float3 motion04;
    float scalar10;
    Float3 point14;
    Float3 point20;
    Float3 control2C;
    Float3 control38;
    Float3 spawn44;
};
typedef char ExAttackType16ExtraSizeIs50[
    (sizeof(ExAttackType16Extra) == 0x50) ? 1 : -1];

struct ExAttackType16RecordView
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
    ExAttackType16Extra *extra34;
};

struct ExAttackType16AnmOwner
{
    unsigned char unknown000[0x2D4];
    AnmLoaded *anm2D4;
};

struct ExAttackType16SideView
{
    unsigned char unknown00[0x0C];
    ExAttackType16AnmOwner *anmOwner0C;
    unsigned char unknown10[0x28];
};

struct ExAttackType16SideLookupView
{
    int unknown00;
    int opponentSide04;
    int side08;
    ExAttackType16SideView *GetSideState();
};

struct ExAttackType16AllocateView
{
    unsigned char bytes[0x38];
    void AllocateDynamicData(int vmCount, int extraBytes);
};

extern RngRuntimeView g_ReplayRng;
extern float g_ExAttackPlayfieldWidth;

int __fastcall ExAttackInitializeCallbackType16(ExAttackRecord *base)
{
    ExAttackType16RecordView *record =
        reinterpret_cast<ExAttackType16RecordView *>(base);

    reinterpret_cast<ExAttackType16AllocateView *>(record)
        ->AllocateDynamicData(1, sizeof(ExAttackType16Extra));

    reinterpret_cast<ExAttackType16SideLookupView *>(record)
        ->GetSideState()->anmOwner0C->anm2D4->ExecuteAnmIdx(
            reinterpret_cast<AnmVm *>(record->dynamicData1C), 37);

    ExAttackType16Extra *extra = record->extra34;
    float angle = g_ReplayRng.GetRandomF32InRange(-3.1415927f);
    extra->motion04.z = 0.0f;
    extra->motion04.FromAngleMagnitude(angle, 2.0f);
    extra->state00 = 0;
    record->unknown00 = 2;

    extra->control2C.FromAngleMagnitude(
        g_ReplayRng.GetRandomF32SignedInRange(3.1415927f),
        g_ReplayRng.GetRandomF32InRange(192.0f));
    extra->control38.FromAngleMagnitude(
        g_ReplayRng.GetRandomF32SignedInRange(3.1415927f),
        g_ReplayRng.GetRandomF32InRange(192.0f));

    extra->point14.x = g_GameManager.TransformPopupX(record->position20.x);
    extra->point14.y = g_GameManager.TransformPopupY(record->position20.y);
    extra->point14.z = 0.0f;

    extra->spawn44.x =
        g_ReplayRng.GetRandomF32SignedInRange(
            g_ExAttackPlayfieldWidth * 0.5f - 64.0f);
    extra->spawn44.y = g_ReplayRng.GetRandomF32InRange(128.0f);
    extra->spawn44.z = 0.0f;

    g_Supervisor.SelectSide(record->opponentSide04);
    extra->point20.x = g_GameManager.TransformPopupX(extra->spawn44.x);
    extra->point20.y = g_GameManager.TransformPopupY(extra->spawn44.y);
    extra->point20.z = 0.0f;
    g_Supervisor.SelectSide(record->side08);
    return 0;
}

int __fastcall ExAttackInitializeCallbackType23(ExAttackRecord *base)
{
    ExAttackType16RecordView *record =
        reinterpret_cast<ExAttackType16RecordView *>(base);

    reinterpret_cast<ExAttackType16AllocateView *>(record)
        ->AllocateDynamicData(1, sizeof(ExAttackType16Extra));

    reinterpret_cast<ExAttackType16SideLookupView *>(record)
        ->GetSideState()->anmOwner0C->anm2D4->ExecuteAnmIdx(
            reinterpret_cast<AnmVm *>(record->dynamicData1C), 37);

    ExAttackType16Extra *extra = record->extra34;
    record->timer10 = 90;
    float angle = g_ReplayRng.GetRandomF32InRange(-3.1415927f);
    extra->motion04.z = 0.0f;
    extra->motion04.FromAngleMagnitude(angle, 2.0f);
    extra->state00 = 0;
    record->unknown00 = 2;

    extra->control2C.FromAngleMagnitude(
        g_ReplayRng.GetRandomF32SignedInRange(3.1415927f),
        g_ReplayRng.GetRandomF32InRange(192.0f));
    extra->control38.FromAngleMagnitude(
        g_ReplayRng.GetRandomF32SignedInRange(3.1415927f),
        g_ReplayRng.GetRandomF32InRange(192.0f));

    extra->point14.x = g_GameManager.TransformPopupX(record->position20.x);
    extra->point14.y = g_GameManager.TransformPopupY(record->position20.y);
    extra->point14.z = 0.0f;

    extra->spawn44.x = record->position20.x;
    extra->spawn44.y = record->position20.y;
    extra->spawn44.z = 0.0f;

    g_Supervisor.SelectSide(record->opponentSide04);
    extra->point20.x = g_GameManager.TransformPopupX(extra->spawn44.x);
    extra->point20.y = g_GameManager.TransformPopupY(extra->spawn44.y);
    extra->point20.z = 0.0f;
    g_Supervisor.SelectSide(record->side08);
    return 0;
}
