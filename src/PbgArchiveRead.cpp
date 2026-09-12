#include "PbgArchive.hpp"
#include "Lzss.hpp"

#include "PbgFileBackend.hpp"

struct PbgArchiveReadStatePrefix
{
    PbgArchiveEntry *entries;
    int entryCount;
    char *archiveFilename;
    PbgFileBackend *file;
};

unsigned char *PbgArchive::ReadDecompressEntry(const char *filename,
                                                unsigned char *outBuffer)
{
    PbgArchiveReadStatePrefix *state = (PbgArchiveReadStatePrefix *)this;
    unsigned char *decompressedData;
    unsigned char *compressedData;
    DWORD compressedSize;
    DWORD decompressedSize;

    if (state->file == NULL)
        return NULL;

    PbgArchiveEntry *entry = FindEntry(filename);
    if (entry == NULL)
        goto read_error;

    if (!state->file->Open(state->archiveFilename, g_PbgFileOpenModes[0]))
        goto read_error;

    compressedSize = entry[1].dataOffset - entry->dataOffset;
    decompressedSize = entry->decompressedSize;
    compressedData = (unsigned char *)GlobalAlloc(0, compressedSize);
    if (compressedData == NULL)
        goto read_error;

    if (!state->file->Seek(entry->dataOffset, g_PbgFileSeekModes[0]))
        goto free_error;
    if (state->file->Read(compressedData, compressedSize) == 0)
        goto free_error;
    goto read_success;

free_error:
    GlobalFree(compressedData);
read_error:
    return NULL;

read_success:
    decompressedData =
        Lzss::Decode(compressedData, compressedSize, outBuffer, decompressedSize);
    GlobalFree(compressedData);
    return decompressedData;
}
