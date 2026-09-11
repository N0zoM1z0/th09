#include "Supervisor.hpp"
#include "AsciiManager.hpp"

#include <windows.h>
#include <mmsystem.h>
#include <stddef.h>
#include <stdio.h>

struct SupervisorCalculateFpsLayout
{
    unsigned char unknown000[0x43A];
    unsigned char frameskipConfig;
    unsigned char unknown43B[0x5A8 - 0x43B];
    int suppressFpsDisplay;
    unsigned char unknown5AC[0x5C0 - 0x5AC];
    float lagNumerator;
    float lagDenominator;
    short recordedFps;
    unsigned char unknown5CA[0x744 - 0x5CA];
    unsigned int fpsPerformanceFrequency;
};

typedef char SupervisorCalculateFrameskipAt43A[
    (offsetof(SupervisorCalculateFpsLayout, frameskipConfig) == 0x43A) ? 1 : -1];
typedef char SupervisorCalculateSuppressAt5A8[
    (offsetof(SupervisorCalculateFpsLayout, suppressFpsDisplay) == 0x5A8) ? 1 : -1];
typedef char SupervisorCalculateLagNumeratorAt5C0[
    (offsetof(SupervisorCalculateFpsLayout, lagNumerator) == 0x5C0) ? 1 : -1];
typedef char SupervisorCalculateLagDenominatorAt5C4[
    (offsetof(SupervisorCalculateFpsLayout, lagDenominator) == 0x5C4) ? 1 : -1];
typedef char SupervisorCalculateRecordedFpsAt5C8[
    (offsetof(SupervisorCalculateFpsLayout, recordedFps) == 0x5C8) ? 1 : -1];
typedef char SupervisorCalculateFrequencyAt744[
    (offsetof(SupervisorCalculateFpsLayout, fpsPerformanceFrequency) == 0x744) ? 1 : -1];

struct SupervisorCalculateGameManagerView
{
    unsigned char unknown000[0x134];
    unsigned int flags;

    int IsReplayNeutral();
};

extern SupervisorCalculateGameManagerView g_GameManager;
extern unsigned int g_FpsQpcSampleCount;
extern LARGE_INTEGER g_FpsLastQpc;
extern unsigned int g_FpsLastTime;
extern unsigned int g_FpsTimeInitialized;
extern unsigned int g_FpsFrameCount;
extern char g_FpsBuffer[];
extern char g_FpsDebugBuffer[];
extern AsciiManager g_AsciiManager;

void __fastcall SupervisorCalculateFps(int shouldDraw)
{
    float fps;
    float elapsed;
    DWORD currentTime;
    float framerate;
    SupervisorCalculateFpsLayout *supervisor =
        reinterpret_cast<SupervisorCalculateFpsLayout *>(&g_Supervisor);

    g_FpsFrameCount += (unsigned char)supervisor->frameskipConfig + 1;

    if (supervisor->fpsPerformanceFrequency == 0)
    {
        if ((g_FpsTimeInitialized & 1) == 0)
        {
            g_FpsTimeInitialized |= 1;
            g_FpsLastTime = timeGetTime();
        }

        currentTime = timeGetTime();
        if (currentTime < g_FpsLastTime)
        {
            g_FpsLastTime = currentTime;
            g_FpsFrameCount = 0;
        }

        if (currentTime - g_FpsLastTime >= 500)
        {
            elapsed = (float)(currentTime - g_FpsLastTime) * 0.001f;
            g_FpsLastTime = currentTime;
calculateFps:
            fps = (float)g_FpsFrameCount / elapsed;
            g_FpsFrameCount = 0;
            sprintf(g_FpsBuffer, "%.02ffps", fps);

            if ((g_GameManager.flags & 4) != 0 && shouldDraw)
            {
                framerate = 60.0f;
                supervisor->lagDenominator += framerate;

                if (fps > framerate * 0.89999998f)
                    supervisor->lagNumerator += framerate;
                else if (fps > framerate * 0.69999999f)
                    supervisor->lagNumerator += framerate * 0.80000001f;
                else if (fps > framerate * 0.5f)
                    supervisor->lagNumerator += framerate * 0.60000002f;
                else
                    supervisor->lagNumerator += framerate * 0.5f;

                if (!g_GameManager.IsReplayNeutral())
                    supervisor->recordedFps = (short)(fps + 0.5f);
                else
                    sprintf(g_FpsDebugBuffer, "%2d", supervisor->recordedFps);
            }
        }
    }
    else
    {
        LARGE_INTEGER currentQpc;

        if (g_FpsLastQpc.LowPart == 0)
            QueryPerformanceCounter(&g_FpsLastQpc);

        QueryPerformanceCounter(&currentQpc);
        if (currentQpc.LowPart < g_FpsLastQpc.LowPart)
        {
            g_FpsLastQpc = currentQpc;
            g_FpsFrameCount = 0;
        }

        if (currentQpc.LowPart >=
            g_FpsLastQpc.LowPart + (supervisor->fpsPerformanceFrequency >> 1))
        {
            elapsed = (float)(currentQpc.LowPart - g_FpsLastQpc.LowPart) /
                      (float)supervisor->fpsPerformanceFrequency;
            g_FpsLastQpc = currentQpc;
            g_FpsQpcSampleCount++;
            goto calculateFps;
        }
    }

    if (!supervisor->suppressFpsDisplay && shouldDraw && (g_GameManager.flags & 4) != 0)
    {
        Float3 fpsCounterPos(280.0f, 466.0f, 0.0f);
        g_AsciiManager.AddString(&fpsCounterPos, g_FpsBuffer);
    }
}
