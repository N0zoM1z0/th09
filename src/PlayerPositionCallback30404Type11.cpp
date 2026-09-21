struct PlayerType11Point
{
    PlayerType11Point();
    PlayerType11Point operator+(const PlayerType11Point &other) const;
    PlayerType11Point operator-(const PlayerType11Point &other) const;
    PlayerType11Point operator/(float scalar) const;
    PlayerType11Point *FromAngleMagnitude(float angle, float magnitude);

    float x;
    float y;
    float z;
};

typedef char PlayerType11PointSizeIs0C[
    (sizeof(PlayerType11Point) == 0x0C) ? 1 : -1];

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
    PlayerType11Point position1B88;
};

float AddNormalizeAngle(float angle, float delta);

static int PlayerType11OrientationSign(
    const PlayerType11Point *a,
    const PlayerType11Point *b,
    const PlayerType11Point *c)
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
    const PlayerType11Point *other)
{
    PlayerType11Point delta = *other - player->position1B88;
    PlayerType11Point points[10];
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
        (PlayerType11OrientationSign(&points[4], &delta, &points[0]) >= 0 &&
         PlayerType11OrientationSign(&points[7], &delta, &points[4]) >= 0 &&
         PlayerType11OrientationSign(&points[0], &delta, &points[7]) >= 0) ||
        (PlayerType11OrientationSign(&points[2], &delta, &points[8]) >= 0 &&
         PlayerType11OrientationSign(&points[5], &delta, &points[2]) >= 0 &&
         PlayerType11OrientationSign(&points[8], &delta, &points[5]) >= 0) ||
        (PlayerType11OrientationSign(&points[0], &delta, &points[6]) >= 0 &&
         PlayerType11OrientationSign(&points[3], &delta, &points[0]) >= 0 &&
         PlayerType11OrientationSign(&points[6], &delta, &points[3]) >= 0);
}
