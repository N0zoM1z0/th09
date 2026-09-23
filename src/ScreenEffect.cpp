#include "ScreenEffect.hpp"

#include "AnmManager.hpp"
#include "AsciiManager.hpp"
#include "Supervisor.hpp"
#include "ZunMemory.hpp"

#include <windows.h>
#include <d3d8.h>
#include <math.h>
#include <new>
#include <string.h>

extern Chain g_Chain;
extern ZunMemory g_ZunMemory;

struct ScreenEffectReplayRngView
{
    unsigned int GetRandomU32InRange(unsigned int max);
};
extern ScreenEffectReplayRngView g_ReplayRng;

struct ScreenEffectSideStateView
{
    unsigned char unknown000[0x34];
    unsigned char flags34;
    unsigned char unknown035[3];
};

typedef char ScreenEffectSideStateSizeIs38[
    (sizeof(ScreenEffectSideStateView) == 0x38) ? 1 : -1];

struct ScreenEffectGameManagerView
{
    ScreenEffectSideStateView sides[2];
    unsigned char unknown070[0x134 - 0x70];
    unsigned int flags134;
    unsigned char unknown138[4];
    unsigned char inGameMenu;
    unsigned char menuBlock13D;
    unsigned char unknown13E[0x348 - 0x13E];
    int value348;
};
extern ScreenEffectGameManagerView g_GameManager;

struct ScreenEffectSupervisorView
{
    unsigned char unknown000[0x08];
    IDirect3DDevice8 *d3dDevice;
    unsigned char unknown00C[0x350 - 0x0C];
    D3DPRESENT_PARAMETERS presentParameters;
};

typedef char ScreenEffectSupervisorDeviceAt08[
    (offsetof(ScreenEffectSupervisorView, d3dDevice) == 0x08) ? 1 : -1];
typedef char ScreenEffectSupervisorPresentAt350[
    (offsetof(ScreenEffectSupervisorView, presentParameters) == 0x350) ? 1 : -1];

struct ScreenEffectAnmManagerView
{
    unsigned char unknown000[0x1C];
    float shakeX;
    float shakeY;
};

struct ScreenEffectSupervisorShakeView
{
    unsigned char unknown000[0x160];
    float side0ShakeX;
    float side0ShakeY;
    unsigned char unknown168[0xF0 - 8];
    float side1ShakeX;
    float side1ShakeY;
};

typedef char ScreenEffectSupervisorSide0ShakeAt160[
    (offsetof(ScreenEffectSupervisorShakeView, side0ShakeX) == 0x160) ? 1 : -1];
typedef char ScreenEffectSupervisorSide1ShakeAt250[
    (offsetof(ScreenEffectSupervisorShakeView, side1ShakeX) == 0x250) ? 1 : -1];

struct VertexDiffuseXyzrhw
{
    VertexDiffuseXyzrhw();

    Float3 pos;
    float w;
    unsigned long diffuse;
};

typedef char VertexDiffuseXyzrhwSizeIs14[
    (sizeof(VertexDiffuseXyzrhw) == 0x14) ? 1 : -1];

static __inline ScreenEffectSupervisorView *ScreenEffectSupervisor()
{
    return reinterpret_cast<ScreenEffectSupervisorView *>(&g_Supervisor);
}

static __inline ScreenEffectSupervisorShakeView *ScreenEffectSupervisorShake()
{
    return reinterpret_cast<ScreenEffectSupervisorShakeView *>(&g_Supervisor);
}

static __inline ScreenEffectAnmManagerView *ScreenEffectAnmManager()
{
    return reinterpret_cast<ScreenEffectAnmManagerView *>(g_AnmManager);
}

ScreenEffect::ScreenEffect()
{
}

void ScreenEffect::Clear(unsigned int color)
{
    ScreenEffectSupervisorView *supervisor = ScreenEffectSupervisor();
    supervisor->d3dDevice->Clear(
        0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, color, 1.0f, 0);
    if (supervisor->d3dDevice->Present(NULL, NULL, NULL, NULL) < 0)
        supervisor->d3dDevice->Reset(&supervisor->presentParameters);
    supervisor->d3dDevice->Clear(
        0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, color, 1.0f, 0);
    if (supervisor->d3dDevice->Present(NULL, NULL, NULL, NULL) < 0)
        supervisor->d3dDevice->Reset(&supervisor->presentParameters);
}

void ScreenEffect::SetViewport(unsigned int clearColor)
{
    if (g_AnmManager != NULL)
        g_AnmManager->FlushVertexBuffer();
    g_Supervisor.ConfigureGameplayViewport(2);
    ScreenEffect::Clear(clearColor);
}

int ScreenEffect::CalcFadeIn(ScreenEffect *screenEffect)
{
    int duration = screenEffect->duration;
    if (duration != 0)
    {
        screenEffect->overlayAlpha =
            (int)(255.0f - ((255.0f * (float)screenEffect->timer) /
                              duration));
        if (screenEffect->overlayAlpha < 0)
            screenEffect->overlayAlpha = 0;
    }

    if (screenEffect->timer >= duration)
        return CHAIN_CALLBACK_RESULT_CONTINUE_AND_REMOVE_JOB;

    screenEffect->timer++;
    return CHAIN_CALLBACK_RESULT_CONTINUE;
}

int ScreenEffect::CalcFadeOut(ScreenEffect *screenEffect)
{
    if (g_ScreenEffectCounter != 0)
        return CHAIN_CALLBACK_RESULT_CONTINUE_AND_REMOVE_JOB;

    int duration = screenEffect->duration;
    if (duration != 0)
    {
        screenEffect->overlayAlpha =
            (int)((255.0f * (float)screenEffect->timer) /
                  duration);
        if (screenEffect->overlayAlpha < 0)
            screenEffect->overlayAlpha = 0;
    }

    if (screenEffect->timer >= duration)
        return CHAIN_CALLBACK_RESULT_CONTINUE_AND_REMOVE_JOB;

    if (g_GameManager.inGameMenu == 0 && g_GameManager.menuBlock13D == 0)
        screenEffect->timer++;

    return CHAIN_CALLBACK_RESULT_CONTINUE;
}

int ScreenEffect::CalcFadeHold(ScreenEffect *screenEffect)
{
    if (screenEffect->fadeReleaseRequested == 0)
    {
        if (screenEffect->duration != 0 &&
            screenEffect->timer <= screenEffect->duration)
        {
            screenEffect->overlayAlpha =
                (int)(((float)screenEffect->timer * 255.0f) /
                      screenEffect->duration);
        }
        else
        {
            screenEffect->overlayAlpha = 255;
        }
    }
    else
    {
        if (screenEffect->timer <= 8)
        {
            screenEffect->overlayAlpha =
                128 - (int)(((float)screenEffect->timer * 128.0f) / 8.0f);
        }
        else
        {
            return CHAIN_CALLBACK_RESULT_CONTINUE_AND_REMOVE_JOB;
        }
    }

    screenEffect->timer++;
    return CHAIN_CALLBACK_RESULT_CONTINUE;
}

int ScreenEffect::CalcArcadePulse(ScreenEffect *screenEffect)
{
    unsigned int alpha =
        ((unsigned int)screenEffect->rawParameter1 >> 24) & 0xFF;

    if (g_ScreenEffectCounter != 0)
        return CHAIN_CALLBACK_RESULT_CONTINUE_AND_REMOVE_JOB;

    int duration = screenEffect->duration;

    if (screenEffect->timer < duration)
    {
        screenEffect->overlayAlpha =
            alpha - (int)((float)screenEffect->timer * alpha /
                          duration);
        if (screenEffect->overlayAlpha < 0)
            screenEffect->overlayAlpha = 0;
    }
    else
    {
        screenEffect->overlayAlpha = 0;
        screenEffect->rawParameter0--;
        if (screenEffect->rawParameter0 <= 0)
            return CHAIN_CALLBACK_RESULT_CONTINUE_AND_REMOVE_JOB;
        screenEffect->timer = 0;
    }

    screenEffect->timer++;
    return CHAIN_CALLBACK_RESULT_CONTINUE;
}

int ScreenEffect::CalcShake(ScreenEffect *screenEffect)
{
    if ((g_GameManager.sides[screenEffect->contextIndex].flags34 & 1) != 0 ||
        (g_GameManager.flags134 & 0x1800) != 0)
    {
        return CHAIN_CALLBACK_RESULT_CONTINUE;
    }

    if (g_ScreenEffectCounter != 0)
        return CHAIN_CALLBACK_RESULT_CONTINUE_AND_REMOVE_JOB;

    screenEffect->timer++;
    if (screenEffect->timer >= screenEffect->duration)
        return CHAIN_CALLBACK_RESULT_CONTINUE_AND_REMOVE_JOB;

    float shakeAmount =
        (float)screenEffect->timer *
        (screenEffect->rawParameter1 - screenEffect->rawParameter0);
    shakeAmount = shakeAmount / screenEffect->duration;
    shakeAmount = screenEffect->rawParameter0 + shakeAmount;

    switch (g_ReplayRng.GetRandomU32InRange(3))
    {
    case 0:
        *reinterpret_cast<float *>(reinterpret_cast<unsigned char *>(&g_Supervisor) +
            0x160 + screenEffect->contextIndex * 0xF0) = 0.0f;
        break;
    case 1:
        *reinterpret_cast<float *>(reinterpret_cast<unsigned char *>(&g_Supervisor) +
            0x160 + screenEffect->contextIndex * 0xF0) = shakeAmount;
        break;
    case 2:
        *reinterpret_cast<float *>(reinterpret_cast<unsigned char *>(&g_Supervisor) +
            0x160 + screenEffect->contextIndex * 0xF0) = -shakeAmount;
        break;
    }

    switch (g_ReplayRng.GetRandomU32InRange(3))
    {
    case 0:
        *reinterpret_cast<float *>(reinterpret_cast<unsigned char *>(&g_Supervisor) +
            0x164 + screenEffect->contextIndex * 0xF0) = 0.0f;
        break;
    case 1:
        *reinterpret_cast<float *>(reinterpret_cast<unsigned char *>(&g_Supervisor) +
            0x164 + screenEffect->contextIndex * 0xF0) = shakeAmount;
        break;
    case 2:
        *reinterpret_cast<float *>(reinterpret_cast<unsigned char *>(&g_Supervisor) +
            0x164 + screenEffect->contextIndex * 0xF0) = -shakeAmount;
        break;
    }

    return CHAIN_CALLBACK_RESULT_CONTINUE;
}

int ScreenEffect::CalcShakeEnvelope(ScreenEffect *screenEffect)
{
    if (g_GameManager.value348 <= 1)
        return CHAIN_CALLBACK_RESULT_CONTINUE_AND_REMOVE_JOB;

    screenEffect->timer++;

    float envelope;
    if (screenEffect->timer < screenEffect->rawParameter0)
    {
        envelope = (float)screenEffect->timer / screenEffect->rawParameter0;
    }
    else if (screenEffect->timer <
             screenEffect->rawParameter0 + screenEffect->rawParameter1)
    {
        envelope = 1.0f;
    }
    else
    {
        int endFrame = screenEffect->rawParameter0 +
                       screenEffect->rawParameter1 +
                       screenEffect->rawParameter2;
        if (!(screenEffect->timer < endFrame))
            return CHAIN_CALLBACK_RESULT_CONTINUE_AND_REMOVE_JOB;
        envelope = ((float)(unsigned int)endFrame - (float)screenEffect->timer) /
                   (unsigned int)screenEffect->rawParameter2;
    }

    float shakeAmount = (float)screenEffect->duration * envelope;
    switch (g_ReplayRng.GetRandomU32InRange(3))
    {
    case 0:
        ScreenEffectAnmManager()->shakeX = 0.0f;
        break;
    case 1:
        ScreenEffectAnmManager()->shakeX = shakeAmount;
        break;
    case 2:
        ScreenEffectAnmManager()->shakeX = -shakeAmount;
        break;
    }

    switch (g_ReplayRng.GetRandomU32InRange(3))
    {
    case 0:
        ScreenEffectAnmManager()->shakeY = 0.0f;
        break;
    case 1:
        ScreenEffectAnmManager()->shakeY = shakeAmount;
        break;
    case 2:
        ScreenEffectAnmManager()->shakeY = -shakeAmount;
        break;
    }

    return CHAIN_CALLBACK_RESULT_CONTINUE;
}

int ScreenEffect::InitializeTimer(ScreenEffect *screenEffect)
{
    screenEffect->timer = 0;
    return 0;
}

int ScreenEffect::DeleteScreenEffect(ScreenEffect *screenEffect)
{
    screenEffect->calcChainElement->deletedCallback = NULL;
    g_Chain.Cut(screenEffect->drawChainElement);
    screenEffect->drawChainElement = NULL;
    delete screenEffect;
    return 0;
}

void ScreenEffect::BeginFadeRelease()
{
    this->fadeReleaseRequested = 1;
    this->timer = 0;
}

void ScreenEffect::DrawSquare(ScreenEffectRect *rect, unsigned int color)
{
    ScreenEffectSupervisorView *supervisor = ScreenEffectSupervisor();
    g_AnmManager->FlushVertexBuffer();

    VertexDiffuseXyzrhw vertices[4];

    vertices[0].pos = Float3(rect->left, rect->top, 0.0f);
    vertices[1].pos = Float3(rect->right, rect->top, 0.0f);
    vertices[2].pos = Float3(rect->left, rect->bottom, 0.0f);
    vertices[3].pos = Float3(rect->right, rect->bottom, 0.0f);

    vertices[3].w = 1.0f;
    vertices[2].w = vertices[3].w;
    vertices[1].w = vertices[2].w;
    vertices[0].w = vertices[1].w;

    vertices[3].diffuse = color;
    vertices[2].diffuse = vertices[3].diffuse;
    vertices[1].diffuse = vertices[2].diffuse;
    vertices[0].diffuse = vertices[1].diffuse;

    supervisor->d3dDevice->SetTextureStageState(
        0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
    supervisor->d3dDevice->SetTextureStageState(
        0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
    supervisor->d3dDevice->SetTextureStageState(
        0, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE);
    supervisor->d3dDevice->SetTextureStageState(
        0, D3DTSS_COLORARG1, D3DTA_DIFFUSE);
    supervisor->d3dDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
    supervisor->d3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
    supervisor->d3dDevice->SetVertexShader(D3DFVF_XYZRHW | D3DFVF_DIFFUSE);
    supervisor->d3dDevice->DrawPrimitiveUP(
        D3DPT_TRIANGLESTRIP, 2, vertices, sizeof(VertexDiffuseXyzrhw));

    g_AnmManager->ClearVertexShader();
    g_AnmManager->ClearSprite();
    g_AnmManager->ClearTexture();
    g_AnmManager->ClearColorOp();
    g_AnmManager->ClearBlendMode();
    g_AnmManager->ClearZWrite();

    supervisor->d3dDevice->SetTextureStageState(
        0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
    supervisor->d3dDevice->SetTextureStageState(
        0, D3DTSS_COLOROP, D3DTOP_MODULATE);
    supervisor->d3dDevice->SetTextureStageState(
        0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
    supervisor->d3dDevice->SetTextureStageState(
        0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
}

int ScreenEffect::DrawFullFade(ScreenEffect *screenEffect)
{
    ScreenEffectRect rect;
    rect.left = 0.0f;
    rect.top = 0.0f;
    rect.right = 640.0f;
    rect.bottom = 480.0f;

    g_AnmManager->FlushVertexBuffer();
    g_Supervisor.ConfigureGameplayViewport(2);
    ScreenEffect::DrawSquare(
        &rect,
        screenEffect->rawParameter0 | (screenEffect->overlayAlpha << 24));
    return CHAIN_CALLBACK_RESULT_CONTINUE;
}

int ScreenEffect::DrawArcadeFade(ScreenEffect *screenEffect)
{
    ScreenEffectRect rect;
    rect.left = 32.0f;
    rect.top = 16.0f;
    rect.right = 416.0f;
    rect.bottom = 464.0f;
    ScreenEffect::DrawSquare(
        &rect,
        screenEffect->rawParameter0 | (screenEffect->overlayAlpha << 24));
    return CHAIN_CALLBACK_RESULT_CONTINUE;
}

int ScreenEffect::DrawPartialFade(ScreenEffect *screenEffect)
{
    ScreenEffectRect rect;
    rect.left = 0.0f;
    rect.top = 0.0f;
    rect.right = 640.0f;
    rect.bottom = 480.0f;
    ScreenEffect::DrawSquare(
        &rect,
        screenEffect->rawParameter0 | (screenEffect->overlayAlpha << 24));
    return CHAIN_CALLBACK_RESULT_CONTINUE;
}

int ScreenEffect::DrawSplitFade(ScreenEffect *screenEffect)
{
    ScreenEffectRect rect;
    rect.left = 16.0f;
    rect.top = 16.0f;
    rect.right = 304.0f;
    rect.bottom = 464.0f;
    ScreenEffect::DrawSquare(
        &rect,
        screenEffect->rawParameter0 | (screenEffect->overlayAlpha << 24));

    rect.left = 336.0f;
    rect.top = 16.0f;
    rect.right = 624.0f;
    rect.bottom = 464.0f;
    ScreenEffect::DrawSquare(
        &rect,
        screenEffect->rawParameter0 | (screenEffect->overlayAlpha << 24));
    return CHAIN_CALLBACK_RESULT_CONTINUE;
}

int ScreenEffect::DrawArcadePulse(ScreenEffect *screenEffect)
{
    ScreenEffectRect rect;
    rect.left = 32.0f;
    rect.top = 16.0f;
    rect.right = 416.0f;
    rect.bottom = 464.0f;
    ScreenEffect::DrawSquare(
        &rect,
        (screenEffect->overlayAlpha << 24) |
            ((unsigned int)screenEffect->rawParameter1 & 0xFFFFFF));
    return CHAIN_CALLBACK_RESULT_CONTINUE;
}

ScreenEffect *ScreenEffect::RegisterChain(
    ScreenEffectType effect,
    int durationFrames,
    int primaryParameter,
    int secondaryParameter,
    int tertiaryParameter,
    int drawPriority,
    int contextIndex)
{
    ChainElem *calcChain = NULL;
    ChainElem *drawChain = NULL;
    ScreenEffect *screenEffect = static_cast<ScreenEffect *>(
        g_ZunMemory.AddToRegistry(
            new ScreenEffect, sizeof(ScreenEffect), "ScreenInf"));

    if (screenEffect == NULL)
        return NULL;

    memset(screenEffect, 0, sizeof(ScreenEffect));
    screenEffect->contextIndex = contextIndex;

    switch (effect)
    {
    case SCREEN_EFFECT_FULL_FADE_IN:
        calcChain = g_Chain.CreateElem((ChainCallback)ScreenEffect::CalcFadeIn);
        drawChain = g_Chain.CreateElem((ChainCallback)ScreenEffect::DrawFullFade);
        break;
    case SCREEN_EFFECT_SHAKE:
        calcChain = g_Chain.CreateElem((ChainCallback)ScreenEffect::CalcShake);
        break;
    case SCREEN_EFFECT_ARCADE_FADE_OUT:
        calcChain = g_Chain.CreateElem((ChainCallback)ScreenEffect::CalcFadeOut);
        drawChain = g_Chain.CreateElem((ChainCallback)ScreenEffect::DrawArcadeFade);
        break;
    case SCREEN_EFFECT_ARCADE_PULSE:
        calcChain = g_Chain.CreateElem((ChainCallback)ScreenEffect::CalcArcadePulse);
        drawChain = g_Chain.CreateElem((ChainCallback)ScreenEffect::DrawArcadePulse);
        break;
    case SCREEN_EFFECT_FULL_FADE_OUT:
        calcChain = g_Chain.CreateElem((ChainCallback)ScreenEffect::CalcFadeOut);
        drawChain = g_Chain.CreateElem((ChainCallback)ScreenEffect::DrawFullFade);
        break;
    case SCREEN_EFFECT_FULL_FADE_HOLD:
        calcChain = g_Chain.CreateElem((ChainCallback)ScreenEffect::CalcFadeHold);
        drawChain = g_Chain.CreateElem((ChainCallback)ScreenEffect::DrawPartialFade);
        break;
    case SCREEN_EFFECT_SPLIT_FADE_HOLD:
        calcChain = g_Chain.CreateElem((ChainCallback)ScreenEffect::CalcFadeHold);
        drawChain = g_Chain.CreateElem((ChainCallback)ScreenEffect::DrawSplitFade);
        break;
    case SCREEN_EFFECT_SHAKE_ENVELOPE:
        calcChain = g_Chain.CreateElem((ChainCallback)ScreenEffect::CalcShakeEnvelope);
        break;
    }

    calcChain->addedCallback =
        (ChainLifetimeCallback)ScreenEffect::InitializeTimer;
    calcChain->deletedCallback =
        (ChainLifetimeCallback)ScreenEffect::DeleteScreenEffect;
    calcChain->arg = screenEffect;

    screenEffect->type = effect;
    screenEffect->duration = durationFrames;
    screenEffect->rawParameter0 = primaryParameter;
    screenEffect->rawParameter1 = secondaryParameter;
    screenEffect->rawParameter2 = tertiaryParameter;

    if (g_Chain.AddToCalcChain(calcChain, 3) != 0)
        return NULL;

    if (drawChain != NULL)
    {
        drawChain->arg = screenEffect;
        g_Chain.AddToDrawChain(drawChain, drawPriority);
    }

    screenEffect->drawChainElement = drawChain;
    screenEffect->calcChainElement = calcChain;
    return screenEffect;
}
