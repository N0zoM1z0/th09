#include "AnmManager.hpp"
#include "AsciiManager.hpp"
#include "FrontSupport.hpp"
#include "GameManagerMode.hpp"

#include <stddef.h>

struct FrontAddedVertex
{
    Float3 pos;
    float rhw;
    unsigned long diffuse;
};
typedef char FrontAddedVertexSizeIs14[
    (sizeof(FrontAddedVertex) == 0x14) ? 1 : -1];

struct FrontAddedAnmLoadedView
{
    unsigned char unknown00[4];
    void *rawData04;
};

struct FrontAddedMessageRuntimeView
{
    unsigned char storage00[0x1D70];

    int Reset();
    int LoadForSideCount(
        char *primaryPath,
        char *secondaryPath,
        int startIndex);
};

struct FrontAddedMessageOwnerView
{
    void InitializeMessageRuntime();
};

struct FrontAddedView
{
    unsigned char unknown00000[0x08];
    AnmLoaded *frontAnm08;
    AnmVm frontVms00C[7];
    unsigned char unknown01288[0xE86C - 0x1288];
    FrontAddedVertex panelVerticesE86C[2][5];
    unsigned char unknown0E934[0xE944 - 0xE934];
    FrontAddedMessageRuntimeView messageRuntimeE944;
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
extern int g_GameSide0Value20;
extern int g_GameSide1Value20;
extern int g_FrontCharacterIndex;
extern const char *g_PlayerResourcePaths[][6];

int FrontAddedCallback(void *frontArg)
{
    FrontAddedView *front = static_cast<FrontAddedView *>(frontArg);

    AnmManager *anmManager = g_AnmManager;
    AnmLoaded *frontAnm = anmManager->GetAnm(10);
    if (reinterpret_cast<FrontAddedAnmLoadedView *>(frontAnm)->rawData04 == 0)
        frontAnm = anmManager->PreloadAnm(10, "front.anm");

    front->frontAnm08 = frontAnm;
    if (frontAnm == 0)
        return -1;

    frontAnm->ExecuteAnmIdx(&front->frontVms00C[0], 0);
    front->frontAnm08->ExecuteAnmIdx(&front->frontVms00C[1], 1);
    front->frontAnm08->ExecuteAnmIdx(&front->frontVms00C[2], 2);
    front->frontAnm08->ExecuteAnmIdx(&front->frontVms00C[3], 3);
    front->frontAnm08->ExecuteAnmIdx(&front->frontVms00C[4], 4);
    front->frontAnm08->ExecuteAnmIdx(&front->frontVms00C[5], 5);
    front->frontAnm08->ExecuteAnmIdx(&front->frontVms00C[6], 6);

    g_AsciiManager.asciiAnm->ExecuteAnmIdx(&front->markerVm123F4, 0);
    g_AsciiManager.asciiAnm->ExecuteAnmIdx(&front->markerVm12698, 0);
    reinterpret_cast<unsigned char *>(&front->markerVm123F4)[0x1F3] = 0;
    reinterpret_cast<unsigned char *>(&front->markerVm12698)[0x1F3] = 0;

    if (g_GameManager.IsGameMode0() || g_GameManager.IsGameMode1())
    {
        front->frontAnm08->ExecuteAnmIdx(&front->resultVm11EAC, 85);
        front->frontAnm08->ExecuteAnmIdx(&front->resultVm12150, 86);
        front->frontAnm08->SetSprite(
            &front->resultVm12150,
            g_FrontCharacterIndex + 73);
    }

    int panel = 0;
    do
    {
        FrontAddedVertex *vertex = &front->panelVerticesE86C[panel][0];
        int vertexCount = 5;
        do
        {
            vertex->rhw = 1.0f;
            (vertex++)->diffuse = 0x80FFFFFFUL;
        }
        while (--vertexCount != 0);
    }
    while (++panel != 2);

    front->panelVerticesE86C[0][2].pos.x =
        front->panelVerticesE86C[0][3].pos.x = 304.0f;

    Float3 *leftCorner = &front->panelVerticesE86C[0][4].pos;
    leftCorner->x = 15.0f;
    leftCorner->y = 15.0f;
    front->panelVerticesE86C[1][0].pos.y = 15.0f;
    leftCorner->z = 0.0f;

    front->panelVerticesE86C[0][0].pos.x =
        front->panelVerticesE86C[0][0].pos.y =
        front->panelVerticesE86C[0][1].pos.x =
        front->panelVerticesE86C[0][3].pos.y =
        front->panelVerticesE86C[1][3].pos.y = 15.0f;

    front->panelVerticesE86C[0][1].pos.y =
        front->panelVerticesE86C[0][2].pos.y =
        front->panelVerticesE86C[1][1].pos.y =
        front->panelVerticesE86C[1][2].pos.y = 464.0f;

    front->panelVerticesE86C[1][0].pos.x = 335.0f;
    front->panelVerticesE86C[1][0].pos.z = 0.0f;

    front->panelVerticesE86C[1][2].pos.x =
        front->panelVerticesE86C[1][3].pos.x = 624.0f;
    front->panelVerticesE86C[1][4].pos =
        front->panelVerticesE86C[1][0].pos;

    front->panelVerticesE86C[0][0].pos.z =
        front->panelVerticesE86C[0][1].pos.z =
        front->panelVerticesE86C[0][2].pos.z =
        front->panelVerticesE86C[0][3].pos.z = 0.0f;

    front->panelVerticesE86C[1][1].pos.x = 335.0f;
    front->panelVerticesE86C[1][1].pos.z =
        front->panelVerticesE86C[1][2].pos.z =
        front->panelVerticesE86C[1][3].pos.z = 0.0f;

    front->messageRuntimeE944.Reset();

    if (g_GameManager.IsGameMode0() || g_GameManager.IsGameMode1())
    {
        if (front->messageRuntimeE944.LoadForSideCount(
            const_cast<char *>(g_PlayerResourcePaths[g_GameSide0Value20][3]),
            0,
            10 * g_GameSide1Value20) != 0)
            return -1;
    }
    else
    {
        if (front->messageRuntimeE944.LoadForSideCount(
            const_cast<char *>(g_PlayerResourcePaths[g_GameSide0Value20][4]),
            const_cast<char *>(g_PlayerResourcePaths[g_GameSide1Value20][4]),
            0) != 0)
            return -1;
    }

    reinterpret_cast<FrontAddedMessageOwnerView *>(front)
        ->InitializeMessageRuntime();
    return 0;
}
