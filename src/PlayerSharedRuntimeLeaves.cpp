#include "PlayerSharedRuntimeView.hpp"

int PlayerSharedRuntimeView::IsBlocked()
{
    int value = blockStateE94C;
    if (value >= 0 || value == -2)
        return 1;
    return 0;
}
