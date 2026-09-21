#include "FileSystem.hpp"
#include "GameErrorContext.hpp"
#include "GameWindow.hpp"

#include <windows.h>
#include <string.h>

extern HANDLE g_ExclusiveMutex;
extern GameErrorContext g_GameErrorContext;
extern unsigned int g_WindowRuntimeFlags;

int __fastcall ResolveShortcut(
    char *shortcutPath, char *destination, int destinationSize);

int CheckForRunningGameInstance()
{
    char consoleTitleBuffer[MAX_PATH + 1];
    char *fileExtension;
    char moduleFilenameBuffer[MAX_PATH + 1];
    STARTUPINFOA startupInfo;

    g_ExclusiveMutex = CreateMutexA(NULL, TRUE, "Touhou 08 App");
    if (GetLastError() == ERROR_ALREADY_EXISTS)
    {
        g_GameErrorContext.Fatal(
            "\x93\xf1\x82\xc2\x82\xcd\x8b\x4e\x93\xae\x82\xc5\x82\xab\x82\xdc\x82\xb9\x82\xf1\r\n");
        return -1;
    }

    startupInfo.cb = sizeof(startupInfo);
    memset(
        &startupInfo.lpReserved,
        0,
        sizeof(startupInfo) - sizeof(startupInfo.cb));
    GetModuleFileNameA(NULL, moduleFilenameBuffer, sizeof(moduleFilenameBuffer));
    GetConsoleTitleA(consoleTitleBuffer, sizeof(consoleTitleBuffer));
    GetStartupInfoA(&startupInfo);

    if (startupInfo.lpTitle != NULL)
    {
        fileExtension = strrchr(startupInfo.lpTitle, '.');
        if (FileSystem::CheckIfFileAlreadyExists(startupInfo.lpTitle) &&
            fileExtension != NULL)
        {
            if (_stricmp(fileExtension, ".lnk") == 0)
            {
                do
                {
                    ResolveShortcut(
                        startupInfo.lpTitle, consoleTitleBuffer, MAX_PATH);
                    fileExtension = strrchr(consoleTitleBuffer, '.');
                } while (_stricmp(fileExtension, ".lnk") == 0);
            }
            else
            {
                strcpy(consoleTitleBuffer, startupInfo.lpTitle);
            }

            if (strcmp(moduleFilenameBuffer, consoleTitleBuffer) != 0)
                g_GameWindow.startupPathDiffers1C = 1;
        }
        g_WindowRuntimeFlags &= ~0x40u;
    }
    else
    {
        g_WindowRuntimeFlags |= 0x40u;
    }

    if (g_ExclusiveMutex == NULL)
        return -1;
    return 0;
}
