// SPDX-License-Identifier: GPL-3.0-only
// Native SDK vector/formatting source adapted from Cirno-Labs/th09 d7653152.
// Original MIT portions retain their grant; see THIRD_PARTY_NOTICES.md.
#include <d3dx8math.h>
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

// The SDK vector crosses the established Float3 text API at a checked layout.
typedef char ReplayMenuVectorLayoutMatchesFloat3[
    (sizeof(D3DXVECTOR3) == sizeof(Float3) &&
     offsetof(D3DXVECTOR3, x) == offsetof(Float3, x) &&
     offsetof(D3DXVECTOR3, y) == offsetof(Float3, y) &&
     offsetof(D3DXVECTOR3, z) == offsetof(Float3, z)) ? 1 : -1];

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

int TitleScreenView::DrawReplayMenu()
{
    D3DXVECTOR3 position;
    char buffer[256];

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

            if (i < 25)
            {
                g_AsciiManager.AddFormatText(
                    reinterpret_cast<Float3 *>(&position),
                    "No.%.2d %8s %8s",
                    i + 1,
                    (replays[i].magic ? replays[i].playerName : "--------"),
                    (replays[i].magic ? replays[i].playTime : "--/--/--"));
            }
            else
            {
                g_AsciiManager.AddFormatText(
                    reinterpret_cast<Float3 *>(&position), "%c%c%c%c  %8s %8s",
                    replays[i].magic ? replayPaths[i][15] : 'u',
                    replays[i].magic ? replayPaths[i][16] : 's',
                    replays[i].magic ? replayPaths[i][17] : 'e',
                    replays[i].magic ? replayPaths[i][18] : 'r',
                    replays[i].magic ? replays[i].playerName : "--------",
                    replays[i].magic ? replays[i].playTime : "--/--/--");
            }
            position.y += 13.0f;
        }
    }
    else if (currentScreenState == 2)
    {
        // Acquire the record before the selected-index snapshot.
        const ReplayDataView *replay = &replays[selectedReplay];
        i32 selected = selectedReplay;

        position.x = selected < 25 ? 32.0f : 352.0f;
        position.y = (float)selected * 13.0f + 90.0f;
        position.z = 0.0f;

        D3DXVECTOR3 target(216.5f, 160.0f, 0.0f);
        i32 frame = stateTimer2;
        if (frame < 10)
            position = position + (target - position) * (float)frame * 0.1f;
        else
            position = target;

        if (selected < 25)
        {
            g_AsciiManager.AddFormatText(
                reinterpret_cast<Float3 *>(&position),
                "No.%.2d %8s %8s",
                selected + 1,
                (replay->magic ? replay->playerName : "--------"),
                (replay->magic ? replay->playTime : "--/--/--"));
        }
        else
        {
            g_AsciiManager.AddFormatText(
                reinterpret_cast<Float3 *>(&position), "%c%c%c%c  %8s %8s",
                replay->magic ? replayPaths[selected][15] : 'u',
                replay->magic ? replayPaths[selected][16] : 's',
                replay->magic ? replayPaths[selected][17] : 'e',
                replay->magic ? replayPaths[selected][18] : 'r',
                replay->magic ? replay->playerName : "--------",
                replay->magic ? replay->playTime : "--/--/--");
        }

        position.x = 221.0f;
        position.y = 240.0f;

        if (replays[selected].value1E4 == 0)
        {
            sprintf(
                buffer, "%s  -%s-",
                g_ReplayStoryModeName[0],
                g_ReplayDifficultyNames[replay->value0D7]);
            position.x = 320.0f - (float)strlen(buffer) * 4.5f;
            g_AsciiManager.AddFormatText(reinterpret_cast<Float3 *>(&position), buffer);
        }
        else if (replays[selected].value1E4 == 1)
        {
            sprintf(buffer, "%s", g_ReplayExtraModeName[0]);
            position.x = 320.0f - (float)strlen(buffer) * 4.5f;
            g_AsciiManager.AddFormatText(reinterpret_cast<Float3 *>(&position), buffer);
        }
        else if (replays[selected].value1E4 == 2)
        {
            sprintf(
                buffer, "%s  -%s-",
                g_ReplayModeNames[replay->value1E5],
                g_ReplayDifficultyNames[replay->value0D7]);
            position.x = 320.0f - (float)strlen(buffer) * 4.5f;
            g_AsciiManager.AddFormatText(reinterpret_cast<Float3 *>(&position), buffer);
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
                    reinterpret_cast<Float3 *>(&position),
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
