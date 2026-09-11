#pragma once

#include "ZunTimer.hpp"

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
    unsigned char unknown_01FC[0x0C];
    Float3 pos;
    unsigned char unknown_0214[0x10];
    AnmLoadedSprite *loadedSprite;
    unsigned char unknown_0228[0x2A4 - 0x228];
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

class AsciiManager
{
  public:
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
    unsigned char unknown_8284[4];
    AnmLoaded *asciiAnm;
    AnmLoaded *captureAnm;
    int nextScorePopupIndex;
    int resetField8294;
    int unknown_8298;
    int resetField829C;
    unsigned char opaque_82A0[0x1528];
    unsigned char opaque_97C8[0x0FE0];
    unsigned char unknown_A7A8[0x0A98];
    AsciiManagerPopup scorePopups[2][100];
    unsigned char unknown_DE00[0xE0AC - 0xDE00];
};

typedef char AsciiManagerSizeIsE0AC[(sizeof(AsciiManager) == 0xE0AC) ? 1 : -1];
