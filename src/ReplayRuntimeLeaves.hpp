#pragma once

#include <stddef.h>

#include "RngRuntimeLeaves.hpp"

struct SupervisorReplayView
{
    int IsSpeedhackDetected();

    unsigned char unknown000[0x5C8];
    short recordedFps;
    unsigned char unknown5CA[0x0A];
    unsigned int flags5D4;
    unsigned char unknown5D8[0x780 - 0x5D8];
    int playbackFpsWarning;
    unsigned char unknown784[0x7A0 - 0x784];
    unsigned int exeChecksum7A0;
    unsigned int exeSize7A4;
};

typedef char SupervisorReplayFlagsAt5D4[
    (offsetof(SupervisorReplayView, flags5D4) == 0x5D4) ? 1 : -1];
typedef char SupervisorReplayFpsAt5C8[
    (offsetof(SupervisorReplayView, recordedFps) == 0x5C8) ? 1 : -1];
typedef char SupervisorPlaybackWarningAt780[
    (offsetof(SupervisorReplayView, playbackFpsWarning) == 0x780 &&
     offsetof(SupervisorReplayView, exeChecksum7A0) == 0x7A0 &&
     offsetof(SupervisorReplayView, exeSize7A4) == 0x7A4) ? 1 : -1];
