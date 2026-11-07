#pragma once

#include "ZunTimer.hpp"
#include "Float3.hpp"
#include <stddef.h>

class AnmLoaded;
class AnmLoadedSprite;
struct AnmRawInstr;

// Four byte original sprite-script channel view.  The native ANM and Etama
// renderers also consume the same dword as an integral color, so both
// interpretations must address the same physical +0x1F0/+0x1F4 words.
struct AnmVmScriptColor
{
    union {
        unsigned int d3dColor;
        struct { unsigned char b, g, r, a; };
    };
};
typedef char AnmVmScriptColorSizeIs4[
    (sizeof(AnmVmScriptColor) == 4) ? 1 : -1];

struct AnmVmScriptMatrix
{
    float m[4][4];
};
typedef char AnmVmScriptMatrixSizeIs40[
    (sizeof(AnmVmScriptMatrix) == 0x40) ? 1 : -1];

// One target-observed base prefix and VM tail;original nominal classes are unknown.
struct AnmVmBase
{
    AnmVmBase();

    Float3 rotation;
    Float3 angleVel;
    Float2 scale;
    Float2 scaleGrowth;
    Float2 spriteSize;
    Float2 uvScrollPos;
    ZunTimer currentTimeInScript;
    ZunTimer waitTimer;
    ZunTimer interpCurrentTimers[7];
    ZunTimer interpEndTimers[7];
    unsigned char interpModes[7];
    unsigned char interpPadding;
    int intVar0, intVar1, intVar2, intVar3;
    float floatVar0, floatVar1, floatVar2, floatVar3;
    int counterVar0, counterVar1;
    Float2 uvScrollVel;
    AnmVmScriptMatrix matrix1, matrix2, matrix3;
    union { unsigned long color1; AnmVmScriptColor color1Script; unsigned char color1Bytes[4]; };
    union { unsigned long color2; AnmVmScriptColor color2Script; };
    union {
        unsigned int flagsWord;
        unsigned int flags;
        struct { unsigned short flagsLow; unsigned short flagsHigh; };
        struct {
            unsigned int visible : 1;
            unsigned int drawEnabled : 1;
            unsigned int updateRotation : 1;
            unsigned int updateScale : 1;
            unsigned int blendMode : 2;
            unsigned int flag6 : 1;
            unsigned int flag7 : 1;
            unsigned int usePosOffset : 1;
            unsigned int flip : 2;
            unsigned int anchor : 2;
            unsigned int disableZWrite : 1;
            unsigned int stopped : 1;
            unsigned int flag15 : 1;
            unsigned int flag16 : 1;
            unsigned int flag17 : 1;
            unsigned int flag18 : 1;
            unsigned int flag19 : 1;
            unsigned int unknownFlags20_31 : 12;
        };
    };
    union { unsigned char unknown_01FC[2]; short type; };
    short pendingInterrupt;
    int playerBulletHitAnimationType;
    AnmLoaded *anmFile;
    Float3 pos;
    short activeSpriteIndex;
    short anmFileIndex;
    short baseSpriteIndex;
    short scriptIndex;
    AnmRawInstr *beginningOfScript;
    AnmRawInstr *currentInstruction;
    AnmLoadedSprite *loadedSprite;
};
typedef char AnmVmBaseSizeIs228[(sizeof(AnmVmBase) == 0x228) ? 1 : -1];
typedef char AnmVmNativeDrawTypeAt200[
    (offsetof(AnmVmBase, playerBulletHitAnimationType) == 0x200) ? 1 : -1];
typedef char AnmVmNativeBeginningOfScriptAt21C[
    (offsetof(AnmVmBase, beginningOfScript) == 0x21C) ? 1 : -1];
typedef char AnmVmNativeFileIndexAt216[
    (offsetof(AnmVmBase, anmFileIndex) == 0x216) ? 1 : -1];
typedef char AnmVmNativeBaseSpriteAt218[
    (offsetof(AnmVmBase, baseSpriteIndex) == 0x218) ? 1 : -1];


struct AnmVm : AnmVmBase
{
    AnmVm();
    void Initialize();
    void SetInterrupt(short interrupt);
    int IsVisible();
    void SetInvisible();
    void SetZRotation(float angle);

    float GetFloatVar(float varId);
    int GetIntVar(int varId);
    float *GetFloatVarPtr(float *varPtr, unsigned short varMask, unsigned int variableNumber);
    int *GetIntVarPtr(int *varPtr, unsigned short varMask, unsigned int variableNumber);

    ZunTimer interruptReturnTime;
    AnmRawInstr *interruptReturnInstruction;
    Float3 posInitial, posFinal;
    Float3 rotateInitial, rotateFinal;
    Float2 scaleInitial, scaleFinal;
    AnmVmScriptColor color1Initial, color1Final;
    AnmVmScriptColor color2Initial, color2Final;
    Float3 pos2;
    int timeOfLastSpriteSet;
    unsigned char fontWidth, fontHeight;
    unsigned char unknown29A[0x0A];
};
typedef char AnmVmLifecycleSizeIs2A4[(sizeof(AnmVm) == 0x2A4) ? 1 : -1];
typedef char AnmVmNativeColorAndFlagsAliases[
    (offsetof(AnmVmBase, color1Bytes) == 0x1F0 &&
     offsetof(AnmVmBase, color1Script) == 0x1F0 &&
     offsetof(AnmVmBase, flagsWord) == 0x1F8 &&
     offsetof(AnmVmBase, flags) == 0x1F8 &&
     offsetof(AnmVmBase, flagsLow) == 0x1F8 &&
     offsetof(AnmVmBase, flagsHigh) == 0x1FA) ? 1 : -1];

typedef char AnmVmScriptScalarsAt100[
    (offsetof(AnmVmBase, intVar0) == 0x100 &&
     offsetof(AnmVmBase, uvScrollVel) == 0x128 &&
     offsetof(AnmVmBase, matrix1) == 0x130 &&
     offsetof(AnmVmBase, color1) == 0x1F0 &&
     offsetof(AnmVmBase, color2) == 0x1F4) ? 1 : -1];
typedef char AnmVmScriptTailAt294[
    (offsetof(AnmVm, interruptReturnInstruction) == 0x234 &&
     offsetof(AnmVm, posInitial) == 0x238 &&
     offsetof(AnmVm, color1Initial) == 0x278 &&
     offsetof(AnmVm, pos2) == 0x288 &&
     offsetof(AnmVm, timeOfLastSpriteSet) == 0x294) ? 1 : -1];
