#include "PlayerLifecycleView.hpp"
#include "BulletManager.hpp"
#include "EffectManager.hpp"
#include "RngRuntimeLeaves.hpp"

struct PlayerCollisionTimerView2
{
    int previous;
    float subFrame;
    int current;

    unsigned int IsAfter(int value);
};

typedef char PlayerCollisionTimer2SizeIsC[
    (sizeof(PlayerCollisionTimerView2) == 0x0C) ? 1 : -1];

void RotatePlayerCollisionVector(
    PlayerPositionView *outVector,
    PlayerPositionView *point,
    float angle);


struct PlayerCancelRegionView
{
    float centerX00;
    float centerY04;
    float radius08;
    unsigned char unknown0C[0x04];
    float halfWidth10;
    float halfHeight14;
    unsigned char unknown18[0x08];
    float angle20;
    unsigned char unknown24[0x08];
    int hitCount2C;
    unsigned char unknown30[0x08];
    int type38;
    unsigned char unknown3C[0x04];
    int delay40;
};

typedef char PlayerCancelRegionSizeIs44[
    (sizeof(PlayerCancelRegionView) == 0x44) ? 1 : -1];

struct PlayerCancelRuntimeView
{
    unsigned char unknown0000[0xB100];
    PlayerCancelRegionView *activeRegionsB100[514];
};

typedef char PlayerCancelActiveAtB100[
    (offsetof(PlayerCancelRuntimeView, activeRegionsB100) == 0xB100) ? 1 : -1];

struct PlayerCancelEffectView
{
    unsigned char unknown000[0xA0];
    float valueA0;
    short bulletTypeA4;
    short colorA6;
    short sideA8;
    short extraAA;
};

typedef char PlayerCancelEffectValueA0[
    (offsetof(PlayerCancelEffectView, valueA0) == 0xA0) ? 1 : -1];
typedef char PlayerCancelEffectTypeA4[
    (offsetof(PlayerCancelEffectView, bulletTypeA4) == 0xA4) ? 1 : -1];
typedef char PlayerCancelEffectExtraAA[
    (offsetof(PlayerCancelEffectView, extraAA) == 0xAA) ? 1 : -1];

struct PlayerCancelSupervisorView
{
    void SelectSide(int side);
};

struct PlayerCancelGameManagerView
{
    float TransformPopupX(float value);
    float TransformPopupY(float value);
};

struct PlayerCancelSoundView
{
    void PlaySoundByIdx(int soundId, int pan);
};

struct PlayerCancelSideView
{
    void AddScore(int score);
};

struct PlayerCancelOwnerStateView
{
    unsigned char unknown00[0x38];
    int state38;
};

struct PlayerCancelState4OpsView
{
    void AddRespawnResource(float value);
};

extern PlayerCancelSupervisorView g_PlayerSupervisorRuntime;
extern PlayerCancelGameManagerView g_PlayerGameManagerRuntime;
extern PlayerCancelSoundView g_SoundPlayer;
extern RngRuntimeView g_ReplayRng;
extern EffectManager *g_PlayerRewardEffectManager;
extern float g_PlayerRewardVelocitySpan;
extern int g_PlayerRewardModeValue;


int PlayerLifecycleView::CheckBulletCollision(
    PlayerPositionView *position,
    PlayerPositionView *collisionSize,
    Bullet *bullet)
{
    (void)collisionSize;

    PlayerPositionView delta;
    PlayerPositionView rotated;
    PlayerPositionView halfSize;
    PlayerPositionView boundsMax;
    float xDelta;
    float yDelta;

    PlayerLifecycleView *player = this;
    Bullet *currentBullet = bullet;
    PlayerCancelRegionView **slot =
        reinterpret_cast<PlayerCancelRuntimeView *>(player)->activeRegionsB100;

    while (*slot != NULL)
    {
        if ((*slot)->type38 != 1 &&
            (*slot)->type38 != 2 &&
            (*slot)->type38 != 3)
            goto next;
        if ((*slot)->delay40 > 0)
            goto next;

        if ((*slot)->radius08 != 0.0)
        {
            xDelta = position->x - (*slot)->centerX00;
            yDelta = position->y - (*slot)->centerY04;
            if (xDelta * xDelta + yDelta * yDelta <
                (*slot)->radius08 * (*slot)->radius08)
                goto hit;
            goto next;
        }

        if ((*slot)->angle20 != 0.0f)
        {
            delta.x = position->x - (*slot)->centerX00;
            delta.y = position->y - (*slot)->centerY04;
            RotatePlayerCollisionVector(
                &rotated, &delta, -(*slot)->angle20);

            halfSize.x = (*slot)->halfWidth10;
            halfSize.y = (*slot)->halfHeight14;
            if (-halfSize.x <= rotated.x &&
                rotated.x <= halfSize.x &&
                -halfSize.y <= rotated.y &&
                rotated.y <= halfSize.y)
                goto hit;
            goto next;
        }

        halfSize.x = (*slot)->centerX00 - (*slot)->halfWidth10;
        halfSize.y = (*slot)->centerY04 - (*slot)->halfHeight14;
        boundsMax.x = (*slot)->halfWidth10 + (*slot)->centerX00;
        boundsMax.y = (*slot)->halfHeight14 + (*slot)->centerY04;

        if (!(halfSize.x > position->x))
        {
            if (!(boundsMax.x < position->x))
            {
                if (!(halfSize.y > position->y))
                {
                    if (!(boundsMax.y < position->y))
                        goto hit;
                }
            }
        }
        goto next;

hit:
        if ((*slot)->type38 != 2 && (*slot)->type38 != 3)
        {
            ++(*slot)->hitCount2C;
            return 2;
        }

        if (currentBullet == NULL || currentBullet->bulletType != 0)
            goto next;

        {
            EffectFloat3 effectPosition;
            effectPosition.x =
                g_PlayerGameManagerRuntime.TransformPopupX(position->x);
            effectPosition.y =
                g_PlayerGameManagerRuntime.TransformPopupY(position->y);
            effectPosition.z = 0.0f;

            g_PlayerSupervisorRuntime.SelectSide(1 - player->sideIndex);

            EffectFloat3 velocity;
            velocity.x = g_ReplayRng.GetRandomF32SignedInRange(
                g_PlayerRewardVelocitySpan * 0.5f - 8.0f);
            velocity.y = g_ReplayRng.GetRandomF32InRange(128.0f);
            velocity.z = 0.0f;

            Effect *effectBase =
                g_PlayerRewardEffectManager->SpawnEffectWithVelocity(
                    player->sideIndex + 1, &effectPosition, &velocity,
                    1, static_cast<unsigned int>(-1));
            g_PlayerSupervisorRuntime.SelectSide(player->sideIndex);

            PlayerCancelEffectView *effect =
                reinterpret_cast<PlayerCancelEffectView *>(effectBase);
            effect->bulletTypeA4 = currentBullet->bulletType;
            effect->colorA6 = currentBullet->color;
            effect->valueA0 = currentBullet->speed;
            effect->sideA8 =
                static_cast<short>(currentBullet->controller->sideIndex);
            effect->extraAA = currentBullet->extraAttribute10BD;

            reinterpret_cast<PlayerCancelState4OpsView *>(player)
                ->AddRespawnResource(0.2f);
            g_SoundPlayer.PlaySoundByIdx(
                46, player->sideIndex != 0 ? 500 : -500);

            int rewardValue =
                reinterpret_cast<PlayerCancelOwnerStateView *>(
                    &player->ownerState30410)->state38;
            if (rewardValue > 100)
                rewardValue = 1000;
            else
                rewardValue *= 10;

            int rewardMode =
                (g_PlayerRewardModeValue >= 3) + 2;
            player->ownerState30410.ApplyReward(
                position, rewardMode, 2, 0, rewardValue);

            reinterpret_cast<PlayerCancelSideView *>(player->sideState)
                ->AddScore(50);
        }

        ++(*slot)->hitCount2C;
        return 2;

next:
        ++slot;
    }

    return 0;
}

int PlayerLifecycleView::CheckGrazeCollision(
    PlayerPositionView *position,
    PlayerPositionView *collisionSize,
    Bullet *bullet)
{
    if (CheckBulletCollision(position, collisionSize, bullet))
        return 2;

    PlayerPositionView incomingMin;
    PlayerPositionView incomingMax;
    incomingMin.x = position->x - collisionSize->x * 0.5f - 60.0f;
    incomingMin.y = position->y - collisionSize->y * 0.5f - 60.0f;
    incomingMax.x = collisionSize->x * 0.5f + position->x + 60.0f;
    incomingMax.y = collisionSize->y * 0.5f + position->y + 60.0f;

    if (!(grazeBoundsMin1C78.x > incomingMax.x))
    {
        if (!(grazeBoundsMax1C84.x < incomingMin.x))
        {
            if (!(grazeBoundsMin1C78.y > incomingMax.y))
            {
                if (!(grazeBoundsMax1C84.y < incomingMin.y))
                    return 1;
            }
        }
    }
    return 0;
}

int PlayerLifecycleView::CalcItemBoxCollision(
    PlayerPositionView *position,
    PlayerPositionView *halfSize)
{
    PlayerPositionView incomingMin;
    PlayerPositionView incomingMax;

    incomingMin.x = position->x - halfSize->x;
    incomingMin.y = position->y - halfSize->y;
    incomingMax.x = position->x + halfSize->x;
    incomingMax.y = position->y + halfSize->y;

    if (collisionBoundsMin1C60.x > incomingMax.x ||
        collisionBoundsMin1C60.y > incomingMax.y ||
        collisionBoundsMax1C6C.x < incomingMin.x ||
        collisionBoundsMax1C6C.y < incomingMin.y)
        return 0;
    return 1;
}

int PlayerLifecycleView::CalcCircleCollision(
    PlayerPositionView *position,
    float radius)
{
    PlayerPositionView delta;
    delta = position1B88 - *position;
    float collisionRadius = radius + primaryShtFile->hurtboxSize;
    if (delta.x * delta.x + delta.y * delta.y >=
        collisionRadius * collisionRadius)
        return 0;
    return 1;
}

int PlayerLifecycleView::CalcLaserHitbox(
    PlayerPositionView *position,
    PlayerPositionView *halfSize,
    PlayerPositionView *origin,
    float angle,
    int expandForGraze)
{
    PlayerPositionView incomingMin;
    PlayerPositionView incomingMax;
    PlayerPositionView playerMin;
    PlayerPositionView playerMax;

    incomingMin = position1B88 - *origin;
    RotatePlayerCollisionVector(&incomingMax, &incomingMin, -angle);
    incomingMax.z = 0.0f;
    incomingMin = incomingMax + *origin;

    playerMin = incomingMin - hurtboxHalfSize;
    playerMax = incomingMin + hurtboxHalfSize;
    incomingMin = *position - *halfSize;
    incomingMax = *position + *halfSize;

    if (!(playerMin.x > incomingMax.x))
    {
        if (!(playerMax.x < incomingMin.x))
        {
            if (!(playerMin.y > incomingMax.y))
            {
                if (!(playerMax.y < incomingMin.y))
                    return 1;
            }
        }
    }

    if (!expandForGraze)
        return 0;

    incomingMin.x -= 48.0f;
    incomingMin.y -= 48.0f;
    incomingMax.x += 48.0f;
    incomingMax.y += 48.0f;

    if (playerMin.x > incomingMax.x || playerMax.x < incomingMin.x ||
        playerMin.y > incomingMax.y || playerMax.y < incomingMin.y)
        return 0;
    return 2;
}

PlayerCollisionQueryRecordView *PlayerCollisionQueryStateView::FindCollisionAtPlayer()
{
    PlayerLifecycleView *player = player00;
    if (player->GetUpdateState() != 0)
        return NULL;
    if (reinterpret_cast<PlayerCollisionTimerView2 *>(&player->timer1B74)->IsAfter(0))
        return NULL;

    PlayerCollisionQueryRecordView *record = records;
    player->collisionBoundsMin1C60 = player->position1B88 - player->hurtboxHalfSize;
    player = player00;
    player->collisionBoundsMax1C6C = player->position1B88 + player->hurtboxHalfSize;

    for (int i = 0; i < count1804; ++i, ++record)
    {
        int hit;
        if (record->radius24 == 0.0f)
        {
            if (record->angle28 == 0.0f)
                hit = player00->CalcItemBoxCollision(&record->position00, &record->halfSize18);
            else
                hit = player00->CalcLaserHitbox(
                    &record->position00,
                    &record->halfSize18,
                    &record->origin0C,
                    record->angle28,
                    0);
        }
        else
        {
            hit = player00->CalcCircleCollision(&record->position00, record->radius24);
        }

        if (hit)
            return record;
    }
    return NULL;
}


int PlayerLifecycleView::CalcItemCollectionCollision(
    PlayerPositionView *position,
    PlayerPositionView *size)
{
    if (updateState00 != 0 && updateState00 != 3)
        return 0;

    PlayerPositionView itemMin;
    PlayerPositionView itemMax;
    itemMin = *position - *size / 2.0f;
    itemMax = *position + *size / 2.0f;

    if (unknownBoundsMin1C90.x > itemMax.x ||
        unknownBoundsMax1C9C.x < itemMin.x ||
        unknownBoundsMin1C90.y > itemMax.y ||
        unknownBoundsMax1C9C.y < itemMin.y)
        return 0;
    return 1;
}
