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
extern const char *g_ReplayDifficultyNames[];
extern const char *g_ReplayModeNames[];
extern const char *g_ReplayCharacterNames[];
extern const char *g_ReplayStoryModeName[];
extern const char *g_ReplayExtraModeName[];

// The original 50-entry Title replay arrays use genuine 32-bit native
// record and path byte strides for the signed selected-replay word.
static __forceinline const ReplayDataView &NativeMenuReplayRecord(
    const ReplayDataView *records, int index)
{
    return *reinterpret_cast<const ReplayDataView *>(
        reinterpret_cast<unsigned int>(records) +
        static_cast<unsigned int>(index) * sizeof(ReplayDataView));
}
static __forceinline const char *NativeMenuReplayPath(
    const char *bytes, int index)
{
    return reinterpret_cast<const char *>(
        reinterpret_cast<unsigned int>(bytes) +
        static_cast<unsigned int>(index) * sizeof(((TitleScreenView *)0)->replayPaths[0]));
}

// Replay header wire values index the original 32-bit pointer table as
// base + unsigned(index)*4, without a C++ array bounds precondition.
static inline const char *NativeReplayTextName(const char **table, int index)
{
    return *reinterpret_cast<const char * const *>(
        reinterpret_cast<unsigned int>(&table[0]) +
        static_cast<unsigned int>(index) * sizeof(table[0]));
}

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
        const ReplayDataView &replayValue = NativeMenuReplayRecord(&replays[0], selectedReplay);
        i32 selected = selectedReplay;

        position.x = selected < TitleReplayNativeSlots::RowsPerColumn ? 32.0f : 352.0f;
        position.y = (float)selected * 13.0f + 90.0f;
        position.z = 0.0f;

        D3DXVECTOR3 target(216.5f, 160.0f, 0.0f);
        i32 frame = stateTimer2;
        if (frame < 10)
        {
            D3DXVECTOR3 displacement = target - position;
            position = position + displacement * (float)frame * 0.1f;
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
                replayValue.magic ? NativeMenuReplayPath(&replayPaths[0][0], selected)[15] : 'u',
                replayValue.magic ? NativeMenuReplayPath(&replayPaths[0][0], selected)[16] : 's',
                replayValue.magic ? NativeMenuReplayPath(&replayPaths[0][0], selected)[17] : 'e',
                replayValue.magic ? NativeMenuReplayPath(&replayPaths[0][0], selected)[18] : 'r',
                replayValue.magic ? replayValue.playerName : "--------",
                replayValue.magic ? replayValue.playTime : "--/--/--");
        }

        position.x = 221.0f;
        position.y = 240.0f;

        if (NativeMenuReplayRecord(&replays[0], selected).value1E4 == 0)
        {
            sprintf(
                buffer, "%s  -%s-",
                g_ReplayStoryModeName[0],
                NativeReplayTextName(g_ReplayDifficultyNames, replayValue.value0D7));
            position.x = 320.0f - (float)strlen(buffer) * 4.5f;
            g_AsciiManager.AddFormatText(&textPosition, buffer);
        }
        else if (NativeMenuReplayRecord(&replays[0], selected).value1E4 == 1)
        {
            sprintf(buffer, "%s", g_ReplayExtraModeName[0]);
            position.x = 320.0f - (float)strlen(buffer) * 4.5f;
            g_AsciiManager.AddFormatText(&textPosition, buffer);
        }
        else if (NativeMenuReplayRecord(&replays[0], selected).value1E4 == 2)
        {
            sprintf(
                buffer, "%s  -%s-",
                NativeReplayTextName(g_ReplayModeNames, replayValue.value1E5),
                NativeReplayTextName(g_ReplayDifficultyNames, replayValue.value0D7));
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
                    NativeReplayTextName(g_ReplayCharacterNames, replayValue.frameStart[0][i]->value06),
                    NativeReplayTextName(g_ReplayCharacterNames, replayValue.frameStart[1][i]->value06));
                position.y += 13.0f;
            }
        }
    }

    g_AsciiManager.color = 0xFFFFFFFFu;
    g_AsciiManager.isSelected = 0;
    return 1;
}
