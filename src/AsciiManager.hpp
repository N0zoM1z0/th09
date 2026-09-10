#pragma once

class AnmLoaded;

struct Float3
{
    float x;
    float y;
    float z;
};

typedef char Float3SizeIs0C[(sizeof(Float3) == 0x0C) ? 1 : -1];

struct AnmVm
{
    unsigned char unknown_0000[0x1F8];
    union {
        unsigned int flagsWord;
        struct {
            unsigned int unknownFlags0 : 11;
            unsigned int anchor : 2;
            unsigned int unknownFlags13 : 19;
        };
    };
    unsigned char unknown_01FC[0x0C];
    Float3 pos;
    unsigned char unknown_0214[0x2A4 - 0x214];
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

class AsciiManager
{
  public:
    static int AddedCallback(AsciiManager *ascii);
    static int DeletedCallback(AsciiManager *ascii);
    static void CutChain();

    void AddString(Float3 *position, const char *string);
    void AddFormatText(Float3 *position, const char *fmt, ...);
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
    int resetField8290;
    int resetField8294;
    int unknown_8298;
    int resetField829C;
    unsigned char opaque_82A0[0x1528];
    unsigned char opaque_97C8[0x0FE0];
    unsigned char unknown_A7A8[0x0A98];
    unsigned char opaque_B240[0x15E0];
    unsigned char unknown_C820[0xE0AC - 0xC820];
};

typedef char AsciiManagerSizeIsE0AC[(sizeof(AsciiManager) == 0xE0AC) ? 1 : -1];
