#include <stddef.h>

struct TitleChangeScreenView
{
    unsigned char unknown00000[0x28];
    int currentScreenState;
    int stateTimer;
    unsigned char unknown00030[0x84 - 0x30];
    int previousScreen;
    unsigned char unknown00088[0xC910 - 0x88];
    int screenFrameCounter;
    unsigned char unknown0C914[0x1B228 - 0xC914];
    int currentScreen;
    int registrationContext;
    int phaseTimer;

    int ChangeCurrentScreen(int screen);
};

typedef char TitleChangePreviousAt84[
    (offsetof(TitleChangeScreenView, previousScreen) == 0x84) ? 1 : -1];
typedef char TitleChangeFrameAtC910[
    (offsetof(TitleChangeScreenView, screenFrameCounter) == 0xC910) ? 1 : -1];
typedef char TitleChangeCurrentAt1B228[
    (offsetof(TitleChangeScreenView, currentScreen) == 0x1B228) ? 1 : -1];
typedef char TitleChangePhaseAt1B230[
    (offsetof(TitleChangeScreenView, phaseTimer) == 0x1B230) ? 1 : -1];

int TitleChangeScreenView::ChangeCurrentScreen(int screen)
{
    previousScreen = currentScreen;
    currentScreen = screen;
    stateTimer = 0;
    phaseTimer = 0;
    currentScreenState = 0;
    screenFrameCounter = 0;
    return 0;
}
