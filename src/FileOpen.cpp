#include "FileSystem.hpp"
#include "GameErrorContext.hpp"
#include "PbgArchive.hpp"
#include "Supervisor.hpp"
#include "ZunMemory.hpp"

#include <windows.h>
#include <string.h>

extern GameErrorContext g_GameErrorContext;

namespace FileSystem
{
unsigned char *TryDecryptFromTable(unsigned char *data, int *fileSize, int size);

unsigned char *OpenFile(const char *path, int *fileSize, int isExternalResource)
{
    const char *entryname;
    DWORD size;
    unsigned char *data;
    HANDLE handle;

    g_Supervisor.EnterCriticalSectionWrapper(2);

    if (!isExternalResource)
    {
        entryname = strrchr(path, '\\');
        if (entryname == NULL)
            entryname = path;
        else
            entryname++;

        entryname = strrchr(entryname, '/');
        if (entryname == NULL)
            entryname = path;
        else
            entryname++;

        size = g_PbgArchive.GetEntryDecompressedSize(entryname);
        if (fileSize != NULL)
            *fileSize = size;
        if (size == 0)
        {
            g_GameErrorContext.Fatal("error : %s is not found in arcfile.\r\n", entryname);
            goto error;
        }

        data = (unsigned char *)g_ZunMemory.Alloc(size, path);
        if (data == NULL)
            goto error;

        g_PbgArchive.ReadDecompressEntry(entryname, data);
        data = TryDecryptFromTable(data, fileSize, size);
        goto done;
    }

    path = ResolvePath(path);
    handle = CreateFileA(
        path, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING,
        FILE_FLAG_SEQUENTIAL_SCAN | FILE_ATTRIBUTE_NORMAL, NULL);
    if (handle == INVALID_HANDLE_VALUE)
        goto error;

    size = GetFileSize(handle, NULL);
    data = (unsigned char *)g_ZunMemory.Alloc(size, path);
    if (data == NULL)
    {
        CloseHandle(handle);
        goto error;
    }

    ReadFile(handle, data, size, &size, NULL);
    if (fileSize != NULL)
        *fileSize = size;
    CloseHandle(handle);
    data = TryDecryptFromTable(data, fileSize, size);

done:
    g_Supervisor.LeaveCriticalSectionWrapper(2);
    return data;

error:
    g_Supervisor.LeaveCriticalSectionWrapper(2);
    return NULL;
}
} // namespace FileSystem
