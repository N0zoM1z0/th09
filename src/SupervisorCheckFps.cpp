#include "Supervisor.hpp"
#include "AnmManager.hpp"
#include "GameErrorContext.hpp"

#include <windows.h>
#include <mmsystem.h>
#include <d3d8.h>

extern GameErrorContext g_GameErrorContext;
extern const char g_VsyncHighRefreshMessageNoPeriod[] = "\x90\x82\x92\xBC\x93\xAF\x8A\xFA\x82\xAA\x8E\xE6\x82\xEA\x82\xC4\x82\xC8\x82\xA2\x82\xA9\x81\x41\x83\x8A\x83\x74\x83\x8C\x83\x62\x83\x56\x83\x85\x83\x8C\x81\x5B\x83\x67\x82\xAA\x8D\x82\x82\xB7\x82\xAC\x82\xDC\x82\xB7\r\n"; // 垂直同期が取れてないか、リフレッシュレートが高すぎます\r\n
extern const char g_Force60FpsMessage[] = "\x8B\xAD\x90\xA7\x82\x55\x82\x4F\x83\x74\x83\x8C\x81\x5B\x83\x80\x83\x82\x81\x5B\x83\x68\x82\xC5\x93\xAE\x8D\xEC\x82\xB5\x82\xDC\x82\xB7\r\n"; // 強制６０フレームモードで動作します\r\n
extern const char g_VsyncHighRefreshMessage[] = "\x90\x82\x92\xBC\x93\xAF\x8A\xFA\x82\xAA\x8E\xE6\x82\xEA\x82\xC4\x82\xC8\x82\xA2\x82\xA9\x81\x41\x83\x8A\x83\x74\x83\x8C\x83\x62\x83\x56\x83\x85\x83\x8C\x81\x5B\x83\x67\x82\xAA\x8D\x82\x82\xB7\x82\xAC\x82\xDC\x82\xB7\x81\x42\r\n"; // 垂直同期が取れてないか、リフレッシュレートが高すぎます。\r\n

// Frame-time measurements occupy the real thirty-element local float array.
// Keep the original sampleCount postincrement and independent reads.

int SupervisorCheckFps()
{
    int frameIndex;
    int framesInWindow;
    DWORD lastTime;
    float samples[30];
    int sampleCount;
    DWORD currentTime;
    int deltaTime;
    float fps;
    int averageIndex;
    float average;
    Supervisor *supervisor = &g_Supervisor;

    frameIndex = 0;
    framesInWindow = 0;
    sampleCount = 0;

    timeBeginPeriod(1);
    lastTime = timeGetTime();
    timeEndPeriod(1);

    while (frameIndex < 600 && sampleCount < 4)
    {
        supervisor->d3dDevice08->BeginScene();
        g_AnmManager->CopySurfaceToBackbuffer(
            8, 0, 0, 0, 0);
        supervisor->d3dDevice08->EndScene();
        if (supervisor->d3dDevice08->Present(NULL, NULL, NULL, NULL) < 0)
            supervisor->d3dDevice08->Reset(&supervisor->presentParameters350);

        ++frameIndex;
        timeBeginPeriod(1);
        currentTime = timeGetTime();
        timeEndPeriod(1);
        ++framesInWindow;
        // Retail subtracts the original WinMM clock DWORDs modulo 2^32
        // before consuming the signed elapsed-time window.
        deltaTime = static_cast<int>(currentTime - lastTime);

        if (deltaTime >= 700)
        {
            lastTime = currentTime;
            framesInWindow = 0;
        }
        else if (deltaTime >= 500)
        {
            fps = (float)framesInWindow * 1000.0f / deltaTime;
            if (fps >= 57.0f)
                samples[sampleCount++] = fps;
            lastTime = currentTime;
            framesInWindow = 0;
        }
    }

    if (g_Supervisor.IsVsyncDisabled())
        return 0;

    average = 0.0f;
    if (sampleCount >= 2)
    {
        for (averageIndex = 0; averageIndex < sampleCount; averageIndex++)
            average += samples[averageIndex];
        average /= averageIndex;
    }
    else
    {
        average = 1000.0f;
    }

    if (average > 160.0f)
    {
        g_GameErrorContext.Log(g_VsyncHighRefreshMessageNoPeriod);
        g_GameErrorContext.Log(g_Force60FpsMessage);
        supervisor->disableVsync = 1;
        return -2;
    }

    if (average >= 65.0f)
    {
        g_GameErrorContext.Log(g_VsyncHighRefreshMessage);
        g_GameErrorContext.Log(g_Force60FpsMessage);
        supervisor->disableVsync = 1;
        return -2;
    }

    return 0;
}
