#include "ExAttackController.hpp"
#include "PlayerLifecycleView.hpp"

#include <stddef.h>
#include "GameManagerRuntimeView.hpp"

// Reward callbacks receive the real Player +0x30410 owner-state window.
// Its +0x30 spawn counter and +0x34 accumulated reward are shared with
// exact ApplyReward; there is no separate callback state allocation.
typedef char PlayerRewardCallbackNativeStateAt34[
    (sizeof(PlayerOwnerStateView) == 0x44 &&
     offsetof(PlayerOwnerStateView, playerOwner00) == 0x00 &&
     offsetof(PlayerOwnerStateView, spawnCounter30) == 0x30 &&
     offsetof(PlayerOwnerStateView, value34) == 0x34) ? 1 : -1];


int __fastcall PlayerOwnerRewardCallbackType1(
    PlayerOwnerStateView *base, PlayerPositionView *position)
{
    PlayerOwnerStateView *state = base;

    while (state->value34 >= 3 * (40 - g_GameManager.valueB4))
    {
        g_GameManager.exAttackControllerAC->Spawn(
            2,
            reinterpret_cast<const Float3 *>(position),
            state->playerOwner00->sideIndex,
            0);
        state->value34 += 4 * g_GameManager.valueB4 - 120;
    }
    return 0;
}

int __fastcall PlayerOwnerRewardCallbackType2(
    PlayerOwnerStateView *base, PlayerPositionView *position)
{
    PlayerOwnerStateView *state = base;

    while (state->value34 >= 2 * (45 - g_GameManager.valueB4))
    {
        g_GameManager.exAttackControllerAC->Spawn(
            3,
            reinterpret_cast<const Float3 *>(position),
            state->playerOwner00->sideIndex,
            0);
        state->value34 += 2 * g_GameManager.valueB4 - 90;
    }
    return 0;
}

int __fastcall PlayerOwnerRewardCallbackType3(
    PlayerOwnerStateView *base, PlayerPositionView *position)
{
    PlayerOwnerStateView *state = base;

    while (state->value34 >= 70 - 3 * g_GameManager.valueB4)
    {
        g_GameManager.exAttackControllerAC->Spawn(
            13,
            reinterpret_cast<const Float3 *>(position),
            state->playerOwner00->sideIndex,
            0);
        state->value34 -= 70;
    }
    return 0;
}

int __fastcall PlayerOwnerRewardCallbackType4(
    PlayerOwnerStateView *base, PlayerPositionView *position)
{
    PlayerOwnerStateView *state = base;

    while (state->value34 >= 5 * (28 - g_GameManager.valueB4))
    {
        g_GameManager.exAttackControllerAC->Spawn(
            14,
            reinterpret_cast<const Float3 *>(position),
            state->playerOwner00->sideIndex,
            0);
        state->value34 += 5 * g_GameManager.valueB4 - 140;
    }
    return 0;
}

int __fastcall PlayerOwnerRewardCallbackType5(
    PlayerOwnerStateView *base, PlayerPositionView *position)
{
    PlayerOwnerStateView *state = base;

    while (state->value34 >= 25 - g_GameManager.valueB4 / 2)
    {
        g_GameManager.exAttackControllerAC->Spawn(
            5,
            reinterpret_cast<const Float3 *>(position),
            state->playerOwner00->sideIndex,
            0);
        state->value34 += g_GameManager.valueB4 / 2 - 25;
    }
    return 0;
}

int __fastcall PlayerOwnerRewardCallbackType6(
    PlayerOwnerStateView *base, PlayerPositionView *position)
{
    PlayerOwnerStateView *state = base;

    while (state->value34 >= 100 - 3 * g_GameManager.valueB4)
    {
        g_GameManager.exAttackControllerAC->Spawn(
            15,
            reinterpret_cast<const Float3 *>(position),
            state->playerOwner00->sideIndex,
            0);
        state->value34 += 5 * g_GameManager.valueB4 - 140;
    }
    return 0;
}

int __fastcall PlayerOwnerRewardCallbackType7(
    PlayerOwnerStateView *base, PlayerPositionView *position)
{
    PlayerOwnerStateView *state = base;

    while (state->value34 >= 200 - 3 * g_GameManager.valueB4)
    {
        g_GameManager.exAttackControllerAC->Spawn(
            6,
            reinterpret_cast<const Float3 *>(position),
            state->playerOwner00->sideIndex,
            0);
        int oldValue30 = state->spawnCounter30;
        state->value34 += 3 * g_GameManager.valueB4 - 200;
        state->spawnCounter30 = oldValue30 - 5;
        if (state->spawnCounter30 < 0)
            state->spawnCounter30 = 0;
    }
    return 0;
}

int __fastcall PlayerOwnerRewardCallbackType8(
    PlayerOwnerStateView *base, PlayerPositionView *position)
{
    PlayerOwnerStateView *state = base;

    while (state->value34 >= 100 - 3 * g_GameManager.valueB4)
    {
        g_GameManager.exAttackControllerAC->Spawn(
            16,
            reinterpret_cast<const Float3 *>(position),
            state->playerOwner00->sideIndex,
            0);
        state->value34 += 3 * g_GameManager.valueB4 - 100;
    }
    return 0;
}

int __fastcall PlayerOwnerRewardCallbackType9(
    PlayerOwnerStateView *base, PlayerPositionView *position)
{
    PlayerOwnerStateView *state = base;

    while (state->value34 >= 2 * (30 - g_GameManager.valueB4))
    {
        g_GameManager.exAttackControllerAC->Spawn(
            17,
            reinterpret_cast<const Float3 *>(position),
            state->playerOwner00->sideIndex,
            0);
        state->value34 += 2 * g_GameManager.valueB4 - 60;
    }
    return 0;
}

int __fastcall PlayerOwnerRewardCallbackType10(
    PlayerOwnerStateView *base, PlayerPositionView *position)
{
    PlayerOwnerStateView *state = base;

    while (state->value34 >= 4 * (25 - g_GameManager.valueB4))
    {
        g_GameManager.exAttackControllerAC->Spawn(
            18,
            reinterpret_cast<const Float3 *>(position),
            state->playerOwner00->sideIndex,
            0);
        state->value34 += 4 * g_GameManager.valueB4 - 100;
    }
    return 0;
}

int __fastcall PlayerOwnerRewardCallbackType11(
    PlayerOwnerStateView *base, PlayerPositionView *position)
{
    PlayerOwnerStateView *state = base;

    while (state->value34 >= 5 * (28 - g_GameManager.valueB4))
    {
        g_GameManager.exAttackControllerAC->Spawn(
            19,
            reinterpret_cast<const Float3 *>(position),
            state->playerOwner00->sideIndex,
            0);
        state->value34 += 5 * g_GameManager.valueB4 - 140;
    }
    return 0;
}

int __fastcall PlayerOwnerRewardCallbackType12(
    PlayerOwnerStateView *base, PlayerPositionView *position)
{
    PlayerOwnerStateView *state = base;

    while (state->value34 >= 4 * (40 - g_GameManager.valueB4))
    {
        g_GameManager.exAttackControllerAC->Spawn(
            20,
            reinterpret_cast<const Float3 *>(position),
            state->playerOwner00->sideIndex,
            0);
        state->value34 += 4 * g_GameManager.valueB4 - 160;
    }
    return 0;
}

int __fastcall PlayerOwnerRewardCallbackType13(
    PlayerOwnerStateView *base, PlayerPositionView *position)
{
    PlayerOwnerStateView *state = base;

    while (state->value34 >= 5 * (28 - g_GameManager.valueB4))
    {
        g_GameManager.exAttackControllerAC->Spawn(
            21,
            reinterpret_cast<const Float3 *>(position),
            state->playerOwner00->sideIndex,
            0);
        state->value34 += 5 * g_GameManager.valueB4 - 140;
    }
    return 0;
}

int __fastcall PlayerOwnerRewardCallbackType14(
    PlayerOwnerStateView *base, PlayerPositionView *position)
{
    PlayerOwnerStateView *state = base;

    while (state->value34 >= 100 - 3 * g_GameManager.valueB4)
    {
        g_GameManager.exAttackControllerAC->Spawn(
            25,
            reinterpret_cast<const Float3 *>(position),
            state->playerOwner00->sideIndex,
            0);
        state->value34 += 5 * g_GameManager.valueB4 - 140;
    }
    return 0;
}

int __fastcall PlayerOwnerRewardCallbackType15(
    PlayerOwnerStateView *base, PlayerPositionView *position)
{
    PlayerOwnerStateView *state = base;

    while (state->value34 >= 80 - 3 * g_GameManager.valueB4)
    {
        g_GameManager.exAttackControllerAC->Spawn(
            26,
            reinterpret_cast<const Float3 *>(position),
            state->playerOwner00->sideIndex,
            0);
        state->value34 += 3 * g_GameManager.valueB4 - 80;
    }
    return 0;
}

int __fastcall PlayerOwnerRewardCallbackType0(
    PlayerOwnerStateView *base, PlayerPositionView *position)
{
    PlayerOwnerStateView *state = base;

    while (state->value34 >= 5 * (28 - g_GameManager.valueB4))
    {
        g_GameManager.exAttackControllerAC->Spawn(
            0,
            reinterpret_cast<const Float3 *>(position),
            state->playerOwner00->sideIndex,
            0);
        state->value34 += 5 * g_GameManager.valueB4 - 140;
    }
    return 0;
}
