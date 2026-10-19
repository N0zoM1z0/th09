#include "EffectRandomAxesRuntimeView.hpp"
#include "AsciiGameManagerView.hpp"
#include "EnemyManager.hpp"
#include "EnemyManagerCardAttackSpawn.hpp"
#include "PlayerAngleView.hpp"
#include "PlayerSharedTransitionView.hpp"
#include "RngRuntimeLeaves.hpp"

struct PlayerSharedRuntimeView;
extern PlayerSharedRuntimeView *g_PlayerSharedRuntime;
extern RngRuntimeView g_Rng;

struct EffectTrailEnemyFlags
{
    unsigned int unknown00_05 : 6;
    unsigned int category : 3;
    unsigned int unknown09_31 : 23;
};

struct EffectTrailEnemyView
{
    unsigned char unknown0000[0x2DD4];
    PlayerAngleFloat3View worldPosition2DD4;
    float movementAngle2DE0;
    unsigned char unknown2DE4[0x2DF8 - 0x2DE4];
    float speed2DF8;
    unsigned char unknown2DFC[0x3380 - 0x2DFC];
    EffectTrailEnemyFlags flags3380;
};

typedef char EffectTrailEnemyWorldPositionAt2DD4[
    (offsetof(EffectTrailEnemyView, worldPosition2DD4) == 0x2DD4) ? 1 : -1];
typedef char EffectTrailEnemySpeedAt2DF8[
    (offsetof(EffectTrailEnemyView, speed2DF8) == 0x2DF8) ? 1 : -1];
typedef char EffectTrailEnemyFlagsAt3380[
    (offsetof(EffectTrailEnemyView, flags3380) == 0x3380 &&
     sizeof(EffectTrailEnemyFlags) == 4) ? 1 : -1];

static inline EnemySideStateView *EffectTrailOpponent(EffectRandomAxesRuntimeView *state)
{
    return reinterpret_cast<EnemySideStateView *>(
        &g_GameManager.sides[1 - state->sideIndexA8]);
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

    if (reinterpret_cast<PlayerSharedTransitionView *>(g_PlayerSharedRuntime)
            ->GetTransitionBlockFlag() == 0 &&
        EffectTrailOpponent(state)->enemyManager10->rewardEnemyCount2AC3B8 < 10)
    {
        EffectTrailEnemyView *enemy = reinterpret_cast<EffectTrailEnemyView *>(
            reinterpret_cast<EnemyManagerCardAttackSpawnView *>(
                EffectTrailOpponent(state)->enemyManager10)->SpawnEnemyRecord(
                    state->subroutineA4,
                    reinterpret_cast<CardAttackSpawnPosition *>(&state->velocity18),
                    20, -2, 1000, 0, 0));
        enemy->flags3380.category = state->colorA6;
        enemy->speed2DF8 += state->speedA0;
        int range = EffectTrailOpponent(state)->characterIndex20 != 7 ? 5 : 4;
        if (g_Rng.GetRandomU32InRange(range) != 0)
        {
            enemy->movementAngle2DE0 =
                g_Rng.GetRandomF32SignedInRange(0.015707964f) + 1.5707964f;
        }
        else
        {
            float angle = reinterpret_cast<PlayerAngleView *>(
                EffectTrailOpponent(state)->player04)->AngleToPoint(
                    &enemy->worldPosition2DD4);
            enemy->movementAngle2DE0 =
                angle + g_Rng.GetRandomF32SignedInRange(0.3926991f);
        }
    }
    return 0;
}
