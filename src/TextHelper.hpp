#pragma once

#include <windows.h>
#include <d3d8.h>

struct TextHelperFormatInfo
{
    D3DFORMAT format;
    int bitCount;
    unsigned int alphaMask;
    unsigned int redMask;
    unsigned int greenMask;
    unsigned int blueMask;
};

typedef char TextHelperFormatInfoSizeIs18[(sizeof(TextHelperFormatInfo) == 0x18) ? 1 : -1];

class TextHelper
{
  public:
    TextHelper();
    ~TextHelper();

    bool ReleaseBuffer();
    bool AllocateBufferWithFallback(int width, int height, D3DFORMAT format);
    bool TryAllocateBuffer(int width, int height, D3DFORMAT format);
    TextHelperFormatInfo *GetFormatInfo(D3DFORMAT format);
    bool InvertAlpha(int unusedX, int y, int spriteWidth, int fontHeight,
                     BOOL useGentleColorFalloff);
    unsigned char *GetBuffer();
    unsigned int GetImageWidthInBytes();
    int GetHeight();
    HDC GetHDC();
    int GetWidth();
    D3DFORMAT GetFormat();
    bool IsAllocated();
    bool CopyTextToSurface(IDirect3DSurface8 *outSurface);

    static void CreateTextBuffer();
    static void ReleaseTextBuffer();

    static void RenderTextToTextureBold(
        int x, int y, int width, int height, int fontWidth, int fontHeight,
        unsigned long textColor, unsigned long outlineColor,
        const char *text, IDirect3DTexture8 *outTexture);
    static void RenderTextToTexture(
        int x, int y, int width, int height, int fontWidth, int fontHeight,
        unsigned long textColor, unsigned long outlineColor,
        const char *text, IDirect3DTexture8 *outTexture);

  private:
    D3DFORMAT format;
    int width;
    int height;
    unsigned int imageSizeInBytes;
    int imageWidthInBytes;
    HDC hdc;
    HGDIOBJ gdiObj;
    HGDIOBJ gdiObj2;
    unsigned char *buffer;
};

typedef char TextHelperSizeIs24[(sizeof(TextHelper) == 0x24) ? 1 : -1];

extern IDirect3DSurface8 *g_TextBufferSurface;
extern unsigned long g_TextOutlineColor;
extern const char g_TextFontFaceName[];

extern TextHelperFormatInfo g_TextHelperFormats[];
