struct PlayerShotSpawn6Vec3
{
    float x;
    float y;
    float z;
};

struct PlayerShotSpawn6EffectView
{
    unsigned char unknown00[0x0C];
    PlayerShotSpawn6Vec3 position0C;
};

struct PlayerShotSpawn6EffectManagerView
{
    unsigned char unknown00[0x30];
    void *effects30;
    PlayerShotSpawn6EffectView *GetFixedSlotEffect(int slot);
};

struct PlayerShotSpawn6SideView
{
    unsigned char unknown00[0x0C];
    PlayerShotSpawn6EffectManagerView *effectManager0C;
};

struct PlayerShotSpawn6DescriptorView
{
    short scheduleValue00;
    unsigned char unknown02[0x16];
    float speed18;
    unsigned char unknown1C[0x1C];
};

struct PlayerShotSpawn6ShotView
{
    unsigned char unknown000[0x2A4];
    PlayerShotSpawn6Vec3 position2A4;
    unsigned char unknown2B0[0x43C - 0x2B0];
    float velocityX43C;
    float velocityY440;
    unsigned char unknown444[0x450 - 0x444];
    float angle450;
};

struct PlayerShotSpawn6PlayerView
{
    unsigned char unknown0000[0x0C];
    PlayerShotSpawn6SideView *sideState0C;

    void __fastcall InitializeShot(
        PlayerShotSpawn6ShotView *shot,
        PlayerShotSpawn6DescriptorView *entry);
};

int __fastcall PlayerShotSpawnCallbackType6(
    PlayerShotSpawn6PlayerView *player,
    PlayerShotSpawn6ShotView *shot,
    int value,
    PlayerShotSpawn6DescriptorView *entry)
{
    if (value == entry->scheduleValue00)
    {
        player->InitializeShot(shot, entry);
        shot->position2A4 =
            player->sideState0C->effectManager0C->GetFixedSlotEffect(5)
                ->position0C;
        shot->angle450 = -1.5707964f;
        shot->velocityX43C = 0.0f;
        shot->velocityY440 = -entry->speed18;
        return 1;
    }
    return 0;
}
