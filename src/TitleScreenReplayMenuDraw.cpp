#include "AsciiManager.hpp"

#include <stddef.h>
#include <stdio.h>
#include <string.h>

typedef unsigned char u8;
typedef unsigned int u32;
typedef int i32;

struct ReplayMenuFrameView
{
    u8 unknown000[6];
    u8 character;
    u8 unknown007[0x20 - 7];
};
typedef char ReplayMenuFrameSizeIs20[
    (sizeof(ReplayMenuFrameView) == 0x20) ? 1 : -1];

struct ReplayMenuReplayView
{
    u32 magic;
    u8 unknown004[0x20 - 4];
    ReplayMenuFrameView *frameStart[3][10];
    u8 unknown098[0xC4 - 0x98];
    char playTime[10];
    char playerName[8];
    u8 unknown0D6;
    u8 difficulty;
    u8 unknown0D8[0x1E4 - 0xD8];
    u8 replayMode;
    u8 versusMode;
    u8 unknown1E6[0x1EC - 0x1E6];
};
typedef char ReplayMenuReplaySizeIs1EC[
    (sizeof(ReplayMenuReplayView) == 0x1EC) ? 1 : -1];
typedef char ReplayMenuFrameAt20[
    (offsetof(ReplayMenuReplayView, frameStart) == 0x20) ? 1 : -1];
typedef char ReplayMenuPlayTimeAtC4[
    (offsetof(ReplayMenuReplayView, playTime) == 0xC4) ? 1 : -1];
typedef char ReplayMenuPlayerNameAtCE[
    (offsetof(ReplayMenuReplayView, playerName) == 0xCE) ? 1 : -1];
typedef char ReplayMenuDifficultyAtD7[
    (offsetof(ReplayMenuReplayView, difficulty) == 0xD7) ? 1 : -1];
typedef char ReplayMenuModeAt1E4[
    (offsetof(ReplayMenuReplayView, replayMode) == 0x1E4) ? 1 : -1];
typedef char ReplayMenuVersusAt1E5[
    (offsetof(ReplayMenuReplayView, versusMode) == 0x1E5) ? 1 : -1];

struct TitleScreenView
{
    i32 keyboardSelection;                   // +0x00000
    u8 unknown00004[0x24];
    i32 currentScreenState;                  // +0x00028
    u8 unknown0002C[0x28C - 0x2C];
    char replayPaths[50][0x200];             // +0x0028C
    u8 unknown0668C[0x68E4 - 0x668C];
    ReplayMenuReplayView replays[50];        // +0x068E4
    u8 unknown0C8FC[0x0C];
    i32 selectedReplay;                      // +0x0C908
    u8 unknown0C90C[0x1B230 - 0xC90C];
    i32 stateTimer2;                         // +0x1B230

    int DrawReplayMenu();
};

typedef char ReplayMenuStateAt28[
    (offsetof(TitleScreenView, currentScreenState) == 0x28) ? 1 : -1];
typedef char ReplayMenuPathsAt28C[
    (offsetof(TitleScreenView, replayPaths) == 0x28C) ? 1 : -1];
typedef char ReplayMenuReplaysAt68E4[
    (offsetof(TitleScreenView, replays) == 0x68E4) ? 1 : -1];
typedef char ReplayMenuSelectedAtC908[
    (offsetof(TitleScreenView, selectedReplay) == 0xC908) ? 1 : -1];
typedef char ReplayMenuTimerAt1B230[
    (offsetof(TitleScreenView, stateTimer2) == 0x1B230) ? 1 : -1];

extern AsciiManager g_AsciiManager;
extern const char *g_ReplayDifficultyNames[];
extern const char *g_ReplayModeNames[];
extern const char *g_ReplayCharacterNames[];
extern const char *g_ReplayStoryModeName[];
extern const char *g_ReplayExtraModeName[];

static __inline const char *ReplayMenuPlayTime(ReplayMenuReplayView *replay)
{
    return replay->magic ? replay->playTime : "--/--/--";
}

static __inline const char *ReplayMenuPlayerName(ReplayMenuReplayView *replay)
{
    return replay->magic ? replay->playerName : "--------";
}

int TitleScreenView::DrawReplayMenu()
{
    Float3 position;

    if (currentScreenState == 1)
    {
        position.x = 32.0f;
        position.y = 90.0f;
        position.z = 0.0f;

        for (i32 i = 0; i < 50; ++i)
        {
            if (i == 25)
            {
                position.x = 352.0f;
                position.y = 90.0f;
            }

            g_AsciiManager.color =
                (i == keyboardSelection) ? 0xFFFFFFFFu : 0xFF808080u;

            ReplayMenuReplayView *replay = &replays[i];
            if (i < 25)
            {
                g_AsciiManager.AddFormatText(
                    &position,
                    "No.%.2d %8s %8s",
                    i + 1,
                    ReplayMenuPlayerName(replay),
                    ReplayMenuPlayTime(replay));
            }
            else
            {
                char c0 = replay->magic ? replayPaths[i][15] : 'u';
                char c1 = replay->magic ? replayPaths[i][16] : 's';
                char c2 = replay->magic ? replayPaths[i][17] : 'e';
                char c3 = replay->magic ? replayPaths[i][18] : 'r';
                g_AsciiManager.AddFormatText(
                    &position,
                    "%c%c%c%c  %8s %8s",
                    c0, c1, c2, c3,
                    ReplayMenuPlayerName(replay),
                    ReplayMenuPlayTime(replay));
            }
            position.y += 13.0f;
        }
    }
    else if (currentScreenState == 2)
    {
        i32 selected = selectedReplay;
        ReplayMenuReplayView *replay = &replays[selected];
        i32 frame = stateTimer2;

        position.x = selected < 25 ? 32.0f : 352.0f;
        position.y = (float)selected * 13.0f + 90.0f;
        position.z = 0.0f;

        Float3 target;
        target.x = 216.5f;
        target.y = 160.0f;
        target.z = 0.0f;
        if (frame < 10)
        {
            Float3 scaled;
            scaled.x = target.x - position.x;
            scaled.y = target.y - position.y;
            scaled.z = target.z - position.z;
            target.x = scaled.x * (float)frame;
            target.y = scaled.y * (float)frame;
            target.z = scaled.z * (float)frame;
            scaled.x = target.x * 0.1f;
            scaled.y = target.y * 0.1f;
            scaled.z = target.z * 0.1f;
            target.x = scaled.x + position.x;
            target.y = scaled.y + position.y;
            target.z = scaled.z + position.z;
        }
        position = target;

        if (selected < 25)
        {
            g_AsciiManager.AddFormatText(
                &position,
                "No.%.2d %8s %8s",
                selected + 1,
                ReplayMenuPlayerName(replay),
                ReplayMenuPlayTime(replay));
        }
        else
        {
            char c0 = replay->magic ? replayPaths[selected][15] : 'u';
            char c1 = replay->magic ? replayPaths[selected][16] : 's';
            char c2 = replay->magic ? replayPaths[selected][17] : 'e';
            char c3 = replay->magic ? replayPaths[selected][18] : 'r';
            g_AsciiManager.AddFormatText(
                &position,
                "%c%c%c%c  %8s %8s",
                c0, c1, c2, c3,
                ReplayMenuPlayerName(replay),
                ReplayMenuPlayTime(replay));
        }

        position.x = 221.0f;
        position.y = 240.0f;

        if (replays[selected].replayMode == 0)
        {
            char buffer[256];
            sprintf(
                buffer, "%s  -%s-",
                g_ReplayStoryModeName[0],
                g_ReplayDifficultyNames[replay->difficulty]);
            position.x = 320.0f - (float)strlen(buffer) * 4.5f;
            g_AsciiManager.AddFormatText(&position, buffer);
        }
        else if (replays[selected].replayMode == 1)
        {
            char buffer[256];
            sprintf(buffer, "%s", g_ReplayExtraModeName[0]);
            position.x = 320.0f - (float)strlen(buffer) * 4.5f;
            g_AsciiManager.AddFormatText(&position, buffer);
        }
        else if (replays[selected].replayMode == 2)
        {
            char buffer[256];
            sprintf(
                buffer, "%s  -%s-",
                g_ReplayModeNames[replay->versusMode],
                g_ReplayDifficultyNames[replay->difficulty]);
            position.x = 320.0f - (float)strlen(buffer) * 4.5f;
            g_AsciiManager.AddFormatText(&position, buffer);
        }

        position.x = 230.0f;
        position.y = 270.0f;

        for (i32 i = 0; i < 10; ++i)
        {
            if (replay->frameStart[0][i] != 0)
            {
                g_AsciiManager.color =
                    (i == keyboardSelection) ? 0xFFFFFFFFu : 0xFF808080u;
                g_AsciiManager.AddFormatText(
                    &position,
                    "%.8s vs %.8s",
                    g_ReplayCharacterNames[replay->frameStart[0][i]->character],
                    g_ReplayCharacterNames[replay->frameStart[1][i]->character]);
                position.y += 13.0f;
            }
        }
    }

    g_AsciiManager.color = 0xFFFFFFFFu;
    g_AsciiManager.isSelected = 0;
    return 1;
}
