#include "PlayerLifecycleView.hpp"

#include <stddef.h>

struct PlayerType1RangeSourceView
{
    unsigned char unknown00[0x84];
    float scale84;
};

struct PlayerPositionCallbackType1View
{
    unsigned char unknown00000[0x368];
    PlayerType1RangeSourceView *rangeSource368;
    unsigned char unknown0036C[0x1B88 - 0x36C];
    PlayerPositionView position1B88;
};

int __fastcall PlayerPositionCallback30404Type1(
    PlayerPositionCallbackType1View *player,
    const PlayerPositionView *other)
{
    float playerY = player->position1B88.y + 32.0f;
    if (playerY <= other->y)
        return 0;

    float shiftedY = other->y - (playerY - 448.0f);
    float deltaX = other->x - player->position1B88.x;

    float limit =
        (200704.0f - shiftedY * shiftedY) *
        player->rangeSource368->scale84 *
        player->rangeSource368->scale84;
    if (limit < 196.0f)
        limit = 196.0f;

    return deltaX * deltaX <= limit;
}
