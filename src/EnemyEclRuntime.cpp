#include "AnmManager.hpp"
#include "AsciiManager.hpp"
#include "EclOpcodes.hpp"
#include "EclRunControl.inl"
#include "EnemyManager.hpp"

#include <stddef.h>

// Maintained non-exact reconstruction of the two post-ECL Enemy owners at
// 0x00408180 and 0x00408560.  The target passes the Enemy through compiler-
// private registers (ESI/EDI); the natural source-level member interface is
// corroborated by adjacent-game source but the ABI and exact TU layout remain
// independently open.

namespace Th09EclRunBullet
{
void DispatchShotInstruction(
    EnemyView *enemy,
    Th09EclRawInstructionHeaderView *instruction);
}

struct EnemyEclGameManagerView
{
    unsigned char unknown000[0xEC];
    float speedEC;
};
extern EnemyEclGameManagerView g_GameManager;

struct EnemyEclSupervisorView
{
    unsigned char unknown000[0x5B8];
    float framerateMultiplier5B8;
};
extern EnemyEclSupervisorView g_Supervisor;

struct EnemyEclSoundPlayerView
{
    void PlaySoundByIdx(int soundIndex, int pan);
};
extern EnemyEclSoundPlayerView g_SoundPlayer;

struct EnemyEclPlayerView;
typedef void (__fastcall *EnemySpecialInteractionCallback)(
    EnemyEclPlayerView *player,
    EnemyView *enemy);

struct EnemyEclPlayerView
{
    unsigned char unknown00000[0x364];
    int interactionState364;
    unsigned char unknown00368[0x30408 - 0x00368];
    EnemySpecialInteractionCallback interactionCallback30408;

    int CheckEnemyInteraction(EnemyFloat3 *position);
};
typedef char EnemyEclPlayerStateAt364[
    (offsetof(EnemyEclPlayerView, interactionState364) == 0x364) ? 1 : -1];
typedef char EnemyEclPlayerCallbackAt30408[
    (offsetof(EnemyEclPlayerView, interactionCallback30408) == 0x30408)
        ? 1
        : -1];

struct EnemyPostEclRuntimeView
{
    EnemyManagerView *manager0000;
    unsigned char unknown0004[0x0008 - 0x0004];
    AnmVm primaryAnmVm0008;
    unsigned char unknown02AC[0x2D74 - 0x02AC];
    Float3 position2D74;
    Float3 positionOffset2D80;
    Float3 velocity2D8C;
    unsigned char unknown2D98[0x2DE0 - 0x2D98];
    float movementAngle2DE0;
    float angularVelocity2DE4;
    float orbitAngle2DE8;
    float orbitAngularVelocity2DEC;
    unsigned char unknown2DF0[0x2DF4 - 0x2DF0];
    float speed2DF4;
    float acceleration2DF8;
    float orbitRadius2DFC;
    float radialVelocity2E00;
    unsigned char unknown2E04[0x2E10 - 0x2E04];
    Float3 movementDelta2E10;
    Float3 movementOrigin2E1C;
    ZunTimer movementTimer2E28;
    int movementDuration2E34;
    unsigned char unknown2E38[0x2E48 - 0x2E38];
    int life2E48;
    unsigned char unknown2E4C[0x3088 - 0x2E4C];
    unsigned char pendingShotInstruction3088[0x2C];
    int shootIntervalFrames30B4;
    ZunTimer shootIntervalTimer30B8;
    unsigned char unknown30C4[0x337C - 0x30C4];
    unsigned int primaryFlags337C;
    unsigned int secondaryFlags3380;
    unsigned char unknown3384[0x3386 - 0x3384];
    unsigned char anmDirection3386;
    unsigned char unknown3387[0x338A - 0x3387];
    short idleInitialAnm338A;
    short idleFromLeftAnm338C;
    short idleFromRightAnm338E;
    short moveLeftAnm3390;
    short moveRightAnm3392;
};

typedef char EnemyPostEclPrimaryVmAt0008[
    (offsetof(EnemyPostEclRuntimeView, primaryAnmVm0008) == 0x0008)
        ? 1
        : -1];
typedef char EnemyPostEclPositionAt2D74[
    (offsetof(EnemyPostEclRuntimeView, position2D74) == 0x2D74) ? 1 : -1];
typedef char EnemyPostEclVelocityAt2D8C[
    (offsetof(EnemyPostEclRuntimeView, velocity2D8C) == 0x2D8C) ? 1 : -1];
typedef char EnemyPostEclMovementTimerAt2E28[
    (offsetof(EnemyPostEclRuntimeView, movementTimer2E28) == 0x2E28)
        ? 1
        : -1];
typedef char EnemyPostEclLifeAt2E48[
    (offsetof(EnemyPostEclRuntimeView, life2E48) == 0x2E48) ? 1 : -1];
typedef char EnemyPostEclPendingShotAt3088[
    (offsetof(EnemyPostEclRuntimeView, pendingShotInstruction3088) == 0x3088)
        ? 1
        : -1];
typedef char EnemyPostEclFlagsAt337C[
    (offsetof(EnemyPostEclRuntimeView, primaryFlags337C) == 0x337C)
        ? 1
        : -1];
typedef char EnemyPostEclDirectionAt3386[
    (offsetof(EnemyPostEclRuntimeView, anmDirection3386) == 0x3386)
        ? 1
        : -1];
typedef char EnemyPostEclMoveLeftAt3390[
    (offsetof(EnemyPostEclRuntimeView, moveLeftAnm3390) == 0x3390)
        ? 1
        : -1];

enum EnemyPostEclFlag
{
    ENEMY_MOVEMENT_POLAR = 0x00000200,
    ENEMY_MOVEMENT_INTERPOLATED = 0x00000400,
    ENEMY_MOVEMENT_ORBIT = 0x00000600,
    ENEMY_MOVEMENT_MODE_MASK = 0x00000600,
    ENEMY_MOVEMENT_EASING_SHIFT = 11,
    ENEMY_MIRROR_MOVEMENT_X = 0x00008000,
    ENEMY_ALTERNATE_ANM_BANK = 0x80000000,
    ENEMY_INTERACTION_KIND_MASK = 0x000001C0,
    ENEMY_INTERACTION_KIND_SHIFT = 6,
    ENEMY_INTERACTION_TRIGGERED = 0x00001000
};

static __forceinline EnemyPostEclRuntimeView *PostEclView(EnemyView *enemy)
{
    return reinterpret_cast<EnemyPostEclRuntimeView *>(enemy);
}

static __forceinline EnemyEclPlayerView *PostEclPlayer(
    EnemyPostEclRuntimeView *enemy)
{
    return reinterpret_cast<EnemyEclPlayerView *>(
        enemy->manager0000->sideState320->player04);
}

void EnemyView::UpdateMovement()
{
    EnemyPostEclRuntimeView *enemy = PostEclView(this);

    switch ((enemy->primaryFlags337C >> 9) & 3)
    {
    case 3:
    {
        float polarVelocity[3];

        enemy->orbitAngle2DE8 = Th09EclRunControl::AddNormalizeAngle(
            enemy->orbitAngle2DE8,
            g_Supervisor.framerateMultiplier5B8 *
                enemy->orbitAngularVelocity2DEC);
        enemy->orbitRadius2DFC =
            g_GameManager.speedEC * enemy->radialVelocity2E00 *
                g_Supervisor.framerateMultiplier5B8 +
            enemy->orbitRadius2DFC;
        reinterpret_cast<Float3 *>(polarVelocity)->FromAngleMagnitude(
            enemy->orbitAngle2DE8, enemy->orbitRadius2DFC);
        enemy->velocity2D8C.x =
            polarVelocity[0] + enemy->movementOrigin2E1C.x -
            enemy->position2D74.x;
        enemy->velocity2D8C.y =
            polarVelocity[1] + enemy->movementOrigin2E1C.y -
            enemy->position2D74.y;
        enemy->movementAngle2DE0 = Th09EclRunControl::VectorAngle(
            enemy->velocity2D8C.y, enemy->velocity2D8C.x);
        if (enemy->movementDuration2E34 > 0)
        {
            enemy->movementTimer2E28 -= g_GameManager.speedEC;
            if (enemy->movementTimer2E28 <= 0)
                enemy->primaryFlags337C &= ~ENEMY_MOVEMENT_MODE_MASK;
        }
        break;
    }

    case 1:
        enemy->movementAngle2DE0 = Th09EclRunControl::AddNormalizeAngle(
            enemy->movementAngle2DE0,
            g_Supervisor.framerateMultiplier5B8 * enemy->angularVelocity2DE4);
        enemy->speed2DF4 =
            g_Supervisor.framerateMultiplier5B8 * enemy->acceleration2DF8 +
            enemy->speed2DF4;
        enemy->velocity2D8C.FromAngleMagnitude(
            enemy->movementAngle2DE0, enemy->speed2DF4);
        enemy->velocity2D8C.operator float *()[2] = 0.0f;
        if (enemy->movementDuration2E34 > 0)
        {
            enemy->movementTimer2E28--;
            if (enemy->movementTimer2E28 <= 0)
                enemy->primaryFlags337C &= ~ENEMY_MOVEMENT_MODE_MASK;
        }
        break;

    case 2:
    {
        float progress;

        enemy->movementTimer2E28--;
        progress = 1.0f -
            static_cast<float>(enemy->movementTimer2E28) /
                enemy->movementDuration2E34;
        if (progress < 0.0f)
            progress = 0.0f;

        switch ((enemy->primaryFlags337C >>
                 ENEMY_MOVEMENT_EASING_SHIFT) & 7)
        {
        case 1:
            progress *= progress;
            break;
        case 2:
            progress = progress * progress * progress;
            break;
        case 3:
            progress = progress * progress * progress * progress;
            break;
        case 4:
            progress = 1.0f - progress;
            progress *= progress;
            progress = 1.0f - progress;
            break;
        case 5:
            progress = 1.0f - progress;
            progress = progress * progress * progress;
            progress = 1.0f - progress;
            break;
        case 6:
            progress = 1.0f - progress;
            progress = progress * progress * progress * progress;
            progress = 1.0f - progress;
            break;
        default:
            break;
        }

        enemy->velocity2D8C =
            enemy->movementOrigin2E1C +
            enemy->movementDelta2E10 * progress - enemy->position2D74;
        if ((enemy->primaryFlags337C & ENEMY_MIRROR_MOVEMENT_X) != 0)
            enemy->velocity2D8C.x = -enemy->velocity2D8C.x;
        enemy->movementAngle2DE0 = Th09EclRunControl::VectorAngle(
            enemy->velocity2D8C.y, enemy->velocity2D8C.x);
        if (enemy->movementTimer2E28 <= 0)
        {
            enemy->primaryFlags337C &= ~ENEMY_MOVEMENT_MODE_MASK;
            enemy->position2D74 =
                enemy->movementOrigin2E1C + enemy->movementDelta2E10;
            enemy->velocity2D8C = Float3(0.0f, 0.0f, 0.0f);
        }
        break;
    }

    default:
        break;
    }

    if ((enemy->secondaryFlags3380 & ENEMY_INTERACTION_KIND_MASK) != 0)
    {
        if ((enemy->secondaryFlags3380 & ENEMY_INTERACTION_TRIGGERED) == 0)
        {
            EnemyEclPlayerView *player = PostEclPlayer(enemy);
            if (player->interactionState364 != 0 &&
                player->CheckEnemyInteraction(
                    reinterpret_cast<EnemyFloat3 *>(&enemy->position2D74)))
            {
                enemy->secondaryFlags3380 |= ENEMY_INTERACTION_TRIGGERED;
                enemy->manager0000->enemyAnm2AC420->SetAndExecuteScriptIdx(
                    &enemy->primaryAnmVm0008,
                    ((enemy->secondaryFlags3380 >>
                      ENEMY_INTERACTION_KIND_SHIFT) & 7) + 22);
                enemy->moveLeftAnm3390 = -1;
                g_SoundPlayer.PlaySoundByIdx(
                    47, enemy->manager0000->sideIndex31C != 0 ? 500 : -500);
            }
        }

        if ((enemy->secondaryFlags3380 & ENEMY_INTERACTION_TRIGGERED) != 0)
        {
            EnemyEclPlayerView *player = PostEclPlayer(enemy);
            player->interactionCallback30408(player, this);
        }
    }
}

void EnemyView::UpdateShotAndAnm()
{
    EnemyPostEclRuntimeView *enemy = PostEclView(this);
    int direction;
    float horizontalVelocity;
    AnmLoaded *anm;

    if (enemy->life2E48 <= 0)
        return;

    if (enemy->shootIntervalFrames30B4 > 0)
    {
        enemy->shootIntervalTimer30B8++;
        if (enemy->shootIntervalTimer30B8 >=
            enemy->shootIntervalFrames30B4)
        {
            Th09EclRunBullet::DispatchShotInstruction(
                this,
                reinterpret_cast<Th09EclRawInstructionHeaderView *>(
                    enemy->pendingShotInstruction3088));
            enemy->shootIntervalTimer30B8 = 0;
        }
    }

    if (enemy->moveLeftAnm3390 < 0)
        return;

    horizontalVelocity = enemy->velocity2D8C.x;
    if ((enemy->primaryFlags337C & ENEMY_MIRROR_MOVEMENT_X) != 0)
        horizontalVelocity = -horizontalVelocity;

    direction = 0;
    if (horizontalVelocity < -0.01f)
        direction = 1;
    else if (horizontalVelocity > 0.01f)
        direction = 2;

    if (enemy->anmDirection3386 == direction)
        return;

    anm = (enemy->primaryFlags337C & ENEMY_ALTERNATE_ANM_BANK) != 0
        ? enemy->manager0000->opposingEnemyAnm2AC424
        : enemy->manager0000->enemyAnm2AC420;

    switch (direction)
    {
    case 0:
        if (enemy->anmDirection3386 == 0xFF)
        {
            anm->SetAndExecuteScriptIdx(
                &enemy->primaryAnmVm0008, enemy->idleInitialAnm338A);
        }
        else if (enemy->anmDirection3386 == 1)
        {
            anm->SetAndExecuteScriptIdx(
                &enemy->primaryAnmVm0008, enemy->idleFromLeftAnm338C);
        }
        else
        {
            anm->SetAndExecuteScriptIdx(
                &enemy->primaryAnmVm0008, enemy->idleFromRightAnm338E);
        }
        break;

    case 1:
        anm->SetAndExecuteScriptIdx(
            &enemy->primaryAnmVm0008, enemy->moveLeftAnm3390);
        break;

    case 2:
        anm->SetAndExecuteScriptIdx(
            &enemy->primaryAnmVm0008, enemy->moveRightAnm3392);
        break;

    default:
        break;
    }

    enemy->anmDirection3386 = static_cast<unsigned char>(direction);
}
