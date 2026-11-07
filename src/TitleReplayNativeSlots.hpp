#pragma once
#include "TitleScreenReplayView.hpp"
#include <stddef.h>

// Both native TitleScreen replay renderers consume the same 50 original
// replay rows and paths, split between two 25-row on-screen columns.
struct TitleReplayNativeSlots
{
    enum
    {
        ReplayCount = sizeof(((TitleScreenView *)0)->replays) /
                      sizeof(((TitleScreenView *)0)->replays[0]),
        PathCount = sizeof(((TitleScreenView *)0)->replayPaths) /
                    sizeof(((TitleScreenView *)0)->replayPaths[0]),
        ColumnCount = 2,
        RowsPerColumn = ReplayCount / ColumnCount,
        StageCount = sizeof(((ReplayDataView *)0)->frameStart[0]) /
                     sizeof(((ReplayDataView *)0)->frameStart[0][0])
    };
};
typedef char TitleReplayNativeSlotDimensions[
    (TitleReplayNativeSlots::ReplayCount == 50 &&
     TitleReplayNativeSlots::PathCount == 50 &&
     TitleReplayNativeSlots::ColumnCount == 2 &&
     TitleReplayNativeSlots::RowsPerColumn == 25 &&
     TitleReplayNativeSlots::StageCount == 10 &&
     offsetof(TitleScreenView, replays) == 0x68E4 &&
     sizeof(((TitleScreenView *)0)->replays) == 0x6018 &&
     offsetof(TitleScreenView, currentReplay) ==
         offsetof(TitleScreenView, replays) +
         sizeof(((TitleScreenView *)0)->replays) &&
     offsetof(TitleScreenView, selectedReplay) == 0xC908 &&
     offsetof(TitleScreenView, replayPaths) == 0x28C &&
     sizeof(ReplayDataView) == 0x1EC) ? 1 : -1];
