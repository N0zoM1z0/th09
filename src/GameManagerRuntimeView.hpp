#pragma once

#include "Float3.hpp"
#include "GameManagerSideState.hpp"

#include <stddef.h>

struct EffectManager;
class EtamaController;
struct PlayerLifecycleView;
struct ReplayInputGateView;
struct Background;
struct EnemyManagerView;
struct CardAttack;
struct FrontSide;
struct ReplayManagerView;
struct GameConfiguration;
struct ReplayStageDispatchView;
struct OpponentSelectionEntry;

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
    union { unsigned int value08; int score08; };
    unsigned int value0C;
    union { unsigned char unknown10[4]; int phase10; };
    union { unsigned char counter14; unsigned char value14; };
    unsigned char unknown15[0x8B];
};

// Both the independently exact 33-byte score leaf and real GameManager
// setup operate on the same side +0x1C, runtime +0x08 native score word.
typedef char AsciiGameSideNativeScoreAt08[
    (offsetof(AsciiGameSideRuntimeView, value08) == 0x08 &&
     offsetof(AsciiGameManagerSideView, state) == 0x1C) ? 1 : -1];

typedef AsciiGameSideRuntimeView SetupStateBuffer;
// The 33-byte original score leaf adds signed score/10 at side-runtime
// +0x08. This shares the exact native scoreboard word with value08;
// no second side-score allocation or signedness-changing memory copy.
typedef char AsciiGameSideNativeScoreWordAt08[
    (sizeof(AsciiGameSideRuntimeView) == 0xA0 &&
     offsetof(AsciiGameSideRuntimeView, value08) == 0x08 &&
     offsetof(AsciiGameSideRuntimeView, score08) == 0x08) ? 1 : -1];

typedef char AsciiGameSideNativeScoreSizeIsA0[
    (sizeof(AsciiGameSideRuntimeView) == 0xA0 &&
     offsetof(AsciiGameSideRuntimeView, phase10) == 0x10 &&
     offsetof(AsciiGameSideRuntimeView, value14) == 0x14) ? 1 : -1];

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
    union { void *stageObjectD4; ReplayManagerView *replayManagerD4; ReplayStageDispatchView *replayDispatchD4; };
    union { unsigned char clearD8[0x10]; unsigned char usedCharactersD8[16]; };
    union { ReplayInputGateView *replayInputGateE8; GameConfiguration *configurationE8; void *heapE8; };
    float speedEC;
    union { unsigned char unknown0F0[4]; OpponentSelectionEntry *selectedEntryF0; };
    union { int valueF4; int selectedValueF4; };
    int valueF8;
    union { int field0FC; int counterFC; };
    int value100;
    int value104;
    int field108;
    int field10C;
    unsigned char unknown110[4];
    int replayMode114;
    int gameMode;
    union { int difficulty; int difficulty11C; int setupMode11C; };
    // Native ECL and exact enemy-timeline instruction dispatch share the
    // +0x120 word/byte owner, never a second GameManager projection.
    union { unsigned int instructionMask120; unsigned char instructionMaskLowByte120; };
    int gameplaySetupState124;
    int gameplaySetupWaitFrames128;
    int playtimeFrames12C;
    int replayPauseRecorded130;
    // CardAttack::OnUpdate and StartAttack consume exactly the original
    // two-bit transition window in the native +0x134 GameManager flags.
    union {
        unsigned int flags;
        struct {
            unsigned int lowFlags134 : 11;
            unsigned int transitionBits134 : 2;
            unsigned int highFlags134 : 19;
        };
    };
    union { unsigned char unknown138[4]; int value138; int selector138; };
    unsigned char inGameMenu;
    union { unsigned char menuState97C8Active; unsigned char menuBlock13D; };
    union { unsigned char menuStateA7A8Active; unsigned char menuBlock13E; };
    union { unsigned char unknown13F; unsigned char resetFlag13F; };
    int demoFrameCount140;
    unsigned char unknown144;
    unsigned char modeByte145;
    unsigned short seed146;
    unsigned char unknown148[0x346 - 0x148];
    unsigned short seedCopy346;
    // Original TH09 ScreenEffect::CalcShakeEnvelope reads this i32 counter.
    int value348;
    unsigned char unknown34C[0x350 - 0x34C];
    float coordinateOffsetX;
    float coordinateOffsetY;
    // Same original eight-float playfield record initialized by the
    // independently exact GameManager coordinate-bound initializer.
    float coordinate358;
    float coordinate35C;
    float coordinate360;
    float coordinate364;
    float coordinate368;
    float coordinate36C;
    unsigned char unknown370[0x378 - 0x370];
    int value378;
    int value37C;
    // Original RunEcl opcode 148 adds 1800 here; the GameManager reset
    // clears exactly the same physical four-byte word at +0x380.
    int value380;
    unsigned char unknown384[0x390 - 0x384];

    int IsWithinPlayfield(float x, float y, float width, float height);
    float TransformPopupX(float value);
    float TransformPopupY(float value);
    float InverseTransformPopupX(float value);
    float InverseTransformPopupY(float value);
    int IsGameMode0();
    int IsGameMode1();
    int IsGameMode2();
    void SelectOpponentConfiguration();
    int IsReplayNeutral();
    int HasFlagBit0();
};

typedef char AsciiGameManagerNativeEclStorage[
    (offsetof(AsciiGameManagerView, instructionMask120) == 0x120 &&
     offsetof(AsciiGameManagerView, instructionMaskLowByte120) == 0x120 &&
     offsetof(AsciiGameManagerView, value380) == 0x380 &&
     sizeof(AsciiGameManagerView) == 0x390) ? 1 : -1];
typedef char AsciiGameManagerNativeCardAttackTransitionFlags[
    (offsetof(AsciiGameManagerView, flags) == 0x134 &&
     sizeof(AsciiGameManagerView) == 0x390) ? 1 : -1];
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
     offsetof(AsciiGameManagerView, replayManagerD4) == 0xD4 &&
     offsetof(AsciiGameManagerView, replayDispatchD4) == 0xD4 &&
     offsetof(AsciiGameManagerView, clearD8) == 0xD8 &&
     offsetof(AsciiGameManagerView, heapE8) == 0xE8) ? 1 : -1];
typedef char AsciiGameManagerSpeedAtEC[
    (offsetof(AsciiGameManagerView, speedEC) == 0xEC) ? 1 : -1];
typedef char AsciiGameManagerGameModeAt118[
    (offsetof(AsciiGameManagerView, gameMode) == 0x118) ? 1 : -1];
typedef char AsciiGameManagerNativeSetupControlOffsets[
    (offsetof(AsciiGameManagerView, valueF4) == 0xF4 &&
     offsetof(AsciiGameManagerView, valueF8) == 0xF8 &&
     offsetof(AsciiGameManagerView, counterFC) == 0xFC &&
     offsetof(AsciiGameManagerView, value100) == 0x100 &&
     offsetof(AsciiGameManagerView, value104) == 0x104 &&
     offsetof(AsciiGameManagerView, setupMode11C) == 0x11C &&
     offsetof(AsciiGameManagerView, instructionMask120) == 0x120 &&
     offsetof(AsciiGameManagerView, gameplaySetupState124) == 0x124 &&
     offsetof(AsciiGameManagerView, gameplaySetupWaitFrames128) == 0x128 &&
     offsetof(AsciiGameManagerView, playtimeFrames12C) == 0x12C &&
     offsetof(AsciiGameManagerView, menuBlock13D) == 0x13D &&
     offsetof(AsciiGameManagerView, menuBlock13E) == 0x13E &&
     offsetof(AsciiGameManagerView, demoFrameCount140) == 0x140 &&
     offsetof(AsciiGameManagerView, modeByte145) == 0x145 &&
     offsetof(AsciiGameManagerView, seed146) == 0x146 &&
     offsetof(AsciiGameManagerView, seedCopy346) == 0x346) ? 1 : -1];
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
// This native allocation, the previously separate 0x390-byte reset view
// and the exact 79-byte coordinate initializer cover the same byte owner.
typedef char AsciiGameManagerNativePlayfieldBounds[
    (offsetof(AsciiGameManagerView, coordinate358) == 0x358 &&
     offsetof(AsciiGameManagerView, coordinate35C) == 0x35C &&
     offsetof(AsciiGameManagerView, coordinate360) == 0x360 &&
     offsetof(AsciiGameManagerView, coordinate364) == 0x364 &&
     offsetof(AsciiGameManagerView, coordinate368) == 0x368 &&
     offsetof(AsciiGameManagerView, coordinate36C) == 0x36C &&
     offsetof(AsciiGameManagerView, value378) == 0x378 &&
     offsetof(AsciiGameManagerView, value37C) == 0x37C &&
     sizeof(AsciiGameManagerView) == 0x390) ? 1 : -1];

// StartupThread needs only the shared layout while retaining its original
// StartupGameManager-typed external symbol (the type participates in MSVC's
// decorated name). Ordinary owners keep the canonical runtime declaration.
#ifndef TH09_GAME_MANAGER_TYPES_ONLY
extern AsciiGameManagerView g_GameManager;
#endif

struct RenderCoordinateOriginView
{
    unsigned char unknown000[0xCC];
    unsigned int x;
    unsigned int y;
    unsigned int width;
    unsigned int height;
};

extern RenderCoordinateOriginView *g_RenderCoordinateOrigin;

// Opponent selection's original 799-byte exact owner uses these same
// physical +0xD8/+0xF0/+0xF4/+0xFC/+0x11C/+0x138 manager words.
// No independent source-only 0x13C-byte opponent manager is allocated.
typedef char NativeGameManagerOpponentSelectionOffsets[
    (sizeof(AsciiGameManagerView) == 0x390 &&
     offsetof(AsciiGameManagerView, usedCharactersD8) == 0xD8 &&
     sizeof(((AsciiGameManagerView *)0)->usedCharactersD8) == 16 &&
     offsetof(AsciiGameManagerView, selectedEntryF0) == 0xF0 &&
     offsetof(AsciiGameManagerView, selectedValueF4) == 0xF4 &&
     offsetof(AsciiGameManagerView, counterFC) == 0xFC &&
     offsetof(AsciiGameManagerView, setupMode11C) == 0x11C &&
     offsetof(AsciiGameManagerView, selector138) == 0x138 &&
     offsetof(AsciiGameManagerSideView, selectedParameter2C) == 0x2C &&
     sizeof(AsciiGameManagerSideView) == 0x38) ? 1 : -1];
