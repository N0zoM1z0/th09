#include "AnmManager.hpp"
#include "AsciiGameManagerView.hpp"
#include "ExAttackController.hpp"
#include "RngRuntimeLeaves.hpp"
#include "Supervisor.hpp"

struct ExAttackType22Extra
{
    int state00;
    Float3 motion04;
    Float3 point10;
    Float3 point1C;
    Float3 point28;
    Float3 point34;
    Float3 spawnPosition40;
    unsigned char unknown4C[4];
    float value50;
    unsigned char unknown54[0xF48 - 0x54];
};
typedef char ExAttackType22ExtraSizeIsF48[
    (sizeof(ExAttackType22Extra) == 0xF48) ? 1 : -1];

struct ExAttackType22Record
{
    int state00;
    int opponentSide04;
    int side08;
    int active0C;
    ZunTimer timer10;
    void *dynamicData1C;
    Float3 position20;
    int vmCount2C;
    int unknown30;
    ExAttackType22Extra *extra34;
    Float3 *parameter38;
};

struct ExAttackType22AllocateView
{
    unsigned char bytes[0x38];
    void AllocateDynamicData(int vmCount, int extraBytes);
};

struct ExAttackType22AnmOwner
{
    unsigned char unknown000[0x2D4];
    AnmLoaded *anm2D4;
};

struct ExAttackType22SideView
{
    unsigned char unknown00[0x0C];
    ExAttackType22AnmOwner *anmOwner0C;
    unsigned char unknown10[0x28];
};

struct ExAttackType22SideLookupView
{
    int unknown00;
    int opponentSide04;
    int side08;
    ExAttackType22SideView *GetSideState();
};

extern RngRuntimeView g_ReplayRng;

int __fastcall ExAttackInitializeCallbackType22(ExAttackRecord *base)
{
    ExAttackType22Record *record =
        reinterpret_cast<ExAttackType22Record *>(base);

    reinterpret_cast<ExAttackType22AllocateView *>(record)->
        AllocateDynamicData(1, sizeof(ExAttackType22Extra));

    reinterpret_cast<ExAttackType22SideLookupView *>(record)
        ->GetSideState()
        ->anmOwner0C
        ->anm2D4
        ->ExecuteAnmIdx(
            reinterpret_cast<AnmVm *>(record->dynamicData1C), 37);

    Float3 parameter = *record->parameter38;
    ExAttackType22Extra *extra = record->extra34;

    extra->motion04.z = 0.0f;
    extra->motion04.FromAngleMagnitude(parameter.z, 2.0f);
    extra->state00 = 0;
    record->state00 = 2;

    extra->point28.FromAngleMagnitude(
        g_ReplayRng.GetRandomF32SignedInRange(3.1415927f),
        g_ReplayRng.GetRandomF32InRange(192.0f));

    extra->point34.FromAngleMagnitude(
        g_ReplayRng.GetRandomF32SignedInRange(3.1415927f),
        g_ReplayRng.GetRandomF32InRange(192.0f));

    extra->point10.x = g_GameManager.TransformPopupX(record->position20.x);
    extra->point10.y = g_GameManager.TransformPopupY(record->position20.y);
    extra->point10.z = 0.0f;

    extra->spawnPosition40.x = parameter.x;
    extra->spawnPosition40.y = parameter.y;
    extra->spawnPosition40.z = 0.0f;

    g_Supervisor.SelectSide(record->opponentSide04);
    extra->point1C.x =
        g_GameManager.TransformPopupX(extra->spawnPosition40.x);
    extra->point1C.y =
        g_GameManager.TransformPopupY(extra->spawnPosition40.y);
    extra->point1C.z = 0.0f;

    extra->value50 = 4.0f;
    return 0;
}
