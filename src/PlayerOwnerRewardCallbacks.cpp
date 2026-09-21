#include "ExAttackController.hpp"
#include "PlayerLifecycleView.hpp"

#include <stddef.h>

struct PlayerOwnerRewardCallbackStateView
{
    PlayerLifecycleView *owner00;
    unsigned char unknown04[0x30];
    int value34;
};

typedef char PlayerOwnerRewardCallbackValue34At34[
    (offsetof(PlayerOwnerRewardCallbackStateView, value34) == 0x34) ? 1 : -1];

extern ExAttackController *g_ExAttackController;
extern int g_PlayerRewardBaseValue;

int __fastcall PlayerOwnerRewardCallbackType1(
    PlayerOwnerStateView *base, PlayerPositionView *position)
{
    PlayerOwnerRewardCallbackStateView *state =
        reinterpret_cast<PlayerOwnerRewardCallbackStateView *>(base);

    while (state->value34 >= 3 * (40 - g_PlayerRewardBaseValue))
    {
        g_ExAttackController->Spawn(
            2,
            reinterpret_cast<const Float3 *>(position),
            state->owner00->sideIndex,
            0);
        state->value34 += 4 * g_PlayerRewardBaseValue - 120;
    }
    return 0;
}

int __fastcall PlayerOwnerRewardCallbackType2(
    PlayerOwnerStateView *base, PlayerPositionView *position)
{
    PlayerOwnerRewardCallbackStateView *state =
        reinterpret_cast<PlayerOwnerRewardCallbackStateView *>(base);

    while (state->value34 >= 2 * (45 - g_PlayerRewardBaseValue))
    {
        g_ExAttackController->Spawn(
            3,
            reinterpret_cast<const Float3 *>(position),
            state->owner00->sideIndex,
            0);
        state->value34 += 2 * g_PlayerRewardBaseValue - 90;
    }
    return 0;
}

int __fastcall PlayerOwnerRewardCallbackType3(
    PlayerOwnerStateView *base, PlayerPositionView *position)
{
    PlayerOwnerRewardCallbackStateView *state =
        reinterpret_cast<PlayerOwnerRewardCallbackStateView *>(base);

    while (state->value34 >= 70 - 3 * g_PlayerRewardBaseValue)
    {
        g_ExAttackController->Spawn(
            13,
            reinterpret_cast<const Float3 *>(position),
            state->owner00->sideIndex,
            0);
        state->value34 -= 70;
    }
    return 0;
}

int __fastcall PlayerOwnerRewardCallbackType4(
    PlayerOwnerStateView *base, PlayerPositionView *position)
{
    PlayerOwnerRewardCallbackStateView *state =
        reinterpret_cast<PlayerOwnerRewardCallbackStateView *>(base);

    while (state->value34 >= 5 * (28 - g_PlayerRewardBaseValue))
    {
        g_ExAttackController->Spawn(
            14,
            reinterpret_cast<const Float3 *>(position),
            state->owner00->sideIndex,
            0);
        state->value34 += 5 * g_PlayerRewardBaseValue - 140;
    }
    return 0;
}
