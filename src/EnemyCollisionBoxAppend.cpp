#include "EnemyManager.hpp"
#include "PlayerLifecycleView.hpp"

struct EnemyCollisionPlayerView
{
    unsigned char unknown000[0x36C];
    PlayerCollisionQueryStateView collisionQuery36C;
};

struct EnemyCollisionSideView
{
    unsigned char unknown00[0x04];
    EnemyCollisionPlayerView *player04;
};

struct EnemyAppendCollisionView
{
    EnemyManagerView *manager00;

    void AppendPlayerCollisionBox(
        const PlayerPositionView *position,
        const PlayerPositionView *size);
};

void EnemyAppendCollisionView::AppendPlayerCollisionBox(
    const PlayerPositionView *position,
    const PlayerPositionView *size)
{
    PlayerPositionView scaledSize;
    scaledSize = *size / 1.5f;

    reinterpret_cast<EnemyCollisionSideView *>(
        manager00->sideState320)->player04->collisionQuery36C
        .AppendBoxRecord(position, &scaledSize, 0);
}
