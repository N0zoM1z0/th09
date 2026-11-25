#include "GameWindow.hpp"
#include "Midi.hpp"
#include "Supervisor.hpp"

#include <windows.h>
#include <mmsystem.h>

extern Supervisor g_Supervisor;

LRESULT CALLBACK GameWindowProc(
    HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_ERASEBKGND:
        return 1;

    case 969:
        if (g_Supervisor.midiOutput5BC != NULL)
            g_Supervisor.midiOutput5BC->UnprepareHeader(
                reinterpret_cast<LPMIDIHDR>(lParam));
        break;

    case WM_ACTIVATEAPP:
        g_GameWindow.isActive08 = static_cast<int>(wParam);
        if (g_GameWindow.isActive08)
            g_GameWindow.isInactive0C = 0;
        else
            g_GameWindow.isInactive0C = 1;
        break;

    case WM_SETCURSOR:
        if (!g_Supervisor.IsWindowed())
        {
            if (g_GameWindow.isInactive0C)
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
        g_Supervisor.flags |= 0x80;
        return 1;
    }

    return DefWindowProcA(hWnd, message, wParam, lParam);
}
