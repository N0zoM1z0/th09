#include "ExAttackController.hpp"
#include "ZunMemory.hpp"

#include <stddef.h>
#include <string.h>

struct ExAttackGameManagerSideView
{
    unsigned char unknown000[0x20];
    int selector20;
    unsigned char unknown024[0x38 - 0x24];
};

struct ExAttackGameManagerView
{
    ExAttackGameManagerSideView sides[2];
    unsigned char unknown070[0x11C - 0x70];
    int difficulty;
};

typedef char ExAttackSideSize[
    (sizeof(ExAttackGameManagerSideView) == 0x38) ? 1 : -1];
typedef char ExAttackSide0SelectorAt20[
    ((offsetof(ExAttackGameManagerView, sides) +
      offsetof(ExAttackGameManagerSideView, selector20)) == 0x20) ? 1 : -1];
typedef char ExAttackSide1SelectorAt58[
    ((offsetof(ExAttackGameManagerView, sides) +
      sizeof(ExAttackGameManagerSideView) +
      offsetof(ExAttackGameManagerSideView, selector20)) == 0x58) ? 1 : -1];
typedef char ExAttackDifficultyAt11C[
    (offsetof(ExAttackGameManagerView, difficulty) == 0x11C) ? 1 : -1];

extern ExAttackGameManagerView g_GameManager;
extern int g_ExAttackBaseLimitTable[];
extern int g_ExAttackDifficultyScaleTable[];
extern ZunMemory g_ZunMemory;

void __fastcall ResetExAttackRecords(ExAttackController *controller)
{
    ExAttackRecord *record = controller->records;
    for (int i = 0; i < 256; ++i, ++record)
    {
        if (record->active)
        {
            if (record->releaseCallback != NULL)
                record->releaseCallback(record);
            if (record->dynamicData != NULL)
            {
                g_ZunMemory.Free(record->dynamicData);
                record->dynamicData = NULL;
            }
            memset(record, 0, sizeof(ExAttackRecord));
        }
    }
}

int ExAttackController::AddedCallback(ExAttackController *controller)
{
    int selector = g_GameManager.sides[0].selector20;
    controller->sideLimits[0] =
        g_ExAttackDifficultyScaleTable[selector] * g_GameManager.difficulty +
        g_ExAttackBaseLimitTable[selector];

    selector = g_GameManager.sides[1].selector20;
    controller->sideLimits[1] =
        g_ExAttackDifficultyScaleTable[selector] * g_GameManager.difficulty +
        g_ExAttackBaseLimitTable[selector];

    ResetExAttackRecords(controller);
    return 0;
}
