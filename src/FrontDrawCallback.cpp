#include "AnmManager.hpp"
#include "AsciiGameManagerView.hpp"
#include "AsciiManager.hpp"
#include "Supervisor.hpp"

#include <stddef.h>

struct FrontLineVertex
{
    float x;
    float y;
    float z;
    float rhw;
    unsigned long diffuse;
};
typedef char FrontDrawLineVertexSizeIs14[
    (sizeof(FrontLineVertex) == 0x14) ? 1 : -1];

int __fastcall FrontDrawLineStrip(
    FrontLineVertex *vertices,
    int primitiveCount);

struct FrontMessageRuntimeView
{
    int Draw();
};

struct GameManagerModeView
{
    int IsGameMode2();
};

struct FrontDrawTextPosition
{
    float x;
    float y;
    float z;
};

struct FrontDrawView
{
    unsigned char unknown00000[0x0C];
    AnmVm baseVms00C[7];
    unsigned char unknown01288[0xE86C - 0x1288];
    FrontLineVertex panelVerticesE86C[2][5];
    unsigned char panelTailE934[0x10];
    unsigned char messageRuntimeE944[0x1D70];
    AnmVm dynamicVm106B4;
    int dynamicSprite10958;
    int transitionVmCount1095C;
    int transitionFrame10960;
    int transitionSide10964;
    AnmVm transitionVms10968[8];
    int resultValues11E88[7];
    int transitionStarted11EA4;
    int transitionState11EA8;
    AnmVm resultVm11EAC;
    AnmVm resultVm12150;
    AnmVm markerVm123F4;
    AnmVm markerVm12698;
};

typedef char FrontDrawSizeIs1293C[
    (sizeof(FrontDrawView) == 0x1293C) ? 1 : -1];
typedef char FrontDrawPanelAtE86C[
    (offsetof(FrontDrawView, panelVerticesE86C) == 0xE86C) ? 1 : -1];
typedef char FrontDrawMessageAtE944[
    (offsetof(FrontDrawView, messageRuntimeE944) == 0xE944) ? 1 : -1];
typedef char FrontDrawDynamicAt106B4[
    (offsetof(FrontDrawView, dynamicVm106B4) == 0x106B4) ? 1 : -1];
typedef char FrontDrawTransitionAt10968[
    (offsetof(FrontDrawView, transitionVms10968) == 0x10968) ? 1 : -1];
typedef char FrontDrawValuesAt11E88[
    (offsetof(FrontDrawView, resultValues11E88) == 0x11E88) ? 1 : -1];
typedef char FrontDrawResultAt11EAC[
    (offsetof(FrontDrawView, resultVm11EAC) == 0x11EAC) ? 1 : -1];

extern AsciiManager g_AsciiManager;
extern int g_GameTimeFrames;
extern int g_GameStageValue;

int __fastcall FrontDrawCallback(void *frontArg)
{
    FrontDrawView *front = static_cast<FrontDrawView *>(frontArg);

    g_Supervisor.ConfigureScreenViewport(2);

    g_AnmManager->DrawNoRotation(&front->baseVms00C[0]);
    g_AnmManager->DrawNoRotation(&front->baseVms00C[1]);
    g_AnmManager->DrawNoRotation(&front->baseVms00C[2]);
    g_AnmManager->DrawNoRotation(&front->baseVms00C[3]);
    g_AnmManager->DrawNoRotation(&front->baseVms00C[4]);
    g_AnmManager->DrawNoRotation(&front->baseVms00C[5]);
    g_AnmManager->DrawNoRotation(&front->baseVms00C[6]);
    g_AnmManager->DrawNoRotation(&front->markerVm123F4);
    g_AnmManager->DrawNoRotation(&front->markerVm12698);

    FrontDrawLineStrip(&front->panelVerticesE86C[0][0], 4);
    FrontDrawLineStrip(&front->panelVerticesE86C[1][0], 4);

    reinterpret_cast<FrontMessageRuntimeView *>(
        &front->messageRuntimeE944)->Draw();

    if (front->transitionVmCount1095C != 0)
    {
        FrontDrawTextPosition scorePosition;
        scorePosition.z = 0.0f;

        g_AnmManager->DrawNoRotation(&front->transitionVms10968[0]);

        g_Supervisor.ConfigureScreenViewport(0);

        front->transitionVms10968[1].pos.x =
            g_GameManager.TransformPopupX(
                front->transitionVms10968[1].pos2.x);
        front->transitionVms10968[1].pos.y =
            g_GameManager.TransformPopupY(
                front->transitionVms10968[1].pos2.y);
        g_AnmManager->DrawNoRotation(&front->transitionVms10968[1]);

        if (front->transitionVmCount1095C >= 2 &&
            front->transitionSide10964 == 0)
        {
            front->transitionVms10968[3].pos.x =
                g_GameManager.TransformPopupX(
                    front->transitionVms10968[3].pos2.x);
            front->transitionVms10968[3].pos.y =
                g_GameManager.TransformPopupY(
                    front->transitionVms10968[3].pos2.y);
            g_AnmManager->DrawNoRotation(
                &front->transitionVms10968[3]);

            front->transitionVms10968[4].pos.x =
                g_GameManager.TransformPopupX(
                    front->transitionVms10968[4].pos2.x);
            front->transitionVms10968[4].pos.y =
                g_GameManager.TransformPopupY(
                    front->transitionVms10968[4].pos2.y);
            g_AnmManager->DrawNoRotation(
                &front->transitionVms10968[4]);

            front->transitionVms10968[5].pos.x =
                g_GameManager.TransformPopupX(
                    front->transitionVms10968[5].pos2.x);
            front->transitionVms10968[5].pos.y =
                g_GameManager.TransformPopupY(
                    front->transitionVms10968[5].pos2.y);
            g_AnmManager->DrawNoRotation(
                &front->transitionVms10968[5]);

            front->transitionVms10968[6].pos.x =
                g_GameManager.TransformPopupX(
                    front->transitionVms10968[6].pos2.x);
            front->transitionVms10968[6].pos.y =
                g_GameManager.TransformPopupY(
                    front->transitionVms10968[6].pos2.y);
            g_AnmManager->DrawNoRotation(
                &front->transitionVms10968[6]);

            scorePosition.x = 224.0f;
            scorePosition.y = 226.0f;
        }

        g_Supervisor.ConfigureScreenViewport(1);

        front->transitionVms10968[2].pos.x =
            g_GameManager.TransformPopupX(
                front->transitionVms10968[2].pos2.x);
        front->transitionVms10968[2].pos.y =
            g_GameManager.TransformPopupY(
                front->transitionVms10968[2].pos2.y);
        g_AnmManager->DrawNoRotation(&front->transitionVms10968[2]);

        if (front->transitionVmCount1095C >= 2 &&
            front->transitionSide10964 == 1)
        {
            front->transitionVms10968[3].pos.x =
                g_GameManager.TransformPopupX(
                    front->transitionVms10968[3].pos2.x);
            front->transitionVms10968[3].pos.y =
                g_GameManager.TransformPopupY(
                    front->transitionVms10968[3].pos2.y);
            g_AnmManager->DrawNoRotation(
                &front->transitionVms10968[3]);

            front->transitionVms10968[6].pos.x =
                g_GameManager.TransformPopupX(
                    front->transitionVms10968[6].pos2.x);
            front->transitionVms10968[6].pos.y =
                g_GameManager.TransformPopupY(
                    front->transitionVms10968[6].pos2.y);
            g_AnmManager->DrawNoRotation(
                &front->transitionVms10968[6]);

            scorePosition.x = 544.0f;
            scorePosition.y = 226.0f;
        }

        if (front->transitionVmCount1095C >= 2 &&
            front->transitionFrame10960 > 20)
        {
            Float3 *position =
                reinterpret_cast<Float3 *>(&scorePosition);

            g_AsciiManager.AddFormatText(
                position, "%7d", front->resultValues11E88[0]);
            scorePosition.y += 16.0f;
            g_AsciiManager.AddFormatText(
                position, "%7d", front->resultValues11E88[1]);
            scorePosition.y += 16.0f;
            g_AsciiManager.AddFormatText(
                position, "%7d", front->resultValues11E88[2]);
            scorePosition.y += 16.0f;
            g_AsciiManager.AddFormatText(
                position, "%7d", front->resultValues11E88[3]);
            scorePosition.y += 16.0f;
            g_AsciiManager.AddFormatText(
                position, "%7d", front->resultValues11E88[4]);
            scorePosition.y += 16.0f;
            scorePosition.x -= 9.0f;

            if (g_GameStageValue == 8)
            {
                g_AsciiManager.AddFormatText(
                    position, "%8d", front->resultValues11E88[5]);
            }

            scorePosition.y += 32.0f;
            g_AsciiManager.AddFormatText(
                position, "%8d", front->resultValues11E88[6]);
        }
    }

    g_Supervisor.ConfigureScreenViewport(2);

    if (reinterpret_cast<GameManagerModeView *>(
            &g_GameManager)->IsGameMode2())
    {
        Float3 position(297.0f, 0.0f, 0.0f);
        g_AsciiManager.AddFormatText(
            &position,
            "%.2d:%.2d",
            g_GameTimeFrames / 3600,
            (g_GameTimeFrames / 60) % 60);
    }
    else
    {
        Float3 position(260.0f, 0.0f, 0.0f);
        g_AsciiManager.AddFormatText(
            &position,
            "STAGE %d %.2d:%.2d",
            g_GameStageValue + 1,
            g_GameTimeFrames / 3600,
            (g_GameTimeFrames / 60) % 60);
    }

    g_AnmManager->DrawNoRotation(&front->dynamicVm106B4);
    g_AnmManager->DrawNoRotation(&front->resultVm11EAC);
    g_AnmManager->DrawNoRotation(&front->resultVm12150);
    return 1;
}
