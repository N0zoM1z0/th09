#include "AsciiManager.hpp"

#include <stddef.h>

typedef unsigned char u8;
typedef unsigned int u32;
typedef int i32;

struct ResultDrawScoreRecord
{
    u8 unknown00[0x0C];
    i32 score;
    u8 unknown10[0x08];
    char name[9];
    u8 unknown21[0x0A];
    char scoreDigit;
};
typedef char ResultDrawScoreRecordSizeIs2C[
    (sizeof(ResultDrawScoreRecord) == 0x2C) ? 1 : -1];
typedef char ResultDrawScoreAt0C[
    (offsetof(ResultDrawScoreRecord, score) == 0x0C) ? 1 : -1];
typedef char ResultDrawNameAt18[
    (offsetof(ResultDrawScoreRecord, name) == 0x18) ? 1 : -1];
typedef char ResultDrawDigitAt2B[
    (offsetof(ResultDrawScoreRecord, scoreDigit) == 0x2B) ? 1 : -1];

struct ResultDrawPosition
{
    float x;
    float y;
    float z;
};

struct TitleScreenView
{
    i32 keyboardSelection;       // +0x00000
    i32 replayNameCursor;        // +0x00004
    i32 nameBankIndex;           // +0x00008
    i32 nameSlotIndex;           // +0x0000C
    u8 unknown00010[0x18];
    i32 currentScreenState;      // +0x00028
    i32 stateTimer;              // +0x0002C
    u8 unknown00030[0x1B228 - 0x30];
    i32 currentScreen;           // +0x1B228
    u8 unknown1B22C[4];
    i32 stateTimer2;             // +0x1B230

    int DrawResult();
};

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
extern i32 g_TitleNameTableIndex;
extern ResultDrawScoreRecord g_TitleScoreTable[16][5][5];
extern const char *g_TitleRankLabels[];
extern char *g_TitleAlphabet;

static __inline Float3 *ResultDrawFloat3(ResultDrawPosition *position)
{
    return reinterpret_cast<Float3 *>(position);
}

#define DRAW_RANKING_BLOCK(bitValue, xValue, yValue, headingText, headingActiveColor) \
    do { \
        active = difficultyMask & (bitValue); \
        g_AsciiManager.color = alphaColor | \
            (active ? (headingActiveColor) : 0x00808080u); \
        position.x = (xValue); \
        position.y = (yValue); \
        position.z = 0.0f; \
        g_AsciiManager.AddFormatText(ResultDrawFloat3(&position), (headingText)); \
        position.y += 16.0f; \
        row = 0; \
        ResultDrawScoreRecord *entry = scoreGroup; \
        while (row < 5) { \
            if (!active) \
                g_AsciiManager.color = alphaColor | 0x00809090u; \
            else if (selectedRank == row) \
                g_AsciiManager.color = alphaColor | 0x00FFF0EFu; \
            else \
                g_AsciiManager.color = alphaColor | 0x00C0F0FFu; \
            if (active && selectedRank == row) { \
                temp = 9 * (replayNameCursor + 4); \
                position.x += (float)temp; \
                g_AsciiManager.AddFormatText( \
                    ResultDrawFloat3(&position), "%.8s", "_"); \
                temp = 9 * (replayNameCursor + 4); \
                position.x -= (float)temp; \
            } \
            g_AsciiManager.AddFormatText( \
                ResultDrawFloat3(&position), "%s %.8s %.9d%d", \
                g_TitleRankLabels[row], entry->name, entry->score, \
                entry->scoreDigit); \
            ++row; \
            ++entry; \
            position.y += 16.0f; \
        } \
        scoreGroup += 5; \
    } while (0)

int TitleScreenView::DrawResult()
{
    char text[16];
    ResultDrawPosition characterPosition;
    ResultDrawPosition position;
    i32 temp;
    i32 active;
    i32 difficultyMask;
    i32 row;
    i32 selectedRank;
    ResultDrawScoreRecord *scoreGroup;
    u32 alphaColor;

    scoreGroup = &g_TitleScoreTable[nameBankIndex][0][0];

    if (currentScreen == 13)
    {
        difficultyMask = 0xFF;
        selectedRank = 99;
    }
    else if (currentScreen == 14)
    {
        difficultyMask = 1 << g_TitleNameTableIndex;
        selectedRank = nameSlotIndex;
    }

    i32 alpha;
    if (stateTimer2 < 30)
        alpha = 255 * stateTimer2 / 30;
    else
        alpha = 255;
    alphaColor = (u32)alpha << 24;

    DRAW_RANKING_BLOCK(1, 102.0f, 80.0f, "Easy Ranking", 0x00D0FFD0u);
    DRAW_RANKING_BLOCK(2, 358.0f, 80.0f, "Normal Ranking", 0x00D0FFD0u);
    DRAW_RANKING_BLOCK(4, 102.0f, 192.0f, "Hard Ranking", 0x00FFF0D0u);
    DRAW_RANKING_BLOCK(8, 358.0f, 192.0f, "Lunatic Ranking", 0x00FFC0C0u);
    DRAW_RANKING_BLOCK(0x10, 102.0f, 304.0f, "Extra Ranking", 0x00FFC0F0u);

    if (currentScreen == 14 && nameSlotIndex < 5 && currentScreenState >= 1)
    {
        position.y = 336.0f;
        position.z = 0.0f;
        row = 0;

        while (row < 6)
        {
            i32 column = 0;
            position.x = 348.0f;
            i32 rowBase = row * 16;
            while (column < 16)
            {
                position.x += 12.0f;
                float offset = 0.0f;

                if (keyboardSelection == rowBase + column)
                {
                    g_AsciiManager.color = 0xFFFFFFC0u;

                    float scale;
                    if ((stateTimer % 64) < 32)
                    {
                        temp = stateTimer % 32;
                        scale = (float)temp * 0.025f + 1.2f;
                    }
                    else
                    {
                        temp = stateTimer % 32;
                        scale = 2.0f - (float)temp * 0.025f;
                    }

                    g_AsciiManager.scaleX = scale;
                    g_AsciiManager.scaleY = scale;
                    offset = (scale - 1.0f) * -8.0f;
                }
                else
                {
                    g_AsciiManager.color = 0xC0C0C0C0u;
                    g_AsciiManager.scaleX = 1.0f;
                    g_AsciiManager.scaleY = 1.0f;
                }

                characterPosition.x = position.x + offset;
                characterPosition.y = position.y + offset;
                characterPosition.z = position.z;
                text[0] = g_TitleAlphabet[rowBase + column];
                text[1] = 0;

                if (row == 5)
                {
                    if (column == 14)
                        text[0] = 0x7F;
                    else if (column == 15)
                        text[0] = (char)0x80;
                }

                g_AsciiManager.AddString(
                    ResultDrawFloat3(&characterPosition), text);
                ++column;
            }
            ++row;
            position.y += 16.0f;
        }
    }

    g_AsciiManager.color = 0xFFFFFFFFu;
    g_AsciiManager.scaleX = 1.0f;
    g_AsciiManager.scaleY = 1.0f;
    return 1;
}

#undef DRAW_RANKING_BLOCK
