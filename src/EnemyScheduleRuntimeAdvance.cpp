#include "EnemyManagerCardAttackSpawn.hpp"
#include "RngRuntimeLeaves.hpp"
#include "ZunTimer.hpp"

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

struct EnemyTimelineEnemyView
{
    unsigned char unknown0000[0x2D70];
    short pendingEclSubroutine2D70;
    unsigned char unknown2D72[0x3360 - 0x2D72];
    int timelineParam0_3360;
    int timelineParam1_3364;
    unsigned char unknown3368[0x337C - 0x3368];
    unsigned int flags337C;
    unsigned int flags3380;
};

struct EnemyTimelineManagerView
{
    unsigned char unknown000000[0x2AC388];
    EnemyTimelineEnemyView *remoteSlots2AC388[9];
    unsigned char unknown2AC3AC[0x2AC430 - 0x2AC3AC];
    int timelineEventSlots2AC430[4];
};

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

struct EnemyTimelineSharedRuntimeView
{
    int IsBlocked();
};

struct EnemyScheduleRuntimeAdvanceView
{
    ZunTimer timer00;
    ZunTimer delayTimer0C;
    int mirrorMovementX18;
    EnemyTimelineManagerView *manager1C;
    EnemyTimelineInstructionView *instruction20;

    int Run();
};

typedef char EnemyScheduleAdvanceMirrorAt18[
    (offsetof(EnemyScheduleRuntimeAdvanceView, mirrorMovementX18) == 0x18) ? 1 : -1];
typedef char EnemyScheduleAdvanceManagerAt1C[
    (offsetof(EnemyScheduleRuntimeAdvanceView, manager1C) == 0x1C) ? 1 : -1];
typedef char EnemyScheduleAdvanceInstructionAt20[
    (offsetof(EnemyScheduleRuntimeAdvanceView, instruction20) == 0x20) ? 1 : -1];

extern EnemyTimelineSharedRuntimeView *g_PlayerSharedRuntime;
extern unsigned char g_EnemyTimelineInstructionMask;
extern float g_EnemyTimelineStep;
extern unsigned char g_FrontMode0Done;
extern RngRuntimeView g_Rng;

int EnemyScheduleRuntimeAdvanceView::Run()
{
    if (g_PlayerSharedRuntime->IsBlocked())
        return 0;

    if (static_cast<float>(delayTimer0C) > 0.0f)
    {
        delayTimer0C--;
        return 0;
    }

    while (instruction20->time00 >= 0)
    {
        EnemyTimelineInstructionView *instruction = instruction20;
        int instructionTime = instruction->time00;

        if (timer00 == instructionTime)
        {
            if ((g_EnemyTimelineInstructionMask & instruction->mask07) != 0)
            {
                switch (instruction->opcode04)
                {
                case 16:
                    g_FrontMode0Done = 1;
                    break;

                case 0:
                case 1:
                {
                    int *args1 = reinterpret_cast<int *>(
                        reinterpret_cast<unsigned char *>(instruction) + 8);
                    CardAttackSpawnPosition position1;
                    position1.x = *reinterpret_cast<float *>(&args1[1]);
                    position1.y = *reinterpret_cast<float *>(&args1[2]);
                    position1.z = 0.0f;
                    reinterpret_cast<EnemyTimelineSpawnManagerView *>(
                        manager1C)->SpawnEnemyRecord(
                            args1[0], &position1,
                            args1[3], args1[4], args1[5],
                            mirrorMovementX18, 0);
                    break;
                }

                case 17:
                {
                    int *args17 = reinterpret_cast<int *>(
                        reinterpret_cast<unsigned char *>(instruction) + 8);
                    CardAttackSpawnPosition position17;
                    position17.x = *reinterpret_cast<float *>(&args17[1]);
                    position17.y = *reinterpret_cast<float *>(&args17[2]);
                    position17.z = 0.0f;
                    EnemyTimelineEnemyView *enemy17 =
                        reinterpret_cast<EnemyTimelineEnemyView *>(
                            reinterpret_cast<EnemyTimelineSpawnManagerView *>(
                                manager1C)->SpawnEnemyRecord(
                                    args17[0],
                                    &position17, args17[3],
                                    args17[4], args17[5],
                                    mirrorMovementX18, 0));
                    enemy17->flags3380 |= 0x200u;
                    break;
                }

                case 15:
                {
                    int *args15 = reinterpret_cast<int *>(
                        reinterpret_cast<unsigned char *>(instruction) + 8);
                    CardAttackSpawnPosition position15;
                    position15.x = *reinterpret_cast<float *>(&args15[1]);
                    position15.y = *reinterpret_cast<float *>(&args15[2]);
                    position15.z = 0.0f;
                    reinterpret_cast<EnemyTimelineSpawnManagerView *>(
                        manager1C)->SpawnEnemyRecord(
                            args15[0], &position15,
                            args15[3], args15[4], args15[5],
                            mirrorMovementX18, 0);
                    break;
                }

                case 12:
                    mirrorMovementX18 = 1;
                case 11:
                {
                    struct SpecialSpawnLocals
                    {
                        EnemyTimelineEnemyView *enemy;
                        int *args;
                    } locals;
                    locals.args = reinterpret_cast<int *>(
                        reinterpret_cast<unsigned char *>(instruction) + 8);
                    CardAttackSpawnPosition position11;
                    position11.x =
                        *reinterpret_cast<float *>(&locals.args[1]);
                    position11.y =
                        *reinterpret_cast<float *>(&locals.args[2]);
                    position11.z = 0.0f;
                    locals.enemy = reinterpret_cast<EnemyTimelineEnemyView *>(
                        reinterpret_cast<EnemyTimelineSpawnManagerView *>(
                            manager1C)->SpawnEnemyRecord(
                                locals.args[0],
                                &position11, locals.args[3], -1,
                                locals.args[6], mirrorMovementX18, 0));
                    locals.enemy->timelineParam0_3360 = locals.args[4];
                    locals.enemy->timelineParam1_3364 = locals.args[5];
                    break;
                }

                case 4:
                    mirrorMovementX18 = 1;
                case 2:
                {
                    int *argsRange = reinterpret_cast<int *>(
                        reinterpret_cast<unsigned char *>(instruction) + 8);
                    CardAttackSpawnPosition positionRange;
                    positionRange.x = g_Rng.GetRandomF32InRange(
                            *reinterpret_cast<float *>(&argsRange[2]) -
                            *reinterpret_cast<float *>(&argsRange[1])) +
                        *reinterpret_cast<float *>(&argsRange[1]);
                    positionRange.y =
                        *reinterpret_cast<float *>(&argsRange[3]);
                    positionRange.z = 0.0f;
                    reinterpret_cast<EnemyTimelineSpawnManagerView *>(
                        manager1C)->SpawnEnemyRecord(
                            argsRange[0], &positionRange,
                            argsRange[4], argsRange[5],
                            argsRange[6], mirrorMovementX18, 0);
                    break;
                }

                case 5:
                    mirrorMovementX18 = 1;
                case 3:
                {
                    int *argsPlay = reinterpret_cast<int *>(
                        reinterpret_cast<unsigned char *>(instruction) + 8);
                    CardAttackSpawnPosition positionPlay;
                    positionPlay.x = g_Rng.GetRandomF32InRange(384.0f);
                    positionPlay.y =
                        *reinterpret_cast<float *>(&argsPlay[1]);
                    positionPlay.z = 0.0f;
                    reinterpret_cast<EnemyTimelineSpawnManagerView *>(
                        manager1C)->SpawnEnemyRecord(
                            argsPlay[0], &positionPlay,
                            argsPlay[2], argsPlay[3],
                            argsPlay[4], mirrorMovementX18, 0);
                    break;
                }

                case 8:
                {
                    int *args8 = reinterpret_cast<int *>(
                        reinterpret_cast<unsigned char *>(instruction) + 8);
                    manager1C->remoteSlots2AC388[args8[0]]
                        ->pendingEclSubroutine2D70 =
                            static_cast<short>(args8[1]);
                    break;
                }

                case 10:
                {
                    int *args10 = reinterpret_cast<int *>(
                        reinterpret_cast<unsigned char *>(instruction) + 8);
                    if (manager1C->remoteSlots2AC388[args10[0]] != 0 &&
                        (manager1C->remoteSlots2AC388[args10[0]]->flags337C &
                         1u) != 0)
                    {
                        timer00--;
                        goto finish;
                    }
                    break;
                }

                case 13:
                {
                    int matchCount = 0;
                    for (unsigned int i = 0; i < 4; ++i)
                    {
                        if (manager1C->timelineEventSlots2AC430[i] ==
                            reinterpret_cast<int *>(
                                reinterpret_cast<unsigned char *>(
                                    instruction20) + 8)[0])
                        {
                            ++matchCount;
                            manager1C->timelineEventSlots2AC430[i] = -1;
                        }
                    }
                    if (matchCount == 0)
                    {
                        timer00--;
                        goto finish;
                    }
                    break;
                }

                case 14:
                    for (unsigned int j = 0; j < 4; ++j)
                    {
                        if (manager1C->timelineEventSlots2AC430[j] >= 0)
                            continue;
                        manager1C->timelineEventSlots2AC430[j] =
                            reinterpret_cast<int *>(
                                reinterpret_cast<unsigned char *>(
                                    instruction20) + 8)[0];
                    }
                    break;

                default:
                    break;
                }
            }
        }
        else if (timer00 < instructionTime)
        {
            break;
        }

        instruction20 = reinterpret_cast<EnemyTimelineInstructionView *>(
            reinterpret_cast<unsigned char *>(instruction20) +
            instruction20->size06);
    }

finish:
    if (instruction20->time00 < 0)
    {
        instruction20 = 0;
        timer00 = 0;
        return 1;
    }

    timer00 += g_EnemyTimelineStep;
    return 0;
}
