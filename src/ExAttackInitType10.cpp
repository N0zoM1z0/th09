#include "RuntimePolarMath.hpp"
#include "AnmManager.hpp"
#include "AsciiGameManagerView.hpp"
#include "ExAttackController.hpp"
#include "ExAttackRecordRuntimeView.hpp"
#include "RngRuntimeLeaves.hpp"
#include "Supervisor.hpp"

struct ExAttackType10Extra
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
typedef char ExAttackType10ExtraSizeIs58[
    (sizeof(ExAttackType10Extra) == 0x58) ? 1 : -1];

struct ExAttackType10RecordView
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
    ExAttackType10Extra *extra34;
};

extern RngRuntimeView g_ReplayRng;

int __fastcall ExAttackInitializeCallbackType10(ExAttackRecord *base)
{
    ExAttackType10RecordView *record =
        reinterpret_cast<ExAttackType10RecordView *>(base);

    reinterpret_cast<ExAttackAllocateRecordView *>(record)
        ->AllocateDynamicData(1, sizeof(ExAttackType10Extra));

    reinterpret_cast<ExAttackRecordSideView *>(record)
        ->GetSideState()->effectManager0C->effectAnm->ExecuteAnmIdx(
            reinterpret_cast<AnmVm *>(record->dynamicData1C), 37);

    ExAttackType10Extra *extra = record->extra34;
    extra->vector04.x = 0.0f;
    extra->vector04.y = 0.0f;
    extra->vector04.z = 0.0f;
    extra->vector10.x = g_ReplayRng.GetRandomF32SignedInRange(0.001f);
    extra->vector10.y = 0.025f;
    extra->vector10.z = 0.0f;
    extra->state00 = 0;
    record->timer10 = 40;
    record->unknown00 = 2;

    extra->point1C.x = g_GameManager.TransformPopupX(record->position20.x);
    extra->point1C.y = g_GameManager.TransformPopupY(record->position20.y);
    extra->point1C.z = 0.0f;

    g_Supervisor.SelectSide(record->opponentSide04);

    SetPolar2D(reinterpret_cast<Float2 *>(&(extra->control34)),
        g_ReplayRng.GetRandomF32SignedInRange(3.1415927f),
        g_ReplayRng.GetRandomF32InRange(192.0f));
    SetPolar2D(reinterpret_cast<Float2 *>(&(extra->control40)),
        g_ReplayRng.GetRandomF32SignedInRange(3.1415927f),
        g_ReplayRng.GetRandomF32InRange(192.0f));

    extra->spawn4C.x = record->position20.x;
    extra->spawn4C.y = record->position20.y;
    extra->spawn4C.z = 0.0f;

    extra->point28.x = g_GameManager.TransformPopupX(record->position20.x);
    extra->point28.y = g_GameManager.TransformPopupY(record->position20.y);
    extra->point28.z = 0.0f;

    g_Supervisor.SelectSide(record->side08);

    return 0;
}
