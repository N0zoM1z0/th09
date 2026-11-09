#include "AnmManager.hpp"
#include "AsciiGameManagerView.hpp"
#include "AsciiManager.hpp"
#include "Supervisor.hpp"
#include "FrontFactoryConstructView.hpp"

#include <stddef.h>

// Render the two five-vertex panel strips from their constructed native
// FrontPanelVertexView storage, without a separate vertex allocation.
typedef char FrontDrawNativePanelVertex[
    (sizeof(FrontPanelVertexView) == 0x14 &&
     offsetof(FrontPanelVertexView, rhw) == 0x0C &&
     offsetof(FrontPanelVertexView, diffuse) == 0x10) ? 1 : -1];

int __fastcall FrontDrawLineStrip(
    FrontPanelVertexView *vertices, int primitiveCount);

extern AsciiManager g_AsciiManager;
extern int g_GameTimeFrames;
extern int g_GameStageValue;

int __fastcall FrontDrawCallback(void *frontArg)
{
    FrontFactoryConstructView *front = static_cast<FrontFactoryConstructView *>(frontArg);

    g_Supervisor.ConfigureScreenViewport(2);

    g_AnmManager->DrawNoRotation(&front->frontVms00C[0]);
    g_AnmManager->DrawNoRotation(&front->frontVms00C[1]);
    g_AnmManager->DrawNoRotation(&front->frontVms00C[2]);
    g_AnmManager->DrawNoRotation(&front->frontVms00C[3]);
    g_AnmManager->DrawNoRotation(&front->frontVms00C[4]);
    g_AnmManager->DrawNoRotation(&front->frontVms00C[5]);
    g_AnmManager->DrawNoRotation(&front->frontVms00C[6]);
    g_AnmManager->DrawNoRotation(&front->markerVms123F4[0]);
    g_AnmManager->DrawNoRotation(&front->markerVms123F4[1]);

    FrontDrawLineStrip((&front->panelVerticesE86C[0][0]), 4);
    FrontDrawLineStrip((&front->panelVerticesE86C[1][0]), 4);

    front->messageRuntimeE944.Draw();

    if (front->transitionVmCount1095C != 0)
    {
        Float3 scorePosition;
        scorePosition.z = 0.0f;

        g_AnmManager->DrawNoRotation(&front->resultVms10968[0]);

        g_Supervisor.ConfigureScreenViewport(0);

        front->resultVms10968[1].pos.x =
            g_GameManager.TransformPopupX(
                front->resultVms10968[1].pos2.x);
        front->resultVms10968[1].pos.y =
            g_GameManager.TransformPopupY(
                front->resultVms10968[1].pos2.y);
        g_AnmManager->DrawNoRotation(&front->resultVms10968[1]);

        if (front->transitionVmCount1095C >= 2 &&
            front->transitionSide10964 == 0)
        {
            front->resultVms10968[3].pos.x =
                g_GameManager.TransformPopupX(
                    front->resultVms10968[3].pos2.x);
            front->resultVms10968[3].pos.y =
                g_GameManager.TransformPopupY(
                    front->resultVms10968[3].pos2.y);
            g_AnmManager->DrawNoRotation(
                &front->resultVms10968[3]);

            front->resultVms10968[4].pos.x =
                g_GameManager.TransformPopupX(
                    front->resultVms10968[4].pos2.x);
            front->resultVms10968[4].pos.y =
                g_GameManager.TransformPopupY(
                    front->resultVms10968[4].pos2.y);
            g_AnmManager->DrawNoRotation(
                &front->resultVms10968[4]);

            front->resultVms10968[5].pos.x =
                g_GameManager.TransformPopupX(
                    front->resultVms10968[5].pos2.x);
            front->resultVms10968[5].pos.y =
                g_GameManager.TransformPopupY(
                    front->resultVms10968[5].pos2.y);
            g_AnmManager->DrawNoRotation(
                &front->resultVms10968[5]);

            front->resultVms10968[6].pos.x =
                g_GameManager.TransformPopupX(
                    front->resultVms10968[6].pos2.x);
            front->resultVms10968[6].pos.y =
                g_GameManager.TransformPopupY(
                    front->resultVms10968[6].pos2.y);
            g_AnmManager->DrawNoRotation(
                &front->resultVms10968[6]);

            scorePosition.x = 224.0f;
            scorePosition.y = 226.0f;
        }

        g_Supervisor.ConfigureScreenViewport(1);

        front->resultVms10968[2].pos.x =
            g_GameManager.TransformPopupX(
                front->resultVms10968[2].pos2.x);
        front->resultVms10968[2].pos.y =
            g_GameManager.TransformPopupY(
                front->resultVms10968[2].pos2.y);
        g_AnmManager->DrawNoRotation(&front->resultVms10968[2]);

        if (front->transitionVmCount1095C >= 2 &&
            front->transitionSide10964 == 1)
        {
            front->resultVms10968[3].pos.x =
                g_GameManager.TransformPopupX(
                    front->resultVms10968[3].pos2.x);
            front->resultVms10968[3].pos.y =
                g_GameManager.TransformPopupY(
                    front->resultVms10968[3].pos2.y);
            g_AnmManager->DrawNoRotation(
                &front->resultVms10968[3]);

            front->resultVms10968[6].pos.x =
                g_GameManager.TransformPopupX(
                    front->resultVms10968[6].pos2.x);
            front->resultVms10968[6].pos.y =
                g_GameManager.TransformPopupY(
                    front->resultVms10968[6].pos2.y);
            g_AnmManager->DrawNoRotation(
                &front->resultVms10968[6]);

            scorePosition.x = 544.0f;
            scorePosition.y = 226.0f;
        }

        if (front->transitionVmCount1095C >= 2 &&
            front->transitionFrame10960 > 20)
        {
            Float3 *position =
                &scorePosition;

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

    if (g_GameManager.IsGameMode2())
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

    g_AnmManager->DrawNoRotation(&front->vm106B4);
    g_AnmManager->DrawNoRotation(&front->resultVms11EAC[0]);
    g_AnmManager->DrawNoRotation(&front->resultVms11EAC[1]);
    return 1;
}
