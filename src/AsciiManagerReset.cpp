#include "AnmManager.hpp"
#include "AsciiManager.hpp"

#include <string.h>

void AsciiManager::Reset()
{
    memset(&this->smallScoreText, 0, sizeof(AnmVm));
    memset(&this->popupText, 0, sizeof(AnmVm));
    memset(&this->largeText, 0, sizeof(AnmVm));
    memset(&this->strings, 0, sizeof(this->strings));
    memset(&this->opaque_82A0, 0, sizeof(this->opaque_82A0));
    memset(&this->opaque_97C8, 0, sizeof(this->opaque_97C8));
    memset(&this->scorePopups[0], 0, sizeof(this->scorePopups[0]));

    this->numStrings = 0;
    this->isGui = 0;
    this->isSelected = 0;
    this->nextScorePopupIndex = 0;
    this->resetField8294 = 0;
    this->resetField829C = 0;
    this->color = 0xFFFFFFFF;
    this->scaleX = 1.0f;
    this->scaleY = 1.0f;
    this->smallScoreText.anchor = 3;
    this->popupText.anchor = 3;
    this->asciiAnm->InitializeAndSetSprite(&this->smallScoreText, 97);
    this->asciiAnm->InitializeAndSetSprite(&this->largeText, 32);
    this->smallScoreText.pos.z = 0.1f;
    this->isSelected = 0;
    this->SetSpaceWidth(9);
}
