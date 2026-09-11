#pragma once

class Supervisor
{
  public:
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
};

extern Supervisor g_Supervisor;
