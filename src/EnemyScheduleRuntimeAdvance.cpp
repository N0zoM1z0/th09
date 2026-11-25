#include "EnemyManager.hpp"
#include "EnemyManagerCardAttackSpawn.hpp"
#include "RngRuntimeLeaves.hpp"
#include "GameManagerRuntimeView.hpp"
#include "FrontFactoryConstructView.hpp"
#include "ZunTimer.hpp"
#include "ZunTimerEqual.inl"

#include <stddef.h>

typedef unsigned char u8;
typedef unsigned int u32;

struct EnemyTimelineInstructionView
{
    int time00;
    short opcode04;
    u8 size06;
    u8 mask07;
};

// TH09 v1.50a enemy stage-timeline opcodes recovered from
// EnemyScheduleRuntimeView::Run.
enum EnemyTimelineOpcode
{
    ENEMY_TIMELINE_OPCODE_SPAWN_0 = 0,
    ENEMY_TIMELINE_OPCODE_SPAWN_1 = 1,
    ENEMY_TIMELINE_OPCODE_SPAWN_RANDOM_X_RANGE = 2,
    ENEMY_TIMELINE_OPCODE_SPAWN_RANDOM_X_PLAYFIELD = 3,
    ENEMY_TIMELINE_OPCODE_SPAWN_RANDOM_X_RANGE_MIRRORED = 4,
    ENEMY_TIMELINE_OPCODE_SPAWN_RANDOM_X_PLAYFIELD_MIRRORED = 5,
    ENEMY_TIMELINE_OPCODE_SET_REMOTE_PENDING_SUBROUTINE = 8,
    ENEMY_TIMELINE_OPCODE_WAIT_WHILE_REMOTE_ACTIVE = 10,
    ENEMY_TIMELINE_OPCODE_SPAWN_WITH_DROP_COUNTS = 11,
    ENEMY_TIMELINE_OPCODE_SPAWN_WITH_DROP_COUNTS_MIRRORED = 12,
    ENEMY_TIMELINE_OPCODE_WAIT_FOR_EVENT = 13,
    ENEMY_TIMELINE_OPCODE_SIGNAL_EVENT = 14,
    ENEMY_TIMELINE_OPCODE_SPAWN_15 = 15,
    ENEMY_TIMELINE_OPCODE_MARK_MODE0_DONE = 16,
    ENEMY_TIMELINE_OPCODE_SPAWN_WITH_MOVEMENT_FLAG = 17
};

// Original timeline opcodes address the native Enemy pool/manager. These
// +0x2D70/+0x3360/+0x3364 Enemy words and manager +0x2AC388/+0x2AC430
// slots are shared with the exact Enemy spawn and ECL owners, not a second
// padded 0x2AC430-byte allocation.
typedef char EnemyTimelineNativePoolAndEvents[
    (sizeof(EnemyView) == 0x5430 &&
     offsetof(EnemyView, pendingSubroutineId2D70) == 0x2D70 &&
     offsetof(EnemyView, itemDropCount3360) == 0x3360 &&
     offsetof(EnemyView, itemDropExtraCount3364) == 0x3364 &&
     offsetof(EnemyView, secondaryFlags3380) == 0x3380 &&
     offsetof(EnemyManagerView, remoteSlots2AC388) == 0x2AC388 &&
     offsetof(EnemyManagerView, timelineEventSlots2AC430) == 0x2AC430 &&
     sizeof(EnemyManagerView) == 0x2AC450) ? 1 : -1];

struct EnemyTimelineSpawnManagerView
{
    void *SpawnEnemyRecord(
        int subroutine,
        CardAttackSpawnPosition *position,
        int life,
        int defeatTokenType,
        int score,
        int mirrorMovementX,
        int useOpposingEclManager);
};

struct EnemyScheduleRuntimeAdvanceOverlay
{
    ZunTimer timer00;
    ZunTimer delayTimer0C;
    int mirrorMovementX18;
    EnemyManagerView *manager1C;
    EnemyTimelineInstructionView *instruction20;
};

typedef char EnemyScheduleAdvanceMirrorAt18[
    (offsetof(EnemyScheduleRuntimeAdvanceOverlay, mirrorMovementX18) == 0x18) ? 1 : -1];
typedef char EnemyScheduleAdvanceManagerAt1C[
    (offsetof(EnemyScheduleRuntimeAdvanceOverlay, manager1C) == 0x1C) ? 1 : -1];
typedef char EnemyScheduleAdvanceInstructionAt20[
    (offsetof(EnemyScheduleRuntimeAdvanceOverlay, instruction20) == 0x20) ? 1 : -1];

extern float g_EnemyTimelineStep;
extern unsigned char g_FrontMode0Done;
extern RngRuntimeView g_Rng;

// Timeline opcodes 8/10 select the real remote Enemy slots; opcodes
// 13/14 visit the four native event flags. Their live access ordering is
// retained, with no extra bounds checks or cached copies.
//
// Each timeline instruction is a variable-length file-backed record.
// Address the payload through the instruction's byte representation,
// then read its original 32-bit argument words using int *.

int EnemyScheduleRuntimeView::Run()
{
    EnemyScheduleRuntimeAdvanceOverlay *runtime =
        reinterpret_cast<EnemyScheduleRuntimeAdvanceOverlay *>(this);
    if (g_GameManager.frontFactoryA8->IsBlocked())
        return 0;

    if (static_cast<float>(runtime->delayTimer0C) > 0.0f)
    {
        runtime->delayTimer0C--;
        return 0;
    }

    while (runtime->instruction20->time00 >= 0)
    {
        EnemyTimelineInstructionView *instruction = runtime->instruction20;
        int instructionTime = instruction->time00;

        if (runtime->timer00 == instructionTime)
        {
            if ((g_GameManager.instructionMaskLowByte120 & instruction->mask07) != 0)
            {
                switch (instruction->opcode04)
                {
                case ENEMY_TIMELINE_OPCODE_MARK_MODE0_DONE:
                    g_FrontMode0Done = 1;
                    break;

                case ENEMY_TIMELINE_OPCODE_SPAWN_0:
                case ENEMY_TIMELINE_OPCODE_SPAWN_1:
                {
                    int *args1 = reinterpret_cast<int *>(
                        reinterpret_cast<unsigned char *>(instruction) +
                        sizeof(EnemyTimelineInstructionView));
                    CardAttackSpawnPosition position1;
                    position1.x = *reinterpret_cast<float *>(&args1[1]);
                    position1.y = *reinterpret_cast<float *>(&args1[2]);
                    position1.z = 0.0f;
                    reinterpret_cast<EnemyTimelineSpawnManagerView *>(
                        runtime->manager1C)->SpawnEnemyRecord(
                            args1[0], &position1,
                            args1[3], args1[4], args1[5],
                            runtime->mirrorMovementX18, 0);
                    break;
                }

                case ENEMY_TIMELINE_OPCODE_SPAWN_WITH_MOVEMENT_FLAG:
                {
                    int *args17 = reinterpret_cast<int *>(
                        reinterpret_cast<unsigned char *>(instruction) +
                        sizeof(EnemyTimelineInstructionView));
                    CardAttackSpawnPosition position17;
                    position17.x = *reinterpret_cast<float *>(&args17[1]);
                    position17.y = *reinterpret_cast<float *>(&args17[2]);
                    position17.z = 0.0f;
                    EnemyView *enemy17 =
                        reinterpret_cast<EnemyView *>(
                            reinterpret_cast<EnemyTimelineSpawnManagerView *>(
                                runtime->manager1C)->SpawnEnemyRecord(
                                    args17[0],
                                    &position17, args17[3],
                                    args17[4], args17[5],
                                    runtime->mirrorMovementX18, 0));
                    enemy17->secondaryFlagBits3380.movement.movementMode09_10 |= 1u;
                    break;
                }

                case ENEMY_TIMELINE_OPCODE_SPAWN_15:
                {
                    int *args15 = reinterpret_cast<int *>(
                        reinterpret_cast<unsigned char *>(instruction) +
                        sizeof(EnemyTimelineInstructionView));
                    CardAttackSpawnPosition position15;
                    position15.x = *reinterpret_cast<float *>(&args15[1]);
                    position15.y = *reinterpret_cast<float *>(&args15[2]);
                    position15.z = 0.0f;
                    reinterpret_cast<EnemyTimelineSpawnManagerView *>(
                        runtime->manager1C)->SpawnEnemyRecord(
                            args15[0], &position15,
                            args15[3], args15[4], args15[5],
                            runtime->mirrorMovementX18, 0);
                    break;
                }

                case ENEMY_TIMELINE_OPCODE_SPAWN_WITH_DROP_COUNTS_MIRRORED:
                    runtime->mirrorMovementX18 = 1;
                case ENEMY_TIMELINE_OPCODE_SPAWN_WITH_DROP_COUNTS:
                {
                    struct SpecialSpawnLocals
                    {
                        EnemyView *enemy;
                        int *args;
                    } locals;
                    locals.args = reinterpret_cast<int *>(
                        reinterpret_cast<unsigned char *>(instruction) +
                        sizeof(EnemyTimelineInstructionView));
                    CardAttackSpawnPosition position11;
                    position11.x =
                        *reinterpret_cast<float *>(&locals.args[1]);
                    position11.y =
                        *reinterpret_cast<float *>(&locals.args[2]);
                    position11.z = 0.0f;
                    locals.enemy = reinterpret_cast<EnemyView *>(
                        reinterpret_cast<EnemyTimelineSpawnManagerView *>(
                            runtime->manager1C)->SpawnEnemyRecord(
                                locals.args[0],
                                &position11, locals.args[3], -1,
                                locals.args[6], runtime->mirrorMovementX18, 0));
                    locals.enemy->itemDropCount3360 = locals.args[4];
                    locals.enemy->itemDropExtraCount3364 = locals.args[5];
                    break;
                }

                case ENEMY_TIMELINE_OPCODE_SPAWN_RANDOM_X_RANGE_MIRRORED:
                    runtime->mirrorMovementX18 = 1;
                case ENEMY_TIMELINE_OPCODE_SPAWN_RANDOM_X_RANGE:
                {
                    int *argsRange = reinterpret_cast<int *>(
                        reinterpret_cast<unsigned char *>(instruction) +
                        sizeof(EnemyTimelineInstructionView));
                    CardAttackSpawnPosition positionRange;
                    positionRange.x = g_Rng.GetRandomF32InRange(
                            *reinterpret_cast<float *>(&argsRange[2]) -
                            *reinterpret_cast<float *>(&argsRange[1])) +
                        *reinterpret_cast<float *>(&argsRange[1]);
                    positionRange.y =
                        *reinterpret_cast<float *>(&argsRange[3]);
                    positionRange.z = 0.0f;
                    reinterpret_cast<EnemyTimelineSpawnManagerView *>(
                        runtime->manager1C)->SpawnEnemyRecord(
                            argsRange[0], &positionRange,
                            argsRange[4], argsRange[5],
                            argsRange[6], runtime->mirrorMovementX18, 0);
                    break;
                }

                case ENEMY_TIMELINE_OPCODE_SPAWN_RANDOM_X_PLAYFIELD_MIRRORED:
                    runtime->mirrorMovementX18 = 1;
                case ENEMY_TIMELINE_OPCODE_SPAWN_RANDOM_X_PLAYFIELD:
                {
                    int *argsPlay = reinterpret_cast<int *>(
                        reinterpret_cast<unsigned char *>(instruction) +
                        sizeof(EnemyTimelineInstructionView));
                    CardAttackSpawnPosition positionPlay;
                    positionPlay.x = g_Rng.GetRandomF32InRange(384.0f);
                    positionPlay.y =
                        *reinterpret_cast<float *>(&argsPlay[1]);
                    positionPlay.z = 0.0f;
                    reinterpret_cast<EnemyTimelineSpawnManagerView *>(
                        runtime->manager1C)->SpawnEnemyRecord(
                            argsPlay[0], &positionPlay,
                            argsPlay[2], argsPlay[3],
                            argsPlay[4], runtime->mirrorMovementX18, 0);
                    break;
                }

                case ENEMY_TIMELINE_OPCODE_SET_REMOTE_PENDING_SUBROUTINE:
                {
                    int *args8 = reinterpret_cast<int *>(
                        reinterpret_cast<unsigned char *>(instruction) +
                        sizeof(EnemyTimelineInstructionView));
                    runtime->manager1C->remoteSlots2AC388[static_cast<unsigned int>(args8[0])]
                        ->pendingSubroutineId2D70 =
                            static_cast<short>(args8[1]);
                    break;
                }

                case ENEMY_TIMELINE_OPCODE_WAIT_WHILE_REMOTE_ACTIVE:
                {
                    int *args10 = reinterpret_cast<int *>(
                        reinterpret_cast<unsigned char *>(instruction) +
                        sizeof(EnemyTimelineInstructionView));
                    if (runtime->manager1C->remoteSlots2AC388[static_cast<unsigned int>(args10[0])] != 0 &&
                        runtime->manager1C->remoteSlots2AC388[static_cast<unsigned int>(args10[0])]->
                            primaryFlagBits337C.active00 != 0)
                    {
                        runtime->timer00--;
                        goto finish;
                    }
                    break;
                }

                case ENEMY_TIMELINE_OPCODE_WAIT_FOR_EVENT:
                {
                    int matchCount = 0;
                    for (unsigned int i = 0; i < 4; ++i)
                    {
                        if (runtime->manager1C->timelineEventSlots2AC430[i] ==
                            *reinterpret_cast<const int *>(
                                reinterpret_cast<const unsigned char *>(runtime->instruction20) +
                                sizeof(EnemyTimelineInstructionView)))
                        {
                            ++matchCount;
                            runtime->manager1C->timelineEventSlots2AC430[i] = -1;
                        }
                    }
                    if (matchCount == 0)
                    {
                        runtime->timer00--;
                        goto finish;
                    }
                    break;
                }

                case ENEMY_TIMELINE_OPCODE_SIGNAL_EVENT:
                    for (unsigned int j = 0; j < 4; ++j)
                    {
                        if (runtime->manager1C->timelineEventSlots2AC430[j] >= 0)
                            continue;
                        runtime->manager1C->timelineEventSlots2AC430[j] =
                            *reinterpret_cast<const int *>(
                                reinterpret_cast<const unsigned char *>(runtime->instruction20) +
                                sizeof(EnemyTimelineInstructionView));
                    }
                    break;

                default:
                    break;
                }
            }
        }
        else if (runtime->timer00 < instructionTime)
        {
            break;
        }

        // Advance the original file-backed instruction pointer by its
        // live byte length; the payload is not a fixed-size C++ array.
        runtime->instruction20 = reinterpret_cast<EnemyTimelineInstructionView *>(
            reinterpret_cast<unsigned char *>(runtime->instruction20) +
            runtime->instruction20->size06);
    }

finish:
    if (runtime->instruction20->time00 < 0)
    {
        runtime->instruction20 = 0;
        runtime->timer00 = 0;
        return 1;
    }

    runtime->timer00 += g_EnemyTimelineStep;
    return 0;
}
