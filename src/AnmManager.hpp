#pragma once

#include <stddef.h>

#if defined(TH09_ANM_MANAGER_LIFECYCLE_LAYOUT)
#include "AnmVmLifecycle.hpp"
#else
struct AnmVm;
#endif
struct AnmRawInstr;
struct AnmEntry;
struct AnmRawEntry;
struct AnmTextureHeader;
struct IDirect3DTexture8;
struct VertexTex1DiffuseXyzrhw;
struct VertexDiffuseXyzrhw;
class AnmLoadedSprite
{
  public:
    int anmIdx;
    void *texture;
    // This storage is shared by the exact native ANM resource loader,
    // sprite assignment and the Enemy boss's visible-size checks.
    float startPixelX08;
    float startPixelY0C;
    float endPixelX10;
    float endPixelY14;
    float textureHeight18;
    float textureWidth1C;
    float uvStartX;
    float uvStartY;
    float uvEndX;
    float uvEndY;
    float heightPx;
    float widthPx;
    float scaleX38;
    float scaleY3C;
    unsigned int unknown40;
};

typedef char AnmLoadedSpriteSizeIs44[(sizeof(AnmLoadedSprite) == 0x44) ? 1 : -1];

class AnmLoaded
{
  public:
    // Independently original-bound +0x00..+0x1C resource slot. Its physical
    // fields are already used by the exact ANM loader, runtime and SetSprite.
    int anmIdx;
    AnmRawEntry *rawData04;
    int totalEntries08;
    AnmLoadedSprite *sprites0C;
    AnmRawInstr **scripts10;
    AnmEntry *textures14;
    union {
        int postloadEntryNumber18;
        int numberEntriesToBeLoaded18;
    };

    void LoadSprite(int spriteIndex, AnmLoadedSprite *loadedSprite);
    AnmLoadedSprite *GetSprite(int spriteIndex);
    void InitializeAndSetSprite(AnmVm *vm, int spriteIndex);
    void ExecuteAnmIdx(AnmVm *vm, int scriptIndex);
    void SetAndExecuteScriptIdx(AnmVm *vm, int scriptIndex);
    int SetSprite(AnmVm *vm, int spriteIndex);
    void SetAndExecuteScript(AnmVm *vm, AnmRawInstr *beginningOfScript);
    void ExecuteAnmIdxArray(AnmVm *vm, int scriptIndex, int count);
};

typedef char AnmLoadedNativeSizeIs1C[
    (sizeof(AnmLoaded) == 0x1C) ? 1 : -1];

enum AnmFileSlot
{
    ANM_FILE_SLOT_ASCII = 1,
    ANM_FILE_SLOT_CAPTURE = 3,
};

class AnmManager
{
  public:
    AnmManager();
    void FlushVertexBuffer();
    int AddSpriteToDrawBuffer(VertexTex1DiffuseXyzrhw *vertices);
    int DrawInner(AnmVm *vm, int flags);
    int DrawInnerFlippedX(AnmVm *vm, int flags);
    void SetRenderStateForVm(AnmVm *vm);
    int DrawNoRotation(AnmVm *vm);
    int DrawNoRotationFlippedX(AnmVm *vm);
    void TranslateRotation(VertexTex1DiffuseXyzrhw *vertex, float x, float y, float sine, float cosine, float xOffset, float yOffset);
    int Draw2D(AnmVm *vm);
    void Draw2DAndFlush(AnmVm *vm);
    void SetCameraMode(int mode);
    void SetMixColorDefault();
    void SetMixColor(unsigned long color);
    int Draw2DRotatedOrAxisAligned(AnmVm *vm);
    int DrawNoRotationNoRound(AnmVm *vm);
    int ProjectCameraFacingQuad(AnmVm *vm);
    int DrawCameraFacingQuad(AnmVm *vm);
    void Project3DQuad(AnmVm *vm);
    int DrawProjected3DQuad(AnmVm *vm);
    void SetRenderStateForVm3D(AnmVm *vm);
    int Draw3D(AnmVm *vm);
    int DrawVertices(AnmVm *vm, VertexTex1DiffuseXyzrhw *vertices, int vertexCount);
    int QueueSpriteQuad(AnmVm *vm, VertexTex1DiffuseXyzrhw *vertices);
    int DrawTriangleStrip(AnmVm *vm, VertexDiffuseXyzrhw *vertices, int vertexCount);
    int DrawTriangleFan(AnmVm *vm, VertexDiffuseXyzrhw *vertices, int vertexCount);
    int DrawTexturedTriangleFan(AnmVm *vm, VertexTex1DiffuseXyzrhw *vertices, int vertexCount);
    void SetupVertexBuffer();
    void ReleaseVertexBuffer();
    void ClearBlendMode();
    void ClearColorOp();
    void ClearTexture();
    void ClearSprite();
    void ClearVertexShader();
    void ClearZWrite();
    void ResetFrameDebugInfo();
    void ClearCameraSettings();
    void ResetForSupervisorFrame();
    int ExecuteScript(AnmVm *vm);
    void ExecuteScriptArray(AnmVm *vm, int count);
    void SetInterruptArray(AnmVm *vm, int count, short interrupt);
    int SpriteHasTexture(AnmVm *vm);
    void DrawPlayerBullet(AnmVm *vm);
    AnmLoaded *LoadAnm(int anmIdx, const char *filename);
    AnmLoaded *GetAnm(int anmIdx);
    AnmLoaded *ReadAnmEntries(int anmIdx, const char *filename);
    int CreateEmptyTexture(IDirect3DTexture8 **outTexture, int width, int height, int format);
    int CreateTextureFromFile(AnmEntry *entry, int format, int colorKey);
    int CreateTextureFromAnm(IDirect3DTexture8 **outTexture, AnmTextureHeader *textureHeader, int format);
    int LoadExternalTextureData(AnmLoaded *anmLoaded, int entryNumber, int *sprites, int *scripts, AnmRawEntry *rawEntry);
    int LoadTextureData(AnmLoaded *anmLoaded, int entryNumber, int currentSpriteNumber, int currentScriptNumber, AnmRawEntry *rawEntry);
    AnmLoaded *PreloadAnm(int anmIdx, const char *filename);
    AnmLoaded *PostloadAnmEntry(AnmLoaded *anmLoaded);
    int ServicePreloadedAnims();
    void ReleaseAnm(int anmIdx);
    void ReleaseAnmEntry(AnmEntry *entry);
    void DrawTextInner(IDirect3DTexture8 *outTexture, int x, int y, int width, int height, int fontWidth, int fontHeight, unsigned long textColor, unsigned long outlineColor, const char *buffer, float scaleFactorX, float scaleFactorY);
    void DrawTextLeft(AnmVm *vm, unsigned long textColor, unsigned long shadowColor, const char *format, ...);
    void DrawTextCentered(AnmVm *vm, unsigned long textColor, unsigned long shadowColor, const char *format, ...);
    int LoadSurface(int surfaceIndex, const char *path);
    int PreloadSurface(int surfaceIndex, const char *path);
    void ReleaseSurface(int surfaceIndex);
    void ReleaseSurfaces();
    void CopySurfaceToBackbuffer(int surfaceIndex, int left, int top, int x, int y);
    void CopySurfaceToBackbuffer2(int surfaceIndex, int rectX, int rectY, int rectLeft, int rectTop, int width, int height);
    void CaptureToTexture(int captureAnmIndex, int srcX, int srcY, int srcW, int srcH, int dstX, int dstY, int dstW, int dstH);
    void CaptureToSurface(int captureSurfaceIndex, int srcX, int srcY, int srcW, int srcH, int dstX, int dstY, int dstW, int dstH);
    void ServiceCaptureRequests();
    int InitializeHorizontalTextureStrip(
        AnmVm *vm,
        VertexTex1DiffuseXyzrhw *vertices,
        int vertexCount);
    int InitializeVerticalTextureStrip(
        AnmVm *vm,
        VertexTex1DiffuseXyzrhw *vertices,
        int vertexCount);

    // Original ScreenEffect::CalcShakeEnvelope writes these two ANM manager
    // scalars at +0x1C/+0x20. Preserve the full loader class offset map.
    // The Title loading callback observes the capture-active slot at +0x08
    // in the same manager allocation as the exact surface and ANM leaves.
    union
    {
        unsigned char unknown000000[0x1C];
        struct {
            unsigned char unknown000008[8];
            int captureSurfaceIndex08;
            int scriptsStartedThisFrame0C;
            int scriptsExecutedThisFrame10;
            int renderStateChangesThisFrame14;
            int flushesThisFrame18;
        };
    };
    // Exact ExecuteScript and ResetFrameDebugInfo both address these
    // four native counters on the same AnmManager allocation.
    float frameShakeX1C;
    float frameShakeY20;

#if defined(TH09_ANM_MANAGER_LIFECYCLE_LAYOUT)
  private:
    struct ManagerDiffuseVertexStorage
    {
        ManagerDiffuseVertexStorage() {}
        float x;
        float y;
        float z;
        float rhw;
        unsigned long diffuse;
    };

    struct ManagerTexturedVertexStorage
    {
        ManagerTexturedVertexStorage() {}
        float x;
        float y;
        float z;
        float rhw;
        unsigned long diffuse;
        float u;
        float v;
    };

    typedef char ManagerDiffuseVertexStorageSizeIs14[
        (sizeof(ManagerDiffuseVertexStorage) == 0x14) ? 1 : -1];
    typedef char ManagerTexturedVertexStorageSizeIs1C[
        (sizeof(ManagerTexturedVertexStorage) == 0x1C) ? 1 : -1];

    unsigned char unknown000024[0x12064 - 0x24];
    AnmVm managerVm12064;
    // These native device-state words are independently used by render,
    // background and vertex-buffer owners; they share this same allocation.
    unsigned char unknown12308[0x1287C - 0x12308];
    unsigned long currentTextureFactor1287C;
    void *currentTexture12880;
    unsigned char currentBlendMode12884;
    unsigned char unknown12885;
    unsigned char currentVertexShader12886;
    unsigned char disableZWrite12887;
    unsigned char cameraMode12888;
    unsigned char unknown12889[0x12890 - 0x12889];
    void *quadVertexBuffer12890;
    ManagerDiffuseVertexStorage untexturedVertices12894[4];
    int spritesToDraw128E4;
    ManagerTexturedVertexStorage vertexStorage128E8[0x18000];
    ManagerTexturedVertexStorage *vertexBufferEnd2B28E8;
    ManagerTexturedVertexStorage *vertexBufferStart2B28EC;
    int field2B28F0;
    unsigned char unknown2B28F4[0x2B2934 - 0x2B28F4];
#endif
};

typedef char AnmManagerNativeCaptureSurfaceAt08[
    (offsetof(AnmManager, captureSurfaceIndex08) == 0x08) ? 1 : -1];
typedef char AnmManagerNativeScreenShakeAt1C[
    (offsetof(AnmManager, frameShakeX1C) == 0x1C &&
     offsetof(AnmManager, frameShakeY20) == 0x20) ? 1 : -1];

#if defined(TH09_ANM_MANAGER_LIFECYCLE_LAYOUT)
typedef char AnmManagerSizeIs2B2934[
    (sizeof(AnmManager) == 0x2B2934) ? 1 : -1];
#endif

extern AnmManager *g_AnmManager;
