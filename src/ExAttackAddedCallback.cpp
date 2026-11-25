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
int g_ExAttackBaseLimitTable[21] = {
    9, 8, 11, 20, 15, 25, 9, 9, 10, 10, 20, 10, 9, 10, 10, 10, 20, 20, 20, 20, 20,
};
int g_ExAttackDifficultyScaleTable[15] = {
    1, 1, 1, 2, 1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1,
};
extern ZunMemory g_ZunMemory;

// The base-limit and difficulty-scale data are original DWORD arrays.
// Keep each side selector and the live difficulty read unmodified.

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
        static_cast<int>(
            static_cast<unsigned int>(g_ExAttackDifficultyScaleTable[selector]) *
            static_cast<unsigned int>(g_GameManager.difficulty) +
            static_cast<unsigned int>(g_ExAttackBaseLimitTable[selector]));

    selector = g_GameManager.sides[1].selector20;
    controller->sideLimits[1] =
        static_cast<int>(
            static_cast<unsigned int>(g_ExAttackDifficultyScaleTable[selector]) *
            static_cast<unsigned int>(g_GameManager.difficulty) +
            static_cast<unsigned int>(g_ExAttackBaseLimitTable[selector]));

    ResetExAttackRecords(controller);
    return 0;
}
