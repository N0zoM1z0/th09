#pragma once

#include <windows.h>

class PbgFileBackend;

struct PbgArchiveEntry
{
    PbgArchiveEntry();
    ~PbgArchiveEntry();

    char *filename;
    DWORD dataOffset;
    DWORD decompressedSize;
    DWORD fieldC;
};

class PbgArchive
{
  public:
    ~PbgArchive();
    bool Load(const char *filename);
    void Release();
    unsigned char *ReadDecompressEntry(const char *filename, unsigned char *outBuffer);
    DWORD GetEntryDecompressedSize(const char *filename);
    PbgArchiveEntry *FindEntry(const char *filename);
    bool ParseHeader(const char *filename);
    PbgArchiveEntry *AllocEntries(void *entryBuffer, int count, unsigned int dataOffset);
    char *CopyFileName(const char *filename);
    static int SeekPastInt(void **ptr);
    static void *SeekPastString(void **ptr);

  private:
    PbgArchiveEntry *entries;
    int entryCount;
    char *archiveFilename;
    PbgFileBackend *file;
};

typedef char PbgArchiveSizeIs10[(sizeof(PbgArchive) == 0x10) ? 1 : -1];

extern PbgArchive g_PbgArchive;
