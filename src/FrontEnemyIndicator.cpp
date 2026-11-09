#include "FrontFactoryConstructView.hpp"
#include "AsciiGameManagerView.hpp"
#include "Supervisor.hpp"

#include <stddef.h>

// Verified against the complete 0x1293C-byte native factory allocation;
// no second 0x12698-byte indicator owner exists.
typedef char FrontEnemyIndicatorNativeVmFields[
    (sizeof(AnmVm) == 0x2A4 &&
     offsetof(AnmVmBase, color1Script) + offsetof(AnmVmScriptColor, a) == 0x1F3 &&
     offsetof(AnmVmBase, pos) == 0x208 &&
     offsetof(FrontFactoryConstructView, markerVms123F4) == 0x123F4) ? 1 : -1];

extern AsciiGameManagerView g_GameManager;
extern Supervisor g_Supervisor;

void FrontFactoryConstructView::SetSideEnemyIndicatorPosition(
    int sideIndex, const Float3 *enemyPosition)
{
    g_Supervisor.SelectSide(sideIndex);

    if (enemyPosition->x < -112.0f || enemyPosition->x > 112.0f)
    {
        this->markerVms123F4[sideIndex].color1Script.a = 0;
        return;
    }

    float distance =
        g_GameManager.sides[sideIndex].player->position.x -
        enemyPosition->x;
    if (distance < 0.0f)
        distance = -distance;

    if (distance < 64.0f)
        this->markerVms123F4[sideIndex].color1Script.a =
            static_cast<unsigned char>(distance) * 3 + 64;
    else
        this->markerVms123F4[sideIndex].color1Script.a = 0xFF;

    this->markerVms123F4[sideIndex].pos.x =
        g_GameManager.TransformPopupX(enemyPosition->x);
    this->markerVms123F4[sideIndex].pos.y = 472.0f;
    this->markerVms123F4[sideIndex].pos.z = 0.0f;
}
