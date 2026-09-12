#pragma once

#include <windows.h>

class PbgFileBackend
{
  public:
    PbgFileBackend();
    virtual bool Open(const char *filename, char *mode) = 0;
    virtual void Close() = 0;
    virtual DWORD Read(void *data, DWORD dataLen) = 0;
    virtual bool Write(void *data, DWORD dataLen) = 0;
    virtual DWORD Tell() = 0;
    virtual DWORD GetSize() = 0;
    virtual bool Seek(DWORD offset, DWORD seekFrom) = 0;
    virtual ~PbgFileBackend();

    DWORD ReadInt(int *outData);
};

class PbgFileView : public PbgFileBackend
{
  public:
    PbgFileView();
    virtual ~PbgFileView();
    virtual bool Open(const char *filename, char *mode);
    virtual void Close();
    virtual DWORD Read(void *data, DWORD dataLen);
    virtual bool Write(void *data, DWORD dataLen);
    virtual DWORD Tell();
    virtual DWORD GetSize();
    virtual bool Seek(DWORD offset, DWORD seekFrom);
    virtual HGLOBAL ReadWholeFile(DWORD maxSize);
    static void GetFullFilePath(char *buffer, const char *filename);

  private:
    HANDLE handle;
    DWORD desiredAccess;
};

typedef char PbgFileViewSizeIs0C[(sizeof(PbgFileView) == 0x0C) ? 1 : -1];

extern char *g_PbgFileOpenModes[3];
extern int g_PbgFileSeekModes[3];
