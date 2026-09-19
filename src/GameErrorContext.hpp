#pragma once

#include <stddef.h>

class GameErrorContext
{
  public:
    GameErrorContext();
    void ResetContext();
    void Flush();
    const char *Log(const char *fmt, ...);
    const char *Fatal(const char *fmt, ...);

  private:
    char buffer[0x2000];
    char *bufferEnd;
    char showMessageBox;
};
