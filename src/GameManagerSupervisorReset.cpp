#include "GameManagerResetView.hpp"

#include <string.h>

void GameManagerResetView::ResetForSupervisorAdded()
{
    memset(this, 0, sizeof(*this));
    InitializeCoordinateBounds();
    flag13F = 0;
    value378 = 10;
    value37C = 10;
}
