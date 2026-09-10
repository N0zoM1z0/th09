#pragma once

#include <windows.h>

class PbgArchive
{
  public:
    unsigned char *ReadDecompressEntry(const char *filename, unsigned char *outBuffer);
    DWORD GetEntryDecompressedSize(const char *filename);
};

extern PbgArchive g_PbgArchive;
