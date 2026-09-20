#include "PlayerLifecycleView.hpp"

#include <stddef.h>

struct PlayerPredicateRangeSourceView
{
    unsigned char unknown00[0x7C];
    float radius7C;
};

struct PlayerPositionPredicateView
{
    unsigned char unknown00000[0x368];
    PlayerPredicateRangeSourceView *rangeSource368;
    unsigned char unknown0036C[0x1B88 - 0x36C];
    PlayerPositionView position1B88;
};

int __fastcall PlayerPositionCallback30404Type6(
    PlayerPositionPredicateView *player,
    const PlayerPositionView *other)
{
    PlayerPositionView delta = player->position1B88 - *other;
    float radius = player->rangeSource368->radius7C;
    return delta.x * delta.x < radius * radius;
}

int __fastcall PlayerPositionCallback30404Type7(
    PlayerPositionPredicateView *player,
    const PlayerPositionView *other)
{
    PlayerPositionView delta = player->position1B88 - *other;
    PlayerPredicateRangeSourceView *source = player->rangeSource368;
    float y = delta.y - source->radius7C * 1.5f;
    return delta.x * delta.x + y * y <
        source->radius7C * source->radius7C;
}

int __fastcall PlayerPositionCallback30404Type14(
    PlayerPositionPredicateView *player,
    const PlayerPositionView *other)
{
    PlayerPositionView delta = player->position1B88 - *other;
    float radius = player->rangeSource368->radius7C;
    return delta.y * delta.y < radius * radius;
}

int __fastcall PlayerPositionCallback30404Type15(
    PlayerPositionPredicateView *player,
    const PlayerPositionView *other)
{
    PlayerPositionView delta = player->position1B88 - *other;
    if (delta.x * delta.x <
            player->rangeSource368->radius7C *
                player->rangeSource368->radius7C ||
        delta.y * delta.y <
            player->rangeSource368->radius7C *
                player->rangeSource368->radius7C)
        return 1;
    return 0;
}
