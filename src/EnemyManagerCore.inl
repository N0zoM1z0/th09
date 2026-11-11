#include "EnemyNativeTrailRenderCapacity.hpp"
#include "EnemyNativePoolCapacity.hpp"
#include "AsciiGameManagerView.hpp"
#include "PlayerDamageView.hpp"
#include "FrontFactoryConstructView.hpp"
#include "BulletSpawnDescriptorCtorView.hpp"
#include "ZunTimer.hpp"
// Lexical member of EclManager.cpp; reuses Th09EclRunState::ManagerStateView.
#include "AsciiManager.hpp"
#include "EnemyManager.hpp"
#include "EnemyChildEclBlocks.hpp"
#include "EnemyAttachedEffectStateView.hpp"
#include "BulletManager.hpp"
#include "PlayerLifecycleView.hpp"
#include "PlayerCollisionQueryCapacity.hpp"
#include "Supervisor.hpp"
#include "SoundPlayer.hpp"
#include "AnmManager.hpp"
#include "ZunMemory.hpp"
#include "PlayerEnemyTrackingNative.hpp"
#include "PlayerSharedRuntimeView.hpp"

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
    if (count1804 >= PlayerCollisionQueryCapacity::RecordCount)
        return;

    // Retail recomputes the live count seven times, addressing the original
    // 0x30-byte records through native modulo-32 byte arithmetic. A negative
    // native count must not become a C++ out-of-bounds array subscript.

    (*reinterpret_cast<PlayerCollisionQueryRecordView *>(
        reinterpret_cast<unsigned int>(&records[0]) +
        static_cast<unsigned int>(count1804) * sizeof(records[0]))).position00 = *position;
    (*reinterpret_cast<PlayerCollisionQueryRecordView *>(
        reinterpret_cast<unsigned int>(&records[0]) +
        static_cast<unsigned int>(count1804) * sizeof(records[0]))).halfSize18 = *size;
    (*reinterpret_cast<PlayerCollisionQueryRecordView *>(
        reinterpret_cast<unsigned int>(&records[0]) +
        static_cast<unsigned int>(count1804) * sizeof(records[0]))).halfSize18.x *= 0.5f;
    (*reinterpret_cast<PlayerCollisionQueryRecordView *>(
        reinterpret_cast<unsigned int>(&records[0]) +
        static_cast<unsigned int>(count1804) * sizeof(records[0]))).halfSize18.y *= 0.5f;
    (*reinterpret_cast<PlayerCollisionQueryRecordView *>(
        reinterpret_cast<unsigned int>(&records[0]) +
        static_cast<unsigned int>(count1804) * sizeof(records[0]))).radius24 = 0.0f;
    (*reinterpret_cast<PlayerCollisionQueryRecordView *>(
        reinterpret_cast<unsigned int>(&records[0]) +
        static_cast<unsigned int>(count1804) * sizeof(records[0]))).angle28 = 0.0f;
    (*reinterpret_cast<PlayerCollisionQueryRecordView *>(
        reinterpret_cast<unsigned int>(&records[0]) +
        static_cast<unsigned int>(count1804) * sizeof(records[0]))).unknown2C = unknown;
    // The final INC DWORD wraps before the next append's signed bound test.
    count1804 = static_cast<int>(static_cast<unsigned int>(count1804) + 1u);
}

void EnemyView::AppendPlayerCollisionBox(
    const Float3 *position,
    const Float3 *size)
{
    Float3 scaledSize;
    scaledSize = *size / 1.5f;

    manager00->sideState320->playerLifecycle04->collisionQuery36C.AppendBoxRecord(
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

typedef char EnemyCoreLoadedSpriteNativeDimensions[
    (sizeof(AnmLoadedSprite) == 0x44 &&
     offsetof(AnmLoadedSprite, heightPx) == 0x30 &&
     offsetof(AnmLoadedSprite, widthPx) == 0x34) ? 1 : -1];

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

struct EnemyCoreView;


// The live +0x320 side pointer in the 3,900-byte Enemy calc callback
// addresses the same 0x38-byte GameManager side as ECL/Etama. Its +0x08
// Etama and +0x20 character value are not a separate Enemy side copy.
typedef char EnemyCoreRealGameManagerSpawnSide[
    (sizeof(AsciiGameManagerSideView) == 0x38 &&
     sizeof(EnemySideStateView) == 0x38 &&
     offsetof(AsciiGameManagerSideView, etama08) == 0x08 &&
     offsetof(AsciiGameManagerSideView, character20) == 0x20 &&
     offsetof(EnemySideStateView, etama08) == 0x08 &&
     offsetof(EnemySideStateView, characterIndex20) == 0x20 &&
     offsetof(EnemyManagerView, sideState320) == 0x320) ? 1 : -1];

typedef char EnemyCoreSideStateSizeIs38[
    (sizeof(EnemyCoreSideStateView) == 0x38 &&
     offsetof(EnemyCoreSideStateView, owner14) == 0x14 &&
     offsetof(EnemyCoreSideStateView, characterIndex20) == 0x20 &&
     offsetof(EnemyCoreSideStateView, flags34) == 0x34) ? 1 : -1];

// The reset copy and the native Etama call use the same 0x214-byte payload.
// Keep the independently reviewed offsets without a second descriptor class.
typedef char EnemyCoreNativeDescriptorResetAt2E74[
    (offsetof(EnemyView, bulletDescriptor2E74) == 0x2E74 &&
     sizeof(((EnemyView *)0)->bulletDescriptor2E74) == 0x214) ? 1 : -1];
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
    (sizeof(EnemyView) == 0x5430) ? 1 : -1];
typedef char EnemyCoreWorldAt2DD4[
    (offsetof(EnemyView, worldPosition2DD4) == 0x2DD4) ? 1 : -1];
typedef char EnemyCoreFlagsAt337C[
    (offsetof(EnemyView, primaryFlags337C) == 0x337C) ? 1 : -1];
typedef char EnemyCoreTrailAt33E8[
    (offsetof(EnemyView, trailSamples33E8) == 0x33E8) ? 1 : -1];
typedef char EnemyCoreTrailVerticesAt3E68[
    (offsetof(EnemyView, trailVertices3E68) == 0x3E68) ? 1 : -1];
typedef char EnemyCoreTrailHistoryEndsAtVertices[
    (offsetof(EnemyView, trailSamples33E8) +
     sizeof(((EnemyView *)0)->trailSamples33E8) ==
     offsetof(EnemyView, trailVertices3E68)) ? 1 : -1];
typedef char EnemyCoreTrailVerticesEndAt53A0[
    (offsetof(EnemyView, trailVertices3E68) +
     sizeof(((EnemyView *)0)->trailVertices3E68) ==
     offsetof(EnemyView, trailFlags53A0)) ? 1 : -1];
typedef char EnemyCoreTrailFlagsAt53A0[
    (offsetof(EnemyView, trailFlags53A0) == 0x53A0) ? 1 : -1];
typedef char EnemyCoreAttachedEffectStateAt53B4[
    (offsetof(EnemyView, attachedEffectState53B4) == 0x53B4) ? 1 : -1];
typedef char EnemyCorePhaseEndSecondsAt5420[
    (offsetof(EnemyView, phaseEndSeconds5420) == 0x5420) ? 1 : -1];
typedef char EnemyCoreDamageTimerAt53A8[
    (offsetof(EnemyView, damageReductionTimer53A8) == 0x53A8) ? 1 : -1];


typedef char EnemyManagerCoreSizeIs2AC450[
    (sizeof(EnemyManagerCoreView) == 0x2AC450) ? 1 : -1];
typedef char EnemyManagerCoreEnemiesAt5758[
    (offsetof(EnemyManagerCoreView, enemies5758) == 0x5758) ? 1 : -1];
typedef char EnemyManagerCoreBossSlotsAt2AC388[
    (offsetof(EnemyManagerCoreView, remoteSlots2AC388) == 0x2AC388 &&
     sizeof(((EnemyManagerCoreView *)0)->remoteSlots2AC388) == 0x24 &&
     offsetof(EnemyManagerCoreView, activeEnemyCount2AC3AC) ==
         offsetof(EnemyManagerCoreView, remoteSlots2AC388) +
         sizeof(((EnemyManagerCoreView *)0)->remoteSlots2AC388)) ? 1 : -1];
typedef char EnemyManagerCoreDrawHeadsAt2AC410[
    (offsetof(EnemyManagerCoreView, drawGroupHeads2AC410) == 0x2AC410) ? 1 : -1];
typedef char EnemyManagerCoreLateProtocolAt2AC3DC[
    (offsetof(EnemyManagerCoreView, protocolValue2AC3DC) == 0x2AC3DC &&
     offsetof(EnemyManagerCoreView, scheduleRuntime2AC3E0) == 0x2AC3E0) ? 1 : -1];
typedef char EnemyManagerCoreTimelineAt2AC440[
    (offsetof(EnemyManagerCoreView, timelineSpawnsSuppressed2AC440) == 0x2AC440 &&
     offsetof(EnemyManagerCoreView, priorityEnemy2AC444) == 0x2AC444) ? 1 : -1];

extern AsciiManager g_AsciiManager;
extern unsigned char g_EnemyCoreSchedule[];

// Reconstruction aliases for the shared physical leaf at 0x0044AB20.
// Its complete body is xor eax,eax;ret, not an unreconstructed callback stub.
// The ignored receiver and original source ownership remain unresolved.
int EnemyCoreRunLifeCallback(EnemyView *enemy)
{
    return 0;
}

int EnemyCoreRunTimerCallback(EnemyView *enemy)
{
    return 0;
}
// Use the actual Th09EclRunState release API declared by EclRunState.inl.
static void EnemyCoreUpdateAttachedEffects(EnemyView *enemy);

// This exact Enemy effect updater and the Effect ring/fan callbacks touch
// the same physical per-effect origin, radius and phase. Keep one shared
// bounded runtime view, while leaving the unknown original C++ type open.
#include "EffectRingRuntimeView.hpp"
typedef char EnemyCoreAttachedEffectNativeOffsets[
    (offsetof(EffectRingRuntimeView, origin48) == 0x48 &&
     offsetof(EffectRingRuntimeView, radius7C) == 0x7C &&
     offsetof(EffectRingRuntimeView, angle80) == 0x80) ? 1 : -1];

#include "RuntimeAngleMath.hpp"
extern long double __stdcall AnmProjectionAbs(float value);

static void EnemyCoreUpdateAttachedEffects(EnemyView *nativeEnemy)
{
    // The exact effect updater shares the constructed Enemy with RunEcl,
    // EnemyManager::OnUpdate and native Enemy cleanup.
    for (int i = 0; i < nativeEnemy->attachedEffectState53B4.count60; ++i)
    {
        EffectRingRuntimeView *effect =
            reinterpret_cast<EffectRingRuntimeView *>(
                nativeEnemy->attachedEffectState53B4.effects00[i]);
        if (effect != 0)
        {
            effect->origin48 =
                reinterpret_cast<const Float3 &>(nativeEnemy->position2D74);
            if (effect->radius7C < nativeEnemy->attachedEffectState53B4.distance64)
                effect->radius7C += 0.3f;
            else
                effect->radius7C = nativeEnemy->attachedEffectState53B4.distance64;
            effect->angle80 =
                AddNormalizeAngle(effect->angle80, 0.031415928f);
        }
    }
}


// The 3,900-byte Enemy calc walks the same native 129-slot pool consumed by
// its exact callers and the ECL interpreter.  The separately named CoreView
// remains only for still-unresolved lexical ECL helper source/ABI.  The
// native EnemyFloat3 and old Float3 share three physical float words; the
// explicit Float3 views preserve original vector-operator calls and copies.
// The third hitbox component at +0x2DC4 is still opaque to the shared Enemy
// layout, but is accessed as a vector component in this original calc path.
typedef char EnemyCoreCalcNativePoolAndFields[
    (sizeof(EnemyView) == 0x5430 &&
     offsetof(EnemyManagerView, enemies5758) == 0x5758 &&
     offsetof(EnemyView, primaryAnmVm0008) == 0x08 &&
     offsetof(EnemyView, hitboxDimensions2DBC) == 0x2DBC &&
     offsetof(EnemyView, secondaryHitboxDimensions2DC8) == 0x2DC8 &&
     offsetof(EnemyView, worldPosition2DD4) == 0x2DD4 &&
     offsetof(EnemyView, deathEffectVariant3368) == 0x3368 &&
     offsetof(EnemyView, shootIntervalFrames30B4) == 0x30B4 &&
     offsetof(EnemyView, trailSamples33E8) == 0x33E8) ? 1 : -1];

int __fastcall EnemyManagerView::OnUpdate(EnemyManagerView *manager)
{
    int bombHit = 0;
    int damageOccurred;
    int trailIndex;
    int hitKind;
    int enemyIndex;

    if ((g_GameManager.flags & 0x1800) != 0)
        return 1;

    g_GameManager.frontFactoryA8->SetSideEnemyIndicatorPosition(
        manager->sideIndex31C, &Float3(-999.0f, 0.0f, 0.0f));
    g_Supervisor.SelectSide(manager->sideIndex31C);

    manager->drawGroupHeads2AC410[EnemyNativeTrailRenderCapacity::DrawGroupCount - 1] = 0;
    manager->drawGroupHeads2AC410[2] = 0;
    manager->drawGroupHeads2AC410[1] = 0;
    manager->drawGroupHeads2AC410[0] = 0;

    if (g_GameManager.frontFactoryA8->transitionVmCount1095C == 0)
    {
        if (manager->scheduledSubroutine2AC400 != 0)
        {
            manager->scheduleRuntime2AC3E0.Run();
        }
        else if (manager->normalEnemyCount2AC3B0 == 0 ||
                 (manager->sideTimer2AC3C8 > 0))
        {
            // Native TH09 schedule bytes are read from the real global
            // table by unsigned 32-bit address arithmetic. The second
            // original read still post-increments the live u8 schedule index.
            manager->scheduleHighBit2AC3F8 =
                (*reinterpret_cast<const unsigned char *>(
                    reinterpret_cast<unsigned int>(&g_EnemyCoreSchedule[0]) +
                    static_cast<unsigned int>(manager->scheduleIndex2AC3D4))) >> 7;
            manager->scheduledSubroutine2AC400 =
                manager->primaryEclManager000.GetSubroutine(
                    (*reinterpret_cast<const unsigned char *>(
                    reinterpret_cast<unsigned int>(&g_EnemyCoreSchedule[0]) +
                    static_cast<unsigned int>(manager->scheduleIndex2AC3D4++))) & 0x7F);
            manager->scheduleRuntime2AC3E0.timer00 = 0;
        }
    }

    manager->activeEnemyCount2AC3AC = 0;
    manager->normalEnemyCount2AC3B0 = 0;
    manager->priorityEnemy2AC444 = 0;
    manager->firstActiveEnemy2AC448 = 0;
    manager->specialEnemyCount2AC3B4 = 0;
    manager->rewardEnemyCount2AC3B8 = 0;

    EnemyView *enemy = manager->enemies5758;
    if (manager->sideState320->playerLifecycle04->focusEffect364 != NULL)
        manager->sideTimer2AC3C8 = 0;
    else
        manager->sideTimer2AC3C8++;

    // One of 129 native records is reserved as the full-pool sentinel.
    // Keep this loop's signed integer comparison: promoting enemyIndex to
    // size_t changes the original branch at the callback's loop tail.
    // Retail increments the signed index as a native DWORD and advances
    // the actual EnemyView allocation by its full record byte stride.
    // Keep the original signed comparison and 129th failure sentinel.
    for (enemyIndex = 0;
         enemyIndex < EnemyNativePoolCapacity::ActiveSlots;
         enemyIndex = static_cast<int>(
             static_cast<unsigned int>(enemyIndex) + 1u),
         enemy = reinterpret_cast<EnemyView *>(
             reinterpret_cast<unsigned int>(enemy) + sizeof(EnemyView)))
    {
        EnemyView **drawHead;
        unsigned int flags = enemy->primaryFlags337C;

        if ((flags & ENEMY_CORE_ACTIVE) == 0)
        {
            // The shared native Player owns the tracking record at +0x30364.
            if (manager->sideState320->playerLifecycle04->tracking30364.homingTarget18 == enemy)
                manager->sideState320->playerLifecycle04->tracking30364.homingTarget18 = NULL;
            continue;
        }

        if ((manager->sideState320->flags34 & 1) != 0)
        {
            const unsigned int sideCategory =
                (enemy->secondaryFlags3380 >> 10) & 3;
            if (sideCategory != 1 && sideCategory != 2)
            {
                if ((flags & ENEMY_CORE_NO_SPRITE) == 0)
                {
                    drawHead = reinterpret_cast<EnemyView **>(
                        reinterpret_cast<unsigned int>(manager->drawGroupHeads2AC410) +
                        static_cast<unsigned int>(enemy->drawGroup3387) *
                        static_cast<unsigned int>(sizeof(manager->drawGroupHeads2AC410[0])));
                    goto append_enemy_to_draw_group;
                }
                continue;
            }
        }

        {
            damageOccurred = 0;

            if ((flags & ENEMY_CORE_FORCE_DEATH) != 0)
            {
                reinterpret_cast<Float3 &>(enemy->worldPosition2DD4) =
                    reinterpret_cast<const Float3 &>(enemy->position2D74) +
                    reinterpret_cast<const Float3 &>(enemy->positionOffset2D80);
                enemy->worldPosition2DD4.z = 0.0f;
                goto process_enemy_death;
            }

            manager->activeEnemyCount2AC3AC = static_cast<int>(
                    static_cast<unsigned int>(manager->activeEnemyCount2AC3AC) + 1u);
            if ((enemy->secondaryFlags3380 & 0x4DC0) == 0)
                manager->normalEnemyCount2AC3B0 = static_cast<int>(
                    static_cast<unsigned int>(manager->normalEnemyCount2AC3B0) + 1u);
            if ((enemy->secondaryFlags3380 & 0x0C00) != 0)
                manager->specialEnemyCount2AC3B4 = static_cast<int>(
                    static_cast<unsigned int>(manager->specialEnemyCount2AC3B4) + 1u);
            if ((enemy->secondaryFlags3380 & 0x01C0) != 0)
                manager->rewardEnemyCount2AC3B8 = static_cast<int>(
                    static_cast<unsigned int>(manager->rewardEnemyCount2AC3B8) + 1u);

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
                    // The native Enemy position and Etama descriptor both
                    // use the same three-float vector storage.
                    descriptor.position =
                        reinterpret_cast<const Float3 &>(enemy->position2D74);
                    EnemySideStateView &spawnSide = *manager->sideState320;
                    descriptor.bulletType = 1;
                    descriptor.aimMode = 0;
                    descriptor.count1 =
                        static_cast<short>(spawnSide.characterIndex20 == 13 ? 1 : 3);
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
                    spawnSide.etama08->SpawnBulletPatternPrimary(
                        &descriptor);
                    enemy->primaryFlags337C &= ~ENEMY_CORE_ACTIVE;
                    enemy->CleanupAfterDeactivation();
                    continue;
                }
            }

            if (((enemy->primaryFlags337C & ENEMY_CORE_PAUSE_WITH_PLAYER) != 0 &&
                 manager->sideState320->playerLifecycle04->GetUpdateState() != 0) ||
                (enemy->secondaryFlags3380 & 0x10) != 0)
            {
                enemy->bossTimer2E64--;
                goto update_damage_flash;
            }

        run_enemy_ecl:
            if (manager->primaryEclManager000.RunEcl(
                    enemy) == -1)
            {
                enemy->primaryFlags337C &= ~ENEMY_CORE_ACTIVE;
                enemy->CleanupAfterDeactivation();
                continue;
            }

            if ((enemy->primaryFlags337C & ENEMY_CORE_SKIP_MOVEMENT) == 0)
            {
                enemy->ClampPositionToMovementBounds();
                enemy->IntegrateMotion();
                enemy->ClampPositionToMovementBounds();
                reinterpret_cast<Float3 &>(enemy->worldPosition2DD4) =
                    reinterpret_cast<const Float3 &>(enemy->position2D74) +
                    reinterpret_cast<const Float3 &>(enemy->positionOffset2D80);
                enemy->worldPosition2DD4.z = 0.0f;
            }
            else
            {
                reinterpret_cast<Float3 &>(enemy->worldPosition2DD4) =
                    reinterpret_cast<const Float3 &>(enemy->position2D74) +
                    reinterpret_cast<const Float3 &>(enemy->positionOffset2D80);
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
                    reinterpret_cast<const Float3 &>(enemy->worldPosition2DD4);
                enemy->trailSamples33E8[0].velocity0C =
                    reinterpret_cast<const Float3 &>(enemy->velocity2D8C);
                enemy->trailSamples33E8[0].angle18 = enemy->movementAngle2DE0;
            }

            AnmLoadedSprite *loadedSprite =
                enemy->primaryAnmVm0008.loadedSprite;
            if (loadedSprite == 0)
                enemy->primaryFlags337C |= ENEMY_CORE_NO_SPRITE;

            const Float3 *worldPosition;
            if ((enemy->primaryFlags337C & ENEMY_CORE_NO_SPRITE) == 0 &&
                (enemy->primaryFlags337C & ENEMY_CORE_HAS_BEEN_IN_BOUNDS) == 0 &&
                (worldPosition = reinterpret_cast<const Float3 *>(
                     reinterpret_cast<Float3 &>(
                         enemy->worldPosition2DD4).operator float *()),
                 g_GameManager.IsWithinPlayfield(
                    worldPosition->x,
                    worldPosition->y,
                    loadedSprite->widthPx,
                    loadedSprite->heightPx)))
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
                        enemy->primaryAnmVm0008.loadedSprite->widthPx,
                        enemy->primaryAnmVm0008.loadedSprite->heightPx)) ||
                    (enemy->trailFlags53A0 != 0 &&
                     !g_GameManager.IsWithinPlayfield(
                         enemy->worldPosition2DD4.x,
                         enemy->worldPosition2DD4.y,
                        enemy->primaryAnmVm0008.loadedSprite->widthPx,
                        enemy->primaryAnmVm0008.loadedSprite->heightPx) &&
                     !g_GameManager.IsWithinPlayfield(
                        enemy->trailSamples33E8[
                            enemy->trailHistoryLength53A2 - 1].position00.x,
                        enemy->trailSamples33E8[
                            enemy->trailHistoryLength53A2 - 1].position00.y,
                        enemy->primaryAnmVm0008.loadedSprite->widthPx,
                        enemy->primaryAnmVm0008.loadedSprite->heightPx)))
                {
                    enemy->primaryFlags337C &= ~ENEMY_CORE_ACTIVE;
                    enemy->CleanupAfterDeactivation();
                    continue;
                }
            }

            if (EnemyCoreRunLifeCallback(enemy))
                goto run_enemy_ecl;
            if (enemy->timerCallbackThreshold33D0 >= 0 &&
                EnemyCoreRunTimerCallback(enemy))
                goto run_enemy_ecl;

            enemy->primaryAnmVm0008.color1 = enemy->displayColor2E70;
            g_AnmManager->ExecuteScript(&enemy->primaryAnmVm0008);
            enemy->displayColor2E70 = enemy->primaryAnmVm0008.color1;
            for (int vmIndex = 0;
                 vmIndex < static_cast<int>(
                     EnemyNativeTrailRenderCapacity::SecondaryVmCount);
                 ++vmIndex)
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
                    enemy->AppendPlayerCollisionBox(
                            reinterpret_cast<const Float3 *>(
                                &enemy->worldPosition2DD4),
                            reinterpret_cast<const Float3 *>(
                                &enemy->hitboxDimensions2DBC));
                    if (enemy->trailFlags53A0 != 0)
                    {
                        Float3 trailHitbox = reinterpret_cast<const Float3 &>(
                            enemy->hitboxDimensions2DBC);
                        for (trailIndex = 1;
                             trailIndex < enemy->trailCollisionLength53A4;
                             trailIndex += 6)
                        {
                            if ((enemy->trailFlags53A0 & 2) != 0)
                            {
                                trailHitbox = reinterpret_cast<const Float3 &>(
                                    enemy->hitboxDimensions2DBC) -
                                    (reinterpret_cast<const Float3 &>(
                                         enemy->hitboxDimensions2DBC) *
                                     static_cast<float>(trailIndex)) /
                                    static_cast<float>(
                                        enemy->trailCollisionLength53A4);
                            }
                            enemy->AppendPlayerCollisionBox(
                                    &enemy->trailSamples33E8[
                                        trailIndex].position00,
                                    &trailHitbox);
                        }
                    }
                }

                enemy->lastDamage33AC = 0;
                if ((enemy->primaryFlags337C & ENEMY_CORE_ACCEPTS_DAMAGE) != 0)
                {
                    int damage = manager->sideState320->playerLifecycle04->CalcDamageToEnemy(
                        reinterpret_cast<PlayerDamageFloat3 *>(
                            &enemy->worldPosition2DD4),
                        reinterpret_cast<PlayerDamageFloat3 *>(
                            &enemy->hitboxDimensions2DBC),
                        &enemy->primaryDamageAccumulator2E5C,
                        &bombHit,
                        &enemy->secondaryDamageAccumulator2E60);
                    if (enemy->secondaryHitboxDimensions2DC8.x > 0.0f)
                    {
                        manager->sideState320->playerLifecycle04->CalcDamageToEnemy(
                            reinterpret_cast<PlayerDamageFloat3 *>(
                                &enemy->worldPosition2DD4),
                            reinterpret_cast<PlayerDamageFloat3 *>(
                                &enemy->secondaryHitboxDimensions2DC8),
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
                                int secondaryPart = static_cast<int>(
                                    static_cast<unsigned int>(damage) -
                                    static_cast<unsigned int>(enemy->primaryDamageAccumulator2E5C));
                                int primaryPart;
                                if ((enemy->secondaryFlags3380 & 0x1000) == 0)
                                    primaryPart =
                                        enemy->primaryDamageAccumulator2E5C / 4;
                                else
                                    primaryPart =
                                        enemy->primaryDamageAccumulator2E5C / 2;
                                damage = static_cast<int>(
                                    static_cast<unsigned int>(primaryPart) +
                                    static_cast<unsigned int>(secondaryPart) * 4u);
                            }

                            enemy->life2E48 = static_cast<int>(
                                static_cast<unsigned int>(enemy->life2E48) -
                                static_cast<unsigned int>(damage));
                            enemy->lastDamage33AC = damage;
                        }
                        damageOccurred = 1;
                    }

                    EnemyCoreSideStateView *side = manager->sideState320;
                    Float3 oldTargetDelta =
                        reinterpret_cast<const Float3 &>(
                            side->playerLifecycle04->tracking30364.trackedX00) -
                        reinterpret_cast<const Float3 &>(
                            side->playerLifecycle04->position1B88);
                    PlayerLifecycleView *player = side->playerLifecycle04;
                    Float3 newTargetDelta =
                        reinterpret_cast<const Float3 &>(enemy->worldPosition2DD4) -
                        reinterpret_cast<const Float3 &>(player->position1B88);
                    if (oldTargetDelta.x * oldTargetDelta.x +
                            oldTargetDelta.y * oldTargetDelta.y >
                        newTargetDelta.x * newTargetDelta.x +
                            newTargetDelta.y * newTargetDelta.y)
                    {
                        reinterpret_cast<Float3 &>(
                            player->tracking30364.trackedX00) =
                            reinterpret_cast<const Float3 &>(enemy->worldPosition2DD4);
                    }

                    player = manager->sideState320->playerLifecycle04;
                    if (static_cast<float>(AnmProjectionAbs(
                            enemy->worldPosition2DD4.x -
                            player->position1B88.x)) < 64.0f)
                    {
                        EnemyView *homing = player->tracking30364.homingTarget18;
                        if (homing == 0 ||
                            homing->position2D74.y > enemy->worldPosition2DD4.y)
                        {
                            player->tracking30364.homingTarget18 = enemy;
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
            // The retail signed quotient follows a native i386 DWORD
            // subtraction: perform that subtraction modulo 2^32 before
            // interpreting its result as a signed frame count.
            enemy->phaseEndSeconds5420 =
                static_cast<int>(
                    static_cast<unsigned int>(enemy->timerCallbackThreshold33D0) -
                    static_cast<unsigned int>(static_cast<int>(enemy->bossTimer2E64))) / 60;
            enemy->timerCallbackThreshold33D0 = -1;
            for (int callbackIndex = 0;
                 callbackIndex < static_cast<int>(
                     sizeof(enemy->lifeCallbackThresholds33B0) /
                     sizeof(enemy->lifeCallbackThresholds33B0[0]));
                 ++callbackIndex)
                enemy->lifeCallbackThresholds33B0[callbackIndex] = -1;
            for (int childIndex = 0;
                 childIndex < static_cast<int>(
                     sizeof(enemy->childEclBlocks33D8) / sizeof(enemy->childEclBlocks33D8[0]));
                 ++childIndex)
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
                if (enemy->deathEffectVariant3368 >= 0)
                {
                    manager->sideState320->effectManager0C->SpawnEffect(
                        enemy->deathEffectVariant3368 + 20,
                        reinterpret_cast<EffectFloat3 *>(
                            &enemy->worldPosition2DD4),
                        1,
                        static_cast<unsigned int>(-1));
                    manager->sideState320->effectManager0C->SpawnEffect(
                        enemy->deathEffectVariant3368 + 20,
                        reinterpret_cast<EffectFloat3 *>(
                            &enemy->worldPosition2DD4),
                        1,
                        static_cast<unsigned int>(-1));
                    manager->sideState320->effectManager0C->SpawnEffect(
                        enemy->deathEffectVariant3368 + 20,
                        reinterpret_cast<EffectFloat3 *>(
                            &enemy->worldPosition2DD4),
                        1,
                        static_cast<unsigned int>(-1));
                }
                PlayerLifecycleView *player = manager->sideState320->playerLifecycle04;
                if (player->GetUpdateState() == 0)
                {
                    player->timer303C8 = 90;
                    manager->sideState320->playerLifecycle04->SetUpdateState(3);
                }
                enemy->primaryFlags337C &= 0xE7FFFFFF;
                break;
            }

            case 1:
                reinterpret_cast<AsciiGameManagerSideView *>(
                    manager->sideState320)->AddScore(enemy->score2E54);
                enemy->primaryFlags337C =
                    (enemy->primaryFlags337C & 0xFFEFFFB3) |
                    ENEMY_CORE_PERSIST_AFTER_DEATH;
                goto release_death_effects;

            case 0:
                reinterpret_cast<AsciiGameManagerSideView *>(
                    manager->sideState320)->AddScore(enemy->score2E54);
                enemy->primaryFlags337C &= ~ENEMY_CORE_ACTIVE;
            release_death_effects:
                if ((enemy->primaryFlags337C & ENEMY_CORE_BOSS) != 0)
                    Th09EclRunState::ReleaseAttachedEffects(
                        enemy);
            case 2:
                enemy->life2E48 = 0;
                break;

            default:
                break;
            }

            if ((enemy->primaryFlags337C & ENEMY_CORE_FORCE_DEATH) == 0)
            {
                enemy->HandleDeathRewards(hitKind);
            }

            if (enemy->deathCallbackSubId2D2E >= 0)
            {
                enemy->ResetBulletRankInfluence();
                enemy->activeCallDepth2D2A = 0;
                for (int callbackIndex = 0;
                 callbackIndex < static_cast<int>(
                     sizeof(enemy->lifeCallbackThresholds33B0) /
                     sizeof(enemy->lifeCallbackThresholds33B0[0]));
                 ++callbackIndex)
                    enemy->lifeCallbackThresholds33B0[callbackIndex] = -1;
                enemy->timerCallbackThreshold33D0 = -1;
                EnemyCoreReleaseChildEclBlocks(
                    enemy);
                // The death callback resets exactly the native 0x214-byte
                // bullet descriptor from the manager's spawn template, not
                // the neighboring pending-shot/timer fields.
                EnemyView *nativeEnemy = enemy;
                EnemyManagerView *nativeManager = nativeEnemy->manager00;
                nativeEnemy->bulletDescriptor2E74 =
                    nativeManager->spawnTemplate328.bulletDescriptor2E74;
                enemy->shootIntervalFrames30B4 = 0;
                manager->primaryEclManager000.InitializeSubroutine(
                        &enemy->mainContext07F4,
                        enemy->deathCallbackSubId2D2E);
                enemy->deathCallbackSubId2D2E = -1;
            }

        update_damage_flash:
            if (enemy->damageFlashTimer336C != 0)
            {
                --enemy->damageFlashTimer336C;
                enemy->primaryAnmVm0008.flagsWord &= ~0x20000u;
            }
            else if (damageOccurred)
            {
                if ((enemy->secondaryFlags3380 & 6) < 4)
                    g_SoundPlayer.PlaySoundPositionedByIdx(20, enemy->worldPosition2DD4.x);
                else
                    g_SoundPlayer.PlaySoundPositionedByIdx(37, enemy->worldPosition2DD4.x);

                reinterpret_cast<unsigned char *>(&enemy->primaryAnmVm0008.color2)[2] = 0xFF;
                reinterpret_cast<unsigned char *>(&enemy->primaryAnmVm0008.color2)[1] = 0x60;
                reinterpret_cast<unsigned char *>(&enemy->primaryAnmVm0008.color2)[0] = 0x80;
                reinterpret_cast<unsigned char *>(&enemy->primaryAnmVm0008.color2)[3] =
                    static_cast<unsigned char>(
                        enemy->primaryAnmVm0008.color1 >> 24);
                enemy->primaryAnmVm0008.flagsWord |= 0x20000;
                enemy->damageFlashTimer336C = 1;
            }
            else
            {
                enemy->primaryAnmVm0008.flagsWord &= ~0x20000u;
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
                g_AsciiManager.SetBossMarkerPosition(enemy->bossSlot336B,
                                                   &markerPosition);
                int markerState = (enemy->secondaryFlags3380 >> 1) & 3;
                if (markerState == 0)
                    g_AsciiManager.SetBossMarkerState(enemy->bossSlot336B,
                        (enemy->primaryAnmVm0008.flagsWord >> 17) & 1);
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
                g_GameManager.frontFactoryA8->SetSideEnemyIndicatorPosition(
                    manager->sideIndex31C, reinterpret_cast<const Float3 *>(&enemy->position2D74));
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
            drawHead = reinterpret_cast<EnemyView **>(
                        reinterpret_cast<unsigned int>(manager->drawGroupHeads2AC410) +
                        static_cast<unsigned int>(enemy->drawGroup3387) *
                        static_cast<unsigned int>(sizeof(manager->drawGroupHeads2AC410[0])));
        }
        else
            continue;

    append_enemy_to_draw_group:
        EnemyView *nativeEnemy = enemy;
        nativeEnemy->nextInDrawGroup04 = *drawHead;
        *drawHead = nativeEnemy;
    }

    manager->frameTimer2AC404++;
    return 1;
}
