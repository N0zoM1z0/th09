#pragma once

#include <stddef.h>

class GameErrorContext
{
  public:
    void ResetContext();
    void Flush();
    const char *Log(const char *fmt, ...);
    const char *Fatal(const char *fmt, ...);

  private:
    char buffer[0x2000];
    char *bufferEnd;
    char showMessageBox;
};

namespace FileSystem
{
int WriteDataToFile(const char *path, void *data, size_t size);
}
