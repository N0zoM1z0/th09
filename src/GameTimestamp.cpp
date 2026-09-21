#include <windows.h>
#include <mmsystem.h>

extern unsigned int g_PerformanceFrequency;

double GetGameTimestampSeconds()
{
    if (g_PerformanceFrequency != 0)
    {
        LARGE_INTEGER performanceCounter;
        QueryPerformanceCounter(&performanceCounter);
        return (double)(unsigned int)performanceCounter.LowPart /
               (double)g_PerformanceFrequency;
    }

    timeBeginPeriod(1);
    double timestamp = (float)timeGetTime() * 0.001f;
    timeEndPeriod(1);
    return timestamp;
}
