#include "Supervisor.hpp"

#include <windows.h>
#include <string.h>

extern Supervisor g_Supervisor;
extern int g_WindowActivateWParam;
extern int g_WindowInactive;
extern HWND g_GameWindowWindow;
extern HWND g_SupervisorGameWindow;
extern const char g_WindowTitle[];

LRESULT CALLBACK GameWindowProc(
    HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
void __stdcall ActivateGameWindow(HWND hWnd);

int CreateGameWindow(HINSTANCE instance)
{
    WNDCLASSA baseClass;
    int width;
    int height;

    ZeroMemory(&baseClass, sizeof(baseClass));
    baseClass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    baseClass.hCursor = LoadCursorA(NULL, IDC_ARROW);
    baseClass.hInstance = instance;
    baseClass.lpfnWndProc = GameWindowProc;
    g_WindowActivateWParam = TRUE;
    g_WindowInactive = FALSE;
    baseClass.lpszClassName = "BASE";
    RegisterClassA(&baseClass);

    HWND window;
    if (!g_Supervisor.IsWindowed())
    {
        width = 640;
        height = 480;
        window = CreateWindowExA(
            0, "BASE", g_WindowTitle, WS_OVERLAPPEDWINDOW,
            0, 0, width, height, NULL, NULL, instance, NULL);
    }
    else
    {
        width = GetSystemMetrics(SM_CXDLGFRAME) * 2 + 640;
        height =
            GetSystemMetrics(SM_CYDLGFRAME) * 2 +
            GetSystemMetrics(SM_CYCAPTION) + 480;
        window = CreateWindowExA(
            0, "BASE", g_WindowTitle,
            WS_VISIBLE | WS_MINIMIZEBOX | WS_SYSMENU,
            CW_USEDEFAULT, CW_USEDEFAULT, width, height,
            NULL, NULL, instance, NULL);
    }

    g_GameWindowWindow = window;
    g_SupervisorGameWindow = window;
    if (window == NULL)
        return 1;

    ActivateGameWindow(window);
    return 0;
}

