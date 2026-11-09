#pragma once

#include "EnemyManager.hpp"
#include <stddef.h>

// The original 96 history samples render as 192 ANM vertices (two per
// sample), but the Enemy constructor actually constructs 194 vertices.
// The final two are allocated/constructed tail sentinels, not extra
// samples to render; do not conflate render capacity with owner extent.
struct EnemyNativeTrailRenderCapacity
{
    enum
    {
        DrawGroupCount = sizeof(((EnemyManagerView *)0)->drawGroupHeads2AC410) /
                         sizeof(((EnemyManagerView *)0)->drawGroupHeads2AC410[0]),
        SecondaryVmCount = sizeof(((EnemyView *)0)->secondaryVms2AC) /
                           sizeof(((EnemyView *)0)->secondaryVms2AC[0]),
        TrailSampleCount = sizeof(((EnemyView *)0)->trailSamples33E8) /
                           sizeof(((EnemyView *)0)->trailSamples33E8[0]),
        ConstructedTrailVertexCount = sizeof(((EnemyView *)0)->trailVertices3E68) /
                                      sizeof(((EnemyView *)0)->trailVertices3E68[0]),
        TrailVertexCount = TrailSampleCount * 2,
        VerticesPerSample = TrailVertexCount / TrailSampleCount
    };
};
typedef char EnemyNativeTrailRenderCapacityLayout[
    (EnemyNativeTrailRenderCapacity::DrawGroupCount == 4 &&
     EnemyNativeTrailRenderCapacity::SecondaryVmCount == 2 &&
     EnemyNativeTrailRenderCapacity::TrailSampleCount == 96 &&
     EnemyNativeTrailRenderCapacity::TrailVertexCount == 192 &&
     EnemyNativeTrailRenderCapacity::ConstructedTrailVertexCount == 194 &&
     EnemyNativeTrailRenderCapacity::VerticesPerSample == 2 &&
     sizeof(EnemyTrailSampleView) == 0x1C &&
     sizeof(EnemyTrailVertexView) == 0x1C &&
     offsetof(EnemyView, trailVertices3E68) ==
         offsetof(EnemyView, trailSamples33E8) +
         sizeof(((EnemyView *)0)->trailSamples33E8)) ? 1 : -1];
