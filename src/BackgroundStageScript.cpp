// Target-bound reconstruction of TH09's stage-script/camera phase at
// 0x004018F0 and its file-local interpolation helper at 0x004016E0.
//
// The private views below expose only offsets independently observed in TH09.
// They deliberately do not claim original class/member spelling. The current
// /O2 /Ob0 /Oy- source reproduces the complete private interpolation helper.
// Both the 2472-byte stage-script owner and the 423-byte interpolation helper
// replay exactly with their complete compiler switch tables. Real same-TU
// timer comparisons and scalar Hermite visibility recover private transport.

#include "AsciiManager.hpp"
#include "AnmManager.hpp"
#include "ZunTimer.hpp"
#include "Supervisor.hpp"

#include <stddef.h>
#include <math.h>
#include <windows.h>
#include <d3d8.h>

struct Background;

struct D3DXVECTOR3 : public D3DVECTOR {};
D3DXVECTOR3 *WINAPI D3DXVec3Normalize(
    D3DXVECTOR3 *out, const D3DXVECTOR3 *vector);

struct RawStageInstrProbe
{
    int frame;
    short opcode;
    short size;
    int args[3];
};
typedef char RawStageInstrProbeSizeIs14[
    sizeof(RawStageInstrProbe) == 0x14 ? 1 : -1];

struct BackgroundFogProbe
{
    float nearPlane;
    float farPlane;
    unsigned int color;
};
typedef char BackgroundFogProbeSizeIs0C[
    sizeof(BackgroundFogProbe) == 0x0C ? 1 : -1];

// The four Background-owned camera snapshots have the same physical 0xF0
// representation as the native Supervisor viewport. This reuses the data
// layout, not the lifetime or ownership of the Supervisor's allocation.
typedef SupervisorCtorArrayElement BackgroundStageCameraRecord;
typedef char BackgroundStageNativeViewportLayout[
    (sizeof(BackgroundStageCameraRecord) == 0xF0 &&
     offsetof(BackgroundStageCameraRecord, cameraPosition00) == 0x00 &&
     offsetof(BackgroundStageCameraRecord, cameraLookAtOffset0C) == 0x0C &&
     offsetof(BackgroundStageCameraRecord, cameraUp18) == 0x18 &&
     offsetof(BackgroundStageCameraRecord, cameraForward24) == 0x24 &&
     offsetof(BackgroundStageCameraRecord, cameraPositionOffset3C) == 0x3C &&
     offsetof(BackgroundStageCameraRecord, fieldOfView48) == 0x48 &&
     offsetof(SupervisorCtorArrayElement, cameraPosition00) == 0x00 &&
     offsetof(SupervisorCtorArrayElement, cameraLookAtOffset0C) == 0x0C &&
     offsetof(SupervisorCtorArrayElement, cameraUp18) == 0x18 &&
     offsetof(SupervisorCtorArrayElement, cameraForward24) == 0x24 &&
     offsetof(SupervisorCtorArrayElement, cameraPositionOffset3C) == 0x3C &&
     offsetof(SupervisorCtorArrayElement, fieldOfView48) == 0x48) ? 1 : -1];

struct BackgroundStageView
{
    unsigned char unknown0000[0x0C];
    int viewportIndex00C;
    unsigned char unknown0010[0x18 - 0x10];
    int pendingStageLabel018;
    unsigned char unknown001C[0x28 - 0x1C];
    RawStageInstrProbe *stageScript028;
    AnmVm stageVm0_02C;
    AnmVm stageVm1_2D0;
    AnmVm stageVm2_574;
    AnmLoaded *stageAnm818;
    unsigned char unknown81C[0x834 - 0x81C];

    BackgroundStageCameraRecord cameraTarget834;
    BackgroundStageCameraRecord cameraStart924;
    BackgroundStageCameraRecord cameraTangentEndA14;
    BackgroundStageCameraRecord cameraTangentStartB04;

    int interpolationDurationBF4[5];
    ZunTimer interpolationTimersC08[5];
    int interpolationModesC44[5];

    ZunTimer stageScriptTimerC58;
    int stageScriptInstructionIndexC64;
    int frameCounterC68;
    int unknownC6C;
    Float3 stagePositionC70;
    unsigned int clearColorC7C;
    unsigned char unknownC80[0xC94 - 0xC80];
    BackgroundFogProbe skyFogC94;
    BackgroundFogProbe skyFogInitialCA0;
    BackgroundFogProbe skyFogFinalCAC;
    int skyFogDurationCB8;
    ZunTimer skyFogTimerCBC;

    unsigned char unknownCC8[0x6408 - 0xCC8];
    Float3 stagePositionTarget6408;
    int stagePositionEndFrame6414;
    Float3 stagePositionInitial6418;
    int stagePositionStartFrame6424;
    unsigned char compensateCameraJump6428;
    unsigned char unknown6429[3];
    unsigned int tintColor642C;
    int retainTint6430;
    float cullingDistanceSq6434;
    unsigned char cameraMotionMode6438;
};

typedef char BackgroundStageScriptAt28[
    offsetof(BackgroundStageView, stageScript028) == 0x28 ? 1 : -1];
typedef char BackgroundStageAnmAt818[
    offsetof(BackgroundStageView, stageAnm818) == 0x818 ? 1 : -1];
typedef char BackgroundStageTargetCameraAt834[
    offsetof(BackgroundStageView, cameraTarget834) == 0x834 ? 1 : -1];
typedef char BackgroundStageStartCameraAt924[
    offsetof(BackgroundStageView, cameraStart924) == 0x924 ? 1 : -1];
typedef char BackgroundStageTangentEndAtA14[
    offsetof(BackgroundStageView, cameraTangentEndA14) == 0xA14 ? 1 : -1];
typedef char BackgroundStageTangentStartAtB04[
    offsetof(BackgroundStageView, cameraTangentStartB04) == 0xB04 ? 1 : -1];
typedef char BackgroundStageDurationAtBF4[
    offsetof(BackgroundStageView, interpolationDurationBF4) == 0xBF4 ? 1 : -1];
typedef char BackgroundStageTimerAtC08[
    offsetof(BackgroundStageView, interpolationTimersC08) == 0xC08 ? 1 : -1];
typedef char BackgroundStageModeAtC44[
    offsetof(BackgroundStageView, interpolationModesC44) == 0xC44 ? 1 : -1];
typedef char BackgroundStageScriptTimerAtC58[
    offsetof(BackgroundStageView, stageScriptTimerC58) == 0xC58 ? 1 : -1];
typedef char BackgroundStageScriptIndexAtC64[
    offsetof(BackgroundStageView, stageScriptInstructionIndexC64) == 0xC64 ? 1 : -1];
typedef char BackgroundStageFogAtC94[
    offsetof(BackgroundStageView, skyFogC94) == 0xC94 ? 1 : -1];
typedef char BackgroundStagePositionTargetAt6408[
    offsetof(BackgroundStageView, stagePositionTarget6408) == 0x6408 ? 1 : -1];
typedef char BackgroundStageMotionAt6438[
    offsetof(BackgroundStageView, cameraMotionMode6438) == 0x6438 ? 1 : -1];

// The background target/start/tangent records belong to the Background
// allocation, while live viewport state resides in native Supervisor.
float __stdcall CubicHermiteInterpolateScalar(
    float startValue,
    float endValue,
    float startTangent,
    float endTangent,
    float time);


unsigned int ZunTimer::operator<(int value)
{
    return this->current < value;
}

float __stdcall CubicHermiteInterpolateScalar(
    float startValue,
    float endValue,
    float startTangent,
    float endTangent,
    float time)
{
    float startWeight =
        (time - 1.0f) * (time - 1.0f) * (2.0f * time + 1.0f);
    float endWeight = time * time * (3.0f - 2.0f * time);
    float startTangentWeight = (1.0f - time) * (1.0f - time) * time;
    float endTangentWeight = (time - 1.0f) * time * time;
    return startWeight * startValue + endWeight * endValue +
           startTangentWeight * startTangent +
           endTangentWeight * endTangent;
}

// A lexical view of the three native Background interpolation arrays.
// Preserve the original call/slot lvalues under the /Ob0 owner profile.
#define TH09_BG_NATIVE_SLOT(array, elementType, index) \
    (*reinterpret_cast<elementType *>( \
        reinterpret_cast<unsigned int>(&(array)[0]) + \
        static_cast<unsigned int>(index) * sizeof((array)[0])))

static void InterpolateBackgroundCameraVector(
    BackgroundStageView *background,
    int index,
    Float3 *out,
    const Float3 *start,
    const Float3 *end,
    const Float3 *startTangent,
    const Float3 *endTangent)
{
    float time;

    if (TH09_BG_NATIVE_SLOT(background->interpolationTimersC08, ZunTimer, index) <
        TH09_BG_NATIVE_SLOT(background->interpolationDurationBF4, int, index))
    {
        TH09_BG_NATIVE_SLOT(background->interpolationTimersC08, ZunTimer, index)++;
        time = (float)TH09_BG_NATIVE_SLOT(background->interpolationTimersC08, ZunTimer, index) /
               TH09_BG_NATIVE_SLOT(background->interpolationDurationBF4, int, index);
    }
    else
    {
        TH09_BG_NATIVE_SLOT(background->interpolationTimersC08, ZunTimer, index) =
            TH09_BG_NATIVE_SLOT(background->interpolationDurationBF4, int, index);
        time = 1.0f;
        TH09_BG_NATIVE_SLOT(background->interpolationDurationBF4, int, index) = 0;
    }

    switch (TH09_BG_NATIVE_SLOT(background->interpolationModesC44, int, index))
    {
    case 1:
        time = 1.0f - time;
        time = 1.0f - time * time;
        break;
    case 2:
        time = 1.0f - time;
        time = 1.0f - time * time * time;
        break;
    case 3:
        time = 1.0f - time;
        time = 1.0f - time * time * time * time;
        break;
    case 4:
        time = time * time;
        break;
    case 5:
        time = time * time * time;
        break;
    case 6:
        time = time * time * time * time;
        break;
    }

    if (TH09_BG_NATIVE_SLOT(background->interpolationModesC44, int, index) != 7)
    {
        *out = *end - *start;
        *out = (*out * time) + *start;
    }
    else
    {
        out->x = CubicHermiteInterpolateScalar(
            start->x, end->x, startTangent->x, endTangent->x, time);
        out->y = CubicHermiteInterpolateScalar(
            start->y, end->y, startTangent->y, endTangent->y, time);
        out->z = CubicHermiteInterpolateScalar(
            start->z, end->z, startTangent->z, endTangent->z, time);
    }
}

#undef TH09_BG_NATIVE_SLOT

// All 19 live stage-camera selectors target the same three original
// 0xF0-byte Supervisor viewport records as exact viewport/routing code.
// Form the native Win32 signed-DWORD address without a bounded array view.
#define TH09_BG_NATIVE_CAMERA(owner) \
    (*reinterpret_cast<BackgroundStageCameraRecord *>( \
        reinterpret_cast<unsigned int>(&g_Supervisor.ctorArray078[0]) + \
        static_cast<unsigned int>((owner)->viewportIndex00C) * \
            sizeof(g_Supervisor.ctorArray078[0])))

void __fastcall BackgroundRunStageScriptPhase(Background *backgroundOwner)
{
    BackgroundStageView *background =
        reinterpret_cast<BackgroundStageView *>(backgroundOwner);
    D3DXVECTOR3 position;
    RawStageInstrProbe *instruction;

read_instruction:
    // Native stage instructions use a 0x14-byte signed wire index scaled as
    // an unsigned i386 word, without a C++ pointer-in-array precondition.
    instruction = reinterpret_cast<RawStageInstrProbe *>(
        reinterpret_cast<unsigned int>(background->stageScript028) +
        static_cast<unsigned int>(background->stageScriptInstructionIndexC64) *
            sizeof(RawStageInstrProbe));
    if (background->stageScriptTimerC58 >= instruction->frame)
    {
        if (instruction->frame == -1)
            goto script_done;

        switch (instruction->opcode)
        {
        case 0:
            position = *reinterpret_cast<D3DXVECTOR3 *>(instruction->args);
            background->stagePositionC70.x = position.x;
            background->stagePositionC70.y = position.y;
            background->stagePositionC70.z = position.z;
            background->stagePositionInitial6418 =
                *reinterpret_cast<Float3 *>(&position);
            background->stagePositionStartFrame6424 = instruction->frame;
            // The following camera keyframe is the next physical 0x14-byte
            // record in the original file-backed instruction stream.
            // Advance its native i386 byte address, not a bounded C++ array
            // pointer to a single projected instruction object.
            instruction = reinterpret_cast<RawStageInstrProbe *>(
                reinterpret_cast<unsigned int>(instruction) +
                sizeof(RawStageInstrProbe));
            background->stagePositionEndFrame6414 = instruction->frame;
            background->stagePositionTarget6408 =
                *reinterpret_cast<Float3 *>(instruction->args);
            break;

        case 1:
            background->skyFogC94.color =
                static_cast<unsigned int>(instruction->args[0]);
            background->skyFogC94.nearPlane =
                *reinterpret_cast<float *>(&instruction->args[1]);
            background->skyFogC94.farPlane =
                *reinterpret_cast<float *>(&instruction->args[2]);
            background->skyFogFinalCAC = background->skyFogC94;
            break;

        case 2:
            background->skyFogInitialCA0 = background->skyFogC94;
            background->skyFogDurationCB8 = instruction->args[0];
            background->skyFogTimerCBC = 0;
            break;

        case 5:
            if (background->compensateCameraJump6428 != 0)
                background->compensateCameraJump6428 = 0;
            background->cameraStart924.cameraPosition00 =
                background->cameraTarget834.cameraPosition00;
            background->cameraTarget834.cameraPosition00 =
                *reinterpret_cast<Float3 *>(instruction->args);
            if (background->interpolationDurationBF4[0] == 0)
                TH09_BG_NATIVE_CAMERA(background)
                    .cameraPosition00 =
                    *reinterpret_cast<Float3 *>(instruction->args);
            break;

        case 6:
            background->interpolationDurationBF4[0] = instruction->args[0];
            background->interpolationTimersC08[0] = 0;
            background->interpolationModesC44[0] = instruction->args[1];
            break;

        case 7:
            background->cameraStart924.cameraLookAtOffset0C =
                background->cameraTarget834.cameraLookAtOffset0C;
            background->cameraTarget834.cameraLookAtOffset0C =
                *reinterpret_cast<Float3 *>(instruction->args);
            if (background->interpolationDurationBF4[1] == 0)
                TH09_BG_NATIVE_CAMERA(background)
                    .cameraLookAtOffset0C =
                    *reinterpret_cast<Float3 *>(instruction->args);
            break;

        case 8:
            background->interpolationDurationBF4[1] = instruction->args[0];
            background->interpolationTimersC08[1] = 0;
            background->interpolationModesC44[1] = instruction->args[1];
            break;

        case 9:
            background->cameraStart924.cameraUp18 =
                background->cameraTarget834.cameraUp18;
            background->cameraTarget834.cameraUp18 =
                *reinterpret_cast<Float3 *>(instruction->args);
            if (background->interpolationDurationBF4[2] == 0)
                TH09_BG_NATIVE_CAMERA(background).cameraUp18 =
                    *reinterpret_cast<Float3 *>(instruction->args);
            break;

        case 10:
            background->interpolationDurationBF4[2] = instruction->args[0];
            background->interpolationModesC44[2] = instruction->args[1];
            background->interpolationTimersC08[2] = 0;
            break;

        case 11:
            background->cameraStart924.fieldOfView48 =
                background->cameraTarget834.fieldOfView48;
            background->cameraTarget834.fieldOfView48 =
                *reinterpret_cast<float *>(&instruction->args[0]);
            if (background->interpolationDurationBF4[3] == 0)
                TH09_BG_NATIVE_CAMERA(background)
                    .fieldOfView48 =
                    *reinterpret_cast<float *>(&instruction->args[0]);
            break;

        case 12:
            background->interpolationDurationBF4[3] = instruction->args[0];
            background->interpolationTimersC08[3] = 0;
            background->interpolationModesC44[3] = instruction->args[1];
            break;

        case 13:
            background->clearColorC7C =
                static_cast<unsigned int>(instruction->args[0]);
            break;

        case 3:
            if (background->pendingStageLabel018 == 0)
                goto script_done;
            background->pendingStageLabel018 = 0;
            break;

        case 4:
            background->stageScriptInstructionIndexC64 = instruction->args[0];
            background->stageScriptTimerC58 = instruction->args[1];
            background->interpolationDurationBF4[0] = 0;
            background->compensateCameraJump6428 = 1;
            goto script_done;

        case 14:
            background->cameraStart924.cameraPosition00 =
                *reinterpret_cast<Float3 *>(instruction->args);
            break;

        case 15:
            background->cameraTarget834.cameraPosition00 =
                *reinterpret_cast<Float3 *>(instruction->args);
            break;

        case 16:
            background->cameraTangentStartB04.cameraPosition00 =
                *reinterpret_cast<Float3 *>(instruction->args);
            break;

        case 17:
            background->cameraTangentEndA14.cameraPosition00 =
                *reinterpret_cast<Float3 *>(instruction->args);
            break;

        case 18:
            background->interpolationDurationBF4[0] = instruction->args[0];
            background->interpolationTimersC08[0] = 0;
            background->interpolationModesC44[0] = 7;
            break;

        case 19:
            background->cameraStart924.cameraLookAtOffset0C =
                *reinterpret_cast<Float3 *>(instruction->args);
            break;

        case 20:
            background->cameraTarget834.cameraLookAtOffset0C =
                *reinterpret_cast<Float3 *>(instruction->args);
            break;

        case 21:
            background->cameraTangentStartB04.cameraLookAtOffset0C =
                *reinterpret_cast<Float3 *>(instruction->args);
            break;

        case 22:
            background->cameraTangentEndA14.cameraLookAtOffset0C =
                *reinterpret_cast<Float3 *>(instruction->args);
            break;

        case 23:
            background->interpolationDurationBF4[1] = instruction->args[0];
            background->interpolationTimersC08[1] = 0;
            background->interpolationModesC44[1] = 7;
            break;

        case 24:
            background->cameraStart924.cameraUp18 =
                *reinterpret_cast<Float3 *>(instruction->args);
            break;

        case 25:
            background->cameraTarget834.cameraUp18 =
                *reinterpret_cast<Float3 *>(instruction->args);
            break;

        case 26:
            background->cameraTangentStartB04.cameraUp18 =
                *reinterpret_cast<Float3 *>(instruction->args);
            break;

        case 27:
            background->cameraTangentEndA14.cameraUp18 =
                *reinterpret_cast<Float3 *>(instruction->args);
            break;

        case 28:
            background->interpolationDurationBF4[2] = instruction->args[0];
            background->interpolationTimersC08[2] = 0;
            background->interpolationModesC44[2] = 7;
            break;

        case 29:
            if (instruction->args[0] >= 0)
                background->stageAnm818->ExecuteAnmIdx(
                    &background->stageVm0_02C, instruction->args[0]);
            else
                background->stageVm0_02C.activeSpriteIndex = -1;
            break;

        case 30:
            if (instruction->args[0] >= 0)
                background->stageAnm818->ExecuteAnmIdx(
                    &background->stageVm1_2D0, instruction->args[0]);
            else
                background->stageVm0_02C.activeSpriteIndex = -1;
            break;

        case 33:
            background->cameraMotionMode6438 =
                *reinterpret_cast<unsigned char *>(&instruction->args[0]);
            background->interpolationDurationBF4[4] = 0;
            background->interpolationTimersC08[4] = 0;
            background->interpolationModesC44[4] = 0;
            break;

        case 32:
            TH09_BG_NATIVE_CAMERA(background)
                .cameraPositionOffset3C =
                *reinterpret_cast<Float3 *>(instruction->args);
            break;

        case 34:
            if (instruction->args[0] >= 0)
                background->stageAnm818->ExecuteAnmIdx(
                    &background->stageVm2_574, instruction->args[0]);
            else
                background->stageVm2_574.activeSpriteIndex = -1;
            break;

        case 31:
            break;
        }

        // Retail advances this signed wire index with an i386 INC DWORD.
        // Preserve its modulo-32 result instead of signed C++ overflow.
        ++*reinterpret_cast<unsigned int *>(
            &background->stageScriptInstructionIndexC64);
        goto read_instruction;
    }

script_done:
    if (background->interpolationDurationBF4[0] != 0)
        InterpolateBackgroundCameraVector(
            background,
            0,
            &TH09_BG_NATIVE_CAMERA(background).cameraPosition00,
            &background->cameraStart924.cameraPosition00,
            &background->cameraTarget834.cameraPosition00,
            &background->cameraTangentStartB04.cameraPosition00,
            &background->cameraTangentEndA14.cameraPosition00);

    if (background->interpolationDurationBF4[1] != 0)
        InterpolateBackgroundCameraVector(
            background,
            1,
            &TH09_BG_NATIVE_CAMERA(background).cameraLookAtOffset0C,
            &background->cameraStart924.cameraLookAtOffset0C,
            &background->cameraTarget834.cameraLookAtOffset0C,
            &background->cameraTangentStartB04.cameraLookAtOffset0C,
            &background->cameraTangentEndA14.cameraLookAtOffset0C);

    if (background->interpolationDurationBF4[2] != 0)
        InterpolateBackgroundCameraVector(
            background,
            2,
            &TH09_BG_NATIVE_CAMERA(background).cameraUp18,
            &background->cameraStart924.cameraUp18,
            &background->cameraTarget834.cameraUp18,
            &background->cameraTangentStartB04.cameraUp18,
            &background->cameraTangentEndA14.cameraUp18);

    if (background->interpolationDurationBF4[3] != 0)
    {
        float time;
        if (background->interpolationTimersC08[3] <
            background->interpolationDurationBF4[3])
        {
            background->interpolationTimersC08[3]++;
            time = (float)background->interpolationTimersC08[3] /
                   background->interpolationDurationBF4[3];
        }
        else
        {
            background->interpolationTimersC08[3] =
                background->interpolationDurationBF4[3];
            time = 1.0f;
            background->interpolationDurationBF4[3] = 0;
        }

        switch (background->interpolationModesC44[3])
        {
        case 1:
            time = 1.0f - time;
            time = 1.0f - time * time;
            break;
        case 2:
            time = 1.0f - time;
            time = 1.0f - time * time * time;
            break;
        case 3:
            time = 1.0f - time;
            time = 1.0f - time * time * time * time;
            break;
        case 4:
            time = time * time;
            break;
        case 5:
            time = time * time * time;
            break;
        case 6:
            time = time * time * time * time;
            break;
        }

        TH09_BG_NATIVE_CAMERA(background).fieldOfView48 =
            (background->cameraTarget834.fieldOfView48 -
             background->cameraStart924.fieldOfView48) *
                time +
            background->cameraStart924.fieldOfView48;
    }

    D3DXVec3Normalize(
        reinterpret_cast<D3DXVECTOR3 *>(
            &TH09_BG_NATIVE_CAMERA(background).cameraForward24),
        reinterpret_cast<D3DXVECTOR3 *>(
            &TH09_BG_NATIVE_CAMERA(background).cameraLookAtOffset0C));

    if (background->cameraMotionMode6438 != 0)
    {
        switch (background->cameraMotionMode6438)
        {
        case 1:
        {
            TH09_BG_NATIVE_CAMERA(background)
                .cameraPositionOffset3C.x = sinf(
                    (float)background->interpolationTimersC08[4] *
                        0.01308997f - 3.1415927f) * 40.0f;
            background->interpolationTimersC08[4]++;
            if (background->interpolationTimersC08[4] >= 480)
                background->interpolationTimersC08[4] = 0;
            break;
        }
        case 2:
        {
            float angle =
                (float)background->interpolationTimersC08[4] * 0.01308997f -
                3.1415927f;
            TH09_BG_NATIVE_CAMERA(background)
                .cameraPositionOffset3C.x = sinf(angle) * 70.0f;
            TH09_BG_NATIVE_CAMERA(background).cameraUp18.x =
                sinf(angle) * -0.1f;
            background->interpolationTimersC08[4]++;
            if (background->interpolationTimersC08[4] >= 480)
                background->interpolationTimersC08[4] = 0;
            break;
        }
        case 4:
        {
            float angle =
                (float)background->interpolationTimersC08[4] * 0.0030679617f -
                3.1415927f;
            TH09_BG_NATIVE_CAMERA(background)
                .cameraPositionOffset3C.x = sinf(angle) * 30.0f;
            TH09_BG_NATIVE_CAMERA(background)
                .cameraPositionOffset3C.z = cosf(angle) * 30.0f;
            TH09_BG_NATIVE_CAMERA(background).cameraUp18.x =
                sinf(angle) * -0.1f;
            background->interpolationTimersC08[4]++;
            if (background->interpolationTimersC08[4] >= 2048)
                background->interpolationTimersC08[4] = 0;
            break;
        }
        case 3:
        {
            float angle =
                (float)background->interpolationTimersC08[4] * 0.0013089969f -
                3.1415927f;
            TH09_BG_NATIVE_CAMERA(background).cameraUp18.x =
                sinf(angle);
            TH09_BG_NATIVE_CAMERA(background).cameraUp18.z =
                cosf(angle);
            background->interpolationTimersC08[4]++;
            if (background->interpolationTimersC08[4] >= 4800)
                background->interpolationTimersC08[4] = 0;
            break;
        }
        }

    }

    if (background->skyFogDurationCB8 != 0)
    {
        background->skyFogTimerCBC++;
        float fogRatio =
            (float)background->skyFogTimerCBC / background->skyFogDurationCB8;
        if (fogRatio >= 1.0f)
            fogRatio = 1.0f;

        for (int i = 0; i < 4; ++i)
            reinterpret_cast<unsigned char *>(&background->skyFogC94.color)[i] =
                static_cast<unsigned char>(
                    ((float)reinterpret_cast<unsigned char *>(
                         &background->skyFogFinalCAC.color)[i] -
                     (float)reinterpret_cast<unsigned char *>(
                         &background->skyFogInitialCA0.color)[i]) *
                        fogRatio +
                    (float)reinterpret_cast<unsigned char *>(
                        &background->skyFogInitialCA0.color)[i]);

        background->skyFogC94.nearPlane =
            (background->skyFogFinalCAC.nearPlane -
             background->skyFogInitialCA0.nearPlane) *
                fogRatio +
            background->skyFogInitialCA0.nearPlane;
        background->skyFogC94.farPlane =
            (background->skyFogFinalCAC.farPlane -
             background->skyFogInitialCA0.farPlane) *
                fogRatio +
            background->skyFogInitialCA0.farPlane;

        if (background->skyFogTimerCBC >= background->skyFogDurationCB8)
            background->skyFogDurationCB8 = 0;
    }
}

#undef TH09_BG_NATIVE_CAMERA
