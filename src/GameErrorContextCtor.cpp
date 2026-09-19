#include "GameErrorContext.hpp"

GameErrorContext::GameErrorContext()
{
    bufferEnd = buffer;
    buffer[0] = 0;
    showMessageBox = 0;
}
