#include "AsciiManager.hpp"
#include "EnemyManager.hpp"
#include "Supervisor.hpp"
#include "ZunMemory.hpp"

#include <math.h>
#include <stddef.h>
#include <string.h>

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

struct EnemyCoreTimerView
{
    unsigned char storage00[0x0C];

    void Set(int value);
    void operator++(int value);
    void operator--(int value);
    int IsAfter(int value);
    int HasTicked(int interval);
    int Current();
};
typedef char EnemyCoreTimerSizeIs0C[
    (sizeof(EnemyCoreTimerView) == 0x0C) ? 1 : -1];

struct EnemyCoreLoadedSpriteView
{
    unsigned char unknown00[0x30];
    float extent30;
    float extent34;
};

struct EnemyCoreColorBytes
{
    unsigned char b;
    unsigned char g;
    unsigned char r;
    unsigned char a;
};

struct EnemyCoreAnmVmView
{
    unsigned char unknown000[0x1F0];
    unsigned int color1_1F0;
    EnemyCoreColorBytes color2_1F4;
    unsigned int flags1F8;
    short type1FC;
    short pendingInterrupt1FE;
    unsigned char unknown200[0x21A - 0x200];
    short scriptIndex21A;
    unsigned char unknown21C[0x224 - 0x21C];
    EnemyCoreLoadedSpriteView *loadedSprite224;
    unsigned char unknown228[0x2A4 - 0x228];
};
typedef char EnemyCoreAnmVmSizeIs2A4[
    (sizeof(EnemyCoreAnmVmView) == 0x2A4) ? 1 : -1];

struct EnemyCoreTrailSampleView
{
    Float3 position00;
    Float3 velocity0C;
    float angle18;
};
typedef char EnemyCoreTrailSampleSizeIs1C[
    (sizeof(EnemyCoreTrailSampleView) == 0x1C) ? 1 : -1];

struct EnemyCoreEtamaControllerView;
struct EnemyCorePlayerView;

struct EnemyCoreSideStateView
{
    unsigned char unknown00[0x04];
    EnemyCorePlayerView *player04;
    EnemyCoreEtamaControllerView *etama08;
    EffectManager *effectManager0C;
    EnemyManagerView *enemyManager10;
    unsigned char unknown14[0x0C];
    int characterIndex20;
    unsigned char unknown24[0x10];
    unsigned int flags34;

    void AddScore(int value);
};
typedef char EnemyCoreSideStateSizeIs38[
    (sizeof(EnemyCoreSideStateView) == 0x38) ? 1 : -1];

struct EnemyCorePlayerView
{
    unsigned char unknown00000[0x364];
    int state364;
    unsigned char unknown00368[0x1B88 - 0x368];
    Float3 position1B88;
    unsigned char unknown01B94[0x30364 - 0x1B94];
    Float3 trackedEnemyPosition30364;
    unsigned char unknown30370[0x3037C - 0x30370];
    void *homingTarget3037C;
    unsigned char unknown30380[0x303C8 - 0x30380];
    EnemyCoreTimerView stateTimer303C8;

    int GetState();
    void SetState(int state);
    int CalcDamageToEnemy(
        Float3 *position,
        Float3 *hitbox,
        int *primaryAccumulator,
        int *bombHit,
        int *secondaryAccumulator);
};
typedef char EnemyCorePlayerPositionAt1B88[
    (offsetof(EnemyCorePlayerView, position1B88) == 0x1B88) ? 1 : -1];
typedef char EnemyCorePlayerHomingAt3037C[
    (offsetof(EnemyCorePlayerView, homingTarget3037C) == 0x3037C) ? 1 : -1];

struct EnemyCoreBulletTransformRecordView
{
    unsigned char storage00[0x18];
};

struct EnemyCoreBulletSpawnDescriptorView
{
    short bulletType00;
    short color02;
    Float3 position04;
    float angle10;
    float angleStep14;
    float speed118;
    float speed21C;
    EnemyCoreBulletTransformRecordView transforms20[18];
    float laserStartOffset1D0;
    float laserEndOffset1D4;
    float laserStartLength1D8;
    float laserWidth1DC;
    int laserStartTime1E0;
    int laserDuration1E4;
    int laserDespawnDuration1E8;
    int laserHitboxStartTime1EC;
    int laserHitboxEndDelay1F0;
    short count1_1F4;
    short count2_1F6;
    unsigned short aimMode1F8;
    unsigned short unknown1FA;
    unsigned int transformFlags1FC;
    int spawnSound200;
    int transformSound204;
    int transformStartIndex208;
    unsigned char extraAttribute20C;
    unsigned char unknown20D[3];
    void *templateSprites210;

    EnemyCoreBulletSpawnDescriptorView();
};
typedef char EnemyCoreBulletSpawnDescriptorSizeIs214[
    (sizeof(EnemyCoreBulletSpawnDescriptorView) == 0x214) ? 1 : -1];

struct EnemyCoreEtamaControllerView
{
    void *SpawnBulletPatternPrimary(
        EnemyCoreBulletSpawnDescriptorView *descriptor);
};

struct EnemyCoreEffectView
{
    unsigned char unknown000[0x2A4];

    void SetInterrupt(int interrupt);
};

struct EnemyCoreView
{
    EnemyManagerView *manager00;
    EnemyCoreView *nextInDrawGroup04;
    EnemyCoreAnmVmView primaryVm008;
    EnemyCoreAnmVmView secondaryVms2AC[2];
    unsigned char eclStorage7F4[0x2D2A - 0x7F4];
    short eclCallStackDepth2D2A;
    unsigned char unknown2D2C[0x02];
    short deathCallbackSubId2D2E;
    unsigned char unknown2D30[0x2D74 - 0x2D30];
    Float3 position2D74;
    Float3 positionOffset2D80;
    Float3 velocity2D8C;
    unsigned char unknown2D98[0x2DA4 - 0x2D98];
    Float3 previousPosition2DA4;
    Float3 displacement2DB0;
    Float3 hitbox2DBC;
    Float3 secondaryHitbox2DC8;
    Float3 worldPosition2DD4;
    float movementAngle2DE0;
    unsigned char unknown2DE4[0x2E48 - 0x2DE4];
    int life2E48;
    int maxLife2E4C;
    int phase2E50;
    int score2E54;
    int sequenceIndex2E58;
    int primaryDamageAccumulator2E5C;
    int secondaryDamageAccumulator2E60;
    EnemyCoreTimerView bossTimer2E64;
    unsigned int displayColor2E70;
    unsigned char bulletState2E74[0x30B4 - 0x2E74];
    int shootInterval30B4;
    unsigned char unknown30B8[0x335C - 0x30B8];
    unsigned int defeatTokenType335C;
    unsigned char unknown3360[0x08];
    signed char deathEffectVariant3368;
    unsigned char unknown3369[0x02];
    unsigned char bossSlot336B;
    unsigned char damageFlashTimer336C;
    unsigned char unknown336D[0x337C - 0x336D];
    unsigned int flags337C;
    unsigned int flags3380;
    unsigned char unknown3384[0x03];
    unsigned char drawGroup3387;
    unsigned char unknown3388[0x33AC - 0x3388];
    int lastDamage33AC;
    int lifeCallbackThresholds33B0[4];
    int lifeCallbackSubIds33C0[4];
    int timerCallbackThreshold33D0;
    int timerCallbackSubId33D4;
    void *childEclBlocks33D8[4];
    EnemyCoreTrailSampleView trailSamples33E8[290];
    unsigned char trailFlags53A0;
    unsigned char unknown53A1;
    short trailHistoryLength53A2;
    short trailCollisionLength53A4;
    short trailSampleStride53A6;
    EnemyCoreTimerView damageReductionTimer53A8;
    EnemyCoreEffectView *attachedEffects53B4[24];
    int attachedEffectCount5414;
    float attachedEffectDistance5418;
    EnemyCoreEffectView *alignmentEffect541C;
    int phaseEndSeconds5420;
    EnemyCoreTimerView specialAttackTimer5424;
};
typedef char EnemyCoreViewSizeIs5430[
    (sizeof(EnemyCoreView) == 0x5430) ? 1 : -1];
typedef char EnemyCoreWorldAt2DD4[
    (offsetof(EnemyCoreView, worldPosition2DD4) == 0x2DD4) ? 1 : -1];
typedef char EnemyCoreFlagsAt337C[
    (offsetof(EnemyCoreView, flags337C) == 0x337C) ? 1 : -1];
typedef char EnemyCoreTrailAt33E8[
    (offsetof(EnemyCoreView, trailSamples33E8) == 0x33E8) ? 1 : -1];
typedef char EnemyCoreDamageTimerAt53A8[
    (offsetof(EnemyCoreView, damageReductionTimer53A8) == 0x53A8) ? 1 : -1];

struct EnemyManagerCoreView
{
    unsigned char unknown000000[0x31C];
    int sideIndex31C;
    EnemyCoreSideStateView *sideState320;
    EnemyCoreSideStateView *opposingSideState324;
    EnemyCoreView spawnTemplate328;
    EnemyCoreView enemies5758[128];
    unsigned char unknown2A6F58[0x2AC3AC - 0x2A6F58];
    int activeEnemyCount2AC3AC;
    int normalEnemyCount2AC3B0;
    int specialEnemyCount2AC3B4;
    int rewardEnemyCount2AC3B8;
    unsigned char unknown2AC3BC[0x2AC3C8 - 0x2AC3BC];
    EnemyCoreTimerView sideTimer2AC3C8;
    unsigned char scheduleIndex2AC3D4;
    unsigned char unknown2AC3D5[0x2AC3E0 - 0x2AC3D5];
    EnemyScheduleRuntimeView scheduleRuntime2AC3E0;
    int scheduleHighBit2AC3F8;
    unsigned char unknown2AC3FC[0x04];
    void *scheduleObject2AC400;
    EnemyCoreTimerView frameTimer2AC404;
    EnemyCoreView *drawGroupHeads2AC410[4];
    unsigned char unknown2AC420[0x24];
    EnemyCoreView *priorityEnemy2AC444;
    EnemyCoreView *firstActiveEnemy2AC448;
    int specialAttackThreshold2AC44C;
};
typedef char EnemyManagerCoreSizeIs2AC450[
    (sizeof(EnemyManagerCoreView) == 0x2AC450) ? 1 : -1];
typedef char EnemyManagerCoreEnemiesAt5758[
    (offsetof(EnemyManagerCoreView, enemies5758) == 0x5758) ? 1 : -1];
typedef char EnemyManagerCoreDrawHeadsAt2AC410[
    (offsetof(EnemyManagerCoreView, drawGroupHeads2AC410) == 0x2AC410) ? 1 : -1];

struct EnemyCoreAnmManagerView
{
    int ExecuteScript(EnemyCoreAnmVmView *vm);
};

struct EnemyCoreEclManagerView
{
    int RunEcl(EnemyCoreView *enemy);
    void CallSub(void *eclContext, short subId);
};

struct EnemyCoreUiView
{
    void SetBossMarkerPosition(int slot, Float3 *position);
    void SetBossMarkerState(int slot, int state);
};

struct EnemyCoreFrontView
{
    void SetSideEnemyIndicatorPosition(
        int sideIndex, Float3 *position);
};

extern EnemyCoreAnmManagerView *g_EnemyCoreAnmManager;
extern EnemyCoreEclManagerView g_EnemyCoreEclManager;
extern EnemyCoreUiView g_EnemyCoreUi;
extern EnemyCoreFrontView *g_EnemyCoreFront;
extern unsigned int g_EnemyCoreRuntimeFlags;
extern int g_EnemyCoreScriptedUpdateFreeze;
extern int g_EnemyCoreDifficultyValue;
extern unsigned char g_EnemyCoreSchedule[];

extern void EnemyCoreDespawn(EnemyCoreView *enemy);
extern void EnemyCoreClampPosition(EnemyCoreView *enemy);
extern void EnemyCoreIntegrateVelocity(EnemyCoreView *enemy);
extern int EnemyCoreRunLifeCallback(EnemyCoreView *enemy);
extern int EnemyCoreRunTimerCallback(EnemyCoreView *enemy);
struct EnemyCoreGameManagerPlayfieldView
{
    int IsWithinPlayfield(float x, float y, float width, float height);
};

extern EnemyCoreGameManagerPlayfieldView g_EnemyCoreGameManager;
struct PlayerPositionView;

struct EnemyAppendCollisionView
{
    EnemyManagerView *manager00;

    void AppendPlayerCollisionBox(
        const PlayerPositionView *position,
        const PlayerPositionView *size);
};
extern void EnemyCoreResetBulletInfluence(EnemyCoreView *enemy);
extern void EnemyCoreReleaseChildEclBlocks(EnemyCoreView *enemy);
extern void EnemyCoreReleaseAttachedEffects(EnemyCoreView *enemy);
static void EnemyCoreUpdateAttachedEffects(EnemyCoreView *enemy);
struct EnemyCoreSoundPlayerView
{
    void PlaySoundPositionedByIdx(int soundIndex, float positionX);
};

extern EnemyCoreSoundPlayerView g_EnemyCoreSoundPlayer;

struct EnemyAttachedEffectUpdateView
{
    unsigned char unknown000[0x48];
    Float3 position48;
    unsigned char unknown054[0x7C - 0x54];
    float distance7C;
    float angle80;
};

extern float __stdcall AddNormalizeAngle(float angle, float delta);
extern long double __stdcall AnmProjectionAbs(float value);

static void EnemyCoreUpdateAttachedEffects(EnemyCoreView *enemy)
{
    for (int i = 0; i < enemy->attachedEffectCount5414; ++i)
    {
        EnemyAttachedEffectUpdateView *effect =
            reinterpret_cast<EnemyAttachedEffectUpdateView *>(
                enemy->attachedEffects53B4[i]);
        if (effect != 0)
        {
            effect->position48 = enemy->position2D74;
            if (effect->distance7C < enemy->attachedEffectDistance5418)
                effect->distance7C += 0.3f;
            else
                effect->distance7C = enemy->attachedEffectDistance5418;
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

    if ((g_EnemyCoreRuntimeFlags & 0x1800) != 0)
        return 1;

    Float3 markerPosition(-999.0f, 0.0f, 0.0f);
    g_EnemyCoreFront->SetSideEnemyIndicatorPosition(
        manager->sideIndex31C, &markerPosition);
    g_Supervisor.SelectSide(manager->sideIndex31C);

    manager->drawGroupHeads2AC410[3] = 0;
    manager->drawGroupHeads2AC410[2] = 0;
    manager->drawGroupHeads2AC410[1] = 0;
    manager->drawGroupHeads2AC410[0] = 0;

    if (!g_EnemyCoreScriptedUpdateFreeze)
    {
        if (manager->scheduleObject2AC400 != 0)
        {
            manager->scheduleRuntime2AC3E0.Run();
        }
        else if (manager->normalEnemyCount2AC3B0 == 0 ||
                 manager->sideTimer2AC3C8.IsAfter(0))
        {
            unsigned char schedule =
                g_EnemyCoreSchedule[manager->scheduleIndex2AC3D4++];
            manager->scheduleHighBit2AC3F8 = schedule >> 7;
            manager->scheduleObject2AC400 =
                enemyManager->primaryEclManager000.GetSubroutine(
                    schedule & 0x7F);
            manager->scheduleRuntime2AC3E0.timer00 = 0;
        }
    }

    manager->activeEnemyCount2AC3AC = 0;
    manager->normalEnemyCount2AC3B0 = 0;
    manager->specialEnemyCount2AC3B4 = 0;
    manager->rewardEnemyCount2AC3B8 = 0;
    manager->priorityEnemy2AC444 = 0;
    manager->firstActiveEnemy2AC448 = 0;

    EnemyCoreSideStateView *side = manager->sideState320;
    EnemyCorePlayerView *player = side->player04;
    if (player->state364 != 0)
        manager->sideTimer2AC3C8.Set(0);
    else
        manager->sideTimer2AC3C8++;

    for (int enemyIndex = 0; enemyIndex < 128; ++enemyIndex)
    {
        EnemyCoreView *enemy = &manager->enemies5758[enemyIndex];
        unsigned int flags = enemy->flags337C;

        if ((flags & ENEMY_CORE_ACTIVE) == 0)
        {
            if (player->homingTarget3037C == enemy)
                player->homingTarget3037C = 0;
            continue;
        }

        unsigned int sideCategory = (enemy->flags3380 >> 10) & 3;
        if ((side->flags34 & 1) != 0 && sideCategory != 1 &&
            sideCategory != 2)
        {
            if ((flags & ENEMY_CORE_NO_SPRITE) == 0)
            {
                EnemyCoreView **head =
                    &manager->drawGroupHeads2AC410[enemy->drawGroup3387];
                enemy->nextInDrawGroup04 = *head;
                *head = enemy;
            }
            continue;
        }

        int damageOccurred = 0;
        int hitKind = 0;

        if ((flags & ENEMY_CORE_FORCE_DEATH) != 0)
        {
            enemy->worldPosition2DD4 =
                enemy->position2D74 + enemy->positionOffset2D80;
            enemy->worldPosition2DD4.z = 0.0f;
            goto process_enemy_death;
        }

        ++manager->activeEnemyCount2AC3AC;
        if ((enemy->flags3380 & 0x4DC0) == 0)
            ++manager->normalEnemyCount2AC3B0;
        if ((enemy->flags3380 & 0x0C00) != 0)
            ++manager->specialEnemyCount2AC3B4;
        if ((enemy->flags3380 & 0x01C0) != 0)
            ++manager->rewardEnemyCount2AC3B8;

        if ((enemy->flags3380 & 0x1000) != 0)
        {
            if (enemy->specialAttackTimer5424.HasTicked(2))
            {
                side->effectManager0C->SpawnEffect(
                    9,
                    reinterpret_cast<EffectFloat3 *>(&enemy->position2D74),
                    1,
                    static_cast<unsigned int>(-1));
            }

            if (enemy->specialAttackTimer5424.IsAfter(
                    manager->specialAttackThreshold2AC44C))
            {
                EnemyCoreBulletSpawnDescriptorView descriptor;
                descriptor.bulletType00 = 1;
                descriptor.color02 = 2;
                descriptor.position04 = enemy->position2D74;
                descriptor.angle10 = 0.0f;
                descriptor.angleStep14 = 0.15707964f;
                descriptor.speed118 =
                    static_cast<float>(g_EnemyCoreDifficultyValue) * 0.1f +
                    1.0f;
                descriptor.speed21C = 0.0f;
                descriptor.count1_1F4 =
                    static_cast<short>(side->characterIndex20 == 13 ? 1 : 3);
                descriptor.count2_1F6 = 1;
                descriptor.aimMode1F8 = 0;
                descriptor.unknown1FA = 0;
                descriptor.transformFlags1FC = 4;
                side->etama08->SpawnBulletPatternPrimary(&descriptor);
                enemy->flags337C &= ~ENEMY_CORE_ACTIVE;
                EnemyCoreDespawn(enemy);
                continue;
            }
        }

        if (((enemy->flags337C & ENEMY_CORE_PAUSE_WITH_PLAYER) != 0 &&
             player->GetState() != 0) ||
            (enemy->flags3380 & 0x10) != 0)
        {
            enemy->bossTimer2E64--;
            goto update_damage_flash;
        }

    run_enemy_ecl:
        if (g_EnemyCoreEclManager.RunEcl(enemy) == -1)
        {
            enemy->flags337C &= ~ENEMY_CORE_ACTIVE;
            EnemyCoreDespawn(enemy);
            continue;
        }

        if ((enemy->flags337C & ENEMY_CORE_SKIP_MOVEMENT) == 0)
        {
            EnemyCoreClampPosition(enemy);
            EnemyCoreIntegrateVelocity(enemy);
            EnemyCoreClampPosition(enemy);
            enemy->worldPosition2DD4 =
                enemy->position2D74 + enemy->positionOffset2D80;
        }
        else
        {
            enemy->worldPosition2DD4 =
                enemy->position2D74 + enemy->positionOffset2D80;
        }
        enemy->worldPosition2DD4.z = 0.0f;

        if (enemy->trailFlags53A0 != 0)
        {
            for (int trailIndex = enemy->trailHistoryLength53A2 - 1;
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
            enemy->primaryVm008.loadedSprite224;
        if (loadedSprite == 0)
            enemy->flags337C |= ENEMY_CORE_NO_SPRITE;

        float *worldPosition = enemy->worldPosition2DD4;
        if ((enemy->flags337C & ENEMY_CORE_NO_SPRITE) == 0 &&
            (enemy->flags337C & ENEMY_CORE_HAS_BEEN_IN_BOUNDS) == 0 &&
            g_EnemyCoreGameManager.IsWithinPlayfield(
                worldPosition[0],
                worldPosition[1],
                loadedSprite->extent34,
                loadedSprite->extent30))
        {
            enemy->flags337C |= ENEMY_CORE_HAS_BEEN_IN_BOUNDS;
        }
        else if ((enemy->flags337C & ENEMY_CORE_HAS_BEEN_IN_BOUNDS) != 0 &&
                 (enemy->flags337C & ENEMY_CORE_ALLOW_OFFSCREEN) == 0)
        {
            EnemyCoreTrailSampleView *tail =
                &enemy->trailSamples33E8[
                    enemy->trailHistoryLength53A2 - 1];
            if ((enemy->trailFlags53A0 == 0 &&
                 !g_EnemyCoreGameManager.IsWithinPlayfield(
                     enemy->worldPosition2DD4.x,
                     enemy->worldPosition2DD4.y,
                     loadedSprite->extent34,
                     loadedSprite->extent30)) ||
                (enemy->trailFlags53A0 != 0 &&
                 !g_EnemyCoreGameManager.IsWithinPlayfield(
                     enemy->worldPosition2DD4.x,
                     enemy->worldPosition2DD4.y,
                     loadedSprite->extent34,
                     loadedSprite->extent30) &&
                 !g_EnemyCoreGameManager.IsWithinPlayfield(
                     tail->position00.x,
                     tail->position00.y,
                     loadedSprite->extent34,
                     loadedSprite->extent30)))
            {
                enemy->flags337C &= ~ENEMY_CORE_ACTIVE;
                EnemyCoreDespawn(enemy);
                continue;
            }
        }

        if (EnemyCoreRunLifeCallback(enemy))
            goto run_enemy_ecl;
        if (enemy->timerCallbackThreshold33D0 >= 0 &&
            EnemyCoreRunTimerCallback(enemy))
            goto run_enemy_ecl;

        enemy->primaryVm008.color1_1F0 = enemy->displayColor2E70;
        g_EnemyCoreAnmManager->ExecuteScript(&enemy->primaryVm008);
        enemy->displayColor2E70 = enemy->primaryVm008.color1_1F0;
        for (int vmIndex = 0; vmIndex < 2; ++vmIndex)
        {
            if (enemy->secondaryVms2AC[vmIndex].scriptIndex21A >= 0 &&
                g_EnemyCoreAnmManager->ExecuteScript(
                    &enemy->secondaryVms2AC[vmIndex]))
            {
                enemy->secondaryVms2AC[vmIndex].scriptIndex21A = -1;
            }
        }

        if ((enemy->flags337C &
             (ENEMY_CORE_NO_SPRITE | ENEMY_CORE_HIDE_PRIMARY)) == 0)
        {
            if ((enemy->flags337C & ENEMY_CORE_COLLISION) != 0 &&
                ((enemy->flags3380 & 0x01C0) == 0 ||
                 side->characterIndex20 != 11))
            {
                reinterpret_cast<EnemyAppendCollisionView *>(enemy)
                    ->AppendPlayerCollisionBox(
                        reinterpret_cast<const PlayerPositionView *>(
                            &enemy->worldPosition2DD4),
                        reinterpret_cast<const PlayerPositionView *>(
                            &enemy->hitbox2DBC));
                if (enemy->trailFlags53A0 != 0)
                {
                    Float3 trailHitbox = enemy->hitbox2DBC;
                    for (int trailIndex = 1;
                         trailIndex < enemy->trailCollisionLength53A4;
                         trailIndex += 6)
                    {
                        if ((enemy->trailFlags53A0 & 2) != 0)
                        {
                            Float3 reduction =
                                (enemy->hitbox2DBC *
                                 static_cast<float>(trailIndex)) /
                                static_cast<float>(
                                    enemy->trailCollisionLength53A4);
                            trailHitbox = enemy->hitbox2DBC - reduction;
                        }
                        reinterpret_cast<EnemyAppendCollisionView *>(enemy)
                            ->AppendPlayerCollisionBox(
                                reinterpret_cast<const PlayerPositionView *>(
                                    &enemy->trailSamples33E8[
                                        trailIndex].position00),
                                reinterpret_cast<const PlayerPositionView *>(
                                    &trailHitbox));
                    }
                }
            }

            enemy->lastDamage33AC = 0;
            if ((enemy->flags337C & ENEMY_CORE_ACCEPTS_DAMAGE) != 0)
            {
                int damage = player->CalcDamageToEnemy(
                    &enemy->worldPosition2DD4,
                    &enemy->hitbox2DBC,
                    &enemy->primaryDamageAccumulator2E5C,
                    &bombHit,
                    &enemy->secondaryDamageAccumulator2E60);
                if (enemy->secondaryHitbox2DC8.x > 0.0f)
                {
                    player->CalcDamageToEnemy(
                        &enemy->worldPosition2DD4,
                        &enemy->secondaryHitbox2DC8,
                        &enemy->primaryDamageAccumulator2E5C,
                        &bombHit,
                        &enemy->secondaryDamageAccumulator2E60);
                }

                if (damage > 0)
                {
                    if ((enemy->flags337C & ENEMY_CORE_DAMAGEABLE) != 0)
                    {
                        if (enemy->damageReductionTimer53A8.IsAfter(0))
                        {
                            if ((enemy->flags337C & ENEMY_CORE_BOSS) != 0)
                                damage /= 9;
                            else
                                damage = 0;
                        }

                        if (damage == enemy->primaryDamageAccumulator2E5C)
                            hitKind = 1;
                        if (damage == enemy->secondaryDamageAccumulator2E60)
                            hitKind = 2;

                        if ((enemy->flags3380 & 0x01C0) != 0)
                        {
                            int secondaryPart =
                                damage - enemy->primaryDamageAccumulator2E5C;
                            int primaryPart;
                            if ((enemy->flags3380 & 0x1000) != 0)
                                primaryPart =
                                    enemy->primaryDamageAccumulator2E5C / 2;
                            else
                                primaryPart =
                                    enemy->primaryDamageAccumulator2E5C / 4;
                            damage = primaryPart + secondaryPart * 4;
                        }

                        enemy->life2E48 -= damage;
                        enemy->lastDamage33AC = damage;
                    }
                    damageOccurred = 1;
                }

                Float3 oldTargetDelta =
                    player->trackedEnemyPosition30364 - player->position1B88;
                Float3 newTargetDelta =
                    enemy->worldPosition2DD4 - player->position1B88;
                if (newTargetDelta.x * newTargetDelta.x +
                        newTargetDelta.y * newTargetDelta.y <
                    oldTargetDelta.x * oldTargetDelta.x +
                        oldTargetDelta.y * oldTargetDelta.y)
                {
                    player->trackedEnemyPosition30364 =
                        enemy->worldPosition2DD4;
                }

                if (AnmProjectionAbs(
                        enemy->worldPosition2DD4.x -
                        player->position1B88.x) < 64.0f)
                {
                    EnemyCoreView *homing =
                        reinterpret_cast<EnemyCoreView *>(
                            player->homingTarget3037C);
                    if (homing == 0 ||
                        homing->position2D74.y > enemy->worldPosition2DD4.y)
                    {
                        player->homingTarget3037C = enemy;
                    }
                }
            }
        }

        if ((enemy->flags3380 & 1) != 0 && enemy->life2E48 > 0)
            enemy->flags3380 &= ~1u;
        if (enemy->life2E48 > 0 || (enemy->flags3380 & 9) != 0)
            goto update_damage_flash;

    process_enemy_death:
        enemy->flags3380 |= 1;
        enemy->phaseEndSeconds5420 =
            (enemy->timerCallbackThreshold33D0 -
             enemy->bossTimer2E64.Current()) / 60;
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

        switch ((enemy->flags337C >> 17) & 7)
        {
        case 0:
            side->AddScore(enemy->score2E54);
            enemy->flags337C &= ~ENEMY_CORE_ACTIVE;
            if ((enemy->flags337C & ENEMY_CORE_BOSS) != 0)
                EnemyCoreReleaseAttachedEffects(enemy);
            enemy->life2E48 = 0;
            break;

        case 1:
            side->AddScore(enemy->score2E54);
            enemy->flags337C =
                (enemy->flags337C & 0xFFEFFFB3) |
                ENEMY_CORE_PERSIST_AFTER_DEATH;
            if ((enemy->flags337C & ENEMY_CORE_BOSS) != 0)
                EnemyCoreReleaseAttachedEffects(enemy);
            enemy->life2E48 = 0;
            break;

        case 2:
            enemy->life2E48 = 0;
            break;

        case 3:
            enemy->flags337C &= 0xFFF1FFF7;
            enemy->life2E48 = 1;
            if (enemy->deathEffectVariant3368 >= 0)
            {
                side->effectManager0C->SpawnEffect(
                    enemy->deathEffectVariant3368 + 20,
                    reinterpret_cast<EffectFloat3 *>(
                        &enemy->worldPosition2DD4),
                    1,
                    static_cast<unsigned int>(-1));
                side->effectManager0C->SpawnEffect(
                    enemy->deathEffectVariant3368 + 20,
                    reinterpret_cast<EffectFloat3 *>(
                        &enemy->worldPosition2DD4),
                    1,
                    static_cast<unsigned int>(-1));
                side->effectManager0C->SpawnEffect(
                    enemy->deathEffectVariant3368 + 20,
                    reinterpret_cast<EffectFloat3 *>(
                        &enemy->worldPosition2DD4),
                    1,
                    static_cast<unsigned int>(-1));
            }
            if (player->GetState() == 0)
            {
                player->stateTimer303C8.Set(90);
                player->SetState(3);
            }
            enemy->flags337C &= 0xE7FFFFFF;
            break;

        default:
            break;
        }

        if ((enemy->flags337C & ENEMY_CORE_FORCE_DEATH) == 0)
        {
            reinterpret_cast<EnemyView *>(enemy)->HandleDeathRewards(hitKind);
        }

        if (enemy->deathCallbackSubId2D2E >= 0)
        {
            short callbackSubId = enemy->deathCallbackSubId2D2E;
            EnemyCoreResetBulletInfluence(enemy);
            enemy->eclCallStackDepth2D2A = 0;
            for (int callbackIndex = 0; callbackIndex < 4; ++callbackIndex)
                enemy->lifeCallbackThresholds33B0[callbackIndex] = -1;
            enemy->timerCallbackThreshold33D0 = -1;
            EnemyCoreReleaseChildEclBlocks(enemy);
            EnemyManagerCoreView *owner =
                reinterpret_cast<EnemyManagerCoreView *>(enemy->manager00);
            memcpy(enemy->bulletState2E74,
                   owner->spawnTemplate328.bulletState2E74,
                   0x214);
            enemy->shootInterval30B4 = 0;
            g_EnemyCoreEclManager.CallSub(enemy->eclStorage7F4,
                                         callbackSubId);
            enemy->deathCallbackSubId2D2E = -1;
        }

    update_damage_flash:
        if (enemy->damageFlashTimer336C != 0)
        {
            --enemy->damageFlashTimer336C;
            enemy->primaryVm008.flags1F8 &= ~0x20000u;
        }
        else if (damageOccurred)
        {
            if ((enemy->flags3380 & 6) >= 4)
                g_EnemyCoreSoundPlayer.PlaySoundPositionedByIdx(37, enemy->worldPosition2DD4.x);
            else
                g_EnemyCoreSoundPlayer.PlaySoundPositionedByIdx(20, enemy->worldPosition2DD4.x);

            enemy->primaryVm008.color2_1F4.r = 0xFF;
            enemy->primaryVm008.color2_1F4.g = 0x60;
            enemy->primaryVm008.color2_1F4.b = 0x80;
            enemy->primaryVm008.color2_1F4.a =
                static_cast<unsigned char>(
                    enemy->primaryVm008.color1_1F0 >> 24);
            enemy->primaryVm008.flags1F8 |= 0x20000;
            enemy->damageFlashTimer336C = 1;
        }
        else
        {
            enemy->primaryVm008.flags1F8 &= ~0x20000u;
        }

        if ((enemy->flags337C & ENEMY_CORE_BOSS) != 0)
        {
            markerPosition.x =
                (enemy->flags337C & ENEMY_CORE_NO_SPRITE) != 0
                    ? -999.0f
                    : enemy->worldPosition2DD4.x + 32.0f;
            markerPosition.y = 472.0f;
            markerPosition.z = 0.0f;
            g_EnemyCoreUi.SetBossMarkerPosition(enemy->bossSlot336B,
                                               &markerPosition);
            int markerState = (enemy->flags3380 >> 1) & 3;
            if (markerState != 0)
                ++markerState;
            else
                markerState = (enemy->primaryVm008.flags1F8 >> 17) & 1;
            g_EnemyCoreUi.SetBossMarkerState(enemy->bossSlot336B,
                                            markerState);
        }

        if (((enemy->flags3380 & 0x200) != 0 &&
             manager->priorityEnemy2AC444 == 0) ||
            (enemy->flags3380 & 0x1000) != 0)
        {
            manager->priorityEnemy2AC444 = enemy;
        }
        if (manager->firstActiveEnemy2AC448 == 0)
            manager->firstActiveEnemy2AC448 = enemy;
        if ((enemy->flags3380 & 0x0C00) == 0x0C00)
        {
            g_EnemyCoreFront->SetSideEnemyIndicatorPosition(
                manager->sideIndex31C, &enemy->position2D74);
            manager->priorityEnemy2AC444 = enemy;
        }
        else if ((enemy->flags3380 & 0x2000) != 0)
        {
            manager->priorityEnemy2AC444 = enemy;
        }

        EnemyCoreUpdateAttachedEffects(enemy);
        enemy->bossTimer2E64++;
        if (enemy->damageReductionTimer53A8.IsAfter(0))
            enemy->damageReductionTimer53A8--;

        if ((enemy->flags337C & ENEMY_CORE_NO_SPRITE) == 0 &&
            (enemy->flags337C & ENEMY_CORE_ACTIVE) != 0)
        {
            EnemyCoreView **head =
                &manager->drawGroupHeads2AC410[enemy->drawGroup3387];
            enemy->nextInDrawGroup04 = *head;
            *head = enemy;
        }
    }

    manager->frameTimer2AC404++;
    return 1;
}
