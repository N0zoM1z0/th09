#include "GameManagerRuntimeView.hpp"

void AsciiGameManagerSideView::AddScore(int score)
{
    // Original ADD DWORD accumulates the signed score quotient modulo 2^32.
    this->state->score08 = static_cast<int>(
        static_cast<unsigned int>(this->state->score08) +
        static_cast<unsigned int>(score / 10));
}
