#include <math.h>

struct PlayerShotSpawn7Vec3
{
    float x;
    float y;
    float z;
};

struct PlayerShotSpawn7DescriptorView
{
    short scheduleValue00;
    unsigned char unknown02[0x12];
    float angle14;
    float speed18;
    unsigned char unknown1C[0x1C];
};

struct PlayerShotSpawn7ShotView
{
    unsigned char unknown000[0x43C];
    float velocityX43C;
    float velocityY440;
    unsigned char unknown444[0x450 - 0x444];
    float angle450;
};

struct PlayerShotSpawn7PlayerView
{
    unsigned char unknown0000[0x30364];
    PlayerShotSpawn7Vec3 trackedEnemyPosition30364;

    float AngleFromPlayerToPoint(PlayerShotSpawn7Vec3 *position);
    void __fastcall InitializeShot(
        PlayerShotSpawn7ShotView *shot,
        PlayerShotSpawn7DescriptorView *entry);
};

int __fastcall PlayerShotSpawnCallbackType7(
    PlayerShotSpawn7PlayerView *player,
    PlayerShotSpawn7ShotView *shot,
    int value,
    PlayerShotSpawn7DescriptorView *entry)
{
    if (value == entry->scheduleValue00)
    {
        float angle =
            player->AngleFromPlayerToPoint(
                &player->trackedEnemyPosition30364) +
            entry->angle14;

        if (player->trackedEnemyPosition30364.x < -144.0f)
            angle = -1.5707964f;

        player->InitializeShot(shot, entry);

        shot->velocityX43C = cosf(angle) * entry->speed18;
        shot->velocityY440 = sinf(angle) * entry->speed18;
        shot->angle450 = angle;
        return 1;
    }

    return 0;
}
