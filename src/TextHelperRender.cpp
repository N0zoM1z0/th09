#include "TextHelper.hpp"

#include <string.h>

HRESULT WINAPI D3DXLoadSurfaceFromSurface(
    IDirect3DSurface8 *destSurface, const PALETTEENTRY *destPalette,
    const RECT *destRect, IDirect3DSurface8 *srcSurface,
    const PALETTEENTRY *srcPalette, const RECT *srcRect,
    unsigned long filter, D3DCOLOR colorKey);

void TextHelper::RenderTextToTextureBold(
    int x, int y, int width, int height, int fontWidth, int fontHeight,
    unsigned long textColor, unsigned long outlineColor,
    const char *text, IDirect3DTexture8 *outTexture)
{
    HGDIOBJ previousFont;
    IDirect3DSurface8 *destSurface;
    RECT destRect;
    RECT srcRect;
    D3DSURFACE_DESC textSurfaceDesc;
    HFONT font;
    HDC hdc;

    font = CreateFontA(fontWidth * 2, 0, 0, 0, FW_NORMAL,
                       FALSE, FALSE, FALSE, SHIFTJIS_CHARSET,
                       OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
                       ANTIALIASED_QUALITY, FF_ROMAN | FIXED_PITCH,
                       g_TextFontFaceName);
    TextHelper textHelper;
    g_TextBufferSurface->GetDesc(&textSurfaceDesc);
    textHelper.AllocateBufferWithFallback(
        textSurfaceDesc.Width, textSurfaceDesc.Height, textSurfaceDesc.Format);
    hdc = textHelper.GetHDC();
    previousFont = SelectObject(hdc, font);
    textHelper.InvertAlpha(0, 0, width * 2, fontWidth * 2 + 6, FALSE);
    SetBkMode(hdc, TRANSPARENT);

    if (outlineColor != 0xffffffff)
    {
        SetTextColor(hdc, g_TextOutlineColor);
        TextOutA(hdc, x * 2 + 4, 2, text, strlen(text));
        TextOutA(hdc, x * 2, 2, text, strlen(text));
        TextOutA(hdc, x * 2 + 2, 0, text, strlen(text));
        TextOutA(hdc, x * 2 + 2, 4, text, strlen(text));
    }
    else
    {
        SetTextColor(hdc, g_TextOutlineColor);
        TextOutA(hdc, x * 2 + 3, 2, text, strlen(text));
        TextOutA(hdc, x * 2 + 1, 2, text, strlen(text));
        TextOutA(hdc, x * 2 + 2, 1, text, strlen(text));
        TextOutA(hdc, x * 2 + 2, 3, text, strlen(text));
    }

    SetTextColor(hdc, textColor);
    TextOutA(hdc, x * 2 + 2, 2, text, strlen(text));
    SelectObject(hdc, previousFont);
    textHelper.InvertAlpha(
        0, 0, width * 2, fontWidth * 2 + 6,
        outlineColor == 0xffffffff);
    textHelper.CopyTextToSurface(g_TextBufferSurface);
    SelectObject(hdc, previousFont);
    DeleteObject(font);

    destRect.left = 0;
    destRect.top = y;
    destRect.right = width;
    destRect.bottom = y + fontHeight;
    srcRect.left = 0;
    srcRect.top = 0;
    srcRect.right = width * 2 + 2;
    srcRect.bottom = fontWidth * 2 + 2;
    if (srcRect.right > 1024)
        srcRect.right = 1024;

    outTexture->GetSurfaceLevel(0, &destSurface);
    D3DXLoadSurfaceFromSurface(
        destSurface, NULL, &destRect, g_TextBufferSurface, NULL, &srcRect, 4, 0);
    if (destSurface != NULL)
        destSurface->Release();
}

void TextHelper::RenderTextToTexture(
    int x, int y, int width, int height, int fontWidth, int fontHeight,
    unsigned long textColor, unsigned long outlineColor,
    const char *text, IDirect3DTexture8 *outTexture)
{
    HGDIOBJ previousFont;
    IDirect3DSurface8 *destSurface;
    RECT destRect;
    RECT srcRect;
    D3DSURFACE_DESC textSurfaceDesc;
    HFONT font;
    HDC hdc;

    font = CreateFontA(fontWidth * 2, 0, 0, 0, FW_NORMAL,
                       FALSE, FALSE, FALSE, SHIFTJIS_CHARSET,
                       OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
                       ANTIALIASED_QUALITY, FF_ROMAN | FIXED_PITCH,
                       g_TextFontFaceName);
    TextHelper textHelper;
    g_TextBufferSurface->GetDesc(&textSurfaceDesc);
    textHelper.AllocateBufferWithFallback(
        textSurfaceDesc.Width, textSurfaceDesc.Height, textSurfaceDesc.Format);
    hdc = textHelper.GetHDC();
    previousFont = SelectObject(hdc, font);
    textHelper.InvertAlpha(
        0, 0, textSurfaceDesc.Width, fontWidth * 2 + 6, FALSE);
    SetBkMode(hdc, TRANSPARENT);

    if (outlineColor != 0xffffffff)
    {
        SetTextColor(hdc, g_TextOutlineColor);
        TextOutA(hdc, x * 2 + 4, 2, text, strlen(text));
        TextOutA(hdc, x * 2, 2, text, strlen(text));
        TextOutA(hdc, x * 2 + 2, 0, text, strlen(text));
        TextOutA(hdc, x * 2 + 2, 4, text, strlen(text));
    }
    else
    {
        SetTextColor(hdc, g_TextOutlineColor);
        TextOutA(hdc, x * 2 + 3, 2, text, strlen(text));
        TextOutA(hdc, x * 2 + 1, 2, text, strlen(text));
        TextOutA(hdc, x * 2 + 2, 1, text, strlen(text));
        TextOutA(hdc, x * 2 + 2, 3, text, strlen(text));
    }

    SetTextColor(hdc, textColor);
    TextOutA(hdc, x * 2 + 2, 2, text, strlen(text));
    SelectObject(hdc, previousFont);
    textHelper.InvertAlpha(
        0, 0, textSurfaceDesc.Width, fontWidth * 2 + 6,
        outlineColor == 0xffffffff);
    textHelper.CopyTextToSurface(g_TextBufferSurface);
    SelectObject(hdc, previousFont);
    DeleteObject(font);

    destRect.left = 0;
    destRect.top = y;
    destRect.right = width;
    destRect.bottom = y + fontHeight;
    srcRect.left = 0;
    srcRect.top = 0;
    srcRect.right = width * 2;
    srcRect.bottom = fontWidth * 2;
    if (srcRect.right > 1024)
        srcRect.right = 1024;

    outTexture->GetSurfaceLevel(0, &destSurface);
    D3DXLoadSurfaceFromSurface(
        destSurface, NULL, &destRect, g_TextBufferSurface, NULL, &srcRect, 4, 0);
    if (destSurface != NULL)
        destSurface->Release();
}
