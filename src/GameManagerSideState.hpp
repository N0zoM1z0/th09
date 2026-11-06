#pragma once

#include <stddef.h>

struct EffectManager;
class EtamaController;
struct PlayerLifecycleView;
struct Background;
struct EnemyManagerView;
struct CardAttack;
struct FrontSide;
struct FrontSidePlayerRuntimeView;
struct FrontSideSideRuntimeView;
struct AsciiPlayerPopupPositionView;
struct AsciiGameSideRuntimeView;

// Shared 0x38-byte gameplay side allocation. Legacy Player names below
// identify the same slots; original C++ names and several roles are unknown.
struct AsciiGameManagerSideView
{
    union { unsigned char unknown00[4]; void *subsystem0; Background *background00; };
    // The same original +0x04..+0x1C gameplay side controllers are consumed
    // by GameManager setup, ECL/Enemy and the replay/ASCII owners.
    union {
        AsciiPlayerPopupPositionView *player;
        PlayerLifecycleView *player04;
        FrontSidePlayerRuntimeView *hudPlayer04;
        void *subsystem1;
        void *manager04;
    };
    union { EtamaController *etama08; void *subsystem2; unsigned char unknown08[4]; };
    union { EffectManager *effectManager0C; void *subsystem3; };
    union { void *subsystem4; EnemyManagerView *enemyManager10; void *attackTarget10; };
    union { void *subsystem5; CardAttack *cardAttack14; void *attackOwner14; };
    union { void *subsystem6; FrontSide *frontSide18; };
    union { AsciiGameSideRuntimeView *runtime; AsciiGameSideRuntimeView *state; FrontSideSideRuntimeView *runtime1C; unsigned char unknown1C[4]; };
    union { int character20; int value20; int shotType20; };
    union { unsigned char unknown24[4]; int value24; int previousShotType24; };
    union { int replaySelector28; int selector; unsigned char unknown28[4]; };
    union {
        unsigned char unknown2C[8];
        struct { unsigned int value2C; unsigned int value30; };
        struct { int characterIndex2C; union { unsigned char unknown30[4]; int auxPathIndex30; }; };
    };
    union { unsigned int flags; unsigned int flags34; };
};

typedef char AsciiGameManagerSideViewSizeIs38[
    (sizeof(AsciiGameManagerSideView) == 0x38) ? 1 : -1];
// Genuine Create/Register call targets independently identify these side
// subsystems; the aliases do not change the 0x38-byte i386 storage record.
typedef char AsciiGameManagerTypedSubsystemsAtOffsets[
    (offsetof(AsciiGameManagerSideView, background00) == 0x00 &&
     offsetof(AsciiGameManagerSideView, player04) == 0x04 &&
     offsetof(AsciiGameManagerSideView, etama08) == 0x08 &&
     offsetof(AsciiGameManagerSideView, effectManager0C) == 0x0C &&
     offsetof(AsciiGameManagerSideView, enemyManager10) == 0x10 &&
     offsetof(AsciiGameManagerSideView, cardAttack14) == 0x14 &&
     offsetof(AsciiGameManagerSideView, frontSide18) == 0x18) ? 1 : -1];

typedef char AsciiGameManagerNativeSetupSideOffsets[
    (offsetof(AsciiGameManagerSideView, subsystem0) == 0x00 &&
     offsetof(AsciiGameManagerSideView, subsystem1) == 0x04 &&
     offsetof(AsciiGameManagerSideView, subsystem2) == 0x08 &&
     offsetof(AsciiGameManagerSideView, subsystem3) == 0x0C &&
     offsetof(AsciiGameManagerSideView, subsystem4) == 0x10 &&
     offsetof(AsciiGameManagerSideView, subsystem5) == 0x14 &&
     offsetof(AsciiGameManagerSideView, subsystem6) == 0x18 &&
     offsetof(AsciiGameManagerSideView, state) == 0x1C &&
     offsetof(AsciiGameManagerSideView, value20) == 0x20 &&
     offsetof(AsciiGameManagerSideView, value24) == 0x24 &&
     offsetof(AsciiGameManagerSideView, selector) == 0x28 &&
     offsetof(AsciiGameManagerSideView, flags) == 0x34) ? 1 : -1];

