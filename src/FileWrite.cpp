#include "FileSystem.hpp"
#include "Supervisor.hpp"

#include <windows.h>

namespace FileSystem
{
int WriteDataToFile(const char *path, void *data, size_t size)
{
    DWORD bytesWritten;

    path = ResolvePath(path);
    g_Supervisor.EnterCriticalSectionWrapper(2);
    HANDLE handle = CreateFileA(
        path, GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS,
        FILE_ATTRIBUTE_NORMAL, NULL);
    if (handle == INVALID_HANDLE_VALUE)
    {
        LPSTR errorMessage;
        FormatMessageA(
            FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM |
                FORMAT_MESSAGE_IGNORE_INSERTS,
            NULL, GetLastError(), 0x400,
            reinterpret_cast<LPSTR>(&errorMessage), 0, NULL);
        LocalFree(errorMessage);
        g_Supervisor.LeaveCriticalSectionWrapper(2);
        return -1;
    }

    WriteFile(handle, data, size, &bytesWritten, NULL);
    if (size != bytesWritten)
    {
        CloseHandle(handle);
        g_Supervisor.LeaveCriticalSectionWrapper(2);
        return -2;
    }

    CloseHandle(handle);
    g_Supervisor.LeaveCriticalSectionWrapper(2);
    return 0;
}
} // namespace FileSystem
