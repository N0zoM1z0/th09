// TH09 v1.50a RunEcl bullet/laser family (wire opcodes 96..123).
//
// This lexical switch fragment covers 26 active handlers and the two target-
// default slots 122/123. It remains part of the shared EclManager::RunEcl
// physical owner; the declarations below do not invent a separately
// callable handler boundary.
//
// Target evidence:
//   shot dispatch:     0x0040A168..0x0040A1BA / helper 0x00408040
//   interval/control:  0x0040A290..0x0040A409 / 0x0040B5B1..0x0040B63D
//   laser creation:    0x0040A414..0x0040A5AA / callee 0x00413290
//   laser controls:    0x0040A5B3..0x0040A945
//
// Committed TH08 supplied source-shape hypotheses only.  TH09 independently
// fixes every offset, mask, repeated resolver, active slot, helper call and
// default entry represented here.

#ifndef TH09_ECL_RUN_BULLET_DECLARATIONS
#define TH09_ECL_RUN_BULLET_DECLARATIONS

#include "BulletManager.hpp"
#include "EclRunRemote.inl"

#include <stddef.h>
#include <string.h>

namespace Th09EclRunBullet
{

enum EnemyBulletFlag
{
    ENEMY_DEFER_BULLET_PATTERN = 0x00004000
};

// The target's two 0x214-byte descriptor records and shot state are the
// same EnemyView allocation used by Movement, Enemy update and the exact
// operand leaves, not a second private 0x337C-byte Enemy projection.
typedef ::EnemyView EnemyBulletView;

typedef char Th09EclBulletDescriptorAt2E74[
    (offsetof(EnemyBulletView, bulletDescriptor2E74) == 0x2E74) ? 1 : -1];
typedef char Th09EclPendingShotAt3088[
    (offsetof(EnemyBulletView, pendingShotInstruction3088) == 0x3088) ? 1 : -1];
typedef char Th09EclShootIntervalAt30B4[
    (offsetof(EnemyBulletView, shootIntervalFrames30B4) == 0x30B4) ? 1 : -1];
typedef char Th09EclLaserDescriptorAt30C4[
    (offsetof(EnemyBulletView, laserDescriptor30C4) == 0x30C4) ? 1 : -1];
typedef char Th09EclLaserSlotsAt32D8[
    (offsetof(EnemyBulletView, laserSlots32D8) == 0x32D8) ? 1 : -1];
typedef char Th09EclSelectedLaserAt3358[
    (offsetof(EnemyBulletView, selectedLaserSlot3358) == 0x3358) ? 1 : -1];
// Same original +0x3368..+0x336A three-byte ANM/death-mode window
// initialized by the 554-byte exact EnemyManager template owner.
typedef char Th09EclBulletNativeDeathAnmScriptsAt3368[
    (offsetof(EnemyView, deathEffectVariant3368) == 0x3368 &&
     offsetof(EnemyView, deathEffectVariant3369) == 0x3369 &&
     offsetof(EnemyView, deathEffectVariant336A) == 0x336A) ? 1 : -1];
typedef char Th09EclBulletFlagsAt337C[
    (offsetof(EnemyBulletView, primaryFlags337C) == 0x337C) ? 1 : -1];

struct LaserInstructionArgs
{
    unsigned short bulletType00;
    short color02;
    float angle04;
    float speed08;
    float startOffset0C;
    float endOffset10;
    float startLength14;
    float width18;
    int startTime1C;
    int duration20;
    int despawnDuration24;
    int hitboxStartTime28;
    int hitboxEndDelay2C;
    unsigned int transformFlags30;
};
typedef char Th09EclLaserArgsSizeIs34[
    (sizeof(LaserInstructionArgs) == 0x34) ? 1 : -1];
// The original 16-bit laser-color wire element lives at payload +0x02.
typedef char Th09EclLaserColorAt02[
    (offsetof(LaserInstructionArgs, color02) == 0x02) ? 1 : -1];

inline EnemyBulletView *View(EnemyView *enemy)
{
    return enemy;
}

inline EtamaController *Controller(EnemyView *enemy)
{
    return enemy->manager00->sideState320->etama08;
}

// The original i386 forms the physical pointer slot from a signed ECL word
// and the actual EnemyView 32-pointer table; no new index guard is inserted.
// Target-observed laser control opcodes index the genuine thirty-two
// EnemyView laser pointers as native 32-bit byte offsets. Preserve the
// selected-slot creation spelling separately: its target-bound callsite
// identity has a distinct, still-unresolved comparison contract.
inline Laser *&LaserSlot(EnemyView *enemy, int slot)
{
    return *reinterpret_cast<Laser **>(
        reinterpret_cast<unsigned int>(&enemy->laserSlots32D8[0]) +
        static_cast<unsigned int>(slot) * sizeof(enemy->laserSlots32D8[0]));
}

inline Th09EclContextView *ActiveContext(EnemyView *enemy)
{
    return enemy->activeContext2CE0;
}

inline int &ConditionResult(EnemyView *enemy)
{
    // Original context +0x64 is the third integer in the +0x5C family.
    return ActiveContext(enemy)->intVariables5C[2];
}

// These mixed-offset packet readers are lexical in the target owner: only the
// scalar resolver remains out of line. Compute the full signed wire offset as
// native i386 unsigned address arithmetic rather than subscripting arguments[0].
__forceinline float ReadLaserFloat(
    EnemyView *enemy,
    Th09EclRawInstructionHeaderView *instruction,
    int byteOffset,
    int flagIndex)
{
    float *operand = reinterpret_cast<float *>(
        reinterpret_cast<unsigned int>(instruction) +
        static_cast<unsigned int>(offsetof(Th09EclRawInstructionHeaderView, arguments)) +
        static_cast<unsigned int>(byteOffset));
    return (instruction->parameterMask0A & (1U << flagIndex))
        ? enemy->ResolveFloat(*operand) : *operand;
}

__forceinline int ReadLaserInt(
    EnemyView *enemy,
    Th09EclRawInstructionHeaderView *instruction,
    int byteOffset,
    int flagIndex)
{
    int *operand = reinterpret_cast<int *>(
        reinterpret_cast<unsigned int>(instruction) +
        static_cast<unsigned int>(offsetof(Th09EclRawInstructionHeaderView, arguments)) +
        static_cast<unsigned int>(byteOffset));
    return (instruction->parameterMask0A & (1U << flagIndex))
        ? Th09EclRunControl::ResolveInt(enemy, *operand) : *operand;
}

__forceinline short ReadLaserColor(
    EnemyView *enemy,
    Th09EclRawInstructionHeaderView *instruction)
{
    const LaserInstructionArgs *const args =
        reinterpret_cast<const LaserInstructionArgs *>(
            reinterpret_cast<unsigned int>(instruction) +
            static_cast<unsigned int>(offsetof(Th09EclRawInstructionHeaderView, arguments)));
    return (instruction->parameterMask0A & (1U << 1))
        ? static_cast<short>(Th09EclRunControl::ResolveInt(enemy, args->color02))
        : args->color02;
}

inline void SetLaserPosition(
    BulletSpawnDescriptor &descriptor,
    const Float3 &worldPosition,
    const Float3 &shootOffset)
{
    descriptor.position.x = worldPosition.x + shootOffset.x;
    descriptor.position.y = worldPosition.y + shootOffset.y;
    descriptor.position.z = worldPosition.z + shootOffset.z;
}

// Target-facing role declarations used by RunEcl. DispatchShotInstruction is
// a TU-private maintained owner in EclPostRuntime.inl; its own exactness remains
// independent of the exact post-ECL caller that invokes it.
static void DispatchShotInstruction(
    EnemyView *enemy,
    Th09EclRawInstructionHeaderView *instruction);

} // namespace Th09EclRunBullet

#endif // TH09_ECL_RUN_BULLET_DECLARATIONS

#if defined(TH09_ECL_RUN_BULLET_BODY)

#if !defined(TH09_ECL_RUN_SHARED_SWITCH)
#error EclRunBullet.inl must be included lexically inside RunEcl's switch
#endif

    int bulletInt;
    int laserIndex;
    BulletTransformRecord *bulletTransform;
    BulletSpawnDescriptor *laserDescriptor;
    Th09EclRunBullet::LaserInstructionArgs *laserArgs;
    Laser *laser;

    case TH09_ECL_OPCODE_SHOOT_FAN_AIMED:
    case TH09_ECL_OPCODE_SHOOT_FAN:
    case TH09_ECL_OPCODE_SHOOT_CIRCLE_AIMED:
    case TH09_ECL_OPCODE_SHOOT_CIRCLE:
    case TH09_ECL_OPCODE_SHOOT_OFFSET_CIRCLE_AIMED:
    case TH09_ECL_OPCODE_SHOOT_OFFSET_CIRCLE:
    case TH09_ECL_OPCODE_SHOOT_RANDOM_ANGLE:
    case TH09_ECL_OPCODE_SHOOT_RANDOM_SPEED:
    case TH09_ECL_OPCODE_SHOOT_RANDOM:
        if (Th09EclRunBullet::View(enemy)->life2E48 > 0)
        {
            if ((Th09EclRunBullet::View(enemy)->primaryFlags337C &
                 Th09EclRunBullet::ENEMY_DEFER_BULLET_PATTERN) != 0)
            {
                memcpy(
                    Th09EclRunBullet::View(enemy)->pendingShotInstruction3088,
                    instruction,
                    sizeof(Th09EclRunBullet::View(enemy)->
                               pendingShotInstruction3088));
            }
            else
            {
                Th09EclRunBullet::DispatchShotInstruction(enemy, instruction);
            }
        }
        break;

    case TH09_ECL_OPCODE_SET_BULLET_TRANSFORM:
        bulletInt = Th09EclRunControl::ReadInt(enemy, instruction, 0);
        bulletTransform =
            &Th09EclRunBullet::View(enemy)->bulletDescriptor2E74.transforms[
                bulletInt];
        bulletTransform->kind =
            Th09EclRunControl::ReadInt(enemy, instruction, 1);
        bulletTransform->allowWhileActive =
            Th09EclRunControl::ReadInt(enemy, instruction, 2);
        bulletTransform->payload.raw.int0 =
            Th09EclRunControl::ReadInt(enemy, instruction, 3);
        bulletTransform->payload.raw.int1 =
            Th09EclRunControl::ReadInt(enemy, instruction, 4);
        bulletTransform->payload.raw.float0 =
            Th09EclRunControl::ReadFloat(enemy, instruction, 5);
        bulletTransform->payload.raw.float1 =
            Th09EclRunControl::ReadFloat(enemy, instruction, 6);
        break;

    case TH09_ECL_OPCODE_SET_DEATH_ANM_SCRIPTS:
        enemy->deathEffectVariant3368 =
            Th09EclRunState::RawByte(instruction, 0);
        enemy->deathEffectVariant3369 =
            Th09EclRunState::RawByte(instruction, 1);
        enemy->deathEffectVariant336A =
            Th09EclRunState::RawByte(instruction, 2);
        break;

    case TH09_ECL_OPCODE_SET_SHOOT_INTERVAL:
        bulletInt = Th09EclRunControl::ReadInt(enemy, instruction, 0);
        Th09EclRunBullet::View(enemy)->shootIntervalFrames30B4 = bulletInt;
        if (bulletInt != 0)
        {
            Th09EclRunBullet::View(enemy)->shootIntervalTimer30B8 = 0;
        }
        break;

    case TH09_ECL_OPCODE_SET_SHOOT_INTERVAL_DELAYED:
        bulletInt = Th09EclRunControl::ReadInt(enemy, instruction, 0);
        Th09EclRunBullet::View(enemy)->shootIntervalFrames30B4 = bulletInt;
        if (bulletInt != 0)
        {
            Th09EclRunBullet::View(enemy)->shootIntervalTimer30B8 =
                Th09EclRunControl::g_Rng.GetRandomU32InRange(bulletInt);
        }
        break;

    case TH09_ECL_OPCODE_ENABLE_DEFERRED_SHOOTING:
        Th09EclRunBullet::View(enemy)->primaryFlags337C |=
            Th09EclRunBullet::ENEMY_DEFER_BULLET_PATTERN;
        break;

    case TH09_ECL_OPCODE_DISABLE_DEFERRED_SHOOTING:
        Th09EclRunBullet::View(enemy)->primaryFlags337C &=
            ~Th09EclRunBullet::ENEMY_DEFER_BULLET_PATTERN;
        break;

    case TH09_ECL_OPCODE_SHOOT_NOW:
        Th09EclRunBullet::View(enemy)->bulletDescriptor2E74.position =
            *position + reinterpret_cast<const Float3 &>(
                Th09EclRunBullet::View(enemy)->shootOffset2E04);
        Th09EclRunBullet::Controller(enemy)->SpawnBulletPatternSecondary(
            &Th09EclRunBullet::View(enemy)->bulletDescriptor2E74);
        break;

    case TH09_ECL_OPCODE_SET_SHOOT_OFFSET:
        Th09EclRunBullet::View(enemy)->shootOffset2E04.x =
            Th09EclRunControl::ReadFloat(enemy, instruction, 0);
        Th09EclRunBullet::View(enemy)->shootOffset2E04.y =
            Th09EclRunControl::ReadFloat(enemy, instruction, 1);
        Th09EclRunBullet::View(enemy)->shootOffset2E04.z = 0.0f;
        break;

    case TH09_ECL_OPCODE_CREATE_LASER:
    case TH09_ECL_OPCODE_CREATE_LASER_AIMED:
        // Packed laser arguments occupy the native ECL wire tail.
        laserArgs = reinterpret_cast<Th09EclRunBullet::LaserInstructionArgs *>(
            reinterpret_cast<unsigned int>(instruction) +
            static_cast<unsigned int>(offsetof(Th09EclRawInstructionHeaderView, arguments)));
        laserDescriptor =
            &Th09EclRunBullet::View(enemy)->laserDescriptor30C4;
        laserDescriptor->position =
            reinterpret_cast<const Float3 &>(
                Th09EclRunBullet::View(enemy)->worldPosition2DD4) +
            reinterpret_cast<const Float3 &>(
                Th09EclRunBullet::View(enemy)->shootOffset2E04);
        laserDescriptor->bulletType = laserArgs->bulletType00;
        {
            // Keep both paths signed-int width; narrow only at the descriptor field.
            int resolvedColor =
                (instruction->parameterMask0A & (1U << 1))
                    ? Th09EclRunControl::ResolveInt(enemy, laserArgs->color02)
                    : laserArgs->color02;
            laserDescriptor->color = static_cast<short>(resolvedColor);
        }
        laserDescriptor->angle = Th09EclRunBullet::ReadLaserFloat(
            enemy, instruction, 0x04, 2);
        laserDescriptor->speed1 = Th09EclRunBullet::ReadLaserFloat(
            enemy, instruction, 0x08, 3);
        laserDescriptor->laserStartOffset = Th09EclRunBullet::ReadLaserFloat(
            enemy, instruction, 0x0C, 4);
        laserDescriptor->laserEndOffset = Th09EclRunBullet::ReadLaserFloat(
            enemy, instruction, 0x10, 5);
        laserDescriptor->laserStartLength = Th09EclRunBullet::ReadLaserFloat(
            enemy, instruction, 0x14, 6);
        laserDescriptor->laserWidth = Th09EclRunBullet::ReadLaserFloat(
            enemy, instruction, 0x18, 7);
        laserDescriptor->laserStartTime = Th09EclRunBullet::ReadLaserInt(
            enemy, instruction, 0x1C, 8);
        laserDescriptor->laserDuration = Th09EclRunBullet::ReadLaserInt(
            enemy, instruction, 0x20, 9);
        laserDescriptor->laserDespawnDuration = Th09EclRunBullet::ReadLaserInt(
            enemy, instruction, 0x24, 10);
        laserDescriptor->laserHitboxStartTime = laserArgs->hitboxStartTime28;
        laserDescriptor->laserHitboxEndDelay = laserArgs->hitboxEndDelay2C;
        laserDescriptor->transformFlags = laserArgs->transformFlags30;
        laserDescriptor->aimMode =
            instruction->opcode04 != TH09_ECL_OPCODE_CREATE_LASER_AIMED;
        enemy->laserSlots32D8[
            enemy->selectedLaserSlot3358] =
            Th09EclRunBullet::Controller(enemy)->SpawnLaserPattern(
                laserDescriptor);
        break;

    case TH09_ECL_OPCODE_SELECT_LASER_SLOT:
        enemy->selectedLaserSlot3358 =
            Th09EclRunControl::ReadInt(enemy, instruction, 0);
        break;

    case TH09_ECL_OPCODE_ROTATE_LASER:
        laserIndex = Th09EclRunControl::ReadInt(enemy, instruction, 0);
        if (Th09EclRunBullet::LaserSlot(enemy, laserIndex) != 0)
        {
            Th09EclRunBullet::LaserSlot(enemy, laserIndex)->angle =
                Th09EclRunControl::AddNormalizeAngle(
                    Th09EclRunBullet::LaserSlot(enemy, laserIndex)->angle,
                    Th09EclRunControl::ReadFloat(enemy, instruction, 1));
        }
        break;

    case TH09_ECL_OPCODE_SET_LASER_ANGLE:
        laserIndex = Th09EclRunControl::ReadInt(enemy, instruction, 0);
        if (Th09EclRunBullet::LaserSlot(enemy, laserIndex) != 0)
        {
            Th09EclRunBullet::LaserSlot(enemy, laserIndex)->angle =
                Th09EclRunControl::ReadFloat(enemy, instruction, 1);
        }
        break;

    case TH09_ECL_OPCODE_AIM_LASER_AT_PLAYER:
        laserIndex = Th09EclRunControl::ReadInt(enemy, instruction, 0);
        if (Th09EclRunBullet::LaserSlot(enemy, laserIndex) != 0)
        {
            float angleOffset =
                Th09EclRunControl::ReadFloat(enemy, instruction, 1);
            Th09EclRunBullet::LaserSlot(enemy, laserIndex)->angle =
                reinterpret_cast<PlayerAngleView *>(
                    enemy->manager00->sideState320->playerLifecycle04)->AngleToPoint(
                        reinterpret_cast<PlayerAngleFloat3View *>(
                            &Th09EclRunBullet::LaserSlot(enemy, laserIndex)->position)) +
                angleOffset;
        }
        break;

    case TH09_ECL_OPCODE_SET_LASER_POSITION:
        laserIndex = Th09EclRunControl::ReadInt(enemy, instruction, 0);
        if (Th09EclRunBullet::LaserSlot(enemy, laserIndex) != 0)
        {
            Th09EclRunBullet::LaserSlot(enemy, laserIndex)->
                position.x =
                    Th09EclRunControl::ReadFloat(enemy, instruction, 1) +
                    Th09EclRunBullet::View(enemy)->worldPosition2DD4.x;
            Th09EclRunBullet::LaserSlot(enemy, laserIndex)->
                position.y =
                    Th09EclRunControl::ReadFloat(enemy, instruction, 2) +
                    Th09EclRunBullet::View(enemy)->worldPosition2DD4.y;
            Th09EclRunBullet::LaserSlot(enemy, laserIndex)->
                position.z =
                    Th09EclRunControl::ReadFloat(enemy, instruction, 3) +
                    Th09EclRunBullet::View(enemy)->worldPosition2DD4.z;
        }
        break;

    case TH09_ECL_OPCODE_SET_LASER_START_CAP_HIDDEN:
        laserIndex = Th09EclRunControl::ReadInt(enemy, instruction, 0);
        if (Th09EclRunBullet::LaserSlot(enemy, laserIndex) != 0)
        {
            Th09EclRunBullet::LaserSlot(enemy, laserIndex)->
                hideCapDuringStartup =
                    static_cast<unsigned char>(
                        Th09EclRunControl::ReadInt(enemy, instruction, 1));
        }
        break;

    case TH09_ECL_OPCODE_TEST_LASER_ACTIVE:
        laserIndex = Th09EclRunControl::ReadInt(enemy, instruction, 0);
        laser = Th09EclRunBullet::LaserSlot(enemy, laserIndex);
        if (laser != 0 && laser->inUse != 0)
            Th09EclRunBullet::ConditionResult(enemy) = 1;
        else
            Th09EclRunBullet::ConditionResult(enemy) = 0;
        break;

    case TH09_ECL_OPCODE_CANCEL_LASER:
        laserIndex = Th09EclRunControl::ReadInt(enemy, instruction, 0);
        laser = Th09EclRunBullet::LaserSlot(enemy, laserIndex);
        if (laser != 0 && laser->inUse != 0 && laser->state < 2)
        {
            Th09EclRunBullet::LaserSlot(enemy, laserIndex)->state = 2;
            Th09EclRunBullet::LaserSlot(enemy, laserIndex)->timer = 0;
            laser = Th09EclRunBullet::LaserSlot(enemy, laserIndex);
            laser->width = laser->currentWidth;
        }
        break;

    case TH09_ECL_OPCODE_CLEAR_LASER_SLOTS:
        for (int i = 0; i < 0x20; ++i)
            Th09EclRunBullet::LaserSlot(enemy, i) = 0;
        break;

    case TH09_ECL_OPCODE_SET_LASER_START_LENGTH:
        laserIndex = Th09EclRunControl::ReadInt(enemy, instruction, 0);
        if (Th09EclRunBullet::LaserSlot(enemy, laserIndex) != 0)
        {
            Th09EclRunBullet::LaserSlot(enemy, laserIndex)->
                startLength =
                    Th09EclRunControl::ReadFloat(enemy, instruction, 1);
        }
        break;

    case TH09_ECL_OPCODE_SET_LASER_OFFSETS:
        laserIndex = Th09EclRunControl::ReadInt(enemy, instruction, 0);
        if (Th09EclRunBullet::LaserSlot(enemy, laserIndex) != 0)
        {
            Th09EclRunBullet::LaserSlot(enemy, laserIndex)->
                startOffset =
                    Th09EclRunControl::ReadFloat(enemy, instruction, 1);
            Th09EclRunBullet::LaserSlot(enemy, laserIndex)->
                endOffset =
                    Th09EclRunControl::ReadFloat(enemy, instruction, 2);
        }
        break;

    case TH09_ECL_OPCODE_SET_MANAGER_PROTOCOL_VALUE:
        enemy->manager00->protocolValue2AC3DC =
            Th09EclRunControl::ReadInt(enemy, instruction, 0);
        break;

#endif // TH09_ECL_RUN_BULLET_BODY

#if defined(TH09_ECL_RUN_BULLET_TRANSITION_BODY)

#if !defined(TH09_ECL_RUN_SHARED_SWITCH)
#error EclRunBullet.inl transition body must be included lexically inside RunEcl's switch
#endif

    int transitionBulletInt;

    case TH09_ECL_OPCODE_CLEAR_BULLETS_FOR_TRANSITION:
        Th09EclRunBullet::Controller(enemy)->ClearBulletsForTransition();
        break;

    case TH09_ECL_OPCODE_SET_BULLET_SOUNDS:
        transitionBulletInt =
            Th09EclRunControl::ReadInt(enemy, instruction, 0);
        if (transitionBulletInt >= 0)
        {
            Th09EclRunBullet::View(enemy)->bulletDescriptor2E74.spawnSound =
                Th09EclRunControl::ReadInt(enemy, instruction, 0);
            Th09EclRunBullet::View(enemy)->bulletDescriptor2E74.transformFlags |=
                0x200U;
        }
        else
        {
            Th09EclRunBullet::View(enemy)->bulletDescriptor2E74.transformFlags &=
                ~0x200U;
        }
        Th09EclRunBullet::View(enemy)->bulletDescriptor2E74.transformSound =
            Th09EclRunControl::ReadInt(enemy, instruction, 1);
        break;

#endif // TH09_ECL_RUN_BULLET_TRANSITION_BODY
