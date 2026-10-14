#include "PlayerLifecycleView.hpp"

#include <stddef.h>

struct PlayerCollisionTimerView
{
    int previous;
    float subFrame;
    int current;

    unsigned int IsAfter(int value);
};

typedef char PlayerCollisionTimerSizeIsC[
    (sizeof(PlayerCollisionTimerView) == 0x0C) ? 1 : -1];

PlayerCollisionQueryRecordView *PlayerCollisionQueryStateView::FindCollision(
    const PlayerPositionView &center,
    const PlayerPositionView &halfSize,
    float extraRadius)
{
    PlayerLifecycleView *player = player00;
    if (player->GetUpdateState() != 0)
        return NULL;
    if (reinterpret_cast<PlayerCollisionTimerView *>(&player->timer1B74)->IsAfter(0))
        return NULL;

    PlayerCollisionQueryRecordView *record = records;
    PlayerPositionView *position = &player->position1B88;
    PlayerPositionView previousPosition = *position;
    PlayerHalfSizeView previousHalfSize = player->hurtboxHalfSize;

    *position = center;
    player00->collisionBoundsMin1C60 = center - halfSize;
    player00->collisionBoundsMax1C6C = center + halfSize;
    player00->hurtboxHalfSize = halfSize;

    for (int i = 0; i < count1804; ++i, ++record)
    {
        if (record->radius24 == 0.0f)
        {
            if (record->angle28 == 0.0f)
            {
                if (player00->CalcItemBoxCollision(&record->position00, &record->halfSize18))
                {
                    player00->position1B88 = previousPosition;
                    player00->hurtboxHalfSize = previousHalfSize;
                    return record;
                }
            }
            else if (player00->CalcLaserHitbox(
                         &record->position00,
                         &record->halfSize18,
                         &record->origin0C,
                         record->angle28,
                         0))
            {
                player00->position1B88 = previousPosition;
                player00->hurtboxHalfSize = previousHalfSize;
                return record;
            }
        }
        else if (player00->CalcCircleCollision(
                     &record->position00, extraRadius + record->radius24))
        {
            player00->position1B88 = previousPosition;
            player00->hurtboxHalfSize = previousHalfSize;
            return record;
        }
    }

    player00->position1B88 = previousPosition;
    player00->hurtboxHalfSize = previousHalfSize;
    return NULL;
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
