// Target-bound TH09 ANM script execution view.
// The TH08 family supplies source shape; TH09 target evidence fixes the ABI,
// VM layout, opcode domain, out-of-line operand helpers, and physical boundary.

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

struct Float2 { float x; float y; };
struct Float3 {
    Float3(float x, float y, float z);
    float x; float y; float z;
};
union ZunColor {
    u32 d3dColor;
    struct { u8 b; u8 g; u8 r; u8 a; };
};
struct AnmMatrix { float m[4][4]; };

struct ZunTimer {
    int previous;
    float subFrame;
    int current;
    void SetCurrent(int value);
    int Tick();
    operator int();
    operator float();
    void operator++(int);
    void operator--(int);
    void operator=(int value);
    unsigned int operator>=(int value);
    unsigned int operator<(int value);
    // These comparisons are out of line in the original ExecuteScript TU.
    // Their callsites are part of the target-exact Wait/interpolation paths.
    ZunBool operator==(int value);
    ZunBool operator<=(int value);
    ZunBool operator>(int value);
};

class AnmLoaded;
struct AnmLoadedSprite;
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

#define ANM_MAX_ARGS 10

struct AnmRawInstr
{
    i16 opcode;
    u16 instructionSize;
    i16 time;
    u16 varMask;
    union {
        i32 intArgs[ANM_MAX_ARGS];
        f32 floatArgs[ANM_MAX_ARGS];
        u8 byteArgs[ANM_MAX_ARGS * sizeof(i32)];
    };
};

class AnmLoaded {
public:
    int SetSprite(struct AnmVm *vm, int spriteIndex);
};

struct AnmLoadedSprite { unsigned char opaque[0x44]; };

struct AnmVm {
    Float3 rotation;                    // +0x000
    Float3 angleVel;                    // +0x00C
    Float2 scale;                       // +0x018
    Float2 scaleGrowth;                 // +0x020
    Float2 spriteSize;                  // +0x028
    Float2 uvScrollPos;                 // +0x030
    ZunTimer currentTimeInScript;       // +0x038
    ZunTimer waitTimer;                 // +0x044
    ZunTimer interpCurrentTimers[AnmInterp_Last]; // +0x050
    ZunTimer interpEndTimers[AnmInterp_Last];     // +0x0A4
    u8 interpModes[AnmInterp_Last];     // +0x0F8
    u8 interpPadding;                   // +0x0FF
    i32 intVar0;                        // +0x100
    i32 intVar1;
    i32 intVar2;
    i32 intVar3;
    f32 floatVar0;                      // +0x110
    f32 floatVar1;
    f32 floatVar2;
    f32 floatVar3;
    i32 counterVar0;                    // +0x120
    i32 counterVar1;
    Float2 uvScrollVel;                 // +0x128
    AnmMatrix matrix1;                  // +0x130
    AnmMatrix matrix2;                  // +0x170
    AnmMatrix matrix3;                  // +0x1B0
    ZunColor color1;                    // +0x1F0
    ZunColor color2;                    // +0x1F4
    union {
        u32 flagsWord;
        struct {
            u32 visible : 1;
            u32 flag1 : 1;
            u32 updateRotation : 1;
            u32 updateScale : 1;
            u32 blendMode : 2;
            u32 flag6 : 1;
            u32 flag7 : 1;
            u32 usePosOffset : 1;
            u32 flip : 2;
            u32 anchor : 2;
            u32 zWriteDisabled : 1;
            u32 stopped : 1;
            u32 flag15 : 1;
            u32 flag16 : 1;
            u32 flag17 : 1;
            u32 flag18 : 1;
            u32 flag19 : 1;
            u32 unknownFlags20_31 : 12;
        };
    };                                  // +0x1F8
    short type;                         // +0x1FC
    short pendingInterrupt;             // +0x1FE
    i32 playerBulletHitAnimationType;   // +0x200
    AnmLoaded *anmFile;                 // +0x204
    Float3 pos;                         // +0x208
    short activeSpriteIndex;            // +0x214
    short anmFileIndex;                 // +0x216
    short baseSpriteIndex;              // +0x218
    short scriptIndex;                  // +0x21A
    AnmRawInstr *beginningOfScript;     // +0x21C
    AnmRawInstr *currentInstruction;    // +0x220
    AnmLoadedSprite *loadedSprite;      // +0x224
    ZunTimer interruptReturnTime;       // +0x228
    AnmRawInstr *interruptReturnInstruction; // +0x234
    Float3 posInitial;                  // +0x238
    Float3 posFinal;                    // +0x244
    Float3 rotateInitial;               // +0x250
    Float3 rotateFinal;                 // +0x25C
    Float2 scaleInitial;                // +0x268
    Float2 scaleFinal;                  // +0x270
    ZunColor color1Initial;             // +0x278
    ZunColor color1Final;               // +0x27C
    ZunColor color2Initial;             // +0x280
    ZunColor color2Final;               // +0x284
    Float3 pos2;                        // +0x288
    i32 timeOfLastSpriteSet;            // +0x294
    u8 fontWidth;                       // +0x298
    u8 fontHeight;                      // +0x299
    u8 unknown29A[0x0A];

    f32 GetFloatVar(f32 varId);
    i32 GetIntVar(i32 varId);
    f32 *GetFloatVarPtr(f32 *varPtr, u16 varMask, u32 variableNumber);
    i32 *GetIntVarPtr(i32 *varPtr, u16 varMask, u32 variableNumber);
};

typedef char AnmExecVmSizeIs2A4[(sizeof(AnmVm) == 0x2A4) ? 1 : -1];
typedef char AnmExecCurrentTimeAt38[(offsetof(AnmVm,currentTimeInScript)==0x38)?1:-1];
typedef char AnmExecIntVarsAt100[(offsetof(AnmVm,intVar0)==0x100)?1:-1];
typedef char AnmExecFlagsAt1F8[(offsetof(AnmVm,flagsWord)==0x1F8)?1:-1];
typedef char AnmExecPendingAt1FE[(offsetof(AnmVm,pendingInterrupt)==0x1FE)?1:-1];
typedef char AnmExecAnmFileAt204[(offsetof(AnmVm,anmFile)==0x204)?1:-1];
typedef char AnmExecCurrentInstructionAt220[(offsetof(AnmVm,currentInstruction)==0x220)?1:-1];
typedef char AnmExecLastSpriteTimeAt294[(offsetof(AnmVm,timeOfLastSpriteSet)==0x294)?1:-1];

struct ReplayRngAnmView {
    u32 GetRandomU32InRange(u32 max);
    f32 GetRandomF32();
    f32 GetRandomF32Signed();
    f32 GetRandomF32InRange(f32 max);
};
extern ReplayRngAnmView g_ReplayRng;

struct SupervisorAnmExecutionView {
    unsigned char unknown000[0x5B8];
    float framerateMultiplier;
};
extern SupervisorAnmExecutionView g_Supervisor;

float AddNormalizeAngle(float value, float base);

class AnmManager {
public:
    unsigned char unknown000[0x10];
    int scriptsExecutedThisFrame;
    int ExecuteScript(AnmVm *vm);
};


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

f32 *AnmVm::GetFloatVarPtr(f32 *varPtr, u16 varMask, u32 variableNumber)
{
    if ((varMask & (1 << variableNumber)) == 0) return varPtr;
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
    if ((varMask & (1 << variableNumber)) == 0) return varPtr;
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
    case 10010: return g_ReplayRng.GetRandomF32();
    case 10011: return g_ReplayRng.GetRandomF32Signed();
    case 10012: return g_ReplayRng.GetRandomF32InRange(3.1415927f);
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
#define GET_INT_VAR(argNumber)                                                                                         \
    ((instruction->varMask & (1 << argNumber)) ? vm->GetIntVar(instruction->intArgs[argNumber])                        \
                                               : instruction->intArgs[argNumber])
#define GET_FLOAT_VAR(argNumber)                                                                                       \
    ((instruction->varMask & (1 << argNumber)) ? vm->GetFloatVar(instruction->floatArgs[argNumber])                    \
                                               : instruction->floatArgs[argNumber])

#define GET_INT_VAR_PTR(idx) vm->GetIntVarPtr(&instruction->intArgs[idx], instruction->varMask, idx)

#define GET_FLOAT_VAR_PTR(idx) vm->GetFloatVarPtr(&instruction->floatArgs[idx], instruction->varMask, idx)

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
            vm->color1.a = GET_INT_VAR(0);
            break;
        case AnmOpcode_Color:
            vm->color1.r = GET_INT_VAR(0);
            vm->color1.g = GET_INT_VAR(1);
            vm->color1.b = GET_INT_VAR(2);
            break;
        case AnmOpcode_Alpha2:
            vm->color2.a = GET_INT_VAR(0);
            break;
        case AnmOpcode_Color2:
            vm->color2.r = GET_INT_VAR(0);
            vm->color2.g = GET_INT_VAR(1);
            vm->color2.b = GET_INT_VAR(2);
            break;
        case AnmOpcode_Jmp:
            vm->currentTimeInScript = instruction->intArgs[1];
            vm->currentInstruction = (AnmRawInstr *)(((u8 *)vm->beginningOfScript) + instruction->intArgs[0]);
            continue;
        case AnmOpcode_JmpDec:
            *GET_INT_VAR_PTR(0) -= 1;

            if (GET_INT_VAR(0) > 0)
            {
                vm->currentTimeInScript = instruction->intArgs[2];
                vm->currentInstruction = (AnmRawInstr *)(((u8 *)vm->beginningOfScript) + instruction->intArgs[1]);
                continue;
            }
            break;
        case AnmOpcode_FlipX:
            vm->flip ^= (1 << 0);
            vm->scale.x *= -1.0f;
            vm->updateScale = true;
            break;
        case AnmOpcode_PosMode:
            vm->usePosOffset = instruction->intArgs[0];
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
            vm->color1Initial.a = vm->color1.a;
            vm->color1Final.a = instruction->intArgs[0];

            vm->interpCurrentTimers[AnmInterp_Alpha1] = 0;
            vm->interpEndTimers[AnmInterp_Alpha1] = GET_INT_VAR(1);
            vm->interpModes[AnmInterp_Alpha1] = AnmInterpMode_Linear;
            break;
        case AnmOpcode_AdditiveBlendMode:
            vm->blendMode = instruction->intArgs[0] != 0;
            break;
        case AnmOpcode_BlendMode:
            vm->blendMode = instruction->intArgs[0];
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
                !(instruction->opcode == AnmOpcode_InterruptLabel && vm->pendingInterrupt == instruction->intArgs[0]) &&
                instruction->opcode != AnmOpcode_EndOfScript)
            {
                if (instruction->opcode == AnmOpcode_InterruptLabel && instruction->intArgs[0] == -1)
                {
                    nextInstruction = instruction;
                }
                instruction = (AnmRawInstr *)((u8 *)instruction + instruction->instructionSize);
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
            instruction = (AnmRawInstr *)((u8 *)instruction + instruction->instructionSize);
            vm->currentInstruction = instruction;
            vm->currentTimeInScript = vm->currentInstruction->time;
            vm->visible = true;
            continue;
        case AnmOpcode_ReturnFromInterrupt:
            vm->currentTimeInScript = vm->interruptReturnTime;
            vm->currentInstruction = vm->interruptReturnInstruction;
            continue;
        case AnmOpcode_Visible:
            vm->visible = instruction->intArgs[0];
            break;
        case AnmOpcode_AnchorTopLeft:
            vm->anchor = 3;
            break;
        case AnmOpcode_Ins25:
            vm->type = instruction->intArgs[0];
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
            vm->zWriteDisabled = instruction->intArgs[0];
            break;
        case AnmOpcode_Ins31:
            vm->flag15 = instruction->intArgs[0];
            break;
        case AnmOpcode_PosTime:
            vm->interpCurrentTimers[AnmInterp_Pos] = 0;
            vm->interpEndTimers[AnmInterp_Pos] = GET_INT_VAR(0);
            vm->interpModes[AnmInterp_Pos] = instruction->intArgs[1];

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

            vm->interpModes[AnmInterp_RGB1] = instruction->intArgs[1];
            vm->color1Initial.r = vm->color1.r;
            vm->color1Initial.g = vm->color1.g;
            vm->color1Initial.b = vm->color1.b;

            vm->color1Final.r = GET_INT_VAR(2);
            vm->color1Final.g = GET_INT_VAR(3);
            vm->color1Final.b = GET_INT_VAR(4);
            break;
        case AnmOpcode_AlphaTime:
            vm->interpCurrentTimers[AnmInterp_Alpha1] = 0;
            vm->interpEndTimers[AnmInterp_Alpha1] = GET_INT_VAR(0);
            vm->interpModes[AnmInterp_Alpha1] = instruction->intArgs[1];

            vm->color1Initial.a = vm->color1.a;
            vm->color1Final.a = GET_INT_VAR(2);
            break;
        case AnmOpcode_Color2Time:
            vm->interpCurrentTimers[AnmInterp_RGB2] = 0;

            vm->interpEndTimers[AnmInterp_RGB2] = GET_INT_VAR(0);

            vm->interpModes[AnmInterp_RGB2] = instruction->intArgs[1];
            vm->color2Initial.r = vm->color2.r;
            vm->color2Initial.g = vm->color2.g;
            vm->color2Initial.b = vm->color2.b;

            vm->color2Final.r = GET_INT_VAR(2);
            vm->color2Final.g = GET_INT_VAR(3);
            vm->color2Final.b = GET_INT_VAR(4);
            break;
        case AnmOpcode_Alpha2Time:
            vm->interpCurrentTimers[AnmInterp_Alpha2] = 0;
            vm->interpEndTimers[AnmInterp_Alpha2] = GET_INT_VAR(0);
            vm->interpModes[AnmInterp_Alpha2] = instruction->intArgs[1];

            vm->color2Initial.a = vm->color2.a;
            vm->color2Final.a = GET_INT_VAR(2);
            break;
        case AnmOpcode_RotateTime:
            vm->interpCurrentTimers[AnmInterp_Rotate] = 0;

            vm->interpEndTimers[AnmInterp_Rotate] = GET_INT_VAR(0);

            vm->interpModes[AnmInterp_Rotate] = instruction->intArgs[1];
            vm->rotateInitial = vm->rotation;

            vm->rotateFinal.x = GET_FLOAT_VAR(2);
            vm->rotateFinal.y = GET_FLOAT_VAR(3);
            vm->rotateFinal.z = GET_FLOAT_VAR(4);

            vm->updateRotation = true;
            break;
        case AnmOpcode_ScaleTime:
            vm->interpCurrentTimers[AnmInterp_Scale] = 0;
            vm->interpEndTimers[AnmInterp_Scale] = GET_INT_VAR(0);

            vm->interpModes[AnmInterp_Scale] = instruction->intArgs[1];
            vm->scaleInitial = vm->scale;

            vm->scaleFinal.x = GET_FLOAT_VAR(2);
            vm->scaleFinal.y = GET_FLOAT_VAR(3);
            vm->updateScale = true;
            break;
        case AnmOpcode_Ins83:
            vm->playerBulletHitAnimationType = instruction->intArgs[0];
            break;
        case AnmOpcode_ISet:
            *GET_INT_VAR_PTR(0) = GET_INT_VAR(1);
            break;
        case AnmOpcode_FSet:
            *GET_FLOAT_VAR_PTR(0) = GET_FLOAT_VAR(1);
            break;
        case AnmOpcode_ISetAdd:
            *GET_INT_VAR_PTR(0) = GET_INT_VAR(1) + GET_INT_VAR(2);
            break;
        case AnmOpcode_FSetAdd:
            *GET_FLOAT_VAR_PTR(0) = GET_FLOAT_VAR(1) + GET_FLOAT_VAR(2);
            break;
        case AnmOpcode_ISetSub:
            *GET_INT_VAR_PTR(0) = GET_INT_VAR(1) - GET_INT_VAR(2);
            break;
        case AnmOpcode_FSetSub:
            *GET_FLOAT_VAR_PTR(0) = GET_FLOAT_VAR(1) - GET_FLOAT_VAR(2);
            break;
        case AnmOpcode_ISetMul:
            *GET_INT_VAR_PTR(0) = GET_INT_VAR(1) * GET_INT_VAR(2);
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
            *GET_INT_VAR_PTR(0) += GET_INT_VAR(1);
            break;
        case AnmOpcode_FAdd:
            *GET_FLOAT_VAR_PTR(0) += GET_FLOAT_VAR(1);
            break;
        case AnmOpcode_ISub:
            *GET_INT_VAR_PTR(0) -= GET_INT_VAR(1);
            break;
        case AnmOpcode_FSub:
            *GET_FLOAT_VAR_PTR(0) -= GET_FLOAT_VAR(1);
            break;
        case AnmOpcode_IMul:
            *GET_INT_VAR_PTR(0) *= GET_INT_VAR(1);
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
            *GET_INT_VAR_PTR(0) = g_ReplayRng.GetRandomU32InRange(GET_INT_VAR(1));
            break;
        case AnmOpcode_FSetRand:
            *GET_FLOAT_VAR_PTR(0) = g_ReplayRng.GetRandomF32InRange(GET_FLOAT_VAR(1));
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
            vm->currentTimeInScript = instruction->intArgs[3];
            vm->currentInstruction = (AnmRawInstr *)(((u8 *)vm->beginningOfScript) + instruction->intArgs[2]);
            continue;
        default:
            break;
        }
#undef GET_FLOAT_VAR_PTR
#undef GET_INT_VAR_PTR
#undef GET_FLOAT_VAR
#undef GET_INT_VAR

        vm->currentInstruction = (AnmRawInstr *)((u8 *)instruction + instruction->instructionSize);
    }
stop:
    if (vm->angleVel.x != 0.0f)
    {
        vm->rotation.x = AddNormalizeAngle(vm->rotation.x, g_Supervisor.framerateMultiplier * vm->angleVel.x);
        vm->updateRotation = true;
    }

    if (vm->angleVel.y != 0.0f)
    {
        vm->rotation.y = AddNormalizeAngle(vm->rotation.y, g_Supervisor.framerateMultiplier * vm->angleVel.y);
        vm->updateRotation = true;
    }

    if (vm->angleVel.z != 0.0f)
    {
        vm->rotation.z = AddNormalizeAngle(vm->rotation.z, g_Supervisor.framerateMultiplier * vm->angleVel.z);
        vm->updateRotation = true;
    }

    for (i = 0; i < AnmInterp_Last; i++)
    {
        if (vm->interpEndTimers[i] > 0)
        {
            vm->interpCurrentTimers[i]++;
            if (vm->interpCurrentTimers[i] >= (int)vm->interpEndTimers[i])
            {
                interp = 1.0f;
                vm->interpEndTimers[i] = 0;
            }
            else
            {
                interp = (float)vm->interpCurrentTimers[i] / (float)vm->interpEndTimers[i];
            }

            switch (vm->interpModes[i])
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
                vm->color1.r = interp * ((float)vm->color1Final.r - vm->color1Initial.r) + vm->color1Initial.r;
                vm->color1.g = interp * ((float)vm->color1Final.g - vm->color1Initial.g) + vm->color1Initial.g;
                vm->color1.b = interp * ((float)vm->color1Final.b - vm->color1Initial.b) + vm->color1Initial.b;
                break;
            case AnmInterp_Alpha1:
                vm->color1.a = interp * ((float)vm->color1Final.a - vm->color1Initial.a) + vm->color1Initial.a;
                break;
            case AnmInterp_RGB2:
                vm->color2.r = interp * ((float)vm->color2Final.r - vm->color2Initial.r) + vm->color2Initial.r;
                vm->color2.g = interp * ((float)vm->color2Final.g - vm->color2Initial.g) + vm->color2Initial.g;
                vm->color2.b = interp * ((float)vm->color2Final.b - vm->color2Initial.b) + vm->color2Initial.b;
                break;
            case AnmInterp_Alpha2:
                vm->color2.a = interp * ((float)vm->color2Final.a - vm->color2Initial.a) + vm->color2Initial.a;
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
        vm->scale.y += g_Supervisor.framerateMultiplier * vm->scaleGrowth.y;
        vm->updateScale = true;
    }

    if (vm->scaleGrowth.x != 0.0f)
    {
        vm->scale.x += g_Supervisor.framerateMultiplier * vm->scaleGrowth.x;
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
    this->scriptsExecutedThisFrame++;

    return FALSE;
}
