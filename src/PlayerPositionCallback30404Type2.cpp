#include "PlayerLifecycleView.hpp"

#include <stddef.h>

struct PlayerType2RangeSourceView
{
    unsigned char unknown00[0x7C];
    float radius7C;
    float angle80;
    float width84;
};

struct PlayerPositionCallbackType2View
{
    unsigned char unknown00000[0x368];
    PlayerType2RangeSourceView *rangeSource368;
    unsigned char unknown0036C[0x1B88 - 0x36C];
    PlayerPositionView position1B88;
};

struct PlayerForwardAngleFloat3View
{
    float x;
    float y;
    float z;
};

struct PlayerForwardAngleView
{
    unsigned char unknown000[0x1B88];
    PlayerForwardAngleFloat3View position1B88;

    float AngleFromPlayerToPoint(PlayerForwardAngleFloat3View *position);
};

long double __stdcall AnmProjectionAbs(float value);

int __fastcall PlayerPositionCallback30404Type2(
    PlayerPositionCallbackType2View *player,
    const PlayerPositionView *other)
{
    PlayerPositionView delta = player->position1B88 - *other;
    PlayerType2RangeSourceView *source = player->rangeSource368;
    float radius = source->radius7C;

    if (delta.x * delta.x + delta.y * delta.y < radius * radius)
    {
        float angle =
            reinterpret_cast<PlayerForwardAngleView *>(player)
                ->AngleFromPlayerToPoint(
                    reinterpret_cast<PlayerForwardAngleFloat3View *>(
                        const_cast<PlayerPositionView *>(other))) -
            source->angle80;

        if (static_cast<float>(AnmProjectionAbs(angle)) > 3.1415927f)
        {
            if (angle > 0.0f)
                angle -= 6.2831855f;
            else
                angle += 6.2831855f;
        }

        return AnmProjectionAbs(angle) <= source->width84 * 0.5f;
    }

    return 0;
}
