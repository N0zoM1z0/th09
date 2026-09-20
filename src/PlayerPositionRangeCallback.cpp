#include "PlayerLifecycleView.hpp"

#include <stddef.h>

struct PlayerRangeSourceView
{
    unsigned char unknown00[0x7C];
    float radius7C;
};

struct PlayerPositionRangeCallbackView
{
    unsigned char unknown00000[0x368];
    PlayerRangeSourceView *rangeSource368;
    unsigned char unknown0036C[0x1B88 - 0x36C];
    PlayerPositionView position1B88;
};

typedef char PlayerPositionRangeSourceAt368[
    (offsetof(PlayerPositionRangeCallbackView, rangeSource368) == 0x368) ? 1 : -1];
typedef char PlayerPositionRangePositionAt1B88[
    (offsetof(PlayerPositionRangeCallbackView, position1B88) == 0x1B88) ? 1 : -1];

int __fastcall PlayerPositionCallbackWithinRange(
    PlayerPositionRangeCallbackView *player,
    const PlayerPositionView *other)
{
    PlayerPositionView delta = player->position1B88 - *other;
    float radius = player->rangeSource368->radius7C;
    return delta.x * delta.x + delta.y * delta.y < radius * radius;
}
