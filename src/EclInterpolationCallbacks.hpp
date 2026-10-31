#pragma once

struct EnemyView;
struct Th09EclInterpolationSlotView;

// Original call transport: Enemy in ECX, slot in EDX, progress on the stack.
// Shared by the table definition, slot storage and RunEcl indirect caller.
typedef void (__fastcall *Th09EclInterpolationCallback)(
    EnemyView *enemy, Th09EclInterpolationSlotView *slot, float progress);

typedef char Th09EclInterpolationCallbackSizeIs4[
    sizeof(Th09EclInterpolationCallback) == 4 ? 1 : -1];

namespace Th09EclRunControl
{

// The eight reviewed slots are defined with their actual native functions.
// This unsized declaration does not infer the original array's declared bound.
extern Th09EclInterpolationCallback g_EclInterpolationCallbacks[];

} // namespace Th09EclRunControl
