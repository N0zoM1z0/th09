#include "Chain.hpp"
#include "TitleScreenFactoryView.hpp"

#include <stddef.h>
#include <string.h>

struct TitleFactoryView
{
    unsigned char unknown00000[0x1B22C];
    int registrationContext1B22C;
    unsigned char unknown1B230[0x1B248 - 0x1B230];
    ChainElem *calcChain1B248;
    ChainElem *drawChain1B24C;
    unsigned char unknown1B250[0x1B388 - 0x1B250];
};
typedef char TitleFactorySizeIs1B388[
    (sizeof(TitleFactoryView) == 0x1B388) ? 1 : -1];

extern TitleFactoryView *g_TitleScreenFactory;
extern unsigned char g_TitleRegistrationByte;
extern Chain g_Chain;

int TitleCalcCallback(void *titleScreen);
int TitleDrawCallback(void *titleScreen);
int TitleAddedCallback(void *titleScreen);
int TitleDeletedCallback(void *titleScreen);

int __fastcall RegisterTitleScreen(int registrationContext)
{
    TitleFactoryConstructView *constructed = new TitleFactoryConstructView;
    TitleFactoryView *title =
        reinterpret_cast<TitleFactoryView *>(constructed);

    g_TitleScreenFactory = title;
    memset(title, 0, sizeof(TitleFactoryView));
    g_TitleRegistrationByte = 0;
    title->registrationContext1B22C = registrationContext;

    title->calcChain1B248 =
        g_Chain.CreateElem(reinterpret_cast<ChainCallback>(TitleCalcCallback));
    title->calcChain1B248->arg = title;
    title->calcChain1B248->addedCallback =
        reinterpret_cast<ChainLifetimeCallback>(TitleAddedCallback);
    title->calcChain1B248->deletedCallback =
        reinterpret_cast<ChainLifetimeCallback>(TitleDeletedCallback);
    if (g_Chain.AddToCalcChain(title->calcChain1B248, 4) != 0)
        return -1;

    title->drawChain1B24C =
        g_Chain.CreateElem(reinterpret_cast<ChainCallback>(TitleDrawCallback));
    title->drawChain1B24C->arg = title;
    g_Chain.AddToDrawChain(title->drawChain1B24C, 3);
    return 0;
}
