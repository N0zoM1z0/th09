#include "AnmManager.hpp"
#include "GameWindow.hpp"
#include "FileSystem.hpp"
#include "Supervisor.hpp"

#include <d3d8.h>
#include <direct.h>
#include <stdio.h>

#include <stddef.h>

struct AsciiInputView
{
    unsigned short WasPressed(unsigned short buttons);
};
extern AsciiInputView g_AsciiInput;
extern unsigned char g_TitleRegistrationByte;

void ResetRenderState();

void GameWindow::Present()
{
    Supervisor *supervisor = &g_Supervisor;

    char snapshotPath[256];

    if (supervisor->d3dDevice08->Present(NULL, NULL, NULL, NULL) < 0)
    {
        g_AnmManager->ReleaseSurfaces();
        supervisor->d3dDevice08->Reset(&supervisor->presentParameters350);
        ResetRenderState();
        supervisor->screenTransitionCountdown = 2;
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

    if (supervisor->screenTransitionCountdown != 0 &&
        g_TitleRegistrationByte == 0)
    {
        // The original D3D reset/transition countdown is a native
        // i386 DWORD. Keep its wrap without a C++ signed-overflow rule.
        --*reinterpret_cast<unsigned int *>(&supervisor->screenTransitionCountdown);
    }
}
