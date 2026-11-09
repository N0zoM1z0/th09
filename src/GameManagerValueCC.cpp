#include "GameManagerValueCC.hpp"

void GameManagerValueCCView::SubtractSaturating(int amount)
{
    value1C -= amount;
    if (value1C < 0)
        value1C = 0;
}
