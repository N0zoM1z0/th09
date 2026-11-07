// SPDX-License-Identifier: GPL-3.0-only
// Adapted from Cirno-Labs/th09 src/mainmenu.cpp MainMenu::DrawState15,
// commit d7653152becabb8c86f2f43d49015ecc11bd0652.
// Original MIT portions retain their grant in LICENSE; see THIRD_PARTY_NOTICES.md.
#include <d3dx8math.h>
#include "AsciiManager.hpp"

#include <stddef.h>

typedef unsigned char u8;
typedef unsigned int u32;
typedef int i32;

#include "TitleScreenReplayView.hpp"
#include "TitleReplayNativeSlots.hpp"

// The text API reads the same three-float representation through its shared view.
typedef char ReplaySdkVectorSizeIs0C[(sizeof(D3DXVECTOR3) == sizeof(Float3)) ? 1 : -1];
typedef char ReplaySdkVectorXAt0[(offsetof(D3DXVECTOR3, x) == offsetof(Float3, x)) ? 1 : -1];
typedef char ReplaySdkVectorYAt4[(offsetof(D3DXVECTOR3, y) == offsetof(Float3, y)) ? 1 : -1];
typedef char ReplaySdkVectorZAt8[(offsetof(D3DXVECTOR3, z) == offsetof(Float3, z)) ? 1 : -1];



typedef char ReplaySaveStateAt28[
    (offsetof(TitleScreenView, replaySaveState) == 0x28) ? 1 : -1];
typedef char ReplaySaveTimerAt2C[
    (offsetof(TitleScreenView, stateTimer) == 0x2C) ? 1 : -1];
typedef char ReplaySaveNameAt8C[
    (offsetof(TitleScreenView, replayName) == 0x8C) ? 1 : -1];
typedef char ReplaySaveReplaysAt68E4[
    (offsetof(TitleScreenView, replays) == 0x68E4) ? 1 : -1];
typedef char ReplaySaveSelectedAtC908[
    (offsetof(TitleScreenView, selectedReplay) == 0xC908) ? 1 : -1];
typedef char ReplaySaveTimer2At1B230[
    (offsetof(TitleScreenView, stateTimer2) == 0x1B230) ? 1 : -1];

extern AsciiManager g_AsciiManager;
extern char *g_TitleAlphabet;
extern char g_ReplayPlayTimeText[];

int TitleScreenView::DrawReplaySave()
{
    D3DXVECTOR3 pos(32.0f, 64.0f, 0.0f);
    int i;
    int row;
    int col;
    float scale;
    char str[16];

    if (this->replaySaveState == 2)
    {
        pos.y = 90.0f;
        for (i = 0; i < TitleReplayNativeSlots::RowsPerColumn; i++)
        {
            g_AsciiManager.color = i == this->keyboardSelection ? 0xffffffff : 0xff808080;
            g_AsciiManager.AddFormatText(reinterpret_cast<Float3 *>(&pos), "No.%.2d %.8s %8s", i + 1,
                                  this->replays[i].magic != 0 ? this->replays[i].playerName : "--------",
                                  this->replays[i].magic != 0 ? this->replays[i].playTime : "--/--/--");
            pos.y += 13.0f;
        }
    }
    else if (this->replaySaveState == 3 || this->replaySaveState == 4)
    {
        row = this->selectedReplay;
        pos.y = row * 13.0f + 90.0f;
        D3DXVECTOR3 end = pos;
        end.x = end.y = 224.0f;
        if (this->stateTimer2 < 10)
        {
            pos = pos + (end - pos) * (float)this->stateTimer2 * 0.1f;
        }
        else
        {
            pos = end;
        }
        g_AsciiManager.color = 0xffffffff;
        g_AsciiManager.AddFormatText(reinterpret_cast<Float3 *>(&pos), "No.%.2d %.8s %8s", row + 1, this->replayName, g_ReplayPlayTimeText);
        pos.x += (this->replayNameCursor + 6) * 9;
        g_AsciiManager.AddFormatText(reinterpret_cast<Float3 *>(&pos), "%.8s", "_");
        if (this->replaySaveState == 4)
        {
            pos.z = 0.0f;
            pos.y = 320.0f;
            for (row = 0; row < 6; row++)
            {
                pos.x = 224.0f;
                for (col = 0; col < 16; col++)
                {
                    pos.x += 12.0f;
                    float offsetX = 0.0f;
                    float offsetY = 0.0f;
                    if (this->keyboardSelection == col + row * 16)
                    {
                        g_AsciiManager.color = 0xffffffc0;
                        if (this->stateTimer % 64 < 32)
                        {
                            scale = this->stateTimer % 32 * 0.025f + 1.2f;
                        }
                        else
                        {
                            scale = 2.0f - this->stateTimer % 32 * 0.025f;
                        }
                        g_AsciiManager.scaleX = scale;
                        g_AsciiManager.scaleY = scale;
                        offsetY = offsetX = (scale - 1.0f) * -8.0f;
                    }
                    else
                    {
                        g_AsciiManager.color = 0xc0c0c0c0;
                        g_AsciiManager.scaleX = 1.0f;
                        g_AsciiManager.scaleY = 1.0f;
                    }
                    D3DXVECTOR3 charPos = D3DXVECTOR3(offsetX, offsetY, 0.0f) + pos;
                    str[0] = g_TitleAlphabet[row * 16 + col];
                    str[1] = 0;
                    if (row == 5)
                    {
                        if (col == 14)
                        {
                            str[0] = 0x7f;
                        }
                        else if (col == 15)
                        {
                            str[0] = 0x80;
                        }
                    }
                    g_AsciiManager.AddString(reinterpret_cast<Float3 *>(&charPos), str);
                }
                pos.y += 16.0f;
            }
        }
    }
    g_AsciiManager.color = 0xffffffff;
    g_AsciiManager.isSelected = 0;
    g_AsciiManager.scaleX = 1.0f;
    g_AsciiManager.scaleY = 1.0f;
    return 1;
}
