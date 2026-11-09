#pragma once

#include "AnmVmLifecycle.hpp"
#include "Chain.hpp"
#include "FrontMessageRuntimeView.hpp"

#include <stddef.h>

class AnmLoaded;

// The front panel contains ten real 0x14-byte vertex records. Their
// original constructor has no calls; Float3's nontrivial constructor
// must not be introduced into this original vertex array.
struct FrontPanelPositionView
{
    float x, y, z;
};
struct FrontPanelVertexView
{
    FrontPanelPositionView pos;
    float rhw;
    unsigned long diffuse;

    FrontPanelVertexView();
};

// Full native 0x1293C-byte allocation composed of the observed member
// arrays and message runtime; no hand-written calls to the VC7 EH iterator.
struct FrontFactoryConstructView
{
    ChainElem *calcChain00;
    ChainElem *drawChain04;
    AnmLoaded *frontAnm08;
    AnmVm frontVms00C[88];
    FrontPanelVertexView panelVerticesE86C[2][5];
    int panelColorsE934[2];
    int panelTimersE93C[2];
    FrontMessageRuntimeView messageRuntimeE944;
    AnmVm vm106B4;
    int dynamicSprite10958;
    int transitionVmCount1095C;
    int transitionFrame10960;
    int transitionSide10964;
    AnmVm resultVms10968[8];
    int resultValues11E88[7];
    int transitionStarted11EA4;
    int transitionState11EA8;
    AnmVm resultVms11EAC[2];
    AnmVm markerVms123F4[2];

    FrontFactoryConstructView();
    void InitializeMessageRuntime();
};

typedef char FrontPanelVertexNativeSize[
    (sizeof(FrontPanelVertexView) == 0x14 &&
     sizeof(FrontPanelPositionView) == 0x0C &&
     offsetof(FrontPanelVertexView, pos) == 0x00 &&
     offsetof(FrontPanelVertexView, rhw) == 0x0C &&
     offsetof(FrontPanelVertexView, diffuse) == 0x10) ? 1 : -1];
typedef char FrontFactoryConstructNativeLayout[
    (sizeof(FrontFactoryConstructView) == 0x1293C &&
     offsetof(FrontFactoryConstructView, frontVms00C) == 0x0C &&
     offsetof(FrontFactoryConstructView, panelVerticesE86C) == 0xE86C &&
     offsetof(FrontFactoryConstructView, messageRuntimeE944) == 0xE944 &&
     offsetof(FrontFactoryConstructView, panelColorsE934) == 0xE934 &&
     offsetof(FrontFactoryConstructView, panelTimersE93C) == 0xE93C &&
     offsetof(FrontFactoryConstructView, vm106B4) == 0x106B4 &&
     offsetof(FrontFactoryConstructView, dynamicSprite10958) == 0x10958 &&
     offsetof(FrontFactoryConstructView, transitionVmCount1095C) == 0x1095C &&
     offsetof(FrontFactoryConstructView, transitionFrame10960) == 0x10960 &&
     offsetof(FrontFactoryConstructView, transitionSide10964) == 0x10964 &&
     offsetof(FrontFactoryConstructView, resultVms10968) == 0x10968 &&
     offsetof(FrontFactoryConstructView, resultValues11E88) == 0x11E88 &&
     offsetof(FrontFactoryConstructView, transitionStarted11EA4) == 0x11EA4 &&
     offsetof(FrontFactoryConstructView, transitionState11EA8) == 0x11EA8 &&
     offsetof(FrontFactoryConstructView, resultVms11EAC) == 0x11EAC &&
     offsetof(FrontFactoryConstructView, markerVms123F4) == 0x123F4) ? 1 : -1];
