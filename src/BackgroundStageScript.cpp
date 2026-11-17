// Target-bound reconstruction of TH09's stage-script/camera phase at
// 0x004018F0 and its file-local interpolation helper at 0x004016E0.
//
// The stage script now uses the constructed Background allocation directly.
// The camera vector compatibility view below borrows the native camera prefix;
// original private source member spellings remain unknown. The pinned
// /O2 /Ob0 /Oy- source reproduces the complete interpolation helper.
// Both the 2472-byte stage-script owner and the 423-byte interpolation helper
// replay exactly with their complete compiler switch tables. Real same-TU
// timer comparisons and scalar Hermite visibility recover private transport.

#include "AsciiManager.hpp"
#include "Background.hpp"
#include "AnmManager.hpp"
#include "ZunTimer.hpp"
#include "Supervisor.hpp"

#include <stddef.h>
#include <math.h>
#include <windows.h>
#include <d3d8.h>

struct D3DXVECTOR3 : public D3DVECTOR {};
D3DXVECTOR3 *WINAPI D3DXVec3Normalize(
    D3DXVECTOR3 *out, const D3DXVECTOR3 *vector);

// Use the actual stage-file and fog records shared with Background.
typedef char BackgroundStageNativeWireRecords[
    (sizeof(RawStageInstr) == 0x14 &&
     offsetof(RawStageInstr, args) == 0x08 &&
     sizeof(BackgroundFog) == 0x0C) ? 1 : -1];

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
// Only the camera-vector prefix is borrowed; the Background's remainder
// remains its own native allocation, not an alias of Supervisor matrices.
typedef char BackgroundStageNativeCameraPrefixIsCompatible[
    (sizeof(BackgroundCameraRecord) == 0xF0 &&
     offsetof(BackgroundCameraRecord, position) == 0x00 &&
     offsetof(BackgroundCameraRecord, lookAt) == 0x0C &&
     offsetof(BackgroundCameraRecord, up) == 0x18 &&
     offsetof(BackgroundCameraRecord, forward) == 0x24 &&
     offsetof(BackgroundCameraRecord, right) == 0x30 &&
     offsetof(BackgroundCameraRecord, positionOffset) == 0x3C &&
     offsetof(BackgroundCameraRecord, fieldOfView) == 0x48) ? 1 : -1];

// The constructed Background allocation owns the stage script and camera.

typedef char BackgroundStageOwnerNativeOffsets[
    (sizeof(Background) == 0x65C4 &&
     offsetof(Background, stageScript) == 0x28 &&
     offsetof(Background, cameraTarget) == 0x834 &&
     offsetof(Background, cameraStart) == 0x924 &&
     offsetof(Background, interpolationDuration) == 0xBF4 &&
     offsetof(Background, interpolationTimers) == 0xC08 &&
     offsetof(Background, interpolationModes) == 0xC44 &&
     offsetof(Background, stageScriptTimer) == 0xC58 &&
     offsetof(Background, skyFog) == 0xC94 &&
     offsetof(Background, stagePositionTarget) == 0x6408 &&
     offsetof(Background, cameraMotionMode) == 0x6438) ? 1 : -1];

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

// The three interpolation groups are real Background-owned C++ arrays.
// The helper's index selects the same channel in each of the five slots.

static void InterpolateBackgroundCameraVector(
    Background *background,
    int index,
    Float3 *out,
    const Float3 *start,
    const Float3 *end,
    const Float3 *startTangent,
    const Float3 *endTangent)
{
    float time;

    if (background->interpolationTimers[index] <
        background->interpolationDuration[index])
    {
        background->interpolationTimers[index]++;
        time = (float)background->interpolationTimers[index] /
               background->interpolationDuration[index];
    }
    else
    {
        background->interpolationTimers[index] =
            background->interpolationDuration[index];
        time = 1.0f;
        background->interpolationDuration[index] = 0;
    }

    switch (background->interpolationModes[index])
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

    if (background->interpolationModes[index] != 7)
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


// The stage wire instruction has a native +0x08 payload. Decode its
// actual variable-length bytes as typed integer, float or vector data.
// No fixed-size surrogate argument array or extra bounds check is added.

// All camera selectors refer directly to the real 0xF0-byte
// Supervisor viewport records with their original unsigned indices.

void __fastcall BackgroundRunStageScriptPhase(Background *backgroundOwner)
{
    Background *background = backgroundOwner;
    D3DXVECTOR3 position;
    RawStageInstr *instruction;

read_instruction:
    // The original file contains consecutive 0x14-byte stage records;
    // retain the exact unsigned word index and live file-memory access.
    instruction = &background->stageScript[static_cast<unsigned int>(
        background->stageScriptInstructionIndex)];
    if (background->stageScriptTimer >= instruction->frame)
    {
        if (instruction->frame == -1)
            goto script_done;

        switch (instruction->opcode)
        {
        case 0:
            position = (*reinterpret_cast<D3DXVECTOR3 *>(reinterpret_cast<unsigned char *>(instruction) + offsetof(RawStageInstr, args)));
            background->stagePosition.x = position.x;
            background->stagePosition.y = position.y;
            background->stagePosition.z = position.z;
            reinterpret_cast<Float3 &>(background->stagePositionInitial) =
                *reinterpret_cast<Float3 *>(&position);
            background->stagePositionStartFrame = instruction->frame;
            // The next camera keyframe is one 0x14-byte wire record
            // later in the original file-backed script.
            instruction = reinterpret_cast<RawStageInstr *>(reinterpret_cast<unsigned char *>(instruction) + sizeof(RawStageInstr));
            background->stagePositionEndFrame = instruction->frame;
            reinterpret_cast<Float3 &>(background->stagePositionTarget) =
                (*reinterpret_cast<Float3 *>(reinterpret_cast<unsigned char *>(instruction) + offsetof(RawStageInstr, args)));
            break;

        case 1:
            background->skyFog.color =
                static_cast<unsigned int>((reinterpret_cast<int *>(reinterpret_cast<unsigned char *>(instruction) + offsetof(RawStageInstr, args))[0]));
            background->skyFog.nearPlane =
                (reinterpret_cast<float *>(reinterpret_cast<unsigned char *>(instruction) + offsetof(RawStageInstr, args))[1]);
            background->skyFog.farPlane =
                (reinterpret_cast<float *>(reinterpret_cast<unsigned char *>(instruction) + offsetof(RawStageInstr, args))[2]);
            background->skyFogFinal = background->skyFog;
            break;

        case 2:
            background->skyFogInitial = background->skyFog;
            background->skyFogInterpolationDuration = (reinterpret_cast<int *>(reinterpret_cast<unsigned char *>(instruction) + offsetof(RawStageInstr, args))[0]);
            background->skyFogInterpolationTimer = 0;
            break;

        case 5:
            if (background->compensateCameraJump != 0)
                background->compensateCameraJump = 0;
            reinterpret_cast<BackgroundStageCameraRecord &>(background->cameraStart).cameraPosition00 =
                reinterpret_cast<BackgroundStageCameraRecord &>(background->cameraTarget).cameraPosition00;
            reinterpret_cast<BackgroundStageCameraRecord &>(background->cameraTarget).cameraPosition00 =
                (*reinterpret_cast<Float3 *>(reinterpret_cast<unsigned char *>(instruction) + offsetof(RawStageInstr, args)));
            if (background->interpolationDuration[0] == 0)
                (g_Supervisor.ctorArray078[static_cast<unsigned int>((background)->viewportIndex)])
                    .cameraPosition00 =
                    (*reinterpret_cast<Float3 *>(reinterpret_cast<unsigned char *>(instruction) + offsetof(RawStageInstr, args)));
            break;

        case 6:
            background->interpolationDuration[0] = (reinterpret_cast<int *>(reinterpret_cast<unsigned char *>(instruction) + offsetof(RawStageInstr, args))[0]);
            background->interpolationTimers[0] = 0;
            background->interpolationModes[0] = (reinterpret_cast<int *>(reinterpret_cast<unsigned char *>(instruction) + offsetof(RawStageInstr, args))[1]);
            break;

        case 7:
            reinterpret_cast<BackgroundStageCameraRecord &>(background->cameraStart).cameraLookAtOffset0C =
                reinterpret_cast<BackgroundStageCameraRecord &>(background->cameraTarget).cameraLookAtOffset0C;
            reinterpret_cast<BackgroundStageCameraRecord &>(background->cameraTarget).cameraLookAtOffset0C =
                (*reinterpret_cast<Float3 *>(reinterpret_cast<unsigned char *>(instruction) + offsetof(RawStageInstr, args)));
            if (background->interpolationDuration[1] == 0)
                (g_Supervisor.ctorArray078[static_cast<unsigned int>((background)->viewportIndex)])
                    .cameraLookAtOffset0C =
                    (*reinterpret_cast<Float3 *>(reinterpret_cast<unsigned char *>(instruction) + offsetof(RawStageInstr, args)));
            break;

        case 8:
            background->interpolationDuration[1] = (reinterpret_cast<int *>(reinterpret_cast<unsigned char *>(instruction) + offsetof(RawStageInstr, args))[0]);
            background->interpolationTimers[1] = 0;
            background->interpolationModes[1] = (reinterpret_cast<int *>(reinterpret_cast<unsigned char *>(instruction) + offsetof(RawStageInstr, args))[1]);
            break;

        case 9:
            reinterpret_cast<BackgroundStageCameraRecord &>(background->cameraStart).cameraUp18 =
                reinterpret_cast<BackgroundStageCameraRecord &>(background->cameraTarget).cameraUp18;
            reinterpret_cast<BackgroundStageCameraRecord &>(background->cameraTarget).cameraUp18 =
                (*reinterpret_cast<Float3 *>(reinterpret_cast<unsigned char *>(instruction) + offsetof(RawStageInstr, args)));
            if (background->interpolationDuration[2] == 0)
                (g_Supervisor.ctorArray078[static_cast<unsigned int>((background)->viewportIndex)]).cameraUp18 =
                    (*reinterpret_cast<Float3 *>(reinterpret_cast<unsigned char *>(instruction) + offsetof(RawStageInstr, args)));
            break;

        case 10:
            background->interpolationDuration[2] = (reinterpret_cast<int *>(reinterpret_cast<unsigned char *>(instruction) + offsetof(RawStageInstr, args))[0]);
            background->interpolationModes[2] = (reinterpret_cast<int *>(reinterpret_cast<unsigned char *>(instruction) + offsetof(RawStageInstr, args))[1]);
            background->interpolationTimers[2] = 0;
            break;

        case 11:
            reinterpret_cast<BackgroundStageCameraRecord &>(background->cameraStart).fieldOfView48 =
                reinterpret_cast<BackgroundStageCameraRecord &>(background->cameraTarget).fieldOfView48;
            reinterpret_cast<BackgroundStageCameraRecord &>(background->cameraTarget).fieldOfView48 =
                (reinterpret_cast<float *>(reinterpret_cast<unsigned char *>(instruction) + offsetof(RawStageInstr, args))[0]);
            if (background->interpolationDuration[3] == 0)
                (g_Supervisor.ctorArray078[static_cast<unsigned int>((background)->viewportIndex)])
                    .fieldOfView48 =
                    (reinterpret_cast<float *>(reinterpret_cast<unsigned char *>(instruction) + offsetof(RawStageInstr, args))[0]);
            break;

        case 12:
            background->interpolationDuration[3] = (reinterpret_cast<int *>(reinterpret_cast<unsigned char *>(instruction) + offsetof(RawStageInstr, args))[0]);
            background->interpolationTimers[3] = 0;
            background->interpolationModes[3] = (reinterpret_cast<int *>(reinterpret_cast<unsigned char *>(instruction) + offsetof(RawStageInstr, args))[1]);
            break;

        case 13:
            background->clearColor =
                static_cast<unsigned int>((reinterpret_cast<int *>(reinterpret_cast<unsigned char *>(instruction) + offsetof(RawStageInstr, args))[0]));
            break;

        case 3:
            if (background->pendingStageLabel18 == 0)
                goto script_done;
            background->pendingStageLabel18 = 0;
            break;

        case 4:
            background->stageScriptInstructionIndex = (reinterpret_cast<int *>(reinterpret_cast<unsigned char *>(instruction) + offsetof(RawStageInstr, args))[0]);
            background->stageScriptTimer = (reinterpret_cast<int *>(reinterpret_cast<unsigned char *>(instruction) + offsetof(RawStageInstr, args))[1]);
            background->interpolationDuration[0] = 0;
            background->compensateCameraJump = 1;
            goto script_done;

        case 14:
            reinterpret_cast<BackgroundStageCameraRecord &>(background->cameraStart).cameraPosition00 =
                (*reinterpret_cast<Float3 *>(reinterpret_cast<unsigned char *>(instruction) + offsetof(RawStageInstr, args)));
            break;

        case 15:
            reinterpret_cast<BackgroundStageCameraRecord &>(background->cameraTarget).cameraPosition00 =
                (*reinterpret_cast<Float3 *>(reinterpret_cast<unsigned char *>(instruction) + offsetof(RawStageInstr, args)));
            break;

        case 16:
            reinterpret_cast<BackgroundStageCameraRecord &>(background->cameraTangentStart).cameraPosition00 =
                (*reinterpret_cast<Float3 *>(reinterpret_cast<unsigned char *>(instruction) + offsetof(RawStageInstr, args)));
            break;

        case 17:
            reinterpret_cast<BackgroundStageCameraRecord &>(background->cameraTangentEnd).cameraPosition00 =
                (*reinterpret_cast<Float3 *>(reinterpret_cast<unsigned char *>(instruction) + offsetof(RawStageInstr, args)));
            break;

        case 18:
            background->interpolationDuration[0] = (reinterpret_cast<int *>(reinterpret_cast<unsigned char *>(instruction) + offsetof(RawStageInstr, args))[0]);
            background->interpolationTimers[0] = 0;
            background->interpolationModes[0] = 7;
            break;

        case 19:
            reinterpret_cast<BackgroundStageCameraRecord &>(background->cameraStart).cameraLookAtOffset0C =
                (*reinterpret_cast<Float3 *>(reinterpret_cast<unsigned char *>(instruction) + offsetof(RawStageInstr, args)));
            break;

        case 20:
            reinterpret_cast<BackgroundStageCameraRecord &>(background->cameraTarget).cameraLookAtOffset0C =
                (*reinterpret_cast<Float3 *>(reinterpret_cast<unsigned char *>(instruction) + offsetof(RawStageInstr, args)));
            break;

        case 21:
            reinterpret_cast<BackgroundStageCameraRecord &>(background->cameraTangentStart).cameraLookAtOffset0C =
                (*reinterpret_cast<Float3 *>(reinterpret_cast<unsigned char *>(instruction) + offsetof(RawStageInstr, args)));
            break;

        case 22:
            reinterpret_cast<BackgroundStageCameraRecord &>(background->cameraTangentEnd).cameraLookAtOffset0C =
                (*reinterpret_cast<Float3 *>(reinterpret_cast<unsigned char *>(instruction) + offsetof(RawStageInstr, args)));
            break;

        case 23:
            background->interpolationDuration[1] = (reinterpret_cast<int *>(reinterpret_cast<unsigned char *>(instruction) + offsetof(RawStageInstr, args))[0]);
            background->interpolationTimers[1] = 0;
            background->interpolationModes[1] = 7;
            break;

        case 24:
            reinterpret_cast<BackgroundStageCameraRecord &>(background->cameraStart).cameraUp18 =
                (*reinterpret_cast<Float3 *>(reinterpret_cast<unsigned char *>(instruction) + offsetof(RawStageInstr, args)));
            break;

        case 25:
            reinterpret_cast<BackgroundStageCameraRecord &>(background->cameraTarget).cameraUp18 =
                (*reinterpret_cast<Float3 *>(reinterpret_cast<unsigned char *>(instruction) + offsetof(RawStageInstr, args)));
            break;

        case 26:
            reinterpret_cast<BackgroundStageCameraRecord &>(background->cameraTangentStart).cameraUp18 =
                (*reinterpret_cast<Float3 *>(reinterpret_cast<unsigned char *>(instruction) + offsetof(RawStageInstr, args)));
            break;

        case 27:
            reinterpret_cast<BackgroundStageCameraRecord &>(background->cameraTangentEnd).cameraUp18 =
                (*reinterpret_cast<Float3 *>(reinterpret_cast<unsigned char *>(instruction) + offsetof(RawStageInstr, args)));
            break;

        case 28:
            background->interpolationDuration[2] = (reinterpret_cast<int *>(reinterpret_cast<unsigned char *>(instruction) + offsetof(RawStageInstr, args))[0]);
            background->interpolationTimers[2] = 0;
            background->interpolationModes[2] = 7;
            break;

        case 29:
            if ((reinterpret_cast<int *>(reinterpret_cast<unsigned char *>(instruction) + offsetof(RawStageInstr, args))[0]) >= 0)
                background->stageAnm->ExecuteAnmIdx(
                    &background->stageVm0, (reinterpret_cast<int *>(reinterpret_cast<unsigned char *>(instruction) + offsetof(RawStageInstr, args))[0]));
            else
                background->stageVm0.activeSpriteIndex = -1;
            break;

        case 30:
            if ((reinterpret_cast<int *>(reinterpret_cast<unsigned char *>(instruction) + offsetof(RawStageInstr, args))[0]) >= 0)
                background->stageAnm->ExecuteAnmIdx(
                    &background->stageVm1, (reinterpret_cast<int *>(reinterpret_cast<unsigned char *>(instruction) + offsetof(RawStageInstr, args))[0]));
            else
                background->stageVm0.activeSpriteIndex = -1;
            break;

        case 33:
            background->cameraMotionMode =
                (reinterpret_cast<unsigned char *>(instruction)[offsetof(RawStageInstr, args) + (0)]);
            background->interpolationDuration[4] = 0;
            background->interpolationTimers[4] = 0;
            background->interpolationModes[4] = 0;
            break;

        case 32:
            (g_Supervisor.ctorArray078[static_cast<unsigned int>((background)->viewportIndex)])
                .cameraPositionOffset3C =
                (*reinterpret_cast<Float3 *>(reinterpret_cast<unsigned char *>(instruction) + offsetof(RawStageInstr, args)));
            break;

        case 34:
            if ((reinterpret_cast<int *>(reinterpret_cast<unsigned char *>(instruction) + offsetof(RawStageInstr, args))[0]) >= 0)
                background->stageAnm->ExecuteAnmIdx(
                    &background->stageVm2, (reinterpret_cast<int *>(reinterpret_cast<unsigned char *>(instruction) + offsetof(RawStageInstr, args))[0]));
            else
                background->stageVm2.activeSpriteIndex = -1;
            break;

        case 31:
            break;
        }

        // Retail advances this signed wire index with an i386 INC DWORD.
        // Preserve its modulo-32 result instead of signed C++ overflow.
        ++*reinterpret_cast<unsigned int *>(
            &background->stageScriptInstructionIndex);
        goto read_instruction;
    }

script_done:
    if (background->interpolationDuration[0] != 0)
        InterpolateBackgroundCameraVector(
            background,
            0,
            &(g_Supervisor.ctorArray078[static_cast<unsigned int>((background)->viewportIndex)]).cameraPosition00,
            &reinterpret_cast<BackgroundStageCameraRecord &>(background->cameraStart).cameraPosition00,
            &reinterpret_cast<BackgroundStageCameraRecord &>(background->cameraTarget).cameraPosition00,
            &reinterpret_cast<BackgroundStageCameraRecord &>(background->cameraTangentStart).cameraPosition00,
            &reinterpret_cast<BackgroundStageCameraRecord &>(background->cameraTangentEnd).cameraPosition00);

    if (background->interpolationDuration[1] != 0)
        InterpolateBackgroundCameraVector(
            background,
            1,
            &(g_Supervisor.ctorArray078[static_cast<unsigned int>((background)->viewportIndex)]).cameraLookAtOffset0C,
            &reinterpret_cast<BackgroundStageCameraRecord &>(background->cameraStart).cameraLookAtOffset0C,
            &reinterpret_cast<BackgroundStageCameraRecord &>(background->cameraTarget).cameraLookAtOffset0C,
            &reinterpret_cast<BackgroundStageCameraRecord &>(background->cameraTangentStart).cameraLookAtOffset0C,
            &reinterpret_cast<BackgroundStageCameraRecord &>(background->cameraTangentEnd).cameraLookAtOffset0C);

    if (background->interpolationDuration[2] != 0)
        InterpolateBackgroundCameraVector(
            background,
            2,
            &(g_Supervisor.ctorArray078[static_cast<unsigned int>((background)->viewportIndex)]).cameraUp18,
            &reinterpret_cast<BackgroundStageCameraRecord &>(background->cameraStart).cameraUp18,
            &reinterpret_cast<BackgroundStageCameraRecord &>(background->cameraTarget).cameraUp18,
            &reinterpret_cast<BackgroundStageCameraRecord &>(background->cameraTangentStart).cameraUp18,
            &reinterpret_cast<BackgroundStageCameraRecord &>(background->cameraTangentEnd).cameraUp18);

    if (background->interpolationDuration[3] != 0)
    {
        float time;
        if (background->interpolationTimers[3] <
            background->interpolationDuration[3])
        {
            background->interpolationTimers[3]++;
            time = (float)background->interpolationTimers[3] /
                   background->interpolationDuration[3];
        }
        else
        {
            background->interpolationTimers[3] =
                background->interpolationDuration[3];
            time = 1.0f;
            background->interpolationDuration[3] = 0;
        }

        switch (background->interpolationModes[3])
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

        (g_Supervisor.ctorArray078[static_cast<unsigned int>((background)->viewportIndex)]).fieldOfView48 =
            (reinterpret_cast<BackgroundStageCameraRecord &>(background->cameraTarget).fieldOfView48 -
             reinterpret_cast<BackgroundStageCameraRecord &>(background->cameraStart).fieldOfView48) *
                time +
            reinterpret_cast<BackgroundStageCameraRecord &>(background->cameraStart).fieldOfView48;
    }

    D3DXVec3Normalize(
        reinterpret_cast<D3DXVECTOR3 *>(
            &(g_Supervisor.ctorArray078[static_cast<unsigned int>((background)->viewportIndex)]).cameraForward24),
        reinterpret_cast<D3DXVECTOR3 *>(
            &(g_Supervisor.ctorArray078[static_cast<unsigned int>((background)->viewportIndex)]).cameraLookAtOffset0C));

    if (background->cameraMotionMode != 0)
    {
        switch (background->cameraMotionMode)
        {
        case 1:
        {
            (g_Supervisor.ctorArray078[static_cast<unsigned int>((background)->viewportIndex)])
                .cameraPositionOffset3C.x = sinf(
                    (float)background->interpolationTimers[4] *
                        0.01308997f - 3.1415927f) * 40.0f;
            background->interpolationTimers[4]++;
            if (background->interpolationTimers[4] >= 480)
                background->interpolationTimers[4] = 0;
            break;
        }
        case 2:
        {
            float angle =
                (float)background->interpolationTimers[4] * 0.01308997f -
                3.1415927f;
            (g_Supervisor.ctorArray078[static_cast<unsigned int>((background)->viewportIndex)])
                .cameraPositionOffset3C.x = sinf(angle) * 70.0f;
            (g_Supervisor.ctorArray078[static_cast<unsigned int>((background)->viewportIndex)]).cameraUp18.x =
                sinf(angle) * -0.1f;
            background->interpolationTimers[4]++;
            if (background->interpolationTimers[4] >= 480)
                background->interpolationTimers[4] = 0;
            break;
        }
        case 4:
        {
            float angle =
                (float)background->interpolationTimers[4] * 0.0030679617f -
                3.1415927f;
            (g_Supervisor.ctorArray078[static_cast<unsigned int>((background)->viewportIndex)])
                .cameraPositionOffset3C.x = sinf(angle) * 30.0f;
            (g_Supervisor.ctorArray078[static_cast<unsigned int>((background)->viewportIndex)])
                .cameraPositionOffset3C.z = cosf(angle) * 30.0f;
            (g_Supervisor.ctorArray078[static_cast<unsigned int>((background)->viewportIndex)]).cameraUp18.x =
                sinf(angle) * -0.1f;
            background->interpolationTimers[4]++;
            if (background->interpolationTimers[4] >= 2048)
                background->interpolationTimers[4] = 0;
            break;
        }
        case 3:
        {
            float angle =
                (float)background->interpolationTimers[4] * 0.0013089969f -
                3.1415927f;
            (g_Supervisor.ctorArray078[static_cast<unsigned int>((background)->viewportIndex)]).cameraUp18.x =
                sinf(angle);
            (g_Supervisor.ctorArray078[static_cast<unsigned int>((background)->viewportIndex)]).cameraUp18.z =
                cosf(angle);
            background->interpolationTimers[4]++;
            if (background->interpolationTimers[4] >= 4800)
                background->interpolationTimers[4] = 0;
            break;
        }
        }

    }

    // The sky/fog color interpolates the four actual packed color bytes.

    if (background->skyFogInterpolationDuration != 0)
    {
        background->skyFogInterpolationTimer++;
        float fogRatio =
            (float)background->skyFogInterpolationTimer / background->skyFogInterpolationDuration;
        if (fogRatio >= 1.0f)
            fogRatio = 1.0f;

        for (int i = 0; i < 4; ++i)
            (reinterpret_cast<unsigned char *>(&(background->skyFog.color))[i]) =
                static_cast<unsigned char>(
                    ((float)(reinterpret_cast<unsigned char *>(&(background->skyFogFinal.color))[i]) -
                     (float)(reinterpret_cast<unsigned char *>(&(background->skyFogInitial.color))[i])) *
                        fogRatio +
                    (float)(reinterpret_cast<unsigned char *>(&(background->skyFogInitial.color))[i]));

        background->skyFog.nearPlane =
            (background->skyFogFinal.nearPlane -
             background->skyFogInitial.nearPlane) *
                fogRatio +
            background->skyFogInitial.nearPlane;
        background->skyFog.farPlane =
            (background->skyFogFinal.farPlane -
             background->skyFogInitial.farPlane) *
                fogRatio +
            background->skyFogInitial.farPlane;

        if (background->skyFogInterpolationTimer >= background->skyFogInterpolationDuration)
            background->skyFogInterpolationDuration = 0;
    }
}
