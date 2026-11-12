// Target-bound TH09 ANM script execution view.
// The TH08 family supplies source shape; TH09 target evidence fixes the ABI,
// VM layout, opcode domain, out-of-line operand helpers, and physical boundary.

#include "Float3.hpp"
#include "AnmManager.hpp"
#include "AnmVmLifecycle.hpp"
#include "RngRuntimeLeaves.hpp"
#include "Supervisor.hpp"

#include <math.h>
#include <stddef.h>

typedef unsigned char u8;
typedef unsigned short u16;
typedef short i16;
typedef unsigned int u32;
typedef int i32;
typedef float f32;
typedef int ZunBool;

#ifndef TRUE
#define TRUE 1
#define FALSE 0
#endif

#include "ZunTimer.hpp"

enum AnmVariable
{
    AnmVariable_I0 = 10000,
    AnmVariable_I1,
    AnmVariable_I2,
    AnmVariable_I3,
    AnmVariable_F0,
    AnmVariable_F1,
    AnmVariable_F2,
    AnmVariable_F3,
    AnmVariable_IC0,
    AnmVariable_IC1,
};

enum AnmInterp
{
    AnmInterp_Pos,
    AnmInterp_RGB1,
    AnmInterp_Alpha1,
    AnmInterp_Rotate,
    AnmInterp_Scale,
    AnmInterp_RGB2,
    AnmInterp_Alpha2,
    AnmInterp_Last
};

enum AnmInterpMode
{
    AnmInterpMode_Linear = 0,
    AnmInterpMode_EaseIn = 1,
    AnmInterpMode_EaseInCubic = 2,
    AnmInterpMode_EaseInQuartic = 3,
    AnmInterpMode_EaseOut = 4,
    AnmInterpMode_EaseOutCubic = 5,
    AnmInterpMode_EaseOutQuartic = 6
};

enum AnmOpcode
{
    AnmOpcode_EndOfScript = -1,
    AnmOpcode_Nop = 0,
    AnmOpcode_Delete = 1,
    AnmOpcode_Static = 2,
    AnmOpcode_Sprite = 3,
    AnmOpcode_Jmp = 4,
    AnmOpcode_JmpDec = 5,
    AnmOpcode_Pos = 6,
    AnmOpcode_Scale = 7,
    AnmOpcode_Alpha = 8,
    AnmOpcode_Color = 9,
    AnmOpcode_FlipX = 10,
    AnmOpcode_FlipY = 11,
    AnmOpcode_Rotate = 12,
    AnmOpcode_AngularVelocity = 13,
    AnmOpcode_ScaleGrowth = 14,
    AnmOpcode_AlphaTimeLinear = 15,
    AnmOpcode_AdditiveBlendMode = 16,
    AnmOpcode_PosTimeLinear = 17,
    AnmOpcode_PosTimeDecel = 18,
    AnmOpcode_PosTimeDecel2 = 19,
    AnmOpcode_Stop = 20,
    AnmOpcode_InterruptLabel = 21,
    AnmOpcode_AnchorTopLeft = 22,
    AnmOpcode_StopHide = 23,
    AnmOpcode_PosMode = 24,
    AnmOpcode_Ins25 = 25,
    AnmOpcode_AddU = 26,
    AnmOpcode_AddV = 27,
    AnmOpcode_Visible = 28,
    AnmOpcode_ScaleTimeLinear = 29,
    AnmOpcode_ZWriteDisable = 30,
    AnmOpcode_Ins31 = 31,
    AnmOpcode_PosTime = 32,
    AnmOpcode_ColorTime = 33,
    AnmOpcode_AlphaTime = 34,
    AnmOpcode_RotateTime = 35,
    AnmOpcode_ScaleTime = 36,
    AnmOpcode_ISet = 37,
    AnmOpcode_FSet = 38,
    AnmOpcode_IAdd = 39,
    AnmOpcode_FAdd = 40,
    AnmOpcode_ISub = 41,
    AnmOpcode_FSub = 42,
    AnmOpcode_IMul = 43,
    AnmOpcode_FMul = 44,
    AnmOpcode_IDiv = 45,
    AnmOpcode_FDiv = 46,
    AnmOpcode_IMod = 47,
    AnmOpcode_FMod = 48,
    AnmOpcode_ISetAdd = 49,
    AnmOpcode_FSetAdd = 50,
    AnmOpcode_ISetSub = 51,
    AnmOpcode_FSetSub = 52,
    AnmOpcode_ISetMul = 53,
    AnmOpcode_FSetMul = 54,
    AnmOpcode_ISetDiv = 55,
    AnmOpcode_FSetDiv = 56,
    AnmOpcode_ISetMod = 57,
    AnmOpcode_FSetMod = 58,
    AnmOpcode_ISetRand = 59,
    AnmOpcode_FSetRand = 60,
    AnmOpcode_FSin = 61,
    AnmOpcode_FCos = 62,
    AnmOpcode_FTan = 63,
    AnmOpcode_FAcos = 64,
    AnmOpcode_FAtan = 65,
    AnmOpcode_NormalizeAngle = 66,
    AnmOpcode_IJmpEq = 67,
    AnmOpcode_FJmpEq = 68,
    AnmOpcode_IJmpNeq = 69,
    AnmOpcode_FJmpNeq = 70,
    AnmOpcode_IJmpLess = 71,
    AnmOpcode_FJmpLess = 72,
    AnmOpcode_IJmpLessOrEq = 73,
    AnmOpcode_FJmpLessOrEq = 74,
    AnmOpcode_IJmpGreater = 75,
    AnmOpcode_FJmpGreater = 76,
    AnmOpcode_IJmpGreaterOrEq = 77,
    AnmOpcode_FJmpGreaterOrEq = 78,
    AnmOpcode_Wait = 79,
    AnmOpcode_UScroll = 80,
    AnmOpcode_VScroll = 81,
    AnmOpcode_BlendMode = 82,
    AnmOpcode_Ins83 = 83,
    AnmOpcode_Color2 = 84,
    AnmOpcode_Alpha2 = 85,
    AnmOpcode_Color2Time = 86,
    AnmOpcode_Alpha2Time = 87,
    AnmOpcode_Ins88 = 88,
    AnmOpcode_ReturnFromInterrupt = 89
};

// The original ANM instruction is an eight-byte header followed by a
// variable-length payload (including opcodes with no operands). The
// one-DWORD union here describes only the first possible wire operand;
// every other integer/float operand is addressed through the real native
// script byte cursor, not through a fictional fixed ten-argument array.
struct AnmRawInstr
{
    i16 opcode;
    u16 instructionSize;
    i16 time;
    u16 varMask;
    union {
        i32 intArgs[1];
        f32 floatArgs[1];
        u8 byteArgs[sizeof(i32)];
    };
};
typedef char AnmWireNativeHeaderAndPayload[
    (offsetof(AnmRawInstr, intArgs) == 8 &&
     offsetof(AnmRawInstr, floatArgs) == 8 &&
     offsetof(AnmRawInstr, byteArgs) == 8 &&
     sizeof(AnmRawInstr) == 12) ? 1 : -1];

// This 9,358-byte original-bound interpreter consumes the same native
// 0x2A4-byte AnmVm whose constructor, runtime and Etama owners are rebuilt
// independently. Only field spellings below change; physical offsets do not.

typedef char AnmExecVmSizeIs2A4[(sizeof(AnmVm) == 0x2A4) ? 1 : -1];
typedef char AnmExecCurrentTimeAt38[(offsetof(AnmVm,currentTimeInScript)==0x38)?1:-1];
typedef char AnmExecIntVarsAt100[(offsetof(AnmVm,intVar0)==0x100)?1:-1];
typedef char AnmExecFlagsAt1F8[(offsetof(AnmVm,flagsWord)==0x1F8)?1:-1];
typedef char AnmExecPendingAt1FE[(offsetof(AnmVm,pendingInterrupt)==0x1FE)?1:-1];
typedef char AnmExecAnmFileAt204[(offsetof(AnmVm,anmFile)==0x204)?1:-1];
typedef char AnmExecCurrentInstructionAt220[(offsetof(AnmVm,currentInstruction)==0x220)?1:-1];
typedef char AnmExecLastSpriteTimeAt294[(offsetof(AnmVm,timeOfLastSpriteSet)==0x294)?1:-1];

extern RngRuntimeView g_Rng;

float AddNormalizeAngle(float value, float base);

// The enormous script interpreter consumes the same native +0x10 manager
// counter as the independently exact frame-reset leaf.
typedef char AnmScriptExecutionCounterNativeAt10[
    (offsetof(AnmManager, scriptsExecutedThisFrame10) == 0x10) ? 1 : -1];


i32 AnmVm::GetIntVar(i32 varId)
{
    switch (varId) {
    case AnmVariable_I0: return intVar0;
    case AnmVariable_I1: return intVar1;
    case AnmVariable_I2: return intVar2;
    case AnmVariable_I3: return intVar3;
    case AnmVariable_F0: return floatVar0;
    case AnmVariable_F1: return floatVar1;
    case AnmVariable_F2: return floatVar2;
    case AnmVariable_F3: return floatVar3;
    case AnmVariable_IC0: return counterVar0;
    case AnmVariable_IC1: return counterVar1;
    default: return varId;
    }
}

// The two script-variable mask resolvers test genuine unsigned
// 32-bit bit patterns, including the sign-bit position of a mask,
// without invoking undefined signed C++ left shifts.
f32 *AnmVm::GetFloatVarPtr(f32 *varPtr, u16 varMask, u32 variableNumber)
{
    if ((varMask & (1u << variableNumber)) == 0) return varPtr;
    switch ((i32)*varPtr) {
    case AnmVariable_F0: return &floatVar0;
    case AnmVariable_F1: return &floatVar1;
    case AnmVariable_F2: return &floatVar2;
    case AnmVariable_F3: return &floatVar3;
    default: return varPtr;
    }
}

i32 *AnmVm::GetIntVarPtr(i32 *varPtr, u16 varMask, u32 variableNumber)
{
    if ((varMask & (1u << variableNumber)) == 0) return varPtr;
    switch (*varPtr) {
    case AnmVariable_I0: return &intVar0;
    case AnmVariable_I1: return &intVar1;
    case AnmVariable_I2: return &intVar2;
    case AnmVariable_I3: return &intVar3;
    case AnmVariable_IC0: return &counterVar0;
    case AnmVariable_IC1: return &counterVar1;
    default: return varPtr;
    }
}

f32 AnmVm::GetFloatVar(f32 varId)
{
    switch ((i32)varId) {
    case AnmVariable_I0: return (f32)intVar0;
    case AnmVariable_I1: return (f32)intVar1;
    case AnmVariable_I2: return (f32)intVar2;
    case AnmVariable_I3: return (f32)intVar3;
    case AnmVariable_F0: return floatVar0;
    case AnmVariable_F1: return floatVar1;
    case AnmVariable_F2: return floatVar2;
    case AnmVariable_F3: return floatVar3;
    case AnmVariable_IC0: return (f32)counterVar0;
    case AnmVariable_IC1: return (f32)counterVar1;
    // TH09 variable IDs select signed range(pi), unit, and signed RNG.
    // Keep the switch labels faithful; relocation tables cannot remap cases.
    case 10011: return g_Rng.GetRandomF32();
    case 10012: return g_Rng.GetRandomF32Signed();
    case 10010: return g_Rng.GetRandomF32SignedInRange(3.1415927f);
    default: return varId;
    }
}

ZunBool AnmManager::ExecuteScript(AnmVm *vm)
{
    AnmRawInstr *instruction;
    AnmRawInstr *nextInstruction;
    int i;
    float interp;

    if (vm->currentInstruction == NULL)
    {
        return TRUE;
    }

    if (vm->flag19 != 0)
    {
        return FALSE;
    }

    if (vm->pendingInterrupt != 0)
    {
        goto handleInterrupt;
    }

    while (instruction = vm->currentInstruction, instruction->time <= (int)vm->currentTimeInScript)
    {
// Animation opcodes, including jumps and direct VM writes, decode packed,
// variable-length file records. Each native operand lvalue is the actual
// buffer's +8 payload and a 32-bit word stride, not an owned intArgs[10]
// allocation. Preserve the individual live reads at every opcode site.
#define TH09_ANM_NATIVE_WIRE_INT(index) \
    (*reinterpret_cast<i32 *>( \
        reinterpret_cast<u32>(instruction) + \
        static_cast<u32>(offsetof(AnmRawInstr, intArgs)) + \
        static_cast<u32>(index) * sizeof(i32)))
// The equivalent real FLOAT operand occupies the same variable-width
// instruction payload. Interpret its four bytes at native Win32 addresses.
#define TH09_ANM_NATIVE_WIRE_FLOAT(index) \
    (*reinterpret_cast<f32 *>( \
        reinterpret_cast<u32>(instruction) + \
        static_cast<u32>(offsetof(AnmRawInstr, floatArgs)) + \
        static_cast<u32>(index) * sizeof(f32)))
// The original variable mask is a wire u16 and a dynamic i386 shift
// count. Shift an unsigned DWORD, preserving even bit 31 without C++
// undefined signed-shift behavior or an extra instruction.
#define GET_INT_VAR(argNumber)                                                                                         \
    ((instruction->varMask & (1u << argNumber)) ? vm->GetIntVar(TH09_ANM_NATIVE_WIRE_INT(argNumber))                        \
                                               : TH09_ANM_NATIVE_WIRE_INT(argNumber))
#define GET_FLOAT_VAR(argNumber)                                                                                       \
    ((instruction->varMask & (1u << argNumber)) ? vm->GetFloatVar(TH09_ANM_NATIVE_WIRE_FLOAT(argNumber))                    \
                                               : TH09_ANM_NATIVE_WIRE_FLOAT(argNumber))

#define GET_INT_VAR_PTR(idx) vm->GetIntVarPtr(&TH09_ANM_NATIVE_WIRE_INT(idx), instruction->varMask, idx)

#define GET_FLOAT_VAR_PTR(idx) vm->GetFloatVarPtr(&TH09_ANM_NATIVE_WIRE_FLOAT(idx), instruction->varMask, idx)

        switch (instruction->opcode)
        {
        case AnmOpcode_EndOfScript:
        case AnmOpcode_Delete:
            vm->visible = false;
        case AnmOpcode_Static:
            vm->currentInstruction = NULL;
            return TRUE;
        case AnmOpcode_Sprite:
            vm->visible = true;

            vm->anmFile->SetSprite(vm, GET_INT_VAR(0));
            vm->timeOfLastSpriteSet = (int)vm->currentTimeInScript;
            break;
        case AnmOpcode_Scale:
            vm->scale.x = GET_FLOAT_VAR(0);
            vm->scale.y = GET_FLOAT_VAR(1);

            vm->updateScale = true;
            break;
        case AnmOpcode_Alpha:
            vm->color1Script.a = GET_INT_VAR(0);
            break;
        case AnmOpcode_Color:
            vm->color1Script.r = GET_INT_VAR(0);
            vm->color1Script.g = GET_INT_VAR(1);
            vm->color1Script.b = GET_INT_VAR(2);
            break;
        case AnmOpcode_Alpha2:
            vm->color2Script.a = GET_INT_VAR(0);
            break;
        case AnmOpcode_Color2:
            vm->color2Script.r = GET_INT_VAR(0);
            vm->color2Script.g = GET_INT_VAR(1);
            vm->color2Script.b = GET_INT_VAR(2);
            break;
        case AnmOpcode_Jmp:
            // Script records have varying physical argument lengths. The
            // original reads this packed DWORD via the native file address.
            vm->currentTimeInScript = TH09_ANM_NATIVE_WIRE_INT(1);
            vm->currentInstruction = reinterpret_cast<AnmRawInstr *>(reinterpret_cast<unsigned int>(vm->beginningOfScript) + static_cast<unsigned int>(TH09_ANM_NATIVE_WIRE_INT(0)));
            continue;
        // The animation interpreter's jump counter is a script-controlled
        // signed DWORD; retail decrements its native bits modulo 2^32.
        case AnmOpcode_JmpDec:
            --*reinterpret_cast<u32 *>(GET_INT_VAR_PTR(0));

            if (GET_INT_VAR(0) > 0)
            {
                vm->currentTimeInScript = TH09_ANM_NATIVE_WIRE_INT(2);
                vm->currentInstruction = reinterpret_cast<AnmRawInstr *>(reinterpret_cast<unsigned int>(vm->beginningOfScript) + static_cast<unsigned int>(TH09_ANM_NATIVE_WIRE_INT(1)));
                continue;
            }
            break;
        case AnmOpcode_FlipX:
            vm->flip ^= (1 << 0);
            vm->scale.x *= -1.0f;
            vm->updateScale = true;
            break;
        case AnmOpcode_PosMode:
            vm->usePosOffset = TH09_ANM_NATIVE_WIRE_INT(0);
            break;
        case AnmOpcode_FlipY:
            vm->flip ^= (1 << 1);
            vm->scale.y *= -1.0f;
            vm->updateScale = true;
            break;
        case AnmOpcode_Rotate:
            vm->rotation.x = GET_FLOAT_VAR(0);
            vm->rotation.y = GET_FLOAT_VAR(1);
            vm->rotation.z = GET_FLOAT_VAR(2);

            vm->updateRotation = true;
            break;
        case AnmOpcode_AngularVelocity:
            vm->angleVel.x = GET_FLOAT_VAR(0);
            vm->angleVel.y = GET_FLOAT_VAR(1);
            vm->angleVel.z = GET_FLOAT_VAR(2);

            vm->updateRotation = true;
            break;
        case AnmOpcode_ScaleGrowth:
            vm->scaleGrowth.x = GET_FLOAT_VAR(0);
            vm->scaleGrowth.y = GET_FLOAT_VAR(1);
            break;
        case AnmOpcode_ScaleTimeLinear:
            vm->interpCurrentTimers[AnmInterp_Scale] = 0;

            vm->interpEndTimers[AnmInterp_Scale] = GET_INT_VAR(2);

            vm->interpModes[AnmInterp_Scale] = AnmInterpMode_Linear;
            vm->scaleInitial = vm->scale;

            vm->scaleFinal.x = GET_FLOAT_VAR(0);
            vm->scaleFinal.y = GET_FLOAT_VAR(1);
            break;
        case AnmOpcode_AlphaTimeLinear:
            vm->color1Initial.a = vm->color1Script.a;
            vm->color1Final.a = TH09_ANM_NATIVE_WIRE_INT(0);

            vm->interpCurrentTimers[AnmInterp_Alpha1] = 0;
            vm->interpEndTimers[AnmInterp_Alpha1] = GET_INT_VAR(1);
            vm->interpModes[AnmInterp_Alpha1] = AnmInterpMode_Linear;
            break;
        case AnmOpcode_AdditiveBlendMode:
            vm->blendMode = TH09_ANM_NATIVE_WIRE_INT(0) != 0;
            break;
        case AnmOpcode_BlendMode:
            vm->blendMode = TH09_ANM_NATIVE_WIRE_INT(0);
            break;
        case AnmOpcode_Pos:
            if (!vm->usePosOffset)
            {
                vm->pos = Float3(GET_FLOAT_VAR(0), GET_FLOAT_VAR(1), GET_FLOAT_VAR(2));
            }
            else
            {
                vm->pos2 = Float3(GET_FLOAT_VAR(0), GET_FLOAT_VAR(1), GET_FLOAT_VAR(2));
            }
            break;
        case AnmOpcode_PosTimeDecel2:
            vm->interpModes[AnmInterp_Pos] = AnmInterpMode_EaseOutQuartic;
            goto posTime;
        case AnmOpcode_PosTimeDecel:
            vm->interpModes[AnmInterp_Pos] = AnmInterpMode_EaseOut;
            goto posTime;
        case AnmOpcode_PosTimeLinear:
            vm->interpModes[AnmInterp_Pos] = AnmInterpMode_Linear;
        posTime:
            if (!vm->usePosOffset)
            {
                vm->posInitial = vm->pos;
            }
            else
            {
                vm->posInitial = vm->pos2;
            }

            vm->posFinal = Float3(GET_FLOAT_VAR(0), GET_FLOAT_VAR(1), GET_FLOAT_VAR(2));

            vm->interpEndTimers[AnmInterp_Pos] = GET_INT_VAR(3);
            vm->interpCurrentTimers[AnmInterp_Pos] = 0;
            break;
        case AnmOpcode_Wait:
            if (vm->waitTimer == 0)
            {
                vm->waitTimer = GET_INT_VAR(0);
            }
            else
            {
                vm->waitTimer--;
            }

            if (vm->waitTimer <= 0)
            {
                vm->waitTimer = 0;
                break;
            }
            vm->currentTimeInScript--;
            goto stop;
        case AnmOpcode_StopHide:
            vm->visible = false;
        case AnmOpcode_Stop:
            if (vm->pendingInterrupt == 0)
            {
                vm->stopped = true;
                vm->currentTimeInScript--;
                goto stop;
            }

        handleInterrupt:
            nextInstruction = NULL;
            instruction = vm->beginningOfScript;
            while (
                !(instruction->opcode == AnmOpcode_InterruptLabel && vm->pendingInterrupt == TH09_ANM_NATIVE_WIRE_INT(0)) &&
                instruction->opcode != AnmOpcode_EndOfScript)
            {
                if (instruction->opcode == AnmOpcode_InterruptLabel && TH09_ANM_NATIVE_WIRE_INT(0) == -1)
                {
                    nextInstruction = instruction;
                }
                instruction = reinterpret_cast<AnmRawInstr *>(reinterpret_cast<unsigned int>(instruction) + static_cast<unsigned int>(instruction->instructionSize));
            }

            vm->pendingInterrupt = 0;
            vm->stopped = false;

            if (instruction->opcode != AnmOpcode_InterruptLabel)
            {
                if (nextInstruction == NULL)
                {
                    vm->currentTimeInScript--;
                    goto stop;
                }
                instruction = nextInstruction;
            }

            vm->interruptReturnTime = vm->currentTimeInScript;
            vm->interruptReturnInstruction = vm->currentInstruction;
            instruction = reinterpret_cast<AnmRawInstr *>(reinterpret_cast<unsigned int>(instruction) + static_cast<unsigned int>(instruction->instructionSize));
            vm->currentInstruction = instruction;
            vm->currentTimeInScript = vm->currentInstruction->time;
            vm->visible = true;
            continue;
        case AnmOpcode_ReturnFromInterrupt:
            vm->currentTimeInScript = vm->interruptReturnTime;
            vm->currentInstruction = vm->interruptReturnInstruction;
            continue;
        case AnmOpcode_Visible:
            vm->visible = TH09_ANM_NATIVE_WIRE_INT(0);
            break;
        case AnmOpcode_AnchorTopLeft:
            vm->anchor = 3;
            break;
        case AnmOpcode_Ins25:
            vm->type = TH09_ANM_NATIVE_WIRE_INT(0);
            break;
        case AnmOpcode_AddU:
            vm->uvScrollPos.x += GET_FLOAT_VAR(0);
            ;
            if (vm->uvScrollPos.x >= 1.0f)
            {
                vm->uvScrollPos.x -= 1.0f;
            }
            else
            {
                if (vm->uvScrollPos.x < 0.0f)
                {
                    vm->uvScrollPos.x += 1.0f;
                }
            }
            break;
        case AnmOpcode_AddV:
            vm->uvScrollPos.y += GET_FLOAT_VAR(0);
            if (vm->uvScrollPos.y >= 1.0f)
            {
                vm->uvScrollPos.y -= 1.0f;
            }
            else
            {
                if (vm->uvScrollPos.y < 0.0f)
                {
                    vm->uvScrollPos.y += 1.0f;
                }
            }
            break;
        case AnmOpcode_UScroll:
            vm->uvScrollVel.x = GET_FLOAT_VAR(0);
            break;
        case AnmOpcode_VScroll:
            vm->uvScrollVel.y = GET_FLOAT_VAR(0);
            break;
        case AnmOpcode_ZWriteDisable:
            vm->disableZWrite = TH09_ANM_NATIVE_WIRE_INT(0);
            break;
        case AnmOpcode_Ins31:
            vm->flag15 = TH09_ANM_NATIVE_WIRE_INT(0);
            break;
        case AnmOpcode_PosTime:
            vm->interpCurrentTimers[AnmInterp_Pos] = 0;
            vm->interpEndTimers[AnmInterp_Pos] = GET_INT_VAR(0);
            vm->interpModes[AnmInterp_Pos] = TH09_ANM_NATIVE_WIRE_INT(1);

            if (!vm->usePosOffset)
            {
                vm->posInitial = vm->pos;
            }
            else
            {
                vm->posInitial = vm->pos2;
            }

            vm->posFinal.x = GET_FLOAT_VAR(2);
            vm->posFinal.y = GET_FLOAT_VAR(3);
            vm->posFinal.z = GET_FLOAT_VAR(4);
            break;
        case AnmOpcode_ColorTime:
            vm->interpCurrentTimers[AnmInterp_RGB1] = 0;

            vm->interpEndTimers[AnmInterp_RGB1] = GET_INT_VAR(0);

            vm->interpModes[AnmInterp_RGB1] = TH09_ANM_NATIVE_WIRE_INT(1);
            vm->color1Initial.r = vm->color1Script.r;
            vm->color1Initial.g = vm->color1Script.g;
            vm->color1Initial.b = vm->color1Script.b;

            vm->color1Final.r = GET_INT_VAR(2);
            vm->color1Final.g = GET_INT_VAR(3);
            vm->color1Final.b = GET_INT_VAR(4);
            break;
        case AnmOpcode_AlphaTime:
            vm->interpCurrentTimers[AnmInterp_Alpha1] = 0;
            vm->interpEndTimers[AnmInterp_Alpha1] = GET_INT_VAR(0);
            vm->interpModes[AnmInterp_Alpha1] = TH09_ANM_NATIVE_WIRE_INT(1);

            vm->color1Initial.a = vm->color1Script.a;
            vm->color1Final.a = GET_INT_VAR(2);
            break;
        case AnmOpcode_Color2Time:
            vm->interpCurrentTimers[AnmInterp_RGB2] = 0;

            vm->interpEndTimers[AnmInterp_RGB2] = GET_INT_VAR(0);

            vm->interpModes[AnmInterp_RGB2] = TH09_ANM_NATIVE_WIRE_INT(1);
            vm->color2Initial.r = vm->color2Script.r;
            vm->color2Initial.g = vm->color2Script.g;
            vm->color2Initial.b = vm->color2Script.b;

            vm->color2Final.r = GET_INT_VAR(2);
            vm->color2Final.g = GET_INT_VAR(3);
            vm->color2Final.b = GET_INT_VAR(4);
            break;
        case AnmOpcode_Alpha2Time:
            vm->interpCurrentTimers[AnmInterp_Alpha2] = 0;
            vm->interpEndTimers[AnmInterp_Alpha2] = GET_INT_VAR(0);
            vm->interpModes[AnmInterp_Alpha2] = TH09_ANM_NATIVE_WIRE_INT(1);

            vm->color2Initial.a = vm->color2Script.a;
            vm->color2Final.a = GET_INT_VAR(2);
            break;
        case AnmOpcode_RotateTime:
            vm->interpCurrentTimers[AnmInterp_Rotate] = 0;

            vm->interpEndTimers[AnmInterp_Rotate] = GET_INT_VAR(0);

            vm->interpModes[AnmInterp_Rotate] = TH09_ANM_NATIVE_WIRE_INT(1);
            vm->rotateInitial = vm->rotation;

            vm->rotateFinal.x = GET_FLOAT_VAR(2);
            vm->rotateFinal.y = GET_FLOAT_VAR(3);
            vm->rotateFinal.z = GET_FLOAT_VAR(4);

            vm->updateRotation = true;
            break;
        case AnmOpcode_ScaleTime:
            vm->interpCurrentTimers[AnmInterp_Scale] = 0;
            vm->interpEndTimers[AnmInterp_Scale] = GET_INT_VAR(0);

            vm->interpModes[AnmInterp_Scale] = TH09_ANM_NATIVE_WIRE_INT(1);
            vm->scaleInitial = vm->scale;

            vm->scaleFinal.x = GET_FLOAT_VAR(2);
            vm->scaleFinal.y = GET_FLOAT_VAR(3);
            vm->updateScale = true;
            break;
        case AnmOpcode_Ins83:
            vm->playerBulletHitAnimationType = TH09_ANM_NATIVE_WIRE_INT(0);
            break;
        case AnmOpcode_ISet:
            *GET_INT_VAR_PTR(0) = GET_INT_VAR(1);
            break;
        case AnmOpcode_FSet:
            *GET_FLOAT_VAR_PTR(0) = GET_FLOAT_VAR(1);
            break;
        // The original interpreter performs integer VM arithmetic as
        // wrapping 32-bit i386 DWORD operations, including negative inputs.
        case AnmOpcode_ISetAdd:
            *GET_INT_VAR_PTR(0) = static_cast<i32>(static_cast<u32>(GET_INT_VAR(1)) +
                static_cast<u32>(GET_INT_VAR(2)));
            break;
        case AnmOpcode_FSetAdd:
            *GET_FLOAT_VAR_PTR(0) = GET_FLOAT_VAR(1) + GET_FLOAT_VAR(2);
            break;
        case AnmOpcode_ISetSub:
            *GET_INT_VAR_PTR(0) = static_cast<i32>(static_cast<u32>(GET_INT_VAR(1)) -
                static_cast<u32>(GET_INT_VAR(2)));
            break;
        case AnmOpcode_FSetSub:
            *GET_FLOAT_VAR_PTR(0) = GET_FLOAT_VAR(1) - GET_FLOAT_VAR(2);
            break;
        case AnmOpcode_ISetMul:
            *GET_INT_VAR_PTR(0) = static_cast<i32>(static_cast<u32>(GET_INT_VAR(1)) *
                static_cast<u32>(GET_INT_VAR(2)));
            break;
        case AnmOpcode_FSetMul:
            *GET_FLOAT_VAR_PTR(0) = GET_FLOAT_VAR(1) * GET_FLOAT_VAR(2);
            break;
        case AnmOpcode_ISetDiv:
            *GET_INT_VAR_PTR(0) = GET_INT_VAR(1) / GET_INT_VAR(2);
            break;
        case AnmOpcode_FSetDiv:
            *GET_FLOAT_VAR_PTR(0) = GET_FLOAT_VAR(1) / GET_FLOAT_VAR(2);
            break;
        case AnmOpcode_ISetMod:
            *GET_INT_VAR_PTR(0) = GET_INT_VAR(1) % GET_INT_VAR(2);
            break;
        case AnmOpcode_FSetMod:
            *GET_FLOAT_VAR_PTR(0) = fmodf(GET_FLOAT_VAR(1), GET_FLOAT_VAR(2));
            break;
        case AnmOpcode_IAdd:
            *reinterpret_cast<u32 *>(GET_INT_VAR_PTR(0)) += static_cast<u32>(GET_INT_VAR(1));
            break;
        case AnmOpcode_FAdd:
            *GET_FLOAT_VAR_PTR(0) += GET_FLOAT_VAR(1);
            break;
        case AnmOpcode_ISub:
            *reinterpret_cast<u32 *>(GET_INT_VAR_PTR(0)) -= static_cast<u32>(GET_INT_VAR(1));
            break;
        case AnmOpcode_FSub:
            *GET_FLOAT_VAR_PTR(0) -= GET_FLOAT_VAR(1);
            break;
        case AnmOpcode_IMul:
            *reinterpret_cast<u32 *>(GET_INT_VAR_PTR(0)) *= static_cast<u32>(GET_INT_VAR(1));
            break;
        case AnmOpcode_FMul:
            *GET_FLOAT_VAR_PTR(0) *= GET_FLOAT_VAR(1);
            break;
        case AnmOpcode_IDiv:
            *GET_INT_VAR_PTR(0) /= GET_INT_VAR(1);
            break;
        case AnmOpcode_FDiv:
            *GET_FLOAT_VAR_PTR(0) /= GET_FLOAT_VAR(1);
            break;
        case AnmOpcode_IMod:
            *GET_INT_VAR_PTR(0) %= GET_INT_VAR(1);
            break;
        case AnmOpcode_FMod:
            *GET_FLOAT_VAR_PTR(0) = fmodf(GET_FLOAT_VAR(0), GET_FLOAT_VAR(1));
            break;
        case AnmOpcode_ISetRand:
            *GET_INT_VAR_PTR(0) = g_Rng.GetRandomU32InRange(GET_INT_VAR(1));
            break;
        case AnmOpcode_FSetRand:
            *GET_FLOAT_VAR_PTR(0) = g_Rng.GetRandomF32InRange(GET_FLOAT_VAR(1));
            break;
        case AnmOpcode_FSin:
            *GET_FLOAT_VAR_PTR(0) = sinf(GET_FLOAT_VAR(1));
            break;
        case AnmOpcode_FCos:
            *GET_FLOAT_VAR_PTR(0) = cosf(GET_FLOAT_VAR(1));
            break;
        case AnmOpcode_FTan:
            *GET_FLOAT_VAR_PTR(0) = tanf(GET_FLOAT_VAR(1));
            break;
        case AnmOpcode_FAcos:
            *GET_FLOAT_VAR_PTR(0) = acosf(GET_FLOAT_VAR(1));
            break;
        case AnmOpcode_FAtan:
            *GET_FLOAT_VAR_PTR(0) = atanf(GET_FLOAT_VAR(1));
            break;
        case AnmOpcode_NormalizeAngle:
            *GET_FLOAT_VAR_PTR(0) = AddNormalizeAngle(GET_FLOAT_VAR(0), 0);
            break;
        case AnmOpcode_IJmpEq:
            if (GET_INT_VAR(0) == GET_INT_VAR(1))
            {
                goto jump;
            }
            break;
        case AnmOpcode_FJmpEq:
            if (GET_FLOAT_VAR(0) == GET_FLOAT_VAR(1))
            {
                goto jump;
            }
            break;
        case AnmOpcode_IJmpNeq:
            if (GET_INT_VAR(0) != GET_INT_VAR(1))
            {
                goto jump;
            }
            break;
        case AnmOpcode_FJmpNeq:
            if (GET_FLOAT_VAR(0) != GET_FLOAT_VAR(1))
            {
                goto jump;
            }
            break;
        case AnmOpcode_IJmpLess:
            if (GET_INT_VAR(0) < GET_INT_VAR(1))
            {
                goto jump;
            }
            break;
        case AnmOpcode_FJmpLess:
            if (GET_FLOAT_VAR(0) < GET_FLOAT_VAR(1))
            {
                goto jump;
            }
            break;
        case AnmOpcode_IJmpLessOrEq:
            if (GET_INT_VAR(0) <= GET_INT_VAR(1))
            {
                goto jump;
            }
            break;
        case AnmOpcode_FJmpLessOrEq:
            if (GET_FLOAT_VAR(0) <= GET_FLOAT_VAR(1))
            {
                goto jump;
            }
            break;
        case AnmOpcode_IJmpGreater:
            if (GET_INT_VAR(0) > GET_INT_VAR(1))
            {
                goto jump;
            }
            break;
        case AnmOpcode_FJmpGreater:
            if (GET_FLOAT_VAR(0) > GET_FLOAT_VAR(1))
            {
                goto jump;
            }
            break;
        case AnmOpcode_IJmpGreaterOrEq:
            if (GET_INT_VAR(0) >= GET_INT_VAR(1))
            {
                goto jump;
            }
            break;
        case AnmOpcode_FJmpGreaterOrEq:
            if (GET_FLOAT_VAR(0) >= GET_FLOAT_VAR(1))
            {
                goto jump;
            }
            break;
        case AnmOpcode_Ins88:
            vm->flag17 = instruction->byteArgs[1];
            break;
        jump:
            vm->currentTimeInScript = TH09_ANM_NATIVE_WIRE_INT(3);
            vm->currentInstruction = reinterpret_cast<AnmRawInstr *>(reinterpret_cast<unsigned int>(vm->beginningOfScript) + static_cast<unsigned int>(TH09_ANM_NATIVE_WIRE_INT(2)));
            continue;
        default:
            break;
        }
#undef TH09_ANM_NATIVE_WIRE_FLOAT
#undef TH09_ANM_NATIVE_WIRE_INT
#undef GET_FLOAT_VAR_PTR
#undef GET_INT_VAR_PTR
#undef GET_FLOAT_VAR
#undef GET_INT_VAR

        vm->currentInstruction = reinterpret_cast<AnmRawInstr *>(reinterpret_cast<unsigned int>(instruction) + static_cast<unsigned int>(instruction->instructionSize));
    }
stop:
    if (vm->angleVel.x != 0.0f)
    {
        vm->rotation.x = AddNormalizeAngle(vm->rotation.x, g_Supervisor.framerateMultiplier5B8 * vm->angleVel.x);
        vm->updateRotation = true;
    }

    if (vm->angleVel.y != 0.0f)
    {
        vm->rotation.y = AddNormalizeAngle(vm->rotation.y, g_Supervisor.framerateMultiplier5B8 * vm->angleVel.y);
        vm->updateRotation = true;
    }

    if (vm->angleVel.z != 0.0f)
    {
        vm->rotation.z = AddNormalizeAngle(vm->rotation.z, g_Supervisor.framerateMultiplier5B8 * vm->angleVel.z);
        vm->updateRotation = true;
    }

    // The seven interpolator channels occupy the original AnmVm arrays:
    // 0x0C-byte ZunTimer records at +0x50/+0xA4 and mode bytes at +0xF8.
    // Keep independent live per-channel reads and native 32-bit offsets.
#define TH09_ANM_NATIVE_INTERP_TIMER(member, index) \
    (*reinterpret_cast<ZunTimer *>( \
        reinterpret_cast<unsigned int>(&vm->member[0]) + \
        static_cast<unsigned int>(index) * sizeof(vm->member[0])))
#define TH09_ANM_NATIVE_INTERP_MODE(index) \
    (*reinterpret_cast<unsigned char *>( \
        reinterpret_cast<unsigned int>(&vm->interpModes[0]) + \
        static_cast<unsigned int>(index)))
    for (i = 0; i < AnmInterp_Last; i++)
    {
        if (TH09_ANM_NATIVE_INTERP_TIMER(interpEndTimers, i) > 0)
        {
            TH09_ANM_NATIVE_INTERP_TIMER(interpCurrentTimers, i)++;
            if (TH09_ANM_NATIVE_INTERP_TIMER(interpCurrentTimers, i) >= (int)TH09_ANM_NATIVE_INTERP_TIMER(interpEndTimers, i))
            {
                interp = 1.0f;
                TH09_ANM_NATIVE_INTERP_TIMER(interpEndTimers, i) = 0;
            }
            else
            {
                interp = (float)TH09_ANM_NATIVE_INTERP_TIMER(interpCurrentTimers, i) / (float)TH09_ANM_NATIVE_INTERP_TIMER(interpEndTimers, i);
            }

            switch (TH09_ANM_NATIVE_INTERP_MODE(i))
            {
            case AnmInterpMode_EaseIn:
                interp = interp * interp;
                break;
            case AnmInterpMode_EaseInCubic:
                interp = interp * interp * interp;
                break;
            case AnmInterpMode_EaseInQuartic:
                interp = interp * interp;
                interp = interp * interp;
                break;
            case AnmInterpMode_EaseOut:
                interp = (1.0f - interp);
                interp *= interp;
                interp = (1.0f - interp);
                break;
            case AnmInterpMode_EaseOutCubic:
                interp = (1.0f - interp);
                interp = interp * interp * interp;
                interp = (1.0f - interp);
                break;
            case AnmInterpMode_EaseOutQuartic:
                interp = (1.0f - interp);
                interp = interp * interp;
                interp = interp * interp;
                interp = (1.0f - interp);
                break;
            }

            switch (i)
            {
            case AnmInterp_Pos:
                if (!vm->usePosOffset)
                {
                    vm->pos.x = interp * (vm->posFinal.x - vm->posInitial.x) + vm->posInitial.x;
                    vm->pos.y = interp * (vm->posFinal.y - vm->posInitial.y) + vm->posInitial.y;
                    vm->pos.z = interp * (vm->posFinal.z - vm->posInitial.z) + vm->posInitial.z;
                }
                else
                {
                    vm->pos2.x = interp * (vm->posFinal.x - vm->posInitial.x) + vm->posInitial.x;
                    vm->pos2.y = interp * (vm->posFinal.y - vm->posInitial.y) + vm->posInitial.y;
                    vm->pos2.z = interp * (vm->posFinal.z - vm->posInitial.z) + vm->posInitial.z;
                }
                break;
            case AnmInterp_RGB1:
                vm->color1Script.r = interp * ((float)vm->color1Final.r - vm->color1Initial.r) + vm->color1Initial.r;
                vm->color1Script.g = interp * ((float)vm->color1Final.g - vm->color1Initial.g) + vm->color1Initial.g;
                vm->color1Script.b = interp * ((float)vm->color1Final.b - vm->color1Initial.b) + vm->color1Initial.b;
                break;
            case AnmInterp_Alpha1:
                vm->color1Script.a = interp * ((float)vm->color1Final.a - vm->color1Initial.a) + vm->color1Initial.a;
                break;
            case AnmInterp_RGB2:
                vm->color2Script.r = interp * ((float)vm->color2Final.r - vm->color2Initial.r) + vm->color2Initial.r;
                vm->color2Script.g = interp * ((float)vm->color2Final.g - vm->color2Initial.g) + vm->color2Initial.g;
                vm->color2Script.b = interp * ((float)vm->color2Final.b - vm->color2Initial.b) + vm->color2Initial.b;
                break;
            case AnmInterp_Alpha2:
                vm->color2Script.a = interp * ((float)vm->color2Final.a - vm->color2Initial.a) + vm->color2Initial.a;
                break;
            case AnmInterp_Rotate:
                vm->rotation.x =
                    AddNormalizeAngle((vm->rotateFinal.x - vm->rotateInitial.x) * interp, vm->rotateInitial.x);
                vm->rotation.y =
                    AddNormalizeAngle((vm->rotateFinal.y - vm->rotateInitial.y) * interp, vm->rotateInitial.y);
                vm->rotation.z =
                    AddNormalizeAngle((vm->rotateFinal.z - vm->rotateInitial.z) * interp, vm->rotateInitial.z);
                vm->updateRotation = true;
                break;
            case AnmInterp_Scale:
                vm->scale.x = interp * (vm->scaleFinal.x - vm->scaleInitial.x) + vm->scaleInitial.x;
                vm->scale.y = interp * (vm->scaleFinal.y - vm->scaleInitial.y) + vm->scaleInitial.y;
                vm->updateScale = true;
                break;
            }
        }
    }

    if (vm->scaleGrowth.y != 0.0f)
    {
        vm->scale.y += g_Supervisor.framerateMultiplier5B8 * vm->scaleGrowth.y;
        vm->updateScale = true;
    }

    if (vm->scaleGrowth.x != 0.0f)
    {
        vm->scale.x += g_Supervisor.framerateMultiplier5B8 * vm->scaleGrowth.x;
        vm->updateScale = true;
        vm->updateRotation = true;
    }

    vm->uvScrollPos.x += vm->uvScrollVel.x;

    if (vm->uvScrollPos.x >= 1.0f)
    {
        vm->uvScrollPos.x -= 1.0f;
    }
    else
    {
        if (vm->uvScrollPos.x < 0.0f)
        {
            vm->uvScrollPos.x += 1.0f;
        }
    }

    vm->uvScrollPos.y += vm->uvScrollVel.y;
    if (vm->uvScrollPos.y >= 1.0f)
    {
        vm->uvScrollPos.y -= 1.0f;
    }
    else
    {
        if (vm->uvScrollPos.y < 0.0f)
        {
            vm->uvScrollPos.y += 1.0f;
        }
    }

    vm->currentTimeInScript++;
    ++*reinterpret_cast<u32 *>(&this->scriptsExecutedThisFrame10);

    return FALSE;
}

#undef TH09_ANM_NATIVE_INTERP_TIMER
#undef TH09_ANM_NATIVE_INTERP_MODE
