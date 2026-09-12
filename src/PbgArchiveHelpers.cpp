#include "PbgArchive.hpp"
#include "PbgFileBackend.hpp"

#include <string.h>
#include <windows.h>

PbgArchiveEntry::PbgArchiveEntry()
{
    filename = NULL;
}

PbgArchiveEntry::~PbgArchiveEntry()
{
    if (filename != NULL)
    {
        GlobalFree(filename);
        filename = NULL;
    }
}

char *PbgArchive::CopyFileName(const char *filename)
{
    char *mem = (char *)GlobalAlloc(0, strlen(filename) + 1);
    if (mem != NULL)
        strcpy(mem, filename);
    return mem;
}

int PbgArchive::SeekPastInt(void **ptr)
{
    *ptr = (int *)*ptr + 1;
    return *(int *)*ptr;
}

void *PbgArchive::SeekPastString(void **ptr)
{
    *ptr = (char *)*ptr + strlen((char *)*ptr) + 1;
    return *ptr;
}

PbgArchiveEntry *PbgArchive::AllocEntries(void *entryBuffer, int count,
                                          unsigned int dataOffset)
{
    void *entryData;
    int i;
    PbgArchiveEntry *buffer = NULL;

    buffer = new PbgArchiveEntry[count + 1]();
    if (buffer == NULL)
        goto buffer_alloc_error;

    entryData = entryBuffer;
    for (i = 0; i < count; i++)
    {
        buffer[i].filename = CopyFileName((char *)entryData);
        SeekPastString(&entryData);
        buffer[i].dataOffset = *(DWORD *)entryData;
        SeekPastInt(&entryData);
        buffer[i].decompressedSize = *(DWORD *)entryData;
        SeekPastInt(&entryData);
        buffer[i].fieldC = *(DWORD *)entryData;
        SeekPastInt(&entryData);
    }

    buffer[count].dataOffset = dataOffset;
    buffer[count].decompressedSize = 0;
    return buffer;

buffer_alloc_error:
    delete[] buffer;
    return NULL;
}

void PbgArchive::Release()
{
    if (archiveFilename != NULL)
    {
        GlobalFree(archiveFilename);
        archiveFilename = NULL;
    }
    if (entries != NULL)
    {
        delete[] entries;
        entries = NULL;
    }
    if (file != NULL)
    {
        delete file;
        file = NULL;
    }
    entryCount = 0;
}
