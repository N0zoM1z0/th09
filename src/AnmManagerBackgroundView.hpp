#pragma once

#include "AnmManagerRenderCacheView.hpp"

// Original ANM Background and Title cache are this same manager's prefix.
// The retained typedef preserves local source spellings, not a new allocation.
typedef AnmManagerRenderCacheView AnmManagerBackgroundView;

typedef char AnmBackgroundCameraModeAt12888[
    (offsetof(AnmManagerBackgroundView, cameraMode) == 0x12888) ? 1 : -1];
typedef char AnmNativeCurrentTextureAt12880[
    (offsetof(AnmManagerBackgroundView, currentTexture) == 0x12880) ? 1 : -1];
