#include "AsciiManager.hpp"

void AsciiManager::CreateScorePopup(int playerIndex, Float3 *position, int number, unsigned long color)
{
    AsciiManagerPopup *popup;
    int characterCount;

    if (this->nextScorePopupIndex >= 100) {
        this->nextScorePopupIndex = 0;
    }

    popup = &this->scorePopups[playerIndex][this->nextScorePopupIndex];
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
    this->nextScorePopupIndex++;
}
