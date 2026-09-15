#include "Chain.hpp"
#include "ZunMemory.hpp"

#include <stdlib.h>
#include <string.h>

class ZunTimer
{
  public:
    void operator=(int value);
};

struct PlayerLifecycleView
{
    unsigned char unknown00[0x08];
    int sideIndex;
    void *sideState;
    void *opponentState;
    ChainElem *calcChain;
    ChainElem *drawChainHighPrio;
    ChainElem *drawChainLowPrio;
    int selector;
    unsigned char unknown24[0x30F70 - 0x24];

    PlayerLifecycleView();
    void InitializeType(unsigned char playerType);
    void ReleaseOwnedState();

    static int __fastcall OnUpdate(PlayerLifecycleView *player);
    static int __fastcall AddedCallback(PlayerLifecycleView *player);
    static int __fastcall OnDrawHighPrio(PlayerLifecycleView *player);
    static int __fastcall OnDrawLowPrio(PlayerLifecycleView *player);
};

typedef char PlayerLifecycleSizeIs30F70[(sizeof(PlayerLifecycleView) == 0x30F70) ? 1 : -1];
typedef char PlayerLifecycleSideIndexAt08[(offsetof(PlayerLifecycleView, sideIndex) == 0x08) ? 1 : -1];
typedef char PlayerLifecycleSideStateAt0C[(offsetof(PlayerLifecycleView, sideState) == 0x0C) ? 1 : -1];
typedef char PlayerLifecycleOpponentAt10[(offsetof(PlayerLifecycleView, opponentState) == 0x10) ? 1 : -1];
typedef char PlayerLifecycleCalcAt14[(offsetof(PlayerLifecycleView, calcChain) == 0x14) ? 1 : -1];
typedef char PlayerLifecycleDrawHighAt18[(offsetof(PlayerLifecycleView, drawChainHighPrio) == 0x18) ? 1 : -1];
typedef char PlayerLifecycleDrawLowAt1C[(offsetof(PlayerLifecycleView, drawChainLowPrio) == 0x1C) ? 1 : -1];
typedef char PlayerLifecycleSelectorAt20[(offsetof(PlayerLifecycleView, selector) == 0x20) ? 1 : -1];

extern unsigned char g_GameManager[];
extern Chain g_Chain;
extern ZunMemory g_ZunMemory;

PlayerLifecycleView *__fastcall PlayerRegisterChain(unsigned char playerType, int sideIndex, int selector)
{
    PlayerLifecycleView *player = static_cast<PlayerLifecycleView *>(
        g_ZunMemory.AddToRegistry(new PlayerLifecycleView, sizeof(PlayerLifecycleView),
                                  const_cast<char *>("PlayerInf")));

    memset(player, 0, sizeof(PlayerLifecycleView));
    *reinterpret_cast<ZunTimer *>(reinterpret_cast<unsigned char *>(player) + 0x303C8) = 0;
    player->InitializeType(playerType);

    player->sideState = g_GameManager + sideIndex * 0x38;
    player->sideIndex = sideIndex;
    player->opponentState = g_GameManager + 0x38 - sideIndex * 0x38;
    player->selector = selector;

    player->calcChain = g_Chain.CreateElem(
        reinterpret_cast<ChainCallback>(PlayerLifecycleView::OnUpdate));
    player->calcChain->arg = player;
    player->calcChain->addedCallback =
        reinterpret_cast<ChainLifetimeCallback>(PlayerLifecycleView::AddedCallback);
    if (g_Chain.AddToCalcChain(player->calcChain, sideIndex + 15) == 0) {
        player->drawChainHighPrio = g_Chain.CreateElem(
            reinterpret_cast<ChainCallback>(PlayerLifecycleView::OnDrawHighPrio));
        player->drawChainLowPrio = g_Chain.CreateElem(
            reinterpret_cast<ChainCallback>(PlayerLifecycleView::OnDrawLowPrio));
        player->drawChainHighPrio->arg = player;
        player->drawChainLowPrio->arg = player;
        g_Chain.AddToDrawChain(player->drawChainHighPrio, sideIndex + 12);
        g_Chain.AddToDrawChain(player->drawChainLowPrio, sideIndex + 14);
    }
    return player;
}

void __fastcall PlayerRelease(PlayerLifecycleView *player)
{
    if (player != NULL) {
        player->ReleaseOwnedState();
        g_Chain.Cut(player->drawChainHighPrio);
        player->drawChainHighPrio = NULL;
        g_Chain.Cut(player->drawChainLowPrio);
        player->drawChainLowPrio = NULL;
        g_Chain.Cut(player->calcChain);
        player->calcChain = NULL;
        free(player);
    }
}
