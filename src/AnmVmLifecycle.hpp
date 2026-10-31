#pragma once

#include "ZunTimer.hpp"
#include "Float3.hpp"
#include <stddef.h>

class AnmLoaded;
class AnmLoadedSprite;

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
            unsigned int unknownFlags2_3 : 2;
            unsigned int blendMode : 2;
            unsigned int unknownFlags6_10 : 5;
            unsigned int anchor : 2;
            unsigned int disableZWrite : 1;
            unsigned int unknownFlags14 : 18;
        };
    };
    unsigned char unknown_01FC[2];
    short pendingInterrupt;
    unsigned char unknown_0200[4];
    AnmLoaded *anmFile;
    Float3 pos;
    short activeSpriteIndex;
    unsigned char unknown_0216[4];
    short scriptIndex;
    unsigned char unknown_021C[4];
    void *currentInstruction;
    AnmLoadedSprite *loadedSprite;
};
typedef char AnmVmBaseSizeIs228[(sizeof(AnmVmBase) == 0x228) ? 1 : -1];

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
