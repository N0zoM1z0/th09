#include "EffectManager.hpp"
#include "AsciiManager.hpp"
#include "AnmCameraProjectionViews.hpp"
#include "RuntimeAngleMath.hpp"

int __fastcall EffectInitializeCallbackOrientedOffset(Effect *effect)
{
    EffectFloat3 offset;
    reinterpret_cast<Float3 *>(&offset)->FromAngleMagnitude(effect->velocity.x, 256.0f);
    effect->position.x += offset.x;
    effect->position.y += offset.y;
    reinterpret_cast<AnmVmProjectionView *>(effect->vms)->rotation.z =
        AddNormalizeAngle(effect->velocity.x, 1.5707964f);
    return 0;
}
