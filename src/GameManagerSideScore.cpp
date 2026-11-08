#include "GameManagerRuntimeView.hpp"

void AsciiGameManagerSideView::AddScore(int score)
{
    this->state->score08 += score / 10;
}
