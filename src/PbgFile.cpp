#include "PbgFileBackend.hpp"

#include <string.h>

PbgFileView::PbgFileView()
{
    handle = INVALID_HANDLE_VALUE;
    desiredAccess = 0;
}

PbgFileView::~PbgFileView()
{
    Close();
}

bool PbgFileView::Open(const char *filename, char *mode)
{
    DWORD creationDisposition;
    BOOL goToEnd = FALSE;
    char filePathBuffer[MAX_PATH];

    Close();

    char *curMode;
    for (curMode = mode; *curMode != '\0'; curMode++)
    {
        if (*curMode == 'r')
        {
            desiredAccess = GENERIC_READ;
            creationDisposition = OPEN_EXISTING;
            break;
        }
        if (*curMode == 'w')
        {
            DeleteFileA(filename);
            desiredAccess = GENERIC_WRITE;
            creationDisposition = CREATE_ALWAYS;
            break;
        }
        if (*curMode == 'a')
        {
            goToEnd = TRUE;
            desiredAccess = GENERIC_WRITE;
            creationDisposition = OPEN_ALWAYS;
            break;
        }
    }

    if (*curMode == '\0')
        return false;

    GetFullFilePath(filePathBuffer, filename);
    handle = CreateFileA(filePathBuffer, desiredAccess, FILE_SHARE_READ, NULL,
                         creationDisposition,
                         FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN,
                         NULL);
    if (handle == INVALID_HANDLE_VALUE)
        return false;

    if (goToEnd)
        SetFilePointer(handle, 0, NULL, FILE_END);
    return true;
}

void PbgFileView::Close()
{
    if (handle != INVALID_HANDLE_VALUE)
    {
        CloseHandle(handle);
        handle = INVALID_HANDLE_VALUE;
        desiredAccess = 0;
    }
}

DWORD PbgFileView::Read(void *data, DWORD dataLen)
{
    DWORD numBytesRead = 0;

    if (desiredAccess != GENERIC_READ)
        return 0;

    ReadFile(handle, data, dataLen, &numBytesRead, NULL);
    return numBytesRead;
}

bool PbgFileView::Write(void *data, DWORD dataLen)
{
    DWORD outWritten = 0;

    if (desiredAccess != GENERIC_WRITE)
        return false;

    WriteFile(handle, data, dataLen, &outWritten, NULL);
    return dataLen == outWritten ? true : false;
}

DWORD PbgFileView::Tell()
{
    if (handle == INVALID_HANDLE_VALUE)
        return 0;

    return SetFilePointer(handle, 0, NULL, FILE_CURRENT);
}

DWORD PbgFileView::GetSize()
{
    if (handle == INVALID_HANDLE_VALUE)
        return 0;

    return GetFileSize(handle, NULL);
}

bool PbgFileView::Seek(DWORD offset, DWORD seekFrom)
{
    if (handle == INVALID_HANDLE_VALUE)
        return false;

    SetFilePointer(handle, offset, NULL, seekFrom);
    return true;
}

HGLOBAL PbgFileView::ReadWholeFile(DWORD maxSize)
{
    if (desiredAccess != GENERIC_READ)
        return NULL;

    DWORD dataLen = GetSize();
    if (dataLen > maxSize)
        return NULL;

    HGLOBAL data = GlobalAlloc(LPTR, dataLen);
    if (data == NULL)
        return NULL;

    DWORD oldLocation = Tell();
    if (Seek(oldLocation, g_PbgFileSeekModes[0]) == 0)
        return NULL;

    if (Read(data, dataLen) == 0)
    {
        if (data)
        {
            GlobalFree(data);
            data = NULL;
        }
        return NULL;
    }

    Seek(oldLocation, g_PbgFileSeekModes[0]);
    return data;
}

void PbgFileView::GetFullFilePath(char *buffer, const char *filename)
{
    if (strchr(filename, ':') != NULL)
    {
        strcpy(buffer, filename);
    }
    else
    {
        GetModuleFileNameA(NULL, buffer, MAX_PATH);

        char *endOfModulePath = strrchr(buffer, '\\');
        if (endOfModulePath == NULL)
            strcpy(buffer, "");

        endOfModulePath[1] = '\0';
        strcat(buffer, filename);
    }
}
