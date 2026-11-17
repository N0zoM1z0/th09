#include "SoundPlayer.hpp"
#include "Supervisor.hpp"
#include "AsciiGameManagerView.hpp"
#include "PlayerLifecycleView.hpp"
#include "BulletManager.hpp"
#include "EffectManager.hpp"
#include "RngRuntimeLeaves.hpp"
#include "ZunTimer.hpp"
#include "PlayerCollisionRegionCreate.hpp"

void __fastcall RotatePlayerCollisionVector(
    PlayerPositionView *outVector,
    PlayerPositionView *point,
    float angle);


// The cancellation reward is a real Effect. Its +0xA0..+0xAB payload
// overlays other effect variants but is never a separate padded object.
typedef char PlayerCancelNativeEffectPayload[
    (offsetof(Effect, cancel) == 0xA0 &&
     offsetof(EffectCancelPayloadA0, bulletSpeed) == 0x00 &&
     offsetof(EffectCancelPayloadA0, bulletType) == 0x04 &&
     offsetof(EffectCancelPayloadA0, color) == 0x06 &&
     offsetof(EffectCancelPayloadA0, side) == 0x08 &&
     offsetof(EffectCancelPayloadA0, extraAttribute) == 0x0A) ? 1 : -1];

typedef char PlayerCancelGameManagerCoordinateAt358[
    (offsetof(AsciiGameManagerView, coordinate358) == 0x358) ? 1 : -1];

extern RngRuntimeView g_ReplayRng;
extern EffectManager *g_PlayerRewardEffectManager;


int PlayerLifecycleView::CheckBulletCollision(
    PlayerPositionView *position,
    PlayerPositionView *collisionSize,
    Bullet * volatile bullet)
{

    // Overlay geometry, reward payloads and the native cursor home in one frame.
    union CollisionScratch
    {
        struct CollisionVectors
        {
            PlayerPositionView boundsMax;
            PlayerPositionView halfSize;
            PlayerPositionView delta;
            PlayerPositionView rotated;
        } collision;
        struct RewardVectors
        {
            unsigned char collisionScratchPrefix[24];
            EffectFloat3 effectPosition;
            EffectFloat3 velocity;
        } reward;
        struct CursorHome
        {
            unsigned char collisionScratchPrefix[48];
            PlayerCollisionRegionCreateRegionView **slot;
        } cursor;
    } scratch;
    // Keep the native parameter/cursor homes observable across callbacks.
    PlayerPositionView *volatile &livePosition = position;
    volatile CollisionScratch &liveScratch = scratch;
    float xDelta;
    float yDelta;

    Bullet *currentBullet = bullet;
    PlayerCollisionRegionCreateRegionView **slot = activeB100;
    PlayerCollisionRegionCreateRegionView *region;
    liveScratch.cursor.slot = slot;

    for (; (region = *slot) != NULL; ++slot)
    {
        if (region->type38 != 1 &&
            region->type38 != 2 &&
            region->type38 != 3)
            continue;
        if (region->delay40 > 0)
            continue;

        if (region->radius08 != 0.0)
        {
            xDelta = livePosition->x - region->centerX00;
            yDelta = livePosition->y - region->centerY04;
            if (xDelta * xDelta + yDelta * yDelta <
                region->radius08 * region->radius08)
                goto hit;
            continue;
        }

        if (region->angle20 != 0.0f)
        {
            scratch.collision.delta.x = livePosition->x - region->centerX00;
            scratch.collision.delta.y = livePosition->y - region->centerY04;
            RotatePlayerCollisionVector(
                &scratch.collision.rotated, &scratch.collision.delta, -region->angle20);

            scratch.collision.halfSize.x = (*slot)->halfWidth10;
            scratch.collision.halfSize.y = (*slot)->halfHeight14;
            if (-scratch.collision.halfSize.x <= scratch.collision.rotated.x &&
                scratch.collision.rotated.x <= scratch.collision.halfSize.x &&
                -scratch.collision.halfSize.y <= scratch.collision.rotated.y &&
                scratch.collision.rotated.y <= scratch.collision.halfSize.y)
                goto hit;
            continue;
        }

        scratch.collision.halfSize.x = region->centerX00 - region->halfWidth10;
        scratch.collision.halfSize.y = region->centerY04 - region->halfHeight14;
        scratch.collision.boundsMax.x = region->halfWidth10 + region->centerX00;
        scratch.collision.boundsMax.y = region->halfHeight14 + region->centerY04;

        if (!(scratch.collision.halfSize.x > livePosition->x))
        {
            if (!(scratch.collision.boundsMax.x < livePosition->x))
            {
                if (!(scratch.collision.halfSize.y > livePosition->y))
                {
                    if (!(scratch.collision.boundsMax.y < livePosition->y))
                        goto hit;
                }
            }
        }
        continue;

hit:
        if ((*slot)->type38 == 2)
            goto cancel_hit;
        if ((*slot)->type38 == 3)
            goto cancel_hit;
        ++(*slot)->hitCount2C;
        return 2;

    cancel_hit:
        if (currentBullet == NULL || currentBullet->bulletType != 0)
            continue;

        {
            liveScratch.cursor.slot = slot;
            scratch.reward.effectPosition.x =
                g_GameManager.TransformPopupX(livePosition->x);
            scratch.reward.effectPosition.y =
                g_GameManager.TransformPopupY(livePosition->y);
            scratch.reward.effectPosition.z = 0.0f;

            g_Supervisor.SelectSide(1 - this->sideIndex);

            scratch.reward.velocity.x = g_ReplayRng.GetRandomF32SignedInRange(
                g_GameManager.coordinate358 * 0.5f - 8.0f);
            scratch.reward.velocity.y = g_ReplayRng.GetRandomF32InRange(128.0f);
            scratch.reward.velocity.z = 0.0f;

            Effect *effectBase =
                g_PlayerRewardEffectManager->SpawnEffectWithVelocity(
                    this->sideIndex + 1, &scratch.reward.effectPosition, &scratch.reward.velocity,
                    1, static_cast<unsigned int>(-1));
            g_Supervisor.SelectSide(this->sideIndex);

            effectBase->cancel.bulletType = currentBullet->bulletType;
            effectBase->cancel.color = currentBullet->color;
            effectBase->cancel.bulletSpeed = currentBullet->speed;
            effectBase->cancel.side =
                static_cast<short>(currentBullet->controller->sideIndex);
            effectBase->cancel.extraAttribute = currentBullet->extraAttribute10BD;

            this->AddRespawnResource(0.2f);
            g_SoundPlayer.PlaySoundByIdx(
                46, this->sideIndex != 0 ? 500 : -500);

            int rewardValue =
                this->ownerState30410.state38;
            if (rewardValue > 100)
                rewardValue = 1000;
            else
                rewardValue *= 10;

            int rewardMode =
                (g_GameManager.difficulty11C >= 3) + 2;
            this->ownerState30410.ApplyReward(
                livePosition,
                rewardMode, 2, 0, rewardValue);

            this->sideState->AddScore(50);
        }

        PlayerCollisionRegionCreateRegionView *hitRegion = *liveScratch.cursor.slot;
        ++hitRegion->hitCount2C;
        return 2;

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
    if (*reinterpret_cast<ZunTimer *>(&player->timer1B74) > 0)
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
