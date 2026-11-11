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

// Retail uses 0x2A4-byte native VM slots in the real FrontFactory
// allocation and two 0x38-byte side records in the real GameManager.
static __forceinline AnmVm &NativeFrontIndicatorVm(
    FrontFactoryConstructView *owner, int side)
{
    return *reinterpret_cast<AnmVm *>(
        reinterpret_cast<unsigned char *>(&owner->markerVms123F4[0]) +
        static_cast<unsigned int>(side) * sizeof(owner->markerVms123F4[0]));
}
static __forceinline AsciiGameManagerSideView &NativeFrontIndicatorSide(int side)
{
    return *reinterpret_cast<AsciiGameManagerSideView *>(
        reinterpret_cast<unsigned char *>(&g_GameManager.sides[0]) +
        static_cast<unsigned int>(side) * sizeof(g_GameManager.sides[0]));
}

void FrontFactoryConstructView::SetSideEnemyIndicatorPosition(
    int sideIndex, const Float3 *enemyPosition)
{
    g_Supervisor.SelectSide(sideIndex);

    if (enemyPosition->x < -112.0f || enemyPosition->x > 112.0f)
    {
        NativeFrontIndicatorVm(this, sideIndex).color1Script.a = 0;
        return;
    }

    float distance =
        NativeFrontIndicatorSide(sideIndex).player->position.x -
        enemyPosition->x;
    if (distance < 0.0f)
        distance = -distance;

    if (distance < 64.0f)
        NativeFrontIndicatorVm(this, sideIndex).color1Script.a =
            static_cast<unsigned char>(distance) * 3 + 64;
    else
        NativeFrontIndicatorVm(this, sideIndex).color1Script.a = 0xFF;

    NativeFrontIndicatorVm(this, sideIndex).pos.x =
        g_GameManager.TransformPopupX(enemyPosition->x);
    NativeFrontIndicatorVm(this, sideIndex).pos.y = 472.0f;
    NativeFrontIndicatorVm(this, sideIndex).pos.z = 0.0f;
}
