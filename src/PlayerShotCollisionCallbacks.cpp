struct PlayerShotCallbackPlayerView
{
    unsigned char unknown00000[0x30354];
    unsigned char counter30354;
};

struct PlayerShotCallbackPositionView
{
    float x;
    float y;
    float z;
};

int __fastcall PlayerShotCollisionCallbackType1(
    PlayerShotCallbackPlayerView *player,
    void *,
    PlayerShotCallbackPositionView *)
{
    ++player->counter30354;
    return 0;
}

int __fastcall PlayerShotCollisionCallbackType2(
    PlayerShotCallbackPlayerView *,
    void *,
    PlayerShotCallbackPositionView *)
{
    return 1;
}
