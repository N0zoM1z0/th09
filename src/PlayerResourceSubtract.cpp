#include "PlayerLifecycleView.hpp"

void PlayerLifecycleView::SubtractResourceClamped(float amount)
{
    float value = scalar30388 - amount;
    if (value < 1.0f)
        scalar30388 = 1.0f;
    else
        scalar30388 = value;
}
