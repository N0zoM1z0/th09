#include "Supervisor.hpp"
#include "GameErrorContext.hpp"
#include "ZunMemory.hpp"

#include <windows.h>
#include <d3d8.h>

extern GameErrorContext g_GameErrorContext;

#pragma pack(push, 1)
struct SnapshotBitmapFileHeader
{
    unsigned short type;
    unsigned int size;
    unsigned short reserved1;
    unsigned short reserved2;
    unsigned int offBits;
};
#pragma pack(pop)

struct SupervisorSnapshotLayout
{
    unsigned char unknown_0000[0x08];
    IDirect3DDevice8 *d3dDevice;
    unsigned char unknown_000C[0x358 - 0x0C];
    D3DFORMAT backBufferFormat;
};

int Supervisor::TakeSnapshot(const char *filePath)
{
    SupervisorSnapshotLayout *supervisor =
        reinterpret_cast<SupervisorSnapshotLayout *>(this);
    SnapshotBitmapFileHeader fileHeader;
    BITMAPINFOHEADER *infoHeader = NULL;
    unsigned char *pixels = NULL;
    IDirect3DSurface8 *backbuffer = NULL;
    D3DLOCKED_RECT lockedRect;
    HANDLE file;
    int widthBytes;
    int y;
    int x;
    int srcRowDelta;
    unsigned char *dst;
    unsigned char *src;
    unsigned char *dstRow;
    unsigned char *srcRow;

    supervisor->d3dDevice->GetBackBuffer(0, D3DBACKBUFFER_TYPE_MONO, &backbuffer);

    memset(&fileHeader, 0, sizeof(fileHeader));
    fileHeader.type = *reinterpret_cast<const unsigned short *>("BM");
    fileHeader.offBits = 0x36;
    fileHeader.size = fileHeader.offBits;

    switch (supervisor->backBufferFormat)
    {
    case D3DFMT_R5G6B5:
        g_GameErrorContext.Log("16bit \x82\xcd\x8e\xe6\x82\xe8\x8d\x9e\x82\xdf\x82\xc8\x82\xa2\r\n");
        goto cleanup;

    case D3DFMT_X8R8G8B8:
        infoHeader = (BITMAPINFOHEADER *)g_ZunMemory.Alloc(0x2C, "./system\\global.h");
        if (infoHeader == NULL)
        {
            g_GameErrorContext.Log("snapShotScreen : \x8a\x6d\x95\xdb\x82\xb5\x82\xad\x82\xe8\r\n");
            goto cleanup;
        }
        memset(infoHeader, 0, 0x2C);
        widthBytes = 0x780;
        pixels = (unsigned char *)g_ZunMemory.Alloc(widthBytes * 0x1E0, "./system\\global.h");
        if (pixels == NULL)
        {
            g_GameErrorContext.Log("snapShotScreen : \x8a\x6d\x95\xdb\x82\xb5\x82\xad\x82\xe8\r\n");
            goto cleanup;
        }
        fileHeader.size += widthBytes * 0x1E0;
        infoHeader->biBitCount = 0x18;
        infoHeader->biSize = 0x28;
        infoHeader->biWidth = 0x280;
        infoHeader->biHeight = 0x1E0;
        infoHeader->biPlanes = 1;
        infoHeader->biCompression = 0;
        backbuffer->LockRect(&lockedRect, NULL, 0);
        srcRowDelta = -lockedRect.Pitch;
        srcRow = (unsigned char *)lockedRect.pBits + lockedRect.Pitch * 0x1DF;
        dstRow = pixels;
        for (y = 0x1E0; y != 0; y--)
        {
            dst = dstRow;
            src = srcRow;
            for (x = 0; x < 0x280; x++)
            {
                *dst = *src++;
                dst++;
                *dst = *src++;
                dst++;
                *dst = *src;
                src += 2;
                dst++;
            }
            srcRow += srcRowDelta;
            dstRow += widthBytes;
        }
        backbuffer->UnlockRect();
        file = CreateFileA(filePath, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
        if (file == INVALID_HANDLE_VALUE)
            goto cleanup;
        WriteFile(file, &fileHeader, sizeof(fileHeader), (LPDWORD)&filePath, NULL);
        WriteFile(file, infoHeader, sizeof(BITMAPINFOHEADER), (LPDWORD)&filePath, NULL);
        WriteFile(file, pixels, widthBytes * 0x1E0, (LPDWORD)&filePath, NULL);
        CloseHandle(file);
        goto cleanup;

    default:
        g_GameErrorContext.Log("error ? .\\system\\mother.cpp\r\n");
        return 1;
    }

cleanup:
    if (backbuffer != NULL)
    {
        backbuffer->Release();
        backbuffer = NULL;
    }
    g_ZunMemory.Free(infoHeader);
    g_ZunMemory.Free(pixels);
    return 0;
}
