#include "AsciiManager.hpp"

#include <stddef.h>
#include <stdio.h>
#include <string.h>

typedef unsigned char u8;
typedef unsigned int u32;
typedef int i32;

#include "ReplayDataOriginalView.hpp"

struct TitleScreenView
{
    i32 keyboardSelection;                   // +0x00000
    u8 unknown00004[0x24];
    i32 currentScreenState;                  // +0x00028
    u8 unknown0002C[0x28C - 0x2C];
    char replayPaths[50][0x200];             // +0x0028C
    u8 unknown0668C[0x68E4 - 0x668C];
    ReplayDataView replays[50];                 // +0x068E4
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

static __inline const char *ReplayMenuPlayTime(ReplayDataView *replay)
{
    return replay->magic ? replay->playTime : "--/--/--";
}

static __inline const char *ReplayMenuPlayerName(ReplayDataView *replay)
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

            ReplayDataView *replay = &replays[i];
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
        ReplayDataView *replay = &replays[selected];
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

        if (replays[selected].value1E4 == 0)
        {
            char buffer[256];
            sprintf(
                buffer, "%s  -%s-",
                g_ReplayStoryModeName[0],
                g_ReplayDifficultyNames[replay->value0D7]);
            position.x = 320.0f - (float)strlen(buffer) * 4.5f;
            g_AsciiManager.AddFormatText(&position, buffer);
        }
        else if (replays[selected].value1E4 == 1)
        {
            char buffer[256];
            sprintf(buffer, "%s", g_ReplayExtraModeName[0]);
            position.x = 320.0f - (float)strlen(buffer) * 4.5f;
            g_AsciiManager.AddFormatText(&position, buffer);
        }
        else if (replays[selected].value1E4 == 2)
        {
            char buffer[256];
            sprintf(
                buffer, "%s  -%s-",
                g_ReplayModeNames[replay->value1E5],
                g_ReplayDifficultyNames[replay->value0D7]);
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
                    g_ReplayCharacterNames[replay->frameStart[0][i]->value06],
                    g_ReplayCharacterNames[replay->frameStart[1][i]->value06]);
                position.y += 13.0f;
            }
        }
    }

    g_AsciiManager.color = 0xFFFFFFFFu;
    g_AsciiManager.isSelected = 0;
    return 1;
}
