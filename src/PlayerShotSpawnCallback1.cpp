#include "PlayerShotInitializeView.hpp"
#include "ZunTimer.hpp"
#include "PlayerLifecycleView.hpp"

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

// This callback's +0x3031C timer/shot record is the same genuine
// PlayerState3031CView constructed by Player at 0x41EE20. It is not
// a second source-only 0x1C-byte callback state allocation.
struct PlayerShotSpawn1PlayerView
{
    unsigned char unknown0000[0x3031C];
    PlayerState3031CView state3031C;
};
typedef char PlayerShotSpawn1RealNativeState[
    (sizeof(PlayerState3031CView) == 0x1C &&
     offsetof(PlayerShotSpawn1PlayerView, state3031C) == 0x3031C &&
     offsetof(PlayerState3031CView, timer00) == 0x00 &&
     offsetof(PlayerState3031CView, shot0C) == 0x0C &&
     sizeof(PlayerShotCtorView) == 0x484) ? 1 : -1];

int __fastcall PlayerShotSpawnCallbackType1(
    PlayerShotSpawn1PlayerView *player,
    PlayerShotSpawn1ShotView *shot,
    int value,
    PlayerShotSpawn1DescriptorView *entry)
{
    if (value != entry->scheduleValue00)
        return 0;

    player->state3031C.timer00 = 30;
    player->state3031C.shot0C = reinterpret_cast<PlayerShotCtorView *>(shot);

    shot->sourceOption468 = entry->sourceOption20;
    shot->velocity43C.z = entry->value04;
    shot->auxiliary448 = entry->value08;
    shot->field46A = 15;

    reinterpret_cast<PlayerShotInitPlayerView *>(player)->InitializeShot(
            reinterpret_cast<PlayerShotInitShotView *>(shot),
            reinterpret_cast<PlayerShotInitDescriptorView *>(entry));
    shot->shotType464 = 2;

    for (int i = 31; i >= 0; --i)
        shot->trail2B0[i].x = -999.0f;
    shot->position2A4.x = -999.0f;

    return 1;
}
