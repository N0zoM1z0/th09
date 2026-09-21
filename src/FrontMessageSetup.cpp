#include "AnmManager.hpp"
#include "AsciiManager.hpp"

#include <stddef.h>
#include <string.h>

struct FrontMessageTableEntrySetupView
{
    void *instructions00;
    int unknown04;
};

struct FrontMessageTableSetupView
{
    int count00;
    FrontMessageTableEntrySetupView entries04[1];
};

struct FrontMessageSetupView
{
    FrontMessageTableSetupView *primary00;
    void *current04;
    int currentIndex08;
    FrontMessageTableSetupView *secondary0C;
    int mode10;
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

    void Setup(int messageIndex, int mode);
};

typedef char FrontMessageSetupSizeIs1D70[
    (sizeof(FrontMessageSetupView) == 0x1D70) ? 1 : -1];

extern AnmLoaded *g_FrontMessageAnm;

void FrontMessageSetupView::Setup(int messageIndex, int mode)
{
    FrontMessageTableSetupView *primary = primary00;
    FrontMessageTableSetupView *secondary = secondary0C;

    memset(this, 0, sizeof(*this));

    primary00 = primary;
    secondary0C = secondary;

    if (mode == 1)
        primary = secondary;

    mode10 = mode;
    currentIndex08 = messageIndex;
    current04 = primary->entries04[messageIndex].instructions00;

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
