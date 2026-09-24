#include "TextHelper.hpp"

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
                    if (bufferCursor->red >= bufferCursor->blue)
                    {
                        adjustedChannel = bufferCursor->red -
                            bufferCursor->red * byteOffset / regionByteCount / 4;
                        bufferCursor->red = adjustedChannel >= 0x20 ? 0x1f : adjustedChannel;
                        adjustedChannel = bufferCursor->green -
                            bufferCursor->green * byteOffset / regionByteCount / 4;
                        bufferCursor->green = adjustedChannel >= 0x20 ? 0x1f : adjustedChannel;
                    }
                    else
                    {
                        adjustedChannel = bufferCursor->blue -
                            bufferCursor->blue * byteOffset / regionByteCount / 4;
                        bufferCursor->blue = adjustedChannel >= 0x20 ? 0x1f : adjustedChannel;
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
