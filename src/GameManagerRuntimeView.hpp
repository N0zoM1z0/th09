#pragma once

#include "Float3.hpp"

#include <stddef.h>

struct ExAttackType8AnmOwner;
class EtamaController;
struct PlayerLifecycleView;

struct AsciiPlayerPopupPositionView
{
    unsigned char unknown0000[0x1B88];
    Float3 position;
};

struct AsciiGameSideRuntimeView
{
    float value00;
    int value04;
    int value08;
    int value0C;
    unsigned char unknown10[4];
    unsigned char counter14;
};

struct AsciiGameManagerSideView
{
    unsigned char unknown00[4];
    // Existing caller sees the player as the known popup-position prefix;
    // ECL/ExAttack reads the same +0x04 pointer as the full player runtime.
    union {
        AsciiPlayerPopupPositionView *player;
        PlayerLifecycleView *player04;
    };
    EtamaController *etama08;
    ExAttackType8AnmOwner *anmOwner0C;
    unsigned char unknown10[0x0C];
    AsciiGameSideRuntimeView *runtime;
    int character20;
    unsigned char unknown24[0x10];
    unsigned int flags;
};

typedef char AsciiGameManagerSideViewSizeIs38[
    (sizeof(AsciiGameManagerSideView) == 0x38) ? 1 : -1];

struct AsciiGameManagerView
{
    AsciiGameManagerSideView sides[2];
    unsigned char unknown070[0x44];
    int valueB4;
    unsigned char unknown0B8[0x34];
    float speedEC;
    unsigned char unknown0F0[0x0C];
    int field0FC;
    unsigned char unknown100[8];
    int field108;
    int field10C;
    unsigned char unknown110[8];
    int gameMode;
    union { int difficulty; int difficulty11C; };
    unsigned char unknown120[0x14];
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
typedef char AsciiGameManagerSideAnmOwnerAt0C[
    (offsetof(AsciiGameManagerSideView, anmOwner0C) == 0x0C) ? 1 : -1];
typedef char AsciiGameManagerField0FC[
    (offsetof(AsciiGameManagerView, field0FC) == 0x0FC) ? 1 : -1];
typedef char AsciiGameManagerValueAtB4[
    (offsetof(AsciiGameManagerView, valueB4) == 0xB4) ? 1 : -1];
typedef char AsciiGameManagerSpeedAtEC[
    (offsetof(AsciiGameManagerView, speedEC) == 0xEC) ? 1 : -1];
typedef char AsciiGameManagerGameModeAt118[
    (offsetof(AsciiGameManagerView, gameMode) == 0x118) ? 1 : -1];
typedef char AsciiGameManagerDifficultyAt11C[
    (offsetof(AsciiGameManagerView, difficulty) == 0x11C) ? 1 : -1];
typedef char AsciiGameManagerFlagsAt134[
    (offsetof(AsciiGameManagerView, flags) == 0x134) ? 1 : -1];
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
