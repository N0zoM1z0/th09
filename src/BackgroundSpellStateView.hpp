#pragma once

#include <stddef.h>

// Shared target-observed prefix used by the native spell-background transition.
struct BackgroundSpellStateView
{
    unsigned char unknown000[0xCCC];
    int spellBackgroundStateCCC;
    int spellBackgroundFrameCD0;

    void BeginSpellBackground();
};

typedef char BackgroundSpellStateAtCCC[
    (offsetof(BackgroundSpellStateView, spellBackgroundStateCCC) == 0xCCC) ? 1 : -1];
typedef char BackgroundSpellFrameAtCD0[
    (offsetof(BackgroundSpellStateView, spellBackgroundFrameCD0) == 0xCD0) ? 1 : -1];
typedef char BackgroundSpellPrefixSizeIsCD4[
    (sizeof(BackgroundSpellStateView) == 0xCD4) ? 1 : -1];
