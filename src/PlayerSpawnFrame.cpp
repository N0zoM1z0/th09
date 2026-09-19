struct PlayerSpawnFrameShotView;
struct PlayerSpawnFrameDescriptorView
{
    short scheduleValue00;
    unsigned char unknown02[0x36];
};

struct PlayerSpawnFrameView
{
    void __fastcall InitializeShot(
        PlayerSpawnFrameShotView *shot,
        PlayerSpawnFrameDescriptorView *entry);

    int __fastcall SpawnShotWhenFrameMatches(
        PlayerSpawnFrameShotView *shot,
        int value,
        PlayerSpawnFrameDescriptorView *entry);
};

int __fastcall PlayerSpawnFrameView::SpawnShotWhenFrameMatches(
    PlayerSpawnFrameShotView *shot,
    int value,
    PlayerSpawnFrameDescriptorView *entry)
{
    if (value == entry->scheduleValue00)
    {
        InitializeShot(shot, entry);
        return 1;
    }
    return 0;
}
