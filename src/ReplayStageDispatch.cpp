#include "ReplayManagerRuntimeView.hpp"

void __fastcall DispatchReplayStage(
    ReplayManagerView *manager, int replayMode)
{
    switch (replayMode)
    {
    case 0:
        ReplayManagerView::BeginRecordingStage(manager);
        break;
    case 1:
        ReplayManagerView::BeginPlaybackStage(manager);
        break;
    }
}
