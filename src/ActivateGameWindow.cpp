#include <windows.h>

void __stdcall ActivateGameWindow(HWND hWnd)
{
    DWORD foregroundThread;
    DWORD targetThread;
    UINT foregroundLockTimeout;

    foregroundThread =
        GetWindowThreadProcessId(GetForegroundWindow(), NULL);
    targetThread = GetWindowThreadProcessId(hWnd, NULL);
    AttachThreadInput(targetThread, foregroundThread, TRUE);
    SystemParametersInfoA(
        SPI_GETFOREGROUNDLOCKTIMEOUT,
        0,
        &foregroundLockTimeout,
        0);
    SystemParametersInfoA(
        SPI_SETFOREGROUNDLOCKTIMEOUT,
        0,
        NULL,
        0);
    SetActiveWindow(hWnd);
    SystemParametersInfoA(
        SPI_SETFOREGROUNDLOCKTIMEOUT,
        0,
        &foregroundLockTimeout,
        0);
    AttachThreadInput(targetThread, foregroundThread, FALSE);
}
