#pragma once

#include "Chain.hpp"
#include "ZunTimer.hpp"

#include <stddef.h>

enum ScreenEffectType
{
    SCREEN_EFFECT_FULL_FADE_IN = 0,
    SCREEN_EFFECT_SHAKE = 1,
    SCREEN_EFFECT_ARCADE_FADE_OUT = 2,
    SCREEN_EFFECT_ARCADE_PULSE = 3,
    SCREEN_EFFECT_FULL_FADE_OUT = 4,
    SCREEN_EFFECT_FULL_FADE_HOLD = 5,
    SCREEN_EFFECT_SPLIT_FADE_HOLD = 6,
    SCREEN_EFFECT_SHAKE_ENVELOPE = 7,
};

struct ScreenEffectRect
{
    float left;
    float top;
    float right;
    float bottom;
};

struct ScreenEffect
{
    ScreenEffect();

    static void Clear(unsigned int color);
    static void SetViewport(unsigned int clearColor);
    static int CalcFadeIn(ScreenEffect *screenEffect);
    static int CalcFadeOut(ScreenEffect *screenEffect);
    static int CalcFadeHold(ScreenEffect *screenEffect);
    static int CalcArcadePulse(ScreenEffect *screenEffect);
    static int CalcShake(ScreenEffect *screenEffect);
    static int CalcShakeEnvelope(ScreenEffect *screenEffect);
    static int InitializeTimer(ScreenEffect *screenEffect);
    static int DeleteScreenEffect(ScreenEffect *screenEffect);
    void BeginFadeRelease();

    static void DrawSquare(ScreenEffectRect *rect, unsigned int color);
    static int DrawFullFade(ScreenEffect *screenEffect);
    static int DrawArcadeFade(ScreenEffect *screenEffect);
    static int DrawPartialFade(ScreenEffect *screenEffect);
    static int DrawSplitFade(ScreenEffect *screenEffect);
    static int DrawArcadePulse(ScreenEffect *screenEffect);

    static ScreenEffect *RegisterChain(
        ScreenEffectType effect,
        int durationFrames,
        int primaryParameter,
        int secondaryParameter,
        int tertiaryParameter,
        int drawPriority,
        int contextIndex);

    int contextIndex;
    ScreenEffectType type;
    ChainElem *calcChainElement;
    ChainElem *drawChainElement;
    int unconsumedDword10;
    int overlayAlpha;
    int duration;
    int rawParameter0;
    int rawParameter1;
    int rawParameter2;
    int fadeReleaseRequested;
    ZunTimer timer;
};

typedef char ScreenEffectSizeIs38[(sizeof(ScreenEffect) == 0x38) ? 1 : -1];
typedef char ScreenEffectTypeAt04[(offsetof(ScreenEffect, type) == 0x04) ? 1 : -1];
typedef char ScreenEffectAlphaAt14[(offsetof(ScreenEffect, overlayAlpha) == 0x14) ? 1 : -1];
typedef char ScreenEffectDurationAt18[(offsetof(ScreenEffect, duration) == 0x18) ? 1 : -1];
typedef char ScreenEffectRawAt1C[(offsetof(ScreenEffect, rawParameter0) == 0x1C) ? 1 : -1];
typedef char ScreenEffectReleaseAt28[(offsetof(ScreenEffect, fadeReleaseRequested) == 0x28) ? 1 : -1];
typedef char ScreenEffectTimerAt2C[(offsetof(ScreenEffect, timer) == 0x2C) ? 1 : -1];

extern int g_ScreenEffectCounter;
