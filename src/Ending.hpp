#pragma once

#include "AnmVmLifecycle.hpp"
#include "Chain.hpp"
#include "ZunTimer.hpp"

#include <stddef.h>

class AnmLoaded;

enum EndingFadeMode
{
    ENDING_FADE_NONE = 0,
    ENDING_FADE_IN_BLACK = 1,
    ENDING_FADE_OUT_BLACK = 2,
    ENDING_FADE_IN_WHITE = 3,
    ENDING_FADE_OUT_WHITE = 4,
};

struct Ending
{
    Ending();

    int ReadScriptParameter();
    void DrawFade();
    void UpdateFade();
    int RunEndingScript();
    int LoadEndingScript(const char *path);

    static int RegisterChain();
    static int OnUpdate(Ending *ending);
    static int OnDraw(Ending *ending);
    static int AddedCallback(Ending *ending);
    static int DeletedCallback(Ending *ending);

    ChainElem *calcChain;
    ChainElem *drawChain;
    float backgroundPosX;
    float backgroundPosY;
    float backgroundScrollSpeed;
    AnmVm endingVms[16];
    unsigned char *scriptData;
    int hasSeenEnding;
    int unknown2A5C;
    AnmLoaded *endingAnm;
    ZunTimer elapsedTimer;
    ZunTimer lineWaitTimer;
    ZunTimer pageWaitTimer;
    int pageSkipLockFrames;
    int lineSkipLockFrames;
    int defaultLineWaitFrames;
    int minimumLineWaitFrames;
    int unknown2A98;
    int nextTextVmIndex;
    unsigned int textColor;
    unsigned int fadeColor;
    int fadeTimer;
    int fadeDuration;
    EndingFadeMode fadeMode;
    char *scriptCursor;
};

typedef char EndingSizeIs2AB8[(sizeof(Ending) == 0x2AB8) ? 1 : -1];
typedef char EndingVmsAt14[(offsetof(Ending, endingVms) == 0x14) ? 1 : -1];
typedef char EndingScriptDataAt2A54[(offsetof(Ending, scriptData) == 0x2A54) ? 1 : -1];
typedef char EndingTimersAt2A64[(offsetof(Ending, elapsedTimer) == 0x2A64) ? 1 : -1];
typedef char EndingTextIndexAt2A9C[(offsetof(Ending, nextTextVmIndex) == 0x2A9C) ? 1 : -1];
typedef char EndingFadeModeAt2AB0[(offsetof(Ending, fadeMode) == 0x2AB0) ? 1 : -1];
typedef char EndingScriptCursorAt2AB4[(offsetof(Ending, scriptCursor) == 0x2AB4) ? 1 : -1];
