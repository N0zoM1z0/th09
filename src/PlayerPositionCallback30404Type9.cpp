#include "AsciiManager.hpp"
#include "PlayerLifecycleView.hpp"

#include <stddef.h>

struct PlayerType9RangeSourceView
{
    unsigned char unknown00[0x7C];
    float radius7C;
    float angle80;
};

struct PlayerPositionCallbackType9View
{
    unsigned char unknown00000[0x368];
    PlayerType9RangeSourceView *rangeSource368;
    unsigned char unknown0036C[0x1B88 - 0x36C];
    PlayerPositionView position1B88;
};

typedef char PlayerType9RangeSourceAt368[
    (offsetof(PlayerPositionCallbackType9View, rangeSource368) == 0x368) ? 1 : -1];
typedef char PlayerType9PositionAt1B88[
    (offsetof(PlayerPositionCallbackType9View, position1B88) == 0x1B88) ? 1 : -1];

float AddNormalizeAngle(float angle, float delta);

int __fastcall PlayerPositionCallback30404Type9(
    PlayerPositionCallbackType9View *player,
    const PlayerPositionView *other)
{
    PlayerPositionView delta = player->position1B88 - *other;
    float distanceSquared =
        delta.y * delta.y + delta.x * delta.x;

    if (distanceSquared <
        player->rangeSource368->radius7C *
            player->rangeSource368->radius7C)
        return 1;

    if (distanceSquared >
        (player->rangeSource368->radius7C + 24.0f) *
            (player->rangeSource368->radius7C + 24.0f))
        return 0;

    reinterpret_cast<Float3 *>(&delta)->FromAngleMagnitude(
        player->rangeSource368->angle80,
        player->rangeSource368->radius7C + 24.0f);
    delta += player->position1B88;
    delta = delta - *other;
    if (delta.y * delta.y + delta.x * delta.x < 576.0f)
        return 1;

    float angle = AddNormalizeAngle(player->rangeSource368->angle80, 1.0471976f);
    reinterpret_cast<Float3 *>(&delta)->FromAngleMagnitude(
        angle, player->rangeSource368->radius7C + 24.0f);
    delta += player->position1B88;
    delta = delta - *other;
    if (delta.y * delta.y + delta.x * delta.x < 576.0f)
        return 1;

    angle = AddNormalizeAngle(player->rangeSource368->angle80, 2.0943952f);
    reinterpret_cast<Float3 *>(&delta)->FromAngleMagnitude(
        angle, player->rangeSource368->radius7C + 24.0f);
    delta += player->position1B88;
    delta = delta - *other;
    if (delta.y * delta.y + delta.x * delta.x < 576.0f)
        return 1;

    angle = AddNormalizeAngle(player->rangeSource368->angle80, 3.1415927f);
    reinterpret_cast<Float3 *>(&delta)->FromAngleMagnitude(
        angle, player->rangeSource368->radius7C + 24.0f);
    delta += player->position1B88;
    delta = delta - *other;
    if (delta.y * delta.y + delta.x * delta.x < 576.0f)
        return 1;

    angle = AddNormalizeAngle(player->rangeSource368->angle80, 4.1887903f);
    reinterpret_cast<Float3 *>(&delta)->FromAngleMagnitude(
        angle, player->rangeSource368->radius7C + 24.0f);
    delta += player->position1B88;
    delta = delta - *other;
    if (delta.y * delta.y + delta.x * delta.x < 576.0f)
        return 1;

    angle = AddNormalizeAngle(player->rangeSource368->angle80, 5.2359877f);
    reinterpret_cast<Float3 *>(&delta)->FromAngleMagnitude(
        angle, player->rangeSource368->radius7C + 24.0f);
    delta += player->position1B88;
    delta = delta - *other;
    return delta.y * delta.y + delta.x * delta.x < 576.0f;
}
