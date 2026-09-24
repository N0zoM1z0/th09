#include "EnemyManager.hpp"
#include "AsciiManager.hpp"

#include <stddef.h>

struct EnemyMotionSupervisorView
{
    unsigned char unknown000[0x5B8];
    float frameRateMultiplier5B8;
};
extern EnemyMotionSupervisorView g_Supervisor;

namespace
{
struct EnemyIntegrateMotionPrivateView
{
    unsigned char unknown0000[0x2D74];
    Float3 position2D74;
    unsigned char unknown2D80[0x2D8C - 0x2D80];
    Float3 velocity2D8C;
    Float3 alternateVelocity2D98;
    Float3 previousPosition2DA4;
    Float3 displacement2DB0;
    unsigned char unknown2DBC[0x337C - 0x2DBC];
    unsigned int flags337C;
    unsigned int flags3380;
};
typedef char EnemyMotionPositionAt2D74[
    (offsetof(EnemyIntegrateMotionPrivateView, position2D74) == 0x2D74) ? 1 : -1];
typedef char EnemyMotionFlagsAt337C[
    (offsetof(EnemyIntegrateMotionPrivateView, flags337C) == 0x337C) ? 1 : -1];
}

void EnemyView::IntegrateMotion()
{
    EnemyIntegrateMotionPrivateView *view =
        reinterpret_cast<EnemyIntegrateMotionPrivateView *>(this);

    view->displacement2DB0 =
        view->position2D74 - view->previousPosition2DA4;
    view->previousPosition2DA4 = view->position2D74;

    if ((view->flags3380 & 0x1000U) != 0)
    {
        view->position2D74.x +=
            g_Supervisor.frameRateMultiplier5B8 *
            view->alternateVelocity2D98.x;
        view->position2D74.y +=
            g_Supervisor.frameRateMultiplier5B8 *
            view->alternateVelocity2D98.y;
    }
    else
    {
        if ((view->flags337C & 0x8000U) == 0)
            view->position2D74.x +=
                g_Supervisor.frameRateMultiplier5B8 *
                view->velocity2D8C.x;
        else
            view->position2D74.x -=
                g_Supervisor.frameRateMultiplier5B8 *
                view->velocity2D8C.x;
        view->position2D74.y +=
            g_Supervisor.frameRateMultiplier5B8 * view->velocity2D8C.y;
    }
}
