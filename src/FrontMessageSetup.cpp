#include "AnmManager.hpp"
#include "AsciiManager.hpp"
#include "FrontMessageRuntimeView.hpp"

#include <stddef.h>
#include <string.h>

extern AnmLoaded *g_FrontMessageAnm;

void FrontMessageRuntimeView::Setup(int messageIndex, int mode)
{
    FrontMessageTableView *primary = primary00;
    FrontMessageTableView *secondary = secondary0C;

    memset(this, 0, sizeof(*this));

    primary00 = primary;
    secondary0C = secondary;

    if (mode == 1)
        primary = secondary;

    mode10 = mode;
    currentIndex08 = messageIndex;
    current04 = primary->entries04[messageIndex].instructions00;

    textVms1534[0].scriptIndex = -1;
    textVms1534[1].scriptIndex = -1;

    glyphSize1D54 = 24;
    textColors1D20[0] = 0x00E8F0FFUL;
    textColors1D20[1] = 0x00F0E8FFUL;
    textColors1D20[2] = 0x00E8D0FFUL;
    textColors1D20[3] = 0x00D8FFFFUL;
    shadowColors1D30[0] = 0;
    shadowColors1D30[1] = 0;
    shadowColors1D30[2] = 0;
    shadowColors1D30[3] = 0;

    skippable1D6C = 1;
    textSelectorCount1D50 = 6;
    textBoxVisible1D6D = 1;
    colorIndex1D64 = 0;
    resetText1D65 = 1;
    textLineIndex1D66 = 0;
    currentPortrait1D67 = 0xFF;

    g_FrontMessageAnm->SetAndExecuteScriptIdx(&textVms1534[0], 0);
    g_FrontMessageAnm->SetAndExecuteScriptIdx(&textVms1534[1], 1);

    unsigned char glyphSize = static_cast<unsigned char>(glyphSize1D54);
    reinterpret_cast<unsigned char *>(&textVms1534[0])[0x299] = glyphSize;
    reinterpret_cast<unsigned char *>(&textVms1534[0])[0x298] = glyphSize;
    reinterpret_cast<unsigned char *>(&textVms1534[1])[0x299] = glyphSize;
    reinterpret_cast<unsigned char *>(&textVms1534[1])[0x298] = glyphSize;

    g_AnmManager->DrawTextLeft(
        &textVms1534[0],
        textColors1D20[colorIndex1D64],
        shadowColors1D30[colorIndex1D64],
        " ");
    g_AnmManager->DrawTextLeft(
        &textVms1534[1],
        textColors1D20[colorIndex1D64],
        shadowColors1D30[colorIndex1D64],
        " ");
}
