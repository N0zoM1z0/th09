#pragma once

struct AnmVm;
struct AnmRawInstr;
struct AnmEntry;
struct AnmRawEntry;
class AnmLoadedSprite;

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
