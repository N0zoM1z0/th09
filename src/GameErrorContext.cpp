#include "GameErrorContext.hpp"

#include <windows.h>
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
