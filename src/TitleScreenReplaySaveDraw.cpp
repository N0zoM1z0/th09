#include <d3dx8math.h>
#include "AsciiManager.hpp"

#include <stddef.h>

typedef unsigned char u8;
typedef unsigned int u32;
typedef int i32;

#include "ReplayDataOriginalView.hpp"

// The text API reads the same three-float representation through its shared view.
typedef char ReplaySdkVectorSizeIs0C[(sizeof(D3DXVECTOR3) == sizeof(Float3)) ? 1 : -1];
typedef char ReplaySdkVectorXAt0[(offsetof(D3DXVECTOR3, x) == offsetof(Float3, x)) ? 1 : -1];
typedef char ReplaySdkVectorYAt4[(offsetof(D3DXVECTOR3, y) == offsetof(Float3, y)) ? 1 : -1];
typedef char ReplaySdkVectorZAt8[(offsetof(D3DXVECTOR3, z) == offsetof(Float3, z)) ? 1 : -1];

struct TitleScreenView
{
    i32 keyboardSelection;                  // +0x00000
    i32 replayNameCursor;                   // +0x00004
    u8 unknown00008[0x20];
    i32 replaySaveState;                    // +0x00028
    i32 stateTimer;                         // +0x0002C
    u8 unknown00030[0x5C];
    char replayName[9];                     // +0x0008C
    u8 unknown00095[0x68E4 - 0x95];
    ReplayDataView replays[50];                 // +0x068E4
    u8 unknown0C8FC[0x0C];
    i32 selectedReplay;                     // +0x0C908
    u8 unknown0C90C[0x1B230 - 0xC90C];
    i32 stateTimer2;                        // +0x1B230

    int DrawReplaySave();
};

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

// One character and its terminator occupy the existing two-byte temporary.
struct ReplayGlyphText
{
    char bytes[2];

    explicit ReplayGlyphText(char character)
    {
        bytes[0] = character;
        bytes[1] = 0;
    }
};
typedef char ReplayGlyphTextSizeIs02[(sizeof(ReplayGlyphText) == 2) ? 1 : -1];
typedef char ReplayGlyphTextBytesAt0[(offsetof(ReplayGlyphText, bytes) == 0) ? 1 : -1];

int TitleScreenView::DrawReplaySave()
{
    D3DXVECTOR3 position;
    position.x = 32.0f;
    position.y = 64.0f;
    position.z = 0.0f;

    if (replaySaveState == 2)
    {
        position.y = 90.0f;
        ReplayDataView *replay = replays;

        for (i32 i = 0; i < 25; ++i, ++replay)
        {
            g_AsciiManager.color =
                (i == keyboardSelection) ? 0xFFFFFFFFu : 0xFF808080u;

            // Reuse the stored magic directly for both displayed fields.
            u32 valid = replay->magic;
            const char *playTime = valid ? replay->playTime : "--/--/--";
            const char *playerName = valid ? replay->playerName : "--------";

            g_AsciiManager.AddFormatText(
                reinterpret_cast<Float3 *>(&position),
                "No.%.2d %.8s %8s",
                i + 1, playerName, playTime);
            position.y += 13.0f;
        }
    }
    else if (replaySaveState == 3 || replaySaveState == 4)
    {
        i32 row = selectedReplay;
        i32 frame = stateTimer2;

        position.y = (float)row * 13.0f + 90.0f;
        D3DXVECTOR3 target = position;
        target.y = target.x = 224.0f;

        if (frame < 10)
        {
            D3DXVECTOR3 scaled;
            // Preserve the two original scale operations and their order.
            D3DXVec3Subtract(&scaled, &target, &position);
            D3DXVec3Scale(&target, &scaled, (float)frame);
            D3DXVec3Scale(&scaled, &target, 0.1f);
            D3DXVec3Add(&target, &scaled, &position);
        }
        position = target;

        g_AsciiManager.color = 0xFFFFFFFFu;
        g_AsciiManager.AddFormatText(
            reinterpret_cast<Float3 *>(&position),
            "No.%.2d %.8s %8s",
            row + 1, replayName, g_ReplayPlayTimeText);

        position.x += (float)((replayNameCursor + 6) * 9);
        g_AsciiManager.AddFormatText(
            reinterpret_cast<Float3 *>(&position), "%.8s", "_");

        if (replaySaveState == 4)
        {
            position.y = 320.0f;
            position.z = 0.0f;

            for (row = 0; row < 6; ++row)
            {
                position.x = 224.0f;
                i32 rowBase = row * 16;
                for (i32 column = 0; column < 16; ++column)
                {
                    position.x += 12.0f;
                    float offsetX = 0.0f;
                    float offsetY = 0.0f;

                    if (keyboardSelection == rowBase + column)
                    {
                        g_AsciiManager.color = 0xFFFFFFC0u;

                        float scale;
                        if ((stateTimer % 64) < 32)
                            scale = (float)(stateTimer % 32) * 0.025f + 1.2f;
                        else
                            scale = 2.0f - (float)(stateTimer % 32) * 0.025f;

                        g_AsciiManager.scaleX = scale;
                        g_AsciiManager.scaleY = scale;
                        offsetY = offsetX = (scale - 1.0f) * -8.0f;
                    }
                    else
                    {
                        g_AsciiManager.color = 0xC0C0C0C0u;
                        g_AsciiManager.scaleX = 1.0f;
                        g_AsciiManager.scaleY = 1.0f;
                    }

                    D3DXVECTOR3 characterPosition;
                    characterPosition.x = position.x + offsetX;
                    characterPosition.y = position.y + offsetY;
                    characterPosition.z = position.z;
                    ReplayGlyphText text(g_TitleAlphabet[rowBase + column]);
                    if (row == 5)
                    {
                        if (column == 14)
                            text.bytes[0] = 0x7F;
                        else if (column == 15)
                            text.bytes[0] = (char)0x80;
                    }

                    g_AsciiManager.AddString(
                        reinterpret_cast<Float3 *>(&characterPosition), text.bytes);
                }
                position.y += 16.0f;
            }
        }
    }

    g_AsciiManager.color = 0xFFFFFFFFu;
    g_AsciiManager.isSelected = 0;
    g_AsciiManager.scaleX = 1.0f;
    g_AsciiManager.scaleY = 1.0f;
    return 1;
}
