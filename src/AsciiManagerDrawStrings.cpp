#include "AnmManager.hpp"
#include "AsciiManager.hpp"
#include "Supervisor.hpp"

void AsciiManager::DrawStrings()
{
    int isGui = 1;
    int i;
    // The actual 256-entry text queue consists of 0x60-byte records.
    // Move between original allocated records and file/user text bytes as
    // native modulo-32 Win32 addresses without a second C++ array view.
    AsciiManagerString *curString = &this->strings[0];
    unsigned char *text;
    float spaceWidth;

    this->largeText.visible = 1;
    this->largeText.anchor = 3;

    for (i = 0; i < this->numStrings;
         i++, curString = reinterpret_cast<AsciiManagerString *>(
             reinterpret_cast<unsigned int>(curString) + sizeof(*curString))) {
        this->largeText.pos = curString->position;
        text = (unsigned char *)curString->text;
        this->largeText.scale.x = curString->scaleX;
        this->largeText.scale.y = curString->scaleY;
        spaceWidth = this->spaceWidth * curString->scaleX;

        if (isGui != curString->isGui) {
            isGui = curString->isGui;
            g_AnmManager->FlushVertexBuffer();
            if (isGui) {
                g_Supervisor.ConfigureGameplayViewport(0);
            } else {
                g_Supervisor.ConfigureGameplayViewport(2);
            }
        }

        while (*text) {
            if (*text == '\n') {
                this->largeText.pos.y += 14.0f * curString->scaleY;
                this->largeText.pos.x = curString->position.x;
            } else if (*text == ' ') {
                this->largeText.pos.x += spaceWidth;
            } else {
                this->largeText.loadedSprite = this->asciiAnm->GetSprite(*text - ' ');
                this->largeText.color1 = curString->color;
                g_AnmManager->DrawNoRotation(&this->largeText);
                this->largeText.pos.x += spaceWidth;
            }
            text = reinterpret_cast<unsigned char *>(
                reinterpret_cast<unsigned int>(text) + 1u);
        }
    }

    if (isGui) {
        g_AnmManager->FlushVertexBuffer();
        g_Supervisor.ConfigureGameplayViewport(2);
    }
}
