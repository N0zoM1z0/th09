// SPDX-License-Identifier: GPL-3.0-only
// Adapted from Cirno-Labs/th09 src/gui.cpp Gui::AddedCallback,
// commit d7653152becabb8c86f2f43d49015ecc11bd0652.
// Original MIT portions retain their grant in LICENSE; see THIRD_PARTY_NOTICES.md.
#include "AnmManager.hpp"
#include "AsciiManager.hpp"
#include "FrontFactoryConstructView.hpp"
#include "FrontNativePanelVertices.hpp"
#include "GameManagerRuntimeView.hpp"

#include <stddef.h>

extern AsciiManager g_AsciiManager;
extern const char *g_PlayerResourcePaths[][6];

int FrontAddedCallback(void *frontArg)
{
    FrontFactoryConstructView *gui = static_cast<FrontFactoryConstructView *>(frontArg);
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
    g_AsciiManager.asciiAnm->ExecuteAnmIdx(&gui->markerVms123F4[0], 0);
    g_AsciiManager.asciiAnm->ExecuteAnmIdx(&gui->markerVms123F4[1], 0);
    gui->markerVms123F4[0].color1Script.a = 0;
    gui->markerVms123F4[1].color1Script.a = 0;
    if (g_GameManager.IsGameMode0() || g_GameManager.IsGameMode1())
    {
        gui->frontAnm08->ExecuteAnmIdx(&gui->resultVms11EAC[0], 0x55);
        gui->frontAnm08->ExecuteAnmIdx(&gui->resultVms11EAC[1], 0x56);
        gui->frontAnm08->SetSprite(&gui->resultVms11EAC[1], g_GameManager.field0FC + 0x49);
    }
    for (i = 0; i < 2; i++)
    {
        for (j = 0; j < 5; j++)
        {
            FRONT_NATIVE_VERTEX(gui, i, j).rhw = 1.0f;
            FRONT_NATIVE_VERTEX(gui, i, j).diffuse = 0x80ffffff;
        }
    }
    FRONT_NATIVE_VERTEX(gui, 0, 0).pos.x = 15.0f;
    FRONT_NATIVE_VERTEX(gui, 0, 0).pos.y = 15.0f;
    FRONT_NATIVE_VERTEX(gui, 0, 0).pos.z = 0.0f;
    FRONT_NATIVE_VERTEX(gui, 0, 1).pos.x = 15.0f;
    FRONT_NATIVE_VERTEX(gui, 0, 1).pos.y = 464.0f;
    FRONT_NATIVE_VERTEX(gui, 0, 1).pos.z = 0.0f;
    FRONT_NATIVE_VERTEX(gui, 0, 2).pos.x = 304.0f;
    FRONT_NATIVE_VERTEX(gui, 0, 2).pos.y = 464.0f;
    FRONT_NATIVE_VERTEX(gui, 0, 2).pos.z = 0.0f;
    FRONT_NATIVE_VERTEX(gui, 0, 3).pos.x = 304.0f;
    FRONT_NATIVE_VERTEX(gui, 0, 3).pos.y = 15.0f;
    FRONT_NATIVE_VERTEX(gui, 0, 3).pos.z = 0.0f;
    FRONT_NATIVE_VERTEX(gui, 0, 4).pos = FRONT_NATIVE_VERTEX(gui, 0, 0).pos;
    FRONT_NATIVE_VERTEX(gui, 1, 0).pos.x = 335.0f;
    FRONT_NATIVE_VERTEX(gui, 1, 0).pos.y = 15.0f;
    FRONT_NATIVE_VERTEX(gui, 1, 0).pos.z = 0.0f;
    FRONT_NATIVE_VERTEX(gui, 1, 1).pos.x = 335.0f;
    FRONT_NATIVE_VERTEX(gui, 1, 1).pos.y = 464.0f;
    FRONT_NATIVE_VERTEX(gui, 1, 1).pos.z = 0.0f;
    FRONT_NATIVE_VERTEX(gui, 1, 2).pos.x = 624.0f;
    FRONT_NATIVE_VERTEX(gui, 1, 2).pos.y = 464.0f;
    FRONT_NATIVE_VERTEX(gui, 1, 2).pos.z = 0.0f;
    FRONT_NATIVE_VERTEX(gui, 1, 3).pos.x = 624.0f;
    FRONT_NATIVE_VERTEX(gui, 1, 3).pos.y = 15.0f;
    FRONT_NATIVE_VERTEX(gui, 1, 3).pos.z = 0.0f;
    FRONT_NATIVE_VERTEX(gui, 1, 4).pos = FRONT_NATIVE_VERTEX(gui, 1, 0).pos;
    gui->messageRuntimeE944.Reset();
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
    gui->InitializeMessageRuntime();
    return 0;
}
