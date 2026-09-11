#pragma once

class Supervisor
{
  public:
    void TickTimer(int *current, float *subFrame);
    void ConfigureGameplayViewport(int index);
    void EnterCriticalSectionWrapper(int id);
    void LeaveCriticalSectionWrapper(int id);
};

extern Supervisor g_Supervisor;
