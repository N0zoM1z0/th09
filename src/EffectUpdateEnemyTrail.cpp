#include "EffectRandomAxesRuntimeView.hpp"
#include "AsciiGameManagerView.hpp"
#include "EnemyManager.hpp"
#include "EnemyManagerCardAttackSpawn.hpp"
#include "PlayerAngleView.hpp"
#include "FrontFactoryConstructView.hpp"
#include "RngRuntimeLeaves.hpp"

struct PlayerSharedRuntimeView;
extern PlayerSharedRuntimeView *g_PlayerSharedRuntime;
extern RngRuntimeView g_Rng;

// The target callback operates on the actual 0x5430-byte Enemy pool.
// Its old private +0x2DF8 field called "speed" is the native acceleration
// member. The original bitfield at +0x3380 bits 6..8 shares the native
// secondary flag allocation without changing the other protocol bits.
typedef char EffectTrailNativeEnemyStorage[
    (sizeof(EnemyView) == 0x5430 &&
     offsetof(EnemyView, worldPosition2DD4) == 0x2DD4 &&
     offsetof(EnemyView, movementAngle2DE0) == 0x2DE0 &&
     offsetof(EnemyView, acceleration2DF8) == 0x2DF8 &&
     offsetof(EnemyView, secondaryFlags3380) == 0x3380 &&
     sizeof(Th09EclEnemySecondaryFlagBits) == 4 &&
     sizeof(PlayerAngleFloat3View) == sizeof(EnemyFloat3)) ? 1 : -1];

static inline AsciiGameManagerSideView *EffectTrailOpponent(
    EffectRandomAxesRuntimeView *state)
{
    return &g_GameManager.sides[1 - state->sideIndexA8];
}

int __fastcall EffectUpdateCallbackType3Type4(Effect *effect)
{
    EffectRandomAxesRuntimeView *state =
        reinterpret_cast<EffectRandomAxesRuntimeView *>(effect);
    ZunTimer &timer = effect->timer;
    if (static_cast<float>(timer) < state->duration9C)
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
        return 1;
    }

    if (g_GameManager.frontFactoryA8->GetTransitionBlockFlag() == 0 &&
        EffectTrailOpponent(state)->enemyManager10->rewardEnemyCount2AC3B8 < 10)
    {
        EnemyView *enemy = reinterpret_cast<EnemyView *>(
            static_cast<EnemyManagerCardAttackSpawnView *>(
                EffectTrailOpponent(state)->enemyManager10)->SpawnEnemyRecord(
                    state->subroutineA4,
                    reinterpret_cast<CardAttackSpawnPosition *>(&state->velocity18),
                    20, -2, 1000, 0, 0));
        enemy->secondaryFlagBits3380.effectCategory06_08 = state->colorA6;
        enemy->acceleration2DF8 += state->speedA0;
        int range = EffectTrailOpponent(state)->character20 != 7 ? 5 : 4;
        if (g_Rng.GetRandomU32InRange(range) != 0)
        {
            enemy->movementAngle2DE0 =
                g_Rng.GetRandomF32SignedInRange(0.015707964f) + 1.5707964f;
        }
        else
        {
            float angle = reinterpret_cast<PlayerAngleView *>(
                EffectTrailOpponent(state)->player04)->AngleToPoint(
                    reinterpret_cast<PlayerAngleFloat3View *>(&enemy->worldPosition2DD4));
            enemy->movementAngle2DE0 =
                angle + g_Rng.GetRandomF32SignedInRange(0.3926991f);
        }
    }
    return 0;
}
