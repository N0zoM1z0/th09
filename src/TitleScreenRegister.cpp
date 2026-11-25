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

struct TitleScreenView
{
    static int __fastcall AddedCallback(TitleScreenView *titleScreen);
    static int __fastcall DeletedCallback(TitleScreenView *titleScreen);
};

TitleScreenView *g_TitleScreen;
extern unsigned char g_TitleRegistrationByte;
extern Chain g_Chain;

char *g_TitleAlphabet =
    "ABCDEFGHIJKLMNOPQRSTUVWXYZ.,:;_@abcdefghijklmnopqrstuvwxyz+-/*=%0123456789#!?'\"$(){}[]<>&\\|~^ --";

const char *g_DemoReplayPaths[3] = {
    "demo/demorpy0.rpy",
    "demo/demorpy1.rpy",
    "demo/demorpy2.rpy",
};

const char *g_TitleHelpText[8] = {
    "\x89\xD4\x89\x66\x92\xCB\x83\x82\x81\x5B\x83\x68\x82\xF0\x8A\x4A\x8E\x6E\x82\xB5\x82\xDC\x82\xB7", // [0] 花映塚モードを開始します
    "\x83\x47\x83\x4C\x83\x58\x83\x67\x83\x89\x83\x82\x81\x5B\x83\x68\x82\xF0\x8A\x4A\x8E\x6E\x82\xB5\x82\xDC\x82\xB7", // [1] エキストラモードを開始します
    "\x91\xCE\x90\x6C\x90\xED\x81\x41\x91\xCE" "CPU\x90\xED\x81\x41\x83\x45\x83\x48\x83\x62\x83\x60\x83\x82\x81\x5B\x83\x68\x82\xF0\x8A\x4A\x8E\x6E\x82\xB5\x82\xDC\x82\xB7", // [2] 対人戦、対CPU戦、ウォッチモードを開始します
    "\x83\x8A\x83\x76\x83\x8C\x83\x43\x82\xF0\x8D\xC4\x90\xB6\x82\xB5\x82\xDC\x82\xB7", // [3] リプレイを再生します
    "\x8B\x4C\x98\x5E\x82\xF0\x8C\xA9\x82\xDC\x82\xB7", // [4] 記録を見ます
    "\x89\xB9\x8A\x79\x8E\xBA\x82\xC9\x93\xFC\x82\xE8\x82\xDC\x82\xB7", // [5] 音楽室に入ります
    "\x8A\x65\x8E\xED\x90\xDD\x92\xE8\x82\xC5\x82\xAB\x82\xDC\x82\xB7", // [6] 各種設定できます
    "\x82\xA2\x82\xEB\x82\xA2\x82\xEB\x82\xC6\x8F\x49\x97\xB9\x82\xB5\x82\xDC\x82\xB7", // [7] いろいろと終了します
};

char g_TitleCharacterOrder[16] = {
    0, 1, 2, 3, 4, 5, 6, 14, 15, 7, 8, 10, 11, 9, 12, 13
};

char g_TitleCharacterOrder14[16] = {
    0, 1, 2, 3, 4, 5, 6, 7, 8, 10, 11, 9, 12, 13, 0, 0
};

char g_TitleCharacterOrderMode123[16] = {
    0, 1, 2, 3, 4, 5, 6, 14, 15, 7, 8, 10, 11, 9, 12, 13
};

char g_TitleScreen16Order[16] = {
    0, 1, 2, 3, 4, 5, 6, 7, 8, 10, 11, 9, 12, 13, 14, 15
};

char g_TitleScreen16Entries[16];
int g_TitleResultUnlockStep;

int TitleCalcCallback(void *titleScreen);
int TitleDrawCallback(void *titleScreen);

int __fastcall RegisterTitleScreen(int registrationContext)
{
    TitleFactoryConstructView *constructed = new TitleFactoryConstructView;
    TitleFactoryView *title =
        reinterpret_cast<TitleFactoryView *>(constructed);

    g_TitleScreen = reinterpret_cast<TitleScreenView *>(title);
    memset(title, 0, sizeof(TitleFactoryView));
    g_TitleRegistrationByte = 0;
    title->registrationContext1B22C = registrationContext;

    title->calcChain1B248 =
        g_Chain.CreateElem(reinterpret_cast<ChainCallback>(TitleCalcCallback));
    title->calcChain1B248->arg = title;
    title->calcChain1B248->addedCallback =
        reinterpret_cast<ChainLifetimeCallback>(TitleScreenView::AddedCallback);
    title->calcChain1B248->deletedCallback =
        reinterpret_cast<ChainLifetimeCallback>(TitleScreenView::DeletedCallback);
    if (g_Chain.AddToCalcChain(title->calcChain1B248, 4) != 0)
        return -1;

    title->drawChain1B24C =
        g_Chain.CreateElem(reinterpret_cast<ChainCallback>(TitleDrawCallback));
    title->drawChain1B24C->arg = title;
    g_Chain.AddToDrawChain(title->drawChain1B24C, 3);
    return 0;
}
