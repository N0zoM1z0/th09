#include "PlayerLifecycleView.hpp"
#include "Float3.hpp"
#include "EffectManager.hpp"
#include "GameConfiguration.hpp"
#include "AnmManager.hpp"
#include "AnmVmLifecycle.hpp"
#include "ZunTimer.hpp"
#include "Supervisor.hpp"
#include "GameManagerRuntimeView.hpp"
#include "ReplayInputState.hpp"

#include <stddef.h>

typedef unsigned short u16;

// The Player side-state +0x0C effect owner is the same 0x38-byte
// GameManager side slot used by the lifecycle and death controllers.
typedef char PlayerMovementSideEffectAt0C[
    (offsetof(PlayerSideStateView, effectManager0C) == 0x0C) ? 1 : -1];
typedef char PlayerMovementSideShotAt20[
    (offsetof(PlayerSideStateView, shotType20) == 0x20) ? 1 : -1];

// Consume the actual Player option record at +0x1CEC; do not maintain a
// second unrelated 0x2F4-byte view for the movement interpreter.
typedef char PlayerMovementOptionSizeIs2F4[
    (sizeof(PlayerOptionStateCtorView) == 0x2F4) ? 1 : -1];
// Movement speed/velocity comes from the native Player object, not an
// independent scratch buffer or second layout projection.
typedef char PlayerMovementNativeFieldsAt1CCC[
    (offsetof(PlayerLifecycleView, movementFields1CCC) == 0x1CCC &&
     sizeof(PlayerMovementFields) == 0x20) ? 1 : -1];

typedef char PlayerMovementOptionTimerAt2E0[
    (offsetof(PlayerOptionStateCtorView, timer2E0) == 0x2E0) ? 1 : -1];
typedef char PlayerMovementOptionCallbackAt2EC[
    (offsetof(PlayerOptionStateCtorView, updateCallback2EC) == 0x2EC) ? 1 : -1];

// These existing Player vectors are three consecutive floats. Arithmetic
// uses the implemented Float3 providers without changing Player construction.
typedef char PlayerMovementNativeVectorStorage[
    (sizeof(PlayerPositionView) == sizeof(Float3) &&
     offsetof(PlayerPositionView, x) == offsetof(Float3, x) &&
     offsetof(PlayerPositionView, y) == offsetof(Float3, y) &&
     offsetof(PlayerPositionView, z) == offsetof(Float3, z)) ? 1 : -1];

extern GameConfiguration *g_GameConfiguration;

// Actual playfield bounds are four fields within the exact GameManager
// coordinate initializer owner, not four independent retail globals.
typedef char PlayerMovementPlayfieldMinXAt360[
    (offsetof(AsciiGameManagerView, coordinate360) == 0x360) ? 1 : -1];
typedef char PlayerMovementPlayfieldMinYAt364[
    (offsetof(AsciiGameManagerView, coordinate364) == 0x364) ? 1 : -1];
typedef char PlayerMovementPlayfieldWidthAt368[
    (offsetof(AsciiGameManagerView, coordinate368) == 0x368) ? 1 : -1];
typedef char PlayerMovementPlayfieldHeightAt36C[
    (offsetof(AsciiGameManagerView, coordinate36C) == 0x36C) ? 1 : -1];
extern int g_PlayerFocusEffectIds[];
extern AnmManager *g_AnmManager;

// The sixteen native Player position-history entries are 0x0C-byte records.
// Use the same Win32 DWORD stride as the original history shift/copy.
static inline PlayerConstructedVector3View &NativeMovementHistorySlot(
    PlayerLifecycleView *player, int index)
{
    return *reinterpret_cast<PlayerConstructedVector3View *>(
        reinterpret_cast<unsigned int>(&player->positions1BA0[0]) +
        static_cast<unsigned int>(index) * sizeof(player->positions1BA0[0]));
}

int PlayerLifecycleView::UpdateMovementAndOptions()
{
    PlayerLifecycleView *player = this;
    float horizontalSpeed = 0.0f;
    float verticalSpeed = 0.0f;

    if (player->header24.state84 == 0)
        return 0;

    // The real three-slot 0x8E-byte replay-input array owns movement input
    // and focus history. TH09 computes its input receiver from the original
    // array base with a native DWORD-scaled side index, not another layout.
    const int inputSide = player->sideIndex;
    ReplayInputState *input = reinterpret_cast<ReplayInputState *>(
        reinterpret_cast<unsigned int>(&g_ReplayInputStates[0]) +
        static_cast<unsigned int>(inputSide) * sizeof(g_ReplayInputStates[0]));

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
    // TH09 reads the configuration focus byte by native row-base plus
    // side DWORD, rather than a C++ array subscript on a two-byte row.
    const unsigned char *focusModes = g_GameConfiguration->sideFocusMode;
    if (*reinterpret_cast<const unsigned char *>(
            reinterpret_cast<unsigned int>(focusModes) +
            static_cast<unsigned int>(player->sideIndex)) == 1)
        // Retail rereads the live side input through the genuine 0x8E-byte
        // ReplayInputState array stride. Do not reuse the earlier receiver:
        // that changes VC7.1's original operand schedule in this boss.
        focused = (*reinterpret_cast<ReplayInputState *>(
            reinterpret_cast<unsigned int>(&g_ReplayInputStates[0]) +
            static_cast<unsigned int>(inputSide) *
                sizeof(g_ReplayInputStates[0]))).auxiliary2A >= 8;
    else
        focused = input->IsHeld(4) != 0;

    player->flags1B80 ^=
        (player->flags1B80 ^ static_cast<unsigned int>(focused)) & 3u;

    PlayerMovementFields &runtime = player->movementFields1CCC;

    if ((player->flags1B80 & 3u) != 0)
    {
        if (player->focusEffect364 == NULL)
        {
            EffectFloat3 *effectPosition =
                reinterpret_cast<EffectFloat3 *>(&player->position1B88);
            // Preserve the first call's side argument before evaluating its
            // manager receiver. The second call still reloads the side slot.
            int spawnSide = player->sideIndex;
            player->focusEffect364 =
                player->sideState->effectManager0C->SpawnEffectInFixedSlot(
                    7,
                    effectPosition,
                    spawnSide,
                    static_cast<unsigned int>(-1));
            player->secondaryEffect368 =
                player->sideState->effectManager0C->SpawnEffectInFixedSlot(
                    // Retail uses a signed shot-type DWORD as an unguarded,
                    // modulo-32 scaled native address into this ID table.
                    *reinterpret_cast<const int *>(
                        reinterpret_cast<unsigned int>(&g_PlayerFocusEffectIds[0]) +
                        static_cast<unsigned int>(player->sideState->shotType20) *
                            sizeof(g_PlayerFocusEffectIds[0])),
                    effectPosition,
                    static_cast<int>(
                        static_cast<unsigned int>(player->sideIndex) + 2u),
                    static_cast<unsigned int>(-1));
        }

        switch (player->movementDirection30358)
        {
        case 4:
        {
            PlayerShtFileView *sht = player->primaryShtFile;
            runtime.movementAngle1CD8 = 0.0f;
            horizontalSpeed = sht->focusedAxisSpeed18;
            break;
        }
        case 3:
        {
            PlayerShtFileView *sht = player->primaryShtFile;
            runtime.movementAngle1CD8 = 3.1415927f;
            horizontalSpeed = -sht->focusedAxisSpeed18;
            break;
        }
        case 1:
        {
            PlayerShtFileView *sht = player->primaryShtFile;
            runtime.movementAngle1CD8 = -1.5707964f;
            verticalSpeed = -sht->focusedAxisSpeed18;
            break;
        }
        case 2:
        {
            PlayerShtFileView *sht = player->primaryShtFile;
            runtime.movementAngle1CD8 = 1.5707964f;
            verticalSpeed = sht->focusedAxisSpeed18;
            break;
        }
        case 5:
        {
            PlayerShtFileView *sht = player->primaryShtFile;
            runtime.movementAngle1CD8 = -2.3561945f;
            horizontalSpeed = -sht->focusedDiagonalSpeed20;
            verticalSpeed = horizontalSpeed;
            break;
        }
        case 7:
        {
            PlayerShtFileView *sht = player->primaryShtFile;
            runtime.movementAngle1CD8 = 2.3561945f;
            verticalSpeed = sht->focusedDiagonalSpeed20;
            horizontalSpeed = -verticalSpeed;
            break;
        }
        case 6:
        {
            PlayerShtFileView *sht = player->primaryShtFile;
            runtime.movementAngle1CD8 = -0.7853982f;
            horizontalSpeed = sht->focusedDiagonalSpeed20;
            verticalSpeed = -horizontalSpeed;
            break;
        }
        case 8:
        {
            PlayerShtFileView *sht = player->primaryShtFile;
            runtime.movementAngle1CD8 = 0.7853982f;
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
        if (player->focusEffect364 != NULL)
        {
            player->focusEffect364->vms->SetInterrupt(1);
            player->focusEffect364 = NULL;
            player->secondaryEffect368->active = 0;
            player->secondaryEffect368 = NULL;
        }

        switch (player->movementDirection30358)
        {
        case 4:
        {
            PlayerShtFileView *sht = player->primaryShtFile;
            runtime.movementAngle1CD8 = 0.0f;
            horizontalSpeed = sht->normalAxisSpeed14;
            break;
        }
        case 3:
        {
            PlayerShtFileView *sht = player->primaryShtFile;
            runtime.movementAngle1CD8 = 3.1415927f;
            horizontalSpeed = -sht->normalAxisSpeed14;
            break;
        }
        case 1:
        {
            PlayerShtFileView *sht = player->primaryShtFile;
            runtime.movementAngle1CD8 = -1.5707964f;
            verticalSpeed = -sht->normalAxisSpeed14;
            break;
        }
        case 2:
        {
            PlayerShtFileView *sht = player->primaryShtFile;
            runtime.movementAngle1CD8 = 1.5707964f;
            verticalSpeed = sht->normalAxisSpeed14;
            break;
        }
        case 5:
        {
            PlayerShtFileView *sht = player->primaryShtFile;
            runtime.movementAngle1CD8 = -2.3561945f;
            horizontalSpeed = -sht->normalDiagonalSpeed1C;
            verticalSpeed = horizontalSpeed;
            break;
        }
        case 7:
        {
            PlayerShtFileView *sht = player->primaryShtFile;
            runtime.movementAngle1CD8 = 2.3561945f;
            verticalSpeed = sht->normalDiagonalSpeed1C;
            horizontalSpeed = -verticalSpeed;
            break;
        }
        case 6:
        {
            PlayerShtFileView *sht = player->primaryShtFile;
            runtime.movementAngle1CD8 = -0.7853982f;
            horizontalSpeed = sht->normalDiagonalSpeed1C;
            verticalSpeed = -horizontalSpeed;
            break;
        }
        case 8:
        {
            PlayerShtFileView *sht = player->primaryShtFile;
            runtime.movementAngle1CD8 = 0.7853982f;
            horizontalSpeed = sht->normalDiagonalSpeed1C;
            verticalSpeed = horizontalSpeed;
            break;
        }
        default:
            break;
        }
    }

    horizontalSpeed =
        runtime.speedMultiplier1CE4 *
        runtime.speedMultiplier1CDC *
        horizontalSpeed;
    verticalSpeed =
        runtime.speedMultiplier1CE8 *
        runtime.speedMultiplier1CE0 *
        verticalSpeed;

    // The second transition group reloads the ANM owner after the first call.
    if (horizontalSpeed < 0.0f && player->horizontalSpeed3035C >= 0.0f)
        player->header24.anmFile98->SetAndExecuteScriptIdx(
            &player->mainVm, 1);
    else if (horizontalSpeed == 0.0f && player->horizontalSpeed3035C < 0.0f)
        player->header24.anmFile98->SetAndExecuteScriptIdx(
            &player->mainVm, 2);

    if (horizontalSpeed > 0.0f && player->horizontalSpeed3035C <= 0.0f)
        player->header24.anmFile98->SetAndExecuteScriptIdx(
            &player->mainVm, 3);
    else if (horizontalSpeed == 0.0f && player->horizontalSpeed3035C > 0.0f)
        player->header24.anmFile98->SetAndExecuteScriptIdx(
            &player->mainVm, 4);

    player->horizontalSpeed3035C = horizontalSpeed;
    player->verticalSpeed30360 = verticalSpeed;

    runtime.velocity1CCC.x =
        horizontalSpeed * g_Supervisor.framerateMultiplier5B8;
    runtime.velocity1CCC.y =
        verticalSpeed * g_Supervisor.framerateMultiplier5B8;

    float *position = player->position1B88;
    position[0] += runtime.velocity1CCC.x;
    position[1] += runtime.velocity1CCC.y;

    if (position[0] < g_GameManager.coordinate360)
        position[0] = g_GameManager.coordinate360;
    else if (position[0] > g_GameManager.coordinate360 + g_GameManager.coordinate368)
        position[0] = g_GameManager.coordinate360 + g_GameManager.coordinate368;

    if (position[1] < g_GameManager.coordinate364)
        position[1] = g_GameManager.coordinate364;
    else if (position[1] > g_GameManager.coordinate364 + g_GameManager.coordinate36C)
        position[1] = g_GameManager.coordinate364 + g_GameManager.coordinate36C;

    const Float3 &positionVector =
        reinterpret_cast<const Float3 &>(player->position1B88);

    reinterpret_cast<Float3 &>(player->collisionBoundsMin1C60) =
        positionVector - reinterpret_cast<const Float3 &>(player->hurtboxHalfSize);
    reinterpret_cast<Float3 &>(player->collisionBoundsMax1C6C) =
        positionVector + reinterpret_cast<const Float3 &>(player->hurtboxHalfSize);
    reinterpret_cast<Float3 &>(player->grazeBoundsMin1C78) =
        positionVector - reinterpret_cast<const Float3 &>(player->grazeHalfSize);
    reinterpret_cast<Float3 &>(player->grazeBoundsMax1C84) =
        positionVector + reinterpret_cast<const Float3 &>(player->grazeHalfSize);
    reinterpret_cast<Float3 &>(player->unknownBoundsMin1C90) =
        positionVector - reinterpret_cast<const Float3 &>(player->itemCollectionHalfSize);
    reinterpret_cast<Float3 &>(player->unknownBoundsMax1C9C) =
        positionVector + reinterpret_cast<const Float3 &>(player->itemCollectionHalfSize);

    enum
    {
        OptionCount = sizeof(player->optionStates) / sizeof(player->optionStates[0]),
        PositionHistoryLast = sizeof(player->positions1BA0) /
                              sizeof(player->positions1BA0[0]) - 1
    };
    PlayerOptionStateCtorView *option = player->optionStates;
    // Advance through the true 0x2F4-byte native option records.
    for (int optionCount = OptionCount; optionCount != 0; --optionCount,
         option = reinterpret_cast<PlayerOptionStateCtorView *>(
             reinterpret_cast<unsigned int>(option) + sizeof(*option)))
    {
        if (option->updateCallback2EC != NULL)
        {
            option->updateCallback2EC(player, option);
            g_AnmManager->ExecuteScript(
                &option->vm00);
            option->timer2E0++;
        }
    }

    if (verticalSpeed != 0.0f || horizontalSpeed != 0.0f)
    {
        for (int historyIndex = PositionHistoryLast; historyIndex > 0; --historyIndex)
            NativeMovementHistorySlot(player, historyIndex) =
                NativeMovementHistorySlot(
                    player, static_cast<int>(static_cast<unsigned int>(historyIndex) - 1u));
        player->positions1BA0[0] =
            *reinterpret_cast<PlayerConstructedVector3View *>(&player->position1B88);
    }

    return 0;
}
