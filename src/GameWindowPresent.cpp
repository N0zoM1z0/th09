#include "AnmManager.hpp"
#include "GameWindow.hpp"
#include "FileSystem.hpp"
#include "Supervisor.hpp"

#include <d3d8.h>
#include <direct.h>
#include <stdio.h>

#include <stddef.h>

struct GameWindowPresentSupervisorView
{
    unsigned char unknown000[0x08];
    IDirect3DDevice8 *d3dDevice08;
    unsigned char unknown00C[0x350 - 0x0C];
    D3DPRESENT_PARAMETERS presentParameters350;
    unsigned char unknown384[0x5A4 - 0x384];
    int screenTransitionCountdown5A4;
    unsigned char unknown5A8[0x5D4 - 0x5A8];
    unsigned int flags5D4;
};

typedef char GameWindowPresentDeviceAt08[
    (offsetof(GameWindowPresentSupervisorView, d3dDevice08) == 0x08) ? 1 : -1];
typedef char GameWindowPresentParamsAt350[
    (offsetof(GameWindowPresentSupervisorView, presentParameters350) == 0x350) ? 1 : -1];
typedef char GameWindowPresentCountdownAt5A4[
    (offsetof(GameWindowPresentSupervisorView, screenTransitionCountdown5A4) == 0x5A4) ? 1 : -1];
typedef char GameWindowPresentFlagsAt5D4[
    (offsetof(GameWindowPresentSupervisorView, flags5D4) == 0x5D4) ? 1 : -1];

struct AsciiInputView
{
    unsigned short WasPressed(unsigned short buttons);
};
extern AsciiInputView g_AsciiInput;
extern unsigned char g_TitleRegistrationByte;

void ResetRenderState();

void GameWindow::Present()
{
    GameWindowPresentSupervisorView *supervisor =
        reinterpret_cast<GameWindowPresentSupervisorView *>(&g_Supervisor);

    char snapshotPath[256];

    if (supervisor->d3dDevice08->Present(NULL, NULL, NULL, NULL) < 0)
    {
        g_AnmManager->ReleaseSurfaces();
        supervisor->d3dDevice08->Reset(&supervisor->presentParameters350);
        ResetRenderState();
        supervisor->screenTransitionCountdown5A4 = 2;
    }

    g_AnmManager->ServiceCaptureRequests();

    if (g_AsciiInput.WasPressed(0x800) != 0 &&
        (supervisor->flags5D4 & 0x400) == 0)
    {
        _mkdir("snapshot");

        int i;
        for (i = 0; i < 1000; ++i)
        {
            sprintf(snapshotPath, "snapshot/th%.3d.bmp", i);
            if (!FileSystem::CheckIfFileAlreadyExists(snapshotPath))
                break;
        }

        if (i < 1000)
            g_Supervisor.TakeSnapshot(snapshotPath);
    }

    if (supervisor->screenTransitionCountdown5A4 != 0 &&
        g_TitleRegistrationByte == 0)
    {
        --supervisor->screenTransitionCountdown5A4;
    }
}
