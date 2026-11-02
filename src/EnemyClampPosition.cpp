#include "EnemyManager.hpp"
#include "AsciiManager.hpp"

#include <stddef.h>

void EnemyView::ClampPositionToMovementBounds()
{
    // The original +0x337E bit zero is native Enemy flag bit 16.
    if (!this->primaryFlagBits337C.clampPosition16)
        return;

    float *position =
        reinterpret_cast<Float3 &>(this->position2D74).operator float *();

    if (position[0] < this->movementLowerBounds3398.x)
        position[0] = this->movementLowerBounds3398.x;
    else if (position[0] > this->movementUpperBounds33A0.x)
        position[0] = this->movementUpperBounds33A0.x;

    if (position[1] < this->movementLowerBounds3398.y)
        position[1] = this->movementLowerBounds3398.y;
    else if (position[1] > this->movementUpperBounds33A0.y)
        position[1] = this->movementUpperBounds33A0.y;
}
