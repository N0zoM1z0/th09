#include "PlayerLifecycleView.hpp"

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
    return delta.x * delta.x + delta.y * delta.y < collisionRadius * collisionRadius;
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
