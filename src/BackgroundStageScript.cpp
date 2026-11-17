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


// STD instruction records carry their operands directly in the file-backed
// variable-size +0x08 DWORD payload. Read the live wire word by its real
// Win32 byte address; the nominal args[3] view is only a prefix.
#define TH09_BG_NATIVE_WIRE_WORD(index) \
    (*reinterpret_cast<int *>( \
        reinterpret_cast<unsigned int>(instruction) + \
        offsetof(RawStageInstr, args) + \
        static_cast<unsigned int>(index) * sizeof(int)))
#define TH09_BG_NATIVE_WIRE_FLOAT(index) \
    (*reinterpret_cast<float *>( \
        reinterpret_cast<unsigned int>(instruction) + \
        offsetof(RawStageInstr, args) + \
        static_cast<unsigned int>(index) * sizeof(float)))
#define TH09_BG_NATIVE_WIRE_BYTE(index) \
    (*reinterpret_cast<unsigned char *>( \
        reinterpret_cast<unsigned int>(instruction) + \
        offsetof(RawStageInstr, args) + \
        static_cast<unsigned int>(index)))
#define TH09_BG_NATIVE_WIRE_VECTOR(type) \
    (*reinterpret_cast<type *>( \
        reinterpret_cast<unsigned int>(instruction) + \
        offsetof(RawStageInstr, args)))

// All 19 live stage-camera selectors target the same three original
// 0xF0-byte Supervisor viewport records as exact viewport/routing code.
// Form the native Win32 signed-DWORD address without a bounded array view.
#define TH09_BG_NATIVE_CAMERA(owner) \
    (*reinterpret_cast<BackgroundStageCameraRecord *>( \
        reinterpret_cast<unsigned int>(&g_Supervisor.ctorArray078[0]) + \
        static_cast<unsigned int>((owner)->viewportIndex) * \
            sizeof(g_Supervisor.ctorArray078[0])))

void __fastcall BackgroundRunStageScriptPhase(Background *backgroundOwner)
{
    Background *background = backgroundOwner;
    D3DXVECTOR3 position;
    RawStageInstr *instruction;

read_instruction:
    // Native stage instructions use a 0x14-byte signed wire index scaled as
    // an unsigned i386 word, without a C++ pointer-in-array precondition.
    instruction = reinterpret_cast<RawStageInstr *>(
        reinterpret_cast<unsigned int>(background->stageScript) +
        static_cast<unsigned int>(background->stageScriptInstructionIndex) *
            sizeof(RawStageInstr));
    if (background->stageScriptTimer >= instruction->frame)
    {
        if (instruction->frame == -1)
            goto script_done;

        switch (instruction->opcode)
        {
        case 0:
            position = TH09_BG_NATIVE_WIRE_VECTOR(D3DXVECTOR3);
            background->stagePosition.x = position.x;
            background->stagePosition.y = position.y;
            background->stagePosition.z = position.z;
            reinterpret_cast<Float3 &>(background->stagePositionInitial) =
                *reinterpret_cast<Float3 *>(&position);
            background->stagePositionStartFrame = instruction->frame;
            // The following camera keyframe is the next physical 0x14-byte
            // record in the original file-backed instruction stream.
            // Advance its native i386 byte address, not a bounded C++ array
            // pointer to a single projected instruction object.
            instruction = reinterpret_cast<RawStageInstr *>(
                reinterpret_cast<unsigned int>(instruction) +
                sizeof(RawStageInstr));
            background->stagePositionEndFrame = instruction->frame;
            reinterpret_cast<Float3 &>(background->stagePositionTarget) =
                TH09_BG_NATIVE_WIRE_VECTOR(Float3);
            break;

        case 1:
            background->skyFog.color =
                static_cast<unsigned int>(TH09_BG_NATIVE_WIRE_WORD(0));
            background->skyFog.nearPlane =
                TH09_BG_NATIVE_WIRE_FLOAT(1);
            background->skyFog.farPlane =
                TH09_BG_NATIVE_WIRE_FLOAT(2);
            background->skyFogFinal = background->skyFog;
            break;

        case 2:
            background->skyFogInitial = background->skyFog;
            background->skyFogInterpolationDuration = TH09_BG_NATIVE_WIRE_WORD(0);
            background->skyFogInterpolationTimer = 0;
            break;

        case 5:
            if (background->compensateCameraJump != 0)
                background->compensateCameraJump = 0;
            reinterpret_cast<BackgroundStageCameraRecord &>(background->cameraStart).cameraPosition00 =
                reinterpret_cast<BackgroundStageCameraRecord &>(background->cameraTarget).cameraPosition00;
            reinterpret_cast<BackgroundStageCameraRecord &>(background->cameraTarget).cameraPosition00 =
                TH09_BG_NATIVE_WIRE_VECTOR(Float3);
            if (background->interpolationDuration[0] == 0)
                TH09_BG_NATIVE_CAMERA(background)
                    .cameraPosition00 =
                    TH09_BG_NATIVE_WIRE_VECTOR(Float3);
            break;

        case 6:
            background->interpolationDuration[0] = TH09_BG_NATIVE_WIRE_WORD(0);
            background->interpolationTimers[0] = 0;
            background->interpolationModes[0] = TH09_BG_NATIVE_WIRE_WORD(1);
            break;

        case 7:
            reinterpret_cast<BackgroundStageCameraRecord &>(background->cameraStart).cameraLookAtOffset0C =
                reinterpret_cast<BackgroundStageCameraRecord &>(background->cameraTarget).cameraLookAtOffset0C;
            reinterpret_cast<BackgroundStageCameraRecord &>(background->cameraTarget).cameraLookAtOffset0C =
                TH09_BG_NATIVE_WIRE_VECTOR(Float3);
            if (background->interpolationDuration[1] == 0)
                TH09_BG_NATIVE_CAMERA(background)
                    .cameraLookAtOffset0C =
                    TH09_BG_NATIVE_WIRE_VECTOR(Float3);
            break;

        case 8:
            background->interpolationDuration[1] = TH09_BG_NATIVE_WIRE_WORD(0);
            background->interpolationTimers[1] = 0;
            background->interpolationModes[1] = TH09_BG_NATIVE_WIRE_WORD(1);
            break;

        case 9:
            reinterpret_cast<BackgroundStageCameraRecord &>(background->cameraStart).cameraUp18 =
                reinterpret_cast<BackgroundStageCameraRecord &>(background->cameraTarget).cameraUp18;
            reinterpret_cast<BackgroundStageCameraRecord &>(background->cameraTarget).cameraUp18 =
                TH09_BG_NATIVE_WIRE_VECTOR(Float3);
            if (background->interpolationDuration[2] == 0)
                TH09_BG_NATIVE_CAMERA(background).cameraUp18 =
                    TH09_BG_NATIVE_WIRE_VECTOR(Float3);
            break;

        case 10:
            background->interpolationDuration[2] = TH09_BG_NATIVE_WIRE_WORD(0);
            background->interpolationModes[2] = TH09_BG_NATIVE_WIRE_WORD(1);
            background->interpolationTimers[2] = 0;
            break;

        case 11:
            reinterpret_cast<BackgroundStageCameraRecord &>(background->cameraStart).fieldOfView48 =
                reinterpret_cast<BackgroundStageCameraRecord &>(background->cameraTarget).fieldOfView48;
            reinterpret_cast<BackgroundStageCameraRecord &>(background->cameraTarget).fieldOfView48 =
                TH09_BG_NATIVE_WIRE_FLOAT(0);
            if (background->interpolationDuration[3] == 0)
                TH09_BG_NATIVE_CAMERA(background)
                    .fieldOfView48 =
                    TH09_BG_NATIVE_WIRE_FLOAT(0);
            break;

        case 12:
            background->interpolationDuration[3] = TH09_BG_NATIVE_WIRE_WORD(0);
            background->interpolationTimers[3] = 0;
            background->interpolationModes[3] = TH09_BG_NATIVE_WIRE_WORD(1);
            break;

        case 13:
            background->clearColor =
                static_cast<unsigned int>(TH09_BG_NATIVE_WIRE_WORD(0));
            break;

        case 3:
            if (background->pendingStageLabel18 == 0)
                goto script_done;
            background->pendingStageLabel18 = 0;
            break;

        case 4:
            background->stageScriptInstructionIndex = TH09_BG_NATIVE_WIRE_WORD(0);
            background->stageScriptTimer = TH09_BG_NATIVE_WIRE_WORD(1);
            background->interpolationDuration[0] = 0;
            background->compensateCameraJump = 1;
            goto script_done;

        case 14:
            reinterpret_cast<BackgroundStageCameraRecord &>(background->cameraStart).cameraPosition00 =
                TH09_BG_NATIVE_WIRE_VECTOR(Float3);
            break;

        case 15:
            reinterpret_cast<BackgroundStageCameraRecord &>(background->cameraTarget).cameraPosition00 =
                TH09_BG_NATIVE_WIRE_VECTOR(Float3);
            break;

        case 16:
            reinterpret_cast<BackgroundStageCameraRecord &>(background->cameraTangentStart).cameraPosition00 =
                TH09_BG_NATIVE_WIRE_VECTOR(Float3);
            break;

        case 17:
            reinterpret_cast<BackgroundStageCameraRecord &>(background->cameraTangentEnd).cameraPosition00 =
                TH09_BG_NATIVE_WIRE_VECTOR(Float3);
            break;

        case 18:
            background->interpolationDuration[0] = TH09_BG_NATIVE_WIRE_WORD(0);
            background->interpolationTimers[0] = 0;
            background->interpolationModes[0] = 7;
            break;

        case 19:
            reinterpret_cast<BackgroundStageCameraRecord &>(background->cameraStart).cameraLookAtOffset0C =
                TH09_BG_NATIVE_WIRE_VECTOR(Float3);
            break;

        case 20:
            reinterpret_cast<BackgroundStageCameraRecord &>(background->cameraTarget).cameraLookAtOffset0C =
                TH09_BG_NATIVE_WIRE_VECTOR(Float3);
            break;

        case 21:
            reinterpret_cast<BackgroundStageCameraRecord &>(background->cameraTangentStart).cameraLookAtOffset0C =
                TH09_BG_NATIVE_WIRE_VECTOR(Float3);
            break;

        case 22:
            reinterpret_cast<BackgroundStageCameraRecord &>(background->cameraTangentEnd).cameraLookAtOffset0C =
                TH09_BG_NATIVE_WIRE_VECTOR(Float3);
            break;

        case 23:
            background->interpolationDuration[1] = TH09_BG_NATIVE_WIRE_WORD(0);
            background->interpolationTimers[1] = 0;
            background->interpolationModes[1] = 7;
            break;

        case 24:
            reinterpret_cast<BackgroundStageCameraRecord &>(background->cameraStart).cameraUp18 =
                TH09_BG_NATIVE_WIRE_VECTOR(Float3);
            break;

        case 25:
            reinterpret_cast<BackgroundStageCameraRecord &>(background->cameraTarget).cameraUp18 =
                TH09_BG_NATIVE_WIRE_VECTOR(Float3);
            break;

        case 26:
            reinterpret_cast<BackgroundStageCameraRecord &>(background->cameraTangentStart).cameraUp18 =
                TH09_BG_NATIVE_WIRE_VECTOR(Float3);
            break;

        case 27:
            reinterpret_cast<BackgroundStageCameraRecord &>(background->cameraTangentEnd).cameraUp18 =
                TH09_BG_NATIVE_WIRE_VECTOR(Float3);
            break;

        case 28:
            background->interpolationDuration[2] = TH09_BG_NATIVE_WIRE_WORD(0);
            background->interpolationTimers[2] = 0;
            background->interpolationModes[2] = 7;
            break;

        case 29:
            if (TH09_BG_NATIVE_WIRE_WORD(0) >= 0)
                background->stageAnm->ExecuteAnmIdx(
                    &background->stageVm0, TH09_BG_NATIVE_WIRE_WORD(0));
            else
                background->stageVm0.activeSpriteIndex = -1;
            break;

        case 30:
            if (TH09_BG_NATIVE_WIRE_WORD(0) >= 0)
                background->stageAnm->ExecuteAnmIdx(
                    &background->stageVm1, TH09_BG_NATIVE_WIRE_WORD(0));
            else
                background->stageVm0.activeSpriteIndex = -1;
            break;

        case 33:
            background->cameraMotionMode =
                TH09_BG_NATIVE_WIRE_BYTE(0);
            background->interpolationDuration[4] = 0;
            background->interpolationTimers[4] = 0;
            background->interpolationModes[4] = 0;
            break;

        case 32:
            TH09_BG_NATIVE_CAMERA(background)
                .cameraPositionOffset3C =
                TH09_BG_NATIVE_WIRE_VECTOR(Float3);
            break;

        case 34:
            if (TH09_BG_NATIVE_WIRE_WORD(0) >= 0)
                background->stageAnm->ExecuteAnmIdx(
                    &background->stageVm2, TH09_BG_NATIVE_WIRE_WORD(0));
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
            &TH09_BG_NATIVE_CAMERA(background).cameraPosition00,
            &reinterpret_cast<BackgroundStageCameraRecord &>(background->cameraStart).cameraPosition00,
            &reinterpret_cast<BackgroundStageCameraRecord &>(background->cameraTarget).cameraPosition00,
            &reinterpret_cast<BackgroundStageCameraRecord &>(background->cameraTangentStart).cameraPosition00,
            &reinterpret_cast<BackgroundStageCameraRecord &>(background->cameraTangentEnd).cameraPosition00);

    if (background->interpolationDuration[1] != 0)
        InterpolateBackgroundCameraVector(
            background,
            1,
            &TH09_BG_NATIVE_CAMERA(background).cameraLookAtOffset0C,
            &reinterpret_cast<BackgroundStageCameraRecord &>(background->cameraStart).cameraLookAtOffset0C,
            &reinterpret_cast<BackgroundStageCameraRecord &>(background->cameraTarget).cameraLookAtOffset0C,
            &reinterpret_cast<BackgroundStageCameraRecord &>(background->cameraTangentStart).cameraLookAtOffset0C,
            &reinterpret_cast<BackgroundStageCameraRecord &>(background->cameraTangentEnd).cameraLookAtOffset0C);

    if (background->interpolationDuration[2] != 0)
        InterpolateBackgroundCameraVector(
            background,
            2,
            &TH09_BG_NATIVE_CAMERA(background).cameraUp18,
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

        TH09_BG_NATIVE_CAMERA(background).fieldOfView48 =
            (reinterpret_cast<BackgroundStageCameraRecord &>(background->cameraTarget).fieldOfView48 -
             reinterpret_cast<BackgroundStageCameraRecord &>(background->cameraStart).fieldOfView48) *
                time +
            reinterpret_cast<BackgroundStageCameraRecord &>(background->cameraStart).fieldOfView48;
    }

    D3DXVec3Normalize(
        reinterpret_cast<D3DXVECTOR3 *>(
            &TH09_BG_NATIVE_CAMERA(background).cameraForward24),
        reinterpret_cast<D3DXVECTOR3 *>(
            &TH09_BG_NATIVE_CAMERA(background).cameraLookAtOffset0C));

    if (background->cameraMotionMode != 0)
    {
        switch (background->cameraMotionMode)
        {
        case 1:
        {
            TH09_BG_NATIVE_CAMERA(background)
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
            TH09_BG_NATIVE_CAMERA(background)
                .cameraPositionOffset3C.x = sinf(angle) * 70.0f;
            TH09_BG_NATIVE_CAMERA(background).cameraUp18.x =
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
            TH09_BG_NATIVE_CAMERA(background)
                .cameraPositionOffset3C.x = sinf(angle) * 30.0f;
            TH09_BG_NATIVE_CAMERA(background)
                .cameraPositionOffset3C.z = cosf(angle) * 30.0f;
            TH09_BG_NATIVE_CAMERA(background).cameraUp18.x =
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
            TH09_BG_NATIVE_CAMERA(background).cameraUp18.x =
                sinf(angle);
            TH09_BG_NATIVE_CAMERA(background).cameraUp18.z =
                cosf(angle);
            background->interpolationTimers[4]++;
            if (background->interpolationTimers[4] >= 4800)
                background->interpolationTimers[4] = 0;
            break;
        }
        }

    }

    // Every sky/fog channel is one byte in the original four-byte color
    // word. Native address arithmetic operates on the real color field.
#define TH09_BG_NATIVE_FOG_COLOR_BYTE(color, index) \
    (*reinterpret_cast<unsigned char *>( \
        reinterpret_cast<unsigned int>(&(color)) + \
        static_cast<unsigned int>(index)))

    if (background->skyFogInterpolationDuration != 0)
    {
        background->skyFogInterpolationTimer++;
        float fogRatio =
            (float)background->skyFogInterpolationTimer / background->skyFogInterpolationDuration;
        if (fogRatio >= 1.0f)
            fogRatio = 1.0f;

        for (int i = 0; i < 4; ++i)
            TH09_BG_NATIVE_FOG_COLOR_BYTE(background->skyFog.color, i) =
                static_cast<unsigned char>(
                    ((float)TH09_BG_NATIVE_FOG_COLOR_BYTE(background->skyFogFinal.color, i) -
                     (float)TH09_BG_NATIVE_FOG_COLOR_BYTE(background->skyFogInitial.color, i)) *
                        fogRatio +
                    (float)TH09_BG_NATIVE_FOG_COLOR_BYTE(background->skyFogInitial.color, i));

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

#undef TH09_BG_NATIVE_CAMERA

#undef TH09_BG_NATIVE_FOG_COLOR_BYTE

#undef TH09_BG_NATIVE_WIRE_WORD

#undef TH09_BG_NATIVE_WIRE_FLOAT
#undef TH09_BG_NATIVE_WIRE_BYTE

#undef TH09_BG_NATIVE_WIRE_VECTOR
