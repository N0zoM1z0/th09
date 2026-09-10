#include "FileSystem.hpp"
#include "Supervisor.hpp"

#include <windows.h>

namespace FileSystem
{
BOOL CheckIfFileAlreadyExists(const char *path)
{
    g_Supervisor.EnterCriticalSectionWrapper(2);
    HANDLE handle = CreateFileA(
        ResolvePath(path), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING,
        FILE_FLAG_SEQUENTIAL_SCAN | FILE_ATTRIBUTE_NORMAL, NULL);
    if (handle != INVALID_HANDLE_VALUE)
    {
        CloseHandle(handle);
        g_Supervisor.LeaveCriticalSectionWrapper(2);
        return TRUE;
    }

    g_Supervisor.LeaveCriticalSectionWrapper(2);
    return FALSE;
}
} // namespace FileSystem
