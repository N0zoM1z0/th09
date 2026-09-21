#include "EnemyManager.hpp"
#include "AsciiManager.hpp"

#include <stddef.h>

namespace
{
struct EnemyClampPositionView
{
    unsigned char unknown0000[0x2D74];
    Float3 position2D74;
    unsigned char unknown2D80[0x337E - 0x2D80];
    unsigned char movementFlags337E;
    unsigned char unknown337F[0x3398 - 0x337F];
    Float2 movementLowerBounds3398;
    Float2 movementUpperBounds33A0;
};

typedef char EnemyClampPositionAt2D74[
    (offsetof(EnemyClampPositionView, position2D74) == 0x2D74) ? 1 : -1];
typedef char EnemyClampFlagAt337E[
    (offsetof(EnemyClampPositionView, movementFlags337E) == 0x337E) ? 1 : -1];
typedef char EnemyClampLowerAt3398[
    (offsetof(EnemyClampPositionView, movementLowerBounds3398) == 0x3398) ? 1 : -1];
typedef char EnemyClampUpperAt33A0[
    (offsetof(EnemyClampPositionView, movementUpperBounds33A0) == 0x33A0) ? 1 : -1];
}

void EnemyView::ClampPositionToMovementBounds()
{
    EnemyClampPositionView *view =
        reinterpret_cast<EnemyClampPositionView *>(this);
    if ((view->movementFlags337E & 1) == 0)
        return;

    float *position = view->position2D74.operator float *();

    if (position[0] < view->movementLowerBounds3398.x)
        position[0] = view->movementLowerBounds3398.x;
    else if (position[0] > view->movementUpperBounds33A0.x)
        position[0] = view->movementUpperBounds33A0.x;

    if (position[1] < view->movementLowerBounds3398.y)
        position[1] = view->movementLowerBounds3398.y;
    else if (position[1] > view->movementUpperBounds33A0.y)
        position[1] = view->movementUpperBounds33A0.y;
}
