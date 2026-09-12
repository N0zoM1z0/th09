#pragma once

struct AnmVm;
struct AnmRawInstr;
class AnmLoadedSprite
{
  public:
    unsigned char unknown000[0x34];
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
    int DrawNoRotation(AnmVm *vm);
    int ExecuteScript(AnmVm *vm);
    AnmLoaded *PreloadAnm(int anmIdx, const char *filename);
    void ReleaseAnm(int anmIdx);
};

extern AnmManager *g_AnmManager;
