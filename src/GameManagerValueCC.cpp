#include "GameManagerValueCC.hpp"

void GameManagerValueCCView::SubtractSaturating(int amount)
{
    // Original SUB DWORD wraps before the native signed nonnegative clamp.
    value1C = static_cast<int>(
        static_cast<unsigned int>(value1C) -
        static_cast<unsigned int>(amount));
    if (value1C < 0)
        value1C = 0;
}
