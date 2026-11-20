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

typedef char PlayerCollisionActiveRegionCursorCountIs514[
    (sizeof(((PlayerLifecycleView *)0)->activeB100) /
         sizeof(((PlayerLifecycleView *)0)->activeB100[0]) == 514) ? 1 : -1];

extern RngRuntimeView g_ReplayRng;
extern EffectManager *g_PlayerRewardEffectManager;

// The native collision scan accepts kinds 1, 2 and 3. Kind 1 reports a
// direct hit, while kinds 2 and 3 take the bullet-cancellation path when the
// bullet's sprite discriminator is zero. Original names are unknown.
enum PlayerCollisionRegionKind
{
    PLAYER_REGION_DIRECT_HIT = 1,
    PLAYER_REGION_BULLET_CANCEL_2 = 2,
    PLAYER_REGION_BULLET_CANCEL_3 = 3
};


int PlayerLifecycleView::CheckBulletCollision(
    PlayerPositionView *position,
    PlayerPositionView *collisionSize,
    Bullet *bullet)
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
        // Geometry's first two vectors share the native 24-byte prefix
        // with reward coordinates; these are the same stack storage.
        struct RewardVectors
        {
            PlayerPositionView boundsMax;
            PlayerPositionView halfSize;
            EffectFloat3 effectPosition;
            EffectFloat3 velocity;
        } reward;
        // The cursor home follows all four physical geometry vectors.
        struct CursorHome
        {
            CollisionVectors frame;
            PlayerCollisionRegionCreateRegionView **slot;
        } cursor;
    } scratch;
    typedef char NativeCollisionScratchLayout[
        (sizeof(CollisionScratch) == 52 &&
         offsetof(CollisionScratch, reward.effectPosition) == 24 &&
         offsetof(CollisionScratch, reward.velocity) == 36 &&
         offsetof(CollisionScratch, cursor.slot) == 48) ? 1 : -1];
    // The native cursor home overlays the geometry/reward frame directly.

    PlayerCollisionRegionCreateRegionView **slot = activeB100;
    PlayerCollisionRegionCreateRegionView *region;
    scratch.cursor.slot = slot;

    do
    {
        for (;;)
        {
            region = *slot;
            if (region == NULL)
                return 0;
            if (region->type38 != PLAYER_REGION_DIRECT_HIT &&
                region->type38 != PLAYER_REGION_BULLET_CANCEL_2 &&
                region->type38 != PLAYER_REGION_BULLET_CANCEL_3)
                goto next_region;
            if (region->collisionDelay40 > 0)
                goto next_region;
            break;
        }

        volatile float &radius = region->radius08;
        if (radius != 0.0)
        {
            float xDelta = position->x - region->centerX00;
            float yDelta = position->y - region->centerY04;
            if (!(xDelta * xDelta + yDelta * yDelta <
                  region->radius08 * region->radius08))
                goto next_region;
        }
        else if (region->angle20 != 0.0f)
        {
            scratch.collision.delta.x = position->x - region->centerX00;
            scratch.collision.delta.y = position->y - region->centerY04;
            RotatePlayerCollisionVector(
                &scratch.collision.rotated, &scratch.collision.delta,
                -region->angle20);
            scratch.collision.halfSize.x = (*slot)->halfWidth10;
            scratch.collision.halfSize.y = (*slot)->halfHeight14;
            if (-scratch.collision.halfSize.x > scratch.collision.rotated.x ||
                scratch.collision.rotated.x > scratch.collision.halfSize.x ||
                -scratch.collision.halfSize.y > scratch.collision.rotated.y ||
                scratch.collision.rotated.y > scratch.collision.halfSize.y)
                goto next_region;
        }
        else
        {
            scratch.collision.halfSize.x =
                region->centerX00 - region->halfWidth10;
            scratch.collision.halfSize.y =
                region->centerY04 - region->halfHeight14;
            scratch.collision.boundsMax.x =
                region->halfWidth10 + region->centerX00;
            scratch.collision.boundsMax.y =
                region->halfHeight14 + region->centerY04;
            if (scratch.collision.halfSize.x > position->x ||
                scratch.collision.boundsMax.x < position->x ||
                scratch.collision.halfSize.y > position->y ||
                scratch.collision.boundsMax.y < position->y)
                goto next_region;
        }

        {
            PlayerCollisionRegionCreateRegionView * volatile hitRegion = *slot;
            const int hitType = hitRegion->type38;
            // Both cancellation region kinds share the bullet test; the
            // remaining kind is the direct-hit counter path.
            if (hitType == PLAYER_REGION_BULLET_CANCEL_2)
                goto cancel_bullet;
            if (hitType == PLAYER_REGION_BULLET_CANCEL_3)
                goto cancel_bullet;
            hitRegion->hitCount2C = static_cast<int>(
                static_cast<unsigned int>(hitRegion->hitCount2C) + 1u);
            return 2;
        cancel_bullet:
            if (bullet != NULL && bullet->bulletType == 0)
                break;
            goto next_region;
        }

next_region:
        ++slot;
    } while (true);

    // Cancellation uses the original position argument after the scan.
    // Cancellation leaves the region scan before creating the reward effect.
{
        scratch.cursor.slot = slot;
        scratch.reward.effectPosition.x =
            g_GameManager.TransformPopupX(position->x);
        scratch.reward.effectPosition.y =
            g_GameManager.TransformPopupY(position->y);
        scratch.reward.effectPosition.z = 0.0f;

        // Retail subtracts the signed side from a native i386 DWORD, wrapping
        // through zero without introducing signed C++ overflow.
        g_Supervisor.SelectSide(static_cast<int>(
            1u - static_cast<unsigned int>(this->sideIndex)));

        scratch.reward.velocity.x = g_ReplayRng.GetRandomF32SignedInRange(
            g_GameManager.coordinate358 * 0.5f - 8.0f);
        scratch.reward.velocity.y = g_ReplayRng.GetRandomF32InRange(128.0f);
        scratch.reward.velocity.z = 0.0f;

        Effect *effectBase =
            g_PlayerRewardEffectManager->SpawnEffectWithVelocity(
                // A native i386 INC supplies the effect side, including DWORD wrap.
                static_cast<int>(static_cast<unsigned int>(this->sideIndex) + 1u),
                &scratch.reward.effectPosition, &scratch.reward.velocity,
                1, static_cast<unsigned int>(-1));
        g_Supervisor.SelectSide(this->sideIndex);

        effectBase->cancel.bulletType = bullet->bulletType;
        effectBase->cancel.color = bullet->color;
        effectBase->cancel.bulletSpeed = bullet->speed;
        effectBase->cancel.side =
            static_cast<short>(bullet->controller->sideIndex);
        effectBase->cancel.extraAttribute = bullet->extraAttribute10BD;

        this->AddRespawnResource(0.2f);
        g_SoundPlayer.PlaySoundByIdx(
            46, this->sideIndex != 0 ? 500 : -500);

        int rewardValue =
            this->ownerState30410.state38;
        if (rewardValue > 100)
            rewardValue = 1000;
        else
            // Retail uses a DWORD LEA/SHL, so multiplication wraps
            // before the signed reward value is forwarded.
            rewardValue = static_cast<int>(
                static_cast<unsigned int>(rewardValue) * 10u);

        int rewardMode =
            (g_GameManager.difficulty11C >= 3) + 2;
        this->ownerState30410.ApplyReward(
            position,
            rewardMode, 2, 0, rewardValue);

        this->sideState->AddScore(50);
    }

    PlayerCollisionRegionCreateRegionView *hitRegion = *scratch.cursor.slot;
    hitRegion->hitCount2C = static_cast<int>(
        static_cast<unsigned int>(hitRegion->hitCount2C) + 1u);
    return 2;
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
    if (player->timer1B74 > 0)
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
