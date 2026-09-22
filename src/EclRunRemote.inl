// TH09 v1.50a RunEcl remote/spawn family (wire opcodes 83..95).
//
// This lexical switch fragment covers six target-default slots and seven
// active handlers.  It does not create a callable boundary inside the shared
// EclManager::RunEcl owner.
//
// Target evidence:
//   remote handlers: 0x00409FD8..0x0040A163
//   spawn handlers:  0x0040B311..0x0040B541
//   manager spawn:   0x0040F340
//   manager clear:   0x0040F640
//
// Committed TH08 supplied source-shape hypotheses only.  TH09 independently
// fixes the remote table, repeated resolver behavior, raw subroutine ids,
// spawn packet geometry, life gate, relative-position addition, and manager
// call arguments used below.

#ifndef TH09_ECL_RUN_REMOTE_DECLARATIONS
#define TH09_ECL_RUN_REMOTE_DECLARATIONS

#include "EclRunMovement.inl"

#include <string.h>

namespace Th09EclRunRemote
{

enum EnemyRemoteOffset
{
    ENEMY_ACTIVE_CONTEXT = 0x2CE0,
    ENEMY_PENDING_SUBROUTINE = 0x2D70,
    ENEMY_LIFE = 0x2E48,
    ENEMY_MANAGER_REMOTE_TABLE = 0x2AC388
};

struct SpawnPacket
{
    int eclSubroutineId00;
    EnemyFloat3 position04;
    int argument10;
    int argument14;
    int argument18;
};
typedef char Th09EclSpawnPacketSizeIs1C[
    (sizeof(SpawnPacket) == 0x1C) ? 1 : -1];
typedef char Th09EclSpawnPacketPositionAt04[
    (offsetof(SpawnPacket, position04) == 0x04) ? 1 : -1];

inline unsigned char *Bytes(void *value)
{
    return reinterpret_cast<unsigned char *>(value);
}

inline EnemyView *RemoteEnemy(EnemyView *enemy, int slot)
{
    return *reinterpret_cast<EnemyView **>(
        Bytes(enemy->manager00) + ENEMY_MANAGER_REMOTE_TABLE + slot * 4);
}

inline Th09EclContextView *ActiveContext(EnemyView *enemy)
{
    return *reinterpret_cast<Th09EclContextView **>(
        Bytes(enemy) + ENEMY_ACTIVE_CONTEXT);
}

inline short &PendingSubroutine(EnemyView *enemy)
{
    return *reinterpret_cast<short *>(Bytes(enemy) + ENEMY_PENDING_SUBROUTINE);
}

inline int Life(EnemyView *enemy)
{
    return *reinterpret_cast<int *>(Bytes(enemy) + ENEMY_LIFE);
}

inline int *ActiveIntVariables(EnemyView *enemy)
{
    return reinterpret_cast<int *>(Bytes(ActiveContext(enemy)) + 0x1C);
}

// Both spawn handlers inline the three mask tests and keep only the float
// resolver calls out of line in the target owner.
__forceinline void ResolveSpawnPosition(
    EnemyFloat3 *position,
    EnemyView *enemy,
    Th09EclRawInstructionHeaderView *instruction,
    const SpawnPacket &packet)
{
    position->x = (instruction->parameterMask0A & (1U << 1))
                      ? Th09EclRunControl::ResolveFloat(
                            enemy, packet.position04.x)
                      : packet.position04.x;
    position->y = (instruction->parameterMask0A & (1U << 2))
                      ? Th09EclRunControl::ResolveFloat(
                            enemy, packet.position04.y)
                      : packet.position04.y;
    position->z = (instruction->parameterMask0A & (1U << 3))
                      ? Th09EclRunControl::ResolveFloat(
                            enemy, packet.position04.z)
                      : packet.position04.z;
}

// Role names for real target boundaries.  Final source TU/internal ABI remains
// open until the complete interpreter and its dependencies are compiled.
struct ManagerClearView
{
    int KillAllNonBossEnemies(int transitionValue, int startingValue);
};
void AddPosition(EnemyFloat3 *position, const EnemyFloat3 *offset);

} // namespace Th09EclRunRemote

#endif // TH09_ECL_RUN_REMOTE_DECLARATIONS

#if defined(TH09_ECL_RUN_REMOTE_BODY)

#if !defined(TH09_ECL_RUN_SHARED_SWITCH)
#error EclRunRemote.inl must be included lexically inside RunEcl's switch
#endif

    int remoteIndex;
    int remoteValue;
    float remoteFloat;
    EnemyView *remoteEnemy;
    Th09EclRunRemote::SpawnPacket spawnPacket93;
    Th09EclRunRemote::SpawnPacket spawnPacket94;
    EnemyFloat3 spawnPosition93;
    EnemyFloat3 spawnPosition94;
    EnemyFloat3 *spawnPosition;
    int spawnSubroutineId;
    int spawnLife;
    signed char spawnItemDrop;
    int spawnScore;
    int *spawnContextVariables;

    case TH09_ECL_OPCODE_UNHANDLED_53:
    case TH09_ECL_OPCODE_UNHANDLED_54:
    case TH09_ECL_OPCODE_UNHANDLED_55:
        break;

    case TH09_ECL_OPCODE_SET_REMOTE_INT:
        if ((instruction->parameterMask0A & (1U << 1)) != 0)
        {
            remoteIndex = Th09EclRunControl::ReadInt(enemy, instruction, 2);
            remoteEnemy =
                Th09EclRunRemote::RemoteEnemy(enemy, remoteIndex);
            lhsInt = Th09EclRunControl::ResolveInt(
                remoteEnemy,
                Th09EclRunControl::RawInt(instruction, 1));
        }
        else
        {
            lhsInt = Th09EclRunControl::RawInt(instruction, 1);
        }
th09_ecl_store_int_result:
        *Th09EclRunControl::WriteInt(enemy, instruction, 0) = lhsInt;
        break;

    case TH09_ECL_OPCODE_SET_REMOTE_FLOAT:
        if (Th09EclRunRemote::RemoteEnemy(
                enemy,
                Th09EclRunControl::ReadInt(enemy, instruction, 2)) != 0)
        {
            *Th09EclRunControl::WriteFloat(enemy, instruction, 0) =
                (instruction->parameterMask0A & (1U << 1))
                    ? Th09EclRunRemote::RemoteEnemy(
                          enemy,
                          Th09EclRunControl::ReadInt(enemy, instruction, 2))
                          ->ResolveFloat(
                              Th09EclRunControl::RawFloat(instruction, 1))
                    : Th09EclRunControl::RawFloat(instruction, 1);
        }
        break;

    case TH09_ECL_OPCODE_CALL_REMOTE:
        remoteIndex = Th09EclRunControl::ReadInt(enemy, instruction, 0);
        remoteEnemy = Th09EclRunRemote::RemoteEnemy(enemy, remoteIndex);
        Th09EclRunControl::CallSubroutine(
            remoteEnemy,
            Th09EclRunRemote::ActiveContext(remoteEnemy)->
                currentInstruction004,
            Th09EclRunControl::RawInt(instruction, 1));
        break;

    case TH09_ECL_OPCODE_SCHEDULE_REMOTE_SUBROUTINE:
        remoteEnemy = Th09EclRunRemote::RemoteEnemy(
            enemy,
            Th09EclRunControl::ReadInt(enemy, instruction, 0));
        if (remoteEnemy != 0)
        {
            remoteValue =
                Th09EclRunControl::ReadInt(enemy, instruction, 1);
            // The target resolves operand 0 again before selecting the store.
            remoteEnemy = Th09EclRunRemote::RemoteEnemy(
                enemy,
                Th09EclRunControl::ReadInt(enemy, instruction, 0));
            Th09EclRunRemote::PendingSubroutine(remoteEnemy) =
                static_cast<short>(remoteValue);
        }
        break;

    case TH09_ECL_OPCODE_UNHANDLED_5A:
    case TH09_ECL_OPCODE_UNHANDLED_5B:
    case TH09_ECL_OPCODE_UNHANDLED_5C:
        break;

    case TH09_ECL_OPCODE_SPAWN_ENEMY_AT_POSITION:
        if (Th09EclRunRemote::Life(enemy) <= 0)
            break;
        memcpy(
            &spawnPacket93,
            &Th09EclRunControl::RawInt(instruction, 0),
            sizeof(spawnPacket93));
        Th09EclRunRemote::ResolveSpawnPosition(
            &spawnPosition93, enemy, instruction, spawnPacket93);
        spawnSubroutineId = spawnPacket93.eclSubroutineId00;
        spawnPosition = &spawnPosition93;
        spawnScore = Th09EclRunControl::ReadInt(enemy, instruction, 6);
        spawnItemDrop = static_cast<signed char>(
            Th09EclRunControl::ReadInt(enemy, instruction, 5));
        spawnLife = Th09EclRunControl::ReadInt(enemy, instruction, 4);
        spawnContextVariables = Th09EclRunRemote::ActiveIntVariables(enemy);
        goto th09_ecl_spawn_enemy_tail;

    case TH09_ECL_OPCODE_SPAWN_ENEMY_RELATIVE:
        if (Th09EclRunRemote::Life(enemy) <= 0)
            break;
        memcpy(
            &spawnPacket94,
            &Th09EclRunControl::RawInt(instruction, 0),
            sizeof(spawnPacket94));
        Th09EclRunRemote::ResolveSpawnPosition(
            &spawnPosition94, enemy, instruction, spawnPacket94);
        Th09EclRunRemote::AddPosition(
            &spawnPosition94,
            &Th09EclRunMovement::View(enemy)->position2D74);
        spawnSubroutineId = spawnPacket94.eclSubroutineId00;
        spawnPosition = &spawnPosition94;
        spawnScore = Th09EclRunControl::ReadInt(enemy, instruction, 6);
        spawnItemDrop = static_cast<signed char>(
            Th09EclRunControl::ReadInt(enemy, instruction, 5));
        spawnLife = Th09EclRunControl::ReadInt(enemy, instruction, 4);
        spawnContextVariables = Th09EclRunRemote::ActiveIntVariables(enemy);

th09_ecl_spawn_enemy_tail:
        enemy->manager00->SpawnEnemy(
            static_cast<short>(spawnSubroutineId),
            spawnPosition,
            spawnLife,
            spawnItemDrop,
            spawnScore,
            spawnContextVariables,
            1);
        break;

    case TH09_ECL_OPCODE_KILL_ALL_NON_BOSS_ENEMIES:
        reinterpret_cast<Th09EclRunRemote::ManagerClearView *>(
            enemy->manager00)->KillAllNonBossEnemies(8000, 0);
        break;

#endif // TH09_ECL_RUN_REMOTE_BODY
