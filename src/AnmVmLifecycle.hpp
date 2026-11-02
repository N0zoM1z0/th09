#pragma once

#include "ZunTimer.hpp"
#include "Float3.hpp"
#include <stddef.h>

class AnmLoaded;
class AnmLoadedSprite;
struct AnmRawInstr;

// One target-observed base prefix and VM tail;original nominal classes are unknown.
struct AnmVmBase
{
    AnmVmBase();

    unsigned char unknown_0000[0x18];
    Float2 scale;
    unsigned char unknown_0020[8];
    Float2 spriteSize;
    Float2 uvScrollPos;
    ZunTimer currentTimeInScript;
    ZunTimer waitTimer;
    ZunTimer interpolationCurrentTimers[7];
    ZunTimer interpolationEndTimers[7];
    unsigned char unknown0F8[0x1F0 - 0x0F8];
    unsigned long color1;
    unsigned long color2;
    union {
        unsigned int flagsWord;
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
    unsigned char unknown_01FC[2];
    short pendingInterrupt;
    int playerBulletHitAnimationType;
    AnmLoaded *anmFile;
    Float3 pos;
    short activeSpriteIndex;
    short anmFileIndex;
    short baseSpriteIndex;
    short scriptIndex;
    AnmRawInstr *beginningOfScript;
    void *currentInstruction;
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

    ZunTimer interruptReturnTime;
    unsigned char unknown234[0x288 - 0x234];
    Float3 pos2;
    unsigned char unknown_0294[0x10];
};
typedef char AnmVmLifecycleSizeIs2A4[(sizeof(AnmVm) == 0x2A4) ? 1 : -1];
