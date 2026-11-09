#pragma once

#include "AnmVmLifecycle.hpp"
#include "Chain.hpp"
#include "FrontMessageRuntimeView.hpp"

#include <stddef.h>

class AnmLoaded;

// The front panel contains ten real 0x14-byte vertex records. Their
// original constructor has no calls; Float3's nontrivial constructor
// must not be introduced into this original vertex array.
struct FrontPanelVertexView
{
    float x, y, z;
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
    FrontPanelVertexView panelVerticesE86C[10];
    unsigned char unknownE934[0x10];
    FrontMessageRuntimeView messageRuntimeE944;
    AnmVm vm106B4;
    unsigned char unknown10958[0x10];
    AnmVm resultVms10968[8];
    unsigned char unknown11E88[0x24];
    AnmVm resultVms11EAC[2];
    AnmVm markerVms123F4[2];

    FrontFactoryConstructView();
};

typedef char FrontPanelVertexNativeSize[
    (sizeof(FrontPanelVertexView) == 0x14) ? 1 : -1];
typedef char FrontFactoryConstructNativeLayout[
    (sizeof(FrontFactoryConstructView) == 0x1293C &&
     offsetof(FrontFactoryConstructView, frontVms00C) == 0x0C &&
     offsetof(FrontFactoryConstructView, panelVerticesE86C) == 0xE86C &&
     offsetof(FrontFactoryConstructView, messageRuntimeE944) == 0xE944 &&
     offsetof(FrontFactoryConstructView, vm106B4) == 0x106B4 &&
     offsetof(FrontFactoryConstructView, resultVms10968) == 0x10968 &&
     offsetof(FrontFactoryConstructView, resultVms11EAC) == 0x11EAC &&
     offsetof(FrontFactoryConstructView, markerVms123F4) == 0x123F4) ? 1 : -1];
