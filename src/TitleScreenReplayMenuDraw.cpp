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

#include "TitleScreenReplayView.hpp"
#include "TitleReplayNativeSlots.hpp"

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
// The title menu has its own display tables at 0x004A1CF8.
const char *g_TitleReplayCharacterNames[17] =
{
    "Reimu   ",
    "Marisa  ",
    "Sakuya  ",
    "Youmu   ",
    "Reisen  ",
    "Cirno   ",
    "Lyrica",
    "Mystia  ",
    "Tewi    ",
    "Yuka    ",
    "Aya     ",
    "Medicine",
    "Komachi ",
    "Sikieiki",
    "Marlin  ",
    "Lunasa  ",
    "     ",
};

const char *g_TitleReplayDifficultyNames[5] =
{
    "Easy   ",
    "Normal ",
    "Hard   ",
    "Lunatic",
    "Extra  ",
};

const char *g_TitleReplayModeNames[2] =
{
    "Story Mode",
    "Extra Mode",
};

const char *g_TitleReplayMatchNames[4] =
{
    "Human vs Human",
    "Human vs Com",
    "Com vs Human",
    "Com vs Com",
};

// Replay header values address the original name-pointer tables directly.

// Each replay keeps the original three by ten frame-header pointer grid.
// The path is the actual 50 by 0x200 character row owned by TitleScreen.

int TitleScreenView::DrawReplayMenu()
{
    D3DXVECTOR3 position;
    char buffer[256];
    // This one verified 12-byte view is passed to the native ASCII text API.
    Float3 &textPosition = reinterpret_cast<Float3 &>(position);

    if (currentScreenState == 1)
    {
        position.x = 32.0f;
        position.y = 90.0f;
        position.z = 0.0f;

        for (i32 i = 0; i < TitleReplayNativeSlots::ReplayCount; ++i)
        {
            if (i == TitleReplayNativeSlots::RowsPerColumn)
            {
                position.x = 352.0f;
                position.y = 90.0f;
            }

            g_AsciiManager.color =
                (i == keyboardSelection) ? 0xFFFFFFFFu : 0xFF808080u;

            if (i < TitleReplayNativeSlots::RowsPerColumn)
            {
                g_AsciiManager.AddFormatText(
                    &textPosition,
                    "No.%.2d %8s %8s",
                    i + 1,
                    (replays[i].magic ? replays[i].playerName : "--------"),
                    (replays[i].magic ? replays[i].playTime : "--/--/--"));
            }
            else
            {
                g_AsciiManager.AddFormatText(
                    &textPosition, "%c%c%c%c  %8s %8s",
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
        const ReplayDataView &replayValue = replays[selectedReplay];
        i32 selected = selectedReplay;

        position.x = 32.0f;
        if (selected >= TitleReplayNativeSlots::RowsPerColumn)
            position.x = 352.0f;
        position.y = (float)selected * 13.0f + 90.0f;
        position.z = 0.0f;

        D3DXVECTOR3 target(216.5f, 160.0f, 0.0f);
        if (stateTimer2 < 10)
        {
            position = position + (target - position) * (float)stateTimer2 * 0.1f;
        }
        else
            position = target;

        if (selected < TitleReplayNativeSlots::RowsPerColumn)
        {
            g_AsciiManager.AddFormatText(
                &textPosition,
                "No.%.2d %8s %8s",
                static_cast<int>(static_cast<unsigned int>(selected) + 1u),
                (replayValue.magic ? replayValue.playerName : "--------"),
                (replayValue.magic ? replayValue.playTime : "--/--/--"));
        }
        else
        {
            g_AsciiManager.AddFormatText(
                &textPosition, "%c%c%c%c  %8s %8s",
                replayValue.magic ? replayPaths[selected][15] : 'u',
                replayValue.magic ? replayPaths[selected][16] : 's',
                replayValue.magic ? replayPaths[selected][17] : 'e',
                replayValue.magic ? replayPaths[selected][18] : 'r',
                replayValue.magic ? replayValue.playerName : "--------",
                replayValue.magic ? replayValue.playTime : "--/--/--");
        }

        position.x = 221.0f;
        position.y = 240.0f;

        if (replays[selected].value1E4 == 0)
        {
            sprintf(
                buffer, "%s  -%s-",
                g_TitleReplayModeNames[0],
                g_TitleReplayDifficultyNames[replayValue.value0D7]);
            position.x = 320.0f - (float)strlen(buffer) * 4.5f;
            g_AsciiManager.AddFormatText(&textPosition, buffer);
        }
        else if (replays[selected].value1E4 == 1)
        {
            sprintf(buffer, "%s", g_TitleReplayModeNames[1]);
            position.x = 320.0f - (float)strlen(buffer) * 4.5f;
            g_AsciiManager.AddFormatText(&textPosition, buffer);
        }
        else if (replays[selected].value1E4 == 2)
        {
            sprintf(
                buffer, "%s  -%s-",
                g_TitleReplayMatchNames[replayValue.value1E5],
                g_TitleReplayDifficultyNames[replayValue.value0D7]);
            position.x = 320.0f - (float)strlen(buffer) * 4.5f;
            g_AsciiManager.AddFormatText(&textPosition, buffer);
        }

        position.x = 230.0f;
        position.y = 270.0f;

        for (i32 i = 0; i < TitleReplayNativeSlots::StageCount; ++i)
        {
            if (replayValue.frameStart[0][i] != 0)
            {
                g_AsciiManager.color =
                    (i == keyboardSelection) ? 0xFFFFFFFFu : 0xFF808080u;
                g_AsciiManager.AddFormatText(
                    &textPosition,
                    "%.8s vs %.8s",
                    g_TitleReplayCharacterNames[replayValue.frameStart[0][i]->value06],
                    g_TitleReplayCharacterNames[replayValue.frameStart[1][i]->value06]);
                position.y += 13.0f;
            }
        }
    }

    g_AsciiManager.color = 0xFFFFFFFFu;
    g_AsciiManager.isSelected = 0;
    return 1;
}
