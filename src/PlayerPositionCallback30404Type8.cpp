#include "AsciiManager.hpp"
#include "PlayerLifecycleView.hpp"

struct PlayerType4RangeSourceView
{
    unsigned char unknown00[0x7C];
    float radius7C;
    float angle80;
};

struct PlayerPositionCallbackType4View
{
    unsigned char unknown00000[0x368];
    PlayerType4RangeSourceView *rangeSource368;
    unsigned char unknown0036C[0x1B88 - 0x36C];
    Float3 position1B88;
};

float AddNormalizeAngle(float angle, float delta);

int __fastcall PlayerPositionCallback30404Type8(
    PlayerPositionCallbackType4View *player,
    const Float3 *other)
{
    Float3 delta = *other - player->position1B88;

    float angle = AddNormalizeAngle(
        player->rangeSource368->angle80, 1.5707964f);
    Float3 offset;
    offset.FromAngleMagnitude(
        angle, player->rangeSource368->radius7C * 0.86602539f);

    Float3 center;
    if (delta.x * offset.x + delta.y * offset.y > 0.0f)
        center = player->position1B88 - offset;
    else
        center = player->position1B88 + offset;

    delta = center - *other;
    float radius = player->rangeSource368->radius7C;
    return delta.x * delta.x + delta.y * delta.y < radius * radius;
}
