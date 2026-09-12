#include "PbgArchive.hpp"
#include "PbgFileBackend.hpp"
#include "FileSystem.hpp"
#include "Lzss.hpp"
#include "ZunMemory.hpp"

#include <windows.h>
#include <string.h>

struct PbgArchiveHeaderView
{
    int encodedEntryCount;
    int encodedFileTableOffset;
    int encodedFileTableDecompressedSize;
};

typedef char PbgArchiveHeaderViewSizeIs0C[
    (sizeof(PbgArchiveHeaderView) == 0x0C) ? 1 : -1];

extern const char g_PbgArchiveCorruptedError[];
extern void __cdecl PbgDebugPrint(const char *fmt, ...);

bool PbgArchive::Load(const char *filename)
{
    Release();
    file = new PbgFileView;
    if (file == NULL)
        return false;

    if (ParseHeader(filename))
    {
        archiveFilename = CopyFileName(filename);
        if (archiveFilename != NULL)
            return true;
    }

    Release();
    return false;
}

bool PbgArchive::ParseHeader(const char *filename)
{
    unsigned char *entryBuffer;
    int fileTableDecompressedSize;
    int magic;
    DWORD size;
    int fileTableOffset;
    PbgArchiveHeaderView header;
    unsigned char *fileTableBuffer;
    unsigned char *decryptedHeader;
    unsigned char *decryptedFileTable;

    fileTableBuffer = NULL;
    entryBuffer = NULL;

    if (file == NULL)
        return false;
    if (!file->Open(filename, g_PbgFileOpenModes[0]))
        goto parse_error;
    if (file->ReadInt(&magic) == 0)
        goto parse_error;
    if (magic != 0x5A474250)
        goto parse_error;
    if (file->Read(&header, sizeof(header)) == 0)
        goto parse_error;

    decryptedHeader = FileSystem::Decrypt(
        (unsigned char *)&header, sizeof(header), 0x1B, 0x37, sizeof(header), 0x400);
    memcpy(&header, decryptedHeader, sizeof(header));
    g_ZunMemory.Free(decryptedHeader);

    entryCount = header.encodedEntryCount - 123456;
    fileTableOffset = header.encodedFileTableOffset - 345678;
    fileTableDecompressedSize = header.encodedFileTableDecompressedSize - 567891;
    if (entryCount <= 0)
        goto parse_error;

    size = file->GetSize();
    if ((DWORD)fileTableOffset >= size)
        goto parse_error;
    size -= fileTableOffset;
    file->Seek(fileTableOffset, g_PbgFileSeekModes[0]);

    fileTableBuffer = (unsigned char *)GlobalAlloc(0, size);
    if (fileTableBuffer == NULL)
        goto parse_error;
    if (file->Read(fileTableBuffer, size) == 0)
        goto parse_error;

    decryptedFileTable =
        FileSystem::Decrypt(fileTableBuffer, size, 0x3E, 0x9B, 0x80, 0x400);
    GlobalFree(fileTableBuffer);
    fileTableBuffer = decryptedFileTable;

    entryBuffer = Lzss::Decode(fileTableBuffer, size, NULL, fileTableDecompressedSize);
    if (entryBuffer == NULL)
        goto parse_error;

    entries = AllocEntries(entryBuffer, entryCount, fileTableOffset);
    if (entries == NULL)
        goto parse_error;

    g_ZunMemory.Free(fileTableBuffer);
    GlobalFree(entryBuffer);
    return true;

parse_error:
    g_ZunMemory.Free(fileTableBuffer);
    if (entryBuffer != NULL)
        GlobalFree(entryBuffer);
    if (file != NULL)
    {
        delete file;
        file = NULL;
    }
    PbgDebugPrint(g_PbgArchiveCorruptedError, filename);
    return false;
}
