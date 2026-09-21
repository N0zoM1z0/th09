#include "AnmManager.hpp"
#include "AsciiGameManagerView.hpp"
#include "ExAttackController.hpp"
#include "RngRuntimeLeaves.hpp"
#include "Supervisor.hpp"

struct ExAttackType17Extra
{
    int state00;
    float scalar04;
    float scalar08;
    float scalar0C;
    Float3 point10;
    Float3 point1C;
    Float3 control28;
    Float3 control34;
    Float3 spawn40;
    float unknown4C;
};
typedef char ExAttackType17ExtraSizeIs50[
    (sizeof(ExAttackType17Extra) == 0x50) ? 1 : -1];

struct ExAttackType17RecordView
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
    ExAttackType17Extra *extra34;
};

struct ExAttackType17AnmOwner
{
    unsigned char unknown000[0x2D4];
    AnmLoaded *anm2D4;
};

struct ExAttackType17SideView
{
    unsigned char unknown00[0x0C];
    ExAttackType17AnmOwner *anmOwner0C;
    unsigned char unknown10[0x28];
};

struct ExAttackType17SideLookupView
{
    int unknown00;
    int opponentSide04;
    int side08;
    ExAttackType17SideView *GetSideState();
};

struct ExAttackType17AllocateView
{
    unsigned char bytes[0x38];
    void AllocateDynamicData(int vmCount, int extraBytes);
};

extern RngRuntimeView g_ReplayRng;
extern float g_ExAttackPlayfieldWidth;

int __fastcall ExAttackInitializeCallbackType17(ExAttackRecord *base)
{
    ExAttackType17RecordView *record =
        reinterpret_cast<ExAttackType17RecordView *>(base);

    reinterpret_cast<ExAttackType17AllocateView *>(record)
        ->AllocateDynamicData(1, sizeof(ExAttackType17Extra));

    reinterpret_cast<ExAttackType17SideLookupView *>(record)
        ->GetSideState()->anmOwner0C->anm2D4->ExecuteAnmIdx(
            reinterpret_cast<AnmVm *>(record->dynamicData1C), 37);

    ExAttackType17Extra *extra = record->extra34;

    g_ReplayRng.GetRandomF32InRange(-3.1415927f);
    extra->state00 = 0;
    record->unknown00 = 2;

    extra->control28.FromAngleMagnitude(
        g_ReplayRng.GetRandomF32SignedInRange(3.1415927f),
        g_ReplayRng.GetRandomF32InRange(192.0f));
    extra->control34.FromAngleMagnitude(
        g_ReplayRng.GetRandomF32SignedInRange(3.1415927f),
        g_ReplayRng.GetRandomF32InRange(192.0f));

    extra->point10.x = g_GameManager.TransformPopupX(record->position20.x);
    extra->point10.y = g_GameManager.TransformPopupY(record->position20.y);
    extra->point10.z = 0.0f;

    extra->spawn40.x =
        g_ReplayRng.GetRandomF32SignedInRange(
            g_ExAttackPlayfieldWidth * 0.5f - 8.0f);
    extra->spawn40.y = g_ReplayRng.GetRandomF32InRange(128.0f);
    extra->spawn40.z = 0.0f;

    g_Supervisor.SelectSide(record->opponentSide04);
    extra->point1C.x = g_GameManager.TransformPopupX(extra->spawn40.x);
    extra->point1C.y = g_GameManager.TransformPopupY(extra->spawn40.y);
    extra->point1C.z = 0.0f;
    g_Supervisor.SelectSide(record->side08);

    extra->scalar04 = 0.0f;
    float scalar = g_ReplayRng.GetRandomF32InRange(2.0f);
    extra->scalar0C = 0.0f;
    extra->scalar08 = scalar + 0.5f;

    return 0;
}
