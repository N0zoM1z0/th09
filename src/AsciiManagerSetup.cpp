#include "AnmManager.hpp"
#include "AsciiManager.hpp"

#include <string.h>

void AsciiManager::SetSpaceWidth(int spaceWidth)
{
    this->spaceWidth = spaceWidth;
}

int AsciiManager::AddedCallback(AsciiManager *ascii)
{
    memset(ascii, 0, sizeof(AsciiManager));

    ascii->asciiAnm = g_AnmManager->PreloadAnm(ANM_FILE_SLOT_ASCII, "ascii.anm");
    if (ascii->asciiAnm == 0) {
        return -1;
    }

    ascii->captureAnm = g_AnmManager->PreloadAnm(ANM_FILE_SLOT_CAPTURE, "capture.anm");
    if (ascii->captureAnm == 0) {
        return -1;
    }

    ascii->Reset();
    return 0;
}
