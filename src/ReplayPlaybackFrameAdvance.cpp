// Original replay Chain control callback. Return 6 restarts the calc chain.
#include "ReplayManagerRuntimeView.hpp"
#include "AsciiGameManagerView.hpp"

struct PlayerSharedRuntimeView
{
    int IsBlocked();
};
extern PlayerSharedRuntimeView *g_PlayerSharedRuntime;

int __fastcall ReplayManagerView::ControlPlaybackFrameAdvance(ReplayManagerView *replayManager)
{
    if ((g_GameManager.flags & 4) != 0 &&
        (g_GameManager.flags & 0x200) == 0 &&
        g_PlayerSharedRuntime->IsBlocked())
    {
        if (replayManager->frameCounter % 3 != 2)
            return 6;
    }
    return 1;
}
