#pragma once

#include "Float3.hpp"
#include "ZunTimer.hpp"
#include <stddef.h>

class AnmLoaded;

typedef Float3 PlayerShotInitFloat3;

struct PlayerShotInitFloat2
{
    float x;
    float y;
};

typedef ZunTimer PlayerShotInitTimer;

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






struct PlayerShotInitPlayerView
{
    unsigned char unknown0000[0xBC];
    AnmLoaded *anmFileBC;
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
