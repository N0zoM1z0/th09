#include "AnmManager.hpp"
#include "AsciiGameManagerView.hpp"
#include "AsciiManager.hpp"
#include "ExAttackController.hpp"
#include "RngRuntimeLeaves.hpp"
#include "Supervisor.hpp"

#include <stddef.h>

struct ExAttackType19Vertex
{
    Float3 pos;
    float rhw;
    unsigned long diffuse;
    float u;
    float v;
};
typedef char ExAttackType19VertexSizeIs1C[
    (sizeof(ExAttackType19Vertex) == 0x1C) ? 1 : -1];

struct ExAttackType19Extra
{
    int state00;
    Float3 motion04;
    Float3 point10;
    Float3 point1C;
    Float3 control28;
    Float3 control34;
    Float3 spawn40;
    ExAttackType19Vertex vertices4C[33];
    Float3 history3E8[32];
    unsigned char unknown568[0x578 - 0x568];
    float magnitudes578[32];
    float unknown5F8;
    float angularSteps5FC[32];
    float drift67C;
    float drift680;
    int drawPending684;
};
typedef char ExAttackType19ExtraSizeIs688[
    (sizeof(ExAttackType19Extra) == 0x688) ? 1 : -1];
typedef char ExAttackType19VerticesAt4C[
    (offsetof(ExAttackType19Extra, vertices4C) == 0x4C) ? 1 : -1];
typedef char ExAttackType19HistoryAt3E8[
    (offsetof(ExAttackType19Extra, history3E8) == 0x3E8) ? 1 : -1];
typedef char ExAttackType19MagnitudesAt578[
    (offsetof(ExAttackType19Extra, magnitudes578) == 0x578) ? 1 : -1];
typedef char ExAttackType19StepsAt5FC[
    (offsetof(ExAttackType19Extra, angularSteps5FC) == 0x5FC) ? 1 : -1];
typedef char ExAttackType19PendingAt684[
    (offsetof(ExAttackType19Extra, drawPending684) == 0x684) ? 1 : -1];

struct ExAttackType19RecordView
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
    ExAttackType19Extra *extra34;
};

struct ExAttackType19AnmOwner
{
    unsigned char unknown000[0x2D4];
    AnmLoaded *anm2D4;
};

struct ExAttackType19SideView
{
    unsigned char unknown00[0x0C];
    ExAttackType19AnmOwner *anmOwner0C;
    unsigned char unknown10[0x28];
};

struct ExAttackType19SideLookupView
{
    int state00;
    int opponentSide04;
    int side08;
    ExAttackType19SideView *GetSideState();
};

struct ExAttackType19AllocateView
{
    unsigned char bytes[0x38];
    void AllocateDynamicData(int vmCount, int extraBytes);
};

extern RngRuntimeView g_ReplayRng;
extern float g_ExAttackPlayfieldWidth;

int __fastcall ExAttackInitializeCallbackType19(ExAttackRecord *base)
{
    ExAttackType19RecordView *record =
        reinterpret_cast<ExAttackType19RecordView *>(base);

    reinterpret_cast<ExAttackType19AllocateView *>(record)
        ->AllocateDynamicData(1, sizeof(ExAttackType19Extra));

    reinterpret_cast<ExAttackType19SideLookupView *>(record)
        ->GetSideState()->anmOwner0C->anm2D4->ExecuteAnmIdx(
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
    float *uv = &vertex->u;
    float angularStep =
        g_ReplayRng.GetRandomF32SignedInRange(0.06666667f);
    Float3 *history = &extra->history3E8[1];
    Float3 *spawn = &extra->spawn40;
    float *magnitude = &extra->magnitudes578[0];
    int remaining = 32;

    do
    {
        if (angle >= 3.1415927f)
            angle -= 6.2831855f;

        uv[-2] = 1.0f;

        float uvStorage[3];
        Float3 *uvVector = reinterpret_cast<Float3 *>(uvStorage);
        uvVector->FromAngleMagnitude(angle, 0.5f);
        uv[0] = uvVector->x + 0.5f;
        uv[1] = uvVector->y + 0.5f;

        history->z = 0.0f;
        *magnitude =
            g_ReplayRng.GetRandomF32SignedInRange(8.0f) + 80.0f;
        magnitude[33] = angularStep;

        angularStep +=
            g_ReplayRng.GetRandomF32SignedInRange(0.033333335f);
        if (angularStep < -0.06666667f)
            angularStep = -0.06666667f;
        else if (angularStep > 0.06666667f)
            angularStep = 0.06666667f;

        history->FromAngleMagnitude(angle, *magnitude);
        *history += *spawn;

        uv += sizeof(ExAttackType19Vertex) / sizeof(float);
        ++history;
        ++magnitude;
        angle += 0.2026834f;
    }
    while (--remaining != 0);

    extra->drift67C =
        g_ReplayRng.GetRandomF32SignedInRange(0.0083333338f);
    extra->drift680 =
        g_ReplayRng.GetRandomF32SignedInRange(0.0083333338f);

    return 0;
}
