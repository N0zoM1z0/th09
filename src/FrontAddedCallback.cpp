// SPDX-License-Identifier: GPL-3.0-only
// Adapted from Cirno-Labs/th09 src/gui.cpp Gui::AddedCallback,
// commit d7653152becabb8c86f2f43d49015ecc11bd0652.
// Original MIT portions retain their grant in LICENSE; see THIRD_PARTY_NOTICES.md.
#include "AnmManager.hpp"
#include "AsciiManager.hpp"
#include "FrontSupport.hpp"
#include "FrontMessageOwnerView.hpp"
#include "FrontMessageRuntimeView.hpp"
#include "GameManagerRuntimeView.hpp"

#include <stddef.h>

struct FrontAddedVertex
{
    Float3 pos;
    float rhw;
    unsigned long diffuse;
};
typedef char FrontAddedVertexSizeIs14[
    (sizeof(FrontAddedVertex) == 0x14) ? 1 : -1];

struct FrontAddedView
{
    unsigned char unknown00000[0x08];
    AnmLoaded *frontAnm08;
    AnmVm frontVms00C[7];
    unsigned char unknown01288[0xE86C - 0x1288];
    FrontAddedVertex panelVerticesE86C[2][5];
    unsigned char unknown0E934[0xE944 - 0xE934];
    FrontMessageRuntimeView messageRuntimeE944;
    unsigned char unknown106B4[0x11EAC - 0x106B4];
    AnmVm resultVm11EAC;
    AnmVm resultVm12150;
    AnmVm markerVm123F4;
    AnmVm markerVm12698;
};

typedef char FrontAddedSizeIs1293C[
    (sizeof(FrontAddedView) == 0x1293C) ? 1 : -1];
typedef char FrontAddedVerticesAtE86C[
    (offsetof(FrontAddedView, panelVerticesE86C) == 0xE86C) ? 1 : -1];
typedef char FrontAddedMessageAtE944[
    (offsetof(FrontAddedView, messageRuntimeE944) == 0xE944) ? 1 : -1];
typedef char FrontAddedResultAt11EAC[
    (offsetof(FrontAddedView, resultVm11EAC) == 0x11EAC) ? 1 : -1];
typedef char FrontAddedMarkerAt123F4[
    (offsetof(FrontAddedView, markerVm123F4) == 0x123F4) ? 1 : -1];

extern AsciiManager g_AsciiManager;
extern const char *g_PlayerResourcePaths[][6];

int FrontAddedCallback(void *frontArg)
{
    FrontAddedView *gui = static_cast<FrontAddedView *>(frontArg);
    int i;
    int j;
    AnmManager *se = g_AnmManager;
    AnmLoaded *anm = se->GetAnm(10);

    if (anm->rawData04 == NULL)
    {
        gui->frontAnm08 = se->PreloadAnm(10, "front.anm");
    }
    else
    {
        gui->frontAnm08 = anm;
    }
    if (gui->frontAnm08 == NULL)
    {
        return -1;
    }
    gui->frontAnm08->ExecuteAnmIdx(&gui->frontVms00C[0], 0);
    gui->frontAnm08->ExecuteAnmIdx(&gui->frontVms00C[1], 1);
    gui->frontAnm08->ExecuteAnmIdx(&gui->frontVms00C[2], 2);
    gui->frontAnm08->ExecuteAnmIdx(&gui->frontVms00C[3], 3);
    gui->frontAnm08->ExecuteAnmIdx(&gui->frontVms00C[4], 4);
    gui->frontAnm08->ExecuteAnmIdx(&gui->frontVms00C[5], 5);
    gui->frontAnm08->ExecuteAnmIdx(&gui->frontVms00C[6], 6);
    g_AsciiManager.asciiAnm->ExecuteAnmIdx(&gui->markerVm123F4, 0);
    g_AsciiManager.asciiAnm->ExecuteAnmIdx(&gui->markerVm12698, 0);
    gui->markerVm123F4.color1Script.a = 0;
    gui->markerVm12698.color1Script.a = 0;
    if (g_GameManager.IsGameMode0() || g_GameManager.IsGameMode1())
    {
        gui->frontAnm08->ExecuteAnmIdx(&gui->resultVm11EAC, 0x55);
        gui->frontAnm08->ExecuteAnmIdx(&gui->resultVm12150, 0x56);
        gui->frontAnm08->SetSprite(&gui->resultVm12150, g_GameManager.field0FC + 0x49);
    }
    for (i = 0; i < 2; i++)
    {
        for (j = 0; j < 5; j++)
        {
            gui->panelVerticesE86C[i][j].rhw = 1.0f;
            gui->panelVerticesE86C[i][j].diffuse = 0x80ffffff;
        }
    }
    gui->panelVerticesE86C[0][0].pos.x = 15.0f;
    gui->panelVerticesE86C[0][0].pos.y = 15.0f;
    gui->panelVerticesE86C[0][0].pos.z = 0.0f;
    gui->panelVerticesE86C[0][1].pos.x = 15.0f;
    gui->panelVerticesE86C[0][1].pos.y = 464.0f;
    gui->panelVerticesE86C[0][1].pos.z = 0.0f;
    gui->panelVerticesE86C[0][2].pos.x = 304.0f;
    gui->panelVerticesE86C[0][2].pos.y = 464.0f;
    gui->panelVerticesE86C[0][2].pos.z = 0.0f;
    gui->panelVerticesE86C[0][3].pos.x = 304.0f;
    gui->panelVerticesE86C[0][3].pos.y = 15.0f;
    gui->panelVerticesE86C[0][3].pos.z = 0.0f;
    gui->panelVerticesE86C[0][4].pos = gui->panelVerticesE86C[0][0].pos;
    gui->panelVerticesE86C[1][0].pos.x = 335.0f;
    gui->panelVerticesE86C[1][0].pos.y = 15.0f;
    gui->panelVerticesE86C[1][0].pos.z = 0.0f;
    gui->panelVerticesE86C[1][1].pos.x = 335.0f;
    gui->panelVerticesE86C[1][1].pos.y = 464.0f;
    gui->panelVerticesE86C[1][1].pos.z = 0.0f;
    gui->panelVerticesE86C[1][2].pos.x = 624.0f;
    gui->panelVerticesE86C[1][2].pos.y = 464.0f;
    gui->panelVerticesE86C[1][2].pos.z = 0.0f;
    gui->panelVerticesE86C[1][3].pos.x = 624.0f;
    gui->panelVerticesE86C[1][3].pos.y = 15.0f;
    gui->panelVerticesE86C[1][3].pos.z = 0.0f;
    gui->panelVerticesE86C[1][4].pos = gui->panelVerticesE86C[1][0].pos;
    reinterpret_cast<FrontMessageRuntimeResetView *>(&gui->messageRuntimeE944)->Reset();
    if (g_GameManager.IsGameMode0() || g_GameManager.IsGameMode1())
    {
        if (gui->messageRuntimeE944.LoadForSideCount(const_cast<char *>(g_PlayerResourcePaths[g_GameManager.sides[0].character20][3]), NULL,
                          g_GameManager.sides[1].character20 * 10) != 0)
        {
            return -1;
        }
    }
    else
    {
        if (gui->messageRuntimeE944.LoadForSideCount(const_cast<char *>(g_PlayerResourcePaths[g_GameManager.sides[0].character20][4]),
                          const_cast<char *>(g_PlayerResourcePaths[g_GameManager.sides[1].character20][4]), 0) != 0)
        {
            return -1;
        }
    }
    reinterpret_cast<FrontMessageOwnerView *>(gui)->InitializeMessageRuntime();
    return 0;
}
