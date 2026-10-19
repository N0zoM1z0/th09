// Included once by EclManager.cpp; the real Float3 helper is already visible.
#include "AnmManager.hpp"
#include "ExAttackRecordRuntimeView.hpp"
#include "ExAttackType8GameManagerView.hpp"
#include "AsciiManager.hpp"
#include "ExAttackController.hpp"
#include "RngRuntimeLeaves.hpp"
#include "Supervisor.hpp"

#include <stddef.h>

#include "ExAttackType19ExtraView.hpp"

extern RngRuntimeView g_ReplayRng;
extern float g_ExAttackPlayfieldWidth;

int __fastcall ExAttackInitializeCallbackType19(ExAttackRecord *base)
{
    ExAttackType19RecordView *record =
        reinterpret_cast<ExAttackType19RecordView *>(base);

    reinterpret_cast<ExAttackAllocateRecordView *>(record)
        ->AllocateDynamicData(1, sizeof(ExAttackType19Extra));

    reinterpret_cast<ExAttackRecordSideView *>(record)
        ->GetSideState()->effectManager0C->effectAnm->ExecuteAnmIdx(
            reinterpret_cast<AnmVm *>(record->dynamicData1C), 37);

    ExAttackType19Extra *extra = record->extra34;

    float initialAngle = g_ReplayRng.GetRandomF32InRange(-3.1415927f);
    extra->motion04.z = 0.0f;
    extra->motion04.FromAngleMagnitude(initialAngle, 2.0f);
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
            g_ExAttackPlayfieldWidth * 0.5f - 24.0f);
    extra->spawn40.y =
        448.0f - g_ReplayRng.GetRandomF32InRange(256.0f);
    extra->spawn40.z = 0.0f;

    g_Supervisor.SelectSide(record->opponentSide04);
    extra->point1C.x = g_GameManager.TransformPopupX(extra->spawn40.x);
    extra->point1C.y = g_GameManager.TransformPopupY(extra->spawn40.y);
    extra->point1C.z = 0.0f;
    g_Supervisor.SelectSide(record->side08);

    extra->history3E8[0] = extra->spawn40;
    ExAttackType19Vertex *vertex = &extra->vertices4C[0];
    vertex->rhw = 1.0f;
    vertex->u = 0.5f;
    vertex->v = 0.5f;

    float angle = -3.1415927f;
    ++vertex;
    float angularStep =
        g_ReplayRng.GetRandomF32SignedInRange(0.06666667f);
    Float3 *spawn = &extra->spawn40;
    // Radius and angular-step arrays have distinct bounds and ownership.
    int sample = 0;

    do
    {
        if (angle >= 3.1415927f)
            angle -= 6.2831855f;

        vertex->rhw = 1.0f;

        Float3 uvVector;
        uvVector.FromAngleMagnitude(angle, 0.5f);
        vertex->u = uvVector.x + 0.5f;
        (vertex++)->v = uvVector.y + 0.5f;

        // Ring samples occupy history slots 1..32 after the center at zero.
        extra->history3E8[sample + 1].z = 0.0f;
        extra->magnitudes578[sample] =
            g_ReplayRng.GetRandomF32SignedInRange(8.0f) + 80.0f;
        extra->angularSteps5FC[sample] = angularStep;

        angularStep +=
            g_ReplayRng.GetRandomF32SignedInRange(0.033333335f);
        if (angularStep < -0.06666667f)
            angularStep = -0.06666667f;
        else if (angularStep > 0.06666667f)
            angularStep = 0.06666667f;

        extra->history3E8[sample + 1].FromAngleMagnitude(
            angle, extra->magnitudes578[sample]);
        extra->history3E8[sample + 1] += *spawn;

        ++sample;
        angle += 0.2026834f;
    }
    while (sample != 32);

    extra->drift67C =
        g_ReplayRng.GetRandomF32SignedInRange(0.0083333338f);
    extra->drift680 =
        g_ReplayRng.GetRandomF32SignedInRange(0.0083333338f);

    return 0;
}
