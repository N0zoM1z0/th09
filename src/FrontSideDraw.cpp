#include "FrontSide.hpp"
#include "AnmManager.hpp"
#include "GameManagerRuntimeView.hpp"
#include "Float3.hpp"
#include "Supervisor.hpp"

#include <d3d8.h>
#include <stddef.h>

typedef unsigned char u8;
typedef unsigned int u32;

typedef char FrontSideDrawFloat3SizeIs0C[
    (sizeof(Float3) == 0x0C) ? 1 : -1];

// HUD cursors are the +0x208 position member of the real AnmVm.
typedef char FrontSideDrawNativePositionCursor[
    (sizeof(AnmVm) == 0x2A4 &&
     offsetof(AnmVm, pos) == 0x208 &&
     offsetof(AnmVm, pos2) == 0x288 &&
     offsetof(AnmVm, pos2) - offsetof(AnmVm, pos) == 0x80)
        ? 1 : -1];

// The HUD stores the genuine shared 0x38-byte side record at +A670,
// whose native Player points to the original position at +1B88.
typedef char FrontSideDrawNativeSideAndPlayer[
    (offsetof(FrontSide, sideStateA670) == 0xA670 &&
     offsetof(PlayerSideStateView, player04) == 0x04 &&
     offsetof(PlayerLifecycleView, position1B88) == 0x1B88) ? 1 : -1];


typedef char FrontSideDrawSupervisorDeviceAt08[
    (offsetof(Supervisor, d3dDevice08) == 0x08) ? 1 : -1];
typedef char FrontSideDrawSupervisorViewportsAt78[
    (offsetof(Supervisor, ctorArray078) == 0x78) ? 1 : -1];
typedef char FrontSideDrawShakeAt160[
    (offsetof(Supervisor, ctorArray078) +
     offsetof(SupervisorCtorArrayElement, screenShakeOffsetE8) ==
     0x160) ? 1 : -1];

struct FrontSideDrawVertex
{
    FrontSideDrawVertex();
    Float3 pos;
    float rhw;
    u32 diffuse;
};
typedef char FrontSideDrawVertexSizeIs14[
    (sizeof(FrontSideDrawVertex) == 0x14) ? 1 : -1];

// Each HUD cursor is the genuine +0x208 AnmVm::pos subobject.
// Recover its owning native VM through the verified member offset;
// every pos2 access remains a member of that same original VM.


// HUD position cursors advance through the original 0x2A4-byte
// AnmVm allocation; the Supervisor viewport uses the genuine native
// 0xF0-byte camera array and unchanged unsigned side index.
int FrontSide::OnDraw(FrontSide *frontSide)
{
    int i;

    g_Supervisor.ctorArray078[static_cast<unsigned int>(frontSide->sideIndexA66C)].screenShakeOffsetE8.x = 0.0f;
    g_Supervisor.ctorArray078[static_cast<unsigned int>(frontSide->sideIndexA66C)].screenShakeOffsetE8.y = 0.0f;
    g_Supervisor.ConfigureScreenViewport(frontSide->sideIndexA66C);

    AnmVm *statusVm = &frontSide->statusVms0000[0];
    for (i = 0; i < 11; ++i, statusVm = statusVm + 1)
    {
        statusVm->pos.x = g_GameManager.TransformPopupX(statusVm->pos2.x);
        statusVm->pos.y = g_GameManager.TransformPopupY(statusVm->pos2.y);
        statusVm->pos.z = 0.0f;
        g_AnmManager->Draw2D(statusVm);
    }

    Float3 *cursor = &frontSide->stockVms1D0C[0].pos;
    for (i = 0; i < 5; ++i, cursor = reinterpret_cast<Float3 *>(reinterpret_cast<unsigned char *>(cursor) + sizeof(AnmVm)))
    {
        cursor->x = g_GameManager.TransformPopupX(reinterpret_cast<AnmVm *>(reinterpret_cast<unsigned char *>(cursor) - offsetof(AnmVm, pos))->pos2.x);
        cursor->y = g_GameManager.TransformPopupY(reinterpret_cast<AnmVm *>(reinterpret_cast<unsigned char *>(cursor) - offsetof(AnmVm, pos))->pos2.y);
        cursor->z = 0.0f;
        g_AnmManager->Draw2D(reinterpret_cast<AnmVm *>(reinterpret_cast<unsigned char *>(cursor) - offsetof(AnmVm, pos)));
    }

    cursor = &frontSide->gaugeVms2A40[0].pos;
    for (i = 0; i < 7; ++i, cursor = reinterpret_cast<Float3 *>(reinterpret_cast<unsigned char *>(cursor) + sizeof(AnmVm)))
    {
        cursor->x = g_GameManager.TransformPopupX(reinterpret_cast<AnmVm *>(reinterpret_cast<unsigned char *>(cursor) - offsetof(AnmVm, pos))->pos2.x);
        cursor->y = g_GameManager.TransformPopupY(reinterpret_cast<AnmVm *>(reinterpret_cast<unsigned char *>(cursor) - offsetof(AnmVm, pos))->pos2.y);
        cursor->z = 0.0f;
        g_AnmManager->Draw2D(reinterpret_cast<AnmVm *>(reinterpret_cast<unsigned char *>(cursor) - offsetof(AnmVm, pos)));
    }

    cursor = &frontSide->auxA678.vms0C[0].pos;
    for (i = 0; i < 2; ++i, cursor = reinterpret_cast<Float3 *>(reinterpret_cast<unsigned char *>(cursor) + sizeof(AnmVm)))
    {
        cursor->x = g_GameManager.TransformPopupX(reinterpret_cast<AnmVm *>(reinterpret_cast<unsigned char *>(cursor) - offsetof(AnmVm, pos))->pos2.x);
        cursor->y = g_GameManager.TransformPopupY(reinterpret_cast<AnmVm *>(reinterpret_cast<unsigned char *>(cursor) - offsetof(AnmVm, pos))->pos2.y);
        cursor->z = 0.0f;
        g_AnmManager->DrawNoRotation(reinterpret_cast<AnmVm *>(reinterpret_cast<unsigned char *>(cursor) - offsetof(AnmVm, pos)));
    }

    cursor = &frontSide->vm3CBC.pos;
    frontSide->vm3CBC.pos.x = g_GameManager.TransformPopupX(frontSide->vm3CBC.pos2.x);
    frontSide->vm3CBC.pos.y = g_GameManager.TransformPopupY(frontSide->vm3CBC.pos2.y);
    frontSide->vm3CBC.pos.z = 0.0f;
    g_AnmManager->DrawNoRotation(&frontSide->vm3CBC);

    cursor = &frontSide->scoreVms3F60[0].pos;
    for (i = 0; i < 10; ++i, cursor = reinterpret_cast<Float3 *>(reinterpret_cast<unsigned char *>(cursor) + sizeof(AnmVm)))
    {
        cursor->x = g_GameManager.TransformPopupX(reinterpret_cast<AnmVm *>(reinterpret_cast<unsigned char *>(cursor) - offsetof(AnmVm, pos))->pos2.x);
        cursor->y = g_GameManager.TransformPopupY(reinterpret_cast<AnmVm *>(reinterpret_cast<unsigned char *>(cursor) - offsetof(AnmVm, pos))->pos2.y);
        cursor->z = 0.0f;
        g_AnmManager->DrawNoRotation(reinterpret_cast<AnmVm *>(reinterpret_cast<unsigned char *>(cursor) - offsetof(AnmVm, pos)));
    }

    cursor = &frontSide->vm59C8.pos;
    frontSide->vm59C8.pos.x = g_GameManager.TransformPopupX(frontSide->vm59C8.pos2.x);
    frontSide->vm59C8.pos.y = g_GameManager.TransformPopupY(frontSide->vm59C8.pos2.y);
    g_AnmManager->DrawNoRotation(&frontSide->vm59C8);

    cursor = &frontSide->meterVms5C6C[0].pos;
    for (i = 0; i < 4; ++i, cursor = reinterpret_cast<Float3 *>(reinterpret_cast<unsigned char *>(cursor) + sizeof(AnmVm)))
    {
        cursor->x = g_GameManager.TransformPopupX(
            frontSide->sideStateA670->player04->position1B88.x +
            reinterpret_cast<AnmVm *>(reinterpret_cast<unsigned char *>(cursor) - offsetof(AnmVm, pos))->pos2.x);
        cursor->y = g_GameManager.TransformPopupY(
            frontSide->sideStateA670->player04->position1B88.y +
            reinterpret_cast<AnmVm *>(reinterpret_cast<unsigned char *>(cursor) - offsetof(AnmVm, pos))->pos2.y);
        g_AnmManager->DrawNoRotation(reinterpret_cast<AnmVm *>(reinterpret_cast<unsigned char *>(cursor) - offsetof(AnmVm, pos)));
    }

    cursor = &frontSide->markerVms66FC[0].pos;
    for (i = 0; i < 3; ++i, cursor = reinterpret_cast<Float3 *>(reinterpret_cast<unsigned char *>(cursor) + sizeof(AnmVm)))
    {
        cursor->x = g_GameManager.TransformPopupX(
            frontSide->sideStateA670->player04->position1B88.x +
            reinterpret_cast<AnmVm *>(reinterpret_cast<unsigned char *>(cursor) - offsetof(AnmVm, pos))->pos2.x);
        cursor->y = g_GameManager.TransformPopupY(
            frontSide->sideStateA670->player04->position1B88.y +
            reinterpret_cast<AnmVm *>(reinterpret_cast<unsigned char *>(cursor) - offsetof(AnmVm, pos))->pos2.y);
        if (frontSide->transitionStateA65C != 0)
            cursor->y -= 24.0f;
        g_AnmManager->DrawNoRotation(reinterpret_cast<AnmVm *>(reinterpret_cast<unsigned char *>(cursor) - offsetof(AnmVm, pos)));
    }

    cursor = &frontSide->timingVms76D4[0].pos;
    for (i = 0; i < 6; ++i, cursor = reinterpret_cast<Float3 *>(reinterpret_cast<unsigned char *>(cursor) + sizeof(AnmVm)))
    {
        cursor->x = g_GameManager.TransformPopupX(
            frontSide->sideStateA670->player04->position1B88.x +
            reinterpret_cast<AnmVm *>(reinterpret_cast<unsigned char *>(cursor) - offsetof(AnmVm, pos))->pos2.x);
        cursor->y = g_GameManager.TransformPopupY(
            frontSide->sideStateA670->player04->position1B88.y +
            reinterpret_cast<AnmVm *>(reinterpret_cast<unsigned char *>(cursor) - offsetof(AnmVm, pos))->pos2.y);
        g_AnmManager->DrawNoRotation(reinterpret_cast<AnmVm *>(reinterpret_cast<unsigned char *>(cursor) - offsetof(AnmVm, pos)));
    }

    cursor = &frontSide->stateVms6EE8[0].pos;
    for (i = 0; i < 2; ++i, cursor = reinterpret_cast<Float3 *>(reinterpret_cast<unsigned char *>(cursor) + sizeof(AnmVm)))
    {
        cursor->x = g_GameManager.TransformPopupX(
            frontSide->sideStateA670->player04->position1B88.x +
            reinterpret_cast<AnmVm *>(reinterpret_cast<unsigned char *>(cursor) - offsetof(AnmVm, pos))->pos2.x);
        cursor->y = g_GameManager.TransformPopupY(
            frontSide->sideStateA670->player04->position1B88.y +
            reinterpret_cast<AnmVm *>(reinterpret_cast<unsigned char *>(cursor) - offsetof(AnmVm, pos))->pos2.y);
        g_AnmManager->DrawNoRotation(reinterpret_cast<AnmVm *>(reinterpret_cast<unsigned char *>(cursor) - offsetof(AnmVm, pos)));
    }

    cursor = &frontSide->vm7430.pos;
    PlayerSideStateView *singleSide = frontSide->sideStateA670;
    cursor->x = g_GameManager.TransformPopupX(
        singleSide->player04->position1B88.x +
        reinterpret_cast<AnmVm *>(reinterpret_cast<unsigned char *>(cursor) - offsetof(AnmVm, pos))->pos2.x);
    cursor->y = g_GameManager.TransformPopupY(
        singleSide->player04->position1B88.y +
        reinterpret_cast<AnmVm *>(reinterpret_cast<unsigned char *>(cursor) - offsetof(AnmVm, pos))->pos2.y);
    g_AnmManager->DrawNoRotation(reinterpret_cast<AnmVm *>(reinterpret_cast<unsigned char *>(cursor) - offsetof(AnmVm, pos)));

    cursor = &frontSide->rankVms86AC[0].pos;
    for (i = 0; i < 7; ++i, cursor = reinterpret_cast<Float3 *>(reinterpret_cast<unsigned char *>(cursor) + sizeof(AnmVm)))
    {
        cursor->x = g_GameManager.TransformPopupX(reinterpret_cast<AnmVm *>(reinterpret_cast<unsigned char *>(cursor) - offsetof(AnmVm, pos))->pos2.x);
        cursor->y = g_GameManager.TransformPopupY(reinterpret_cast<AnmVm *>(reinterpret_cast<unsigned char *>(cursor) - offsetof(AnmVm, pos))->pos2.y);
        g_AnmManager->DrawNoRotation(reinterpret_cast<AnmVm *>(reinterpret_cast<unsigned char *>(cursor) - offsetof(AnmVm, pos)));
    }

    cursor = &frontSide->counterVms9928[0].pos;
    for (i = 0; i < 5; ++i, cursor = reinterpret_cast<Float3 *>(reinterpret_cast<unsigned char *>(cursor) + sizeof(AnmVm)))
    {
        cursor->x = g_GameManager.TransformPopupX(reinterpret_cast<AnmVm *>(reinterpret_cast<unsigned char *>(cursor) - offsetof(AnmVm, pos))->pos2.x);
        cursor->y = g_GameManager.TransformPopupY(reinterpret_cast<AnmVm *>(reinterpret_cast<unsigned char *>(cursor) - offsetof(AnmVm, pos))->pos2.y);
        g_AnmManager->Draw2D(reinterpret_cast<AnmVm *>(reinterpret_cast<unsigned char *>(cursor) - offsetof(AnmVm, pos)));
    }

    if (frontSide->auxA678.unknown554 != 0)
    {
        FrontSideDrawVertex vertices[4];

        float transitionTime;
        if (static_cast<float>(frontSide->transitionTimerABD0) >= 30.0f)
            transitionTime = 30.0f;
        else
            transitionTime = static_cast<float>(frontSide->transitionTimerABD0);

        float direction = 1.0f;
        if (frontSide->auxA678.unknown554 != 1)
            direction = -1.0f;

        g_AnmManager->FlushVertexBuffer();

        g_Supervisor.d3dDevice08->SetTextureStageState(
            0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
        g_Supervisor.d3dDevice08->SetTextureStageState(
            0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
        g_Supervisor.d3dDevice08->SetTextureStageState(
            0, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE);
        g_Supervisor.d3dDevice08->SetTextureStageState(
            0, D3DTSS_COLORARG1, D3DTA_DIFFUSE);
        g_Supervisor.SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
        g_Supervisor.d3dDevice08->SetVertexShader(
            D3DFVF_XYZRHW | D3DFVF_DIFFUSE);

        transitionTime *= 14.933333f;
        float edgeX = direction * -144.0f;

        // Keep these two vertical strips in one built-in-comma full-expression.
        // Each Float3 is copied immediately, but its lifetime lasts through
        // the second draw. Casts to void ensure the comma operators are built-in.
        // This is a reconstruction boundary, not a claim about original macros.
        (void)(vertices[0].pos = Float3(
            g_GameManager.TransformPopupX(edgeX),
            g_GameManager.TransformPopupY(0.0f),
            0.0f)),
        (void)(direction *= 144.0f),
        (void)(vertices[1].pos = Float3(
            g_GameManager.TransformPopupX(direction),
            g_GameManager.TransformPopupY(0.0f),
            0.0f)),
        (void)(vertices[2].pos = Float3(
            g_GameManager.TransformPopupX(edgeX),
            g_GameManager.TransformPopupY(transitionTime),
            0.0f)),
        (void)(vertices[0].diffuse =
            vertices[1].diffuse =
            vertices[2].diffuse = 0xFF000000u),
        (void)(vertices[0].rhw =
            vertices[1].rhw =
            vertices[2].rhw = 1.0f),
        (void)(g_Supervisor.d3dDevice08->DrawPrimitiveUP(
            D3DPT_TRIANGLESTRIP, 1, vertices, sizeof(FrontSideDrawVertex))),
        (void)(transitionTime =
            448.0f -
            static_cast<float>(frontSide->transitionTimerABD0) * 14.933333f),
        (void)(vertices[0].pos = Float3(
            g_GameManager.TransformPopupX(edgeX),
            g_GameManager.TransformPopupY(448.0f),
            0.0f)),
        (void)(vertices[1].pos = Float3(
            g_GameManager.TransformPopupX(direction),
            g_GameManager.TransformPopupY(448.0f),
            0.0f)),
        (void)(vertices[2].pos = Float3(
            g_GameManager.TransformPopupX(direction),
            g_GameManager.TransformPopupY(transitionTime),
            0.0f)),
        (void)(vertices[0].diffuse =
            vertices[1].diffuse =
            vertices[2].diffuse = 0xFF000000u),
        (void)(vertices[0].rhw =
            vertices[1].rhw =
            vertices[2].rhw = 1.0f),
        (void)(g_Supervisor.d3dDevice08->DrawPrimitiveUP(
            D3DPT_TRIANGLESTRIP, 1, vertices, sizeof(FrontSideDrawVertex)));

        transitionTime =
            static_cast<float>(frontSide->transitionTimerABD0) * 9.6000004f -
            144.0f;

        if (frontSide->auxA678.unknown554 == 1)
        {
            // One full-expression keeps this triangle's three Float3
            // temporaries alive until all three immediate copies finish.
            (void)(vertices[0].pos = Float3(
                g_GameManager.TransformPopupX(-144.0f),
                g_GameManager.TransformPopupY(0.0f),
                0.0f)),
            (void)(vertices[1].pos = Float3(
                g_GameManager.TransformPopupX(-144.0f),
                g_GameManager.TransformPopupY(448.0f),
                0.0f)),
            (void)(vertices[2].pos = Float3(
                g_GameManager.TransformPopupX(transitionTime),
                g_GameManager.TransformPopupY(448.0f),
                0.0f));
        }
        else
        {
            // One full-expression keeps this triangle's three Float3
            // temporaries alive until all three immediate copies finish.
            (void)(vertices[0].pos = Float3(
                g_GameManager.TransformPopupX(-144.0f),
                g_GameManager.TransformPopupY(448.0f),
                0.0f)),
            (void)(vertices[1].pos = Float3(
                g_GameManager.TransformPopupX(-144.0f),
                g_GameManager.TransformPopupY(0.0f),
                0.0f)),
            (void)(vertices[2].pos = Float3(
                g_GameManager.TransformPopupX(transitionTime),
                g_GameManager.TransformPopupY(0.0f),
                0.0f));
        }

        vertices[0].diffuse =
            vertices[1].diffuse =
            vertices[2].diffuse = 0xFF000000u;
        vertices[0].rhw =
            vertices[1].rhw =
            vertices[2].rhw = 1.0f;

        g_Supervisor.d3dDevice08->DrawPrimitiveUP(
            D3DPT_TRIANGLESTRIP, 1, vertices, sizeof(FrontSideDrawVertex));

        transitionTime =
            144.0f -
            static_cast<float>(frontSide->transitionTimerABD0) * 9.6000004f;

        if (frontSide->auxA678.unknown554 == 1)
        {
            // One full-expression keeps this triangle's three Float3
            // temporaries alive until all three immediate copies finish.
            (void)(vertices[0].pos = Float3(
                g_GameManager.TransformPopupX(144.0f),
                g_GameManager.TransformPopupY(448.0f),
                0.0f)),
            (void)(vertices[1].pos = Float3(
                g_GameManager.TransformPopupX(144.0f),
                g_GameManager.TransformPopupY(0.0f),
                0.0f)),
            (void)(vertices[2].pos = Float3(
                g_GameManager.TransformPopupX(transitionTime),
                g_GameManager.TransformPopupY(0.0f),
                0.0f));
        }
        else
        {
            // One full-expression keeps this triangle's three Float3
            // temporaries alive until all three immediate copies finish.
            (void)(vertices[0].pos = Float3(
                g_GameManager.TransformPopupX(144.0f),
                g_GameManager.TransformPopupY(0.0f),
                0.0f)),
            (void)(vertices[1].pos = Float3(
                g_GameManager.TransformPopupX(144.0f),
                g_GameManager.TransformPopupY(448.0f),
                0.0f)),
            (void)(vertices[2].pos = Float3(
                g_GameManager.TransformPopupX(transitionTime),
                g_GameManager.TransformPopupY(448.0f),
                0.0f));
        }

        vertices[0].diffuse =
            vertices[1].diffuse =
            vertices[2].diffuse = 0xFF000000u;
        vertices[0].rhw =
            vertices[1].rhw =
            vertices[2].rhw = 1.0f;

        g_Supervisor.d3dDevice08->DrawPrimitiveUP(
            D3DPT_TRIANGLESTRIP, 1, vertices, sizeof(FrontSideDrawVertex));

        g_AnmManager->ClearVertexShader();
        g_AnmManager->ClearColorOp();
        g_AnmManager->ClearBlendMode();
        g_AnmManager->ClearZWrite();

        g_Supervisor.d3dDevice08->SetTextureStageState(
            0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
        g_Supervisor.d3dDevice08->SetTextureStageState(
            0, D3DTSS_COLOROP, D3DTOP_MODULATE);
        g_Supervisor.d3dDevice08->SetTextureStageState(
            0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
        g_Supervisor.d3dDevice08->SetTextureStageState(
            0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
    }

    return 1;
}
