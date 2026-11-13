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

    PlayerPositionView delta;
    PlayerPositionView rotated;
    PlayerPositionView halfSize;
    PlayerPositionView boundsMax;
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
            delta.x = (*positionArgument)->x - (*slot)->centerX00;
            delta.y = (*positionArgument)->y - (*slot)->centerY04;
            RotatePlayerCollisionVector(
                &rotated, &delta, -(*slot)->angle20);
            halfSize.x = (*slot)->halfWidth10;
            halfSize.y = (*slot)->halfHeight14;
            if (!(-halfSize.x <= rotated.x))
                goto next_geometry_region;
            if (!(rotated.x <= halfSize.x))
                goto next_geometry_region;
            if (!(-halfSize.y <= rotated.y))
                goto next_geometry_region;
            if (!(rotated.y <= halfSize.y))
                goto next_geometry_region;
        }
        else
        {
            halfSize.x = (*slot)->centerX00 - (*slot)->halfWidth10;
            halfSize.y = (*slot)->centerY04 - (*slot)->halfHeight14;
            boundsMax.x = (*slot)->halfWidth10 + (*slot)->centerX00;
            boundsMax.y = (*slot)->halfHeight14 + (*slot)->centerY04;
            if (halfSize.x > (*positionArgument)->x)
                goto next_geometry_region;
            if (boundsMax.x < (*positionArgument)->x)
                goto next_geometry_region;
            if (halfSize.y > (*positionArgument)->y)
                goto next_geometry_region;
            if (boundsMax.y < (*positionArgument)->y)
                goto next_geometry_region;
        }
        if ((*slot)->type38 == 2)
            goto cancel_hit;
        if ((*slot)->type38 == 3)
            goto cancel_hit;
        {
            volatile int &hitCount = (*slot)->hitCount2C;
            ++hitCount;
        }
        return 2;
    }

cancel_hit:
    if (bulletParameter == NULL || bulletParameter->bulletType != 0)
        goto next_cancel_region;

    {
        EffectFloat3 effectPosition;
        effectPosition.x =
            g_GameManager.TransformPopupX((*positionArgument)->x);
        effectPosition.y =
            g_GameManager.TransformPopupY((*positionArgument)->y);
        effectPosition.z = 0.0f;

        g_Supervisor.SelectSide(static_cast<int>(
            1u - static_cast<unsigned int>(this->sideIndex)));

        EffectFloat3 velocity;
        velocity.x = g_ReplayRng.GetRandomF32SignedInRange(
            g_GameManager.coordinate358 * 0.5f - 8.0f);
        velocity.y = g_ReplayRng.GetRandomF32InRange(128.0f);
        velocity.z = 0.0f;

        // Retail forms the side-based reward effect id as a DWORD sum.
        Effect *effect =
            g_PlayerRewardEffectManager->SpawnEffectWithVelocity(
                static_cast<int>(
                    static_cast<unsigned int>(this->sideIndex) + 1u),
                &effectPosition, &velocity,
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
            *positionArgument, rewardMode, 2, 0, rewardValue);

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

next_cancel_region:
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
