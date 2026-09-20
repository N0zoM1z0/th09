struct ReplayStageDispatchView
{
    static int __fastcall BeginRecordingStage(ReplayStageDispatchView *manager);
    static int __fastcall BeginPlaybackStage(ReplayStageDispatchView *manager);
};

void __fastcall DispatchReplayStage(
    ReplayStageDispatchView *manager, int replayMode)
{
    switch (replayMode)
    {
    case 0:
        ReplayStageDispatchView::BeginRecordingStage(manager);
        break;
    case 1:
        ReplayStageDispatchView::BeginPlaybackStage(manager);
        break;
    }
}
