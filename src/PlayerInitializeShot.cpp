#include <math.h>
#include <stddef.h>

struct PlayerShotInitFloat3
{
    float x;
    float y;
    float z;

    operator float *();
};

struct PlayerShotInitFloat2
{
    float x;
    float y;
};

struct PlayerShotInitTimer
{
    int previous;
    float subFrame;
    int current;

    void operator=(int value);
};

struct PlayerShotInitVm
{
    unsigned char storage[0x2A4];
};

struct PlayerShotInitShotView
{
    PlayerShotInitVm vm000;
    PlayerShotInitFloat3 position2A4;
    unsigned char unknown2B0[0x430 - 0x2B0];
    PlayerShotInitFloat3 hitbox430;
    PlayerShotInitFloat3 velocity43C;
    float auxiliary448;
    float speed44C;
    float angle450;
    PlayerShotInitTimer timer454;
    short damage460;
    short state462;
    short shotType464;
    unsigned char unknown466[0x46C - 0x466];
    short animationIndex46C;
    unsigned char tintFlag46E;
    unsigned char unknown46F;
    short field470;
    unsigned char unknown472[0x484 - 0x472];
};

typedef char PlayerShotInitShotSizeIs484[
    (sizeof(PlayerShotInitShotView) == 0x484) ? 1 : -1];

struct PlayerShotInitDescriptorView
{
    short fireInterval00;
    short fireFrame02;
    PlayerShotInitFloat2 positionOffset04;
    PlayerShotInitFloat2 hitbox0C;
    float angle14;
    float speed18;
    short damage1C;
    short unknown1E;
    short sourceOptionIndex20;
    short shotType22;
    short animationIndex24;
    short soundIndex26;
    unsigned char unknown28[0x38 - 0x28];
};

typedef char PlayerShotInitDescriptorSizeIs38[
    (sizeof(PlayerShotInitDescriptorView) == 0x38) ? 1 : -1];

struct PlayerShotInitOptionView
{
    unsigned char unknown000[0x2A4];
    PlayerShotInitFloat3 position2A4;
    unsigned char unknown2B0[0x2F4 - 0x2B0];
};

typedef char PlayerShotInitOptionSizeIs2F4[
    (sizeof(PlayerShotInitOptionView) == 0x2F4) ? 1 : -1];

struct PlayerShotInitAnmLoadedView
{
    void SetAndExecuteScriptIdx(void *vm, int scriptIndex);
};

struct PlayerShotInitSoundView
{
    void PlaySoundPositionedByIdx(int soundId, float x);
};

extern PlayerShotInitSoundView g_SoundPlayer;

struct PlayerShotInitPlayerView
{
    unsigned char unknown0000[0xBC];
    PlayerShotInitAnmLoadedView *anmFileBC;
    unsigned char unknown0C0[0x1B88 - 0xC0];
    PlayerShotInitFloat3 position1B88;
    unsigned char unknown1B94[0x1CEC - 0x1B94];
    PlayerShotInitOptionView optionStates1CEC[4];

    void __fastcall InitializeShot(
        PlayerShotInitShotView *shot,
        PlayerShotInitDescriptorView *entry);
};

typedef char PlayerShotInitPlayerPosAt1B88[
    (offsetof(PlayerShotInitPlayerView, position1B88) == 0x1B88) ? 1 : -1];
typedef char PlayerShotInitOptionsAt1CEC[
    (offsetof(PlayerShotInitPlayerView, optionStates1CEC) == 0x1CEC) ? 1 : -1];

void __fastcall PlayerShotInitPlayerView::InitializeShot(
    PlayerShotInitShotView *shot,
    PlayerShotInitDescriptorView *entry)
{
    if (entry->sourceOptionIndex20 == 0)
        shot->position2A4 = position1B88;
    else
        shot->position2A4 =
            optionStates1CEC[entry->sourceOptionIndex20 - 1].position2A4;

    float *position = shot->position2A4.operator float *();
    position[0] += entry->positionOffset04.x;
    position[1] += entry->positionOffset04.y;
    position[2] = 0.495f;

    shot->hitbox430.x = entry->hitbox0C.x;
    shot->hitbox430.y = entry->hitbox0C.y;
    shot->hitbox430.z = 1.0f;

    shot->angle450 = entry->angle14;
    shot->speed44C = entry->speed18;
    shot->velocity43C.x = cosf(entry->angle14) * entry->speed18;
    shot->velocity43C.y = sinf(entry->angle14) * entry->speed18;

    shot->timer454 = 0;
    shot->field470 = 0;
    shot->shotType464 = entry->shotType22;
    shot->damage460 = entry->damage1C;
    shot->animationIndex46C = entry->animationIndex24;

    if (entry->soundIndex26 >= 0)
        g_SoundPlayer.PlaySoundPositionedByIdx(
            entry->soundIndex26, position1B88.x);

    anmFileBC->SetAndExecuteScriptIdx(
        shot, entry->animationIndex24 + 5);
    shot->tintFlag46E = 0;
}
