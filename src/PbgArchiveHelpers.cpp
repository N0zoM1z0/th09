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
    // The packed archive buffer is byte-oriented, not an actual array of
    // standalone int objects. Retail advances by one native DWORD.
    *ptr = reinterpret_cast<void *>(
        reinterpret_cast<unsigned int>(*ptr) + sizeof(int));
    return *(int *)*ptr;
}

void *PbgArchive::SeekPastString(void **ptr)
{
    // Names end in-place in the same packed file buffer. Keep the live
    // strlen and advance past its terminator with an i386 DWORD address.
    *ptr = reinterpret_cast<void *>(
        reinterpret_cast<unsigned int>(*ptr) +
        static_cast<unsigned int>(strlen((char *)*ptr)) + 1u);
    return *ptr;
}

PbgArchiveEntry *PbgArchive::AllocEntries(void *entryBuffer, int count,
                                          unsigned int dataOffset)
{
    void *entryData;
    int i;
    PbgArchiveEntry *buffer = NULL;

    // The archive declares its file entry count as a native i386 word.
    // The extra terminator/sentinel element is counted with DWORD wrap.
    buffer = new PbgArchiveEntry[static_cast<unsigned int>(count) + 1u]();
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
