#include "EffectRandomAxesRuntimeView.hpp"
#include "BulletSpawnDescriptorCtorView.hpp"
#include "PlayerSharedTransitionView.hpp"
#include "AsciiGameManagerView.hpp"
#include "BulletManager.hpp"
#include "RngRuntimeLeaves.hpp"
#include "SoundPlayer.hpp"
#include <stddef.h>

struct PlayerSharedRuntimeView;
extern PlayerSharedRuntimeView *g_PlayerSharedRuntime;
extern int g_PlayerRewardBaseValue;
extern RngRuntimeView g_Rng;

struct EffectSideCharacterView
{
    unsigned char unknown00[0x20];
    int characterIndex20;
};

int __fastcall EffectUpdateCallbackType1Type2(Effect *effect)
{
    EffectRandomAxesRuntimeView *state =
        reinterpret_cast<EffectRandomAxesRuntimeView *>(effect);
    ZunTimer &timer = effect->timer;
    if (state->delay98 != 0.0f)
    {
        effect->suppressDraw = 1;
        if (static_cast<float>(timer) >= state->delay98)
        {
            g_SoundPlayer.PlaySoundByIdx(46, effect->manager->sideIndex != 0 ? 500 : -500);
            state->delay98 = 0.0f;
            timer = 0;
            effect->suppressDraw = 0;
        }
    }

    else if (static_cast<float>(timer) < state->duration9C)
    {
        float t = static_cast<float>(timer) / state->duration9C;
        float twiceT = t + t;
        float minusOne = t - 1.0f;
        float a = (twiceT + 1.0f) * minusOne * minusOne;
        float b = (3.0f - twiceT) * t * t;
        float c = (1.0f - t) * (1.0f - t) * t;
        float d = minusOne * t * t;
        state->position0C.x = d * state->axis48.x + b * state->transformed60.x +
                              c * state->axis3C.x + a * state->base54.x;
        state->position0C.y = d * state->axis48.y + b * state->transformed60.y +
                              c * state->axis3C.y + a * state->base54.y;
    }

    else
    {
        if (reinterpret_cast<PlayerSharedTransitionView *>(g_PlayerSharedRuntime)
                ->GetTransitionBlockFlag() == 0)
        {
            BulletSpawnDescriptorCtorView storage;
            BulletSpawnDescriptor &descriptor =
                *reinterpret_cast<BulletSpawnDescriptor *>(&storage);
            descriptor.position = *reinterpret_cast<Float3 *>(&state->velocity18);
            if (state->levelAA >= 2 && g_Rng.GetRandomU32InRange(5) == 0)
                descriptor.bulletType = 3;
            else
                descriptor.bulletType = state->bulletTypeA4;
            descriptor.color = state->colorA6;
            descriptor.aimMode = 1;
            descriptor.count1 = 1;
            descriptor.count2 = 1;
            if (descriptor.bulletType == 3)
            {
                descriptor.angle = g_Rng.GetRandomF32SignedInRange(0.1308997f) + 1.5707964f;
            }
            else if (g_Rng.GetRandomU32InRange(3) != 0)
            {
                descriptor.color = 5;
                descriptor.aimMode = 0;
                descriptor.angle = g_Rng.GetRandomF32SignedInRange(0.22439948f);
            }
            else
            {
                descriptor.color = 3;
                descriptor.angle = g_Rng.GetRandomF32SignedInRange(0.2617994f) + 1.5707964f;
            }
            descriptor.speed1 = g_Rng.GetRandomF32SignedInRange(0.4f) + state->speedA0;
            if (reinterpret_cast<const EffectSideCharacterView *>(
                    &g_GameManager.sides[1 - state->sideIndexA8])->characterIndex20 == 10)
                descriptor.speed1 *= 0.93f;
            if (descriptor.bulletType == 3)
                descriptor.speed1 += 0.3f;
            float maximumSpeed = static_cast<float>(g_PlayerRewardBaseValue) * 0.1f + 3.0f;
            if (descriptor.speed1 >= maximumSpeed)
                descriptor.speed1 = maximumSpeed;
            else if (descriptor.speed1 < 0.8f)
                descriptor.speed1 = 0.8f;
            descriptor.extraAttribute20C = static_cast<unsigned char>(state->levelAA + 1);
            descriptor.angleStep = 0.0f;
            descriptor.speed2 = 0.0f;
            descriptor.unknown1FA = 0;
            descriptor.transformFlags = 4;
            g_GameManager.sides[1 - state->sideIndexA8].etama08
                ->SpawnBulletPatternPrimary(&descriptor);
        }
        return 0;
    }
    return 1;
}
