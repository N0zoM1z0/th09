#include "EnemyManager.hpp"
#include "AsciiManager.hpp"

#include <stddef.h>

void EnemyView::ClampPositionToMovementBounds()
{
    // The original +0x337E bit zero is native Enemy flag bit 16.
    if (!this->primaryFlagBits337C.clampPosition16)
        return;

    // Preserve the original Float3 conversion, then reference the real
    // vector whose first field is the returned float address.
    Float3 *position = reinterpret_cast<Float3 *>(
        reinterpret_cast<Float3 &>(this->position2D74).operator float *());

    if (position->x < this->movementLowerBounds3398.x)
        position->x = this->movementLowerBounds3398.x;
    else if (position->x > this->movementUpperBounds33A0.x)
        position->x = this->movementUpperBounds33A0.x;

    if (position->y < this->movementLowerBounds3398.y)
        position->y = this->movementLowerBounds3398.y;
    else if (position->y > this->movementUpperBounds33A0.y)
        position->y = this->movementUpperBounds33A0.y;
}
