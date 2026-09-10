#pragma once

struct Float3
{
    float x;
    float y;
    float z;
};

typedef char Float3SizeIs0C[(sizeof(Float3) == 0x0C) ? 1 : -1];

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
    static int DeletedCallback(AsciiManager *ascii);
    static void CutChain();

    void AddString(Float3 *position, const char *string);
    void AddFormatText(Float3 *position, const char *fmt, ...);

    unsigned char unknown_0000[0x2264];
    AsciiManagerString strings[256];
    int numStrings;
    unsigned long color;
    float scaleX;
    float scaleY;
    int isGui;
    unsigned char unknown_8278[0xE0AC - 0x8278];
};

typedef char AsciiManagerSizeIsE0AC[(sizeof(AsciiManager) == 0xE0AC) ? 1 : -1];
