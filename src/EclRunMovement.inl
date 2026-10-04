// TH09 v1.50a RunEcl ANM/movement family (wire opcodes 54..82).
//
// This is a lexical switch fragment, not a callable subowner.  The target
// keeps these handlers inside the shared 14,792-byte EclManager::RunEcl stack
// frame.  The declarations intentionally use a family-local Enemy overlay so
// this fragment does not take ownership of the AnmManager interface or
// overstate the intentionally incomplete public Enemy layout.
//
// Target evidence:
//   family entries: opcodes 54..82 (all 29 active)
//   ANM helpers:    0x00403E00 / 0x00406740 / 0x00407B20
//   movement:       0x004070A0 / 0x00407320 / 0x00407C30
//   clamp/timer:    0x004101D0 / 0x00401500
//   player angle:   0x0041BB70
//
// Committed TH08 supplied source-shape hypotheses only.  Every operand path,
// field offset, flag transition, and helper role below was checked separately
// against the TH09 target.

#ifndef TH09_ECL_RUN_MOVEMENT_DECLARATIONS
#define TH09_ECL_RUN_MOVEMENT_DECLARATIONS

#include "AnmManager.hpp"
#include "EclRunControl.inl"

struct PlayerAngleFloat3View
{
    float x;
    float y;
    float z;
    operator float *();
};

struct PlayerAngleView
{
    unsigned char unknown000[0x1B88];
    PlayerAngleFloat3View position1B88;
    float AngleToPoint(PlayerAngleFloat3View *position);
};

namespace Th09EclRunMovement
{

struct EnemyFloat2
{
    float x;
    float y;
};

struct EnemyPrimaryFlagBits
{
    unsigned int unknown00 : 2;
    unsigned int collision02 : 1;
    unsigned int damageable03 : 1;
    unsigned int noSprite04 : 1;
    unsigned int unknown05 : 1;
    unsigned int acceptsDamage06 : 1;
    unsigned int unknown07 : 9;
    unsigned int clampPosition16 : 1;
    unsigned int unknown17 : 8;
    unsigned int allowOffscreen25 : 1;
    unsigned int unknown26 : 6;
};

struct EnemySecondaryFlagBits
{
    unsigned int unknown00 : 3;
    unsigned int noDeath03 : 1;
    unsigned int unknown04 : 28;
};

typedef char Th09EclEnemyPrimaryFlagBitsSizeIs04[
    (sizeof(EnemyPrimaryFlagBits) == 0x04) ? 1 : -1];
typedef char Th09EclEnemySecondaryFlagBitsSizeIs04[
    (sizeof(EnemySecondaryFlagBits) == 0x04) ? 1 : -1];

// Partial target-facing overlay used only by this opcode family.
struct EnemyMovementView
{
    void *manager0000;
    unsigned char unknown0004[0x0008 - 0x0004];
    unsigned char primaryAnmVm0008[0x2D74 - 0x0008];
    EnemyFloat3 position2D74;
    EnemyFloat3 positionOffset2D80;
    EnemyFloat3 velocity2D8C;
    unsigned char unknown2D98[0x2DBC - 0x2D98];
    EnemyFloat2 hitboxDimensions2DBC;
    unsigned char unknown2DC4[0x2DC8 - 0x2DC4];
    EnemyFloat2 secondaryHitboxDimensions2DC8;
    unsigned char unknown2DD0[0x2DD4 - 0x2DD0];
    EnemyFloat3 worldPosition2DD4;
    float movementAngle2DE0;
    float angularVelocity2DE4;
    float orbitAngle2DE8;
    float orbitAngularVelocity2DEC;
    unsigned char unknown2DF0[0x2DF4 - 0x2DF0];
    float speed2DF4;
    float acceleration2DF8;
    float orbitRadius2DFC;
    float radialVelocity2E00;
    EnemyFloat3 shootOffset2E04;
    EnemyFloat3 movementDelta2E10;
    EnemyFloat3 movementOrigin2E1C;
    unsigned char movementTimer2E28[0x0C];
    int movementDuration2E34;
    unsigned char unknown2E38[0x337C - 0x2E38];
    unsigned int primaryFlags337C;
    unsigned int secondaryFlags3380;
    unsigned char unknown3384[0x02];
    signed char anmDirection3386;
    unsigned char unknown3387[0x03];
    short primaryAnmScript0338A;
    short primaryAnmScript3338C;
    short primaryAnmScript4338E;
    short primaryAnmScript13390;
    short primaryAnmScript23392;
    short specialAnmScript3394;
    unsigned char unknown3396[0x3398 - 0x3396];
    EnemyFloat2 movementLowerBounds3398;
    EnemyFloat2 movementUpperBounds33A0;
    float minimumPlayerDistanceSquared33A8;
};

typedef char Th09EclEnemyMovementAngleAt2DE0[
    (offsetof(EnemyMovementView, movementAngle2DE0) == 0x2DE0) ? 1 : -1];
typedef char Th09EclEnemyMovementTimerAt2E28[
    (offsetof(EnemyMovementView, movementTimer2E28) == 0x2E28) ? 1 : -1];
typedef char Th09EclEnemyPrimaryFlagsAt337C[
    (offsetof(EnemyMovementView, primaryFlags337C) == 0x337C) ? 1 : -1];
typedef char Th09EclEnemySpecialAnmAt3394[
    (offsetof(EnemyMovementView, specialAnmScript3394) == 0x3394) ? 1 : -1];
typedef char Th09EclEnemyMovementBoundsAt3398[
    (offsetof(EnemyMovementView, movementLowerBounds3398) == 0x3398) ? 1 : -1];
typedef char Th09EclEnemyMinimumDistanceAt33A8[
    (offsetof(EnemyMovementView, minimumPlayerDistanceSquared33A8) == 0x33A8)
        ? 1
        : -1];

enum EnemyMovementFlag
{
    ENEMY_MOVEMENT_MODE_POLAR = 0x00000200,
    ENEMY_MOVEMENT_MODE_ORBIT = 0x00000600,
    ENEMY_MOVEMENT_MODE_MASK = 0x00000600,
    ENEMY_CLAMP_POSITION = 0x00010000,
    ENEMY_ALTERNATE_ANM_BANK = 0x80000000
};

enum EclInteractionFlag
{
    ECL_INTERACTION_ACCEPTS_DAMAGE = 0x01,
    ECL_INTERACTION_COLLISION = 0x02,
    ECL_INTERACTION_DAMAGEABLE = 0x04,
    ECL_INTERACTION_NO_SPRITE = 0x08,
    ECL_INTERACTION_ALLOW_OFFSCREEN = 0x10,
    ECL_INTERACTION_NO_DEATH = 0x20
};

inline EnemyMovementView *View(EnemyView *enemy)
{
    return reinterpret_cast<EnemyMovementView *>(enemy);
}

inline void *PrimaryAnm(EnemyView *enemy)
{
    return *reinterpret_cast<void **>(
        reinterpret_cast<unsigned char *>(enemy->manager00) + 0x2AC420);
}

inline void *AlternateAnm(EnemyView *enemy)
{
    return *reinterpret_cast<void **>(
        reinterpret_cast<unsigned char *>(enemy->manager00) + 0x2AC424);
}

inline void *PrimaryAnmVm(EnemyView *enemy)
{
    return reinterpret_cast<unsigned char *>(enemy) + 0x0008;
}

inline void *Player(EnemyView *enemy)
{
    unsigned char *side = *reinterpret_cast<unsigned char **>(
        reinterpret_cast<unsigned char *>(enemy->manager00) + 0x0320);
    return *reinterpret_cast<void **>(side + 0x0004);
}

inline EnemyPrimaryFlagBits *PrimaryFlagBits(EnemyView *enemy)
{
    return reinterpret_cast<EnemyPrimaryFlagBits *>(
        &View(enemy)->primaryFlags337C);
}

inline EnemySecondaryFlagBits *SecondaryFlagBits(EnemyView *enemy)
{
    return reinterpret_cast<EnemySecondaryFlagBits *>(
        &View(enemy)->secondaryFlags3380);
}

// Target-facing helper boundaries. Source ownership and private/public ABI are
// tracked by the live function ledger and canonical match units; do not infer
// unresolved ABI status from older packet-era comments.
__forceinline void SetAndExecuteAnmScript(
    void *anmFile, void *vm, int scriptIndex)
{
    reinterpret_cast<AnmLoaded *>(anmFile)->SetAndExecuteScriptIdx(
        reinterpret_cast<AnmVm *>(vm), scriptIndex);
}
static void SetPrimaryAnmScripts(
    EnemyView *enemy,
    Th09EclRawInstructionHeaderView *instruction,
    int script0,
    int script1,
    int script2,
    int script3,
    int script4,
    int specialScript);
static void SetExtraAnmScript(
    EnemyView *enemy,
    Th09EclRawInstructionHeaderView *instruction);
static void ConfigurePolarMotion(
    EnemyView *enemy,
    Th09EclRawInstructionHeaderView *instruction);
static void ConfigureRelativeMotion(
    EnemyView *enemy,
    Th09EclRawInstructionHeaderView *instruction);
static void BeginBoundaryAwareMove(
    EnemyView *enemy,
    Th09EclRawInstructionHeaderView *instruction);
void ClampPosition(EnemyView *enemy);

} // namespace Th09EclRunMovement

#endif // TH09_ECL_RUN_MOVEMENT_DECLARATIONS

#if defined(TH09_ECL_RUN_MOVEMENT_BODY)

#if !defined(TH09_ECL_RUN_SHARED_SWITCH)
#error EclRunMovement.inl must be included lexically inside RunEcl's switch
#endif

// Keep these pure accessors lexical within the movement switch family.
// Each argument is evaluated once; loaded pointer values are not cached.
// The explicit pointer value casts preserve the helper return categories.
// This is an empirically checked VC7.1 source form, not proof of original macros.
#define TH09_EXPR_MOVEMENT_VIEW(owner) \
    (static_cast<Th09EclRunMovement::EnemyMovementView *>(reinterpret_cast<Th09EclRunMovement::EnemyMovementView *>((owner))))
#define TH09_EXPR_MOVEMENT_PRIMARY_ANM(owner) \
    (static_cast<void *>(*reinterpret_cast<void **>(reinterpret_cast<unsigned char *>((owner)->manager00) + 0x2AC420)))
#define TH09_EXPR_MOVEMENT_ALTERNATE_ANM(owner) \
    (static_cast<void *>(*reinterpret_cast<void **>(reinterpret_cast<unsigned char *>((owner)->manager00) + 0x2AC424)))
#define TH09_EXPR_MOVEMENT_PRIMARY_ANM_VM(owner) \
    (static_cast<void *>(reinterpret_cast<unsigned char *>((owner)) + 0x0008))
#define TH09_EXPR_MOVEMENT_PLAYER(owner) \
    (static_cast<void *>(*reinterpret_cast<void **>((*reinterpret_cast<unsigned char **>(reinterpret_cast<unsigned char *>((owner)->manager00) + 0x0320)) + 0x0004)))
#define TH09_EXPR_MOVEMENT_PRIMARY_FLAG_BITS(owner) \
    (static_cast<Th09EclRunMovement::EnemyPrimaryFlagBits *>(reinterpret_cast<Th09EclRunMovement::EnemyPrimaryFlagBits *>(&(reinterpret_cast<Th09EclRunMovement::EnemyMovementView *>((owner)))->primaryFlags337C)))
#define TH09_EXPR_MOVEMENT_SECONDARY_FLAG_BITS(owner) \
    (static_cast<Th09EclRunMovement::EnemySecondaryFlagBits *>(reinterpret_cast<Th09EclRunMovement::EnemySecondaryFlagBits *>(&(reinterpret_cast<Th09EclRunMovement::EnemyMovementView *>((owner)))->secondaryFlags3380)))

    int movementInt;
    float aimedAngleOffset;
    float aimedMoveAngleOffset;

    case TH09_ECL_OPCODE_SET_MAIN_ANM:
        Th09EclRunMovement::SetAndExecuteAnmScript(
            TH09_EXPR_MOVEMENT_PRIMARY_ANM(enemy),
            TH09_EXPR_MOVEMENT_PRIMARY_ANM_VM(enemy),
            Th09EclRunControl::ReadInt(enemy, instruction, 0));
        TH09_EXPR_MOVEMENT_VIEW(enemy)->primaryFlags337C &=
            ~Th09EclRunMovement::ENEMY_ALTERNATE_ANM_BANK;
        break;

    case TH09_ECL_OPCODE_SET_PRIMARY_ANM_SEQUENCE:
        movementInt = Th09EclRunControl::ReadInt(enemy, instruction, 0);
        Th09EclRunMovement::SetPrimaryAnmScripts(
            enemy, instruction,
            movementInt, movementInt + 1, movementInt + 2,
            movementInt + 3, movementInt + 4, movementInt + 5);
        TH09_EXPR_MOVEMENT_VIEW(enemy)->primaryFlags337C &=
            ~Th09EclRunMovement::ENEMY_ALTERNATE_ANM_BANK;
        break;

    case TH09_ECL_OPCODE_SET_PRIMARY_ANM_SCRIPTS:
        Th09EclRunMovement::SetPrimaryAnmScripts(
            enemy, instruction,
            Th09EclRunControl::ReadInt(enemy, instruction, 0),
            Th09EclRunControl::ReadInt(enemy, instruction, 1),
            Th09EclRunControl::ReadInt(enemy, instruction, 2),
            Th09EclRunControl::ReadInt(enemy, instruction, 3),
            Th09EclRunControl::ReadInt(enemy, instruction, 4),
            Th09EclRunControl::ReadInt(enemy, instruction, 5));
        TH09_EXPR_MOVEMENT_VIEW(enemy)->primaryFlags337C &=
            ~Th09EclRunMovement::ENEMY_ALTERNATE_ANM_BANK;
        break;

    case TH09_ECL_OPCODE_SET_EXTRA_ANM_SCRIPT:
        Th09EclRunMovement::SetExtraAnmScript(enemy, instruction);
        TH09_EXPR_MOVEMENT_VIEW(enemy)->primaryFlags337C &=
            ~Th09EclRunMovement::ENEMY_ALTERNATE_ANM_BANK;
        break;

    case TH09_ECL_OPCODE_SET_MAIN_ANM_ALTERNATE:
        Th09EclRunMovement::SetAndExecuteAnmScript(
            TH09_EXPR_MOVEMENT_ALTERNATE_ANM(enemy),
            TH09_EXPR_MOVEMENT_PRIMARY_ANM_VM(enemy),
            Th09EclRunControl::ReadInt(enemy, instruction, 0));
        TH09_EXPR_MOVEMENT_VIEW(enemy)->primaryFlags337C |=
            Th09EclRunMovement::ENEMY_ALTERNATE_ANM_BANK;
        break;

    case TH09_ECL_OPCODE_SET_PRIMARY_ANM_SEQUENCE_ALTERNATE:
        movementInt = Th09EclRunControl::ReadInt(enemy, instruction, 0);
        Th09EclRunMovement::SetPrimaryAnmScripts(
            enemy, instruction,
            movementInt, movementInt + 1, movementInt + 2,
            movementInt + 3, movementInt + 4, movementInt + 5);
        TH09_EXPR_MOVEMENT_VIEW(enemy)->primaryFlags337C |=
            Th09EclRunMovement::ENEMY_ALTERNATE_ANM_BANK;
        break;

    case TH09_ECL_OPCODE_SET_PRIMARY_ANM_SCRIPTS_ALTERNATE:
        Th09EclRunMovement::SetPrimaryAnmScripts(
            enemy, instruction,
            Th09EclRunControl::ReadInt(enemy, instruction, 0),
            Th09EclRunControl::ReadInt(enemy, instruction, 1),
            Th09EclRunControl::ReadInt(enemy, instruction, 2),
            Th09EclRunControl::ReadInt(enemy, instruction, 3),
            Th09EclRunControl::ReadInt(enemy, instruction, 4),
            Th09EclRunControl::ReadInt(enemy, instruction, 5));
        TH09_EXPR_MOVEMENT_VIEW(enemy)->primaryFlags337C |=
            Th09EclRunMovement::ENEMY_ALTERNATE_ANM_BANK;
        break;

    case TH09_ECL_OPCODE_SET_EXTRA_ANM_SCRIPT_ALTERNATE:
        TH09_EXPR_MOVEMENT_VIEW(enemy)->primaryFlags337C |=
            Th09EclRunMovement::ENEMY_ALTERNATE_ANM_BANK;
        Th09EclRunMovement::SetExtraAnmScript(enemy, instruction);
        break;

    case TH09_ECL_OPCODE_PLAY_SPECIAL_ANM:
        if ((TH09_EXPR_MOVEMENT_VIEW(enemy)->primaryFlags337C &
             Th09EclRunMovement::ENEMY_ALTERNATE_ANM_BANK) == 0)
        {
            Th09EclRunMovement::SetAndExecuteAnmScript(
                TH09_EXPR_MOVEMENT_PRIMARY_ANM(enemy),
                TH09_EXPR_MOVEMENT_PRIMARY_ANM_VM(enemy),
                TH09_EXPR_MOVEMENT_VIEW(enemy)->specialAnmScript3394);
        }
        else
        {
            Th09EclRunMovement::SetAndExecuteAnmScript(
                TH09_EXPR_MOVEMENT_ALTERNATE_ANM(enemy),
                TH09_EXPR_MOVEMENT_PRIMARY_ANM_VM(enemy),
                TH09_EXPR_MOVEMENT_VIEW(enemy)->specialAnmScript3394);
        }
        break;

    case TH09_ECL_OPCODE_SET_POSITION:
        TH09_EXPR_MOVEMENT_VIEW(enemy)->position2D74.x =
            Th09EclRunControl::ReadFloat(enemy, instruction, 0);
        TH09_EXPR_MOVEMENT_VIEW(enemy)->position2D74.y =
            Th09EclRunControl::ReadFloat(enemy, instruction, 1);
        TH09_EXPR_MOVEMENT_VIEW(enemy)->position2D74.z = 0.0f;
        Th09EclRunMovement::ClampPosition(enemy);
        break;

    case TH09_ECL_OPCODE_MOVE_TO:
        Th09EclRunMovement::ConfigureRelativeMotion(enemy, instruction);
        break;

    case TH09_ECL_OPCODE_SET_DIRECTION_AND_SPEED:
        TH09_EXPR_MOVEMENT_VIEW(enemy)->movementAngle2DE0 =
            Th09EclRunControl::AddNormalizeAngle(
                Th09EclRunControl::ReadFloat(enemy, instruction, 0), 0.0f);
        TH09_EXPR_MOVEMENT_VIEW(enemy)->speed2DF4 =
            Th09EclRunControl::ReadFloat(enemy, instruction, 1);
        TH09_EXPR_MOVEMENT_VIEW(enemy)->primaryFlags337C =
            (TH09_EXPR_MOVEMENT_VIEW(enemy)->primaryFlags337C &
             ~Th09EclRunMovement::ENEMY_MOVEMENT_MODE_MASK) |
            Th09EclRunMovement::ENEMY_MOVEMENT_MODE_POLAR;
        TH09_EXPR_MOVEMENT_VIEW(enemy)->movementDuration2E34 = 0;
        *reinterpret_cast<Th09EclTimerStorageView *>(
            TH09_EXPR_MOVEMENT_VIEW(enemy)->movementTimer2E28) = 0;
        break;

    case TH09_ECL_OPCODE_MOVE_IN_DIRECTION:
        if (Th09EclRunControl::ReadInt(enemy, instruction, 0) <= 0)
        {
            TH09_EXPR_MOVEMENT_VIEW(enemy)->movementAngle2DE0 =
                Th09EclRunControl::AddNormalizeAngle(
                    Th09EclRunControl::ReadFloat(enemy, instruction, 2), 0.0f);
            TH09_EXPR_MOVEMENT_VIEW(enemy)->speed2DF4 =
                Th09EclRunControl::ReadFloat(enemy, instruction, 3);
            TH09_EXPR_MOVEMENT_VIEW(enemy)->primaryFlags337C =
                (TH09_EXPR_MOVEMENT_VIEW(enemy)->primaryFlags337C &
                 ~Th09EclRunMovement::ENEMY_MOVEMENT_MODE_MASK) |
                Th09EclRunMovement::ENEMY_MOVEMENT_MODE_POLAR;
            TH09_EXPR_MOVEMENT_VIEW(enemy)->movementDuration2E34 = 0;
            *reinterpret_cast<Th09EclTimerStorageView *>(
                TH09_EXPR_MOVEMENT_VIEW(enemy)->movementTimer2E28) = 0;
        }
        else
        {
            Th09EclRunMovement::ConfigurePolarMotion(enemy, instruction);
        }
        break;

    case TH09_ECL_OPCODE_MOVE_RANDOM_IN_BOUNDS:
        Th09EclRunMovement::BeginBoundaryAwareMove(enemy, instruction);
        break;

    case TH09_ECL_OPCODE_MOVE_RANDOM_BIASED:
        Th09EclRunLate::MoveRandomBiased(enemy, instruction);
        break;

    case TH09_ECL_OPCODE_SET_AIMED_DIRECTION_AND_SPEED:
        aimedAngleOffset = Th09EclRunControl::ReadFloat(enemy, instruction, 0);
        TH09_EXPR_MOVEMENT_VIEW(enemy)->movementAngle2DE0 =
            Th09EclRunControl::AddNormalizeAngle(
                aimedAngleOffset,
                reinterpret_cast<PlayerAngleView *>(
                    TH09_EXPR_MOVEMENT_PLAYER(enemy))->AngleToPoint(
                        reinterpret_cast<PlayerAngleFloat3View *>(
                            &TH09_EXPR_MOVEMENT_VIEW(enemy)->position2D74)));
        TH09_EXPR_MOVEMENT_VIEW(enemy)->speed2DF4 =
            Th09EclRunControl::ReadFloat(enemy, instruction, 1);
        break;

    case TH09_ECL_OPCODE_MOVE_IN_AIMED_DIRECTION:
        if (Th09EclRunControl::ReadInt(enemy, instruction, 0) <= 0)
        {
            aimedMoveAngleOffset =
                Th09EclRunControl::ReadFloat(enemy, instruction, 2);
            TH09_EXPR_MOVEMENT_VIEW(enemy)->movementAngle2DE0 =
                Th09EclRunControl::AddNormalizeAngle(
                    aimedMoveAngleOffset,
                    reinterpret_cast<PlayerAngleView *>(
                        TH09_EXPR_MOVEMENT_PLAYER(enemy))->AngleToPoint(
                            reinterpret_cast<PlayerAngleFloat3View *>(
                                &TH09_EXPR_MOVEMENT_VIEW(enemy)->position2D74)));
            TH09_EXPR_MOVEMENT_VIEW(enemy)->speed2DF4 =
                Th09EclRunControl::ReadFloat(enemy, instruction, 3);
            TH09_EXPR_MOVEMENT_VIEW(enemy)->primaryFlags337C =
                (TH09_EXPR_MOVEMENT_VIEW(enemy)->primaryFlags337C &
                 ~Th09EclRunMovement::ENEMY_MOVEMENT_MODE_MASK) |
                Th09EclRunMovement::ENEMY_MOVEMENT_MODE_POLAR;
            movementInt = Th09EclRunControl::ReadInt(enemy, instruction, 0);
            TH09_EXPR_MOVEMENT_VIEW(enemy)->movementDuration2E34 = movementInt;
            *reinterpret_cast<Th09EclTimerStorageView *>(
                TH09_EXPR_MOVEMENT_VIEW(enemy)->movementTimer2E28) =
                movementInt;
        }
        else
        {
            Th09EclRunMovement::ConfigurePolarMotion(enemy, instruction);
        }
        break;

    case TH09_ECL_OPCODE_SET_ANGULAR_VELOCITY:
        TH09_EXPR_MOVEMENT_VIEW(enemy)->angularVelocity2DE4 =
            Th09EclRunControl::ReadFloat(enemy, instruction, 0);
        TH09_EXPR_MOVEMENT_VIEW(enemy)->primaryFlags337C =
            (TH09_EXPR_MOVEMENT_VIEW(enemy)->primaryFlags337C &
             ~Th09EclRunMovement::ENEMY_MOVEMENT_MODE_MASK) |
            Th09EclRunMovement::ENEMY_MOVEMENT_MODE_POLAR;
        break;

    case TH09_ECL_OPCODE_SET_ACCELERATION:
        TH09_EXPR_MOVEMENT_VIEW(enemy)->acceleration2DF8 =
            Th09EclRunControl::ReadFloat(enemy, instruction, 0);
        TH09_EXPR_MOVEMENT_VIEW(enemy)->primaryFlags337C =
            (TH09_EXPR_MOVEMENT_VIEW(enemy)->primaryFlags337C &
             ~Th09EclRunMovement::ENEMY_MOVEMENT_MODE_MASK) |
            Th09EclRunMovement::ENEMY_MOVEMENT_MODE_POLAR;
        break;

    case TH09_ECL_OPCODE_ORBIT_AROUND_POINT:
        movementInt = Th09EclRunControl::ReadInt(enemy, instruction, 0);
        TH09_EXPR_MOVEMENT_VIEW(enemy)->movementDuration2E34 = movementInt;
        *reinterpret_cast<Th09EclTimerStorageView *>(
            TH09_EXPR_MOVEMENT_VIEW(enemy)->movementTimer2E28) =
            movementInt;
        TH09_EXPR_MOVEMENT_VIEW(enemy)->movementOrigin2E1C.x =
            Th09EclRunControl::ReadFloat(enemy, instruction, 1);
        TH09_EXPR_MOVEMENT_VIEW(enemy)->movementOrigin2E1C.y =
            Th09EclRunControl::ReadFloat(enemy, instruction, 2);
        TH09_EXPR_MOVEMENT_VIEW(enemy)->orbitAngle2DE8 =
            Th09EclRunControl::ReadFloat(enemy, instruction, 3);
        TH09_EXPR_MOVEMENT_VIEW(enemy)->orbitAngularVelocity2DEC =
            Th09EclRunControl::ReadFloat(enemy, instruction, 4);
        TH09_EXPR_MOVEMENT_VIEW(enemy)->orbitRadius2DFC =
            Th09EclRunControl::ReadFloat(enemy, instruction, 5);
        TH09_EXPR_MOVEMENT_VIEW(enemy)->radialVelocity2E00 =
            Th09EclRunControl::ReadFloat(enemy, instruction, 6);
        TH09_EXPR_MOVEMENT_VIEW(enemy)->primaryFlags337C |=
            Th09EclRunMovement::ENEMY_MOVEMENT_MODE_ORBIT;
        break;

    case TH09_ECL_OPCODE_ORBIT_AROUND_CURRENT_POSITION:
        movementInt = Th09EclRunControl::ReadInt(enemy, instruction, 0);
        TH09_EXPR_MOVEMENT_VIEW(enemy)->movementDuration2E34 = movementInt;
        *reinterpret_cast<Th09EclTimerStorageView *>(
            TH09_EXPR_MOVEMENT_VIEW(enemy)->movementTimer2E28) =
            movementInt;
        TH09_EXPR_MOVEMENT_VIEW(enemy)->movementOrigin2E1C =
            TH09_EXPR_MOVEMENT_VIEW(enemy)->position2D74;
        TH09_EXPR_MOVEMENT_VIEW(enemy)->orbitAngle2DE8 =
            Th09EclRunControl::ReadFloat(enemy, instruction, 1);
        TH09_EXPR_MOVEMENT_VIEW(enemy)->orbitAngularVelocity2DEC =
            Th09EclRunControl::ReadFloat(enemy, instruction, 2);
        TH09_EXPR_MOVEMENT_VIEW(enemy)->orbitRadius2DFC = 0.0f;
        TH09_EXPR_MOVEMENT_VIEW(enemy)->radialVelocity2E00 =
            Th09EclRunControl::ReadFloat(enemy, instruction, 3);
        TH09_EXPR_MOVEMENT_VIEW(enemy)->primaryFlags337C |=
            Th09EclRunMovement::ENEMY_MOVEMENT_MODE_ORBIT;
        break;

    case TH09_ECL_OPCODE_SET_ORBIT_VELOCITIES:
        movementInt = Th09EclRunControl::ReadInt(enemy, instruction, 0);
        TH09_EXPR_MOVEMENT_VIEW(enemy)->movementDuration2E34 = movementInt;
        *reinterpret_cast<Th09EclTimerStorageView *>(
            TH09_EXPR_MOVEMENT_VIEW(enemy)->movementTimer2E28) =
            movementInt;
        TH09_EXPR_MOVEMENT_VIEW(enemy)->orbitAngularVelocity2DEC =
            Th09EclRunControl::ReadFloat(enemy, instruction, 1);
        TH09_EXPR_MOVEMENT_VIEW(enemy)->radialVelocity2E00 =
            Th09EclRunControl::ReadFloat(enemy, instruction, 2);
        TH09_EXPR_MOVEMENT_VIEW(enemy)->primaryFlags337C |=
            Th09EclRunMovement::ENEMY_MOVEMENT_MODE_ORBIT;
        break;

    case TH09_ECL_OPCODE_SET_MOVEMENT_BOUNDS:
        TH09_EXPR_MOVEMENT_VIEW(enemy)->movementLowerBounds3398.x =
            Th09EclRunControl::ReadFloat(enemy, instruction, 0);
        TH09_EXPR_MOVEMENT_VIEW(enemy)->movementLowerBounds3398.y =
            Th09EclRunControl::ReadFloat(enemy, instruction, 1);
        TH09_EXPR_MOVEMENT_VIEW(enemy)->movementUpperBounds33A0.x =
            Th09EclRunControl::ReadFloat(enemy, instruction, 2);
        TH09_EXPR_MOVEMENT_VIEW(enemy)->movementUpperBounds33A0.y =
            Th09EclRunControl::ReadFloat(enemy, instruction, 3);
        TH09_EXPR_MOVEMENT_VIEW(enemy)->primaryFlags337C |=
            Th09EclRunMovement::ENEMY_CLAMP_POSITION;
        break;

    case TH09_ECL_OPCODE_DISABLE_MOVEMENT_BOUNDS:
        TH09_EXPR_MOVEMENT_VIEW(enemy)->primaryFlags337C &=
            ~Th09EclRunMovement::ENEMY_CLAMP_POSITION;
        break;

    case TH09_ECL_OPCODE_SET_HITBOX:
        TH09_EXPR_MOVEMENT_VIEW(enemy)->hitboxDimensions2DBC.x =
            Th09EclRunControl::ReadFloat(enemy, instruction, 0);
        TH09_EXPR_MOVEMENT_VIEW(enemy)->hitboxDimensions2DBC.y =
            Th09EclRunControl::ReadFloat(enemy, instruction, 1);
        break;

    case TH09_ECL_OPCODE_SET_SECONDARY_HITBOX:
        TH09_EXPR_MOVEMENT_VIEW(enemy)->secondaryHitboxDimensions2DC8.x =
            Th09EclRunControl::ReadFloat(enemy, instruction, 0);
        TH09_EXPR_MOVEMENT_VIEW(enemy)->secondaryHitboxDimensions2DC8.y =
            Th09EclRunControl::ReadFloat(enemy, instruction, 1);
        break;

    case TH09_ECL_OPCODE_SET_INTERACTION_FLAGS:
    {
        // Take both word snapshots after operand resolution, which may update
        // enemy state. The masks preserve every unrelated bit; the first three
        // interaction inputs have the original inverted polarity.
        movementInt = Th09EclRunControl::ReadInt(enemy, instruction, 0);
        const unsigned int previousSecondary =
            TH09_EXPR_MOVEMENT_VIEW(enemy)->secondaryFlags3380;
        const unsigned int previousPrimary =
            TH09_EXPR_MOVEMENT_VIEW(enemy)->primaryFlags337C;
        TH09_EXPR_MOVEMENT_VIEW(enemy)->primaryFlags337C =
            (previousPrimary & ~0x0200005CU) |
            (static_cast<unsigned int>(
                (movementInt & Th09EclRunMovement::ECL_INTERACTION_ACCEPTS_DAMAGE)
                    == 0) << 6) |
            (static_cast<unsigned int>(
                (movementInt & Th09EclRunMovement::ECL_INTERACTION_COLLISION)
                    == 0) << 2) |
            (static_cast<unsigned int>(
                (movementInt & Th09EclRunMovement::ECL_INTERACTION_DAMAGEABLE)
                    == 0) << 3) |
            (static_cast<unsigned int>(
                (movementInt & Th09EclRunMovement::ECL_INTERACTION_NO_SPRITE)
                    != 0) << 4) |
            (static_cast<unsigned int>(
                (movementInt & Th09EclRunMovement::ECL_INTERACTION_ALLOW_OFFSCREEN)
                    != 0) << 25);
        TH09_EXPR_MOVEMENT_VIEW(enemy)->secondaryFlags3380 =
            (previousSecondary & ~0x08U) |
            (static_cast<unsigned int>(
                (movementInt & Th09EclRunMovement::ECL_INTERACTION_NO_DEATH)
                    != 0) << 3);
        break;
    }

    case TH09_ECL_OPCODE_DISABLE_INTERACTION_FLAGS:
        movementInt = Th09EclRunControl::ReadInt(enemy, instruction, 0);
        if (movementInt & Th09EclRunMovement::ECL_INTERACTION_ACCEPTS_DAMAGE)
            TH09_EXPR_MOVEMENT_PRIMARY_FLAG_BITS(enemy)->acceptsDamage06 = 0;
        if (movementInt & Th09EclRunMovement::ECL_INTERACTION_COLLISION)
            TH09_EXPR_MOVEMENT_PRIMARY_FLAG_BITS(enemy)->collision02 = 0;
        if (movementInt & Th09EclRunMovement::ECL_INTERACTION_DAMAGEABLE)
            TH09_EXPR_MOVEMENT_PRIMARY_FLAG_BITS(enemy)->damageable03 = 0;
        if (movementInt & Th09EclRunMovement::ECL_INTERACTION_NO_SPRITE)
            TH09_EXPR_MOVEMENT_PRIMARY_FLAG_BITS(enemy)->noSprite04 = 1;
        if (movementInt & Th09EclRunMovement::ECL_INTERACTION_ALLOW_OFFSCREEN)
            TH09_EXPR_MOVEMENT_PRIMARY_FLAG_BITS(enemy)->allowOffscreen25 = 1;
        if (movementInt & Th09EclRunMovement::ECL_INTERACTION_NO_DEATH)
            TH09_EXPR_MOVEMENT_SECONDARY_FLAG_BITS(enemy)->noDeath03 = 1;
        break;

    case TH09_ECL_OPCODE_ENABLE_INTERACTION_FLAGS:
        movementInt = Th09EclRunControl::ReadInt(enemy, instruction, 0);
        if (movementInt & Th09EclRunMovement::ECL_INTERACTION_ACCEPTS_DAMAGE)
            TH09_EXPR_MOVEMENT_PRIMARY_FLAG_BITS(enemy)->acceptsDamage06 = 1;
        if (movementInt & Th09EclRunMovement::ECL_INTERACTION_COLLISION)
            TH09_EXPR_MOVEMENT_PRIMARY_FLAG_BITS(enemy)->collision02 = 1;
        if (movementInt & Th09EclRunMovement::ECL_INTERACTION_DAMAGEABLE)
            TH09_EXPR_MOVEMENT_PRIMARY_FLAG_BITS(enemy)->damageable03 = 1;
        if (movementInt & Th09EclRunMovement::ECL_INTERACTION_NO_SPRITE)
            TH09_EXPR_MOVEMENT_PRIMARY_FLAG_BITS(enemy)->noSprite04 = 0;
        if (movementInt & Th09EclRunMovement::ECL_INTERACTION_ALLOW_OFFSCREEN)
            TH09_EXPR_MOVEMENT_PRIMARY_FLAG_BITS(enemy)->allowOffscreen25 = 0;
        if (movementInt & Th09EclRunMovement::ECL_INTERACTION_NO_DEATH)
            TH09_EXPR_MOVEMENT_SECONDARY_FLAG_BITS(enemy)->noDeath03 = 0;
        break;

#undef TH09_EXPR_MOVEMENT_VIEW
#undef TH09_EXPR_MOVEMENT_PRIMARY_ANM
#undef TH09_EXPR_MOVEMENT_ALTERNATE_ANM
#undef TH09_EXPR_MOVEMENT_PRIMARY_ANM_VM
#undef TH09_EXPR_MOVEMENT_PLAYER
#undef TH09_EXPR_MOVEMENT_PRIMARY_FLAG_BITS
#undef TH09_EXPR_MOVEMENT_SECONDARY_FLAG_BITS

#endif // TH09_ECL_RUN_MOVEMENT_BODY
