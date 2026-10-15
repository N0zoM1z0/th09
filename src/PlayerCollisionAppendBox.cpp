#include "PlayerLifecycleView.hpp"
#include "AsciiManager.hpp"
#include "EnemyManager.hpp"
#include "EnemyAppendCollisionView.hpp"

// TH09: keeping this exact 205-byte callee visible to the adjacent Enemy
// wrapper reproduces its exact 82-byte Float3 return-value copy scheduling.
// This is compiler-visible source ownership evidence, not proof of the
// original C++ translation-unit boundaries. Both bodies are defined once.
void PlayerCollisionQueryStateView::AppendBoxRecord(
    const PlayerPositionView *position,
    const PlayerPositionView *size,
    int unknown)
{
    if (count1804 >= 128)
        return;

    records[count1804].position00 = *position;
    records[count1804].halfSize18 = *size;
    records[count1804].halfSize18.x *= 0.5f;
    records[count1804].halfSize18.y *= 0.5f;
    records[count1804].radius24 = 0.0f;
    records[count1804].angle28 = 0.0f;
    records[count1804].unknown2C = unknown;
    ++count1804;
}

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


void EnemyAppendCollisionView::AppendPlayerCollisionBox(
    const Float3 *position,
    const Float3 *size)
{
    Float3 scaledSize;
    scaledSize = *size / 1.5f;

    reinterpret_cast<EnemyCollisionSideView *>(
        manager00->sideState320)->player04->collisionQuery36C
        .AppendBoxRecord(
            reinterpret_cast<const PlayerPositionView *>(position),
            reinterpret_cast<const PlayerPositionView *>(&scaledSize),
            0);
}
