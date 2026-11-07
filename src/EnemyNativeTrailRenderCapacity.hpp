#pragma once

#include "EnemyManager.hpp"
#include <stddef.h>

// The original 96 consecutive history samples and 192 native ANM vertices
// occupy one EnemyView; each rendered strip point owns two vertices.
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
        TrailVertexCount = sizeof(((EnemyView *)0)->trailVertices3E68) /
                           sizeof(((EnemyView *)0)->trailVertices3E68[0]),
        VerticesPerSample = TrailVertexCount / TrailSampleCount
    };
};
typedef char EnemyNativeTrailRenderCapacityLayout[
    (EnemyNativeTrailRenderCapacity::DrawGroupCount == 4 &&
     EnemyNativeTrailRenderCapacity::SecondaryVmCount == 2 &&
     EnemyNativeTrailRenderCapacity::TrailSampleCount == 96 &&
     EnemyNativeTrailRenderCapacity::TrailVertexCount == 192 &&
     EnemyNativeTrailRenderCapacity::VerticesPerSample == 2 &&
     sizeof(EnemyTrailSampleView) == 0x1C &&
     sizeof(EnemyTrailVertexView) == 0x1C &&
     offsetof(EnemyView, trailVertices3E68) ==
         offsetof(EnemyView, trailSamples33E8) +
         sizeof(((EnemyView *)0)->trailSamples33E8)) ? 1 : -1];
