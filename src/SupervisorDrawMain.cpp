#include "Supervisor.hpp"
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

struct SupervisorDrawMainAnmManagerView
{
    void ResetDrawState();
    void *GetSurface(int surfaceIndex);
    int CopySurfaceToBackbuffer(int surfaceIndex, int left, int top, int right, int bottom);
    void ReleaseSurface(int surfaceIndex);
};

struct SupervisorDrawMainAsciiManagerView
{
    void SetScale(float scaleX, float scaleY);
    void SetAlpha(int alpha);
    void ResetStrings();
};

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
        layout->loadingState++;
        if (layout->loadingState >= 5)
        {
            Float3 position(288.0f, 454.0f, 0.0f);
            ascii->SetScale(0.5f, 0.5f);

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
            ascii->SetScale(1.0f, 1.0f);
            g_AsciiManager.DrawStrings();
            g_AsciiManager.ResetStrings();

            if (layout->loadingState >= 65)
                layout->loadingState = 5;
        }
    }

    if (layout->loadingState != 0)
    {
        reinterpret_cast<SupervisorDrawMainAnmManagerView *>(g_AnmManager)->CopySurfaceToBackbuffer(
            8, 0, 0, 0, 0);
    }
    else
    {
        SupervisorDrawMainAnmManagerView *anm =
            reinterpret_cast<SupervisorDrawMainAnmManagerView *>(g_AnmManager);
        if (anm->GetSurface(8) != NULL)
            anm->ReleaseSurface(8);
    }

    return 1;
}
