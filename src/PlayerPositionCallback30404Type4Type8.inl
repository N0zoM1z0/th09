// Included once by EclManager.cpp with ordinary Float3 math definitions.
// The reference keeps the real offset receiver available for the center
// calculation. No separate point object or non-void angle API is introduced.
#include "AsciiManager.hpp"
#include "PlayerLifecycleView.hpp"
#include "RuntimeAngleMath.hpp"

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

typedef char PlayerType48RadiusAt7C[
    (offsetof(PlayerType4RangeSourceView, radius7C) == 0x7C) ? 1 : -1];
typedef char PlayerType48AngleAt80[
    (offsetof(PlayerType4RangeSourceView, angle80) == 0x80) ? 1 : -1];
typedef char PlayerType48RangeSourceAt368[
    (offsetof(PlayerPositionCallbackType4View, rangeSource368) == 0x368) ? 1 : -1];
typedef char PlayerType48PositionAt1B88[
    (offsetof(PlayerPositionCallbackType4View, position1B88) == 0x1B88) ? 1 : -1];

int __fastcall PlayerPositionCallback30404Type4(
    PlayerPositionCallbackType4View *player,
    const Float3 *other)
{
    Float3 delta = *other - player->position1B88;

    float angle = AddNormalizeAngle(
        player->rangeSource368->angle80, 1.5707964f);
    Float3 offsetValue;
    Float3 &offset = offsetValue;
    offset.FromAngleMagnitude(
        angle, player->rangeSource368->radius7C * 0.5f);

    Float3 center;
    if (delta.x * offset.x + delta.y * offset.y > 0.0f)
        center = player->position1B88 - offset;
    else
        center = player->position1B88 + offset;

    delta = center - *other;
    float radius = player->rangeSource368->radius7C;
    return delta.x * delta.x + delta.y * delta.y < radius * radius;
}

int __fastcall PlayerPositionCallback30404Type8(
    PlayerPositionCallbackType4View *player,
    const Float3 *other)
{
    Float3 delta = *other - player->position1B88;

    float angle = AddNormalizeAngle(
        player->rangeSource368->angle80, 1.5707964f);
    Float3 offsetValue;
    Float3 &offset = offsetValue;
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
