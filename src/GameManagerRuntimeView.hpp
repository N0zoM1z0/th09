#pragma once

#include "Float3.hpp"

#include <stddef.h>

struct EffectManager;
class EtamaController;
struct PlayerLifecycleView;
struct ReplayInputGateView;

struct AsciiPlayerPopupPositionView
{
    unsigned char unknown0000[0x1B88];
    Float3 position;
};

// This is the target-backed 0xA0-byte side score/phase buffer, allocated
// from GameManager setup and read from the ASCII side runtime at +0x1C.
struct AsciiGameSideRuntimeView
{
    float value00;
    unsigned int value04;
    unsigned int value08;
    unsigned int value0C;
    union { unsigned char unknown10[4]; int phase10; };
    union { unsigned char counter14; unsigned char value14; };
    unsigned char unknown15[0x8B];
};

typedef AsciiGameSideRuntimeView SetupStateBuffer;
typedef char AsciiGameSideNativeScoreSizeIsA0[
    (sizeof(AsciiGameSideRuntimeView) == 0xA0 &&
     offsetof(AsciiGameSideRuntimeView, phase10) == 0x10 &&
     offsetof(AsciiGameSideRuntimeView, value14) == 0x14) ? 1 : -1];

struct AsciiGameManagerSideView
{
    union { unsigned char unknown00[4]; void *subsystem0; };
    // The same original +0x04..+0x1C gameplay side controllers are consumed
    // by GameManager setup, ECL/Enemy and the replay/ASCII owners.
    union {
        AsciiPlayerPopupPositionView *player;
        PlayerLifecycleView *player04;
        void *subsystem1;
    };
    union { EtamaController *etama08; void *subsystem2; };
    union { EffectManager *effectManager0C; void *subsystem3; };
    void *subsystem4;
    void *subsystem5;
    void *subsystem6;
    union { AsciiGameSideRuntimeView *runtime; SetupStateBuffer *state; };
    union { int character20; int value20; };
    union { unsigned char unknown24[4]; int value24; };
    union { int replaySelector28; int selector; };
    unsigned char unknown2C[8];
    unsigned int flags;
};

typedef char AsciiGameManagerSideViewSizeIs38[
    (sizeof(AsciiGameManagerSideView) == 0x38) ? 1 : -1];
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

struct AsciiGameManagerView
{
    AsciiGameManagerSideView sides[2];
    unsigned char unknown070[0x40];
    unsigned int valueB0;
    union { int valueB4; int limitB4; };
    int valueB8;
    unsigned char unknownBC[4];
    unsigned int valueC0;
    unsigned int valueC4;
    int valueC8;
    int valueCC;
    int valueD0;
    void *stageObjectD4;
    unsigned char clearD8[0x10];
    union { ReplayInputGateView *replayInputGateE8; void *heapE8; };
    float speedEC;
    unsigned char unknown0F0[0x0C];
    int field0FC;
    unsigned char unknown100[8];
    int field108;
    int field10C;
    unsigned char unknown110[4];
    int replayMode114;
    int gameMode;
    union { int difficulty; int difficulty11C; };
    unsigned char unknown120[0x10];
    int replayPauseRecorded130;
    unsigned int flags;
    unsigned char unknown138[4];
    unsigned char inGameMenu;
    unsigned char menuState97C8Active;
    unsigned char menuStateA7A8Active;
    unsigned char unknown13F;
    unsigned char unknown140[0x348 - 0x140];
    // Original TH09 ScreenEffect::CalcShakeEnvelope reads this i32 counter.
    int value348;
    unsigned char unknown34C[0x350 - 0x34C];
    float coordinateOffsetX;
    float coordinateOffsetY;

    int IsWithinPlayfield(float x, float y, float width, float height);
    float TransformPopupX(float value);
    float TransformPopupY(float value);
    float InverseTransformPopupX(float value);
    float InverseTransformPopupY(float value);
    int IsGameMode0();
    int IsGameMode1();
    int IsGameMode2();
    int IsReplayNeutral();
    int HasFlagBit0();
};

typedef char AsciiGameManagerSideCharacterAt20[
    (offsetof(AsciiGameManagerSideView, character20) == 0x20) ? 1 : -1];

typedef char AsciiGameManagerSidePlayerAt04[
    (offsetof(AsciiGameManagerSideView, player) == 0x04) ? 1 : -1];
typedef char AsciiGameManagerSideRuntimeAt1C[
    (offsetof(AsciiGameManagerSideView, runtime) == 0x1C) ? 1 : -1];
typedef char AsciiGameManagerSideEtamaAt08[
    (offsetof(AsciiGameManagerSideView, etama08) == 0x08) ? 1 : -1];
typedef char AsciiGameManagerSideEffectManagerAt0C[
    (offsetof(AsciiGameManagerSideView, effectManager0C) == 0x0C) ? 1 : -1];
typedef char AsciiGameManagerField0FC[
    (offsetof(AsciiGameManagerView, field0FC) == 0x0FC) ? 1 : -1];
typedef char AsciiGameManagerValueAtB4[
    (offsetof(AsciiGameManagerView, valueB4) == 0xB4) ? 1 : -1];
typedef char AsciiGameManagerSetupCountersAtB0[
    (offsetof(AsciiGameManagerView, valueB0) == 0xB0 &&
     offsetof(AsciiGameManagerView, limitB4) == 0xB4 &&
     offsetof(AsciiGameManagerView, valueB8) == 0xB8 &&
     offsetof(AsciiGameManagerView, valueC0) == 0xC0 &&
     offsetof(AsciiGameManagerView, valueC4) == 0xC4 &&
     offsetof(AsciiGameManagerView, valueC8) == 0xC8 &&
     offsetof(AsciiGameManagerView, valueCC) == 0xCC &&
     offsetof(AsciiGameManagerView, valueD0) == 0xD0 &&
     offsetof(AsciiGameManagerView, stageObjectD4) == 0xD4 &&
     offsetof(AsciiGameManagerView, clearD8) == 0xD8 &&
     offsetof(AsciiGameManagerView, heapE8) == 0xE8) ? 1 : -1];
typedef char AsciiGameManagerSpeedAtEC[
    (offsetof(AsciiGameManagerView, speedEC) == 0xEC) ? 1 : -1];
typedef char AsciiGameManagerGameModeAt118[
    (offsetof(AsciiGameManagerView, gameMode) == 0x118) ? 1 : -1];
typedef char AsciiGameManagerDifficultyAt11C[
    (offsetof(AsciiGameManagerView, difficulty) == 0x11C) ? 1 : -1];
typedef char AsciiGameManagerFlagsAt134[
    (offsetof(AsciiGameManagerView, flags) == 0x134) ? 1 : -1];
typedef char AsciiGameManagerReplayStateOffsets[
    (offsetof(AsciiGameManagerView, replayInputGateE8) == 0xE8 &&
     offsetof(AsciiGameManagerView, replayMode114) == 0x114 &&
     offsetof(AsciiGameManagerView, replayPauseRecorded130) == 0x130 &&
     offsetof(AsciiGameManagerSideView, replaySelector28) == 0x28) ? 1 : -1];
typedef char AsciiGameManagerMenuAt13C[
    (offsetof(AsciiGameManagerView, inGameMenu) == 0x13C) ? 1 : -1];
typedef char AsciiGameManagerValue348At348[
    (offsetof(AsciiGameManagerView, value348) == 0x348) ? 1 : -1];
typedef char AsciiGameManagerCoordinateXAt350[
    (offsetof(AsciiGameManagerView, coordinateOffsetX) == 0x350) ? 1 : -1];
typedef char AsciiGameManagerCoordinateYAt354[
    (offsetof(AsciiGameManagerView, coordinateOffsetY) == 0x354) ? 1 : -1];

extern AsciiGameManagerView g_GameManager;

struct RenderCoordinateOriginView
{
    unsigned char unknown000[0xCC];
    unsigned int x;
    unsigned int y;
    unsigned int width;
    unsigned int height;
};

extern RenderCoordinateOriginView *g_RenderCoordinateOrigin;
