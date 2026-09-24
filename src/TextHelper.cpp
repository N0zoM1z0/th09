#include "TextHelper.hpp"
#include "Supervisor.hpp"

#include <string.h>

struct TextHelperSupervisorView
{
    unsigned char unknown00[0x08];
    IDirect3DDevice8 *d3dDevice;
};

typedef char TextHelperSupervisorDeviceAt08[(offsetof(TextHelperSupervisorView, d3dDevice) == 0x08) ? 1 : -1];

struct TextHelperBitmapInfo
{
    BITMAPINFOHEADER bmiHeader;
    RGBQUAD bmiColors[17];
};

typedef char TextHelperBitmapInfoSizeIs6C[(sizeof(TextHelperBitmapInfo) == 0x6C) ? 1 : -1];

TextHelper::TextHelper()
{
    this->format = (D3DFORMAT)-1;
    this->width = 0;
    this->height = 0;
    this->hdc = 0;
    this->gdiObj2 = 0;
    this->gdiObj = 0;
    this->buffer = NULL;
}

TextHelper::~TextHelper()
{
    ReleaseBuffer();
}

bool TextHelper::ReleaseBuffer()
{
    if (this->hdc)
    {
        SelectObject(this->hdc, this->gdiObj);
        DeleteDC(this->hdc);
        DeleteObject(this->gdiObj2);
        this->format = (D3DFORMAT)-1;
        this->width = 0;
        this->height = 0;
        this->hdc = 0;
        this->gdiObj2 = 0;
        this->gdiObj = 0;
        this->buffer = NULL;
        return true;
    }
    return false;
}

TextHelperFormatInfo *TextHelper::GetFormatInfo(D3DFORMAT format)
{
    int formatIndex;
    for (formatIndex = 0;
         g_TextHelperFormats[formatIndex].format != (D3DFORMAT)-1 &&
         g_TextHelperFormats[formatIndex].format != format;
         ++formatIndex)
    {
    }
    if (format == (D3DFORMAT)-1)
        return NULL;
    return &g_TextHelperFormats[formatIndex];
}

bool TextHelper::TryAllocateBuffer(int width, int height, D3DFORMAT format)
{
    HGDIOBJ originalBitmapObj;
    unsigned char *bitmapData;
    HBITMAP bitmapObj;
    TextHelperFormatInfo *formatInfo;
    TextHelperBitmapInfo bitmapInfo;
    HDC deviceContext;
    int imageWidthInBytes;

    ReleaseBuffer();
    memset(&bitmapInfo, 0, sizeof(bitmapInfo));
    formatInfo = GetFormatInfo(format);
    if (formatInfo == NULL)
        return false;

    imageWidthInBytes = ((((width * formatInfo->bitCount) / 8) + 3) / 4) * 4;
    bitmapInfo.bmiHeader.biSize = sizeof(bitmapInfo);
    bitmapInfo.bmiHeader.biWidth = width;
    bitmapInfo.bmiHeader.biHeight = -(height + 1);
    bitmapInfo.bmiHeader.biPlanes = 1;
    bitmapInfo.bmiHeader.biBitCount = formatInfo->bitCount;
    bitmapInfo.bmiHeader.biSizeImage = height * imageWidthInBytes;
    if (format != D3DFMT_X1R5G5B5 && format != D3DFMT_X8R8G8B8)
    {
        bitmapInfo.bmiHeader.biCompression = 3;
        ((unsigned int *)bitmapInfo.bmiColors)[0] = formatInfo->redMask;
        ((unsigned int *)bitmapInfo.bmiColors)[1] = formatInfo->greenMask;
        ((unsigned int *)bitmapInfo.bmiColors)[2] = formatInfo->blueMask;
        ((unsigned int *)bitmapInfo.bmiColors)[3] = formatInfo->alphaMask;
    }

    bitmapObj = CreateDIBSection(NULL, (BITMAPINFO *)&bitmapInfo, 0,
                                 (void **)&bitmapData, NULL, 0);
    if (bitmapObj == NULL)
        return false;

    memset(bitmapData, 0, bitmapInfo.bmiHeader.biSizeImage);
    deviceContext = CreateCompatibleDC(NULL);
    originalBitmapObj = SelectObject(deviceContext, bitmapObj);
    this->hdc = deviceContext;
    this->gdiObj2 = bitmapObj;
    this->buffer = bitmapData;
    this->imageSizeInBytes = bitmapInfo.bmiHeader.biSizeImage;
    this->gdiObj = originalBitmapObj;
    this->width = width;
    this->height = height;
    this->format = format;
    this->imageWidthInBytes = imageWidthInBytes;
    return true;
}

bool TextHelper::AllocateBufferWithFallback(int width, int height, D3DFORMAT format)
{
    if (TryAllocateBuffer(width, height, format))
        return true;
    if (format == D3DFMT_A1R5G5B5 || format == D3DFMT_A4R4G4B4)
        return TryAllocateBuffer(width, height, D3DFMT_A8R8G8B8);
    if (format == D3DFMT_R5G6B5)
        return TryAllocateBuffer(width, height, D3DFMT_X8R8G8B8);
    return false;
}

struct TextHelperA1R5G5B5
{
    unsigned short blue : 5;
    unsigned short green : 5;
    unsigned short red : 5;
    unsigned short alpha : 1;
};

bool TextHelper::InvertAlpha(int unusedX, int y, int spriteWidth, int fontHeight,
                             BOOL useGentleColorFalloff)
{
    int regionByteCount;
    unsigned char *bufferRegion;
    int byteOffset;
    TextHelperA1R5G5B5 *bufferCursor;
    int adjustedChannel;

    regionByteCount = spriteWidth * fontHeight * 2;
    bufferRegion = &GetBuffer()[y * spriteWidth * 2];
    switch (this->format)
    {
    case D3DFMT_A8R8G8B8:
        for (byteOffset = 3; byteOffset < regionByteCount; byteOffset += 4)
            bufferRegion[byteOffset] ^= 0xff;
        break;
    case D3DFMT_A1R5G5B5:
        for (bufferCursor = (TextHelperA1R5G5B5 *)bufferRegion, byteOffset = 0;
             byteOffset < regionByteCount;
             byteOffset += 2, ++bufferCursor)
        {
            bufferCursor->alpha ^= 1;
            if (bufferCursor->alpha)
            {
                if (useGentleColorFalloff)
                {
                    if (bufferCursor->red < bufferCursor->blue)
                    {
                        adjustedChannel = bufferCursor->blue -
                            bufferCursor->blue * byteOffset / regionByteCount / 4;
                        bufferCursor->blue = adjustedChannel >= 0x20 ? 0x1f : adjustedChannel;
                        adjustedChannel = bufferCursor->green -
                            bufferCursor->green * byteOffset / regionByteCount / 4;
                        bufferCursor->green = adjustedChannel >= 0x20 ? 0x1f : adjustedChannel;
                    }
                    else
                    {
                        adjustedChannel = bufferCursor->red -
                            bufferCursor->red * byteOffset / regionByteCount / 4;
                        bufferCursor->red = adjustedChannel >= 0x20 ? 0x1f : adjustedChannel;
                        adjustedChannel = bufferCursor->green -
                            bufferCursor->green * byteOffset / regionByteCount / 4;
                        bufferCursor->green = adjustedChannel >= 0x20 ? 0x1f : adjustedChannel;
                    }
                }
            }
            else
            {
                bufferCursor->red = 0;
                bufferCursor->green = 0;
                bufferCursor->blue = 0;
            }
        }
        break;
    case D3DFMT_A4R4G4B4:
        for (byteOffset = 1; byteOffset < regionByteCount; byteOffset += 2)
            bufferRegion[byteOffset] ^= 0xf0;
        break;
    default:
        return false;
    }
    return true;
}

unsigned char *TextHelper::GetBuffer() { return this->buffer; }
unsigned int TextHelper::GetImageWidthInBytes() { return this->imageWidthInBytes; }
int TextHelper::GetHeight() { return this->height; }
HDC TextHelper::GetHDC() { return this->hdc; }
int TextHelper::GetWidth() { return this->width; }
D3DFORMAT TextHelper::GetFormat() { return this->format; }
bool TextHelper::IsAllocated() { return this->gdiObj2 != NULL; }

void TextHelper::CreateTextBuffer()
{
    reinterpret_cast<TextHelperSupervisorView *>(&g_Supervisor)->d3dDevice->CreateImageSurface(
        1024, 64, D3DFMT_A1R5G5B5, &g_TextBufferSurface);
}

void TextHelper::ReleaseTextBuffer()
{
    if (g_TextBufferSurface != NULL)
    {
        g_TextBufferSurface->Release();
        g_TextBufferSurface = NULL;
    }
}
