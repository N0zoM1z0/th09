#include "AsciiManager.hpp"

#include <stdarg.h>
#include <stdio.h>
#include <string.h>

void AsciiManager::AddString(Float3 *position, const char *string)
{
    AsciiManagerString *nextString;

    if (this->numStrings >= 256) {
        return;
    }

    // The signed queue count selects one of the real 0x60-byte records.
    nextString = reinterpret_cast<AsciiManagerString *>(
        reinterpret_cast<unsigned int>(&this->strings[0]) +
        static_cast<unsigned int>(this->numStrings) * sizeof(this->strings[0]));
    ++*reinterpret_cast<unsigned int *>(&this->numStrings);

    strcpy(nextString->text, string);
    nextString->position = *position;
    nextString->color = this->color;
    nextString->scaleX = this->scaleX;
    nextString->scaleY = this->scaleY;
    nextString->isGui = this->isGui;
    nextString->isSelected = 0;
}

void AsciiManager::AddFormatText(Float3 *position, const char *fmt, ...)
{
    char buffer[512];
    va_list args;

    va_start(args, fmt);
    vsprintf(buffer, fmt, args);
    this->AddString(position, buffer);
    va_end(args);
}
