#include "ExAttackController.hpp"
#include "PlayerLifecycleView.hpp"

#include <stddef.h>

struct PlayerOwnerRewardCallbackStateView
{
    PlayerLifecycleView *owner00;
    unsigned char unknown04[0x2C];
    int value30;
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

int __fastcall PlayerOwnerRewardCallbackType5(
    PlayerOwnerStateView *base, PlayerPositionView *position)
{
    PlayerOwnerRewardCallbackStateView *state =
        reinterpret_cast<PlayerOwnerRewardCallbackStateView *>(base);

    while (state->value34 >= 25 - g_PlayerRewardBaseValue / 2)
    {
        g_ExAttackController->Spawn(
            5,
            reinterpret_cast<const Float3 *>(position),
            state->owner00->sideIndex,
            0);
        state->value34 += g_PlayerRewardBaseValue / 2 - 25;
    }
    return 0;
}

int __fastcall PlayerOwnerRewardCallbackType6(
    PlayerOwnerStateView *base, PlayerPositionView *position)
{
    PlayerOwnerRewardCallbackStateView *state =
        reinterpret_cast<PlayerOwnerRewardCallbackStateView *>(base);

    while (state->value34 >= 100 - 3 * g_PlayerRewardBaseValue)
    {
        g_ExAttackController->Spawn(
            15,
            reinterpret_cast<const Float3 *>(position),
            state->owner00->sideIndex,
            0);
        state->value34 += 5 * g_PlayerRewardBaseValue - 140;
    }
    return 0;
}

int __fastcall PlayerOwnerRewardCallbackType7(
    PlayerOwnerStateView *base, PlayerPositionView *position)
{
    PlayerOwnerRewardCallbackStateView *state =
        reinterpret_cast<PlayerOwnerRewardCallbackStateView *>(base);

    while (state->value34 >= 200 - 3 * g_PlayerRewardBaseValue)
    {
        g_ExAttackController->Spawn(
            6,
            reinterpret_cast<const Float3 *>(position),
            state->owner00->sideIndex,
            0);
        int oldValue30 = state->value30;
        state->value34 += 3 * g_PlayerRewardBaseValue - 200;
        state->value30 = oldValue30 - 5;
        if (state->value30 < 0)
            state->value30 = 0;
    }
    return 0;
}

int __fastcall PlayerOwnerRewardCallbackType8(
    PlayerOwnerStateView *base, PlayerPositionView *position)
{
    PlayerOwnerRewardCallbackStateView *state =
        reinterpret_cast<PlayerOwnerRewardCallbackStateView *>(base);

    while (state->value34 >= 100 - 3 * g_PlayerRewardBaseValue)
    {
        g_ExAttackController->Spawn(
            16,
            reinterpret_cast<const Float3 *>(position),
            state->owner00->sideIndex,
            0);
        state->value34 += 3 * g_PlayerRewardBaseValue - 100;
    }
    return 0;
}

int __fastcall PlayerOwnerRewardCallbackType9(
    PlayerOwnerStateView *base, PlayerPositionView *position)
{
    PlayerOwnerRewardCallbackStateView *state =
        reinterpret_cast<PlayerOwnerRewardCallbackStateView *>(base);

    while (state->value34 >= 2 * (30 - g_PlayerRewardBaseValue))
    {
        g_ExAttackController->Spawn(
            17,
            reinterpret_cast<const Float3 *>(position),
            state->owner00->sideIndex,
            0);
        state->value34 += 2 * g_PlayerRewardBaseValue - 60;
    }
    return 0;
}

int __fastcall PlayerOwnerRewardCallbackType10(
    PlayerOwnerStateView *base, PlayerPositionView *position)
{
    PlayerOwnerRewardCallbackStateView *state =
        reinterpret_cast<PlayerOwnerRewardCallbackStateView *>(base);

    while (state->value34 >= 4 * (25 - g_PlayerRewardBaseValue))
    {
        g_ExAttackController->Spawn(
            18,
            reinterpret_cast<const Float3 *>(position),
            state->owner00->sideIndex,
            0);
        state->value34 += 4 * g_PlayerRewardBaseValue - 100;
    }
    return 0;
}

int __fastcall PlayerOwnerRewardCallbackType11(
    PlayerOwnerStateView *base, PlayerPositionView *position)
{
    PlayerOwnerRewardCallbackStateView *state =
        reinterpret_cast<PlayerOwnerRewardCallbackStateView *>(base);

    while (state->value34 >= 5 * (28 - g_PlayerRewardBaseValue))
    {
        g_ExAttackController->Spawn(
            19,
            reinterpret_cast<const Float3 *>(position),
            state->owner00->sideIndex,
            0);
        state->value34 += 5 * g_PlayerRewardBaseValue - 140;
    }
    return 0;
}

int __fastcall PlayerOwnerRewardCallbackType12(
    PlayerOwnerStateView *base, PlayerPositionView *position)
{
    PlayerOwnerRewardCallbackStateView *state =
        reinterpret_cast<PlayerOwnerRewardCallbackStateView *>(base);

    while (state->value34 >= 4 * (40 - g_PlayerRewardBaseValue))
    {
        g_ExAttackController->Spawn(
            20,
            reinterpret_cast<const Float3 *>(position),
            state->owner00->sideIndex,
            0);
        state->value34 += 4 * g_PlayerRewardBaseValue - 160;
    }
    return 0;
}

int __fastcall PlayerOwnerRewardCallbackType13(
    PlayerOwnerStateView *base, PlayerPositionView *position)
{
    PlayerOwnerRewardCallbackStateView *state =
        reinterpret_cast<PlayerOwnerRewardCallbackStateView *>(base);

    while (state->value34 >= 5 * (28 - g_PlayerRewardBaseValue))
    {
        g_ExAttackController->Spawn(
            21,
            reinterpret_cast<const Float3 *>(position),
            state->owner00->sideIndex,
            0);
        state->value34 += 5 * g_PlayerRewardBaseValue - 140;
    }
    return 0;
}

int __fastcall PlayerOwnerRewardCallbackType14(
    PlayerOwnerStateView *base, PlayerPositionView *position)
{
    PlayerOwnerRewardCallbackStateView *state =
        reinterpret_cast<PlayerOwnerRewardCallbackStateView *>(base);

    while (state->value34 >= 100 - 3 * g_PlayerRewardBaseValue)
    {
        g_ExAttackController->Spawn(
            25,
            reinterpret_cast<const Float3 *>(position),
            state->owner00->sideIndex,
            0);
        state->value34 += 5 * g_PlayerRewardBaseValue - 140;
    }
    return 0;
}

int __fastcall PlayerOwnerRewardCallbackType15(
    PlayerOwnerStateView *base, PlayerPositionView *position)
{
    PlayerOwnerRewardCallbackStateView *state =
        reinterpret_cast<PlayerOwnerRewardCallbackStateView *>(base);

    while (state->value34 >= 80 - 3 * g_PlayerRewardBaseValue)
    {
        g_ExAttackController->Spawn(
            26,
            reinterpret_cast<const Float3 *>(position),
            state->owner00->sideIndex,
            0);
        state->value34 += 3 * g_PlayerRewardBaseValue - 80;
    }
    return 0;
}
