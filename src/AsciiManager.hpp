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
    Float3 operator/(float scalar) const;
    Float3 *operator/=(float scalar);
    Float3 &operator+=(const Float3 &other);
    Float3 &operator-=(const Float3 &other);
    Float3 *FromAngleMagnitude(float angle, float magnitude);
    operator float *();

    float x;
    float y;
    float z;
};

typedef char Float3SizeIs0C[(sizeof(Float3) == 0x0C) ? 1 : -1];

struct AnmVm
{
    unsigned char unknown_0000[0x18];
    Float2 scale;
    unsigned char unknown_0020[8];
    Float2 spriteSize;
    Float2 uvScrollPos;
    unsigned char unknown_0038[0x1F0 - 0x38];
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
    unsigned char unknown_0228[0x60];
    Float3 pos2;
    unsigned char unknown_0294[0x10];

    void Initialize();
    int IsVisible();
    void SetInvisible();
    void SetZRotation(float angle);
};

typedef char AnmVmSizeIs2A4[(sizeof(AnmVm) == 0x2A4) ? 1 : -1];
typedef char AnmVmSpriteSizeAt28[(offsetof(AnmVm, spriteSize) == 0x28) ? 1 : -1];
typedef char AnmVmAnmFileAt204[(offsetof(AnmVm, anmFile) == 0x204) ? 1 : -1];
typedef char AnmVmActiveSpriteIndexAt214[(offsetof(AnmVm, activeSpriteIndex) == 0x214) ? 1 : -1];
typedef char AnmVmScriptIndexAt21A[(offsetof(AnmVm, scriptIndex) == 0x21A) ? 1 : -1];
typedef char AnmVmCurrentInstructionAt220[(offsetof(AnmVm, currentInstruction) == 0x220) ? 1 : -1];
typedef char AnmVmLoadedSpriteAt224[(offsetof(AnmVm, loadedSprite) == 0x224) ? 1 : -1];

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
    void OnDraw();
};

typedef char PauseMenuSizeIs1528[(sizeof(PauseMenu) == 0x1528) ? 1 : -1];

struct AsciiMenuState5
{
    int state;
    int frames;
    AnmVm menuSprites[5];
    AnmVm menuBackground;

    int OnUpdate();
    void OnDraw();
};

typedef char AsciiMenuState5SizeIs0FE0[(sizeof(AsciiMenuState5) == 0x0FE0) ? 1 : -1];

struct AsciiMenuState4
{
    int state;
    int frames;
    AnmVm menuSprites[4];

    int OnUpdate();
    void OnDraw();
};

typedef char AsciiMenuState4SizeIs0A98[(sizeof(AsciiMenuState4) == 0x0A98) ? 1 : -1];

class AsciiManager
{
  public:
    static int OnUpdate(AsciiManager *ascii);
    static int OnDrawLowPrio(AsciiManager *ascii);
    static int OnDrawHighPrio(AsciiManager *ascii);
    static int AddedCallback(AsciiManager *ascii);
    static int DeletedCallback(AsciiManager *ascii);
    static void CutChain();

    void AddString(Float3 *position, const char *string);
    void AddFormatText(Float3 *position, const char *fmt, ...);
    void DrawStrings();
    void ResetStrings();
    void SetScale(float scaleX, float scaleY);
    void OnDrawHighPrioImpl(int playerIndex);
    void CreateScorePopup(int playerIndex, Float3 *position, int number, unsigned long color);
    void Reset();
    void SetSpaceWidth(int spaceWidth);
    void SetBossMarkerState(int slot, int state);

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
typedef char AsciiManagerScaleXAt826C[(offsetof(AsciiManager, scaleX) == 0x826C) ? 1 : -1];
typedef char AsciiManagerScaleYAt8270[(offsetof(AsciiManager, scaleY) == 0x8270) ? 1 : -1];
typedef char AsciiManagerFrameCounterAt8284[(offsetof(AsciiManager, frameCounter) == 0x8284) ? 1 : -1];
typedef char AsciiManagerPauseMenuAt82A0[(offsetof(AsciiManager, pauseMenu) == 0x82A0) ? 1 : -1];
typedef char AsciiManagerMenuState97C8[(offsetof(AsciiManager, menuState97C8) == 0x97C8) ? 1 : -1];
typedef char AsciiManagerMenuStateA7A8[(offsetof(AsciiManager, menuStateA7A8) == 0xA7A8) ? 1 : -1];
typedef char AsciiManagerScorePopupsAtB240[(offsetof(AsciiManager, scorePopups) == 0xB240) ? 1 : -1];
