#include "GameManagerSideScoreView.hpp"
#include "AsciiGameManagerView.hpp"
#include "PlayerDamageView.hpp"
#include "PlayerState4OpsView.hpp"
#include "FrontEnemyIndicatorView.hpp"
#include "BulletSpawnDescriptorCtorView.hpp"
#include "ZunTimer.hpp"
// Lexical member of EclManager.cpp; reuses Th09EclRunState::ManagerStateView.
#include "AsciiManager.hpp"
#include "EnemyManager.hpp"
#include "EnemyChildEclBlocks.hpp"
#include "EnemyAttachedEffectStateView.hpp"
#include "BulletManager.hpp"
#include "EnemyAppendCollisionView.hpp"
#include "PlayerLifecycleView.hpp"
#include "Supervisor.hpp"
#include "SoundPlayer.hpp"
#include "AnmManager.hpp"
#include "AsciiBossMarkerVmView.hpp"
#include "ZunMemory.hpp"
#include "PlayerEnemyTrackingStateView.hpp"

#include <math.h>
#include <stddef.h>
#include <string.h>

// The real comparison body preserves the original Player tracking allocation.
#include "ZunTimerGreater.inl"

// These three target-exact helpers share this translation unit with
// EnemyManager::OnUpdate. The real callee bodies alter VC7.1 copy scheduling
// in the 3,900-byte caller; original C++ TU partition is not proven.
Float3 Float3::operator/(float scalar) const
{
    float inverse = 1.0f / scalar;
    return Float3(this->x * inverse, this->y * inverse, this->z * inverse);
}

void PlayerCollisionQueryStateView::AppendBoxRecord(
    const PlayerPositionView *position,
    const PlayerPositionView *size,
    int unknown)
{
    if (count1804 >= 128)
        return;

    records[count1804].position00 = *position;
    records[count1804].halfSize18 = *size;
    records[count1804].halfSize18.x *= 0.5f;
    records[count1804].halfSize18.y *= 0.5f;
    records[count1804].radius24 = 0.0f;
    records[count1804].angle28 = 0.0f;
    records[count1804].unknown2C = unknown;
    ++count1804;
}

struct EnemyCollisionPlayerView
{
    unsigned char unknown000[0x36C];
    PlayerCollisionQueryStateView collisionQuery36C;
};

struct EnemyCollisionSideView
{
    unsigned char unknown00[0x04];
    EnemyCollisionPlayerView *player04;
};


void EnemyAppendCollisionView::AppendPlayerCollisionBox(
    const Float3 *position,
    const Float3 *size)
{
    Float3 scaledSize;
    scaledSize = *size / 1.5f;

    reinterpret_cast<EnemyCollisionSideView *>(
        manager00->sideState320)->player04->collisionQuery36C
        .AppendBoxRecord(
            reinterpret_cast<const PlayerPositionView *>(position),
            reinterpret_cast<const PlayerPositionView *>(&scaledSize),
            0);
}

// Maintained non-exact reconstruction of the EnemyManager calc callback at
// 0x410730.  The private views below expose only target-observed offsets.  The
// helper names describe roles; original names remain unknown.

enum EnemyCoreFlags
{
    ENEMY_CORE_ACTIVE = 0x00000001,
    ENEMY_CORE_BOSS = 0x00000002,
    ENEMY_CORE_COLLISION = 0x00000004,
    ENEMY_CORE_DAMAGEABLE = 0x00000008,
    ENEMY_CORE_NO_SPRITE = 0x00000010,
    ENEMY_CORE_HIDE_PRIMARY = 0x00000020,
    ENEMY_CORE_ACCEPTS_DAMAGE = 0x00000040,
    ENEMY_CORE_FORCE_DEATH = 0x00000100,
    ENEMY_CORE_PERSIST_AFTER_DEATH = 0x00100000,
    ENEMY_CORE_HAS_BEEN_IN_BOUNDS = 0x00200000,
    ENEMY_CORE_ALLOW_OFFSCREEN = 0x02000000,
    ENEMY_CORE_SKIP_MOVEMENT = 0x04000000,
    ENEMY_CORE_PAUSE_WITH_PLAYER = 0x08000000
};

struct EnemyCoreLoadedSpriteView
{
    unsigned char unknown00[0x30];
    float extent30;
    float extent34;
};

// Shared maintained ANM layout at the three target-observed VM slots.
// Nominal original class identity and complete product ownership remain open.
typedef char EnemyCoreNativeVmSize[sizeof(AnmVm) == 0x2A4 ? 1 : -1];
typedef char EnemyCoreNativeVmColor1[offsetof(AnmVm, color1) == 0x1F0 ? 1 : -1];
typedef char EnemyCoreNativeVmColor2[offsetof(AnmVm, color2) == 0x1F4 ? 1 : -1];
typedef char EnemyCoreNativeVmFlags[offsetof(AnmVm, flagsWord) == 0x1F8 ? 1 : -1];
typedef char EnemyCoreNativeVmScript[offsetof(AnmVm, scriptIndex) == 0x21A ? 1 : -1];
typedef char EnemyCoreNativeVmSprite[offsetof(AnmVm, loadedSprite) == 0x224 ? 1 : -1];


typedef char EnemyCoreTrailSampleSizeIs1C[
    (sizeof(EnemyCoreTrailSampleView) == 0x1C) ? 1 : -1];

// ECL's trail writer and the exact draw owner both use a separate vertex
// array at +0x3E68. It is not an extension of the 96 history records.

typedef char EnemyCoreTrailVertexSizeIs1C[
    (sizeof(EnemyCoreTrailVertexView) == 0x1C) ? 1 : -1];

struct EnemyCorePlayerView;
struct EnemyCoreView;


typedef char EnemyCoreSideStateSizeIs38[
    (sizeof(EnemyCoreSideStateView) == 0x38) ? 1 : -1];

struct EnemyCorePlayerView
{
    unsigned char unknown00000[0x364];
    Effect *focusEffect364;
    unsigned char unknown00368[0x1B88 - 0x368];
    Float3 position1B88;
    unsigned char unknown01B94[0x30364 - 0x1B94];
    PlayerEnemyTrackingStateView tracking30364;
    unsigned char unknown30380[0x303C8 - 0x30380];
    ZunTimer stateTimer303C8;

};
typedef char EnemyCorePlayerFocusAt364[
    (offsetof(EnemyCorePlayerView, focusEffect364) == 0x364) ? 1 : -1];
typedef char EnemyCorePlayerPositionAt1B88[
    (offsetof(EnemyCorePlayerView, position1B88) == 0x1B88) ? 1 : -1];
typedef char EnemyCorePlayerHomingAt3037C[
    (offsetof(EnemyCorePlayerView, tracking30364) +
     offsetof(PlayerEnemyTrackingStateView, homingTarget18) == 0x3037C) ? 1 : -1];

// The reset copy and the native Etama call use the same 0x214-byte payload.
// Keep the independently reviewed offsets without a second descriptor class.
typedef char EnemyCoreBulletDescriptorMatchesEtamaFields[
    (sizeof(BulletSpawnDescriptor) == 0x214 &&
     offsetof(BulletSpawnDescriptor, bulletType) == 0x00 &&
     offsetof(BulletSpawnDescriptor, color) == 0x02 &&
     offsetof(BulletSpawnDescriptor, position) == 0x04 &&
     offsetof(BulletSpawnDescriptor, angle) == 0x10 &&
     offsetof(BulletSpawnDescriptor, angleStep) == 0x14 &&
     offsetof(BulletSpawnDescriptor, speed1) == 0x18 &&
     offsetof(BulletSpawnDescriptor, speed2) == 0x1C &&
     offsetof(BulletSpawnDescriptor, count1) == 0x1F4 &&
     offsetof(BulletSpawnDescriptor, count2) == 0x1F6 &&
     offsetof(BulletSpawnDescriptor, aimMode) == 0x1F8 &&
     offsetof(BulletSpawnDescriptor, unknown1FA) == 0x1FA &&
     offsetof(BulletSpawnDescriptor, transformFlags) == 0x1FC &&
     offsetof(BulletSpawnDescriptor, templateSprites) == 0x210) ? 1 : -1];


typedef char EnemyCoreViewSizeIs5430[
    (sizeof(EnemyCoreView) == 0x5430) ? 1 : -1];
typedef char EnemyCoreWorldAt2DD4[
    (offsetof(EnemyCoreView, worldPosition2DD4) == 0x2DD4) ? 1 : -1];
typedef char EnemyCoreFlagsAt337C[
    (offsetof(EnemyCoreView, primaryFlags337C) == 0x337C) ? 1 : -1];
typedef char EnemyCoreTrailAt33E8[
    (offsetof(EnemyCoreView, trailSamples33E8) == 0x33E8) ? 1 : -1];
typedef char EnemyCoreTrailVerticesAt3E68[
    (offsetof(EnemyCoreView, trailVertices3E68) == 0x3E68) ? 1 : -1];
typedef char EnemyCoreTrailHistoryEndsAtVertices[
    (offsetof(EnemyCoreView, trailSamples33E8) +
     sizeof(((EnemyCoreView *)0)->trailSamples33E8) ==
     offsetof(EnemyCoreView, trailVertices3E68)) ? 1 : -1];
typedef char EnemyCoreTrailVerticesEndAt5368[
    (offsetof(EnemyCoreView, trailVertices3E68) +
     sizeof(((EnemyCoreView *)0)->trailVertices3E68) ==
     offsetof(EnemyCoreView, unknown5368)) ? 1 : -1];
typedef char EnemyCoreTrailFlagsAt53A0[
    (offsetof(EnemyCoreView, trailFlags53A0) == 0x53A0) ? 1 : -1];
typedef char EnemyCoreAttachedEffectStateAt53B4[
    (offsetof(EnemyCoreView, attachedEffectState53B4) == 0x53B4) ? 1 : -1];
typedef char EnemyCorePhaseEndSecondsAt5420[
    (offsetof(EnemyCoreView, phaseEndSeconds5420) == 0x5420) ? 1 : -1];
typedef char EnemyCoreDamageTimerAt53A8[
    (offsetof(EnemyCoreView, damageReductionTimer53A8) == 0x53A8) ? 1 : -1];


typedef char EnemyManagerCoreSizeIs2AC450[
    (sizeof(EnemyManagerCoreView) == 0x2AC450) ? 1 : -1];
typedef char EnemyManagerCoreEnemiesAt5758[
    (offsetof(EnemyManagerCoreView, enemies5758) == 0x5758) ? 1 : -1];
typedef char EnemyManagerCoreDrawHeadsAt2AC410[
    (offsetof(EnemyManagerCoreView, drawGroupHeads2AC410) == 0x2AC410) ? 1 : -1];

struct EnemyCoreFrontView
{
    unsigned char unknown00000[0x1095C];
    int scriptedUpdateFreeze1095C;
};
typedef char EnemyCoreFrontFreezeAt1095C[
    (offsetof(EnemyCoreFrontView, scriptedUpdateFreeze1095C) == 0x1095C) ? 1 : -1];

extern AsciiManager g_AsciiManager;
struct PlayerSharedRuntimeView;
extern PlayerSharedRuntimeView *g_PlayerSharedRuntime;
extern unsigned char g_EnemyCoreSchedule[];

// Reconstruction aliases for the shared physical leaf at 0x0044AB20.
// Its complete body is xor eax,eax;ret, not an unreconstructed callback stub.
// The ignored receiver and original source ownership remain unresolved.
int EnemyCoreRunLifeCallback(EnemyCoreView *enemy)
{
    return 0;
}

int EnemyCoreRunTimerCallback(EnemyCoreView *enemy)
{
    return 0;
}
// Use the actual Th09EclRunState release API declared by EclRunState.inl.
static void EnemyCoreUpdateAttachedEffects(EnemyCoreView *enemy);

struct EnemyAttachedEffectUpdateView
{
    unsigned char unknown000[0x48];
    Float3 position48;
    unsigned char unknown054[0x7C - 0x54];
    float distance7C;
    float angle80;
};

#include "RuntimeAngleMath.hpp"
extern long double __stdcall AnmProjectionAbs(float value);

static void EnemyCoreUpdateAttachedEffects(EnemyCoreView *enemy)
{
    for (int i = 0; i < enemy->attachedEffectState53B4.count60; ++i)
    {
        EnemyAttachedEffectUpdateView *effect =
            reinterpret_cast<EnemyAttachedEffectUpdateView *>(
                enemy->attachedEffectState53B4.effects00[i]);
        if (effect != 0)
        {
            effect->position48 = enemy->position2D74;
            if (effect->distance7C < enemy->attachedEffectState53B4.distance64)
                effect->distance7C += 0.3f;
            else
                effect->distance7C = enemy->attachedEffectState53B4.distance64;
            effect->angle80 =
                AddNormalizeAngle(effect->angle80, 0.031415928f);
        }
    }
}


int __fastcall EnemyManagerView::OnUpdate(EnemyManagerView *enemyManager)
{
    EnemyManagerCoreView *manager =
        reinterpret_cast<EnemyManagerCoreView *>(enemyManager);
    int bombHit = 0;
    int damageOccurred;
    int trailIndex;
    int hitKind;
    int enemyIndex;

    if ((g_GameManager.flags & 0x1800) != 0)
        return 1;

    reinterpret_cast<FrontEnemyIndicatorView *>(g_PlayerSharedRuntime)->SetSideEnemyIndicatorPosition(
        manager->sideIndex31C, &Float3(-999.0f, 0.0f, 0.0f));
    g_Supervisor.SelectSide(manager->sideIndex31C);

    manager->drawGroupHeads2AC410[3] = 0;
    manager->drawGroupHeads2AC410[2] = 0;
    manager->drawGroupHeads2AC410[1] = 0;
    manager->drawGroupHeads2AC410[0] = 0;

    if (!reinterpret_cast<EnemyCoreFrontView *>(g_PlayerSharedRuntime)->scriptedUpdateFreeze1095C)
    {
        if (manager->scheduleObject2AC400 != 0)
        {
            manager->scheduleRuntime2AC3E0.Run();
        }
        else if (manager->normalEnemyCount2AC3B0 == 0 ||
                 (manager->sideTimer2AC3C8 > 0))
        {
            manager->scheduleHighBit2AC3F8 =
                g_EnemyCoreSchedule[manager->scheduleIndex2AC3D4] >> 7;
            manager->scheduleObject2AC400 =
                manager->primaryEclManager000.GetSubroutine(
                    g_EnemyCoreSchedule[manager->scheduleIndex2AC3D4++] & 0x7F);
            manager->scheduleRuntime2AC3E0.timer00 = 0;
        }
    }

    manager->activeEnemyCount2AC3AC = 0;
    manager->normalEnemyCount2AC3B0 = 0;
    manager->priorityEnemy2AC444 = 0;
    manager->firstActiveEnemy2AC448 = 0;
    manager->specialEnemyCount2AC3B4 = 0;
    manager->rewardEnemyCount2AC3B8 = 0;

    EnemyCoreView *enemy = manager->enemies5758;
    if (manager->sideState320->player04->focusEffect364 != 0)
        manager->sideTimer2AC3C8 = 0;
    else
        manager->sideTimer2AC3C8++;

    for (enemyIndex = 0; enemyIndex < 128; ++enemyIndex, ++enemy)
    {
        EnemyCoreView **drawHead;
        unsigned int flags = enemy->primaryFlags337C;

        if ((flags & ENEMY_CORE_ACTIVE) == 0)
        {
            if (manager->sideState320->player04->tracking30364.homingTarget18 ==
                    reinterpret_cast<EnemyView *>(enemy))
                manager->sideState320->player04->tracking30364.homingTarget18 = 0;
            continue;
        }

        if ((manager->sideState320->flags34 & 1) != 0 &&
            ((enemy->secondaryFlags3380 >> 10) & 3) != 1 &&
            ((enemy->secondaryFlags3380 >> 10) & 3) != 2)
        {
            if ((flags & ENEMY_CORE_NO_SPRITE) == 0)
            {
                drawHead =
                    &manager->drawGroupHeads2AC410[enemy->drawGroup3387];
                goto append_enemy_to_draw_group;
            }
            continue;
        }

        {
            damageOccurred = 0;

            if ((flags & ENEMY_CORE_FORCE_DEATH) != 0)
            {
                enemy->worldPosition2DD4 =
                    enemy->position2D74 + enemy->positionOffset2D80;
                enemy->worldPosition2DD4.z = 0.0f;
                goto process_enemy_death;
            }

            ++manager->activeEnemyCount2AC3AC;
            if ((enemy->secondaryFlags3380 & 0x4DC0) == 0)
                ++manager->normalEnemyCount2AC3B0;
            if ((enemy->secondaryFlags3380 & 0x0C00) != 0)
                ++manager->specialEnemyCount2AC3B4;
            if ((enemy->secondaryFlags3380 & 0x01C0) != 0)
                ++manager->rewardEnemyCount2AC3B8;

            if ((enemy->secondaryFlags3380 & 0x1000) != 0)
            {
                if (enemy->specialAttackTimer5424.HasTickedEvery(2))
                {
                    manager->sideState320->effectManager0C->SpawnEffect(
                        9,
                        reinterpret_cast<EffectFloat3 *>(&enemy->position2D74),
                        1,
                        static_cast<unsigned int>(-1));
                }

                if (enemy->specialAttackTimer5424 >
                        manager->specialAttackThreshold2AC44C)
                {
                    BulletSpawnDescriptorCtorView storage;
                    BulletSpawnDescriptor &descriptor = storage.descriptor;
                    descriptor.position = enemy->position2D74;
                    EnemyCoreSideStateView *spawnSide = manager->sideState320;
                    descriptor.bulletType = 1;
                    descriptor.aimMode = 0;
                    descriptor.count1 =
                        static_cast<short>(spawnSide->characterIndex20 == 13 ? 1 : 3);
                    descriptor.speed1 =
                        static_cast<float>(g_GameManager.valueB4) * 0.1f +
                        1.0f;
                    descriptor.count2 = 1;
                    descriptor.angle = 0.0f;
                    descriptor.angleStep = 0.15707964f;
                    descriptor.speed2 = 0.0f;
                    descriptor.unknown1FA = 0;
                    descriptor.transformFlags = 4;
                    descriptor.color = 2;
                    spawnSide->etama08->SpawnBulletPatternPrimary(
                        &descriptor);
                    enemy->primaryFlags337C &= ~ENEMY_CORE_ACTIVE;
                    reinterpret_cast<EnemyView *>(enemy)->CleanupAfterDeactivation();
                    continue;
                }
            }

            if (((enemy->primaryFlags337C & ENEMY_CORE_PAUSE_WITH_PLAYER) != 0 &&
                 reinterpret_cast<PlayerLifecycleView *>(manager->sideState320->player04)->GetUpdateState() != 0) ||
                (enemy->secondaryFlags3380 & 0x10) != 0)
            {
                enemy->bossTimer2E64--;
                goto update_damage_flash;
            }

        run_enemy_ecl:
            if (manager->primaryEclManager000.RunEcl(
                    reinterpret_cast<EnemyView *>(enemy)) == -1)
            {
                enemy->primaryFlags337C &= ~ENEMY_CORE_ACTIVE;
                reinterpret_cast<EnemyView *>(enemy)->CleanupAfterDeactivation();
                continue;
            }

            if ((enemy->primaryFlags337C & ENEMY_CORE_SKIP_MOVEMENT) == 0)
            {
                reinterpret_cast<EnemyView *>(enemy)->ClampPositionToMovementBounds();
                reinterpret_cast<EnemyView *>(enemy)->IntegrateMotion();
                reinterpret_cast<EnemyView *>(enemy)->ClampPositionToMovementBounds();
                enemy->worldPosition2DD4 =
                    enemy->position2D74 + enemy->positionOffset2D80;
                enemy->worldPosition2DD4.z = 0.0f;
            }
            else
            {
                enemy->worldPosition2DD4 =
                    enemy->position2D74 + enemy->positionOffset2D80;
                enemy->worldPosition2DD4.z = 0.0f;
            }

            if (enemy->trailFlags53A0 != 0)
            {
                for (trailIndex = enemy->trailHistoryLength53A2 - 1;
                     trailIndex > 0; --trailIndex)
                {
                    enemy->trailSamples33E8[trailIndex].position00 =
                        enemy->trailSamples33E8[trailIndex - 1].position00;
                    enemy->trailSamples33E8[trailIndex].velocity0C =
                        enemy->trailSamples33E8[trailIndex - 1].velocity0C;
                    enemy->trailSamples33E8[trailIndex].angle18 =
                        enemy->trailSamples33E8[trailIndex - 1].angle18;
                }
                enemy->trailSamples33E8[0].position00 =
                    enemy->worldPosition2DD4;
                enemy->trailSamples33E8[0].velocity0C = enemy->velocity2D8C;
                enemy->trailSamples33E8[0].angle18 = enemy->movementAngle2DE0;
            }

            EnemyCoreLoadedSpriteView *loadedSprite =
                reinterpret_cast<EnemyCoreLoadedSpriteView *>(enemy->primaryVm008.loadedSprite);
            if (loadedSprite == 0)
                enemy->primaryFlags337C |= ENEMY_CORE_NO_SPRITE;

            const Float3 *worldPosition;
            if ((enemy->primaryFlags337C & ENEMY_CORE_NO_SPRITE) == 0 &&
                (enemy->primaryFlags337C & ENEMY_CORE_HAS_BEEN_IN_BOUNDS) == 0 &&
                (worldPosition = reinterpret_cast<const Float3 *>(
                     enemy->worldPosition2DD4.operator float *()),
                 g_GameManager.IsWithinPlayfield(
                    worldPosition->x,
                    worldPosition->y,
                    loadedSprite->extent34,
                    loadedSprite->extent30)))
            {
                enemy->primaryFlags337C |= ENEMY_CORE_HAS_BEEN_IN_BOUNDS;
            }
            else if ((enemy->primaryFlags337C & ENEMY_CORE_HAS_BEEN_IN_BOUNDS) != 0 &&
                     (enemy->primaryFlags337C & ENEMY_CORE_ALLOW_OFFSCREEN) == 0)
            {
                if ((enemy->trailFlags53A0 == 0 &&
                     !g_GameManager.IsWithinPlayfield(
                         enemy->worldPosition2DD4.x,
                         enemy->worldPosition2DD4.y,
                        reinterpret_cast<EnemyCoreLoadedSpriteView *>(enemy->primaryVm008.loadedSprite)->extent34,
                        reinterpret_cast<EnemyCoreLoadedSpriteView *>(enemy->primaryVm008.loadedSprite)->extent30)) ||
                    (enemy->trailFlags53A0 != 0 &&
                     !g_GameManager.IsWithinPlayfield(
                         enemy->worldPosition2DD4.x,
                         enemy->worldPosition2DD4.y,
                        reinterpret_cast<EnemyCoreLoadedSpriteView *>(enemy->primaryVm008.loadedSprite)->extent34,
                        reinterpret_cast<EnemyCoreLoadedSpriteView *>(enemy->primaryVm008.loadedSprite)->extent30) &&
                     !g_GameManager.IsWithinPlayfield(
                        enemy->trailSamples33E8[
                            enemy->trailHistoryLength53A2 - 1].position00.x,
                        enemy->trailSamples33E8[
                            enemy->trailHistoryLength53A2 - 1].position00.y,
                        reinterpret_cast<EnemyCoreLoadedSpriteView *>(enemy->primaryVm008.loadedSprite)->extent34,
                        reinterpret_cast<EnemyCoreLoadedSpriteView *>(enemy->primaryVm008.loadedSprite)->extent30)))
                {
                    enemy->primaryFlags337C &= ~ENEMY_CORE_ACTIVE;
                    reinterpret_cast<EnemyView *>(enemy)->CleanupAfterDeactivation();
                    continue;
                }
            }

            if (EnemyCoreRunLifeCallback(enemy))
                goto run_enemy_ecl;
            if (enemy->timerCallbackThreshold33D0 >= 0 &&
                EnemyCoreRunTimerCallback(enemy))
                goto run_enemy_ecl;

            enemy->primaryVm008.color1 = enemy->displayColor2E70;
            g_AnmManager->ExecuteScript(&enemy->primaryVm008);
            enemy->displayColor2E70 = enemy->primaryVm008.color1;
            for (int vmIndex = 0; vmIndex < 2; ++vmIndex)
            {
                if (enemy->secondaryVms2AC[vmIndex].scriptIndex >= 0 &&
                    g_AnmManager->ExecuteScript(
                        &enemy->secondaryVms2AC[vmIndex]))
                {
                    enemy->secondaryVms2AC[vmIndex].scriptIndex = -1;
                }
            }

            hitKind = 0;
            if ((enemy->primaryFlags337C &
                 (ENEMY_CORE_NO_SPRITE | ENEMY_CORE_HIDE_PRIMARY)) == 0)
            {
                if ((enemy->primaryFlags337C & ENEMY_CORE_COLLISION) != 0 &&
                    ((enemy->secondaryFlags3380 & 0x01C0) == 0 ||
                     manager->sideState320->characterIndex20 != 11))
                {
                    reinterpret_cast<EnemyAppendCollisionView *>(enemy)
                        ->AppendPlayerCollisionBox(
                            &enemy->worldPosition2DD4,
                            &enemy->hitbox2DBC);
                    if (enemy->trailFlags53A0 != 0)
                    {
                        Float3 trailHitbox = enemy->hitbox2DBC;
                        for (trailIndex = 1;
                             trailIndex < enemy->trailCollisionLength53A4;
                             trailIndex += 6)
                        {
                            if ((enemy->trailFlags53A0 & 2) != 0)
                            {
                                trailHitbox = enemy->hitbox2DBC -
                                    (enemy->hitbox2DBC *
                                     static_cast<float>(trailIndex)) /
                                    static_cast<float>(
                                        enemy->trailCollisionLength53A4);
                            }
                            reinterpret_cast<EnemyAppendCollisionView *>(enemy)
                                ->AppendPlayerCollisionBox(
                                    &enemy->trailSamples33E8[
                                        trailIndex].position00,
                                    &trailHitbox);
                        }
                    }
                }

                enemy->lastDamage33AC = 0;
                if ((enemy->primaryFlags337C & ENEMY_CORE_ACCEPTS_DAMAGE) != 0)
                {
                    int damage = reinterpret_cast<PlayerDamagePlayerView *>(
                        manager->sideState320->player04)->CalcDamageToEnemy(
                        reinterpret_cast<PlayerDamageFloat3 *>(
                            &enemy->worldPosition2DD4),
                        reinterpret_cast<PlayerDamageFloat3 *>(
                            &enemy->hitbox2DBC),
                        &enemy->primaryDamageAccumulator2E5C,
                        &bombHit,
                        &enemy->secondaryDamageAccumulator2E60);
                    if (enemy->secondaryHitbox2DC8.x > 0.0f)
                    {
                        reinterpret_cast<PlayerDamagePlayerView *>(
                            manager->sideState320->player04)->CalcDamageToEnemy(
                            reinterpret_cast<PlayerDamageFloat3 *>(
                                &enemy->worldPosition2DD4),
                            reinterpret_cast<PlayerDamageFloat3 *>(
                                &enemy->secondaryHitbox2DC8),
                            &enemy->primaryDamageAccumulator2E5C,
                            &bombHit,
                            &enemy->secondaryDamageAccumulator2E60);
                    }

                    if (damage > 0)
                    {
                        if ((enemy->primaryFlags337C & ENEMY_CORE_DAMAGEABLE) != 0)
                        {
                            if (enemy->damageReductionTimer53A8 > 0)
                            {
                                if ((enemy->primaryFlags337C & ENEMY_CORE_BOSS) != 0)
                                    damage /= 9;
                                else
                                    damage = 0;
                            }

                            if (damage == enemy->primaryDamageAccumulator2E5C)
                                hitKind = 1;
                            if (damage == enemy->secondaryDamageAccumulator2E60)
                                hitKind = 2;

                            if ((enemy->secondaryFlags3380 & 0x01C0) != 0)
                            {
                                int secondaryPart =
                                    damage - enemy->primaryDamageAccumulator2E5C;
                                int primaryPart;
                                if ((enemy->secondaryFlags3380 & 0x1000) == 0)
                                    primaryPart =
                                        enemy->primaryDamageAccumulator2E5C / 4;
                                else
                                    primaryPart =
                                        enemy->primaryDamageAccumulator2E5C / 2;
                                damage = primaryPart + secondaryPart * 4;
                            }

                            enemy->life2E48 -= damage;
                            enemy->lastDamage33AC = damage;
                        }
                        damageOccurred = 1;
                    }

                    EnemyCoreSideStateView *side = manager->sideState320;
                    Float3 oldTargetDelta =
                        side->player04->tracking30364.trackedEnemyPosition00 -
                        side->player04->position1B88;
                    EnemyCorePlayerView *player = side->player04;
                    Float3 newTargetDelta =
                        enemy->worldPosition2DD4 - player->position1B88;
                    if (oldTargetDelta.x * oldTargetDelta.x +
                            oldTargetDelta.y * oldTargetDelta.y >
                        newTargetDelta.x * newTargetDelta.x +
                            newTargetDelta.y * newTargetDelta.y)
                    {
                        player->tracking30364.trackedEnemyPosition00 =
                            enemy->worldPosition2DD4;
                    }

                    player = manager->sideState320->player04;
                    if (static_cast<float>(AnmProjectionAbs(
                            enemy->worldPosition2DD4.x -
                            player->position1B88.x)) < 64.0f)
                    {
                        EnemyCoreView *homing = reinterpret_cast<EnemyCoreView *>(
                            player->tracking30364.homingTarget18);
                        if (homing == 0 ||
                            homing->position2D74.y > enemy->worldPosition2DD4.y)
                        {
                            player->tracking30364.homingTarget18 =
                                reinterpret_cast<EnemyView *>(enemy);
                        }
                    }
                }
            }

            if ((enemy->secondaryFlags3380 & 1) != 0 && enemy->life2E48 > 0)
                enemy->secondaryFlags3380 &= ~1u;
            if (enemy->life2E48 > 0 || (enemy->secondaryFlags3380 & 9) != 0)
                goto update_damage_flash;

        process_enemy_death:
            enemy->secondaryFlags3380 |= 1;
            enemy->phaseEndSeconds5420 =
                (enemy->timerCallbackThreshold33D0 -
                 static_cast<int>(enemy->bossTimer2E64)) / 60;
            enemy->timerCallbackThreshold33D0 = -1;
            for (int callbackIndex = 0; callbackIndex < 4; ++callbackIndex)
                enemy->lifeCallbackThresholds33B0[callbackIndex] = -1;
            for (int childIndex = 0; childIndex < 4; ++childIndex)
            {
                if (enemy->childEclBlocks33D8[childIndex] != 0)
                {
                    g_ZunMemory.Free(enemy->childEclBlocks33D8[childIndex]);
                    enemy->childEclBlocks33D8[childIndex] = 0;
                }
            }

            switch ((enemy->primaryFlags337C >> 17) & 7)
            {
            case 3:
            {
                enemy->primaryFlags337C &= 0xFFF1FFF7;
                enemy->life2E48 = 1;
                if (enemy->deathAnmScripts3368[0] >= 0)
                {
                    manager->sideState320->effectManager0C->SpawnEffect(
                        enemy->deathAnmScripts3368[0] + 20,
                        reinterpret_cast<EffectFloat3 *>(
                            &enemy->worldPosition2DD4),
                        1,
                        static_cast<unsigned int>(-1));
                    manager->sideState320->effectManager0C->SpawnEffect(
                        enemy->deathAnmScripts3368[0] + 20,
                        reinterpret_cast<EffectFloat3 *>(
                            &enemy->worldPosition2DD4),
                        1,
                        static_cast<unsigned int>(-1));
                    manager->sideState320->effectManager0C->SpawnEffect(
                        enemy->deathAnmScripts3368[0] + 20,
                        reinterpret_cast<EffectFloat3 *>(
                            &enemy->worldPosition2DD4),
                        1,
                        static_cast<unsigned int>(-1));
                }
                EnemyCorePlayerView *player = manager->sideState320->player04;
                if (reinterpret_cast<PlayerLifecycleView *>(player)->GetUpdateState() == 0)
                {
                    player->stateTimer303C8 = 90;
                    reinterpret_cast<PlayerState4OpsView *>(manager->sideState320->player04)->SetUpdateState(3);
                }
                enemy->primaryFlags337C &= 0xE7FFFFFF;
                break;
            }

            case 1:
                reinterpret_cast<GameManagerSideStateView *>(
                    manager->sideState320)->AddScore(enemy->score2E54);
                enemy->primaryFlags337C =
                    (enemy->primaryFlags337C & 0xFFEFFFB3) |
                    ENEMY_CORE_PERSIST_AFTER_DEATH;
                goto release_death_effects;

            case 0:
                reinterpret_cast<GameManagerSideStateView *>(
                    manager->sideState320)->AddScore(enemy->score2E54);
                enemy->primaryFlags337C &= ~ENEMY_CORE_ACTIVE;
            release_death_effects:
                if ((enemy->primaryFlags337C & ENEMY_CORE_BOSS) != 0)
                    Th09EclRunState::ReleaseAttachedEffects(
                        reinterpret_cast<EnemyView *>(enemy));
            case 2:
                enemy->life2E48 = 0;
                break;

            default:
                break;
            }

            if ((enemy->primaryFlags337C & ENEMY_CORE_FORCE_DEATH) == 0)
            {
                reinterpret_cast<EnemyView *>(enemy)->HandleDeathRewards(hitKind);
            }

            if (enemy->deathCallbackSubId2D2E >= 0)
            {
                reinterpret_cast<EnemyView *>(enemy)->ResetBulletRankInfluence();
                enemy->activeCallDepth2D2A = 0;
                for (int callbackIndex = 0; callbackIndex < 4; ++callbackIndex)
                    enemy->lifeCallbackThresholds33B0[callbackIndex] = -1;
                enemy->timerCallbackThreshold33D0 = -1;
                EnemyCoreReleaseChildEclBlocks(
                    reinterpret_cast<EnemyView *>(enemy));
                EnemyManagerCoreView *owner =
                    reinterpret_cast<EnemyManagerCoreView *>(enemy->manager0000);
                // Copy only the target-observed 0x214 descriptor, not the
                // remaining unknown bytes in the 0x240 bullet-state region.
                *reinterpret_cast<BulletSpawnDescriptor *>(
                    enemy->bulletState2E74) =
                    *reinterpret_cast<const BulletSpawnDescriptor *>(
                        owner->spawnTemplate328.bulletState2E74);
                enemy->shootInterval30B4 = 0;
                manager->primaryEclManager000.InitializeSubroutine(
                        &enemy->mainContext07F4,
                        enemy->deathCallbackSubId2D2E);
                enemy->deathCallbackSubId2D2E = -1;
            }

        update_damage_flash:
            if (enemy->damageFlashTimer336C != 0)
            {
                --enemy->damageFlashTimer336C;
                enemy->primaryVm008.flagsWord &= ~0x20000u;
            }
            else if (damageOccurred)
            {
                if ((enemy->secondaryFlags3380 & 6) < 4)
                    g_SoundPlayer.PlaySoundPositionedByIdx(20, enemy->worldPosition2DD4.x);
                else
                    g_SoundPlayer.PlaySoundPositionedByIdx(37, enemy->worldPosition2DD4.x);

                reinterpret_cast<unsigned char *>(&enemy->primaryVm008.color2)[2] = 0xFF;
                reinterpret_cast<unsigned char *>(&enemy->primaryVm008.color2)[1] = 0x60;
                reinterpret_cast<unsigned char *>(&enemy->primaryVm008.color2)[0] = 0x80;
                reinterpret_cast<unsigned char *>(&enemy->primaryVm008.color2)[3] =
                    static_cast<unsigned char>(
                        enemy->primaryVm008.color1 >> 24);
                enemy->primaryVm008.flagsWord |= 0x20000;
                enemy->damageFlashTimer336C = 1;
            }
            else
            {
                enemy->primaryVm008.flagsWord &= ~0x20000u;
            }

            // The target retains this boss-kind limit even though the observed
            // TH09 field is one bit. The adjacent TH08 source has the same check.
            unsigned int boss = (enemy->primaryFlags337C >> 1) & 1;
            if (boss != 0 && boss < 4)
            {
                Float3 markerPosition;
                if ((enemy->primaryFlags337C & ENEMY_CORE_NO_SPRITE) == 0)
                    markerPosition.x = enemy->worldPosition2DD4.x + 32.0f;
                else
                    markerPosition.x = -999.0f;
                markerPosition.y = 472.0f;
                markerPosition.z = 0.0f;
                reinterpret_cast<AsciiBossMarkerVmView *>(&g_AsciiManager)->SetBossMarkerPosition(enemy->bossSlot336B,
                                                   &markerPosition);
                int markerState = (enemy->secondaryFlags3380 >> 1) & 3;
                if (markerState == 0)
                    g_AsciiManager.SetBossMarkerState(enemy->bossSlot336B,
                        (enemy->primaryVm008.flagsWord >> 17) & 1);
                else
                    g_AsciiManager.SetBossMarkerState(enemy->bossSlot336B,
                                                    markerState + 1);
            }

            if (((enemy->secondaryFlags3380 & 0x200) != 0 &&
                 manager->priorityEnemy2AC444 == 0) ||
                (enemy->secondaryFlags3380 & 0x1000) != 0)
            {
                manager->priorityEnemy2AC444 = enemy;
            }
            if (manager->firstActiveEnemy2AC448 == 0)
                manager->firstActiveEnemy2AC448 = enemy;
            if ((enemy->secondaryFlags3380 & 0x0C00) == 0x0C00)
            {
                reinterpret_cast<FrontEnemyIndicatorView *>(g_PlayerSharedRuntime)->SetSideEnemyIndicatorPosition(
                    manager->sideIndex31C, &enemy->position2D74);
                manager->priorityEnemy2AC444 = enemy;
            }
            else if ((enemy->secondaryFlags3380 & 0x2000) != 0)
            {
                manager->priorityEnemy2AC444 = enemy;
            }

            EnemyCoreUpdateAttachedEffects(enemy);
            enemy->bossTimer2E64++;
            if (enemy->damageReductionTimer53A8 > 0)
                enemy->damageReductionTimer53A8--;
        }

        if ((enemy->primaryFlags337C & ENEMY_CORE_NO_SPRITE) == 0 &&
            (enemy->primaryFlags337C & ENEMY_CORE_ACTIVE) != 0)
        {
            drawHead =
                &manager->drawGroupHeads2AC410[enemy->drawGroup3387];
        }
        else
            continue;

    append_enemy_to_draw_group:
        enemy->nextInDrawGroup04 = *drawHead;
        *drawHead = enemy;
    }

    manager->frameTimer2AC404++;
    return 1;
}
