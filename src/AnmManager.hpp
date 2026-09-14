#pragma once

struct AnmVm;
struct AnmRawInstr;
struct AnmEntry;
struct AnmRawEntry;
struct AnmTextureHeader;
struct IDirect3DTexture8;
struct VertexTex1DiffuseXyzrhw;
class AnmLoadedSprite
{
  public:
    int anmIdx;
    void *texture;
    unsigned char unknown008[0x18];
    float uvStartX;
    float uvStartY;
    float uvEndX;
    float uvEndY;
    unsigned char unknown030[0x04];
    float widthPx;
    unsigned char unknown038[0x0C];
};

typedef char AnmLoadedSpriteSizeIs44[(sizeof(AnmLoadedSprite) == 0x44) ? 1 : -1];

class AnmLoaded
{
  public:
    void LoadSprite(int spriteIndex, AnmLoadedSprite *loadedSprite);
    AnmLoadedSprite *GetSprite(int spriteIndex);
    void InitializeAndSetSprite(AnmVm *vm, int spriteIndex);
    void ExecuteAnmIdx(AnmVm *vm, int scriptIndex);
    void SetSprite(AnmVm *vm, int spriteIndex);
    void SetAndExecuteScript(AnmVm *vm, AnmRawInstr *beginningOfScript);
    void ExecuteAnmIdxArray(AnmVm *vm, int scriptIndex, int count);
};

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
    int Draw2DRotatedOrAxisAligned(AnmVm *vm);
    int DrawNoRotationNoRound(AnmVm *vm);
    int ProjectCameraFacingQuad(AnmVm *vm);
    int DrawCameraFacingQuad(AnmVm *vm);
    void Project3DQuad(AnmVm *vm);
    int DrawProjected3DQuad(AnmVm *vm);
    void SetRenderStateForVm3D(AnmVm *vm);
    int Draw3D(AnmVm *vm);
    int ExecuteScript(AnmVm *vm);
    void ExecuteScriptArray(AnmVm *vm, int count);
    void SetInterruptArray(AnmVm *vm, int count, short interrupt);
    int SpriteHasTexture(AnmVm *vm);
    void DrawPlayerBullet(AnmVm *vm);
    AnmLoaded *LoadAnm(int anmIdx, const char *filename);
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
    void ReleaseSurface(int surfaceIndex);
    void CopySurfaceToBackbuffer(int surfaceIndex, int left, int top, int x, int y);
    void CopySurfaceToBackbuffer2(int surfaceIndex, int rectX, int rectY, int rectLeft, int rectTop, int width, int height);
    void CaptureToTexture(int captureAnmIndex, int srcX, int srcY, int srcW, int srcH, int dstX, int dstY, int dstW, int dstH);
    void CaptureToSurface(int captureSurfaceIndex, int srcX, int srcY, int srcW, int srcH, int dstX, int dstY, int dstW, int dstH);
};

extern AnmManager *g_AnmManager;
