#include "RuntimePolarMath.hpp"
#include "AnmManager.hpp"
#include "AsciiGameManagerView.hpp"
#include "ExAttackController.hpp"
#include "ExAttackRecordRuntimeView.hpp"
#include "ExAttackType1422ExtraView.hpp"
#include "RngRuntimeLeaves.hpp"
#include "Supervisor.hpp"


extern RngRuntimeView g_ReplayRng;

int __fastcall ExAttackInitializeCallbackType22(ExAttackRecord *base)
{
    ExAttackType22Record *record =
        reinterpret_cast<ExAttackType22Record *>(base);

    reinterpret_cast<ExAttackAllocateRecordView *>(record)->
        AllocateDynamicData(1, sizeof(ExAttackType22Extra));

    reinterpret_cast<ExAttackRecordSideView *>(record)
        ->GetSideState()
        ->effectManager0C
        ->effectAnm
        ->ExecuteAnmIdx(
            reinterpret_cast<AnmVm *>(record->common00.dynamicData1C), 37);

    Float3 parameter = *record->parameter38;
    ExAttackType22Extra *extra = record->common00.extra34;

    extra->motion04.z = 0.0f;
    SetPolar2D(reinterpret_cast<Float2 *>(&(extra->motion04)), parameter.z, 2.0f);
    extra->state00 = 0;
    record->common00.state00 = 2;

    SetPolar2D(reinterpret_cast<Float2 *>(&(extra->point28)),
        g_ReplayRng.GetRandomF32SignedInRange(3.1415927f),
        g_ReplayRng.GetRandomF32InRange(192.0f));

    SetPolar2D(reinterpret_cast<Float2 *>(&(extra->point34)),
        g_ReplayRng.GetRandomF32SignedInRange(3.1415927f),
        g_ReplayRng.GetRandomF32InRange(192.0f));

    extra->point10.x = g_GameManager.TransformPopupX(record->common00.position20.x);
    extra->point10.y = g_GameManager.TransformPopupY(record->common00.position20.y);
    extra->point10.z = 0.0f;

    extra->spawn40.x = parameter.x;
    extra->spawn40.y = parameter.y;
    extra->spawn40.z = 0.0f;

    g_Supervisor.SelectSide(record->common00.opponentSide04);
    extra->point1C.x =
        g_GameManager.TransformPopupX(extra->spawn40.x);
    extra->point1C.y =
        g_GameManager.TransformPopupY(extra->spawn40.y);
    extra->point1C.z = 0.0f;

    extra->value50 = 4.0f;
    return 0;
}
