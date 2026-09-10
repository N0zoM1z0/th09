#include "PbgArchive.hpp"

#include <stddef.h>
#include <string.h>

struct PbgArchiveEntry
{
    char *filename;
    DWORD field4;
    DWORD decompressedSize;
    DWORD fieldC;
};

typedef char PbgArchiveEntrySizeIs10[(sizeof(PbgArchiveEntry) == 0x10) ? 1 : -1];
typedef char PbgArchiveEntryDecompressedSizeAt8[
    (offsetof(PbgArchiveEntry, decompressedSize) == 0x8) ? 1 : -1];

struct PbgArchiveStatePrefix
{
    PbgArchiveEntry *entries;
    int entryCount;
};

PbgArchiveEntry *PbgArchive::FindEntry(const char *filename)
{
    PbgArchiveStatePrefix *state = (PbgArchiveStatePrefix *)this;
    if (state->entries == NULL)
        return NULL;

    PbgArchiveEntry *entry = state->entries;
    for (int i = state->entryCount; i > 0; i--, entry++)
    {
        if (_stricmp(filename, entry->filename) == 0)
            return entry;
    }
    return NULL;
}

DWORD PbgArchive::GetEntryDecompressedSize(const char *filename)
{
    PbgArchiveEntry *entry = FindEntry(filename);
    if (entry != NULL)
        return entry->decompressedSize;
    return 0;
}
