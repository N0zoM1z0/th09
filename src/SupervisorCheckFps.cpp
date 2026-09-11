#include "Supervisor.hpp"
#include "AnmManager.hpp"
#include "GameErrorContext.hpp"

#include <windows.h>
#include <mmsystem.h>
#include <d3d8.h>

struct SupervisorCheckFpsAnmManagerView
{
    int CopySurfaceToBackbuffer(int surfaceIndex, int left, int top, int right, int bottom);
};

struct SupervisorCheckFpsLayout
{
    unsigned char unknown000[0x08];
    IDirect3DDevice8 *d3dDevice;
    unsigned char unknown00C[0x350 - 0x0C];
    D3DPRESENT_PARAMETERS presentParameters;
    unsigned char unknownAfterPresent[0x5AC - 0x350 - sizeof(D3DPRESENT_PARAMETERS)];
    int disableVsync;
};

typedef char SupervisorCheckFpsD3DAt08[
    (offsetof(SupervisorCheckFpsLayout, d3dDevice) == 0x08) ? 1 : -1];
typedef char SupervisorCheckFpsPresentAt350[
    (offsetof(SupervisorCheckFpsLayout, presentParameters) == 0x350) ? 1 : -1];
typedef char SupervisorCheckFpsDisableVsyncAt5AC[
    (offsetof(SupervisorCheckFpsLayout, disableVsync) == 0x5AC) ? 1 : -1];

extern GameErrorContext g_GameErrorContext;
extern const char g_VsyncHighRefreshMessageNoPeriod[];
extern const char g_Force60FpsMessage[];
extern const char g_VsyncHighRefreshMessage[];

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
    SupervisorCheckFpsLayout *supervisor =
        reinterpret_cast<SupervisorCheckFpsLayout *>(&g_Supervisor);

    frameIndex = 0;
    framesInWindow = 0;
    sampleCount = 0;

    timeBeginPeriod(1);
    lastTime = timeGetTime();
    timeEndPeriod(1);

    while (frameIndex < 600 && sampleCount < 4)
    {
        supervisor->d3dDevice->BeginScene();
        reinterpret_cast<SupervisorCheckFpsAnmManagerView *>(g_AnmManager)->CopySurfaceToBackbuffer(
            8, 0, 0, 0, 0);
        supervisor->d3dDevice->EndScene();
        if (supervisor->d3dDevice->Present(NULL, NULL, NULL, NULL) < 0)
            supervisor->d3dDevice->Reset(&supervisor->presentParameters);

        frameIndex++;
        timeBeginPeriod(1);
        currentTime = timeGetTime();
        timeEndPeriod(1);
        framesInWindow++;
        deltaTime = currentTime - lastTime;

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
