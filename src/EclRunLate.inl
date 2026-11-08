// TH09 v1.50a RunEcl late manager/side family (wire opcodes 158..187).
//
// This final wire-protocol fragment covers 21 active handlers and the nine
// target-default slots 158/164/168/174/176/179-181/184.  It is lexical source
// for the shared physical RunEcl owner, not a new callable subowner.
//
// Target evidence:
//   laser/manager state: 0x0040A662..0x0040AC90
//   cleanup/math/timers: 0x0040B875..0x0040BC99
//   side-owner protocol: 0x0040BCAD..0x0040BDD6
//
// Adjacent source supplied hypotheses only.  All offsets, masks, resolver
// repetition, side selection, ANM inputs and inactive slots below are fixed by
// TH09-local target evidence.

#ifndef TH09_ECL_RUN_LATE_DECLARATIONS
#define TH09_ECL_RUN_LATE_DECLARATIONS

#include "EclRunState.inl"
#include "BackgroundSpellStateView.hpp"

#include <stddef.h>

namespace Th09EclRunLate
{

enum EnemyLateFlag
{
    ENEMY_LATE_TIMER_PAUSED = 0x08000000,
    ENEMY_LATE_NO_DAMAGE_DURING_STOP = 0x10000000,
    ENEMY_LATE_EXTRA_ANM_FIXED_OFFSET = 0x00000020,
    ENEMY_LATE_SIDE_CATEGORY_MASK = 0x00000C00,
    ENEMY_LATE_SECONDARY_FLAG_4000 = 0x00004000
};

// These ECL late-script operands refer to the same concrete Background
// instance that runs the independently reconstructed stage script. Do not
// create a second 0xCE0-sized background layout in this lexical switch.
typedef char Th09EclLateBackgroundSideAnmAt81C[
    (offsetof(Background, sideAnm) == 0x81C) ? 1 : -1];
typedef char Th09EclLateBackgroundCountAtCD8[
    (offsetof(Background, spellVmCount) == 0xCD8) ? 1 : -1];
typedef char Th09EclLateBackgroundScriptAtCDC[
    (offsetof(Background, spellVmScriptBase) == 0xCDC) ? 1 : -1];
typedef char Th09EclLateBackgroundVmsAtCE0[
    (offsetof(Background, spellVms) == 0xCE0) ? 1 : -1];

// Reuse Enemy's concrete Player window, rather than an independent ECL-only
// reconstruction of +0x1B88 and +0x303F4.
typedef ::EnemyCorePlayerView PlayerLateView;
typedef char Th09EclLatePlayerPositionAt1B88[
    (offsetof(PlayerLateView, position1B88) == 0x1B88) ? 1 : -1];
typedef char Th09EclLatePlayerTableAt303F4[
    (offsetof(PlayerLateView, tableValue303F4) == 0x303F4) ? 1 : -1];

typedef ::CardAttack SideOwnerView;
typedef char Th09EclLateOwnerPrimaryAtA4[
    (offsetof(SideOwnerView, primaryPresentationCounterA4) == 0xA4 &&
     offsetof(SideOwnerView, secondaryPresentationCounterA8) == 0xA8) ? 1 : -1];

// ECL late instructions and Enemy calc both traverse the same side-state
// object.  Keep the canonical offsets in the shared native struct.
typedef ::EnemyCoreSideStateView SideStateView;
typedef char Th09EclLateSideSizeIs38[
    (sizeof(SideStateView) == 0x38) ? 1 : -1];
typedef char Th09EclLateSideBulletAt08[
    (offsetof(SideStateView, etama08) == 0x08) ? 1 : -1];
typedef char Th09EclLateSideOwnerAt14[
    (offsetof(SideStateView, owner14) == 0x14) ? 1 : -1];
// The late-opcode side/controller access is the same concrete 0x38-byte
// object used by the native EnemyManager and all other ECL families.  The
// private Core view is kept only where its wider Player semantics are needed.
typedef char Th09EclLateSharedNativeSideLayout[
    (offsetof(EnemySideStateView, background00) ==
         offsetof(SideStateView, background00) &&
     offsetof(EnemySideStateView, rewardPlayer04) ==
         offsetof(SideStateView, rewardPlayer04) &&
     offsetof(EnemySideStateView, etama08) ==
         offsetof(SideStateView, etama08) &&
     offsetof(EnemySideStateView, owner14) ==
         offsetof(SideStateView, owner14) &&
     offsetof(EnemySideStateView, flags34) ==
         offsetof(SideStateView, flags34)) ? 1 : -1];
typedef char Th09EclLateSharedPlayerWindow[
    (offsetof(EnemyRewardPlayerView, position1B88) ==
         offsetof(PlayerLateView, position1B88) &&
     offsetof(EnemyRewardPlayerView, tableValue303F4) ==
         offsetof(PlayerLateView, tableValue303F4)) ? 1 : -1];

// The late opcode family runs against the same actual Enemy manager record as
// EnemyManager::OnUpdate. Its two side protocol fields now live in that
// shared record rather than in a second almost-empty 2.8 MiB overlay.
typedef ::EnemyManagerView ManagerLateView;
typedef char Th09EclLateManagerProtocolAt2AC3DC[
    (offsetof(ManagerLateView, protocolValue2AC3DC) == 0x2AC3DC) ? 1 : -1];
typedef char Th09EclLateManagerTimelineAt2AC440[
    (offsetof(ManagerLateView, timelineSpawnsSuppressed2AC440) == 0x2AC440)
        ? 1
        : -1];
typedef char Th09EclLateSharedNativeManagerFields[
    (sizeof(ManagerLateView) == 0x2AC450 &&
     offsetof(ManagerLateView, sideState320) == 0x320 &&
     offsetof(ManagerLateView, protocolValue2AC3DC) == 0x2AC3DC &&
     offsetof(ManagerLateView, timelineSpawnsSuppressed2AC440) == 0x2AC440)
        ? 1 : -1];

inline ManagerLateView *Manager(EnemyView *enemy)
{
    return enemy->manager00;
}

inline EtamaController *BulletController(EnemyView *enemy)
{
    return Manager(enemy)->sideState320->etama08;
}

typedef char Th09EclLateMainVmRotationAt10[
    (offsetof(EnemyView, primaryAnmVm0008) +
     offsetof(AnmVm, rotation) + offsetof(Float3, z) == 0x0010) ? 1 : -1];

inline float &MainAnmRotation(EnemyView *enemy)
{
    return enemy->primaryAnmVm0008.rotation.z;
}

static void MoveRandomBiased(
    EnemyView *enemy,
    Th09EclRawInstructionHeaderView *instruction);

} // namespace Th09EclRunLate

#endif // TH09_ECL_RUN_LATE_DECLARATIONS

#if defined(TH09_ECL_RUN_LATE_BODY)

#if !defined(TH09_ECL_RUN_SHARED_SWITCH)
#error EclRunLate.inl must be included lexically inside RunEcl's switch
#endif

    int lateInt;
    int lateIndex;
    int lateScript;
    unsigned int lateCategory;
    float lateAngle;
    float lateMagnitude;
    float *lateFloatResult;
    Laser *lateLaser;

    case TH09_ECL_OPCODE_SET_DAMAGE_REDUCTION_TIMER:
        enemy->damageReductionTimer53A8 =
            Th09EclRunControl::ReadInt(enemy, instruction, 0);
        break;

    case TH09_ECL_OPCODE_REMOVE_BULLETS_IN_RADIUS:
        lateMagnitude = Th09EclRunControl::ReadFloat(enemy, instruction, 0);
        Th09EclRunLate::BulletController(enemy)->RemoveBulletsInRadius(
            reinterpret_cast<const Float3 *>(
                &enemy->worldPosition2DD4),
            lateMagnitude);
        break;

    case TH09_ECL_OPCODE_REMOVE_ALL_BULLETS:
        Th09EclRunLate::BulletController(enemy)->RemoveAllBullets(4);
        break;

    case TH09_ECL_OPCODE_SET_MAIN_ANM_ROTATION:
        Th09EclRunLate::MainAnmRotation(enemy) =
            Th09EclRunControl::ReadFloat(enemy, instruction, 0);
        break;

    case TH09_ECL_OPCODE_POLAR_TO_CARTESIAN_ALT:
        *Th09EclRunControl::WriteFloat(enemy, instruction, 1) =
            Th09EclRunControl::Sin(
                Th09EclRunControl::ReadFloat(enemy, instruction, 2)) *
            Th09EclRunControl::ReadFloat(enemy, instruction, 3);
        *Th09EclRunControl::WriteFloat(enemy, instruction, 0) =
            Th09EclRunControl::Cos(
                Th09EclRunControl::ReadFloat(enemy, instruction, 2)) *
            Th09EclRunControl::ReadFloat(enemy, instruction, 3);
        break;

    case TH09_ECL_OPCODE_RANDOM_HORIZONTAL_ANGLE:
        if ((enemy->manager00->sideState320->
                 rewardPlayer04->position1B88.x <
                 enemy->position2D74.x &&
             enemy->position2D74.x > 96.0f) ||
            enemy->position2D74.x > 288.0f)
        {
            lateFloatResult = Th09EclRunControl::WriteFloat(
                enemy,
                instruction,
                0);
            *lateFloatResult = Th09EclRunControl::AddNormalizeAngle(
                Th09EclRunControl::g_Rng.GetRandomF32InRange(1.5707964f) +
                    2.3561945f,
                0.0f);
        }
        else
        {
            lateFloatResult = Th09EclRunControl::WriteFloat(
                enemy,
                instruction,
                0);
            *lateFloatResult =
                Th09EclRunControl::g_Rng.GetRandomF32InRange(1.5707964f) -
                0.78539819f;
        }
        break;

    case TH09_ECL_OPCODE_SET_TIMER_PAUSED:
        lateInt = Th09EclRunControl::ReadInt(enemy, instruction, 0);
        Th09EclRunState::AssignFlagField(
            enemy->primaryFlags337C,
            Th09EclRunLate::ENEMY_LATE_TIMER_PAUSED,
            static_cast<unsigned int>(lateInt) << 27);
        break;

    case TH09_ECL_OPCODE_SET_NO_DAMAGE_DURING_STOP:
        lateInt = Th09EclRunControl::ReadInt(enemy, instruction, 0);
        Th09EclRunState::AssignFlagField(
            enemy->primaryFlags337C,
            Th09EclRunLate::ENEMY_LATE_NO_DAMAGE_DURING_STOP,
            static_cast<unsigned int>(lateInt) << 28);
        break;

    case TH09_ECL_OPCODE_SET_MINIMUM_PLAYER_DISTANCE:
        lateMagnitude = Th09EclRunControl::ReadFloat(enemy, instruction, 0);
        Th09EclRunMovement::View(enemy)->minimumPlayerDistanceSquared33A8 =
            lateMagnitude * lateMagnitude;
        break;

    case TH09_ECL_OPCODE_SET_TIMELINE_SPAWNS_SUPPRESSED:
        enemy->manager00->timelineSpawnsSuppressed2AC440 =
            Th09EclRunControl::ReadInt(enemy, instruction, 0);
        break;

    case TH09_ECL_OPCODE_SET_PHASE_STARTING_LIFE:
        enemy->phaseStartingLife2E50 =
            Th09EclRunControl::ReadInt(enemy, instruction, 0);
        break;

    case TH09_ECL_OPCODE_SET_EXTRA_ANM_FIXED_OFFSET:
        lateInt = Th09EclRunControl::ReadInt(enemy, instruction, 0);
        Th09EclRunState::AssignFlagField(
            enemy->secondaryFlags3380,
            Th09EclRunLate::ENEMY_LATE_EXTRA_ANM_FIXED_OFFSET,
            static_cast<unsigned int>(lateInt) << 5);
        break;

    case TH09_ECL_OPCODE_SET_SIDE_CATEGORY:
        lateInt = Th09EclRunControl::ReadInt(enemy, instruction, 0);
        Th09EclRunState::AssignFlagField(
            enemy->secondaryFlags3380,
            Th09EclRunLate::ENEMY_LATE_SIDE_CATEGORY_MASK,
            static_cast<unsigned int>(lateInt) << 10);
        break;

    case TH09_ECL_OPCODE_TRIGGER_SIDE_OWNER_STATE:
        lateCategory =
            (enemy->secondaryFlags3380 >> 10) & 3;
        if (lateCategory < 3)
        {
            if (enemy->manager00->sideState320->
                    owner14->primaryPresentationCounterA4 == 0)
                break;
            enemy->manager00->sideState320->
                owner14->primaryPresentationCounterA4 = 2;
        }
        else if (lateCategory == 3)
        {
            if (enemy->manager00->sideState320->
                    owner14->secondaryPresentationCounterA8 == 0)
                break;
            enemy->manager00->sideState320->
                owner14->secondaryPresentationCounterA8 = 2;
            reinterpret_cast<BackgroundSpellStateView *>(
                enemy->manager00->sideState320->background00)
                ->BeginSpellBackground();
            for (lateIndex = 0;
                 lateIndex <
                     enemy->manager00->sideState320->
                         background00->spellVmCount;
                 ++lateIndex)
            {
                lateScript = lateIndex +
                    enemy->manager00->opposingSideState324->
                        background00->spellVmScriptBase +
                    enemy->manager00->opposingSideState324->
                        rewardPlayer04->tableValue303F4;
                Th09EclRunMovement::SetAndExecuteAnmScript(
                    enemy->manager00->opposingSideState324->
                        background00->sideAnm,
                    &enemy->manager00->sideState320->
                        background00->spellVms[lateIndex],
                    lateScript);
            }
        }
        break;

    case TH09_ECL_OPCODE_SET_SECONDARY_FLAG_4000:
        lateInt = Th09EclRunControl::ReadInt(enemy, instruction, 0);
        Th09EclRunState::AssignFlagField(
            enemy->secondaryFlags3380,
            Th09EclRunLate::ENEMY_LATE_SECONDARY_FLAG_4000,
            static_cast<unsigned int>(lateInt) << 14);
        break;

#endif // TH09_ECL_RUN_LATE_BODY
