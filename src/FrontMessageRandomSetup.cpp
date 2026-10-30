#include "AnmManager.hpp"
#include "AsciiManager.hpp"
#include "AsciiGameManagerView.hpp"
#include "RngRuntimeLeaves.hpp"

#include <stddef.h>
#include <string.h>

struct FrontMessageRandomEntryView
{
    void *instructions00;
    int weight04;
};

struct FrontMessageRandomTableView
{
    int count00;
    FrontMessageRandomEntryView entries04[1];
};

struct FrontMessageRandomView
{
    FrontMessageRandomTableView *primary00;
    void *current04;
    int currentIndex08;
    FrontMessageRandomTableView *secondary0C;
    int side10;
    unsigned char unknown014[0x1534 - 0x14];
    AnmVm textVmA1534;
    AnmVm textVmB17D8;
    unsigned char unknown1A7C[0x1D20 - 0x1A7C];
    unsigned long textColors1D20[4];
    unsigned long shadowColors1D30[4];
    unsigned char unknown1D40[0x10];
    int textSelectorCount1D50;
    int glyphSize1D54;
    unsigned char unknown1D58[0x0C];
    unsigned char colorIndex1D64;
    unsigned char flag1D65;
    unsigned char flag1D66;
    unsigned char flag1D67;
    unsigned char unknown1D68[4];
    unsigned char fontFlag1D6C;
    unsigned char fontFlag1D6D;
    unsigned char unknown1D6E[2];

    void SetupRandomForSide(int sideIndex);
};

typedef char FrontMessageRandomSizeIs1D70[
    (sizeof(FrontMessageRandomView) == 0x1D70) ? 1 : -1];

extern AnmLoaded *g_FrontMessageAnm;
extern RngRuntimeView g_Rng;

void FrontMessageRandomView::SetupRandomForSide(int sideIndex)
{
    int opponentSelector =
        g_GameManager.sides[1 - sideIndex].character20;
    FrontMessageRandomTableView *primary = primary00;
    FrontMessageRandomTableView *secondary = secondary0C;

    memset(this, 0, sizeof(*this));

    primary00 = primary;
    secondary0C = secondary;

    opponentSelector *= 10;

    FrontMessageRandomTableView *table = primary;
    if (sideIndex == 1)
        table = secondary;

    side10 = sideIndex;

    int selected = opponentSelector;
    int randomValue = g_Rng.GetRandomU16InRange(0x100);
    int end = opponentSelector + 10;

    int *entryWeight = &table->entries04[selected].weight04;
    while (selected < end)
    {
        if (*reinterpret_cast<void **>(entryWeight - 1) != 0)
        {
            if (randomValue < *entryWeight)
                break;
            randomValue -= *entryWeight;
        }
        ++selected;
        entryWeight += 2;
    }

    if (selected >= end)
    {
        randomValue = g_Rng.GetRandomU16InRange(0x100);
        selected = 210;

        int *weight = &table->entries04[selected].weight04;
        for (; selected < 220; ++selected, weight += 2)
        {
            if (*reinterpret_cast<void **>(weight - 1) != 0 && selected < table->count00)
            {
                if (randomValue < *weight)
                    break;
                randomValue -= *weight;
            }
        }
        if (selected >= 220 || selected >= table->count00)
            selected = 210;
    }

    current04 = table->entries04[selected].instructions00;

    textVmA1534.scriptIndex = -1;
    textVmB17D8.scriptIndex = -1;

    glyphSize1D54 = 24;
    textColors1D20[0] = 0x00E8F0FFUL;
    textColors1D20[1] = 0x00F0E8FFUL;
    textColors1D20[2] = 0x00E8D0FFUL;
    textColors1D20[3] = 0x00D8FFFFUL;
    shadowColors1D30[0] = 0;
    shadowColors1D30[1] = 0;
    shadowColors1D30[2] = 0;
    shadowColors1D30[3] = 0;

    fontFlag1D6C = 1;
    textSelectorCount1D50 = 6;
    fontFlag1D6D = 1;
    colorIndex1D64 = 0;
    flag1D65 = 1;
    flag1D66 = 0;
    flag1D67 = 0xFF;

    g_FrontMessageAnm->SetAndExecuteScriptIdx(&textVmA1534, 0);
    g_FrontMessageAnm->SetAndExecuteScriptIdx(&textVmB17D8, 1);

    unsigned char glyphSize = static_cast<unsigned char>(glyphSize1D54);
    reinterpret_cast<unsigned char *>(&textVmA1534)[0x299] = glyphSize;
    reinterpret_cast<unsigned char *>(&textVmA1534)[0x298] = glyphSize;
    reinterpret_cast<unsigned char *>(&textVmB17D8)[0x299] = glyphSize;
    reinterpret_cast<unsigned char *>(&textVmB17D8)[0x298] = glyphSize;

    g_AnmManager->DrawTextLeft(
        &textVmA1534,
        textColors1D20[colorIndex1D64],
        shadowColors1D30[colorIndex1D64],
        " ");
    g_AnmManager->DrawTextLeft(
        &textVmB17D8,
        textColors1D20[colorIndex1D64],
        shadowColors1D30[colorIndex1D64],
        " ");
}
