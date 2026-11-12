#include "AsciiManager.hpp"

void AsciiManager::CreateScorePopup(int playerIndex, Float3 *position, int number, unsigned long color)
{
    AsciiManagerPopup *popup;
    int characterCount;

    if (this->nextScorePopupIndex >= 100) {
        this->nextScorePopupIndex = 0;
    }

    // The real AsciiManager owns two rows of 100 live 0x38-byte
    // popup records. The side ID and rolling write index form original
    // unsigned i386 DWORD strides without an extra C++ bounds gate.
    popup = reinterpret_cast<AsciiManagerPopup *>(
        reinterpret_cast<unsigned int>(&this->scorePopups[0][0]) +
        static_cast<unsigned int>(playerIndex) * sizeof(this->scorePopups[0]) +
        static_cast<unsigned int>(this->nextScorePopupIndex) *
            sizeof(this->scorePopups[0][0]));
    popup->inUse = 1;

    characterCount = 0;
    if (number >= 0) {
        while (number != 0) {
            popup->text[characterCount] = number % 10;
            characterCount++;
            number /= 10;
        }
    } else {
        popup->text[characterCount] = 10;
        characterCount++;
    }

    if (characterCount == 0) {
        popup->text[characterCount] = 0;
        characterCount++;
    }

    popup->characterCount = characterCount;
    popup->color = color;
    popup->timer = 0;
    popup->position = *position;
    ++*reinterpret_cast<unsigned int *>(&this->nextScorePopupIndex);
}
