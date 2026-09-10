#pragma once

enum AnmFileSlot
{
    ANM_FILE_SLOT_ASCII = 1,
    ANM_FILE_SLOT_CAPTURE = 3,
};

class AnmManager
{
  public:
    void ReleaseAnm(int anmIdx);
};

extern AnmManager *g_AnmManager;
