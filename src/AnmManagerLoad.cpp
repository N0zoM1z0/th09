#define TH09_ANM_MANAGER_LIFECYCLE_LAYOUT
#include "AnmManager.hpp"
#include "AnmManagerFileSlotView.hpp"
#include "FileSystem.hpp"
#include "GameErrorContext.hpp"
#include "Supervisor.hpp"
#include "ZunMemory.hpp"

#include <stddef.h>
#include <string.h>
#include <windows.h>
#include <d3d8.h>

struct AnmEntry
{
    IDirect3DTexture8 *texture;
    unsigned char *rawData;
    int size;
};

typedef char AnmTextureEntrySizeIs0C[(sizeof(AnmEntry) == 0x0C) ? 1 : -1];

struct AnmRawEntry
{
    int numSprites;
    int numScripts;
    unsigned int textureIdx;
    int width;
    int height;
    unsigned int format;
    unsigned int colorKey;
    unsigned int nameOffset;
    unsigned int spriteIdxOffset;
    unsigned int mipmapNameOffset;
    unsigned int version;
    unsigned int priority;
    unsigned int textureOffset;
    unsigned char hasData;
    unsigned char padding035[3];
    unsigned int nextOffset;
    unsigned int reserved03C;
};

typedef char AnmRawEntrySizeIs40[(sizeof(AnmRawEntry) == 0x40) ? 1 : -1];
typedef char AnmRawEntryVersionAt28[(offsetof(AnmRawEntry, version) == 0x28) ? 1 : -1];
typedef char AnmRawEntryNextAt38[(offsetof(AnmRawEntry, nextOffset) == 0x38) ? 1 : -1];

struct AnmTextureHeader
{
    char magic[4];
    unsigned short reserved04;
    short format;
    short width;
    short height;
    unsigned short reserved0C;
    unsigned short reserved0E;
};

typedef char AnmTextureHeaderSizeIs10[(sizeof(AnmTextureHeader) == 0x10) ? 1 : -1];

struct AnmRawSpriteView
{
    unsigned int id;
    float x;
    float y;
    float width;
    float height;
};

typedef char AnmRawSpriteSizeIs14[(sizeof(AnmRawSpriteView) == 0x14) ? 1 : -1];

// The native ANM resource loader owns this same 0x44-byte sprite consumed
// by exact SetSprite, Etama and Enemy. Avoid a second per-TU layout.
typedef char AnmLoadedSpriteLoadNativeOffsets[
    (sizeof(AnmLoadedSprite) == 0x44 &&
     offsetof(AnmLoadedSprite, startPixelX08) == 0x08 &&
     offsetof(AnmLoadedSprite, endPixelY14) == 0x14 &&
     offsetof(AnmLoadedSprite, textureWidth1C) == 0x1C &&
     offsetof(AnmLoadedSprite, heightPx) == 0x30 &&
     offsetof(AnmLoadedSprite, widthPx) == 0x34 &&
     offsetof(AnmLoadedSprite, scaleX38) == 0x38) ? 1 : -1];

// All sixteen exact ANM loader owners traverse this same 0x1C resource
// record as SetSprite, runtime scripting and the 9776-byte interpreter.
typedef char AnmLoadedLoadSizeIs1C[
    (sizeof(AnmLoaded) == 0x1C &&
     offsetof(AnmLoaded, rawData04) == 0x04 &&
     offsetof(AnmLoaded, totalEntries08) == 0x08 &&
     offsetof(AnmLoaded, sprites0C) == 0x0C &&
     offsetof(AnmLoaded, scripts10) == 0x10 &&
     offsetof(AnmLoaded, textures14) == 0x14 &&
     offsetof(AnmLoaded, postloadEntryNumber18) == 0x18) ? 1 : -1];

struct AnmManagerLoadSlotView
{
    unsigned char unknown000[0x24];
    AnmLoaded loaded;
    int releasePending;
    char path[0xDC];
};

typedef char AnmManagerLoadSlotSizeIs120[(sizeof(AnmManagerLoadSlotView) == 0x120) ? 1 : -1];
typedef char AnmManagerLoadAnmAt24[(offsetof(AnmManagerLoadSlotView, loaded) == 0x24) ? 1 : -1];
typedef char AnmManagerLoadReleaseAt40[(offsetof(AnmManagerLoadSlotView, releasePending) == 0x40) ? 1 : -1];
typedef char AnmManagerLoadPathAt44[(offsetof(AnmManagerLoadSlotView, path) == 0x44) ? 1 : -1];

struct SupervisorAnmLoadView
{
    unsigned char unknown000[0x08];
    IDirect3DDevice8 *d3dDevice;
    unsigned char unknown00C[0x6B4 - 0x0C];
    int subthreadCloseRequestActive;
};

typedef char SupervisorAnmLoadDeviceAt08[(offsetof(SupervisorAnmLoadView, d3dDevice) == 0x08) ? 1 : -1];
typedef char SupervisorAnmLoadCloseAt6B4[(offsetof(SupervisorAnmLoadView, subthreadCloseRequestActive) == 0x6B4) ? 1 : -1];

struct VertexTex1DiffuseXyzrhwLoadView
{
    float x;
    float y;
    float z;
    float rhw;
    unsigned long diffuse;
    float u;
    float v;
};

typedef char VertexTex1DiffuseXyzrhwLoadSizeIs1C[(sizeof(VertexTex1DiffuseXyzrhwLoadView) == 0x1C) ? 1 : -1];

struct VertexTex1XyzrhwLoadView
{
    float x;
    float y;
    float z;
    float rhw;
    float u;
    float v;
};

typedef char VertexTex1XyzrhwLoadSizeIs18[
    (sizeof(VertexTex1XyzrhwLoadView) == 0x18) ? 1 : -1];

extern GameErrorContext g_GameErrorContext;
extern D3DFORMAT g_AnmTextureFormats[];
extern unsigned int g_AnmTextureBytesPerPixel[];
extern VertexTex1DiffuseXyzrhwLoadView g_AnmRenderQuad[4];
extern VertexTex1XyzrhwLoadView g_AnmUntexturedQuad[4];

extern const char g_AnmNoTextureStorageMessage[];
extern const char g_AnmCorruptedMessage[];
extern const char g_AnmWrongVersionMessage[];
extern const char g_AnmTextureMessage[];

HRESULT WINAPI D3DXCreateTextureFromFileInMemoryEx(
    IDirect3DDevice8 *device, const void *srcData, unsigned int srcDataSize,
    unsigned int width, unsigned int height, unsigned int mipLevels,
    unsigned long usage, D3DFORMAT format, D3DPOOL pool, unsigned long filter,
    unsigned long mipFilter, D3DCOLOR colorKey, void *srcInfo, PALETTEENTRY *palette,
    IDirect3DTexture8 **texture);
HRESULT WINAPI D3DXCreateTexture(
    IDirect3DDevice8 *device, unsigned int width, unsigned int height,
    unsigned int mipLevels, unsigned long usage, D3DFORMAT format,
    D3DPOOL pool, IDirect3DTexture8 **texture);
HRESULT WINAPI D3DXLoadSurfaceFromSurface(
    IDirect3DSurface8 *destSurface, const PALETTEENTRY *destPalette,
    const RECT *destRect, IDirect3DSurface8 *srcSurface,
    const PALETTEENTRY *srcPalette, const RECT *srcRect,
    unsigned long filter, D3DCOLOR colorKey);

static __inline SupervisorAnmLoadView *AnmSupervisor()
{
    return reinterpret_cast<SupervisorAnmLoadView *>(&g_Supervisor);
}

static __inline AnmManagerLoadSlotView *AnmLoadSlot(AnmManager *manager, int anmIdx)
{
    // The loader's 0x120-byte resource slots advance by native DWORDs.
    return reinterpret_cast<AnmManagerLoadSlotView *>(
        reinterpret_cast<unsigned int>(manager) +
        static_cast<unsigned int>(anmIdx) * sizeof(AnmManagerLoadSlotView));
}

// Both genuine texture-format and pixel-stride tables hold 4-byte words.
// A const element lvalue retains each retail load from the native Win32
// table base + unsigned-DWORD indexed byte stride, without a new guard.
static inline const D3DFORMAT &NativeAnmFormatRef(int index)
{
    return *reinterpret_cast<const D3DFORMAT *>(
        reinterpret_cast<unsigned int>(&g_AnmTextureFormats[0]) +
        static_cast<unsigned int>(index) * sizeof(g_AnmTextureFormats[0]));
}
static inline unsigned int NativeAnmTextureBytesPerPixel(int index)
{
    return *reinterpret_cast<const unsigned int *>(
        reinterpret_cast<unsigned int>(&g_AnmTextureBytesPerPixel[0]) +
        static_cast<unsigned int>(index) * sizeof(g_AnmTextureBytesPerPixel[0]));
}

static int __fastcall AnmGetFormat(int format)
{
    if (g_Supervisor.Is16BitTexturesForced() != 0)
    {
        if (NativeAnmFormatRef(format) == D3DFMT_A8R8G8B8 ||
            NativeAnmFormatRef(format) == D3DFMT_UNKNOWN)
            return 5;
        if (NativeAnmFormatRef(format) == D3DFMT_R8G8B8)
            return 3;
    }
    return format;
}

int AnmManager::CreateEmptyTexture(
    IDirect3DTexture8 **outTexture, int width, int height, int format)
{
    D3DXCreateTexture(AnmSupervisor()->d3dDevice, width, height, 1, 0,
                      NativeAnmFormatRef(format), D3DPOOL_MANAGED, outTexture);
    return 0;
}

int AnmManager::CreateTextureFromFile(
    AnmEntry *entry, int format, int colorKey)
{
    format = AnmGetFormat(format);
    if (D3DXCreateTextureFromFileInMemoryEx(
            AnmSupervisor()->d3dDevice, entry->rawData, entry->size,
            0, 0, 0, 0, NativeAnmFormatRef(format), D3DPOOL_MANAGED,
            3, static_cast<unsigned long>(-1), colorKey, NULL, NULL,
            &entry->texture) != D3D_OK)
        return -1;
    return 0;
}

int AnmManager::CreateTextureFromAnm(
    IDirect3DTexture8 **outTexture, AnmTextureHeader *textureHeader, int format)
{
    IDirect3DSurface8 *surface = NULL;
    IDirect3DSurface8 *textureSurfaceLevel = NULL;
    D3DLOCKED_RECT lockedRect;
    int currentY;

    format = AnmGetFormat(format);
    AnmSupervisor()->d3dDevice->CreateImageSurface(
        textureHeader->width, textureHeader->height,
        NativeAnmFormatRef(textureHeader->format), &surface);
    surface->LockRect(&lockedRect, NULL, 0);
    for (currentY = 0; currentY < textureHeader->height; currentY++)
    {
        void *textureDest = static_cast<unsigned char *>(lockedRect.pBits) + currentY * lockedRect.Pitch;
        const void *textureSrc = reinterpret_cast<unsigned char *>(textureHeader) + sizeof(AnmTextureHeader) +
            currentY * textureHeader->width * NativeAnmTextureBytesPerPixel(textureHeader->format);
        memcpy(textureDest, textureSrc,
               textureHeader->width * NativeAnmTextureBytesPerPixel(textureHeader->format));
    }
    surface->UnlockRect();

    if (D3DXCreateTexture(AnmSupervisor()->d3dDevice,
                          textureHeader->width, textureHeader->height, 1, 0,
                          NativeAnmFormatRef(format), D3DPOOL_MANAGED,
                          outTexture) != D3D_OK)
        goto error;

    (*outTexture)->GetSurfaceLevel(0, &textureSurfaceLevel);
    if (D3DXLoadSurfaceFromSurface(textureSurfaceLevel, NULL, NULL,
                                   surface, NULL, NULL, 3, 0) != D3D_OK)
        goto error;

    if (surface != NULL)
    {
        surface->Release();
        surface = NULL;
    }
    if (textureSurfaceLevel != NULL)
    {
        textureSurfaceLevel->Release();
        textureSurfaceLevel = NULL;
    }
    return 0;

error:
    if (surface != NULL)
    {
        surface->Release();
        surface = NULL;
    }
    if (textureSurfaceLevel != NULL)
        textureSurfaceLevel->Release();
    return -1;
}

// The ANM loader works with authentic Win32 allocations, not bounded
// shadow arrays: 0x44-byte sprite records, 0x0C-byte texture entries,
// 4-byte script pointers and 0x120-byte AnmManager file-slot records.
// A native signed index scales in an unsigned 32-bit address word; each
// consumer retains its original independent load/store and null gates.
static inline AnmEntry &NativeAnmTextureEntry(AnmLoaded *owner, int index)
{
    return *reinterpret_cast<AnmEntry *>(
        reinterpret_cast<unsigned int>(owner->textures14) +
        static_cast<unsigned int>(index) * sizeof(AnmEntry));
}

static inline AnmRawInstr *&NativeAnmScriptEntry(AnmLoaded *owner, int index)
{
    return *reinterpret_cast<AnmRawInstr **>(
        reinterpret_cast<unsigned int>(owner->scripts10) +
        static_cast<unsigned int>(index) * sizeof(owner->scripts10[0]));
}

static inline AnmManagerReadSlotView &NativeAnmManagerReadSlot(
    AnmManagerReadView *manager, int index)
{
    return *reinterpret_cast<AnmManagerReadSlotView *>(
        reinterpret_cast<unsigned int>(&manager->slots[0]) +
        static_cast<unsigned int>(index) * sizeof(manager->slots[0]));
}

static inline AnmLoadedSprite &NativeAnmLoadedSprite(AnmLoaded *owner, int index)
{
    return *reinterpret_cast<AnmLoadedSprite *>(
        reinterpret_cast<unsigned int>(owner->sprites0C) +
        static_cast<unsigned int>(index) * sizeof(AnmLoadedSprite));
}

int AnmManager::LoadExternalTextureData(
    AnmLoaded *anmLoadedObject, int entryNumber, int *, int *,
    AnmRawEntry *rawEntry)
{
    AnmLoaded *anmLoaded = reinterpret_cast<AnmLoaded *>(anmLoadedObject);
    if (rawEntry == NULL)
    {
        g_GameErrorContext.Fatal(g_AnmCorruptedMessage);
        return -1;
    }
    if (rawEntry->version != 3)
    {
        g_GameErrorContext.Fatal(g_AnmWrongVersionMessage);
        return -1;
    }
    if (!rawEntry->hasData)
    {
        const char *path = reinterpret_cast<const char *>(
            reinterpret_cast<unsigned int>(rawEntry) +
            static_cast<unsigned int>(rawEntry->nameOffset));
        if (path[0] != '@')
        {
            int fileSize;
            unsigned char *fileData = FileSystem::OpenFile(path, &fileSize, 1);
            if (fileData == NULL)
            {
                // Texture file read failure (original CP932 text).
                g_GameErrorContext.Fatal(
                    "\x83\x65\x83\x4e\x83\x58\x83\x60\x83\x83\x20\x25\x73\x20\x82\xaa"
                    "\x93\xc7\x82\xdd\x8d\x9e\x82\xdf\x82\xdc\x82\xb9\x82\xf1\x81\x42"
                    "\x83\x66\x81\x5b\x83\x5e\x82\xaa\x8e\xb8\x82\xed\x82\xea\x82\xc4"
                    "\x82\xe9\x82\xa9\x89\xf3\x82\xea\x82\xc4\x82\xa2\x82\xdc\x82\xb7"
                    "\x0d\x0a",
                    path);
                return -1;
            }
            NativeAnmTextureEntry(anmLoaded, entryNumber).size = fileSize;
            NativeAnmTextureEntry(anmLoaded, entryNumber).rawData = fileData;
        }
    }
    return 1;
}

void AnmManager::ReleaseAnmEntry(AnmEntry *entry)
{
    if (entry->texture != NULL)
    {
        entry->texture->Release();
        entry->texture = NULL;
    }
    if (entry->rawData != NULL)
        g_ZunMemory.Free(entry->rawData);
}

void AnmLoaded::LoadSprite(int spriteIndex, AnmLoadedSprite *loadedSprite)
{
    AnmLoaded *anm = reinterpret_cast<AnmLoaded *>(this);
    AnmLoadedSprite *source = loadedSprite;
    NativeAnmLoadedSprite(anm, spriteIndex) = *source;

    NativeAnmLoadedSprite(anm, spriteIndex).uvStartX =
        NativeAnmLoadedSprite(anm, spriteIndex).startPixelX08 /
        NativeAnmLoadedSprite(anm, spriteIndex).textureWidth1C;
    NativeAnmLoadedSprite(anm, spriteIndex).uvEndX =
        NativeAnmLoadedSprite(anm, spriteIndex).endPixelX10 /
        NativeAnmLoadedSprite(anm, spriteIndex).textureWidth1C;
    NativeAnmLoadedSprite(anm, spriteIndex).uvStartY =
        NativeAnmLoadedSprite(anm, spriteIndex).startPixelY0C /
        NativeAnmLoadedSprite(anm, spriteIndex).textureHeight18;
    NativeAnmLoadedSprite(anm, spriteIndex).uvEndY =
        NativeAnmLoadedSprite(anm, spriteIndex).endPixelY14 /
        NativeAnmLoadedSprite(anm, spriteIndex).textureHeight18;
    NativeAnmLoadedSprite(anm, spriteIndex).widthPx =
        (NativeAnmLoadedSprite(anm, spriteIndex).endPixelX10 -
         NativeAnmLoadedSprite(anm, spriteIndex).startPixelX08) /
        source->scaleX38;
    NativeAnmLoadedSprite(anm, spriteIndex).heightPx =
        (NativeAnmLoadedSprite(anm, spriteIndex).endPixelY14 -
         NativeAnmLoadedSprite(anm, spriteIndex).startPixelY0C) /
        source->scaleY3C;
}

int AnmManager::LoadTextureData(
    AnmLoaded *anmLoadedObject, int entryNumber, int currentSpriteNumber,
    int currentScriptNumber, AnmRawEntry *rawEntry)
{
    AnmLoaded *anmLoaded = reinterpret_cast<AnmLoaded *>(anmLoadedObject);
    int i;
    const char *path;
    D3DSURFACE_DESC surfaceDesc;
    AnmLoadedSprite loadedSprite;
    unsigned int *currentOffset;

    if (rawEntry == NULL)
    {
        g_GameErrorContext.Fatal(g_AnmCorruptedMessage);
        return -1;
    }
    if (rawEntry->version != 3)
    {
        g_GameErrorContext.Fatal(g_AnmWrongVersionMessage);
        return -1;
    }

    if (!rawEntry->hasData)
    {
        path = reinterpret_cast<const char *>(
            reinterpret_cast<unsigned int>(rawEntry) +
            static_cast<unsigned int>(rawEntry->nameOffset));
        if (path[0] == '@')
        {
            this->CreateEmptyTexture(&NativeAnmTextureEntry(anmLoaded, entryNumber).texture,
                                  rawEntry->width, rawEntry->height, rawEntry->format);
        }
        else if (this->CreateTextureFromFile(&NativeAnmTextureEntry(anmLoaded, entryNumber),
                                          rawEntry->format, rawEntry->colorKey) != 0)
        {
            // Texture creation failure (original CP932 text).
            g_GameErrorContext.Fatal(
                "\x83\x65\x83\x4e\x83\x58\x83\x60\x83\x83\x20\x25\x73\x20\x82\xaa"
                "\x8d\xec\x90\xac\x82\xc5\x82\xab\x82\xdc\x82\xb9\x82\xf1\x81\x42"
                "\x83\x66\x81\x5b\x83\x5e\x82\xaa\x8e\xb8\x82\xed\x82\xea\x82\xc4"
                "\x82\xe9\x82\xa9\x89\xf3\x82\xea\x82\xc4\x82\xa2\x82\xdc\x82\xb7"
                "\x0d\x0a",
                path);
            return -1;
        }
    }
    else if (this->CreateTextureFromAnm(
                 &NativeAnmTextureEntry(anmLoaded, entryNumber).texture,
                 reinterpret_cast<AnmTextureHeader *>(
                     reinterpret_cast<unsigned int>(rawEntry) +
                     static_cast<unsigned int>(rawEntry->textureOffset)),
                 rawEntry->format) != 0)
    {
        g_GameErrorContext.Fatal(g_AnmTextureMessage);
        return -1;
    }

    NativeAnmTextureEntry(anmLoaded, entryNumber).texture->SetPriority(rawEntry->priority);
    NativeAnmTextureEntry(anmLoaded, entryNumber).texture->PreLoad();
    NativeAnmTextureEntry(anmLoaded, entryNumber).texture->GetLevelDesc(0, &surfaceDesc);

    // The real file-backed Sprite and Script offset words follow each
    // 0x40-byte header; native byte cursor stepping preserves every original
    // independently loaded entry rather than constructing C++ arrays.
    currentOffset = reinterpret_cast<unsigned int *>(
        reinterpret_cast<unsigned int>(rawEntry) + sizeof(AnmRawEntry));
    for (i = 0; i < rawEntry->numSprites; i++, currentOffset = reinterpret_cast<unsigned int *>(
             reinterpret_cast<unsigned int>(currentOffset) + sizeof(*currentOffset)))
    {
        AnmRawSpriteView *rawSprite = reinterpret_cast<AnmRawSpriteView *>(
            reinterpret_cast<unsigned int>(rawEntry) + *currentOffset);
        loadedSprite.anmIdx = anmLoaded->anmIdx;
        loadedSprite.texture = NativeAnmTextureEntry(anmLoaded, entryNumber).texture;
        loadedSprite.scaleX38 = surfaceDesc.Width / static_cast<float>(rawEntry->width);
        loadedSprite.scaleY3C = surfaceDesc.Height / static_cast<float>(rawEntry->height);
        loadedSprite.startPixelX08 = rawSprite->x * loadedSprite.scaleX38;
        loadedSprite.startPixelY0C = rawSprite->y * loadedSprite.scaleY3C;
        loadedSprite.endPixelX10 = (rawSprite->x + rawSprite->width) * loadedSprite.scaleX38;
        loadedSprite.endPixelY14 = (rawSprite->y + rawSprite->height) * loadedSprite.scaleY3C;
        loadedSprite.textureWidth1C = static_cast<float>(surfaceDesc.Width);
        loadedSprite.textureHeight18 = static_cast<float>(surfaceDesc.Height);
        anmLoadedObject->LoadSprite(currentSpriteNumber, &loadedSprite);
        ++*reinterpret_cast<unsigned int *>(&currentSpriteNumber);
    }

    for (i = 0; i < rawEntry->numScripts; i++, currentOffset = reinterpret_cast<unsigned int *>(
             reinterpret_cast<unsigned int>(currentOffset) + 2u * sizeof(*currentOffset)))
    {
        NativeAnmScriptEntry(anmLoaded, currentScriptNumber) = reinterpret_cast<AnmRawInstr *>(
            reinterpret_cast<unsigned int>(rawEntry) +
            *reinterpret_cast<unsigned int *>(reinterpret_cast<unsigned int>(currentOffset) + sizeof(*currentOffset)));
        ++*reinterpret_cast<unsigned int *>(&currentScriptNumber);
    }
    return 1;
}

AnmLoaded *AnmManager::ReadAnmEntries(int anmIdx, const char *filename)
{
    if (anmIdx >= 20)
    {
        g_GameErrorContext.Fatal(g_AnmNoTextureStorageMessage);
        return NULL;
    }

    AnmManagerReadView *manager =
        reinterpret_cast<AnmManagerReadView *>(this);
    if (NativeAnmManagerReadSlot(manager, anmIdx).loaded.rawData04 != NULL)
    {
        NativeAnmManagerReadSlot(manager, anmIdx).releasePending = 1;
        while (NativeAnmManagerReadSlot(manager, anmIdx).releasePending != 0 &&
               !AnmSupervisor()->subthreadCloseRequestActive)
            Sleep(1);
    }

    AnmRawEntry *entry = reinterpret_cast<AnmRawEntry *>(
        FileSystem::OpenFile(filename, NULL, 0));
    int currentEntryNumber = 0;
    AnmManagerReadSlotView *slot = &NativeAnmManagerReadSlot(manager, anmIdx);
    AnmLoaded *anmLoaded = &slot->loaded;
    if (entry == NULL)
        return NULL;

    anmLoaded->anmIdx = anmIdx;
    anmLoaded->rawData04 = entry;
    strcpy(slot->path, filename);

    // A genuine .anm file chains variable-length 0x40-byte headers.
    // Its signed counts/offsets are accumulated as native wrapping
    // i386 DWORDs before the original allocations and per-entry loads.
    int totalEntries = 1;
    int totalScripts = entry->numScripts;
    int totalSprites = entry->numSprites;
    AnmRawEntry *currentEntry = entry;
    while (currentEntry->nextOffset != 0)
    {
        currentEntry = reinterpret_cast<AnmRawEntry *>(
            reinterpret_cast<unsigned int>(currentEntry) +
            static_cast<unsigned int>(currentEntry->nextOffset));
        ++*reinterpret_cast<unsigned int *>(&totalEntries);
        totalScripts = static_cast<int>(static_cast<unsigned int>(totalScripts) +
            static_cast<unsigned int>(currentEntry->numScripts));
        totalSprites = static_cast<int>(static_cast<unsigned int>(totalSprites) +
            static_cast<unsigned int>(currentEntry->numSprites));
    }

    anmLoaded->totalEntries08 = totalEntries;
    anmLoaded->textures14 = static_cast<AnmEntry *>(
        g_ZunMemory.Alloc(totalEntries * sizeof(AnmEntry), "./system\\global.h"));
    memset(anmLoaded->textures14, 0, totalEntries * sizeof(AnmEntry));
    anmLoaded->sprites0C = static_cast<AnmLoadedSprite *>(
        g_ZunMemory.Alloc(totalSprites * sizeof(AnmLoadedSprite), "./system\\global.h"));
    anmLoaded->scripts10 = static_cast<AnmRawInstr **>(
        g_ZunMemory.Alloc(totalScripts * sizeof(AnmRawInstr *), "./system\\global.h"));

    currentEntry = entry;
    totalSprites = 0;
    totalScripts = 0;
    while (true)
    {
        if (this->LoadExternalTextureData(reinterpret_cast<AnmLoaded *>(anmLoaded), currentEntryNumber,
                                       &totalSprites, &totalScripts,
                                       currentEntry) < 0)
            return NULL;
        ++*reinterpret_cast<unsigned int *>(&currentEntryNumber);
        if (currentEntry->nextOffset == 0)
            break;
        currentEntry = reinterpret_cast<AnmRawEntry *>(
            reinterpret_cast<unsigned int>(currentEntry) +
            static_cast<unsigned int>(currentEntry->nextOffset));
    }
    return reinterpret_cast<AnmLoaded *>(anmLoaded);
}

AnmLoaded *AnmManager::PreloadAnm(int anmIdx, const char *filename)
{
    AnmLoaded *anmLoaded = this->ReadAnmEntries(anmIdx, filename);
    if (anmLoaded == NULL)
        return NULL;

    AnmLoaded *view = reinterpret_cast<AnmLoaded *>(anmLoaded);
    view->postloadEntryNumber18 = 1;
    while (view->postloadEntryNumber18 != 0 && !AnmSupervisor()->subthreadCloseRequestActive)
        Sleep(1);
    return AnmSupervisor()->subthreadCloseRequestActive ? NULL : anmLoaded;
}

AnmLoaded *AnmManager::PostloadAnmEntry(AnmLoaded *anmLoadedObject)
{
    AnmLoaded *anmLoaded = reinterpret_cast<AnmLoaded *>(anmLoadedObject);
    AnmRawEntry *rawEntry = anmLoaded->rawData04;
    int entryLoadNumber = 0;
    int currentNumScripts = 0;
    int currentNumSprites = 0;
    int currentEntryNumber = 0;

    while (true)
    {
        if (entryLoadNumber == static_cast<int>(static_cast<unsigned int>(anmLoaded->postloadEntryNumber18) - 1u) &&
            this->LoadTextureData(anmLoadedObject, currentEntryNumber, currentNumSprites,
                               currentNumScripts, rawEntry) < 0)
        {
            anmLoaded->postloadEntryNumber18 = 0;
            return NULL;
        }
        currentNumSprites = static_cast<int>(
            static_cast<unsigned int>(currentNumSprites) +
            static_cast<unsigned int>(rawEntry->numSprites));
        currentNumScripts = static_cast<int>(
            static_cast<unsigned int>(currentNumScripts) +
            static_cast<unsigned int>(rawEntry->numScripts));
        ++*reinterpret_cast<unsigned int *>(&currentEntryNumber);
        if (rawEntry->nextOffset == 0)
            break;
        rawEntry = reinterpret_cast<AnmRawEntry *>(
            reinterpret_cast<unsigned int>(rawEntry) +
            static_cast<unsigned int>(rawEntry->nextOffset));
        ++*reinterpret_cast<unsigned int *>(&entryLoadNumber);
        if (entryLoadNumber == anmLoaded->postloadEntryNumber18)
        {
            ++*reinterpret_cast<unsigned int *>(&anmLoaded->postloadEntryNumber18);
            return anmLoadedObject;
        }
    }
    anmLoaded->postloadEntryNumber18 = 0;
    return anmLoadedObject;
}

AnmLoaded *AnmManager::LoadAnm(int anmIdx, const char *filename)
{
    AnmLoaded *anmLoaded = this->ReadAnmEntries(anmIdx, filename);
    if (anmLoaded != NULL)
    {
        AnmLoaded *view = reinterpret_cast<AnmLoaded *>(anmLoaded);
        view->postloadEntryNumber18 = 1;
        while (view->postloadEntryNumber18 != 0)
        {
            anmLoaded = this->PostloadAnmEntry(anmLoaded);
            view = reinterpret_cast<AnmLoaded *>(anmLoaded);
        }
    }
    return anmLoaded;
}

void AnmManager::ReleaseAnm(int anmIdx)
{
    if (anmIdx < 0 || static_cast<unsigned int>(anmIdx) >= 0x100)
        return;

    if (AnmLoadSlot(this, anmIdx)->loaded.rawData04 != NULL)
    {
        AnmManagerLoadSlotView *slot = AnmLoadSlot(this, anmIdx);
        for (int i = 0; i < slot->loaded.totalEntries08; i++)
            this->ReleaseAnmEntry(&slot->loaded.textures14[i]);
        g_ZunMemory.Free(slot->loaded.textures14);
        g_ZunMemory.Free(slot->loaded.sprites0C);
        g_ZunMemory.Free(slot->loaded.scripts10);
        g_ZunMemory.Free(slot->loaded.rawData04);
        memset(&slot->loaded, 0, 0x120);
    }
}

int AnmManager::ServicePreloadedAnims()
{
    for (unsigned int i = 0; i < 0x100; i++)
    {
        AnmManagerLoadSlotView *slot = AnmLoadSlot(this, i);
        if (slot->releasePending != 0)
        {
            this->ReleaseAnm(i);
            slot->releasePending = 0;
        }
        else if (slot->loaded.postloadEntryNumber18 != 0 &&
                 this->PostloadAnmEntry(
                     reinterpret_cast<AnmLoaded *>(&slot->loaded)) == NULL)
        {
            return -1;
        }
    }
    return 0;
}

AnmManager::AnmManager()
{
    // This is the actual Manager allocation used by the vertex/render TUs.
    // Verify every newly typed native state member against the original PE
    // addresses before trusting the constructor's normal C++ member writes.
    typedef char NativeCtorRenderStateOffsets[
        (offsetof(AnmManager, currentTextureFactor1287C) == 0x1287C &&
         offsetof(AnmManager, currentTexture12880) == 0x12880 &&
         offsetof(AnmManager, currentBlendMode12884) == 0x12884 &&
         offsetof(AnmManager, unknown12885) == 0x12885 &&
         offsetof(AnmManager, currentVertexShader12886) == 0x12886 &&
         offsetof(AnmManager, disableZWrite12887) == 0x12887 &&
         offsetof(AnmManager, cameraMode12888) == 0x12888 &&
         offsetof(AnmManager, quadVertexBuffer12890) == 0x12890 &&
         offsetof(AnmManager, field2B28F0) == 0x2B28F0) ? 1 : -1];
    memset(this, 0, sizeof(*this));

    g_AnmUntexturedQuad[0].u = 0.0f;
    g_AnmUntexturedQuad[0].v = 0.0f;
    g_AnmUntexturedQuad[1].v = 0.0f;
    g_AnmUntexturedQuad[2].u = 0.0f;

    g_AnmRenderQuad[0].u = 0.0f;
    g_AnmRenderQuad[0].v = 0.0f;
    g_AnmRenderQuad[1].v = 0.0f;
    g_AnmRenderQuad[2].u = 0.0f;

    for (int i = 3; i >= 0; --i)
        g_AnmUntexturedQuad[i].rhw = 1.0f;

    g_AnmUntexturedQuad[1].u = 1.0f;
    g_AnmUntexturedQuad[2].v = 1.0f;
    g_AnmUntexturedQuad[3].u = 1.0f;
    g_AnmUntexturedQuad[3].v = 1.0f;

    for (int i = 3; i >= 0; --i)
        g_AnmRenderQuad[i].rhw = 1.0f;

    g_AnmRenderQuad[1].u = 1.0f;
    g_AnmRenderQuad[2].v = 1.0f;
    g_AnmRenderQuad[3].u = 1.0f;
    g_AnmRenderQuad[3].v = 1.0f;

    unsigned char *self = reinterpret_cast<unsigned char *>(this);
    quadVertexBuffer12890 = NULL;
    currentTexture12880 = NULL;
    currentBlendMode12884 = 0;
    unknown12885 = 0;
    currentVertexShader12886 = 0;
    disableZWrite12887 = 0;
    field2B28F0 = -1;
    *reinterpret_cast<int *>(self + 0x08) = -1;
    currentTextureFactor1287C = 1;
    cameraMode12888 = 0xFF;
}

struct D3DXIMAGE_INFO
{
    unsigned int Width;
    unsigned int Height;
    unsigned int Depth;
    unsigned int MipLevels;
    D3DFORMAT Format;
};
typedef char D3dxImageInfoSizeIs14[(sizeof(D3DXIMAGE_INFO) == 0x14) ? 1 : -1];

struct AnmManagerSurfaceLoadView
{
    unsigned char unknown000[0x123FC];
    IDirect3DSurface8 *surfaces[32];
    IDirect3DSurface8 *surfacesBis[32];
    unsigned char *surfaceData[32];
    int surfaceDataSizes[32];
    D3DXIMAGE_INFO surfaceInfo[32];
};
typedef char AnmLoadSurfacePrimaryAt123FC[
    (offsetof(AnmManagerSurfaceLoadView, surfaces) == 0x123FC) ? 1 : -1];
typedef char AnmLoadSurfaceSecondaryAt1247C[
    (offsetof(AnmManagerSurfaceLoadView, surfacesBis) == 0x1247C) ? 1 : -1];
typedef char AnmLoadSurfaceDataAt124FC[
    (offsetof(AnmManagerSurfaceLoadView, surfaceData) == 0x124FC) ? 1 : -1];
typedef char AnmLoadSurfaceSizesAt1257C[
    (offsetof(AnmManagerSurfaceLoadView, surfaceDataSizes) == 0x1257C) ? 1 : -1];
typedef char AnmLoadSurfaceInfoAt125FC[
    (offsetof(AnmManagerSurfaceLoadView, surfaceInfo) == 0x125FC) ? 1 : -1];

extern const char g_AnmSurfaceCannotLoadMessage[];

HRESULT WINAPI D3DXLoadSurfaceFromFileInMemory(
    IDirect3DSurface8 *destSurface, const PALETTEENTRY *destPalette,
    const RECT *destRect, const void *srcData, unsigned int srcDataSize,
    const RECT *srcRect, unsigned long filter, D3DCOLOR colorKey,
    D3DXIMAGE_INFO *srcInfo);

static __inline AnmManagerSurfaceLoadView *SurfaceLoadView(AnmManager *manager)
{
    return reinterpret_cast<AnmManagerSurfaceLoadView *>(manager);
}

// Surface slots are native 32-bit addresses with 4-byte pointer/size
// entries and 0x14-byte D3DX metadata entries. The signed caller index
// is multiplied as a wrapping DWORD, not bounded by the C++ array view.
template <class T>
static __forceinline T &NativeAnmSurfaceSlot(T *base, int index)
{
    return *reinterpret_cast<T *>(
        reinterpret_cast<unsigned int>(base) +
        static_cast<unsigned int>(index) * sizeof(T));
}

int AnmManager::LoadSurface(int surfaceIndex, const char *path)
{
    unsigned char *fileData;
    int fileSize;
    IDirect3DSurface8 *surface;
    AnmManagerSurfaceLoadView *manager = SurfaceLoadView(this);

    if (NativeAnmSurfaceSlot(manager->surfaces, surfaceIndex) != NULL)
        this->ReleaseSurface(surfaceIndex);

    if (NativeAnmSurfaceSlot(manager->surfaceData, surfaceIndex) == NULL)
    {
        fileData = FileSystem::OpenFile(path, &fileSize, 0);
        if (fileData == NULL)
        {
            g_GameErrorContext.Fatal(g_AnmSurfaceCannotLoadMessage, path);
            return -1;
        }
    }
    else
    {
        fileData = NativeAnmSurfaceSlot(manager->surfaceData, surfaceIndex);
        fileSize = NativeAnmSurfaceSlot(manager->surfaceDataSizes, surfaceIndex);
        NativeAnmSurfaceSlot(manager->surfaceData, surfaceIndex) = NULL;
    }

    if (g_Supervisor.d3dDevice08->CreateImageSurface(
            640, 1024,
            g_Supervisor.presentParameters350.BackBufferFormat,
            &surface) != D3D_OK)
        return -1;

    if (D3DXLoadSurfaceFromFileInMemory(
            surface, NULL, NULL, fileData, fileSize, NULL, 1, 0,
            &NativeAnmSurfaceSlot(manager->surfaceInfo, surfaceIndex)) != D3D_OK)
        goto error;

    if (g_Supervisor.d3dDevice08->CreateRenderTarget(
            NativeAnmSurfaceSlot(manager->surfaceInfo, surfaceIndex).Width,
            NativeAnmSurfaceSlot(manager->surfaceInfo, surfaceIndex).Height,
            g_Supervisor.presentParameters350.BackBufferFormat,
            D3DMULTISAMPLE_NONE, TRUE, &NativeAnmSurfaceSlot(manager->surfaces, surfaceIndex)) != D3D_OK)
    {
        if (g_Supervisor.d3dDevice08->CreateImageSurface(
                NativeAnmSurfaceSlot(manager->surfaceInfo, surfaceIndex).Width,
                NativeAnmSurfaceSlot(manager->surfaceInfo, surfaceIndex).Height,
                g_Supervisor.presentParameters350.BackBufferFormat,
                &NativeAnmSurfaceSlot(manager->surfaces, surfaceIndex)) != D3D_OK)
            goto error;
    }

    if (g_Supervisor.d3dDevice08->CreateImageSurface(
            NativeAnmSurfaceSlot(manager->surfaceInfo, surfaceIndex).Width,
            NativeAnmSurfaceSlot(manager->surfaceInfo, surfaceIndex).Height,
            g_Supervisor.presentParameters350.BackBufferFormat,
            &NativeAnmSurfaceSlot(manager->surfacesBis, surfaceIndex)) != D3D_OK)
        goto error;

    if (D3DXLoadSurfaceFromSurface(
            NativeAnmSurfaceSlot(manager->surfaces, surfaceIndex), NULL, NULL,
            surface, NULL, NULL, 1, 0) != D3D_OK)
        goto error;

    if (D3DXLoadSurfaceFromSurface(
            NativeAnmSurfaceSlot(manager->surfacesBis, surfaceIndex), NULL, NULL,
            surface, NULL, NULL, 1, 0) != D3D_OK)
        goto error;

    if (surface != NULL)
    {
        surface->Release();
        surface = NULL;
    }
    g_ZunMemory.Free(fileData);
    return 0;

error:
    if (surface != NULL)
    {
        surface->Release();
        surface = NULL;
    }
    g_ZunMemory.Free(fileData);
    return -1;
}

int AnmManager::PreloadSurface(int surfaceIndex, const char *path)
{
    AnmManagerSurfaceLoadView *manager = SurfaceLoadView(this);
    int fileSize;

    if (NativeAnmSurfaceSlot(manager->surfaces, surfaceIndex) != NULL)
        this->ReleaseSurface(surfaceIndex);

    unsigned char *fileData = FileSystem::OpenFile(path, &fileSize, 0);
    if (fileData == NULL)
    {
        g_GameErrorContext.Fatal(g_AnmSurfaceCannotLoadMessage, path);
        return -1;
    }

    NativeAnmSurfaceSlot(manager->surfaceData, surfaceIndex) = fileData;
    NativeAnmSurfaceSlot(manager->surfaceDataSizes, surfaceIndex) = fileSize;
    return 0;
}
