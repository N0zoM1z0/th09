struct PlayerShotSpawn3DescriptorView
{
    short scheduleValue00;
    unsigned char unknown02[0x36];
};

struct PlayerShotSpawn3ShotView
{
    unsigned char unknown000[0x43C];
    float velocityX43C;
    float velocityY440;
    float velocityZ444;
};

struct PlayerShotSpawn3PlayerView
{
    unsigned char unknown0000[0x1CDC];
    float speedMultiplier1CDC;
    float speedMultiplier1CE0;

    void __fastcall InitializeShot(
        PlayerShotSpawn3ShotView *shot,
        PlayerShotSpawn3DescriptorView *entry);
};

int __fastcall PlayerShotSpawnCallbackType3(
    PlayerShotSpawn3PlayerView *player,
    PlayerShotSpawn3ShotView *shot,
    int value,
    PlayerShotSpawn3DescriptorView *entry)
{
    if (value == entry->scheduleValue00)
    {
        player->InitializeShot(shot, entry);
        shot->velocityX43C = 0.0f;
        shot->velocityY440 = -24.0f;
        player->speedMultiplier1CE0 = 0.5f;
        player->speedMultiplier1CDC = 0.5f;
        return 1;
    }

    return 0;
}
