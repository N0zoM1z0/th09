#pragma once

#include <stddef.h>

#include "RngRuntimeLeaves.hpp"

struct SupervisorReplayView
{
    int IsSpeedhackDetected();
    int CheckVersion(char *versionString, int exeSize, int exeChecksum);

    unsigned char unknown000[0x5C8];
    short recordedFps;
    unsigned char unknown5CA[0x0A];
    unsigned int flags5D4;
    unsigned char unknown5D8[0x780 - 0x5D8];
    int playbackFpsWarning;
};

typedef char SupervisorReplayFlagsAt5D4[
    (offsetof(SupervisorReplayView, flags5D4) == 0x5D4) ? 1 : -1];
typedef char SupervisorReplayFpsAt5C8[
    (offsetof(SupervisorReplayView, recordedFps) == 0x5C8) ? 1 : -1];
typedef char SupervisorPlaybackWarningAt780[
    (offsetof(SupervisorReplayView, playbackFpsWarning) == 0x780) ? 1 : -1];
