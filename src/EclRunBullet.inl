// TH09 v1.50a RunEcl bullet/laser family (wire opcodes 96..123).
//
// This lexical switch fragment covers 26 active handlers and the two target-
// default slots 122/123.  It remains part of the shared 14,792-byte
// EclManager::RunEcl owner; the declarations below do not invent a separately
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

// The two descriptors exactly tile the target ranges around the deferred
// instruction and interval timer: 0x2E74 + 0x214 == 0x3088 and
// 0x30C4 + 0x214 == 0x32D8.
struct EnemyBulletView
{
    EnemyManagerView *manager0000;
    unsigned char unknown0004[0x2DD4 - 0x0004];
    Float3 worldPosition2DD4;
    unsigned char unknown2DE0[0x2E04 - 0x2DE0];
    Float3 shootOffset2E04;
    unsigned char unknown2E10[0x2E48 - 0x2E10];
    int life2E48;
    unsigned char unknown2E4C[0x2E74 - 0x2E4C];
    BulletSpawnDescriptor bulletDescriptor2E74;
    unsigned char pendingShotInstruction3088[0x2C];
    int shootIntervalFrames30B4;
    Th09EclTimerStorageView shootIntervalTimer30B8;
    BulletSpawnDescriptor laserDescriptor30C4;
    Laser *laserSlots32D8[32];
    int selectedLaserSlot3358;
    unsigned char unknown335C[0x337C - 0x335C];
    unsigned int primaryFlags337C;
};

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

inline EnemyBulletView *View(EnemyView *enemy)
{
    return reinterpret_cast<EnemyBulletView *>(enemy);
}

inline EtamaController *Controller(EnemyView *enemy)
{
    unsigned char *side = *reinterpret_cast<unsigned char **>(
        reinterpret_cast<unsigned char *>(enemy->manager00) + 0x0320);
    return *reinterpret_cast<EtamaController **>(side + 0x0008);
}

inline Th09EclContextView *ActiveContext(EnemyView *enemy)
{
    return *reinterpret_cast<Th09EclContextView **>(
        reinterpret_cast<unsigned char *>(enemy) +
        TH09_ECL_ENEMY_ACTIVE_CONTEXT);
}

inline int &ConditionResult(EnemyView *enemy)
{
    return *reinterpret_cast<int *>(
        reinterpret_cast<unsigned char *>(ActiveContext(enemy)) + 0x64);
}

// These mixed-offset packet readers are lexical in the target owner: only the
// scalar resolver remains out of line.
__forceinline float ReadLaserFloat(
    EnemyView *enemy,
    Th09EclRawInstructionHeaderView *instruction,
    int byteOffset,
    int flagIndex)
{
    float value = *reinterpret_cast<float *>(
        reinterpret_cast<unsigned char *>(instruction) + 0x0C + byteOffset);
    return (instruction->parameterMask0A & (1U << flagIndex))
               ? Th09EclRunControl::ResolveFloat(enemy, value)
               : value;
}

__forceinline int ReadLaserInt(
    EnemyView *enemy,
    Th09EclRawInstructionHeaderView *instruction,
    int byteOffset,
    int flagIndex)
{
    int value = *reinterpret_cast<int *>(
        reinterpret_cast<unsigned char *>(instruction) + 0x0C + byteOffset);
    return (instruction->parameterMask0A & (1U << flagIndex))
               ? Th09EclRunControl::ResolveInt(enemy, value)
               : value;
}

__forceinline short ReadLaserColor(
    EnemyView *enemy,
    Th09EclRawInstructionHeaderView *instruction)
{
    short color = *reinterpret_cast<short *>(
        reinterpret_cast<unsigned char *>(instruction) + 0x0E);
    return (instruction->parameterMask0A & (1U << 1))
               ? static_cast<short>(
                     Th09EclRunControl::ResolveInt(enemy, color))
               : color;
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

// Role names for target boundaries whose maintained implementations are not
// yet present.  Their callee exactness is independent of RunEcl coverage.
void DispatchShotInstruction(
    EnemyView *enemy,
    Th09EclRawInstructionHeaderView *instruction);
void ClearBulletsForTransition(EtamaController *controller);

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
            *position + Th09EclRunBullet::View(enemy)->shootOffset2E04;
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

    case TH09_ECL_OPCODE_CLEAR_BULLETS_FOR_TRANSITION:
        Th09EclRunBullet::ClearBulletsForTransition(
            Th09EclRunBullet::Controller(enemy));
        break;

    case TH09_ECL_OPCODE_SET_BULLET_SOUNDS:
        bulletInt = Th09EclRunControl::ReadInt(enemy, instruction, 0);
        if (bulletInt < 0)
        {
            Th09EclRunBullet::View(enemy)->bulletDescriptor2E74.transformFlags &=
                ~0x200U;
        }
        else
        {
            // The target resolves operand 0 again before storing the sound.
            Th09EclRunBullet::View(enemy)->bulletDescriptor2E74.spawnSound =
                Th09EclRunControl::ReadInt(enemy, instruction, 0);
            Th09EclRunBullet::View(enemy)->bulletDescriptor2E74.transformFlags |=
                0x200U;
        }
        Th09EclRunBullet::View(enemy)->bulletDescriptor2E74.transformSound =
            Th09EclRunControl::ReadInt(enemy, instruction, 1);
        break;

    case TH09_ECL_OPCODE_CREATE_LASER:
    case TH09_ECL_OPCODE_CREATE_LASER_AIMED:
        laserArgs = reinterpret_cast<Th09EclRunBullet::LaserInstructionArgs *>(
            reinterpret_cast<unsigned char *>(instruction) + 0x0C);
        laserDescriptor =
            &Th09EclRunBullet::View(enemy)->laserDescriptor30C4;
        laserDescriptor->position =
            Th09EclRunBullet::View(enemy)->worldPosition2DD4 +
            Th09EclRunBullet::View(enemy)->shootOffset2E04;
        laserDescriptor->bulletType = laserArgs->bulletType00;
        laserDescriptor->color =
            Th09EclRunBullet::ReadLaserColor(enemy, instruction);
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
        Th09EclRunBullet::View(enemy)->laserSlots32D8[
            Th09EclRunBullet::View(enemy)->selectedLaserSlot3358] =
            Th09EclRunBullet::Controller(enemy)->SpawnLaserPattern(
                laserDescriptor);
        break;

    case TH09_ECL_OPCODE_SELECT_LASER_SLOT:
        Th09EclRunBullet::View(enemy)->selectedLaserSlot3358 =
            Th09EclRunControl::ReadInt(enemy, instruction, 0);
        break;

    case TH09_ECL_OPCODE_ROTATE_LASER:
        laserIndex = Th09EclRunControl::ReadInt(enemy, instruction, 0);
        if (Th09EclRunBullet::View(enemy)->laserSlots32D8[laserIndex] != 0)
        {
            Th09EclRunBullet::View(enemy)->laserSlots32D8[laserIndex]->angle =
                Th09EclRunControl::AddNormalizeAngle(
                    Th09EclRunBullet::View(enemy)->
                        laserSlots32D8[laserIndex]->angle,
                    Th09EclRunControl::ReadFloat(enemy, instruction, 1));
        }
        break;

    case TH09_ECL_OPCODE_AIM_LASER_AT_PLAYER:
        laserIndex = Th09EclRunControl::ReadInt(enemy, instruction, 0);
        if (Th09EclRunBullet::View(enemy)->laserSlots32D8[laserIndex] != 0)
        {
            Th09EclRunBullet::View(enemy)->laserSlots32D8[laserIndex]->angle =
                Th09EclRunMovement::PlayerAngleToPoint(
                    Th09EclRunMovement::Player(enemy),
                    reinterpret_cast<EnemyFloat3 *>(
                        &Th09EclRunBullet::View(enemy)->
                            laserSlots32D8[laserIndex]->position)) +
                Th09EclRunControl::ReadFloat(enemy, instruction, 1);
        }
        break;

    case TH09_ECL_OPCODE_SET_LASER_POSITION:
        laserIndex = Th09EclRunControl::ReadInt(enemy, instruction, 0);
        laser = Th09EclRunBullet::View(enemy)->laserSlots32D8[laserIndex];
        if (laser != 0)
        {
            laser->position.x =
                Th09EclRunControl::ReadFloat(enemy, instruction, 1) +
                Th09EclRunBullet::View(enemy)->worldPosition2DD4.x;
            laser->position.y =
                Th09EclRunControl::ReadFloat(enemy, instruction, 2) +
                Th09EclRunBullet::View(enemy)->worldPosition2DD4.y;
            laser->position.z =
                Th09EclRunControl::ReadFloat(enemy, instruction, 3) +
                Th09EclRunBullet::View(enemy)->worldPosition2DD4.z;
        }
        break;

    case TH09_ECL_OPCODE_TEST_LASER_ACTIVE:
        laserIndex = Th09EclRunControl::ReadInt(enemy, instruction, 0);
        laser = Th09EclRunBullet::View(enemy)->laserSlots32D8[laserIndex];
        Th09EclRunBullet::ConditionResult(enemy) =
            (laser != 0 && laser->inUse != 0) ? 1 : 0;
        break;

    case TH09_ECL_OPCODE_CANCEL_LASER:
        laserIndex = Th09EclRunControl::ReadInt(enemy, instruction, 0);
        laser = Th09EclRunBullet::View(enemy)->laserSlots32D8[laserIndex];
        if (laser != 0 && laser->inUse != 0 && laser->state < 2)
        {
            laser->state = 2;
            Th09EclRunBullet::View(enemy)->laserSlots32D8[laserIndex]->timer = 0;
            laser = Th09EclRunBullet::View(enemy)->laserSlots32D8[laserIndex];
            laser->width = laser->currentWidth;
        }
        break;

    case TH09_ECL_OPCODE_UNHANDLED_7A:
    case TH09_ECL_OPCODE_UNHANDLED_7B:
        break;

#endif // TH09_ECL_RUN_BULLET_BODY
