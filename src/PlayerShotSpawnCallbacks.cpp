struct PlayerShotSpawnPlayerView;
struct PlayerShotSpawnShotView;
struct PlayerShotSpawnDescriptorView;

struct PlayerShotSpawnVec3
{
    float x;
    float y;
    float z;
};

struct PlayerShotSpawnEffectView
{
    unsigned char unknown000[0xC4];
    unsigned char activeC4;
    unsigned char unknownC5[0xD8 - 0xC5];
};

struct PlayerShotSpawnEffectManagerView
{
    unsigned char unknown00[0x30];
    PlayerShotSpawnEffectView *effects30;

    PlayerShotSpawnEffectView *GetFixedSlotEffect(int slot);
    PlayerShotSpawnEffectView *SpawnEffectInFixedSlot(
        int effectId, const PlayerShotSpawnVec3 *position,
        int slotIndex, unsigned int color);
};

struct PlayerShotSpawnSideView
{
    unsigned char unknown00[0x0C];
    PlayerShotSpawnEffectManagerView *effectManager0C;
};

struct PlayerShotSpawnDescriptorView
{
    short scheduleValue00;
    unsigned char unknown02[0x12];
    float angle14;
    unsigned char unknown18[0x20];
};

struct PlayerShotSpawnShotView
{
    unsigned char unknown000[0x43C];
    PlayerShotSpawnVec3 velocity43C;
    unsigned char unknown448[0x450 - 0x448];
    float angle450;
    unsigned char unknown454[0x484 - 0x454];
};

struct PlayerShotSpawnPlayerView
{
    unsigned char unknown0000[0x0C];
    PlayerShotSpawnSideView *sideState0C;
    unsigned char unknown0010[0x1B88 - 0x10];
    PlayerShotSpawnVec3 position1B88;

    void __fastcall InitializeShot(
        PlayerShotSpawnShotView *shot,
        PlayerShotSpawnDescriptorView *entry);
};

int __fastcall PlayerShotSpawnCallbackType2(
    PlayerShotSpawnPlayerView *player,
    PlayerShotSpawnShotView *shot,
    int value,
    PlayerShotSpawnDescriptorView *entry)
{
    if (value == entry->scheduleValue00)
    {
        player->InitializeShot(shot, entry);
        shot->velocity43C.x = 0.0f;
        shot->velocity43C.y = 0.0f;
        shot->angle450 = entry->angle14;
        return 1;
    }

    return 0;
}

int __fastcall PlayerShotSpawnCallbackType4(
    PlayerShotSpawnPlayerView *player,
    PlayerShotSpawnShotView *,
    int value,
    PlayerShotSpawnDescriptorView *entry)
{
    if (value == entry->scheduleValue00)
    {
        player->sideState0C->effectManager0C->SpawnEffectInFixedSlot(
            35, &player->position1B88, 5,
            static_cast<unsigned int>(-1));
    }
    return 0;
}

int __fastcall PlayerShotSpawnCallbackType5(
    PlayerShotSpawnPlayerView *player,
    PlayerShotSpawnShotView *shot,
    int value,
    PlayerShotSpawnDescriptorView *entry)
{
    PlayerShotSpawnEffectView *effect =
        player->sideState0C->effectManager0C->GetFixedSlotEffect(5);

    if (effect->activeC4 == 0 &&
        value == entry->scheduleValue00)
    {
        player->InitializeShot(shot, entry);
        return 1;
    }

    return 0;
}
