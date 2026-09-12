#pragma once

struct AnmVm;
struct AnmRawInstr;
struct VertexTex1DiffuseXyzrhw;
class AnmLoadedSprite
{
  public:
    unsigned char unknown000[0x04];
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
    AnmLoadedSprite *GetSprite(int spriteIndex);
    void InitializeAndSetSprite(AnmVm *vm, int spriteIndex);
    void ExecuteAnmIdx(AnmVm *vm, int scriptIndex);
    void SetSprite(AnmVm *vm, int spriteIndex);
    void SetAndExecuteScript(AnmVm *vm, AnmRawInstr *beginningOfScript);
};

enum AnmFileSlot
{
    ANM_FILE_SLOT_ASCII = 1,
    ANM_FILE_SLOT_CAPTURE = 3,
};

class AnmManager
{
  public:
    void FlushVertexBuffer();
    int AddSpriteToDrawBuffer(VertexTex1DiffuseXyzrhw *vertices);
    int DrawInner(AnmVm *vm, int flags);
    int DrawInnerFlippedX(AnmVm *vm, int flags);
    void SetRenderStateForVm(AnmVm *vm);
    int DrawNoRotation(AnmVm *vm);
    int DrawNoRotationFlippedX(AnmVm *vm);
    void TranslateRotation(VertexTex1DiffuseXyzrhw *vertex, float x, float y, float sine, float cosine, float xOffset, float yOffset);
    int Draw2D(AnmVm *vm);
    int ExecuteScript(AnmVm *vm);
    AnmLoaded *PreloadAnm(int anmIdx, const char *filename);
    void ReleaseAnm(int anmIdx);
};

extern AnmManager *g_AnmManager;
