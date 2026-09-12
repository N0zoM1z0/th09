#pragma once

#include "ZunTimer.hpp"

#include <stddef.h>

class AnmLoaded;
class AnmLoadedSprite;

struct Float2
{
    float x;
    float y;
};

typedef char Float2SizeIs08[(sizeof(Float2) == 0x08) ? 1 : -1];

struct Float3
{
    Float3(float x, float y, float z);
    Float3 operator+(const Float3 &other) const;
    Float3 operator-(const Float3 &other) const;
    Float3 operator*(float scalar) const;

    float x;
    float y;
    float z;
};

typedef char Float3SizeIs0C[(sizeof(Float3) == 0x0C) ? 1 : -1];

struct AnmVm
{
    unsigned char unknown_0000[0x18];
    Float2 scale;
    unsigned char unknown_0020[0x1F0 - 0x20];
    unsigned long color1;
    unsigned char unknown_01F4[4];
    union {
        unsigned int flagsWord;
        struct {
            unsigned int visible : 1;
            unsigned int unknownFlags1 : 10;
            unsigned int anchor : 2;
            unsigned int unknownFlags13 : 19;
        };
    };
    unsigned char unknown_01FC[2];
    short pendingInterrupt;
    unsigned char unknown_0200[8];
    Float3 pos;
    unsigned char unknown_0214[0x10];
    AnmLoadedSprite *loadedSprite;
    unsigned char unknown_0228[0x60];
    Float3 pos2;
    unsigned char unknown_0294[0x10];

    int IsVisible();
    void SetInvisible();
};

typedef char AnmVmSizeIs2A4[(sizeof(AnmVm) == 0x2A4) ? 1 : -1];

struct AsciiManagerString
{
    char text[64];
    Float3 position;
    unsigned long color;
    float scaleX;
    float scaleY;
    int isSelected;
    int isGui;
};

typedef char AsciiManagerStringSizeIs60[(sizeof(AsciiManagerString) == 0x60) ? 1 : -1];

struct AsciiManagerPopup
{
    char text[12];
    Float3 position;
    unsigned long color;
    ZunTimer timer;
    Float2 scale;
    unsigned char inUse;
    unsigned char characterCount;
    unsigned char unknown_0032[2];
    unsigned int unknown_0034;
};

typedef char AsciiManagerPopupSizeIs38[(sizeof(AsciiManagerPopup) == 0x38) ? 1 : -1];


struct PauseMenu
{
    int state;
    int frames;
    AnmVm menuSprites[7];
    AnmVm menuBackground;

    int OnUpdate();
};

typedef char PauseMenuSizeIs1528[(sizeof(PauseMenu) == 0x1528) ? 1 : -1];

struct AsciiMenuState5
{
    int state;
    int frames;
    AnmVm menuSprites[5];
    AnmVm menuBackground;

    int OnUpdate();
};

typedef char AsciiMenuState5SizeIs0FE0[(sizeof(AsciiMenuState5) == 0x0FE0) ? 1 : -1];

struct AsciiMenuState4
{
    int state;
    int frames;
    AnmVm menuSprites[4];

    int OnUpdate();
};

typedef char AsciiMenuState4SizeIs0A98[(sizeof(AsciiMenuState4) == 0x0A98) ? 1 : -1];

class AsciiManager
{
  public:
    static int OnUpdate(AsciiManager *ascii);
    static int AddedCallback(AsciiManager *ascii);
    static int DeletedCallback(AsciiManager *ascii);
    static void CutChain();

    void AddString(Float3 *position, const char *string);
    void AddFormatText(Float3 *position, const char *fmt, ...);
    void DrawStrings();
    void CreateScorePopup(int playerIndex, Float3 *position, int number, unsigned long color);
    void Reset();
    void SetSpaceWidth(int spaceWidth);

    AnmVm largeText;
    AnmVm smallScoreText;
    AnmVm popupText;
    unsigned char unknown_07EC[0x2264 - 0x7EC];
    AsciiManagerString strings[256];
    int numStrings;
    unsigned long color;
    float scaleX;
    float scaleY;
    int isGui;
    int isSelected;
    unsigned char unknown_827C[4];
    int spaceWidth;
    int frameCounter;
    AnmLoaded *asciiAnm;
    AnmLoaded *captureAnm;
    int nextScorePopupIndex;
    int resetField8294;
    int unknown_8298;
    int resetField829C;
    PauseMenu pauseMenu;
    AsciiMenuState5 menuState97C8;
    AsciiMenuState4 menuStateA7A8;
    AsciiManagerPopup scorePopups[2][100];
    unsigned char unknown_DE00[0xE0AC - 0xDE00];
};

typedef char AsciiManagerSizeIsE0AC[(sizeof(AsciiManager) == 0xE0AC) ? 1 : -1];
typedef char AsciiManagerFrameCounterAt8284[(offsetof(AsciiManager, frameCounter) == 0x8284) ? 1 : -1];
typedef char AsciiManagerPauseMenuAt82A0[(offsetof(AsciiManager, pauseMenu) == 0x82A0) ? 1 : -1];
typedef char AsciiManagerMenuState97C8[(offsetof(AsciiManager, menuState97C8) == 0x97C8) ? 1 : -1];
typedef char AsciiManagerMenuStateA7A8[(offsetof(AsciiManager, menuStateA7A8) == 0xA7A8) ? 1 : -1];
typedef char AsciiManagerScorePopupsAtB240[(offsetof(AsciiManager, scorePopups) == 0xB240) ? 1 : -1];
