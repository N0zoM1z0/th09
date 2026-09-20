#include <stddef.h>

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

void BackgroundSpellStateView::BeginSpellBackground()
{
    spellBackgroundStateCCC = 1;
    spellBackgroundFrameCD0 = 0;
}
