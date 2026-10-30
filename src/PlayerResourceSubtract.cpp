#include "PlayerResourceSubtractView.hpp"

void PlayerResourceSubtractView::SubtractResourceClamped(float amount)
{
    float value = resource30388 - amount;
    if (value < 1.0f)
        resource30388 = 1.0f;
    else
        resource30388 = value;
}
