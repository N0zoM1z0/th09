#include "BulletManager.hpp"
#include "ZunMemory.hpp"

#include <math.h>
#include <new>
#include <string.h>

extern Chain g_Chain;
extern long double __stdcall AnmProjectionAbs(float value);
namespace Th09EclRunControl
{
float __stdcall VectorAngle(float y, float x);
}

struct BulletLoadedSpriteView
{
    unsigned char unknown00[0x30];
    float widthPx;
    float heightPx;
};

struct BulletSoundPlayerView
{
    void PlaySoundByIdx(int soundIndex, int pan);
    void PlaySoundAtPosition(int soundIndex, float x);
};
extern BulletSoundPlayerView g_SoundPlayer;

struct BulletRngView
{
    float GetRandomF32InRange(float maximum);
};
extern BulletRngView g_Rng;

struct BulletSupervisorView
{
    void SelectSide(int sideIndex);
    unsigned char unknown000[0x5B8];
    float framerateMultiplier;
};
extern BulletSupervisorView g_Supervisor;

struct BulletCancelCollisionView
{
    int CheckBulletCancelCollision(Float3 *position, Float3 *collisionSize, Bullet *bullet);
};

struct BulletTimerCurrentView
{
    int previous;
    float subFrame;
    int current;

    int GetCurrent();
};

struct PlayerPositionView;
struct PlayerCollisionQueryStateView
{
    void AppendLaserRecord(
        const PlayerPositionView *position,
        const PlayerPositionView *size,
        const PlayerPositionView *origin,
        float angle,
        int unknown);
    void AppendBoxRecord(
        const PlayerPositionView *position,
        const PlayerPositionView *size,
        int unknown);
};

struct BulletPlayerView
{
    unsigned char unknown000[0x36C];
    BulletCancelCollisionView cancelCollision;

    float AngleToPoint(Float3 *point);
    int CheckGrazeCollision(Float3 *position, Float3 *collisionSize, Bullet *bullet);
    int CheckBulletCollision(Float3 *position, Float3 *collisionSize, Bullet *bullet);
    void CalcLaserHitbox(
        Float3 *center, Float3 *size, Float3 *origin, float angle, int pulse);
};

struct BulletGameManagerView
{
    BulletSideStateView sides[2];
    unsigned char unknown070[0x134 - 0x70];
    unsigned int flags;

    int IsWithinPlayfield(float x, float y, float height, float width);
};
extern BulletGameManagerView g_GameManager;

struct BulletAnmManagerView
{
    int ExecuteScript(AnmVm *vm);
};
extern BulletAnmManagerView *g_AnmManager;

extern int g_BulletSpriteOffsetSmall[];
extern int g_BulletSpriteOffsetMedium[];
extern float __stdcall AddNormalizeAngle(float angle, float delta);

static const float kPi = 3.1415927f;
static const float kTwoPi = 6.2831855f;

enum BulletState
{
    BULLET_STATE_UNUSED = 0,
    BULLET_STATE_FIRED = 1,
    BULLET_STATE_SPAWNING_FAST = 2,
    BULLET_STATE_SPAWNING_NORMAL = 3,
    BULLET_STATE_SPAWNING_SLOW = 4,
    BULLET_STATE_DESPAWNING = 5,
    BULLET_STATE_SENTINEL = 6,
};

enum BulletTransformFlags
{
    BULLET_TRANSFORM_DECELERATE = 0x1,
    BULLET_TRANSFORM_SPAWN_FAST = 0x2,
    BULLET_TRANSFORM_SPAWN_NORMAL = 0x4,
    BULLET_TRANSFORM_SPAWN_SLOW = 0x8,
    BULLET_TRANSFORM_ACCELERATE_VECTOR = 0x10,
    BULLET_TRANSFORM_ACCELERATE_POLAR = 0x20,
    BULLET_TRANSFORM_CHANGE_DIRECTION_RELATIVE = 0x40,
    BULLET_TRANSFORM_CHANGE_DIRECTION_AIMED = 0x80,
    BULLET_TRANSFORM_CHANGE_DIRECTION_ABSOLUTE = 0x100,
    BULLET_TRANSFORM_PLAY_SPAWN_SOUND = 0x200,
    BULLET_TRANSFORM_BOUNCE_ALL_EDGES = 0x400,
    BULLET_TRANSFORM_BOUNCE_EXCEPT_BOTTOM = 0x800,
    BULLET_TRANSFORM_CANCEL_IMMUNE = 0x1000,
    BULLET_TRANSFORM_WAIT = 0x20000,
    BULLET_TRANSFORM_WRAP_X = 0x400000,
    BULLET_TRANSFORM_WRAP_Y = 0x800000,
};

enum BulletAimMode
{
    BULLET_AIM_FAN_AIMED = 0,
    BULLET_AIM_FAN = 1,
    BULLET_AIM_CIRCLE_AIMED = 2,
    BULLET_AIM_CIRCLE = 3,
    BULLET_AIM_OFFSET_CIRCLE_AIMED = 4,
    BULLET_AIM_OFFSET_CIRCLE = 5,
    BULLET_AIM_RANDOM_ANGLE = 6,
    BULLET_AIM_RANDOM_SPEED = 7,
    BULLET_AIM_RANDOM = 8,
};

static BulletLoadedSpriteView *GetLoadedSprite(const AnmVm *vm)
{
    return reinterpret_cast<BulletLoadedSpriteView *>(vm->loadedSprite);
}

static const AnmVm *CopyBulletAnmVmCore(const AnmVm *src, AnmVm *dst)
{
    *dst = *src;
    return src;
}

void EtamaController::SelectBulletSprite(
    AnmVm *dst, const AnmVm *base, const AnmVm *sizeSource, int offset)
{
    int baseSprite = base->activeSpriteIndex;
    if (dst->activeSpriteIndex != baseSprite + offset)
    {
        BulletLoadedSpriteView *sprite =
            reinterpret_cast<BulletLoadedSpriteView *>(sizeSource->loadedSprite);
        if (sprite->widthPx <= 16.0f)
        {
            this->bulletAnm->SetSprite(
                dst, baseSprite + g_BulletSpriteOffsetSmall[offset]);
            return;
        }
        else if (sprite->widthPx <= 32.0f)
        {
            this->bulletAnm->SetSprite(
                dst, baseSprite + g_BulletSpriteOffsetMedium[offset]);
            return;
        }
        else
        {
            this->bulletAnm->SetSprite(dst, baseSprite + offset);
            return;
        }
    }
}

int EtamaController::ClearDrawBuckets()
{
    this->drawBuckets[5] = NULL;
    this->drawBuckets[4] = NULL;
    this->drawBuckets[3] = NULL;
    this->drawBuckets[2] = NULL;
    this->drawBuckets[1] = NULL;
    this->drawBuckets[0] = NULL;
    return 0;
}

void Bullet::Deactivate()
{
    this->state = BULLET_STATE_UNUSED;
    this->stateTimer = 0;
    this->activeTimer = 0;
}

static void UpdateBulletDeceleration(Bullet *bullet)
{
    float magnitude;
    BulletExState &state = bullet->exStates[0];
    if (state.timer <= 16)
    {
        magnitude = 5.0f - (float)state.timer * (5.0f / 16.0f);
        bullet->velocity.FromAngleMagnitude(
            bullet->angle, (magnitude + bullet->speed) * g_Supervisor.framerateMultiplier);
    }
    else
    {
        bullet->activeTransformFlags ^= BULLET_TRANSFORM_DECELERATE;
    }
    state.timer++;
}

static void UpdateBulletVectorAcceleration(Bullet *bullet)
{
    BulletExState &state = bullet->exStates[1];
    if (state.timer >= state.durationFrames)
    {
        bullet->activeTransformFlags &= ~BULLET_TRANSFORM_ACCELERATE_VECTOR;
    }
    else
    {
        bullet->velocity += bullet->exStates[1].vector * g_Supervisor.framerateMultiplier;
        if ((float)AnmProjectionAbs(bullet->velocity.x) > 0.0001f ||
            (float)AnmProjectionAbs(bullet->velocity.y) > 0.0001f)
            bullet->angle = Th09EclRunControl::VectorAngle(
                bullet->velocity.y, bullet->velocity.x);
    }
    state.timer++;
}

static void UpdateBulletPolarAcceleration(Bullet *bullet)
{
    BulletExState &state = bullet->exStates[2];
    if (state.timer >= state.durationFrames)
    {
        bullet->activeTransformFlags &= ~BULLET_TRANSFORM_ACCELERATE_POLAR;
    }
    else
    {
        bullet->angle = AddNormalizeAngle(
            bullet->angle,
            g_Supervisor.framerateMultiplier * bullet->exStates[2].angleDelta);
        bullet->speed +=
            g_Supervisor.framerateMultiplier * bullet->exStates[2].speedDelta;
        bullet->velocity.FromAngleMagnitude(
            bullet->angle, g_Supervisor.framerateMultiplier * bullet->speed);
    }
    state.timer++;
}

static void UpdateBulletRelativeDirectionChange(Bullet *bullet)
{
    float magnitude;
    BulletExState &state = bullet->exStates[3];
    int interval = bullet->exStates[3].directionChangeIntervalFrames;
    if (state.timer >= interval)
    {
        if (bullet->transformSound >= 0)
            g_SoundPlayer.PlaySoundByIdx(bullet->transformSound, 0);
        bullet->exStates[3].directionChangesCompleted += 1;
        if (bullet->exStates[3].directionChangesCompleted >=
            bullet->exStates[3].directionChangeRepeatCount)
            bullet->activeTransformFlags &= ~BULLET_TRANSFORM_CHANGE_DIRECTION_RELATIVE;
        bullet->angle += bullet->exStates[3].directionChangeAngle;
        bullet->speed = bullet->exStates[3].directionChangeSpeed;
        magnitude = bullet->speed;
        state.timer = 0;
    }
    else
    {
        magnitude = bullet->speed -
                    ((float)state.timer * bullet->speed) / interval;
    }
    bullet->velocity.FromAngleMagnitude(
        bullet->angle, magnitude * g_Supervisor.framerateMultiplier);
    state.timer++;
}

static void UpdateBulletAbsoluteDirectionChange(Bullet *bullet)
{
    float magnitude;
    BulletExState &state = bullet->exStates[3];
    int interval = bullet->exStates[3].directionChangeIntervalFrames;
    if (state.timer >= interval)
    {
        if (bullet->transformSound >= 0)
            g_SoundPlayer.PlaySoundByIdx(bullet->transformSound, 0);
        bullet->exStates[3].directionChangesCompleted += 1;
        if (bullet->exStates[3].directionChangesCompleted >=
            bullet->exStates[3].directionChangeRepeatCount)
            bullet->activeTransformFlags &= ~BULLET_TRANSFORM_CHANGE_DIRECTION_ABSOLUTE;
        bullet->angle = bullet->exStates[3].directionChangeAngle;
        bullet->speed = bullet->exStates[3].directionChangeSpeed;
        magnitude = bullet->speed;
        state.timer = 0;
    }
    else
    {
        magnitude = bullet->speed -
                    ((float)state.timer * bullet->speed) / interval;
    }
    bullet->velocity.FromAngleMagnitude(
        bullet->angle, magnitude * g_Supervisor.framerateMultiplier);
    state.timer++;
}

static void UpdateBulletAimedDirectionChange(Bullet *bullet)
{
    float magnitude;
    BulletExState &state = bullet->exStates[3];
    int interval = bullet->exStates[3].directionChangeIntervalFrames;
    if (state.timer >= interval)
    {
        if (bullet->transformSound >= 0)
            g_SoundPlayer.PlaySoundByIdx(bullet->transformSound, 0);
        bullet->exStates[3].directionChangesCompleted += 1;
        if (bullet->exStates[3].directionChangesCompleted >=
            bullet->exStates[3].directionChangeRepeatCount)
            bullet->activeTransformFlags &= ~BULLET_TRANSFORM_CHANGE_DIRECTION_AIMED;
        bullet->angle = AddNormalizeAngle(
            bullet->controller->sideState->player->AngleToPoint(&bullet->position),
            bullet->exStates[3].directionChangeAngle);
        bullet->speed = bullet->exStates[3].directionChangeSpeed;
        magnitude = bullet->speed;
        state.timer = 0;
    }
    else
    {
        magnitude = bullet->speed -
                    ((float)state.timer * bullet->speed) / interval;
    }
    bullet->velocity.FromAngleMagnitude(
        bullet->angle, magnitude * g_Supervisor.framerateMultiplier);
    state.timer++;
}

static void UpdateBulletBoundaryBounce(Bullet *bullet)
{
    float magnitude;
    BulletLoadedSpriteView *sprite = reinterpret_cast<BulletLoadedSpriteView *>(
        bullet->sprites.bulletVm.loadedSprite);
    float *position = bullet->position.operator float *();
    if (!g_GameManager.IsWithinPlayfield(
            position[0], position[1], sprite->heightPx, sprite->widthPx))
    {
        if (bullet->transformSound >= 0)
            g_SoundPlayer.PlaySoundByIdx(bullet->transformSound, 0);
        if (bullet->position.x < -144.0f || bullet->position.x >= 144.0f)
        {
            bullet->angle = -bullet->angle - kPi;
            bullet->angle = AddNormalizeAngle(bullet->angle, 0.0f);
        }
        if (bullet->position.y < 0.0f ||
            (bullet->position.y >= 448.0f &&
             (bullet->activeTransformFlags & BULLET_TRANSFORM_BOUNCE_ALL_EDGES) != 0))
            bullet->angle = -bullet->angle;
        bullet->speed = bullet->exStates[4].bounceSpeed;
        magnitude = bullet->speed;
        bullet->velocity.FromAngleMagnitude(
            bullet->angle, magnitude * g_Supervisor.framerateMultiplier);
        bullet->exStates[4].bouncesCompleted += 1;
        if (bullet->exStates[4].bouncesCompleted >= bullet->exStates[4].bounceLimit)
            bullet->activeTransformFlags &=
                ~(BULLET_TRANSFORM_BOUNCE_ALL_EDGES | BULLET_TRANSFORM_BOUNCE_EXCEPT_BOTTOM);
    }
}

static void UpdateBulletHorizontalWrap(Bullet *bullet)
{
    if (bullet->position.x < 0.0)
        bullet->position.x += 384.0f;
    else if (bullet->position.x > 384.0)
        bullet->position.x -= 384.0f;
    if (bullet->exStates[6].timer <= 0)
        bullet->activeTransformFlags ^= BULLET_TRANSFORM_WRAP_X;
    else
        bullet->exStates[6].timer--;
}

static void UpdateBulletVerticalWrap(Bullet *bullet)
{
    if (bullet->position.y < 0.0)
        bullet->position.y += 448.0f;
    else if (bullet->position.y > 448.0)
        bullet->position.y -= 448.0f;
    if (bullet->exStates[6].timer <= 0)
        bullet->activeTransformFlags ^= BULLET_TRANSFORM_WRAP_Y;
    else
        bullet->exStates[6].timer--;
}

Bullet *EtamaController::SpawnSingleBullet(
    BulletSpawnDescriptor *descriptor, int index1, int index2,
    float angleToPlayer, int poolIndex)
{
    Bullet *bullet;
    int i;
    if (poolIndex == 0)
    {
        bullet = this->primaryPoolStart;
        for (i = 0; i < 175; ++i)
        {
            if (bullet->state == BULLET_STATE_UNUSED)
                break;
            ++bullet;
            if (bullet->state == BULLET_STATE_SENTINEL)
                bullet = &this->primaryBullets[0];
        }
        if (i >= 175)
            return bullet;
    }
    else
    {
        bullet = this->secondaryPoolStart;
        for (i = 0; i < 360; ++i)
        {
            if (bullet->state == BULLET_STATE_UNUSED)
                break;
            ++bullet;
            if (bullet->state == BULLET_STATE_SENTINEL)
                bullet = &this->secondaryBullets[0];
        }
        if (i >= 360)
            return bullet;
    }

    float angle = 0.0f;
    float speed;
    if (descriptor->count2 > 1)
        speed = descriptor->speed1 -
                (descriptor->speed1 - descriptor->speed2) * (float)index2 /
                    (float)descriptor->count2;
    else
        speed = descriptor->speed1;

    switch (descriptor->aimMode)
    {
    case BULLET_AIM_FAN_AIMED:
    case BULLET_AIM_FAN:
        if ((descriptor->count1 & 1) != 0)
            angle += (float)((index1 + 1) / 2) * descriptor->angleStep;
        else
            angle += (float)(index1 / 2) * descriptor->angleStep + descriptor->angleStep * 0.5f;
        if ((index1 & 1) != 0)
            angle *= -1.0f;
        if (descriptor->aimMode == BULLET_AIM_FAN_AIMED)
            angle += angleToPlayer;
        angle += descriptor->angle;
        break;
    case BULLET_AIM_CIRCLE_AIMED:
        angle += angleToPlayer;
    case BULLET_AIM_CIRCLE:
        angle += (float)index1 * kTwoPi / (float)descriptor->count1;
        angle += (float)index2 * descriptor->angleStep + descriptor->angle;
        break;
    case BULLET_AIM_OFFSET_CIRCLE_AIMED:
        angle += angleToPlayer;
    case BULLET_AIM_OFFSET_CIRCLE:
        angle += kPi / (float)descriptor->count1;
        angle += (float)index1 * kTwoPi / (float)descriptor->count1;
        angle += descriptor->angle;
        break;
    case BULLET_AIM_RANDOM_ANGLE:
        angle = g_Rng.GetRandomF32InRange(descriptor->angle - descriptor->angleStep) +
                descriptor->angleStep;
        break;
    case BULLET_AIM_RANDOM_SPEED:
        speed = g_Rng.GetRandomF32InRange(descriptor->speed1 - descriptor->speed2) +
                descriptor->speed2;
        angle += (float)index1 * kTwoPi / (float)descriptor->count1;
        angle += (float)index2 * descriptor->angleStep + descriptor->angle;
        break;
    case BULLET_AIM_RANDOM:
        angle = g_Rng.GetRandomF32InRange(descriptor->angle - descriptor->angleStep) +
                descriptor->angleStep;
        speed = g_Rng.GetRandomF32InRange(descriptor->speed1 - descriptor->speed2) +
                descriptor->speed2;
        break;
    }

    bullet->controller = this;
    bullet->state = BULLET_STATE_FIRED;
    bullet->spawnMarker = 1;
    bullet->isGrazed = 0;
    bullet->stateTimer = 0;
    bullet->collisionDisabled = 0;
    bullet->activeTimer = 0;
    bullet->unknown10BE = 0;
    bullet->speed = speed;
    bullet->angle = AddNormalizeAngle(angle, 0.0f);
    bullet->extraAttribute10BD = descriptor->extraAttribute20C;
    bullet->position = descriptor->position;
    float *bulletPosition = bullet->position;
    bulletPosition[2] = 0.1f;
    bullet->velocity.FromAngleMagnitude(
        angle, speed * g_Supervisor.framerateMultiplier);
    bullet->activeTransformFlags = descriptor->transformFlags;
    bullet->color = descriptor->color;
    bullet->zoneTransitionCooldownFrames = 0;
    bullet->cancelledDuringSpawn = 0;

    CopyBulletAnmVmCore(&descriptor->templateSprites->bulletVm, &bullet->sprites.bulletVm);
    CopyBulletAnmVmCore(&descriptor->templateSprites->despawnVm, &bullet->sprites.despawnVm);
    bullet->bulletType = descriptor->bulletType;
    bullet->color = descriptor->color;
    bullet->sprites.unknownD44 = descriptor->templateSprites->unknownD44;
    bullet->sprites.collisionSize = descriptor->templateSprites->collisionSize;
    bullet->sprites.unknownD40 = descriptor->templateSprites->unknownD40;
    bullet->sprites.spriteHeightPx = descriptor->templateSprites->spriteHeightPx;
    bullet->sprites.drawBucketIndex = descriptor->templateSprites->drawBucketIndex;
    bullet->transformSound = descriptor->transformSound;
    bullet->offscreenCullDelayFrames = 0;

    if (bullet->sprites.bulletVm.activeSpriteIndex !=
        descriptor->templateSprites->bulletVm.activeSpriteIndex + descriptor->color)
        this->bulletAnm->SetSprite(
            &bullet->sprites.bulletVm,
            descriptor->templateSprites->bulletVm.activeSpriteIndex + descriptor->color);

    int despawnBaseSprite =
        descriptor->templateSprites->despawnVm.activeSpriteIndex;
    if (bullet->sprites.despawnVm.activeSpriteIndex !=
        descriptor->templateSprites->despawnVm.activeSpriteIndex + descriptor->color)
    {
        if (reinterpret_cast<BulletLoadedSpriteView *>(
                bullet->sprites.bulletVm.loadedSprite)->widthPx <= 16.0f)
        {
            this->bulletAnm->SetSprite(
                &bullet->sprites.despawnVm,
                despawnBaseSprite +
                    g_BulletSpriteOffsetSmall[descriptor->color]);
        }
        else if (reinterpret_cast<BulletLoadedSpriteView *>(
                     bullet->sprites.bulletVm.loadedSprite)->widthPx <= 32.0f)
        {
            this->bulletAnm->SetSprite(
                &bullet->sprites.despawnVm,
                despawnBaseSprite +
                    g_BulletSpriteOffsetMedium[descriptor->color]);
        }
        else
        {
            this->bulletAnm->SetSprite(
                &bullet->sprites.despawnVm,
                despawnBaseSprite + descriptor->color);
        }
    }

    if ((static_cast<unsigned char>(descriptor->transformFlags) &
         BULLET_TRANSFORM_SPAWN_FAST) != 0)
    {
        CopyBulletAnmVmCore(
            &descriptor->templateSprites->spawnFastVm, &bullet->sprites.spawnFastVm);
        this->SelectBulletSprite(
            &bullet->sprites.spawnFastVm, &descriptor->templateSprites->spawnFastVm,
            &bullet->sprites.bulletVm, descriptor->color);
        bullet->state = BULLET_STATE_SPAWNING_FAST;
        bullet->position -= bullet->velocity * 4.0f;
    }
    else if ((descriptor->transformFlags & BULLET_TRANSFORM_SPAWN_NORMAL) != 0)
    {
        CopyBulletAnmVmCore(
            &descriptor->templateSprites->spawnNormalVm, &bullet->sprites.spawnNormalVm);
        this->SelectBulletSprite(
            &bullet->sprites.spawnNormalVm, &descriptor->templateSprites->spawnNormalVm,
            &bullet->sprites.bulletVm, descriptor->color);
        bullet->state = BULLET_STATE_SPAWNING_NORMAL;
        bullet->position -= bullet->velocity * 4.0f;
    }
    else if ((descriptor->transformFlags & BULLET_TRANSFORM_SPAWN_SLOW) != 0)
    {
        CopyBulletAnmVmCore(
            &descriptor->templateSprites->spawnSlowVm, &bullet->sprites.spawnSlowVm);
        this->SelectBulletSprite(
            &bullet->sprites.spawnSlowVm, &descriptor->templateSprites->spawnSlowVm,
            &bullet->sprites.bulletVm, descriptor->color);
        bullet->state = BULLET_STATE_SPAWNING_SLOW;
        bullet->position -= bullet->velocity * 4.0f;
    }

    memcpy(bullet->transforms, descriptor->transforms, sizeof(descriptor->transforms));
    bullet->transformFlags = descriptor->transformFlags;
    bullet->activeTransformFlags = 0;
    bullet->transformIndex = descriptor->transformStartIndex;
    bullet->AdvanceTransformProgram();

    if (this->spawnSuppressionFrames != 0 &&
        (bullet->transformFlags & BULLET_TRANSFORM_CANCEL_IMMUNE) == 0)
        bullet->state = BULLET_STATE_DESPAWNING;
    return bullet;
}

Bullet *EtamaController::SpawnBulletPatternPrimary(BulletSpawnDescriptor *descriptor)
{
    int index1;
    float angleToPlayer;
    int index2;
    Bullet *result;
    descriptor->templateSprites = &this->bulletTypeSprites[descriptor->bulletType];
    angleToPlayer = this->sideState->player->AngleToPoint(&descriptor->position);
    for (index2 = 0; index2 < descriptor->count2; ++index2)
    {
        for (index1 = 0; index1 < descriptor->count1; ++index1)
        {
            result = this->SpawnSingleBullet(descriptor, index1, index2, angleToPlayer, 0);
            if (result == &this->primaryBullets[175])
                goto done;
        }
    }
done:
    if ((descriptor->transformFlags & BULLET_TRANSFORM_PLAY_SPAWN_SOUND) != 0)
        g_SoundPlayer.PlaySoundAtPosition(descriptor->spawnSound, descriptor->position.x);
    return result;
}

Bullet *EtamaController::SpawnBulletPatternSecondary(BulletSpawnDescriptor *descriptor)
{
    int index1;
    float angleToPlayer;
    int index2;
    Bullet *result;
    descriptor->templateSprites = &this->bulletTypeSprites[descriptor->bulletType];
    angleToPlayer = this->sideState->player->AngleToPoint(&descriptor->position);
    for (index2 = 0; index2 < descriptor->count2; ++index2)
    {
        for (index1 = 0; index1 < descriptor->count1; ++index1)
        {
            result = this->SpawnSingleBullet(descriptor, index1, index2, angleToPlayer, 1);
            if (result == &this->secondaryBullets[360])
                goto done;
        }
    }
done:
    if ((descriptor->transformFlags & BULLET_TRANSFORM_PLAY_SPAWN_SOUND) != 0)
        g_SoundPlayer.PlaySoundAtPosition(descriptor->spawnSound, descriptor->position.x);
    return result;
}

int EtamaController::OnUpdate(EtamaController *controller)
{
    float currentWidth;

    Bullet *bullet = &controller->primaryBullets[0];
    if ((g_GameManager.flags & 0x1800) != 0)
        return 1;

    g_Supervisor.SelectSide(controller->sideIndex);
    controller->activeTotalCount = 0;
    controller->activePrimaryCount = 0;
    controller->activeSecondaryCount = 0;
    controller->ClearDrawBuckets();

    for (int i = 0; i < 536; ++bullet, ++i)
    {
        if (bullet->state == BULLET_STATE_UNUSED || bullet->state == BULLET_STATE_SENTINEL)
            continue;

        Bullet **drawBucket;
        if ((controller->sideState->flags & 1) != 0)
        {
            drawBucket = &controller->drawBuckets[bullet->sprites.drawBucketIndex];
            goto queueBullet;
        }

        ++controller->activeTotalCount;
        if (i < 175)
            ++controller->activePrimaryCount;
        else
            ++controller->activeSecondaryCount;

        switch (bullet->state)
            {
        activateBullet:
                bullet->state = BULLET_STATE_FIRED;
                bullet->stateTimer = 0;
                break;
            case BULLET_STATE_SPAWNING_FAST:
                bullet->activeTimer--;
                bullet->position += bullet->velocity / 2.0f;
                if (g_AnmManager->ExecuteScript(&bullet->sprites.spawnFastVm) == 0)
                    goto updateTimers;
                if (bullet->cancelledDuringSpawn != 0)
                    bullet->state = BULLET_STATE_DESPAWNING;
                goto activateBullet;
            case BULLET_STATE_SPAWNING_NORMAL:
                bullet->activeTimer--;
                bullet->position += bullet->velocity / 2.5f;
                if (g_AnmManager->ExecuteScript(&bullet->sprites.spawnNormalVm) == 0)
                    goto updateTimers;
                if (bullet->cancelledDuringSpawn != 0)
                    bullet->state = BULLET_STATE_DESPAWNING;
                goto activateBullet;
            case BULLET_STATE_SPAWNING_SLOW:
                bullet->activeTimer--;
                bullet->position += bullet->velocity / 3.0f;
                if (g_AnmManager->ExecuteScript(&bullet->sprites.spawnSlowVm) == 0)
                    goto updateTimers;
                if (bullet->cancelledDuringSpawn != 0)
                    bullet->state = BULLET_STATE_DESPAWNING;
                goto activateBullet;
            case BULLET_STATE_FIRED:
                break;
            case BULLET_STATE_DESPAWNING:
                bullet->position += bullet->velocity / 2.0f;
                if (g_AnmManager->ExecuteScript(&bullet->sprites.despawnVm) != 0)
                {
                    bullet->Deactivate();
                    continue;
                }
                goto updateTimers;
            default:
                goto updateTimers;
            }

            bullet->AdvanceTransformProgram();
            if (bullet->activeTransformFlags != 0)
            {
                if ((bullet->activeTransformFlags & BULLET_TRANSFORM_DECELERATE) != 0)
                    UpdateBulletDeceleration(bullet);
                if ((bullet->activeTransformFlags & BULLET_TRANSFORM_ACCELERATE_VECTOR) != 0)
                    UpdateBulletVectorAcceleration(bullet);
                if ((bullet->activeTransformFlags & BULLET_TRANSFORM_ACCELERATE_POLAR) != 0)
                    UpdateBulletPolarAcceleration(bullet);
                if ((bullet->activeTransformFlags & BULLET_TRANSFORM_CHANGE_DIRECTION_RELATIVE) != 0)
                    UpdateBulletRelativeDirectionChange(bullet);
                if ((bullet->activeTransformFlags & BULLET_TRANSFORM_CHANGE_DIRECTION_ABSOLUTE) != 0)
                    UpdateBulletAbsoluteDirectionChange(bullet);
                if ((bullet->activeTransformFlags & BULLET_TRANSFORM_CHANGE_DIRECTION_AIMED) != 0)
                    UpdateBulletAimedDirectionChange(bullet);
                if ((bullet->activeTransformFlags &
                     (BULLET_TRANSFORM_BOUNCE_ALL_EDGES | BULLET_TRANSFORM_BOUNCE_EXCEPT_BOTTOM)) != 0)
                    UpdateBulletBoundaryBounce(bullet);
                if ((bullet->activeTransformFlags & BULLET_TRANSFORM_WRAP_X) != 0)
                    UpdateBulletHorizontalWrap(bullet);
                if ((bullet->activeTransformFlags & BULLET_TRANSFORM_WRAP_Y) != 0)
                    UpdateBulletVerticalWrap(bullet);
                unsigned int activeTransformFlags = bullet->activeTransformFlags;
                if ((activeTransformFlags & BULLET_TRANSFORM_WAIT) != 0)
                {
                    if (bullet->exStates[5].timer <= 0)
                        bullet->activeTransformFlags =
                            activeTransformFlags ^ BULLET_TRANSFORM_WAIT;
                    else
                        bullet->exStates[5].timer--;
                }
            }

            if (bullet->offscreenCullDelayFrames != 0)
                --bullet->offscreenCullDelayFrames;
            bullet->position += bullet->velocity;

            if (bullet->offscreenCullDelayFrames == 0)
            {
                BulletLoadedSpriteView *sprite =
                    reinterpret_cast<BulletLoadedSpriteView *>(
                        bullet->sprites.bulletVm.loadedSprite);
                float *position = bullet->position.operator float *();
                if (!g_GameManager.IsWithinPlayfield(
                        position[0], position[1], sprite->heightPx, sprite->widthPx))
                {
                    if ((bullet->activeTransformFlags & 0xDC0) != 0)
                    {
                        ++bullet->offscreenFrames;
                        if (bullet->offscreenFrames >= 0x80)
                        {
                            bullet->Deactivate();
                            continue;
                        }
                    }
                    else if (bullet->offscreenFrames == 0)
                    {
                        bullet->Deactivate();
                        continue;
                    }
                    else
                    {
                        --bullet->offscreenFrames;
                    }
                }
                else
                {
                    bullet->offscreenFrames = 0;
                }
            }

            if (bullet->collisionDisabled == 0)
            {
                BulletPlayerView *player;
                int collisionResult;
                if (bullet->isGrazed == 0)
                {
                    player = controller->sideState->player;
                    collisionResult = player->CheckGrazeCollision(
                        &bullet->position, &bullet->sprites.collisionSize, bullet);
                    if (collisionResult == 1)
                    {
                        bullet->isGrazed = 1;
                    }
                    else
                    {
                        if (collisionResult == 2 &&
                            (bullet->transformFlags & BULLET_TRANSFORM_CANCEL_IMMUNE) == 0)
                            bullet->state = BULLET_STATE_DESPAWNING;
                        goto executeBulletScript;
                    }
                }

                player = controller->sideState->player;
                collisionResult = player->CheckBulletCollision(
                    &bullet->position, &bullet->sprites.collisionSize, bullet);
                if (collisionResult != 0 &&
                    (collisionResult != 2 ||
                     (bullet->transformFlags & BULLET_TRANSFORM_CANCEL_IMMUNE) == 0))
                {
                    bullet->state = BULLET_STATE_DESPAWNING;
                }
                else
                {
                    player = controller->sideState->player;
                    reinterpret_cast<PlayerCollisionQueryStateView *>(
                        reinterpret_cast<unsigned char *>(player) + 0x36C)
                        ->AppendBoxRecord(
                            reinterpret_cast<PlayerPositionView *>(&bullet->position),
                            reinterpret_cast<PlayerPositionView *>(
                                &bullet->sprites.collisionSize),
                            reinterpret_cast<int>(bullet));
                }
            }

executeBulletScript:
        if (bullet->sprites.bulletVm.currentInstruction != NULL)
            g_AnmManager->ExecuteScript(&bullet->sprites.bulletVm);

updateTimers:
        bullet->stateTimer++;
        bullet->activeTimer++;
        drawBucket = &controller->drawBuckets[bullet->sprites.drawBucketIndex];

queueBullet:
        bullet->nextInDrawBucket = *drawBucket;
        *drawBucket = bullet;
    }

    if ((controller->sideState->flags & 1) != 0)
        return 1;

    Laser *laser = &controller->lasers[0];
    for (int i = 0; i < 48; ++i, ++laser)
    {
        if (laser->inUse == 0)
            continue;

        float laserCenter[3];
        float laserSize[3];

        laser->endOffset += g_Supervisor.framerateMultiplier * laser->speed;
        if (laser->endOffset - laser->startOffset > laser->startLength)
            laser->startOffset = laser->endOffset - laser->startLength;
        if (laser->startOffset < 0.0f)
            laser->startOffset = 0.0f;

        laserSize[1] = laser->width / 2.0f;
        laserSize[0] = laser->endOffset - laser->startOffset;
        laserCenter[0] = (laser->endOffset - laser->startOffset) / 2.0f +
                         laser->startOffset + laser->position.x;
        laserCenter[1] = laser->position.y;
        BulletLoadedSpriteView *laserSprite =
            reinterpret_cast<BulletLoadedSpriteView *>(laser->bodyVm.loadedSprite);
        laser->bodyVm.scale.x = laser->width / laserSprite->heightPx;
        float currentLength = laser->endOffset - laser->startOffset;
        laser->bodyVm.scale.y = currentLength / laserSprite->widthPx;
        laser->bodyVm.SetZRotation(AddNormalizeAngle(kPi / 2.0f + laser->angle, 0.0f));

        switch (laser->state)
        {
        case 0:
            if ((laser->flags & 1) != 0)
            {
                int alpha = (int)((float)laser->timer * 255.0f / laser->startTime);
                if (alpha > 255)
                    alpha = 255;
                laser->bodyVm.color1 = (unsigned int)alpha << 24;
            }
            else
            {
                int startTime = laser->startTime;
                int rampWindow = startTime > 30 ? 30 : startTime;
                if (startTime - rampWindow <
                    reinterpret_cast<BulletTimerCurrentView *>(
                        &laser->timer)->GetCurrent())
                    currentWidth =
                        (float)laser->timer * laser->width / startTime;
                else
                    currentWidth = 1.2f;
                laser->currentWidth = currentWidth;
                laser->bodyVm.scale.x = currentWidth / 16.0f;
                laserSize[0] = currentWidth / 2.0f;
            }
            if (laser->timer >= laser->hitboxStartTime)
                reinterpret_cast<PlayerCollisionQueryStateView *>(
                    reinterpret_cast<unsigned char *>(controller->sideState->player) + 0x36C)
                    ->AppendLaserRecord(
                        reinterpret_cast<PlayerPositionView *>(laserCenter),
                        reinterpret_cast<PlayerPositionView *>(laserSize),
                        reinterpret_cast<PlayerPositionView *>(&laser->position),
                        laser->angle,
                        0);
            if (laser->timer < laser->startTime)
                break;
            laser->timer = 0;
            ++laser->state;
            laser->currentWidth = laser->width;
        case 1:
            reinterpret_cast<PlayerCollisionQueryStateView *>(
                    reinterpret_cast<unsigned char *>(controller->sideState->player) + 0x36C)
                    ->AppendLaserRecord(
                        reinterpret_cast<PlayerPositionView *>(laserCenter),
                        reinterpret_cast<PlayerPositionView *>(laserSize),
                        reinterpret_cast<PlayerPositionView *>(&laser->position),
                        laser->angle,
                        0);
            if (laser->timer < laser->duration)
                break;
            laser->timer = 0;
            ++laser->state;
            if (laser->despawnDuration == 0)
            {
                laser->inUse = 0;
                continue;
            }
        case 2:
            if ((laser->flags & 1) != 0)
            {
                int alpha = (int)((float)laser->timer * 255.0f / laser->startTime);
                if (alpha > 255)
                    alpha = 255;
                laser->bodyVm.color1 = (unsigned int)alpha << 24;
            }
            else
            {
                int despawnDuration = laser->despawnDuration;
                if (despawnDuration > 0)
                {
                    currentWidth = laser->width -
                        (float)laser->timer * laser->width / despawnDuration;
                    laser->bodyVm.scale.x = currentWidth / 16.0f;
                    laserSize[0] = currentWidth / 2.0f;
                }
            }
            if (laser->timer < laser->hitboxEndDelay)
                reinterpret_cast<PlayerCollisionQueryStateView *>(
                    reinterpret_cast<unsigned char *>(controller->sideState->player) + 0x36C)
                    ->AppendLaserRecord(
                        reinterpret_cast<PlayerPositionView *>(laserCenter),
                        reinterpret_cast<PlayerPositionView *>(laserSize),
                        reinterpret_cast<PlayerPositionView *>(&laser->position),
                        laser->angle,
                        0);
            if (laser->timer < laser->despawnDuration)
                break;
            laser->inUse = 0;
            continue;
        }

        if (laser->startOffset >= 640.0f)
            laser->inUse = 0;
        laser->timer++;
        g_AnmManager->ExecuteScript(&laser->bodyVm);
    }

    if (controller->spawnSuppressionFrames != 0)
        --controller->spawnSuppressionFrames;
    controller->timer++;
    ++controller->frameCounter;
    return 1;
}

void EtamaController::Release(EtamaController *controller)
{
    if (controller == NULL)
        return;
    g_Chain.Cut(controller->drawChain);
    g_Chain.Cut(controller->calcChain);
    delete controller;
}

EtamaController *EtamaController::Register(int sideIndex)
{
    EtamaController *controller = new EtamaController;
    controller = static_cast<EtamaController *>(g_ZunMemory.AddToRegistry(
        controller, sizeof(EtamaController), const_cast<char *>("EtamaCtrlInf")));
    controller->Initialize();
    controller->sideState = &g_GameManager.sides[sideIndex];
    controller->sideIndex = sideIndex;
    controller->opponentSideState = &g_GameManager.sides[1 - sideIndex];

    controller->calcChain = g_Chain.CreateElem(
        reinterpret_cast<ChainCallback>(EtamaController::OnUpdate));
    controller->calcChain->arg = controller;
    controller->calcChain->addedCallback =
        reinterpret_cast<ChainLifetimeCallback>(EtamaController::AddedCallback);
    if (g_Chain.AddToCalcChain(controller->calcChain, sideIndex + 12) != 0)
        return NULL;

    controller->drawChain = g_Chain.CreateElem(
        reinterpret_cast<ChainCallback>(EtamaController::OnDraw));
    controller->drawChain->arg = controller;
    g_Chain.AddToDrawChain(controller->drawChain, sideIndex + 21);
    return controller;
}
