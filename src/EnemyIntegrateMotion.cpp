#include "EnemyManager.hpp"
#include "AsciiManager.hpp"
#include "Supervisor.hpp"

#include <stddef.h>

void EnemyView::IntegrateMotion()
{
    // Float3's exact original operator- takes typed vector operands; both
    // source buffers and the destination now belong to the real EnemyView.

    reinterpret_cast<Float3 &>(this->displacement2DB0) =
        reinterpret_cast<const Float3 &>(this->position2D74) -
        reinterpret_cast<const Float3 &>(this->previousPosition2DA4);
    this->previousPosition2DA4 = this->position2D74;

    if (this->secondaryFlagBits3380.movement.interactionTriggered12 != 0)
    {
        this->position2D74.x +=
            g_Supervisor.framerateMultiplier5B8 *
            this->alternateVelocity2D98.x;
        this->position2D74.y +=
            g_Supervisor.framerateMultiplier5B8 *
            this->alternateVelocity2D98.y;
    }
    else
    {
        if (this->primaryFlagBits337C.mirrorMovementX15 == 0)
            this->position2D74.x +=
                g_Supervisor.framerateMultiplier5B8 *
                this->velocity2D8C.x;
        else
            this->position2D74.x -=
                g_Supervisor.framerateMultiplier5B8 *
                this->velocity2D8C.x;
        this->position2D74.y +=
            g_Supervisor.framerateMultiplier5B8 * this->velocity2D8C.y;
    }
}
