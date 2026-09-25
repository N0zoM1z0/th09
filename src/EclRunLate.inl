// TH09 v1.50a RunEcl late manager/side family (wire opcodes 158..187).
//
// This final wire-protocol fragment covers 21 active handlers and the nine
// target-default slots 158/164/168/174/176/179-181/184.  It is lexical source
// for the shared 14,792-byte RunEcl switch, not a new callable subowner.
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

struct BackgroundLateView
{
    unsigned char unknown000[0x818];
    void *stageAnm818;
    void *sideAnm81C;
    unsigned char unknown820[0xCD8 - 0x820];
    int spellVmCountCD8;
    int spellVmScriptBaseCDC;
    AnmVm spellVmsCE0[32];

    void StartSpellBackground();
};
typedef char Th09EclLateBackgroundSideAnmAt81C[
    (offsetof(BackgroundLateView, sideAnm81C) == 0x81C) ? 1 : -1];
typedef char Th09EclLateBackgroundCountAtCD8[
    (offsetof(BackgroundLateView, spellVmCountCD8) == 0xCD8) ? 1 : -1];
typedef char Th09EclLateBackgroundVmsAtCE0[
    (offsetof(BackgroundLateView, spellVmsCE0) == 0xCE0) ? 1 : -1];

struct PlayerLateView
{
    unsigned char unknown00000[0x1B88];
    EnemyFloat3 position1B88;
    unsigned char unknown01B94[0x303F4 - 0x1B94];
    int tableValue303F4;
};
typedef char Th09EclLatePlayerPositionAt1B88[
    (offsetof(PlayerLateView, position1B88) == 0x1B88) ? 1 : -1];
typedef char Th09EclLatePlayerTableAt303F4[
    (offsetof(PlayerLateView, tableValue303F4) == 0x303F4) ? 1 : -1];

struct SideOwnerView
{
    unsigned char unknown000[0xA4];
    int primaryStateA4;
    int alternateStateA8;
};
typedef char Th09EclLateOwnerPrimaryAtA4[
    (offsetof(SideOwnerView, primaryStateA4) == 0xA4) ? 1 : -1];

struct SideStateView
{
    BackgroundLateView *background00;
    PlayerLateView *player04;
    EtamaController *bulletController08;
    EffectManager *effectManager0C;
    EnemyManagerView *enemyManager10;
    SideOwnerView *owner14;
    unsigned char unknown18[0x20];
};
typedef char Th09EclLateSideSizeIs38[
    (sizeof(SideStateView) == 0x38) ? 1 : -1];
typedef char Th09EclLateSideBulletAt08[
    (offsetof(SideStateView, bulletController08) == 0x08) ? 1 : -1];
typedef char Th09EclLateSideOwnerAt14[
    (offsetof(SideStateView, owner14) == 0x14) ? 1 : -1];

struct ManagerLateView
{
    unsigned char unknown000000[0x000320];
    SideStateView *sideState320;
    SideStateView *opposingSideState324;
    unsigned char unknown000328[0x2AC3DC - 0x000328];
    int protocolValue2AC3DC;
    unsigned char unknown2AC3E0[0x2AC440 - 0x2AC3E0];
    int timelineSpawnsSuppressed2AC440;
};
typedef char Th09EclLateManagerProtocolAt2AC3DC[
    (offsetof(ManagerLateView, protocolValue2AC3DC) == 0x2AC3DC) ? 1 : -1];
typedef char Th09EclLateManagerTimelineAt2AC440[
    (offsetof(ManagerLateView, timelineSpawnsSuppressed2AC440) == 0x2AC440)
        ? 1
        : -1];

struct BulletControllerLateView
{
    void RemoveAllBullets(int mode);
    void RemoveBulletsInRadius(const Float3 *position, float radius);
};

inline Th09EclRunState::EnemyStateView *View(EnemyView *enemy)
{
    return Th09EclRunState::View(enemy);
}

inline ManagerLateView *Manager(EnemyView *enemy)
{
    return reinterpret_cast<ManagerLateView *>(View(enemy)->manager0000);
}

inline BulletControllerLateView *BulletController(EnemyView *enemy)
{
    return reinterpret_cast<BulletControllerLateView *>(
        Manager(enemy)->sideState320->bulletController08);
}

inline float &MainAnmRotation(EnemyView *enemy)
{
    return *reinterpret_cast<float *>(
        reinterpret_cast<unsigned char *>(enemy) + 0x0010);
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

    case TH09_ECL_OPCODE_UNHANDLED_9E:
        break;

    case TH09_ECL_OPCODE_SET_DRAW_GROUP:
        Th09EclRunLate::View(enemy)->drawGroup3387 =
            static_cast<unsigned char>(
                Th09EclRunControl::ReadInt(enemy, instruction, 0));
        break;

    case TH09_ECL_OPCODE_SET_DAMAGE_REDUCTION_TIMER:
        Th09EclRunLate::View(enemy)->damageReductionTimer53A8 =
            Th09EclRunControl::ReadInt(enemy, instruction, 0);
        break;

    case TH09_ECL_OPCODE_REMOVE_BULLETS_IN_RADIUS:
        lateMagnitude = Th09EclRunControl::ReadFloat(enemy, instruction, 0);
        Th09EclRunLate::BulletController(enemy)->RemoveBulletsInRadius(
            reinterpret_cast<const Float3 *>(
                &Th09EclRunLate::View(enemy)->worldPosition2DD4),
            lateMagnitude);
        break;

    case TH09_ECL_OPCODE_REMOVE_ALL_BULLETS:
        Th09EclRunLate::BulletController(enemy)->RemoveAllBullets(4);
        break;

    case TH09_ECL_OPCODE_SET_MANAGER_PROTOCOL_VALUE:
        Th09EclRunLate::Manager(enemy)->protocolValue2AC3DC =
            Th09EclRunControl::ReadInt(enemy, instruction, 0);
        break;

    case TH09_ECL_OPCODE_UNHANDLED_A4:
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

    case TH09_ECL_OPCODE_SET_LASER_ANGLE:
        lateIndex = Th09EclRunControl::ReadInt(enemy, instruction, 0);
        if (Th09EclRunLate::View(enemy)->laserSlots32D8[lateIndex])
        {
            Th09EclRunLate::View(enemy)->laserSlots32D8[lateIndex]->angle =
                Th09EclRunControl::ReadFloat(enemy, instruction, 1);
        }
        break;

    case TH09_ECL_OPCODE_UNHANDLED_A8:
        break;

    case TH09_ECL_OPCODE_RANDOM_HORIZONTAL_ANGLE:
        if ((Th09EclRunLate::Manager(enemy)->sideState320->
                 player04->position1B88.x >=
                 Th09EclRunLate::View(enemy)->position2D74.x ||
             Th09EclRunLate::View(enemy)->position2D74.x <= 96.0f) &&
            Th09EclRunLate::View(enemy)->position2D74.x <= 288.0f)
        {
            lateFloatResult = Th09EclRunControl::WriteFloat(
                enemy,
                instruction,
                0);
            *lateFloatResult =
                Th09EclRunControl::g_Rng.GetRandomF32InRange(1.5707964f) -
                0.78539819f;
        }
        else
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
        break;

    case TH09_ECL_OPCODE_SET_LASER_START_CAP_HIDDEN:
        lateIndex = Th09EclRunControl::ReadInt(enemy, instruction, 0);
        if (Th09EclRunLate::View(enemy)->laserSlots32D8[lateIndex])
        {
            Th09EclRunLate::View(enemy)->laserSlots32D8[lateIndex]->
                hideCapDuringStartup =
                    static_cast<unsigned char>(
                        Th09EclRunControl::ReadInt(enemy, instruction, 1));
        }
        break;

    case TH09_ECL_OPCODE_SET_LASER_START_LENGTH:
        lateIndex = Th09EclRunControl::ReadInt(enemy, instruction, 0);
        if (Th09EclRunLate::View(enemy)->laserSlots32D8[lateIndex])
        {
            Th09EclRunLate::View(enemy)->laserSlots32D8[lateIndex]->
                startLength =
                    Th09EclRunControl::ReadFloat(enemy, instruction, 1);
        }
        break;

    case TH09_ECL_OPCODE_SET_LASER_OFFSETS:
        lateIndex = Th09EclRunControl::ReadInt(enemy, instruction, 0);
        if (Th09EclRunLate::View(enemy)->laserSlots32D8[lateIndex])
        {
            Th09EclRunLate::View(enemy)->laserSlots32D8[lateIndex]->
                startOffset =
                    Th09EclRunControl::ReadFloat(enemy, instruction, 1);
            Th09EclRunLate::View(enemy)->laserSlots32D8[lateIndex]->
                endOffset =
                    Th09EclRunControl::ReadFloat(enemy, instruction, 2);
        }
        break;

    case TH09_ECL_OPCODE_SET_TIMER_PAUSED:
        lateInt = Th09EclRunControl::ReadInt(enemy, instruction, 0);
        Th09EclRunState::AssignFlagField(
            Th09EclRunLate::View(enemy)->primaryFlags337C,
            Th09EclRunLate::ENEMY_LATE_TIMER_PAUSED,
            static_cast<unsigned int>(lateInt) << 27);
        break;

    case TH09_ECL_OPCODE_UNHANDLED_AE:
        break;

    case TH09_ECL_OPCODE_SET_TIMELINE_SPAWNS_SUPPRESSED:
        Th09EclRunLate::Manager(enemy)->timelineSpawnsSuppressed2AC440 =
            Th09EclRunControl::ReadInt(enemy, instruction, 0);
        break;

    case TH09_ECL_OPCODE_UNHANDLED_B0:
        break;

    case TH09_ECL_OPCODE_SET_PHASE_STARTING_LIFE:
        Th09EclRunLate::View(enemy)->phaseStartingLife2E50 =
            Th09EclRunControl::ReadInt(enemy, instruction, 0);
        break;

    case TH09_ECL_OPCODE_MOVE_RANDOM_BIASED:
        Th09EclRunLate::MoveRandomBiased(enemy, instruction);
        break;

    case TH09_ECL_OPCODE_UNHANDLED_B3:
    case TH09_ECL_OPCODE_UNHANDLED_B4:
    case TH09_ECL_OPCODE_UNHANDLED_B5:
        break;

    case TH09_ECL_OPCODE_SET_EXTRA_ANM_FIXED_OFFSET:
        lateInt = Th09EclRunControl::ReadInt(enemy, instruction, 0);
        Th09EclRunState::AssignFlagField(
            Th09EclRunLate::View(enemy)->secondaryFlags3380,
            Th09EclRunLate::ENEMY_LATE_EXTRA_ANM_FIXED_OFFSET,
            static_cast<unsigned int>(static_cast<unsigned char>(lateInt)) <<
                5);
        break;

    case TH09_ECL_OPCODE_SET_NO_DAMAGE_DURING_STOP:
        lateInt = Th09EclRunControl::ReadInt(enemy, instruction, 0);
        Th09EclRunState::AssignFlagField(
            Th09EclRunLate::View(enemy)->primaryFlags337C,
            Th09EclRunLate::ENEMY_LATE_NO_DAMAGE_DURING_STOP,
            static_cast<unsigned int>(lateInt) << 28);
        break;

    case TH09_ECL_OPCODE_UNHANDLED_B8:
        break;

    case TH09_ECL_OPCODE_SET_SIDE_CATEGORY:
        lateInt = Th09EclRunControl::ReadInt(enemy, instruction, 0);
        Th09EclRunState::AssignFlagField(
            Th09EclRunLate::View(enemy)->secondaryFlags3380,
            Th09EclRunLate::ENEMY_LATE_SIDE_CATEGORY_MASK,
            static_cast<unsigned int>(lateInt) << 10);
        break;

    case TH09_ECL_OPCODE_TRIGGER_SIDE_OWNER_STATE:
        lateCategory =
            (Th09EclRunLate::View(enemy)->secondaryFlags3380 >> 10) & 3;
        if (lateCategory < 3)
        {
            if (Th09EclRunLate::Manager(enemy)->sideState320->
                    owner14->primaryStateA4 == 0)
                break;
            Th09EclRunLate::Manager(enemy)->sideState320->
                owner14->primaryStateA4 = 2;
        }
        else if (lateCategory == 3)
        {
            if (Th09EclRunLate::Manager(enemy)->sideState320->
                    owner14->alternateStateA8 == 0)
                break;
            Th09EclRunLate::Manager(enemy)->sideState320->
                owner14->alternateStateA8 = 2;
            Th09EclRunLate::Manager(enemy)->sideState320->
                background00->StartSpellBackground();
            for (lateIndex = 0;
                 lateIndex <
                     Th09EclRunLate::Manager(enemy)->sideState320->
                         background00->spellVmCountCD8;
                 ++lateIndex)
            {
                lateScript = lateIndex +
                    Th09EclRunLate::Manager(enemy)->opposingSideState324->
                        background00->spellVmScriptBaseCDC +
                    Th09EclRunLate::Manager(enemy)->opposingSideState324->
                        player04->tableValue303F4;
                Th09EclRunMovement::SetAndExecuteAnmScript(
                    Th09EclRunLate::Manager(enemy)->opposingSideState324->
                        background00->sideAnm81C,
                    &Th09EclRunLate::Manager(enemy)->sideState320->
                        background00->spellVmsCE0[lateIndex],
                    lateScript);
            }
        }
        break;

    case TH09_ECL_OPCODE_SET_SECONDARY_FLAG_4000:
        lateInt = Th09EclRunControl::ReadInt(enemy, instruction, 0);
        Th09EclRunState::AssignFlagField(
            Th09EclRunLate::View(enemy)->secondaryFlags3380,
            Th09EclRunLate::ENEMY_LATE_SECONDARY_FLAG_4000,
            static_cast<unsigned int>(lateInt) << 14);
        break;

#endif // TH09_ECL_RUN_LATE_BODY
