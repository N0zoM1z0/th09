#include "Midi.hpp"
#include "Supervisor.hpp"

#include <windows.h>
#include <mmsystem.h>

extern int g_WindowActivateWParam;
extern int g_WindowInactive;
extern unsigned int g_WindowRuntimeFlags;
extern MidiOutput *g_WindowMidiOutput;
extern Supervisor g_Supervisor;

LRESULT CALLBACK GameWindowProc(
    HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_ERASEBKGND:
        return 1;

    case 969:
        if (g_WindowMidiOutput != NULL)
            g_WindowMidiOutput->UnprepareHeader(
                reinterpret_cast<LPMIDIHDR>(lParam));
        break;

    case WM_ACTIVATEAPP:
        g_WindowActivateWParam = static_cast<int>(wParam);
        if (g_WindowActivateWParam)
            g_WindowInactive = 0;
        else
            g_WindowInactive = 1;
        break;

    case WM_SETCURSOR:
        if (!g_Supervisor.IsWindowed())
        {
            if (g_WindowInactive)
            {
                SetCursor(LoadCursorA(NULL, IDC_ARROW));
                ShowCursor(TRUE);
            }
            else
            {
                ShowCursor(FALSE);
                SetCursor(NULL);
            }
        }
        else
        {
            SetCursor(LoadCursorA(NULL, IDC_ARROW));
            ShowCursor(TRUE);
        }
        return 1;

    case WM_CLOSE:
        g_WindowRuntimeFlags |= 0x80;
        return 1;
    }

    return DefWindowProcA(hWnd, message, wParam, lParam);
}
