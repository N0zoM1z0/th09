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

    PlayerMovementTailFields *tail = PlayerMovementTail(player);
    int inputOffset = player->sideIndex * sizeof(PlayerMovementReplayInputState);
    PlayerMovementReplayInputState *input =
        reinterpret_cast<PlayerMovementReplayInputState *>(
            reinterpret_cast<unsigned char *>(g_ReplayInputStates) + inputOffset);

    if (input->IsHeld(0x50) == 0x50)
        tail->movementDirection30358 = 5;
    else if (input->IsHeld(0x60) == 0x60)
        tail->movementDirection30358 = 7;
    else if (input->IsHeld(0x90) == 0x90)
        tail->movementDirection30358 = 6;
    else if (input->IsHeld(0xA0) == 0xA0)
        tail->movementDirection30358 = 8;
    else if (input->IsHeld(0x20))
        tail->movementDirection30358 = 2;
    else if (input->IsHeld(0x10))
        tail->movementDirection30358 = 1;
    else if (input->IsHeld(0x40))
        tail->movementDirection30358 = 3;
    else if (input->IsHeld(0x80))
        tail->movementDirection30358 = 4;
    else
        tail->movementDirection30358 = 0;

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
            PlayerMovementSideView *side = PlayerMovementSide(player);
            PlayerMovementFocusEffect(player) =
                side->effectManager0C->SpawnEffectInFixedSlot(
                    7,
                    reinterpret_cast<EffectFloat3 *>(&player->position1B88),
                    player->sideIndex,
                    static_cast<unsigned int>(-1));
            PlayerMovementSecondaryEffect(player) =
                side->effectManager0C->SpawnEffectInFixedSlot(
                    g_PlayerFocusEffectIds[side->shotType20],
                    reinterpret_cast<EffectFloat3 *>(&player->position1B88),
                    player->sideIndex + 2,
                    static_cast<unsigned int>(-1));
        }

        switch (tail->movementDirection30358)
        {
        case 4:
            horizontalSpeed = PlayerMovementSht(player)->focusedAxisSpeed18;
            runtime->movementAngle1CD8 = 0.0f;
            break;
        case 3:
            horizontalSpeed = -PlayerMovementSht(player)->focusedAxisSpeed18;
            runtime->movementAngle1CD8 = 3.1415927f;
            break;
        case 1:
            verticalSpeed = -PlayerMovementSht(player)->focusedAxisSpeed18;
            runtime->movementAngle1CD8 = -1.5707964f;
            break;
        case 2:
            verticalSpeed = PlayerMovementSht(player)->focusedAxisSpeed18;
            runtime->movementAngle1CD8 = 1.5707964f;
            break;
        case 5:
            horizontalSpeed = -PlayerMovementSht(player)->focusedDiagonalSpeed20;
            verticalSpeed = horizontalSpeed;
            runtime->movementAngle1CD8 = -2.3561945f;
            break;
        case 7:
            verticalSpeed = PlayerMovementSht(player)->focusedDiagonalSpeed20;
            horizontalSpeed = -verticalSpeed;
            runtime->movementAngle1CD8 = 2.3561945f;
            break;
        case 6:
            horizontalSpeed = PlayerMovementSht(player)->focusedDiagonalSpeed20;
            verticalSpeed = -horizontalSpeed;
            runtime->movementAngle1CD8 = -0.7853982f;
            break;
        case 8:
            horizontalSpeed = PlayerMovementSht(player)->focusedDiagonalSpeed20;
            verticalSpeed = horizontalSpeed;
            runtime->movementAngle1CD8 = 0.7853982f;
            break;
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

        switch (tail->movementDirection30358)
        {
        case 4:
            horizontalSpeed = PlayerMovementSht(player)->normalAxisSpeed14;
            runtime->movementAngle1CD8 = 0.0f;
            break;
        case 3:
            horizontalSpeed = -PlayerMovementSht(player)->normalAxisSpeed14;
            runtime->movementAngle1CD8 = 3.1415927f;
            break;
        case 1:
            verticalSpeed = -PlayerMovementSht(player)->normalAxisSpeed14;
            runtime->movementAngle1CD8 = -1.5707964f;
            break;
        case 2:
            verticalSpeed = PlayerMovementSht(player)->normalAxisSpeed14;
            runtime->movementAngle1CD8 = 1.5707964f;
            break;
        case 5:
            horizontalSpeed = -PlayerMovementSht(player)->normalDiagonalSpeed1C;
            verticalSpeed = horizontalSpeed;
            runtime->movementAngle1CD8 = -2.3561945f;
            break;
        case 7:
            verticalSpeed = PlayerMovementSht(player)->normalDiagonalSpeed1C;
            horizontalSpeed = -verticalSpeed;
            runtime->movementAngle1CD8 = 2.3561945f;
            break;
        case 6:
            horizontalSpeed = PlayerMovementSht(player)->normalDiagonalSpeed1C;
            verticalSpeed = -horizontalSpeed;
            runtime->movementAngle1CD8 = -0.7853982f;
            break;
        case 8:
            horizontalSpeed = PlayerMovementSht(player)->normalDiagonalSpeed1C;
            verticalSpeed = horizontalSpeed;
            runtime->movementAngle1CD8 = 0.7853982f;
            break;
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

    AnmLoaded *playerAnm =
        reinterpret_cast<AnmLoaded *>(player->header24.anmFile98);
    if (horizontalSpeed < 0.0f && tail->horizontalSpeed3035C >= 0.0f)
        playerAnm->SetAndExecuteScriptIdx(
            reinterpret_cast<AnmVm *>(&player->mainVm), 1);
    else if (horizontalSpeed == 0.0f && tail->horizontalSpeed3035C < 0.0f)
        playerAnm->SetAndExecuteScriptIdx(
            reinterpret_cast<AnmVm *>(&player->mainVm), 2);

    if (horizontalSpeed > 0.0f && tail->horizontalSpeed3035C <= 0.0f)
        playerAnm->SetAndExecuteScriptIdx(
            reinterpret_cast<AnmVm *>(&player->mainVm), 3);
    else if (horizontalSpeed == 0.0f && tail->horizontalSpeed3035C > 0.0f)
        playerAnm->SetAndExecuteScriptIdx(
            reinterpret_cast<AnmVm *>(&player->mainVm), 4);

    tail->horizontalSpeed3035C = horizontalSpeed;
    tail->verticalSpeed30360 = verticalSpeed;

    runtime->velocity1CCC.x =
        horizontalSpeed * g_PlayerMovementSupervisor.frameRateMultiplier5B8;
    runtime->velocity1CCC.y =
        verticalSpeed * g_PlayerMovementSupervisor.frameRateMultiplier5B8;

    float *position = player->position1B88.operator float *();
    position[0] += runtime->velocity1CCC.x;
    position[1] += runtime->velocity1CCC.y;

    if (position[0] < g_PlayerPlayfieldMinX)
        position[0] = g_PlayerPlayfieldMinX;
    else if (position[0] > g_PlayerPlayfieldMinX + g_PlayerPlayfieldWidth)
        position[0] = g_PlayerPlayfieldMinX + g_PlayerPlayfieldWidth;

    if (position[1] < g_PlayerPlayfieldMinY)
        position[1] = g_PlayerPlayfieldMinY;
    else if (position[1] > g_PlayerPlayfieldMinY + g_PlayerPlayfieldHeight)
        position[1] = g_PlayerPlayfieldMinY + g_PlayerPlayfieldHeight;

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
    for (int optionIndex = 0; optionIndex < 4; ++optionIndex, ++option)
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
        player->positions1BA0[0].x = player->position1B88.x;
        player->positions1BA0[0].y = player->position1B88.y;
        player->positions1BA0[0].z = player->position1B88.z;
    }

    return 0;
}
