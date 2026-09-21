#include "AnmManager.hpp"
#include "AsciiGameManagerView.hpp"
#include "ExAttackController.hpp"
#include "RngRuntimeLeaves.hpp"
#include "Supervisor.hpp"

struct ExAttackType20InitExtra
{
    int state00;
    Float3 motion04;
    Float3 point10;
    Float3 point1C;
    Float3 control28;
    Float3 control34;
    Float3 spawn40;
    float speed4C;
    float angle50;
    float angleStep54;
    int counter58;
};
typedef char ExAttackType20InitExtraSizeIs5C[
    (sizeof(ExAttackType20InitExtra) == 0x5C) ? 1 : -1];

struct ExAttackType20InitRecord
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
    ExAttackType20InitExtra *extra34;
};

struct ExAttackType20AllocateView
{
    unsigned char bytes[0x38];
    void AllocateDynamicData(int vmCount, int extraBytes);
};

struct ExAttackType20AnmOwner
{
    unsigned char unknown000[0x2D4];
    AnmLoaded *anm2D4;
};

struct ExAttackType20SideView
{
    unsigned char unknown00[0x0C];
    ExAttackType20AnmOwner *anmOwner0C;
    unsigned char unknown10[0x28];
};

struct ExAttackType20SideLookupView
{
    int unknown00;
    int opponentSide04;
    int side08;
    ExAttackType20SideView *GetSideState();
};

extern RngRuntimeView g_ReplayRng;
extern float g_ExAttackPlayfieldWidth;

int __fastcall ExAttackInitializeCallbackType20(ExAttackRecord *base)
{
    ExAttackType20InitRecord *record =
        reinterpret_cast<ExAttackType20InitRecord *>(base);

    reinterpret_cast<ExAttackType20AllocateView *>(record)
        ->AllocateDynamicData(1, sizeof(ExAttackType20InitExtra));

    reinterpret_cast<ExAttackType20SideLookupView *>(record)
        ->GetSideState()
        ->anmOwner0C
        ->anm2D4
        ->ExecuteAnmIdx(
            reinterpret_cast<AnmVm *>(record->dynamicData1C), 37);

    ExAttackType20InitExtra *extra = record->extra34;

    float angle = g_ReplayRng.GetRandomF32InRange(-3.1415927f);
    extra->motion04.z = 0.0f;
    extra->motion04.FromAngleMagnitude(angle, 2.0f);
    extra->state00 = 0;
    record->state00 = 2;

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
    extra->spawn40.y =
        g_ReplayRng.GetRandomF32InRange(144.0f) + 32.0f;
    extra->spawn40.z = 0.0f;

    g_Supervisor.SelectSide(record->opponentSide04);
    extra->point1C.x = g_GameManager.TransformPopupX(extra->spawn40.x);
    extra->point1C.y = g_GameManager.TransformPopupY(extra->spawn40.y);
    extra->point1C.z = 0.0f;
    g_Supervisor.SelectSide(record->side08);

    extra->speed4C = 1.0f;
    extra->angle50 =
        g_ReplayRng.GetRandomF32SignedInRange(3.1415927f);
    float angleStep;
    if (g_ReplayRng.GetRandomU16InRange(2))
        angleStep = 0.10471976f;
    else
        angleStep = -0.10471976f;
    extra->counter58 = 0;
    extra->angleStep54 = angleStep;

    return 0;
}
