#pragma once

#include "GameConfiguration.hpp"

#include <stddef.h>

struct SupervisorCtorArrayElement
{
    unsigned char unknown000[0xF0];

    SupervisorCtorArrayElement();
};

typedef char SupervisorCtorArrayElementSizeCheck[
    (sizeof(SupervisorCtorArrayElement) == 0xF0) ? 1 : -1];

class Supervisor
{
  public:
    Supervisor();
    static int OnUpdate(Supervisor *supervisor);
    void TickTimer(int *current, float *subFrame);
    int TakeSnapshot(const char *filePath);
    int LoadConfig(char *configFile);
    int IsFogDisabled();
    int Is16BitTexturesForced();
    int IsWindowed();
    int IsReferenceRasterizerForced();
    int IsDirectInputDisabled();
    int IsMusicPreloaded();
    int IsVsyncDisabled();
    int IsTextBackgroundDetectionDisabled();
    void ConfigureGameplayViewport(int index);
    void EnterCriticalSectionWrapper(int id);
    void LeaveCriticalSectionWrapper(int id);

    unsigned char unknown000[0x78];
    SupervisorCtorArrayElement ctorArray078[3];
    unsigned char unknown348[0x40];
    GameConfiguration config388;
    unsigned char unknown454[0x180];
    unsigned int flags5D4;
    unsigned char unknown5D8[0x1D8];
};

typedef char SupervisorCtorArrayAt078[
    (offsetof(Supervisor, ctorArray078) == 0x78) ? 1 : -1];
typedef char SupervisorConfigAt388[
    (offsetof(Supervisor, config388) == 0x388) ? 1 : -1];
typedef char SupervisorFlagsAt5D4[
    (offsetof(Supervisor, flags5D4) == 0x5D4) ? 1 : -1];
typedef char SupervisorSizeIs7B0[(sizeof(Supervisor) == 0x7B0) ? 1 : -1];

extern Supervisor g_Supervisor;
