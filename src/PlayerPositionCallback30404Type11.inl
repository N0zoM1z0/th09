// Lexical member of EclManager.cpp. Ordinary Float3 bodies select the
// target temporary lifetimes; the constructor callback owner stays unknown.
#include "AsciiManager.hpp"
#include "RuntimeAngleMath.hpp"

struct PlayerType11RangeSourceView
{
    unsigned char unknown00[0x7C];
    float radius7C;
    float angle80;
};

struct PlayerPositionCallbackType11View
{
    unsigned char unknown00000[0x368];
    PlayerType11RangeSourceView *rangeSource368;
    unsigned char unknown0036C[0x1B88 - 0x36C];
    Float3 position1B88;
};

typedef char PlayerType11RadiusAt7C[
    (offsetof(PlayerType11RangeSourceView, radius7C) == 0x7C) ? 1 : -1];
typedef char PlayerType11AngleAt80[
    (offsetof(PlayerType11RangeSourceView, angle80) == 0x80) ? 1 : -1];
typedef char PlayerType11RangeSourceAt368[
    (offsetof(PlayerPositionCallbackType11View, rangeSource368) == 0x368) ? 1 : -1];
typedef char PlayerType11PositionAt1B88[
    (offsetof(PlayerPositionCallbackType11View, position1B88) == 0x1B88) ? 1 : -1];

static int PlayerType11OrientationSign(
    const Float3 *a,
    const Float3 *b,
    const Float3 *c)
{
    float value =
        (c->y - a->y) * b->x +
        (a->y - b->y) * c->x +
        (b->y - c->y) * a->x;

    if (value > 0.0f)
        return 1;
    if (value < 0.0f)
        return -1;
    return 0;
}

int __fastcall PlayerPositionCallback30404Type11(
    PlayerPositionCallbackType11View *player,
    const Float3 *other)
{
    Float3 delta = *other - player->position1B88;
    Float3 points[10];
    PlayerType11RangeSourceView *source = player->rangeSource368;

    float angle = source->angle80;
    points[0].FromAngleMagnitude(angle, source->radius7C);
    angle = AddNormalizeAngle(angle, 1.2566371f);
    points[2].FromAngleMagnitude(angle, source->radius7C);
    angle = AddNormalizeAngle(angle, 1.2566371f);
    points[4].FromAngleMagnitude(angle, source->radius7C);
    angle = AddNormalizeAngle(angle, 1.2566371f);
    points[6].FromAngleMagnitude(angle, source->radius7C);
    angle = AddNormalizeAngle(angle, 1.2566371f);
    points[8].FromAngleMagnitude(angle, source->radius7C);

    points[1] = (points[0] - points[4]) / 1.618f + points[4];
    points[3] = (points[2] - points[6]) / 1.618f + points[6];
    points[5] = (points[4] - points[8]) / 1.618f + points[8];
    points[7] = (points[6] - points[0]) / 1.618f + points[0];
    points[9] = (points[8] - points[2]) / 1.618f + points[2];

    return
        (PlayerType11OrientationSign(&delta, &points[0], &points[4]) >= 0 &&
         PlayerType11OrientationSign(&delta, &points[4], &points[7]) >= 0 &&
         PlayerType11OrientationSign(&delta, &points[7], &points[0]) >= 0) ||
        (PlayerType11OrientationSign(&delta, &points[8], &points[2]) >= 0 &&
         PlayerType11OrientationSign(&delta, &points[2], &points[5]) >= 0 &&
         PlayerType11OrientationSign(&delta, &points[5], &points[8]) >= 0) ||
        (PlayerType11OrientationSign(&delta, &points[6], &points[0]) >= 0 &&
         PlayerType11OrientationSign(&delta, &points[0], &points[3]) >= 0 &&
         PlayerType11OrientationSign(&delta, &points[3], &points[6]) >= 0);
}
