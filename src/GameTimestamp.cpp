#include "GameWindow.hpp"

#include <mmsystem.h>

double GameWindow::GetTimestamp()
{
    if (g_GameWindow.performanceFrequency14.LowPart != 0)
    {
        LARGE_INTEGER performanceCounter;
        QueryPerformanceCounter(&performanceCounter);
        return (double)(unsigned int)performanceCounter.LowPart /
               (double)g_GameWindow.performanceFrequency14.LowPart;
    }

    timeBeginPeriod(1);
    double timestamp = (float)timeGetTime() * 0.001f;
    timeEndPeriod(1);
    return timestamp;
}
