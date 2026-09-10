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

    nextString = &this->strings[this->numStrings];
    this->numStrings++;

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
