#pragma once

class Supervisor
{
  public:
    void EnterCriticalSectionWrapper(int id);
    void LeaveCriticalSectionWrapper(int id);
};

extern Supervisor g_Supervisor;
