#pragma once

struct AnmVm;

class AnmLoaded
{
  public:
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
    AnmLoaded *PreloadAnm(int anmIdx, const char *filename);
    void ReleaseAnm(int anmIdx);
};

extern AnmManager *g_AnmManager;
