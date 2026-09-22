#pragma once

#include "AnmManager.hpp"

#include <stddef.h>

// Shared TH09 target view for the EclManager TU. The type name is retained
// because exact ExAttack relocations reference its TransformPopupX/Y members;
// +0xEC is independently required by exact post-ECL movement.

struct ExAttackType8PlayerView
{
    unsigned char unknown000[0x1B88];
    Float3 position1B88;
};

struct ExAttackType8AnmOwner
{
    unsigned char unknown000[0x2D4];
    AnmLoaded *anm2D4;
};

struct ExAttackType8SideView
{
    unsigned char unknown00[0x04];
    ExAttackType8PlayerView *player04;
    unsigned char unknown08[0x04];
    ExAttackType8AnmOwner *anmOwner0C;
    unsigned char unknown10[0x28];
};

struct ExAttackType8GameManagerView
{
    ExAttackType8SideView sides[2];
    unsigned char unknown070[0x7C];
    float speedEC;

    float TransformPopupX(float value);
    float TransformPopupY(float value);
};

typedef char ExAttackType8SideSizeIs38[
    (sizeof(ExAttackType8SideView) == 0x38) ? 1 : -1];
typedef char ExAttackType8GameManagerSpeedAtEC[
    (offsetof(ExAttackType8GameManagerView, speedEC) == 0xEC) ? 1 : -1];

extern ExAttackType8GameManagerView g_GameManager;
