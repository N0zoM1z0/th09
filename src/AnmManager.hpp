#pragma once

struct AnmVm;
class AnmLoadedSprite;

class AnmLoaded
{
  public:
    AnmLoadedSprite *GetSprite(int spriteIndex);
    void InitializeAndSetSprite(AnmVm *vm, int spriteIndex);
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
    AnmLoaded *PreloadAnm(int anmIdx, const char *filename);
    void ReleaseAnm(int anmIdx);
};

extern AnmManager *g_AnmManager;
