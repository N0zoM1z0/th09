#include "AsciiManager.hpp"

#include <stddef.h>

typedef unsigned char u8;
typedef unsigned int u32;
typedef int i32;

struct ReplaySaveReplayDataView
{
    u32 magic;
    u8 unknown004[0xC4 - 4];
    char playTime[10];
    char playerName[8];
    u8 unknown0D6[0x1EC - 0xD6];
};

typedef char ReplaySaveReplayDataSizeIs1EC[
    (sizeof(ReplaySaveReplayDataView) == 0x1EC) ? 1 : -1];

struct ReplaySavePosition
{
    float x;
    float y;
    float z;
};

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
    ReplaySaveReplayDataView replays[50];   // +0x068E4
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

static __inline Float3 *ReplaySaveFloat3(ReplaySavePosition *position)
{
    return reinterpret_cast<Float3 *>(position);
}

int TitleScreenView::DrawReplaySave()
{
    ReplaySavePosition position = {32.0f, 64.0f, 0.0f};

    if (replaySaveState == 2)
    {
        position.y = 90.0f;
        ReplaySaveReplayDataView *replay = replays;
        AsciiManager *ascii = &g_AsciiManager;

        for (i32 i = 0; i < 25; ++i, ++replay)
        {
            ascii->color =
                (i == keyboardSelection) ? 0xFFFFFFFFu : 0xFF808080u;

            const char *playTime = replay->playTime;
            const char *playerName;
            if (replay->magic != 0)
            {
                playerName = replay->playerName;
            }
            else
            {
                playTime = "--/--/--";
                playerName = "--------";
            }

            ascii->AddFormatText(
                ReplaySaveFloat3(&position),
                "No.%.2d %.8s %8s",
                i + 1, playerName, playTime);
            position.y += 13.0f;
        }
    }
    else if (replaySaveState == 3 || replaySaveState == 4)
    {
        i32 selected = selectedReplay;
        i32 frame = stateTimer2;

        position.y = (float)selected * 13.0f + 90.0f;
        ReplaySavePosition target = position;
        target.x = 224.0f;
        target.y = 224.0f;

        if (frame < 10)
        {
            target.x -= position.x;
            target.y -= position.y;
            target.z -= position.z;

            target.x *= (float)frame;
            target.y *= (float)frame;
            target.z *= (float)frame;

            target.x *= 0.1f;
            target.y *= 0.1f;
            target.z *= 0.1f;

            target.x += position.x;
            target.y += position.y;
            target.z += position.z;
        }
        position = target;

        g_AsciiManager.color = 0xFFFFFFFFu;
        g_AsciiManager.AddFormatText(
            ReplaySaveFloat3(&position),
            "No.%.2d %.8s %8s",
            selected + 1, replayName, g_ReplayPlayTimeText);

        position.x += (float)((replayNameCursor + 6) * 9);
        g_AsciiManager.AddFormatText(
            ReplaySaveFloat3(&position), "%.8s", "_");

        if (replaySaveState == 4)
        {
            position.y = 320.0f;
            position.z = 0.0f;

            for (i32 row = 0; row < 6; ++row)
            {
                position.x = 224.0f;
                for (i32 column = 0; column < 16; ++column)
                {
                    position.x += 12.0f;
                    i32 index = row * 16 + column;
                    float offset = 0.0f;

                    if (keyboardSelection == index)
                    {
                        g_AsciiManager.color = 0xFFFFFFC0u;

                        float scale;
                        if ((stateTimer % 64) < 32)
                            scale = (float)(stateTimer % 32) * 0.025f + 1.2f;
                        else
                            scale = 2.0f - (float)(stateTimer % 32) * 0.025f;

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

                    ReplaySavePosition characterPosition = {
                        position.x + offset,
                        position.y + offset,
                        position.z,
                    };
                    char text[2] = {g_TitleAlphabet[index], 0};
                    if (row == 5)
                    {
                        if (column == 14)
                            text[0] = 0x7F;
                        else if (column == 15)
                            text[0] = (char)0x80;
                    }

                    g_AsciiManager.AddString(
                        ReplaySaveFloat3(&characterPosition), text);
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
