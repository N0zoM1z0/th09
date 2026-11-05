#include "RuntimePolarMath.hpp"
#include "EffectManager.hpp"
#include "AsciiManager.hpp"
#include "AnmCameraProjectionViews.hpp"
#include "RuntimeAngleMath.hpp"

int __fastcall EffectInitializeCallbackOrientedOffset(Effect *effect)
{
    EffectFloat3 offset;
    SetPolar2D(reinterpret_cast<Float2 *>(reinterpret_cast<Float3 *>(&offset)), effect->velocity.x, 256.0f);
    effect->position.x += offset.x;
    effect->position.y += offset.y;
    reinterpret_cast<AnmVmProjectionView *>(effect->vms)->rotation.z =
        AddNormalizeAngle(effect->velocity.x, 1.5707964f);
    return 0;
}
