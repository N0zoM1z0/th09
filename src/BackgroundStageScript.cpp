// Target-bound reconstruction of TH09's stage-script/camera phase at
// 0x004018F0 and its file-local interpolation helper at 0x004016E0.
//
// The private views below expose only offsets independently observed in TH09.
// They deliberately do not claim original class/member spelling. The current
// natural VC7.1 source is source-present but non-exact: the strongest bounded
// /O2 /Ob0 /Oy- form has the target 0x14 stack frame and complete semantics,
// while the remaining frontier is preserved-register/private-helper allocation.

#include "AsciiManager.hpp"
#include "AnmManager.hpp"
#include "ZunTimer.hpp"

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

struct BackgroundStageCameraRecord
{
    Float3 position00;
    Float3 lookAt0C;
    Float3 up18;
    Float3 forward24;
    Float3 right30;
    Float3 positionOffset3C;
    float fieldOfView48;
    unsigned char unknown04C[0xF0 - 0x4C];
};
typedef char BackgroundStageCameraRecordSizeIsF0[
    sizeof(BackgroundStageCameraRecord) == 0xF0 ? 1 : -1];

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

struct BackgroundStageSupervisorView
{
    unsigned char unknown000[0x78];
    BackgroundStageCameraRecord viewports078[3];
};

typedef char BackgroundStageSupervisorViewportsAt78[
    offsetof(BackgroundStageSupervisorView, viewports078) == 0x78 ? 1 : -1];

extern BackgroundStageSupervisorView g_Supervisor;

#define g_BackgroundStageViewports (g_Supervisor.viewports078)

float __stdcall CubicHermiteInterpolateScalar(
    float startValue,
    float endValue,
    float startTangent,
    float endTangent,
    float time);

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

    if (background->interpolationTimersC08[index] <
        background->interpolationDurationBF4[index])
    {
        background->interpolationTimersC08[index]++;
        time = (float)background->interpolationTimersC08[index] /
               background->interpolationDurationBF4[index];
    }
    else
    {
        background->interpolationTimersC08[index] =
            background->interpolationDurationBF4[index];
        time = 1.0f;
        background->interpolationDurationBF4[index] = 0;
    }

    switch (background->interpolationModesC44[index])
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

    if (background->interpolationModesC44[index] != 7)
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

void __fastcall BackgroundRunStageScriptPhase(Background *backgroundOwner)
{
    BackgroundStageView *background =
        reinterpret_cast<BackgroundStageView *>(backgroundOwner);
    D3DXVECTOR3 position;
    RawStageInstrProbe *instruction =
        background->stageScript028 + background->stageScriptInstructionIndexC64;

    while (background->stageScriptTimerC58 >= instruction->frame)
    {
        if (instruction->frame == -1)
            break;

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
            ++instruction;
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

        case 5:
            if (background->compensateCameraJump6428 != 0)
                background->compensateCameraJump6428 = 0;
            background->cameraStart924.position00 =
                background->cameraTarget834.position00;
            background->cameraTarget834.position00 =
                *reinterpret_cast<Float3 *>(instruction->args);
            if (background->interpolationDurationBF4[0] == 0)
                g_BackgroundStageViewports[background->viewportIndex00C]
                    .position00 =
                    *reinterpret_cast<Float3 *>(instruction->args);
            break;

        case 6:
            background->interpolationDurationBF4[0] = instruction->args[0];
            background->interpolationTimersC08[0] = 0;
            background->interpolationModesC44[0] = instruction->args[1];
            break;

        case 7:
            background->cameraStart924.lookAt0C =
                background->cameraTarget834.lookAt0C;
            background->cameraTarget834.lookAt0C =
                *reinterpret_cast<Float3 *>(instruction->args);
            if (background->interpolationDurationBF4[1] == 0)
                g_BackgroundStageViewports[background->viewportIndex00C]
                    .lookAt0C =
                    *reinterpret_cast<Float3 *>(instruction->args);
            break;

        case 8:
            background->interpolationDurationBF4[1] = instruction->args[0];
            background->interpolationTimersC08[1] = 0;
            background->interpolationModesC44[1] = instruction->args[1];
            break;

        case 9:
            background->cameraStart924.up18 =
                background->cameraTarget834.up18;
            background->cameraTarget834.up18 =
                *reinterpret_cast<Float3 *>(instruction->args);
            if (background->interpolationDurationBF4[2] == 0)
                g_BackgroundStageViewports[background->viewportIndex00C].up18 =
                    *reinterpret_cast<Float3 *>(instruction->args);
            break;

        case 10:
            background->interpolationDurationBF4[2] = instruction->args[0];
            background->interpolationTimersC08[2] = 0;
            background->interpolationModesC44[2] = instruction->args[1];
            break;

        case 11:
            background->cameraStart924.fieldOfView48 =
                background->cameraTarget834.fieldOfView48;
            background->cameraTarget834.fieldOfView48 =
                *reinterpret_cast<float *>(&instruction->args[0]);
            if (background->interpolationDurationBF4[3] == 0)
                g_BackgroundStageViewports[background->viewportIndex00C]
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

        case 14:
            background->cameraStart924.position00 =
                *reinterpret_cast<Float3 *>(instruction->args);
            break;
        case 15:
            background->cameraTarget834.position00 =
                *reinterpret_cast<Float3 *>(instruction->args);
            break;
        case 16:
            background->cameraTangentStartB04.position00 =
                *reinterpret_cast<Float3 *>(instruction->args);
            break;
        case 17:
            background->cameraTangentEndA14.position00 =
                *reinterpret_cast<Float3 *>(instruction->args);
            break;
        case 18:
            background->interpolationDurationBF4[0] = instruction->args[0];
            background->interpolationTimersC08[0] = 0;
            background->interpolationModesC44[0] = 7;
            break;

        case 19:
            background->cameraStart924.lookAt0C =
                *reinterpret_cast<Float3 *>(instruction->args);
            break;
        case 20:
            background->cameraTarget834.lookAt0C =
                *reinterpret_cast<Float3 *>(instruction->args);
            break;
        case 21:
            background->cameraTangentStartB04.lookAt0C =
                *reinterpret_cast<Float3 *>(instruction->args);
            break;
        case 22:
            background->cameraTangentEndA14.lookAt0C =
                *reinterpret_cast<Float3 *>(instruction->args);
            break;
        case 23:
            background->interpolationDurationBF4[1] = instruction->args[0];
            background->interpolationTimersC08[1] = 0;
            background->interpolationModesC44[1] = 7;
            break;

        case 24:
            background->cameraStart924.up18 =
                *reinterpret_cast<Float3 *>(instruction->args);
            break;
        case 25:
            background->cameraTarget834.up18 =
                *reinterpret_cast<Float3 *>(instruction->args);
            break;
        case 26:
            background->cameraTangentStartB04.up18 =
                *reinterpret_cast<Float3 *>(instruction->args);
            break;
        case 27:
            background->cameraTangentEndA14.up18 =
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

        case 31:
            break;

        case 32:
            g_BackgroundStageViewports[background->viewportIndex00C]
                .positionOffset3C =
                *reinterpret_cast<Float3 *>(instruction->args);
            break;

        case 33:
            background->cameraMotionMode6438 =
                *reinterpret_cast<unsigned char *>(&instruction->args[0]);
            background->interpolationDurationBF4[4] = 0;
            background->interpolationTimersC08[4] = 0;
            background->interpolationModesC44[4] = 0;
            break;

        case 34:
            if (instruction->args[0] >= 0)
                background->stageAnm818->ExecuteAnmIdx(
                    &background->stageVm2_574, instruction->args[0]);
            else
                background->stageVm2_574.activeSpriteIndex = -1;
            break;
        }

        ++background->stageScriptInstructionIndexC64;
        instruction = background->stageScript028 +
                      background->stageScriptInstructionIndexC64;
    }

script_done:
    if (background->interpolationDurationBF4[0] != 0)
        InterpolateBackgroundCameraVector(
            background,
            0,
            &g_BackgroundStageViewports[background->viewportIndex00C].position00,
            &background->cameraStart924.position00,
            &background->cameraTarget834.position00,
            &background->cameraTangentStartB04.position00,
            &background->cameraTangentEndA14.position00);

    if (background->interpolationDurationBF4[1] != 0)
        InterpolateBackgroundCameraVector(
            background,
            1,
            &g_BackgroundStageViewports[background->viewportIndex00C].lookAt0C,
            &background->cameraStart924.lookAt0C,
            &background->cameraTarget834.lookAt0C,
            &background->cameraTangentStartB04.lookAt0C,
            &background->cameraTangentEndA14.lookAt0C);

    if (background->interpolationDurationBF4[2] != 0)
        InterpolateBackgroundCameraVector(
            background,
            2,
            &g_BackgroundStageViewports[background->viewportIndex00C].up18,
            &background->cameraStart924.up18,
            &background->cameraTarget834.up18,
            &background->cameraTangentStartB04.up18,
            &background->cameraTangentEndA14.up18);

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

        g_BackgroundStageViewports[background->viewportIndex00C].fieldOfView48 =
            (background->cameraTarget834.fieldOfView48 -
             background->cameraStart924.fieldOfView48) *
                time +
            background->cameraStart924.fieldOfView48;
    }

    D3DXVec3Normalize(
        reinterpret_cast<D3DXVECTOR3 *>(
            &g_BackgroundStageViewports[background->viewportIndex00C].forward24),
        reinterpret_cast<D3DXVECTOR3 *>(
            &g_BackgroundStageViewports[background->viewportIndex00C].lookAt0C));

    switch (background->cameraMotionMode6438)
    {
    case 1:
    {
        float angle =
            (float)background->interpolationTimersC08[4] * 0.01308997f -
            3.1415927f;
        g_BackgroundStageViewports[background->viewportIndex00C]
            .positionOffset3C.x = sinf(angle) * 40.0f;
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
        g_BackgroundStageViewports[background->viewportIndex00C]
            .positionOffset3C.x = sinf(angle) * 70.0f;
        g_BackgroundStageViewports[background->viewportIndex00C].up18.x =
            sinf(angle) * -0.1f;
        background->interpolationTimersC08[4]++;
        if (background->interpolationTimersC08[4] >= 480)
            background->interpolationTimersC08[4] = 0;
        break;
    }
    case 3:
    {
        float angle =
            (float)background->interpolationTimersC08[4] * 0.0013089969f -
            3.1415927f;
        g_BackgroundStageViewports[background->viewportIndex00C].up18.x =
            sinf(angle);
        g_BackgroundStageViewports[background->viewportIndex00C].up18.z =
            cosf(angle);
        background->interpolationTimersC08[4]++;
        if (background->interpolationTimersC08[4] >= 4800)
            background->interpolationTimersC08[4] = 0;
        break;
    }
    case 4:
    {
        float angle =
            (float)background->interpolationTimersC08[4] * 0.0030679617f -
            3.1415927f;
        g_BackgroundStageViewports[background->viewportIndex00C]
            .positionOffset3C.x = sinf(angle) * 30.0f;
        g_BackgroundStageViewports[background->viewportIndex00C]
            .positionOffset3C.z = cosf(angle) * 30.0f;
        g_BackgroundStageViewports[background->viewportIndex00C].up18.x =
            sinf(angle) * -0.1f;
        background->interpolationTimersC08[4]++;
        if (background->interpolationTimersC08[4] >= 2048)
            background->interpolationTimersC08[4] = 0;
        break;
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

#undef g_BackgroundStageViewports
