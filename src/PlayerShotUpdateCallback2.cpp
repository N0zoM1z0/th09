#include "ZunTimer.hpp"

struct PlayerShotUpdate2Vec3
{
    float x;
    float y;
    float z;
};

struct PlayerShotUpdate2PlayerView
{
    unsigned char unknown0000[0x1B88];
    PlayerShotUpdate2Vec3 position1B88;
    unsigned char unknown1B94[0x3031C - 0x1B94];
    ZunTimer shotStateTimer3031C;
    void *activeShot30328;

    void *CreateRectType4(
        const PlayerShotUpdate2Vec3 *center,
        float width, float height,
        int value, int lifetime, int delay);
};

struct PlayerShotUpdate2ShotView
{
    unsigned char unknown000[0x1C];
    float vmField1C;
    unsigned char unknown020[0x2A4 - 0x20];
    PlayerShotUpdate2Vec3 position2A4;
    PlayerShotUpdate2Vec3 history2B0[32];
    PlayerShotUpdate2Vec3 hitboxSize430;
    PlayerShotUpdate2Vec3 velocity43C;
    float auxiliary448;
    float speed44C;
    float angle450;
    ZunTimer timer454;
    short damage460;
    short state462;
    short shotType464;
    short timeline466;
    short sourceOption468;
    short trailSegmentCount46A;
};

int __fastcall PlayerShotUpdateCallbackType2(
    PlayerShotUpdate2PlayerView *player,
    PlayerShotUpdate2ShotView *shot)
{
    ZunTimer *stateTimer = &player->shotStateTimer3031C;

    if (*stateTimer <= 0)
    {
        *stateTimer = 0;
        player->activeShot30328 = 0;
        shot->state462 = 0;
        return 1;
    }

    float velocityZ = shot->velocity43C.z;
    PlayerShotUpdate2Vec3 *position = &shot->position2A4;
    *position = player->position1B88;
    float newX = velocityZ + position->x;
    shot->position2A4.z = 0.44f;
    position->x = newX;
    shot->hitboxSize430.y = shot->position2A4.y;
    shot->vmField1C = shot->position2A4.y * (1.0f / 14.0f);
    shot->position2A4.y *= 0.5f;

    for (int i = 31; i > 0; --i)
    {
        shot->history2B0[i] = shot->history2B0[i - 1];
        shot->history2B0[i].y -= 1.0f;
    }

    for (int i = 0; i < shot->trailSegmentCount46A; ++i)
    {
        PlayerShotUpdate2Vec3 *point = &shot->history2B0[i * 2];
        if (point->x >= -900.0f)
            player->CreateRectType4(point, 4.0f, 448.0f, 1, 1, 0);
    }

    shot->history2B0[0] = *position;
    (*stateTimer)--;
    return 0;
}
