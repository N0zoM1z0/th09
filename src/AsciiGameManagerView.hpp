#pragma once

#include "AsciiManager.hpp"

#include <stddef.h>

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
    AsciiPlayerPopupPositionView *player;
    unsigned char unknown08[0x14];
    AsciiGameSideRuntimeView *runtime;
    unsigned char unknown20[0x14];
    unsigned int flags;
};

typedef char AsciiGameManagerSideViewSizeIs38[
    (sizeof(AsciiGameManagerSideView) == 0x38) ? 1 : -1];

struct AsciiGameManagerView
{
    AsciiGameManagerSideView sides[2];
    unsigned char unknown070[0x8C];
    int field0FC;
    unsigned char unknown100[8];
    int field108;
    int field10C;
    unsigned char unknown110[8];
    int gameMode;
    int difficulty;
    unsigned char unknown120[0x14];
    unsigned int flags;
    unsigned char unknown138[4];
    unsigned char inGameMenu;
    unsigned char menuState97C8Active;
    unsigned char menuStateA7A8Active;
    unsigned char unknown13F;
    unsigned char unknown140[0x350 - 0x140];
    float coordinateOffsetX;
    float coordinateOffsetY;

    float TransformPopupX(float value);
    float TransformPopupY(float value);
    int IsGameMode1();
    int IsReplayNeutral();
    int HasFlagBit0();
};

typedef char AsciiGameManagerSidePlayerAt04[
    (offsetof(AsciiGameManagerSideView, player) == 0x04) ? 1 : -1];
typedef char AsciiGameManagerSideRuntimeAt1C[
    (offsetof(AsciiGameManagerSideView, runtime) == 0x1C) ? 1 : -1];
typedef char AsciiGameManagerField0FC[
    (offsetof(AsciiGameManagerView, field0FC) == 0x0FC) ? 1 : -1];
typedef char AsciiGameManagerGameModeAt118[
    (offsetof(AsciiGameManagerView, gameMode) == 0x118) ? 1 : -1];
typedef char AsciiGameManagerDifficultyAt11C[
    (offsetof(AsciiGameManagerView, difficulty) == 0x11C) ? 1 : -1];
typedef char AsciiGameManagerFlagsAt134[
    (offsetof(AsciiGameManagerView, flags) == 0x134) ? 1 : -1];
typedef char AsciiGameManagerMenuAt13C[
    (offsetof(AsciiGameManagerView, inGameMenu) == 0x13C) ? 1 : -1];
typedef char AsciiGameManagerCoordinateXAt350[
    (offsetof(AsciiGameManagerView, coordinateOffsetX) == 0x350) ? 1 : -1];
typedef char AsciiGameManagerCoordinateYAt354[
    (offsetof(AsciiGameManagerView, coordinateOffsetY) == 0x354) ? 1 : -1];

extern AsciiGameManagerView g_GameManager;
