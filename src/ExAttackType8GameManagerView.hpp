#pragma once

#include "AnmManager.hpp"
#include "AsciiGameManagerView.hpp"

#include <stddef.h>

struct PlayerLifecycleView;
class EtamaController;

// Reuse the physical TH09 GameManager at 0x004A7D90 from the separately
// exact AsciiGameManagerView member and global declarations. The ECL/ExAttack
// side/ANM fragments below remain bounded layout views, not another native
// global object or an additional incompatible C++ owner.

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
    PlayerLifecycleView *player04;
    EtamaController *etama08;
    ExAttackType8AnmOwner *anmOwner0C;
    unsigned char unknown10[0x28];
};

// The game manager at 0x004A7D90 is ONE native global. Reuse its real
// source type instead of introducing a second C++-decorated class identity.
typedef AsciiGameManagerView ExAttackType8GameManagerView;

typedef char ExAttackType8SideSizeIs38[
    (sizeof(ExAttackType8SideView) == 0x38) ? 1 : -1];
typedef char ExAttackType8GameManagerSpeedAtEC[
    (offsetof(AsciiGameManagerView, speedEC) == 0xEC) ? 1 : -1];
typedef char ExAttackType8GameManagerDifficultyAt11C[
    (offsetof(AsciiGameManagerView, difficulty11C) == 0x11C) ? 1 : -1];
