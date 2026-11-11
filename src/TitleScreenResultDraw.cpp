// SPDX-License-Identifier: GPL-3.0-only
// Adapted from Cirno-Labs/th09 src/mainmenu.cpp, MainMenu::DrawState13,
// commit d7653152becabb8c86f2f43d49015ecc11bd0652.
// Original MIT portions retain their grant in LICENSE; see THIRD_PARTY_NOTICES.md.
#include "AsciiManager.hpp"
#include "AsciiGameManagerView.hpp"

#include <stddef.h>


typedef unsigned char u8;
typedef unsigned int u32;
typedef int i32;

#include "TitleScoreRecordView.hpp"

// Result drawing sees the same complete +0x1B388 Title object as
// core, options, and replay menu/save. No duplicate prefix is allocated.
#include "TitleScreenReplayView.hpp"
#include "TitleNativeNameKeyboard.hpp"

typedef char ResultDrawKeyboardAt0[
    (offsetof(TitleScreenView, keyboardSelection) == 0) ? 1 : -1];
typedef char ResultDrawNameCursorAt4[
    (offsetof(TitleScreenView, replayNameCursor) == 4) ? 1 : -1];
typedef char ResultDrawBankAt8[
    (offsetof(TitleScreenView, nameBankIndex) == 8) ? 1 : -1];
typedef char ResultDrawSlotAtC[
    (offsetof(TitleScreenView, nameSlotIndex) == 0x0C) ? 1 : -1];
typedef char ResultDrawStateAt28[
    (offsetof(TitleScreenView, currentScreenState) == 0x28) ? 1 : -1];
typedef char ResultDrawTimerAt2C[
    (offsetof(TitleScreenView, stateTimer) == 0x2C) ? 1 : -1];
typedef char ResultDrawScreenAt1B228[
    (offsetof(TitleScreenView, currentScreen) == 0x1B228) ? 1 : -1];
typedef char ResultDrawTimer2At1B230[
    (offsetof(TitleScreenView, stateTimer2) == 0x1B230) ? 1 : -1];

extern AsciiManager g_AsciiManager;
extern const char *g_TitleRankLabels[];
extern char *g_TitleAlphabet;

// The original 16*5*5 score storage contains 0x2C-byte rank records.
// Use native unsigned-DWORD byte addresses for the live bank, rank and
// label selector. Each difficulty row is five real score records (0xDC).
#define TH09_RESULT_NATIVE_BANK(index) \
    reinterpret_cast<const TitleScoreRecordView (*)[5]>( \
        reinterpret_cast<unsigned int>(&g_TitleScoreTable[0]) + \
        static_cast<unsigned int>(index) * sizeof(g_TitleScoreTable[0]))
#define TH09_RESULT_NATIVE_LABEL(index) \
    (*reinterpret_cast<const char *const *>( \
        reinterpret_cast<unsigned int>(&g_TitleRankLabels[0]) + \
        static_cast<unsigned int>(index) * sizeof(g_TitleRankLabels[0])))
#define TH09_RESULT_NATIVE_SCORE(index) \
    (*reinterpret_cast<const TitleScoreRecordView *>( \
        reinterpret_cast<unsigned int>(&(*scores)[0]) + \
        static_cast<unsigned int>(index) * sizeof((*scores)[0])))
#define TH09_RESULT_NATIVE_ALPHA(index) \
    (*reinterpret_cast<const char *>( \
        reinterpret_cast<unsigned int>(&g_TitleAlphabet[0]) + \
        static_cast<unsigned int>(index)))


int TitleScreenView::DrawResult()
{
    Float3 pos;
    int row;
    int col;
    int mask;
    int highlight;
    int alpha;
    float scale;
    char str[16];
    // Score entries are read while rendering; only the bank cursor advances.
    const TitleScoreRecordView (*scores)[5] = TH09_RESULT_NATIVE_BANK(nameBankIndex);
    if (this->currentScreen == 13)
    {
        mask = 0xff;
        highlight = 99;
    }
    else if (this->currentScreen == 14)
    {
        mask = 1 << g_GameManager.difficulty;
        highlight = this->nameSlotIndex;
    }
    alpha = this->stateTimer2 < 30 ? this->stateTimer2 * 255 / 30 : 255;
    g_AsciiManager.color = mask & 1 ? (alpha << 24) | 0xd0ffd0 : (alpha << 24) | 0x808080;
    pos.x = 102.0f;
    pos.y = 80.0f;
    pos.z = 0.0f;
    g_AsciiManager.AddFormatText(&pos, "Easy Ranking");
    pos.y += 16.0f;
    {
        int i;

        for (i = 0; i < 5; i++)
        {
            if (mask & 1)
            {
                g_AsciiManager.color = highlight == i ? (alpha << 24) | 0xfff0ef : (alpha << 24) | 0xc0f0ff;
            }
            else
            {
                g_AsciiManager.color = (alpha << 24) | 0x809090;
            }
            if (mask & 1 && highlight == i)
            {
                pos.x += (this->replayNameCursor + 4) * 9;
                g_AsciiManager.AddFormatText(&pos, "%.8s", "_");
                pos.x -= (this->replayNameCursor + 4) * 9;
            }
            g_AsciiManager.AddFormatText(&pos, "%s %.8s %.9d%d", TH09_RESULT_NATIVE_LABEL(i), TH09_RESULT_NATIVE_SCORE(i).name, TH09_RESULT_NATIVE_SCORE(i).score,
                                  (char)TH09_RESULT_NATIVE_SCORE(i).scoreDigit);
            pos.y += 16.0f;
        }
    }
    scores = reinterpret_cast<const TitleScoreRecordView (*)[5]>(reinterpret_cast<unsigned int>(scores) + sizeof(*scores));
    g_AsciiManager.color = mask & 2 ? (alpha << 24) | 0xd0ffd0 : (alpha << 24) | 0x808080;
    pos.x = 358.0f;
    pos.y = 80.0f;
    pos.z = 0.0f;
    g_AsciiManager.AddFormatText(&pos, "Normal Ranking");
    pos.y += 16.0f;
    {
        int i;
        for (i = 0; i < 5; i++)
        {
            if (mask & 2)
            {
                g_AsciiManager.color = highlight == i ? (alpha << 24) | 0xfff0ef : (alpha << 24) | 0xc0f0ff;
            }
            else
            {
                g_AsciiManager.color = (alpha << 24) | 0x809090;
            }
            if (mask & 2 && highlight == i)
            {
                pos.x += (this->replayNameCursor + 4) * 9;
                g_AsciiManager.AddFormatText(&pos, "%.8s", "_");
                pos.x -= (this->replayNameCursor + 4) * 9;
            }
            g_AsciiManager.AddFormatText(&pos, "%s %.8s %.9d%d", TH09_RESULT_NATIVE_LABEL(i), TH09_RESULT_NATIVE_SCORE(i).name, TH09_RESULT_NATIVE_SCORE(i).score,
                                  (char)TH09_RESULT_NATIVE_SCORE(i).scoreDigit);
            pos.y += 16.0f;
        }
    }
    scores = reinterpret_cast<const TitleScoreRecordView (*)[5]>(reinterpret_cast<unsigned int>(scores) + sizeof(*scores));
    g_AsciiManager.color = mask & 4 ? (alpha << 24) | 0xfff0d0 : (alpha << 24) | 0x808080;
    pos.x = 102.0f;
    pos.y = 192.0f;
    pos.z = 0.0f;
    g_AsciiManager.AddFormatText(&pos, "Hard Ranking");
    pos.y += 16.0f;
    {
        int i;
        for (i = 0; i < 5; i++)
        {
            if (mask & 4)
            {
                g_AsciiManager.color = highlight == i ? (alpha << 24) | 0xfff0ef : (alpha << 24) | 0xc0f0ff;
            }
            else
            {
                g_AsciiManager.color = (alpha << 24) | 0x809090;
            }
            if (mask & 4 && highlight == i)
            {
                pos.x += (this->replayNameCursor + 4) * 9;
                g_AsciiManager.AddFormatText(&pos, "%.8s", "_");
                pos.x -= (this->replayNameCursor + 4) * 9;
            }
            g_AsciiManager.AddFormatText(&pos, "%s %.8s %.9d%d", TH09_RESULT_NATIVE_LABEL(i), TH09_RESULT_NATIVE_SCORE(i).name, TH09_RESULT_NATIVE_SCORE(i).score,
                                  (char)TH09_RESULT_NATIVE_SCORE(i).scoreDigit);
            pos.y += 16.0f;
        }
    }
    scores = reinterpret_cast<const TitleScoreRecordView (*)[5]>(reinterpret_cast<unsigned int>(scores) + sizeof(*scores));
    g_AsciiManager.color = mask & 8 ? (alpha << 24) | 0xffc0c0 : (alpha << 24) | 0x808080;
    pos.x = 358.0f;
    pos.y = 192.0f;
    pos.z = 0.0f;
    g_AsciiManager.AddFormatText(&pos, "Lunatic Ranking");
    pos.y += 16.0f;
    {
        int i;
        for (i = 0; i < 5; i++)
        {
            if (mask & 8)
            {
                g_AsciiManager.color = highlight == i ? (alpha << 24) | 0xfff0ef : (alpha << 24) | 0xc0f0ff;
            }
            else
            {
                g_AsciiManager.color = (alpha << 24) | 0x809090;
            }
            if (mask & 8 && highlight == i)
            {
                pos.x += (this->replayNameCursor + 4) * 9;
                g_AsciiManager.AddFormatText(&pos, "%.8s", "_");
                pos.x -= (this->replayNameCursor + 4) * 9;
            }
            g_AsciiManager.AddFormatText(&pos, "%s %.8s %.9d%d", TH09_RESULT_NATIVE_LABEL(i), TH09_RESULT_NATIVE_SCORE(i).name, TH09_RESULT_NATIVE_SCORE(i).score,
                                  (char)TH09_RESULT_NATIVE_SCORE(i).scoreDigit);
            pos.y += 16.0f;
        }
    }
    scores = reinterpret_cast<const TitleScoreRecordView (*)[5]>(reinterpret_cast<unsigned int>(scores) + sizeof(*scores));
    g_AsciiManager.color = mask & 16 ? (alpha << 24) | 0xffc0f0 : (alpha << 24) | 0x808080;
    pos.x = 102.0f;
    pos.y = 304.0f;
    pos.z = 0.0f;
    g_AsciiManager.AddFormatText(&pos, "Extra Ranking");
    pos.y += 16.0f;
    {
        int i;
        for (i = 0; i < 5; i++)
        {
            if (mask & 16)
            {
                g_AsciiManager.color = highlight == i ? (alpha << 24) | 0xfff0ef : (alpha << 24) | 0xc0f0ff;
            }
            else
            {
                g_AsciiManager.color = (alpha << 24) | 0x809090;
            }
            if (mask & 16 && highlight == i)
            {
                pos.x += (this->replayNameCursor + 4) * 9;
                g_AsciiManager.AddFormatText(&pos, "%.8s", "_");
                pos.x -= (this->replayNameCursor + 4) * 9;
            }
            g_AsciiManager.AddFormatText(&pos, "%s %.8s %.9d%d", TH09_RESULT_NATIVE_LABEL(i), TH09_RESULT_NATIVE_SCORE(i).name, TH09_RESULT_NATIVE_SCORE(i).score,
                                  (char)TH09_RESULT_NATIVE_SCORE(i).scoreDigit);
            pos.y += 16.0f;
        }
    }
    if (this->currentScreen == 13)
    {
    }
    else if (this->currentScreen == 14 && this->nameSlotIndex < 5 && this->currentScreenState >= 1)
    {
        pos.z = 0.0f;
        pos.y = 336.0f;
        for (row = 0; row < TitleNativeNameKeyboard::Rows; row++)
        {
            pos.x = 348.0f;
            for (col = 0; col < TitleNativeNameKeyboard::Columns; col++)
            {
                pos.x += 12.0f;
                float offsetX = 0.0f;
                float offsetY = 0.0f;
                if (this->keyboardSelection == col + row * TitleNativeNameKeyboard::Columns)
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
                Float3 charPos;
                charPos.x = offsetX + pos.x;
                charPos.y = offsetY + pos.y;
                charPos.z = pos.z;
                str[0] = TH09_RESULT_NATIVE_ALPHA(row * TitleNativeNameKeyboard::Columns + col);
                str[1] = 0;
                if (row == TitleNativeNameKeyboard::LastRow)
                {
                    if (col == TitleNativeNameKeyboard::SpaceColumn)
                    {
                        str[0] = 0x7f;
                    }
                    else if (col == TitleNativeNameKeyboard::ConfirmColumn)
                    {
                        str[0] = 0x80;
                    }
                }
                g_AsciiManager.AddString(&charPos, str);
            }
            pos.y += 16.0f;
        }
    }
    g_AsciiManager.color = 0xffffffff;
    g_AsciiManager.scaleX = 1.0f;
    g_AsciiManager.scaleY = 1.0f;
    return 1;
}

#undef TH09_RESULT_NATIVE_BANK
#undef TH09_RESULT_NATIVE_LABEL
#undef TH09_RESULT_NATIVE_SCORE
#undef TH09_RESULT_NATIVE_ALPHA
