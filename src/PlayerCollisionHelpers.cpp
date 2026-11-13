#include "SoundPlayer.hpp"
#include "Supervisor.hpp"
#include "AsciiGameManagerView.hpp"
#include "PlayerLifecycleView.hpp"
#include "PlayerNativeCollisionSlots.hpp"
#include "BulletManager.hpp"
#include "EffectManager.hpp"
#include "RngRuntimeLeaves.hpp"
#include "ZunTimer.hpp"
#include "PlayerCollisionRegionCreate.hpp"

void RotatePlayerCollisionVector(
    PlayerPositionView *outVector,
    PlayerPositionView *point,
    float angle);


typedef char PlayerCancelGameManagerCoordinateAt358[
    (offsetof(AsciiGameManagerView, coordinate358) == 0x358) ? 1 : -1];

extern RngRuntimeView g_ReplayRng;
extern EffectManager *g_PlayerRewardEffectManager;


int PlayerLifecycleView::CheckBulletCollision(
    PlayerPositionView *position,
    PlayerPositionView *,
    Bullet *bullet)
{

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
    } scratch;
    float xDelta;
    float yDelta;
    register Bullet *bulletParameter = bullet;
    PlayerPositionView * volatile *positionArgument =
        reinterpret_cast<PlayerPositionView * volatile *>(&position);

    register PlayerCollisionRegionCreateRegionView **slot = activeB100;

    // Model the retail slot cursor as a loop-header destination; each
    // rejection retains its own native slot-advance edge.
    goto check_region;

check_region:
    if (*slot == NULL)
        goto no_collision;
    {
        const int regionType = (*slot)->type38;
        if (regionType != 1 && regionType != 2 && regionType != 3)
            goto next_invalid_region;
        if ((*slot)->delay40 > 0)
            goto next_invalid_region;

        if ((*slot)->radius08 != 0.0)
        {
            xDelta = (*positionArgument)->x - (*slot)->centerX00;
            yDelta = (*positionArgument)->y - (*slot)->centerY04;
            if (!(xDelta * xDelta + yDelta * yDelta <
                    (*slot)->radius08 * (*slot)->radius08))
                goto next_geometry_region;
        }
        else if ((*slot)->angle20 != 0.0f)
        {
            scratch.collision.delta.x = (*positionArgument)->x - (*slot)->centerX00;
            scratch.collision.delta.y = (*positionArgument)->y - (*slot)->centerY04;
            RotatePlayerCollisionVector(
                &scratch.collision.rotated, &scratch.collision.delta, -(*slot)->angle20);
            register float halfWidth = (*slot)->halfWidth10;
            scratch.collision.halfSize.y = (*slot)->halfHeight14;
            if (!(-halfWidth <= scratch.collision.rotated.x))
                goto next_geometry_region;
            if (!(scratch.collision.rotated.x <= halfWidth))
                goto next_geometry_region;
            if (!(-scratch.collision.halfSize.y <= scratch.collision.rotated.y))
                goto next_geometry_region;
            if (!(scratch.collision.rotated.y <= scratch.collision.halfSize.y))
                goto next_geometry_region;
        }
        else
        {
            register float minX = (*slot)->centerX00 - (*slot)->halfWidth10;
            scratch.collision.halfSize.y = (*slot)->centerY04 - (*slot)->halfHeight14;
            scratch.collision.boundsMax.x = (*slot)->halfWidth10 + (*slot)->centerX00;
            scratch.collision.boundsMax.y = (*slot)->halfHeight14 + (*slot)->centerY04;
            if (minX > position->x)
                goto next_geometry_region;
            if (scratch.collision.boundsMax.x < position->x)
                goto next_geometry_region;
            if (scratch.collision.halfSize.y > position->y)
                goto next_geometry_region;
            if (scratch.collision.boundsMax.y < position->y)
                goto next_geometry_region;
        }
        PlayerCollisionRegionCreateRegionView * volatile *regionSlot =
            reinterpret_cast<PlayerCollisionRegionCreateRegionView * volatile *>(slot);
        const int hitRegionType = (*regionSlot)->type38;
        if (hitRegionType == 2)
            goto cancel_hit;
        if (hitRegionType == 3)
            goto cancel_hit;
        {
            volatile int &hitCount = (*slot)->hitCount2C;
            ++hitCount;
        }
        return 2;
    }

cancel_hit:
    if (bulletParameter == NULL || bulletParameter->bulletType != 0)
        goto next_invalid_region;

    {
        scratch.reward.effectPosition.x =
            g_GameManager.TransformPopupX(position->x);
        scratch.reward.effectPosition.y =
            g_GameManager.TransformPopupY((*positionArgument)->y);
        scratch.reward.effectPosition.z = 0.0f;

        g_Supervisor.SelectSide(static_cast<int>(
            1u - static_cast<unsigned int>(this->sideIndex)));

        scratch.reward.velocity.x = g_ReplayRng.GetRandomF32SignedInRange(
            g_GameManager.coordinate358 * 0.5f - 8.0f);
        scratch.reward.velocity.y = g_ReplayRng.GetRandomF32InRange(128.0f);
        scratch.reward.velocity.z = 0.0f;

        // Retail forms the side-based reward effect id as a DWORD sum.
        Effect *effect =
            g_PlayerRewardEffectManager->SpawnEffectWithVelocity(
                static_cast<int>(
                    static_cast<unsigned int>(this->sideIndex) + 1u),
                &scratch.reward.effectPosition, &scratch.reward.velocity,
                1, static_cast<unsigned int>(-1));
        g_Supervisor.SelectSide(this->sideIndex);

        effect->cancel.bulletType = bulletParameter->bulletType;
        effect->cancel.color = bulletParameter->color;
        effect->cancel.bulletSpeed = bulletParameter->speed;
        effect->cancel.side =
            static_cast<short>(bulletParameter->controller->sideIndex);
        effect->cancel.extraAttribute = bulletParameter->extraAttribute10BD;

        this->AddRespawnResource(0.2f);
        g_SoundPlayer.PlaySoundByIdx(
            46, this->sideIndex != 0 ? 500 : -500);

        int rewardValue = this->ownerState30410.state38;
        if (rewardValue > 100)
            rewardValue = 1000;
        else
            rewardValue = static_cast<int>(
                static_cast<unsigned int>(rewardValue) * 10u);

        int rewardMode = (g_GameManager.difficulty11C >= 3) + 2;
        this->ownerState30410.ApplyReward(
            position, rewardMode, 2, 0, rewardValue);

        this->sideState->AddScore(50);
    }

    (*slot)->hitCount2C = static_cast<int>(
        static_cast<unsigned int>((*slot)->hitCount2C) + 1u);
    return 2;

no_collision:
    return 0;

next_invalid_region:
    slot = TH09_PLAYER_NATIVE_NEXT_REGION_SLOT(slot);
    goto check_region;

next_geometry_region:
    slot = TH09_PLAYER_NATIVE_NEXT_REGION_SLOT(slot);
    goto check_region;

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

    // Retail traverses the real 0x30-byte collision records as unsigned
    // Win32 byte addresses while keeping the original signed count gate.
    for (int i = 0; i < count1804;
         ++i, record = reinterpret_cast<PlayerCollisionQueryRecordView *>(
            reinterpret_cast<unsigned int>(record) + sizeof(*record)))
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
