#pragma once

#include "Chain.hpp"

#include <stddef.h>

struct PlayerSideStateView
{
    unsigned char unknown00[0x04];
    void *manager04;
    unsigned char unknown08[0x08];
    void *attackTarget10;
    void *attackOwner14;
    void *frontSide18;
    unsigned char unknown1C[0x04];
    int shotType20;
    int previousShotType24;
    unsigned char unknown28[0x04];
    int characterIndex2C;
    unsigned char unknown30[0x04];
    unsigned int flags34;
};

typedef char PlayerSideStateSizeIs38[(sizeof(PlayerSideStateView) == 0x38) ? 1 : -1];

struct PlayerShtDescriptorView
{
    short fireInterval;
    unsigned char unknown02[0x26];
    void *spawnCallback;
    void *updateCallback;
    void *drawCallback;
    void *collisionCallback;
};

typedef char PlayerShtDescriptorSizeIs38[(sizeof(PlayerShtDescriptorView) == 0x38) ? 1 : -1];

struct PlayerShtPowerLevelView
{
    PlayerShtDescriptorView *descriptors;
    int unknown04;
};

typedef char PlayerShtPowerLevelSizeIs8[(sizeof(PlayerShtPowerLevelView) == 8) ? 1 : -1];

struct PlayerShtFileView
{
    unsigned char unknown00[0x02];
    unsigned short shotPowerLevelCount;
    float hurtboxSize;
    float grazeBoxSize;
    unsigned char unknown0C[0x04];
    float itemCollectionBoxSize;
    float sizeSource0;
    float sizeSource1;
    float sizeDerived0;
    float sizeDerived1;
    unsigned char unknown24[0x08];
    const char *modePath0;
    unsigned char unknown30[0x3C];
    const char *modePath1;
    unsigned char unknown70[0x3C];
    const char *modePath2;
    unsigned char unknownB0[0x42C - 0xB0];
    PlayerShtPowerLevelView shotPowerLevels[1];
};

typedef char PlayerShtLevelsAt42C[(offsetof(PlayerShtFileView, shotPowerLevels) == 0x42C) ? 1 : -1];

struct PlayerHalfSizeView
{
    float x;
    float y;
    float z;
};

typedef char PlayerHalfSizeSizeIsC[(sizeof(PlayerHalfSizeView) == 0x0C) ? 1 : -1];

struct PlayerLifecycleView
{
    unsigned char unknown0000[0x08];
    int sideIndex;
    PlayerSideStateView *sideState;
    PlayerSideStateView *opponentState;
    ChainElem *calcChain;
    ChainElem *drawChainHighPrio;
    ChainElem *drawChainLowPrio;
    int selector;
    unsigned char unknown0024[0x9C - 0x24];
    void *selfLink09C;
    int state0A0;
    int state0A4;
    unsigned char unknown0A8[0x04];
    int state0AC;
    int state0B0;
    int state0B4;
    int state0B8;
    void *anmFile;
    unsigned char mainVm[0x36C - 0xC0];
    void *selfLink36C;
    unsigned char unknown0370[0x1B70 - 0x370];
    int transient1B70;
    unsigned char unknown1B74[0x0C];
    unsigned int flags1B80;
    unsigned char unknown1B84[0x1CA8 - 0x1B84];
    PlayerHalfSizeView hurtboxHalfSize;
    PlayerHalfSizeView grazeHalfSize;
    PlayerHalfSizeView itemCollectionHalfSize;
    unsigned char unknown1CCC[0x30338 - 0x1CCC];
    PlayerShtFileView *primaryShtFile;
    unsigned char unknown3033C[0x30384 - 0x3033C];
    float scalar30384;
    float scalar30388;
    unsigned char unknown3038C[0x303F0 - 0x3038C];
    int tableValue303F0;
    int tableValue303F4;
    int tableValue303F8;
    int tableValue303FC;
    int tableValue30400;
    void *callback30404;
    void *callback30408;
    unsigned char unknown3040C[0x04];
    void *owner30410;
    unsigned char unknown30414[0x3044C - 0x30414];
    int activeFrameCounter3044C;
    void *callback30450;
    unsigned char unknown30454[0x30F70 - 0x30454];

    PlayerLifecycleView();
    void InitializeType(unsigned char playerType);
    void ReleaseOwnedState();
    void InitializeAddedState();
    void UpdateBeforeState();
    int GetUpdateState();
    void UpdateStateValue();
    void SetBombMode0();
    void SetBombMode1();
    void SetBombMode2();
    void UpdateAfterAnimation();
    void UpdateHeavyState();

    static int __fastcall OnUpdate(PlayerLifecycleView *player);
    static int __fastcall AddedCallback(PlayerLifecycleView *player);
    static int __fastcall OnDrawHighPrio(PlayerLifecycleView *player);
    static int __fastcall OnDrawLowPrio(PlayerLifecycleView *player);
};

typedef char PlayerLifecycleSizeIs30F70[(sizeof(PlayerLifecycleView) == 0x30F70) ? 1 : -1];
typedef char PlayerLifecycleSideIndexAt08[(offsetof(PlayerLifecycleView, sideIndex) == 0x08) ? 1 : -1];
typedef char PlayerLifecycleSideStateAt0C[(offsetof(PlayerLifecycleView, sideState) == 0x0C) ? 1 : -1];
typedef char PlayerLifecycleOpponentAt10[(offsetof(PlayerLifecycleView, opponentState) == 0x10) ? 1 : -1];
typedef char PlayerLifecycleCalcAt14[(offsetof(PlayerLifecycleView, calcChain) == 0x14) ? 1 : -1];
typedef char PlayerLifecycleDrawHighAt18[(offsetof(PlayerLifecycleView, drawChainHighPrio) == 0x18) ? 1 : -1];
typedef char PlayerLifecycleDrawLowAt1C[(offsetof(PlayerLifecycleView, drawChainLowPrio) == 0x1C) ? 1 : -1];
typedef char PlayerLifecycleSelectorAt20[(offsetof(PlayerLifecycleView, selector) == 0x20) ? 1 : -1];
typedef char PlayerLifecycleAnmAtBC[(offsetof(PlayerLifecycleView, anmFile) == 0xBC) ? 1 : -1];
typedef char PlayerLifecycleFlagsAt1B80[(offsetof(PlayerLifecycleView, flags1B80) == 0x1B80) ? 1 : -1];
typedef char PlayerLifecyclePrimaryShtAt30338[(offsetof(PlayerLifecycleView, primaryShtFile) == 0x30338) ? 1 : -1];
typedef char PlayerLifecycleOwnerAt30410[(offsetof(PlayerLifecycleView, owner30410) == 0x30410) ? 1 : -1];
typedef char PlayerLifecycleCounterAt3044C[(offsetof(PlayerLifecycleView, activeFrameCounter3044C) == 0x3044C) ? 1 : -1];
