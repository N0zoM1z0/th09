#include "ZunTimer.hpp"

struct PlayerShotSpawn1Vec3
{
    float x;
    float y;
    float z;
};

struct PlayerShotSpawn1DescriptorView
{
    short scheduleValue00;
    unsigned char unknown02[0x02];
    float value04;
    float value08;
    unsigned char unknown0C[0x14];
    short sourceOption20;
    unsigned char unknown22[0x16];
};

struct PlayerShotSpawn1ShotView
{
    unsigned char unknown000[0x2A4];
    PlayerShotSpawn1Vec3 position2A4;
    PlayerShotSpawn1Vec3 trail2B0[32];
    PlayerShotSpawn1Vec3 hitbox430;
    PlayerShotSpawn1Vec3 velocity43C;
    float auxiliary448;
    unsigned char unknown44C[0x464 - 0x44C];
    short shotType464;
    unsigned char unknown466[0x468 - 0x466];
    short sourceOption468;
    short field46A;
    unsigned char unknown46C[0x484 - 0x46C];
};

struct PlayerShotSpawn1StateView
{
    ZunTimer timer00;
    PlayerShotSpawn1ShotView *shot0C;
    unsigned char unknown10[0x0C];
};

struct PlayerShotSpawn1PlayerView
{
    unsigned char unknown0000[0x3031C];
    PlayerShotSpawn1StateView state3031C;

    void __fastcall InitializeShot(
        PlayerShotSpawn1ShotView *shot,
        PlayerShotSpawn1DescriptorView *entry);
};

int __fastcall PlayerShotSpawnCallbackType1(
    PlayerShotSpawn1PlayerView *player,
    PlayerShotSpawn1ShotView *shot,
    int value,
    PlayerShotSpawn1DescriptorView *entry)
{
    if (value != entry->scheduleValue00)
        return 0;

    player->state3031C.timer00 = 30;
    player->state3031C.shot0C = shot;

    shot->sourceOption468 = entry->sourceOption20;
    shot->velocity43C.z = entry->value04;
    shot->auxiliary448 = entry->value08;
    shot->field46A = 15;

    player->InitializeShot(shot, entry);
    shot->shotType464 = 2;

    for (int i = 31; i >= 0; --i)
        shot->trail2B0[i].x = -999.0f;
    shot->position2A4.x = -999.0f;

    return 1;
}
