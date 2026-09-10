#include "GameErrorContext.hpp"
#include "FileSystem.hpp"
#include "Supervisor.hpp"

#include <windows.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

void GameErrorContext::ResetContext()
{
    this->bufferEnd = this->buffer;
    this->bufferEnd[0] = '\0';
}

void GameErrorContext::Flush()
{
    if (this->bufferEnd != this->buffer)
    {
        Log("---------------------------------------------------------- \r\n");
        if (this->showMessageBox)
        {
            MessageBoxA(NULL, this->buffer, "log", MB_ICONSTOP);
        }
        FileSystem::WriteDataToFile("./log.txt", this->buffer, strlen(this->buffer));
    }
}

const char *GameErrorContext::Log(const char *fmt, ...)
{
    char tmpBuffer[0x2000];
    size_t tmpBufferSize;
    va_list args;

    va_start(args, fmt);
    g_Supervisor.EnterCriticalSectionWrapper(3);
    vsprintf(tmpBuffer, fmt, args);
    tmpBufferSize = strlen(tmpBuffer);
    if (this->bufferEnd + tmpBufferSize < &this->buffer[sizeof(this->buffer) - 1])
    {
        strcpy(this->bufferEnd, tmpBuffer);
        this->bufferEnd += tmpBufferSize;
        this->bufferEnd[0] = '\0';
    }
    va_end(args);
    g_Supervisor.LeaveCriticalSectionWrapper(3);
    return fmt;
}

const char *GameErrorContext::Fatal(const char *fmt, ...)
{
    char tmpBuffer[512];
    size_t tmpBufferSize;
    va_list args;

    va_start(args, fmt);
    g_Supervisor.EnterCriticalSectionWrapper(3);
    vsprintf(tmpBuffer, fmt, args);
    tmpBufferSize = strlen(tmpBuffer);
    if (this->bufferEnd + tmpBufferSize < &this->buffer[sizeof(this->buffer) - 1])
    {
        strcpy(this->bufferEnd, tmpBuffer);
        this->bufferEnd += tmpBufferSize;
        this->bufferEnd[0] = '\0';
    }
    va_end(args);
    this->showMessageBox = true;
    g_Supervisor.LeaveCriticalSectionWrapper(3);
    return fmt;
}
