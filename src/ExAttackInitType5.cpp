#include "AnmManager.hpp"
#include "ExAttackController.hpp"
#include "RngRuntimeLeaves.hpp"
#include "Supervisor.hpp"

struct ExAttackType5Extra
{
    int state00;
    Float3 vector04;
    Float3 vector10;
    Float3 point1C;
    Float3 point28;
    Float3 control34;
    Float3 control40;
    Float3 spawn4C;
};
typedef char ExAttackType5ExtraSizeIs58[
    (sizeof(ExAttackType5Extra) == 0x58) ? 1 : -1];

struct ExAttackType5RecordView
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
    ExAttackType5Extra *extra34;
};

struct ExAttackType5AnmOwner
{
    unsigned char unknown000[0x2D4];
    AnmLoaded *anm2D4;
};

struct ExAttackType5PlayerView
{
    unsigned char unknown000[0x1B88];
    Float3 position1B88;
};

struct ExAttackType5SideView
{
    unsigned char unknown00[0x04];
    ExAttackType5PlayerView *player04;
    unsigned char unknown08[0x04];
    ExAttackType5AnmOwner *anmOwner0C;
    unsigned char unknown10[0x28];
};

struct ExAttackType5GameManagerView
{
    ExAttackType5SideView sides[2];

    float TransformPopupX(float value);
    float TransformPopupY(float value);
};

struct ExAttackType5SideLookupView
{
    int unknown00;
    int opponentSide04;
    int side08;
    ExAttackType5SideView *GetSideState();
};

struct ExAttackType5AllocateView
{
    unsigned char bytes[0x38];
    void AllocateDynamicData(int vmCount, int extraBytes);
};

extern ExAttackType5GameManagerView g_GameManager;
extern RngRuntimeView g_ReplayRng;

int __fastcall ExAttackInitializeCallbackType5(ExAttackRecord *base)
{
    ExAttackType5RecordView *record =
        reinterpret_cast<ExAttackType5RecordView *>(base);

    reinterpret_cast<ExAttackType5AllocateView *>(record)
        ->AllocateDynamicData(1, sizeof(ExAttackType5Extra));

    reinterpret_cast<ExAttackType5SideLookupView *>(record)
        ->GetSideState()->anmOwner0C->anm2D4->ExecuteAnmIdx(
            reinterpret_cast<AnmVm *>(record->dynamicData1C), 37);

    ExAttackType5Extra *extra = record->extra34;
    extra->vector04.x = 0.0f;
    extra->vector04.y = 0.0f;
    extra->vector04.z = 0.0f;
    extra->vector10.x = 0.0f;
    extra->vector10.y = 0.028f;
    extra->vector10.z = 0.0f;
    extra->state00 = 0;
    record->unknown00 = 2;

    extra->control34.FromAngleMagnitude(
        g_ReplayRng.GetRandomF32SignedInRange(3.1415927f),
        g_ReplayRng.GetRandomF32InRange(192.0f));
    extra->control40.FromAngleMagnitude(
        g_ReplayRng.GetRandomF32SignedInRange(3.1415927f),
        g_ReplayRng.GetRandomF32InRange(192.0f));

    extra->point1C.x = g_GameManager.TransformPopupX(record->position20.x);
    extra->point1C.y = g_GameManager.TransformPopupY(record->position20.y);
    extra->point1C.z = 0.0f;

    extra->spawn4C.x =
        g_ReplayRng.GetRandomF32SignedInRange(32.0f) +
        g_GameManager.sides[record->opponentSide04].player04->position1B88.x;
    extra->spawn4C.y = 16.0f;
    extra->spawn4C.z = 0.0f;

    g_Supervisor.SelectSide(record->opponentSide04);
    extra->point28.x = g_GameManager.TransformPopupX(extra->spawn4C.x);
    extra->point28.y = g_GameManager.TransformPopupY(extra->spawn4C.y);
    extra->point28.z = 0.0f;
    g_Supervisor.SelectSide(record->side08);

    return 0;
}
