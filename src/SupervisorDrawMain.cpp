#include "Supervisor.hpp"
#include "SupervisorDrawMainViews.hpp"
#include "AnmManager.hpp"
#include "AsciiManager.hpp"

#include <stddef.h>

struct SupervisorDrawMainLayout
{
    unsigned char unknown000[0x740];
    int loadingState;
};

typedef char SupervisorDrawMainStateAt740[
    (offsetof(SupervisorDrawMainLayout, loadingState) == 0x740) ? 1 : -1];

extern AsciiManager g_AsciiManager;

int __fastcall SupervisorDrawMain(Supervisor *supervisor)
{
    SupervisorDrawMainLayout *layout =
        reinterpret_cast<SupervisorDrawMainLayout *>(supervisor);
    SupervisorDrawMainAsciiManagerView *ascii =
        reinterpret_cast<SupervisorDrawMainAsciiManagerView *>(&g_AsciiManager);

    reinterpret_cast<SupervisorDrawMainAnmManagerView *>(g_AnmManager)->ResetDrawState();

    if (layout->loadingState >= 2)
    {
        // Retail advances the signed loading-state word using native i386
        // DWORD INC before the original display/timeout comparisons.
        ++*reinterpret_cast<unsigned int *>(&layout->loadingState);
        if (layout->loadingState >= 5)
        {
            Float3 position(288.0f, 454.0f, 0.0f);
            g_AsciiManager.SetScale(0.5f, 0.5f);

            if (layout->loadingState < 35)
            {
                int color1 = 255 - (((layout->loadingState - 5) << 7) / 30);
                ascii->SetAlpha(color1);
            }
            else
            {
                int color2 = 255 - (((65 - layout->loadingState) << 7) / 30);
                ascii->SetAlpha(color2);
            }

            g_AsciiManager.AddFormatText(&position, "Press Shot Button");
            g_AsciiManager.SetScale(1.0f, 1.0f);
            g_AsciiManager.DrawStrings();
            g_AsciiManager.ResetStrings();

            if (layout->loadingState >= 65)
                layout->loadingState = 5;
        }
    }

    if (layout->loadingState != 0)
    {
        g_AnmManager->CopySurfaceToBackbuffer(8, 0, 0, 0, 0);
    }
    else
    {
        SupervisorDrawMainAnmManagerView *anm =
            reinterpret_cast<SupervisorDrawMainAnmManagerView *>(g_AnmManager);
        if (anm->GetSurface(8) != NULL)
            reinterpret_cast<AnmManager *>(anm)->ReleaseSurface(8);
    }

    return 1;
}
