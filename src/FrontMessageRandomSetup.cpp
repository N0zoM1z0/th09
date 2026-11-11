// SPDX-License-Identifier: GPL-3.0-only
// Adapted from Cirno-Labs/th09 src/gui.cpp GuiMsg::StartRandom,
// commit d7653152becabb8c86f2f43d49015ecc11bd0652.
// Original MIT portions retain their grant in LICENSE; see THIRD_PARTY_NOTICES.md.
#include "AnmManager.hpp"
#include "AsciiManager.hpp"
#include "AsciiGameManagerView.hpp"
#include "RngRuntimeLeaves.hpp"
#include "FrontMessageRuntimeView.hpp"

#include <stddef.h>
#include <string.h>

extern AnmLoaded *g_FrontMessageAnm;
extern RngRuntimeView g_Rng;

// The table's one declared C++ entry anchors variable-length wire storage.
// Retail scales any signed slot by eight in a native 32-bit address word.
static inline FrontMessageTableEntryView &NativeMessageEntry(
    FrontMessageTableView *file, int index)
{
    return *reinterpret_cast<FrontMessageTableEntryView *>(
        reinterpret_cast<unsigned int>(&file->entries04[0]) +
        static_cast<unsigned int>(index) * sizeof(file->entries04[0]));
}

void FrontMessageRuntimeView::SetupRandomForSide(int side)
{
    int i;
    int roll;
    // Two sides share the native 0x38-byte GameManager row storage. The
    // opposite-side selection and ten-message bank offset are DWORD math.
    int first = static_cast<int>(
        static_cast<unsigned int>(reinterpret_cast<AsciiGameManagerSideView *>(
            reinterpret_cast<unsigned int>(&g_GameManager.sides[0]) +
            (1u - static_cast<unsigned int>(side)) *
                sizeof(g_GameManager.sides[0]))->character20) * 10u);
    FrontMessageTableView *msgFile = this->primary00;
    FrontMessageTableView *msgFile2 = this->secondary0C;

    memset(this, 0, sizeof(*this));
    this->primary00 = msgFile;
    this->secondary0C = msgFile2;
    if (side == 1)
    {
        msgFile = msgFile2;
    }
    this->mode10 = side;
    roll = g_Rng.GetRandomU16InRange(0x100);
    for (i = first;
         i < static_cast<int>(static_cast<unsigned int>(first) + 10u);
         i = static_cast<int>(static_cast<unsigned int>(i) + 1u))
    {
        if (NativeMessageEntry(msgFile, i).instructions00 != NULL)
        {
            if (roll < NativeMessageEntry(msgFile, i).weight04)
            {
                break;
            }
            roll = static_cast<int>(
                static_cast<unsigned int>(roll) -
                static_cast<unsigned int>(NativeMessageEntry(msgFile, i).weight04));
        }
    }
    if (i >= static_cast<int>(static_cast<unsigned int>(first) + 10u))
    {
        roll = g_Rng.GetRandomU16InRange(0x100);
        for (i = 210; i < 220;
             i = static_cast<int>(static_cast<unsigned int>(i) + 1u))
        {
            if (NativeMessageEntry(msgFile, i).instructions00 != NULL && i < msgFile->count00)
            {
                if (roll < NativeMessageEntry(msgFile, i).weight04)
                {
                    break;
                }
                roll = static_cast<int>(
                    static_cast<unsigned int>(roll) -
                    static_cast<unsigned int>(NativeMessageEntry(msgFile, i).weight04));
            }
        }
        if (i >= 220 || i >= msgFile->count00)
        {
            i = 210;
        }
    }
    this->current04 = NativeMessageEntry(msgFile, i).instructions00;
    this->textVms1534[0].scriptIndex = -1;
    this->textVms1534[1].scriptIndex = -1;
    this->glyphSize1D54 = 24;
    this->textColors1D20[0] = 0xe8f0ff;
    this->textColors1D20[1] = 0xf0e8ff;
    this->textColors1D20[2] = 0xe8d0ff;
    this->textColors1D20[3] = 0xd8ffff;
    this->shadowColors1D30[0] = 0;
    this->shadowColors1D30[1] = 0;
    this->shadowColors1D30[2] = 0;
    this->shadowColors1D30[3] = 0;
    this->skippable1D6C = 1;
    this->textSelectorCount1D50 = 6;
    this->textBoxVisible1D6D = 1;
    this->colorIndex1D64 = 0;
    this->resetText1D65 = 1;
    this->textLineIndex1D66 = 0;
    this->currentPortrait1D67 = 0xff;
    g_FrontMessageAnm->SetAndExecuteScriptIdx(&this->textVms1534[0], 0);
    g_FrontMessageAnm->SetAndExecuteScriptIdx(&this->textVms1534[1], 1);
    this->textVms1534[0].fontWidth = this->textVms1534[0].fontHeight = this->glyphSize1D54;
    this->textVms1534[1].fontWidth = this->textVms1534[1].fontHeight = this->glyphSize1D54;
    g_AnmManager->DrawTextLeft(&this->textVms1534[0], this->textColors1D20[this->colorIndex1D64], this->shadowColors1D30[this->colorIndex1D64], " ");
    g_AnmManager->DrawTextLeft(&this->textVms1534[1], this->textColors1D20[this->colorIndex1D64], this->shadowColors1D30[this->colorIndex1D64], " ");
}
