#include "PlayerLifecycleView.hpp"
#include "EffectManager.hpp"
#include "GameConfiguration.hpp"
#include "AnmManager.hpp"
#include "AnmVmLifecycle.hpp"
#include "ZunTimer.hpp"

#include <stddef.h>

typedef unsigned short u16;

struct PlayerMovementReplayInputState
{
    u16 currentInput00;
    u16 word02;
    u16 repeatOutput04;
    u16 word06;
    u16 word08;
    unsigned char unknown0A[0x20];
    u16 auxiliary2A;
    u16 historyCurrent2C;
    unsigned char unknown2E[0x8E - 0x2E];

    u16 IsHeld(u16 mask);
};
typedef char PlayerMovementReplayInputSizeIs8E[
    (sizeof(PlayerMovementReplayInputState) == 0x8E) ? 1 : -1];

struct PlayerMovementSideView
{
    unsigned char unknown00[0x0C];
    EffectManager *effectManager0C;
    unsigned char unknown10[0x10];
    int shotType20;
};
typedef char PlayerMovementSideEffectAt0C[
    (offsetof(PlayerMovementSideView, effectManager0C) == 0x0C) ? 1 : -1];
typedef char PlayerMovementSideShotAt20[
    (offsetof(PlayerMovementSideView, shotType20) == 0x20) ? 1 : -1];

struct PlayerMovementShtView
{
    unsigned char unknown00[0x14];
    float normalAxisSpeed14;
    float focusedAxisSpeed18;
    float normalDiagonalSpeed1C;
    float focusedDiagonalSpeed20;
};
typedef char PlayerMovementShtFocusedAxisAt18[
    (offsetof(PlayerMovementShtView, focusedAxisSpeed18) == 0x18) ? 1 : -1];

struct PlayerMovementFields
{
    PlayerPositionView velocity1CCC;
    float movementAngle1CD8;
    float speedMultiplier1CDC;
    float speedMultiplier1CE0;
    float speedMultiplier1CE4;
    float speedMultiplier1CE8;
};
typedef char PlayerMovementFieldsSize20[
    (sizeof(PlayerMovementFields) == 0x20) ? 1 : -1];

struct PlayerMovementTailFields
{
    int movementDirection30358;
    float horizontalSpeed3035C;
    float verticalSpeed30360;
};
typedef char PlayerMovementTailFieldsSize0C[
    (sizeof(PlayerMovementTailFields) == 0x0C) ? 1 : -1];

typedef int (__fastcall *PlayerMovementOptionCallback)(
    PlayerLifecycleView *player, void *option);

struct PlayerMovementOptionView
{
    unsigned char vm00[0x2A4];
    unsigned char unknown2A4[0x3C];
    ZunTimer timer2E0;
    PlayerMovementOptionCallback updateCallback2EC;
    unsigned char unknown2F0[4];
};
typedef char PlayerMovementOptionSizeIs2F4[
    (sizeof(PlayerMovementOptionView) == 0x2F4) ? 1 : -1];
typedef char PlayerMovementOptionTimerAt2E0[
    (offsetof(PlayerMovementOptionView, timer2E0) == 0x2E0) ? 1 : -1];
typedef char PlayerMovementOptionCallbackAt2EC[
    (offsetof(PlayerMovementOptionView, updateCallback2EC) == 0x2EC) ? 1 : -1];

struct PlayerMovementSupervisorView
{
    unsigned char unknown000[0x5B8];
    float frameRateMultiplier5B8;
};

struct PlayerMovementEffectVmView
{
    void SetInterrupt(short interrupt);
};

extern GameConfiguration *g_GameConfiguration;
extern PlayerMovementReplayInputState g_ReplayInputStates[3];
extern int g_PlayerFocusEffectIds[];
extern float g_PlayerPlayfieldMinX;
extern float g_PlayerPlayfieldMinY;
extern float g_PlayerPlayfieldWidth;
extern float g_PlayerPlayfieldHeight;
extern PlayerMovementSupervisorView g_PlayerMovementSupervisor;
extern AnmManager *g_AnmManager;

static __inline Effect *&PlayerMovementFocusEffect(
    PlayerLifecycleView *player)
{
    return *reinterpret_cast<Effect **>(
        reinterpret_cast<unsigned char *>(player) + 0x364);
}

static __inline Effect *&PlayerMovementSecondaryEffect(
    PlayerLifecycleView *player)
{
    return *reinterpret_cast<Effect **>(
        reinterpret_cast<unsigned char *>(player) + 0x368);
}

static __inline PlayerMovementFields *PlayerMovementRuntime(
    PlayerLifecycleView *player)
{
    return reinterpret_cast<PlayerMovementFields *>(
        reinterpret_cast<unsigned char *>(player) + 0x1CCC);
}

static __inline PlayerMovementTailFields *PlayerMovementTail(
    PlayerLifecycleView *player)
{
    return reinterpret_cast<PlayerMovementTailFields *>(
        reinterpret_cast<unsigned char *>(player) + 0x30358);
}

static __inline PlayerMovementOptionView *PlayerMovementOptions(
    PlayerLifecycleView *player)
{
    return reinterpret_cast<PlayerMovementOptionView *>(
        reinterpret_cast<unsigned char *>(player) + 0x1CEC);
}

static __inline PlayerMovementShtView *PlayerMovementSht(
    PlayerLifecycleView *player)
{
    return reinterpret_cast<PlayerMovementShtView *>(player->primaryShtFile);
}

static __inline PlayerMovementSideView *PlayerMovementSide(
    PlayerLifecycleView *player)
{
    return reinterpret_cast<PlayerMovementSideView *>(player->sideState);
}

int PlayerLifecycleView::UpdateMovementAndOptions()
{
    PlayerLifecycleView *player = this;
    float horizontalSpeed = 0.0f;
    float verticalSpeed = 0.0f;

    if (player->header24.state84 == 0)
        return 0;

    int inputOffset = player->sideIndex * sizeof(PlayerMovementReplayInputState);
    PlayerMovementReplayInputState *input =
        reinterpret_cast<PlayerMovementReplayInputState *>(
            reinterpret_cast<unsigned char *>(g_ReplayInputStates) + inputOffset);

    if (input->IsHeld(0x50) == 0x50)
        player->movementDirection30358 = 5;
    else if (input->IsHeld(0x60) == 0x60)
        player->movementDirection30358 = 7;
    else if (input->IsHeld(0x90) == 0x90)
        player->movementDirection30358 = 6;
    else if (input->IsHeld(0xA0) == 0xA0)
        player->movementDirection30358 = 8;
    else if (input->IsHeld(0x20))
        player->movementDirection30358 = 2;
    else if (input->IsHeld(0x10))
        player->movementDirection30358 = 1;
    else if (input->IsHeld(0x40))
        player->movementDirection30358 = 3;
    else if (input->IsHeld(0x80))
        player->movementDirection30358 = 4;
    else
        player->movementDirection30358 = 0;

    int focused;
    if ((&g_GameConfiguration->valueB4)[player->sideIndex] == 1)
        focused =
            reinterpret_cast<PlayerMovementReplayInputState *>(
                reinterpret_cast<unsigned char *>(g_ReplayInputStates) +
                inputOffset)->auxiliary2A >= 8;
    else
        focused = input->IsHeld(4) != 0;

    player->flags1B80 ^=
        (player->flags1B80 ^ static_cast<unsigned int>(focused)) & 3u;

    PlayerMovementFields *runtime = PlayerMovementRuntime(player);

    if ((player->flags1B80 & 3u) != 0)
    {
        if (PlayerMovementFocusEffect(player) == NULL)
        {
            EffectFloat3 *effectPosition =
                reinterpret_cast<EffectFloat3 *>(&player->position1B88);
            // TH09 reloads the side after the first effect-manager call.
            PlayerMovementFocusEffect(player) =
                PlayerMovementSide(player)->effectManager0C->SpawnEffectInFixedSlot(
                    7,
                    effectPosition,
                    player->sideIndex,
                    static_cast<unsigned int>(-1));
            PlayerMovementSecondaryEffect(player) =
                PlayerMovementSide(player)->effectManager0C->SpawnEffectInFixedSlot(
                    g_PlayerFocusEffectIds[PlayerMovementSide(player)->shotType20],
                    effectPosition,
                    player->sideIndex + 2,
                    static_cast<unsigned int>(-1));
        }

        switch (player->movementDirection30358)
        {
        case 4:
        {
            PlayerMovementShtView *sht = PlayerMovementSht(player);
            runtime->movementAngle1CD8 = 0.0f;
            horizontalSpeed = sht->focusedAxisSpeed18;
            break;
        }
        case 3:
        {
            PlayerMovementShtView *sht = PlayerMovementSht(player);
            runtime->movementAngle1CD8 = 3.1415927f;
            horizontalSpeed = -sht->focusedAxisSpeed18;
            break;
        }
        case 1:
        {
            PlayerMovementShtView *sht = PlayerMovementSht(player);
            runtime->movementAngle1CD8 = -1.5707964f;
            verticalSpeed = -sht->focusedAxisSpeed18;
            break;
        }
        case 2:
        {
            PlayerMovementShtView *sht = PlayerMovementSht(player);
            runtime->movementAngle1CD8 = 1.5707964f;
            verticalSpeed = sht->focusedAxisSpeed18;
            break;
        }
        case 5:
        {
            PlayerMovementShtView *sht = PlayerMovementSht(player);
            runtime->movementAngle1CD8 = -2.3561945f;
            horizontalSpeed = -sht->focusedDiagonalSpeed20;
            verticalSpeed = horizontalSpeed;
            break;
        }
        case 7:
        {
            PlayerMovementShtView *sht = PlayerMovementSht(player);
            runtime->movementAngle1CD8 = 2.3561945f;
            verticalSpeed = sht->focusedDiagonalSpeed20;
            horizontalSpeed = -verticalSpeed;
            break;
        }
        case 6:
        {
            PlayerMovementShtView *sht = PlayerMovementSht(player);
            runtime->movementAngle1CD8 = -0.7853982f;
            horizontalSpeed = sht->focusedDiagonalSpeed20;
            verticalSpeed = -horizontalSpeed;
            break;
        }
        case 8:
        {
            PlayerMovementShtView *sht = PlayerMovementSht(player);
            runtime->movementAngle1CD8 = 0.7853982f;
            horizontalSpeed = sht->focusedDiagonalSpeed20;
            verticalSpeed = horizontalSpeed;
            break;
        }
        default:
            break;
        }
    }
    else
    {
        if (PlayerMovementFocusEffect(player) != NULL)
        {
            reinterpret_cast<PlayerMovementEffectVmView *>(
                PlayerMovementFocusEffect(player)->vms)
                ->SetInterrupt(1);
            PlayerMovementFocusEffect(player) = NULL;
            PlayerMovementSecondaryEffect(player)->active = 0;
            PlayerMovementSecondaryEffect(player) = NULL;
        }

        switch (player->movementDirection30358)
        {
        case 4:
        {
            PlayerMovementShtView *sht = PlayerMovementSht(player);
            runtime->movementAngle1CD8 = 0.0f;
            horizontalSpeed = sht->normalAxisSpeed14;
            break;
        }
        case 3:
        {
            PlayerMovementShtView *sht = PlayerMovementSht(player);
            runtime->movementAngle1CD8 = 3.1415927f;
            horizontalSpeed = -sht->normalAxisSpeed14;
            break;
        }
        case 1:
        {
            PlayerMovementShtView *sht = PlayerMovementSht(player);
            runtime->movementAngle1CD8 = -1.5707964f;
            verticalSpeed = -sht->normalAxisSpeed14;
            break;
        }
        case 2:
        {
            PlayerMovementShtView *sht = PlayerMovementSht(player);
            runtime->movementAngle1CD8 = 1.5707964f;
            verticalSpeed = sht->normalAxisSpeed14;
            break;
        }
        case 5:
        {
            PlayerMovementShtView *sht = PlayerMovementSht(player);
            runtime->movementAngle1CD8 = -2.3561945f;
            horizontalSpeed = -sht->normalDiagonalSpeed1C;
            verticalSpeed = horizontalSpeed;
            break;
        }
        case 7:
        {
            PlayerMovementShtView *sht = PlayerMovementSht(player);
            runtime->movementAngle1CD8 = 2.3561945f;
            verticalSpeed = sht->normalDiagonalSpeed1C;
            horizontalSpeed = -verticalSpeed;
            break;
        }
        case 6:
        {
            PlayerMovementShtView *sht = PlayerMovementSht(player);
            runtime->movementAngle1CD8 = -0.7853982f;
            horizontalSpeed = sht->normalDiagonalSpeed1C;
            verticalSpeed = -horizontalSpeed;
            break;
        }
        case 8:
        {
            PlayerMovementShtView *sht = PlayerMovementSht(player);
            runtime->movementAngle1CD8 = 0.7853982f;
            horizontalSpeed = sht->normalDiagonalSpeed1C;
            verticalSpeed = horizontalSpeed;
            break;
        }
        default:
            break;
        }
    }

    horizontalSpeed =
        runtime->speedMultiplier1CE4 *
        runtime->speedMultiplier1CDC *
        horizontalSpeed;
    verticalSpeed =
        runtime->speedMultiplier1CE8 *
        runtime->speedMultiplier1CE0 *
        verticalSpeed;

    // The second transition group reloads the ANM owner after the first call.
    if (horizontalSpeed < 0.0f && player->horizontalSpeed3035C >= 0.0f)
        reinterpret_cast<AnmLoaded *>(player->header24.anmFile98)->SetAndExecuteScriptIdx(
            reinterpret_cast<AnmVm *>(&player->mainVm), 1);
    else if (horizontalSpeed == 0.0f && player->horizontalSpeed3035C < 0.0f)
        reinterpret_cast<AnmLoaded *>(player->header24.anmFile98)->SetAndExecuteScriptIdx(
            reinterpret_cast<AnmVm *>(&player->mainVm), 2);

    if (horizontalSpeed > 0.0f && player->horizontalSpeed3035C <= 0.0f)
        reinterpret_cast<AnmLoaded *>(player->header24.anmFile98)->SetAndExecuteScriptIdx(
            reinterpret_cast<AnmVm *>(&player->mainVm), 3);
    else if (horizontalSpeed == 0.0f && player->horizontalSpeed3035C > 0.0f)
        reinterpret_cast<AnmLoaded *>(player->header24.anmFile98)->SetAndExecuteScriptIdx(
            reinterpret_cast<AnmVm *>(&player->mainVm), 4);

    player->horizontalSpeed3035C = horizontalSpeed;
    player->verticalSpeed30360 = verticalSpeed;

    runtime->velocity1CCC.x =
        horizontalSpeed * g_PlayerMovementSupervisor.frameRateMultiplier5B8;
    runtime->velocity1CCC.y =
        verticalSpeed * g_PlayerMovementSupervisor.frameRateMultiplier5B8;

    PlayerPositionView *position =
        reinterpret_cast<PlayerPositionView *>(
            player->position1B88.operator float *());
    position->x += runtime->velocity1CCC.x;
    position->y += runtime->velocity1CCC.y;

    if (position->x < g_PlayerPlayfieldMinX)
        position->x = g_PlayerPlayfieldMinX;
    else if (position->x > g_PlayerPlayfieldMinX + g_PlayerPlayfieldWidth)
        position->x = g_PlayerPlayfieldMinX + g_PlayerPlayfieldWidth;

    if (position->y < g_PlayerPlayfieldMinY)
        position->y = g_PlayerPlayfieldMinY;
    else if (position->y > g_PlayerPlayfieldMinY + g_PlayerPlayfieldHeight)
        position->y = g_PlayerPlayfieldMinY + g_PlayerPlayfieldHeight;

    player->collisionBoundsMin1C60 =
        player->position1B88 - player->hurtboxHalfSize;
    player->collisionBoundsMax1C6C =
        player->position1B88 + player->hurtboxHalfSize;
    player->grazeBoundsMin1C78 =
        player->position1B88 - player->grazeHalfSize;
    player->grazeBoundsMax1C84 =
        player->position1B88 + player->grazeHalfSize;
    player->unknownBoundsMin1C90 =
        player->position1B88 - player->itemCollectionHalfSize;
    player->unknownBoundsMax1C9C =
        player->position1B88 + player->itemCollectionHalfSize;

    PlayerMovementOptionView *option = PlayerMovementOptions(player);
    for (int optionCount = 4; optionCount != 0; --optionCount, ++option)
    {
        if (option->updateCallback2EC != NULL)
        {
            option->updateCallback2EC(player, option);
            g_AnmManager->ExecuteScript(
                reinterpret_cast<AnmVm *>(option));
            option->timer2E0++;
        }
    }

    if (verticalSpeed != 0.0f || horizontalSpeed != 0.0f)
    {
        for (int historyIndex = 15; historyIndex > 0; --historyIndex)
            player->positions1BA0[historyIndex] =
                player->positions1BA0[historyIndex - 1];
        player->positions1BA0[0] =
            *reinterpret_cast<PlayerConstructedVector3View *>(&player->position1B88);
    }

    return 0;
}
