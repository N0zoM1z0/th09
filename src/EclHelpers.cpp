#include "EclRunState.inl"

#include <string.h>

// Maintained natural reconstruction of helper owners around
// EclManager::RunEcl. InterpolateLinear and InterpolateHermite in this TU are
// canonical exact under their recorded match units. Reconstruction names and
// wider RunEcl ownership/TU questions remain independent of that exact credit.

namespace
{

typedef char EclHelperActiveContextAt2CE0[
    (offsetof(EnemyView, activeContext2CE0) == 0x2CE0) ? 1 : -1];
typedef char EclHelperPositionAt2D74[
    (offsetof(EnemyView, position2D74) == 0x2D74) ? 1 : -1];
typedef char EclHelperMovementAt2DE0[
    (offsetof(EnemyView, movementAngle2DE0) == 0x2DE0) ? 1 : -1];
typedef char EclHelperBulletAt2E74[
    (offsetof(EnemyView, bulletDescriptor2E74) == 0x2E74) ? 1 : -1];
typedef char EclHelperFlagsAt337C[
    (offsetof(EnemyView, primaryFlags337C) == 0x337C) ? 1 : -1];
typedef char EclHelperChildrenAt33D8[
    (offsetof(EnemyView, childEclBlocks33D8) == 0x33D8) ? 1 : -1];


void __fastcall InterpolateLinear(
    EnemyView *enemy, Th09EclInterpolationSlotView *slot, float progress)
{
    float start = enemy->ResolveFloat(slot->parameters1C[0]);
    float end = enemy->ResolveFloat(slot->parameters1C[1]);
    *Th09EclRunControl::ResolveFloatLValue(
        enemy, &slot->affectedVariable2C, 0, -1) =
        (end - start) * progress + start;
}

void __fastcall InterpolateHermite(
    EnemyView *enemy, Th09EclInterpolationSlotView *slot, float progress)
{
    float parameter0 = enemy->ResolveFloat(slot->parameters1C[0]);
    float parameter1 = enemy->ResolveFloat(slot->parameters1C[1]);
    float parameter2 = enemy->ResolveFloat(slot->parameters1C[2]);
    float parameter3 = enemy->ResolveFloat(slot->parameters1C[3]);
    float twice = progress + progress;
    float minusOne = progress - 1.0f;
    float inverse = 1.0f - progress;

    *Th09EclRunControl::ResolveFloatLValue(
        enemy, &slot->affectedVariable2C, 0, -1) =
        ((((twice + 1.0f) * minusOne * minusOne) * parameter0 +
          ((3.0f - twice) * progress * progress) * parameter1) +
         ((inverse * inverse) * progress) * parameter2) +
        ((minusOne * progress) * progress) * parameter3;
}


} // namespace

namespace Th09EclRunControl
{

// Eight reviewed callable slots at 0x004A0AF4; the next DWORD begins the
// separate ECL ex-instruction table. Original declared array extent is unknown.
Th09EclInterpolationCallback g_EclInterpolationCallbacks[8] =
{
    InterpolateLinear,
    InterpolateLinear,
    InterpolateLinear,
    InterpolateLinear,
    InterpolateLinear,
    InterpolateLinear,
    InterpolateLinear,
    InterpolateHermite
};

} // namespace Th09EclRunControl


namespace Th09EclRunMovement
{


} // namespace Th09EclRunMovement

namespace Th09EclRunLate
{


} // namespace Th09EclRunLate
