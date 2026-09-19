#include "PlayerLifecycleView.hpp"
#include "AsciiManager.hpp"
#include "EffectManager.hpp"
#include "RngRuntimeLeaves.hpp"
#include "ZunTimer.hpp"

#include <stddef.h>

struct PlayerRewardSharedRuntimeView
{
    int IsRewardBlocked();
};

struct PlayerRewardSupervisorView
{
    void SelectSide(int sideIndex);
};

struct PlayerRewardGameManagerView
{
    float TransformPopupX(float value);
    float TransformPopupY(float value);
};

struct PlayerRewardAttackTargetView
{
    int CheckMode(int mode);
    unsigned char unknown000[0x2AC3B8];
    int activeRewardCount2AC3B8;
};

struct PlayerRewardAttackOwnerView
{
    void Configure(int mode0, int mode1, int zero, const char *path);
};

struct PlayerRewardSideScoreView
{
    void AddScore(int score);
};

struct PlayerRewardTimerCurrentView
{
    int previous;
    float subFrame;
    int current;
    int GetCurrent();
};

struct PlayerRewardEffectView
{
    unsigned char unknown000[0xA0];
    float scaleA0;
    short valueA4;
    short valueA6;
    short sideA8;
};

typedef void (__fastcall *PlayerOwnerRewardCallback)(
    PlayerOwnerStateView *state, PlayerPositionView *position);

struct PlayerOwnerRewardStateView
{
    PlayerLifecycleView *owner00;
    int state04;
    int maxState08;
    int value0C;
    int maxValue10;
    ZunTimer timer14;
    ZunTimer timer20;
    int rewardMeter2C;
    int spawnCounter30;
    int value34;
    int state38;
    int activeFrameCounter3C;
    PlayerOwnerRewardCallback callback40;
};

typedef char PlayerOwnerRewardSizeIs44[
    (sizeof(PlayerOwnerRewardStateView) == 0x44) ? 1 : -1];
typedef char PlayerOwnerRewardTimer14At14[
    (offsetof(PlayerOwnerRewardStateView, timer14) == 0x14) ? 1 : -1];
typedef char PlayerOwnerRewardTimer20At20[
    (offsetof(PlayerOwnerRewardStateView, timer20) == 0x20) ? 1 : -1];
typedef char PlayerOwnerRewardMeterAt2C[
    (offsetof(PlayerOwnerRewardStateView, rewardMeter2C) == 0x2C) ? 1 : -1];
typedef char PlayerOwnerRewardCallbackAt40[
    (offsetof(PlayerOwnerRewardStateView, callback40) == 0x40) ? 1 : -1];

extern PlayerRewardSharedRuntimeView *g_PlayerSharedRuntime;
extern PlayerRewardSupervisorView g_PlayerSupervisorRuntime;
extern PlayerRewardGameManagerView g_PlayerGameManagerRuntime;
extern RngRuntimeView g_ReplayRng;
extern EffectManager *g_PlayerRewardEffectManager;
extern int g_PlayerRewardBaseValue;
extern int g_PlayerRewardModeValue;
extern float g_PlayerRewardVelocitySpan;
extern AsciiManager g_AsciiManager;

int PlayerOwnerStateView::ApplyReward(
    PlayerPositionView *position,
    int value0, int value1, int value2, int value3)
{
    PlayerOwnerRewardStateView *state =
        reinterpret_cast<PlayerOwnerRewardStateView *>(this);
    int oldValue = state->value0C;
    int blocked = g_PlayerSharedRuntime->IsRewardBlocked();
    if (blocked)
        return blocked;

    state->activeFrameCounter3C = 0;
    ++state->state04;
    if (state->maxState08 < state->state04)
        state->maxState08 = state->state04;

    state->value0C += value3 + value3 * g_PlayerRewardBaseValue / 10;
    if (state->value0C % 10 != 0)
        state->value0C = state->value0C - state->value0C % 10 + 10;

    if (state->value0C > 999990)
    {
        state->value0C = 999990;
        reinterpret_cast<PlayerRewardSideScoreView *>(state->owner00->sideState)
            ->AddScore(5000);
    }

    if (state->value0C >= 500000 && oldValue < 500000)
    {
        reinterpret_cast<PlayerRewardAttackOwnerView *>(state->owner00->opponentState->attackOwner14)->Configure(1, 1, 0, state->owner00->primaryShtFile->modePath1);
        if (!reinterpret_cast<PlayerRewardAttackTargetView *>(state->owner00->opponentState->attackTarget10)->CheckMode(3))
            reinterpret_cast<PlayerRewardAttackOwnerView *>(state->owner00->opponentState->attackOwner14)->Configure(2, 2, 0, state->owner00->primaryShtFile->modePath2);
    }
    else if (state->value0C >= 300000 && oldValue < 300000)
    {
        if (!reinterpret_cast<PlayerRewardAttackTargetView *>(state->owner00->opponentState->attackTarget10)->CheckMode(3))
            reinterpret_cast<PlayerRewardAttackOwnerView *>(state->owner00->opponentState->attackOwner14)->Configure(2, 2, 0, state->owner00->primaryShtFile->modePath2);
    }
    else if (state->value0C >= 100000 && oldValue < 100000)
    {
        reinterpret_cast<PlayerRewardAttackOwnerView *>(state->owner00->opponentState->attackOwner14)->Configure(2, 2, 0, state->owner00->primaryShtFile->modePath2);
    }

    if (state->value0C < 999990)
        value3 = state->value0C;
    else
        value3 = -1;

    if (state->state04 < 10)
    {
        if ((state->state04 % 2) == 0)
        {
            g_AsciiManager.CreateScorePopup(
                state->owner00->sideIndex,
                reinterpret_cast<Float3 *>(position),
                value3,
                static_cast<unsigned long>(
                    state->value0C >= 300000 ? -256 :
                    (state->value0C < 100000 ? -1 : -64)));
        }
    }
    else
    {
        int popupPeriod = state->state04 >= 30 ? 10 : 5;
        if ((state->state04 % popupPeriod) == 0)
        {
            g_AsciiManager.CreateScorePopup(
                state->owner00->sideIndex,
                reinterpret_cast<Float3 *>(position),
                value3,
                static_cast<unsigned long>(
                    state->value0C >= 300000 ? -256 :
                    (state->value0C < 100000 ? -1 : -64)));
        }
    }

    int adjustedValue0 = value0;
    if (value0 != 0)
    {
        if (g_PlayerRewardModeValue == 2)
            adjustedValue0 = value0 * 5 / 4;
        else if (g_PlayerRewardModeValue == 3)
            adjustedValue0 = value0 * 4 / 3;

        state->rewardMeter2C += adjustedValue0;
        state->spawnCounter30 += value1;
        state->value34 += value2;
        state->callback40(this, position);

        while (state->spawnCounter30 >=
               60 - 3 * g_PlayerRewardBaseValue / 2)
        {
            if (reinterpret_cast<PlayerRewardAttackTargetView *>(state->owner00->opponentState->attackTarget10)->activeRewardCount2AC3B8 < 25)
            {
                EffectFloat3 effectPosition;
                effectPosition.x =
                    g_PlayerGameManagerRuntime.TransformPopupX(position->x) +
                    g_ReplayRng.GetRandomF32SignedInRange(32.0f);
                effectPosition.y =
                    g_PlayerGameManagerRuntime.TransformPopupY(position->y) +
                    g_ReplayRng.GetRandomF32SignedInRange(32.0f);
                effectPosition.z = 0.0f;

                g_PlayerSupervisorRuntime.SelectSide(1 - state->owner00->sideIndex);

                EffectFloat3 velocity;
                velocity.x = g_ReplayRng.GetRandomF32SignedInRange(
                    g_PlayerRewardVelocitySpan * 0.5f - 8.0f);
                velocity.y = g_ReplayRng.GetRandomF32InRange(128.0f);
                velocity.z = 0.0f;

                PlayerRewardEffectView *effect =
                    reinterpret_cast<PlayerRewardEffectView *>(
                        g_PlayerRewardEffectManager->SpawnEffectWithVelocity(
                            state->owner00->sideIndex + 3, &effectPosition, &velocity,
                            1, static_cast<unsigned int>(-1)));

                g_PlayerSupervisorRuntime.SelectSide(state->owner00->sideIndex);
                effect->valueA4 = 0;
                effect->valueA6 = 1;
                effect->scaleA0 =
                    g_ReplayRng.GetRandomF32InRange(0.5f) *
                        0.016666668f +
                    g_PlayerRewardBaseValue * 0.0016666667f;
                effect->sideA8 = static_cast<short>(state->owner00->sideIndex);
            }

            state->spawnCounter30 += 2 * g_PlayerRewardBaseValue - 60;
            state->rewardMeter2C -= 10;
            if (state->rewardMeter2C < 0)
                state->rewardMeter2C = 0;
        }

        if (state->rewardMeter2C >=
            4 * (30 - g_PlayerRewardBaseValue))
        {
            do
            {
                EffectFloat3 effectPosition;
                effectPosition.x =
                    g_PlayerGameManagerRuntime.TransformPopupX(position->x) +
                    g_ReplayRng.GetRandomF32SignedInRange(32.0f);
                effectPosition.y =
                    g_PlayerGameManagerRuntime.TransformPopupY(position->y) +
                    g_ReplayRng.GetRandomF32SignedInRange(32.0f);
                effectPosition.z = 0.0f;

                g_PlayerSupervisorRuntime.SelectSide(1 - state->owner00->sideIndex);

                EffectFloat3 velocity;
                velocity.x = g_ReplayRng.GetRandomF32SignedInRange(
                    g_PlayerRewardVelocitySpan * 0.5f - 8.0f);
                velocity.y = g_ReplayRng.GetRandomF32InRange(128.0f);
                velocity.z = 0.0f;

                PlayerRewardEffectView *effect =
                    reinterpret_cast<PlayerRewardEffectView *>(
                        g_PlayerRewardEffectManager->SpawnEffectWithVelocity(
                            state->owner00->sideIndex + 1, &effectPosition, &velocity,
                            1, static_cast<unsigned int>(-1)));

                g_PlayerSupervisorRuntime.SelectSide(state->owner00->sideIndex);
                effect->valueA4 = 0;
                effect->valueA6 = 4;

                switch (g_PlayerRewardModeValue)
                {
                case 0:
                    effect->scaleA0 =
                        g_PlayerRewardBaseValue * 0.04f + 0.9f;
                    break;
                case 1:
                    effect->scaleA0 =
                        g_PlayerRewardBaseValue * 0.05f + 1.1f;
                    break;
                case 2:
                    effect->scaleA0 =
                        g_PlayerRewardBaseValue * 0.08f + 1.3f;
                    break;
                case 3:
                    effect->scaleA0 =
                        g_PlayerRewardBaseValue * 0.11f + 1.3f;
                    break;
                case 4:
                    effect->scaleA0 =
                        g_PlayerRewardBaseValue * 0.04f + 1.6f;
                    break;
                default:
                    break;
                }
                effect->sideA8 = static_cast<short>(state->owner00->sideIndex);

                state->rewardMeter2C +=
                    4 * g_PlayerRewardBaseValue - 120;
            }
            while (state->rewardMeter2C >=
                   4 * (30 - g_PlayerRewardBaseValue));
        }
    }

    if (state->timer20 < 30)
        state->timer20 += 30;
    else if (state->timer20 < 50)
        state->timer20 += 5;
    else if (state->timer20 < 60)
        state->timer20 += 2;

    int result = state->value0C;
    if (state->maxValue10 <= result)
    {
        state->maxValue10 = result;
        int current = reinterpret_cast<PlayerRewardTimerCurrentView *>(
            &state->timer20)->GetCurrent();
        state->timer14 = current + 45;
    }
    return result;
}
