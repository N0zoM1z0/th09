#include "AnmManager.hpp"
#include "Chain.hpp"
#include "GameWindow.hpp"
#include "Supervisor.hpp"

#include <d3d8.h>
#include <windows.h>

#include <stddef.h>

struct GameWindowRenderSupervisorView
{
    unsigned char unknown000[0x08];
    IDirect3DDevice8 *d3dDevice08;
    unsigned char unknown00C[0x43A - 0x0C];
    unsigned char frameskipConfig43A;
    unsigned char unknown43B[0x794 - 0x43B];
    int fogState794;
};

typedef char GameWindowRenderFrameskipAt43A[
    (offsetof(GameWindowRenderSupervisorView, frameskipConfig43A) == 0x43A) ? 1 : -1];
typedef char GameWindowRenderFogStateAt794[
    (offsetof(GameWindowRenderSupervisorView, fogState794) == 0x794) ? 1 : -1];

struct GameWindowSoundPlayerView
{
    int ProcessQueues();
};
extern GameWindowSoundPlayerView g_SoundPlayer;
extern Chain g_Chain;

struct GameWindowSupervisorThreadView
{
    void ThreadClose();
};

struct GameWindowAnmDrawView
{
    void ResetDrawState();
};

int GameWindow::Render()
{
    GameWindowRenderSupervisorView *supervisor =
        reinterpret_cast<GameWindowRenderSupervisorView *>(&g_Supervisor);

    this->currentTimestamp2C = this->GetTimestamp();
    if (this->lastTimestamp34 > this->currentTimestamp2C)
        this->lastFrameTime3C = this->currentTimestamp2C;
    this->lastTimestamp34 = this->currentTimestamp2C;

    if (this->lastFrameTime3C < this->currentTimestamp2C)
    {
        while (this->lastFrameTime3C < this->currentTimestamp2C)
            this->lastFrameTime3C += 1.0 / 60.0;

        g_AnmManager->FlushVertexBuffer();
        g_Supervisor.ConfigureScreenViewport(2);

        int calcResult = g_Chain.RunCalcChain();
        g_SoundPlayer.ProcessQueues();

        if (calcResult == 0)
        {
            reinterpret_cast<GameWindowSupervisorThreadView *>(
                &g_Supervisor)->ThreadClose();
            return 1;
        }
        if (calcResult == -1)
        {
            reinterpret_cast<GameWindowSupervisorThreadView *>(
                &g_Supervisor)->ThreadClose();
            return 2;
        }

        ++this->framesSinceRedraw10;
        if (supervisor->frameskipConfig43A < this->framesSinceRedraw10)
        {
            supervisor->d3dDevice08->BeginScene();
            reinterpret_cast<GameWindowAnmDrawView *>(
                g_AnmManager)->ResetDrawState();
            supervisor->fogState794 = 255;
            g_Supervisor.DisableFog();
            g_Chain.RunDrawChain();
            g_AnmManager->FlushVertexBuffer();
            supervisor->d3dDevice08->SetTexture(0, NULL);
            supervisor->d3dDevice08->EndScene();
            this->framesSinceRedraw10 = 0;
        }

        this->currentTimestamp2C = this->GetTimestamp();
        GameWindow::Present();
    }
    else
    {
        Sleep(0);
    }

    return 0;
}
