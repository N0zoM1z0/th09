#pragma once

#include "SupervisorThreadStartView.hpp"
#include <stddef.h>

struct TitleFloat3View;

// Title callbacks borrow the complete native Supervisor allocation; only
// the separately original-bound Title method identities remain in this
// zero-extra-storage source-facing class.  Its anonymous status/FPs window
// is owned by Supervisor at +0x77C..+0x793, not duplicated here.
struct TitleSupervisorView : SupervisorThreadStartView {
    int ClearRecordingFpsWarningState();
    void SetupLoadingVms(TitleFloat3View *position);
    void SetupLoadingVmsAndInitCapture(TitleFloat3View *position);
    void HideLoadingVms();
};

typedef char TitleSupervisorUsesNativeStorage[
    (sizeof(TitleSupervisorView) == sizeof(Supervisor) &&
     offsetof(TitleSupervisorView, d3dDevice08) == 0x08 &&
     offsetof(TitleSupervisorView, value594) == 0x594 &&
     offsetof(TitleSupervisorView, framerateMultiplier5B8) == 0x5B8 &&
     offsetof(TitleSupervisorView, textAnm5CC) == 0x5CC &&
     offsetof(TitleSupervisorView, runningSubthreadHandle) == 0x6AC &&
     offsetof(TitleSupervisorView, subthreadCloseRequestActive) == 0x6B4 &&
     offsetof(TitleSupervisorView, subthreadActive) == 0x6B8 &&
     offsetof(TitleSupervisorView, loadingVmsHaveBeenSetup) == 0x740 &&
     offsetof(TitleSupervisorView, recordingFpsWarning77C) == 0x77C &&
     offsetof(TitleSupervisorView, playbackFpsWarning780) == 0x780 &&
     offsetof(TitleSupervisorView, recordingFpsState790) == 0x790 &&
     offsetof(TitleSupervisorView, totalPlayTime) == 0x798) ? 1 : -1];
