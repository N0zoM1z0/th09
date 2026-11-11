#include "ScreenEffect.hpp"

#include "AnmManager.hpp"
#include "VertexDiffuseXyzrhw.hpp"
#include "RngRuntimeLeaves.hpp"
#include "AsciiManager.hpp"
#include "AsciiGameManagerView.hpp"
#include "Supervisor.hpp"
#include "ZunMemory.hpp"

#include <windows.h>
#include <d3d8.h>
#include <math.h>
#include <new>
#include <string.h>

extern Chain g_Chain;
extern ZunMemory g_ZunMemory;

extern RngRuntimeView g_ReplayRng;



// TH09's two gameplay shake records live in the genuine 0xF0 Supervisor
// constructor-array elements, at +0x160/+0x164 and +0x250/+0x254.
// Do not carry a parallel 0x250-byte Supervisor memory projection here.
// Original low-byte side flag is the low bit of the genuine shared
// GameManager.sides[contextIndex].flags word; not a second byte-only slot.
typedef char ScreenEffectNativeSideFlagsAt34[
    (sizeof(AsciiGameManagerSideView) == 0x38 &&
     offsetof(AsciiGameManagerSideView, flags) == 0x34) ? 1 : -1];

typedef char ScreenEffectNativeSupervisorShakeOffsets[
    (offsetof(Supervisor, ctorArray078) +
         offsetof(SupervisorCtorArrayElement, frameOffsetX) == 0x160 &&
     offsetof(Supervisor, ctorArray078) +
         offsetof(SupervisorCtorArrayElement, frameOffsetY) == 0x164 &&
     sizeof(SupervisorCtorArrayElement) == 0xF0) ? 1 : -1];

typedef char VertexDiffuseXyzrhwSizeIs14[
    (sizeof(VertexDiffuseXyzrhw) == 0x14) ? 1 : -1];

ScreenEffect::ScreenEffect()
{
}

void ScreenEffect::Clear(unsigned int color)
{
    g_Supervisor.d3dDevice08->Clear(
        0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, color, 1.0f, 0);
    if (g_Supervisor.d3dDevice08->Present(NULL, NULL, NULL, NULL) < 0)
        g_Supervisor.d3dDevice08->Reset(&g_Supervisor.presentParameters350);
    g_Supervisor.d3dDevice08->Clear(
        0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, color, 1.0f, 0);
    if (g_Supervisor.d3dDevice08->Present(NULL, NULL, NULL, NULL) < 0)
        g_Supervisor.d3dDevice08->Reset(&g_Supervisor.presentParameters350);
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

    if (g_GameManager.inGameMenu == 0 && g_GameManager.menuState97C8Active == 0)
        screenEffect->timer++;

    return CHAIN_CALLBACK_RESULT_CONTINUE;
}

int ScreenEffect::CalcFadeHold(ScreenEffect *screenEffect)
{
    if (screenEffect->fadeReleaseRequested == 0)
    {
        int duration = screenEffect->duration;
        if (duration != 0 &&
            screenEffect->timer <= duration)
        {
            screenEffect->overlayAlpha =
                (int)(((float)screenEffect->timer * 255.0f) /
                      duration);
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
                128 - (int)((float)screenEffect->timer * (255.0f / 8.0f));
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
        // Retail decrements this signed storage as a native DWORD.
        screenEffect->rawParameter0 = static_cast<int>(
            static_cast<unsigned int>(screenEffect->rawParameter0) - 1u);
        if (screenEffect->rawParameter0 <= 0)
            return CHAIN_CALLBACK_RESULT_CONTINUE_AND_REMOVE_JOB;
        screenEffect->timer = 0;
    }

    screenEffect->timer++;
    return CHAIN_CALLBACK_RESULT_CONTINUE;
}

// The signed shake side index selects the original 0x38-byte side record.
static inline AsciiGameManagerSideView &ScreenShakeSideAtNativeIndex(int side)
{
    return *reinterpret_cast<AsciiGameManagerSideView *>(
        reinterpret_cast<unsigned int>(&g_GameManager.sides[0]) +
        static_cast<unsigned int>(side) * sizeof(g_GameManager.sides[0]));
}

// Each original Supervisor camera/shake slot is a 0xF0-byte record.
static inline SupervisorCtorArrayElement &ScreenShakeViewportAtNativeIndex(int side)
{
    return *reinterpret_cast<SupervisorCtorArrayElement *>(
        reinterpret_cast<unsigned int>(&g_Supervisor.ctorArray078[0]) +
        static_cast<unsigned int>(side) * sizeof(g_Supervisor.ctorArray078[0]));
}

int ScreenEffect::CalcShake(ScreenEffect *screenEffect)
{
    if ((ScreenShakeSideAtNativeIndex(screenEffect->contextIndex).flags & 1u) != 0)
        return CHAIN_CALLBACK_RESULT_CONTINUE;

    if ((g_GameManager.flags & 0x1800) != 0)
        return CHAIN_CALLBACK_RESULT_CONTINUE;

    if (g_ScreenEffectCounter != 0)
        return CHAIN_CALLBACK_RESULT_CONTINUE_AND_REMOVE_JOB;

    screenEffect->timer++;
    int duration = screenEffect->duration;
    if (screenEffect->timer >= duration)
        return CHAIN_CALLBACK_RESULT_CONTINUE_AND_REMOVE_JOB;

    // The native i386 amplitude delta wraps as a 32-bit word before its
    // signed result enters the original float shake calculation.
    float shakeAmount =
        (float)screenEffect->timer *
            static_cast<int>(
                static_cast<unsigned int>(screenEffect->rawParameter1) -
                static_cast<unsigned int>(screenEffect->rawParameter0)) /
            duration +
        screenEffect->rawParameter0;

    switch (g_ReplayRng.GetRandomU32InRange(3))
    {
    case 0:
        ScreenShakeViewportAtNativeIndex(screenEffect->contextIndex).frameOffsetX = 0.0f;
        break;
    case 1:
        ScreenShakeViewportAtNativeIndex(screenEffect->contextIndex).frameOffsetX = shakeAmount;
        break;
    case 2:
        ScreenShakeViewportAtNativeIndex(screenEffect->contextIndex).frameOffsetX = -shakeAmount;
        break;
    }

    switch (g_ReplayRng.GetRandomU32InRange(3))
    {
    case 0:
        ScreenShakeViewportAtNativeIndex(screenEffect->contextIndex).frameOffsetY = 0.0f;
        break;
    case 1:
        ScreenShakeViewportAtNativeIndex(screenEffect->contextIndex).frameOffsetY = shakeAmount;
        break;
    case 2:
        ScreenShakeViewportAtNativeIndex(screenEffect->contextIndex).frameOffsetY = -shakeAmount;
        break;
    }

    return CHAIN_CALLBACK_RESULT_CONTINUE;
}

int ScreenEffect::CalcShakeEnvelope(ScreenEffect *screenEffect)
{
    float shakeAmount;

    if (g_GameManager.value348 <= 1)
        return CHAIN_CALLBACK_RESULT_CONTINUE_AND_REMOVE_JOB;

    ZunTimer &clock = screenEffect->timer;
    clock++;

    int endFrame = screenEffect->shakeEnvelope.fadeInFrames;
    if (clock < endFrame)
    {
        shakeAmount = (float)clock / endFrame;
    }
    else if (clock < static_cast<int>(
        static_cast<unsigned int>(endFrame) +
        static_cast<unsigned int>(screenEffect->shakeEnvelope.holdFrames)))
    {
        shakeAmount = 1.0f;
    }
    else
    {
        // The original loads the hold and fade dwords independently,
        // combines them, then extends the final envelope endpoint.
        // Keep both original volatile field reads; declaring the two
        // reference aliases fade-then-hold reproduces the original VC7.1
        // scalar load scheduling without changing the hold + fade result.
        const volatile int &fade = screenEffect->shakeEnvelope.fadeOutFrames;
        const volatile int &hold = screenEffect->shakeEnvelope.holdFrames;
        // The original i386 endpoint additions wrap as native DWORDs.
        // Preserve the signed timer comparison on the resulting word.
        unsigned int update =
            static_cast<unsigned int>(hold) + static_cast<unsigned int>(fade);
        endFrame = static_cast<unsigned int>(endFrame) + update;
        if (clock < endFrame)
        {
            shakeAmount =
                ((float)(unsigned int)endFrame - (float)clock) /
                (unsigned int)screenEffect->shakeEnvelope.fadeOutFrames;
        }
        else
        {
            return CHAIN_CALLBACK_RESULT_CONTINUE_AND_REMOVE_JOB;
        }
    }

    shakeAmount = (float)screenEffect->duration * shakeAmount;
    switch (g_ReplayRng.GetRandomU32InRange(3))
    {
    case 0:
        g_AnmManager->frameShakeX1C = 0.0f;
        break;
    case 1:
        g_AnmManager->frameShakeX1C = shakeAmount;
        break;
    case 2:
        g_AnmManager->frameShakeX1C = -shakeAmount;
        break;
    }

    switch (g_ReplayRng.GetRandomU32InRange(3))
    {
    case 0:
        g_AnmManager->frameShakeY20 = 0.0f;
        break;
    case 1:
        g_AnmManager->frameShakeY20 = shakeAmount;
        break;
    case 2:
        g_AnmManager->frameShakeY20 = -shakeAmount;
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

    g_Supervisor.d3dDevice08->SetTextureStageState(
        0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
    g_Supervisor.d3dDevice08->SetTextureStageState(
        0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
    g_Supervisor.d3dDevice08->SetTextureStageState(
        0, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE);
    g_Supervisor.d3dDevice08->SetTextureStageState(
        0, D3DTSS_COLORARG1, D3DTA_DIFFUSE);
    g_Supervisor.d3dDevice08->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
    g_Supervisor.d3dDevice08->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
    g_Supervisor.d3dDevice08->SetVertexShader(D3DFVF_XYZRHW | D3DFVF_DIFFUSE);
    g_Supervisor.d3dDevice08->DrawPrimitiveUP(
        D3DPT_TRIANGLESTRIP, 2, vertices, sizeof(VertexDiffuseXyzrhw));

    g_AnmManager->ClearVertexShader();
    g_AnmManager->ClearSprite();
    g_AnmManager->ClearTexture();
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

// Original color packing shifts the alpha bits in an unsigned native DWORD.
// All fade draw variants use this same 32-bit color operation.
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
        screenEffect->rawParameter0 | (static_cast<unsigned int>(screenEffect->overlayAlpha) << 24));
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
        screenEffect->rawParameter0 | (static_cast<unsigned int>(screenEffect->overlayAlpha) << 24));
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
        screenEffect->rawParameter0 | (static_cast<unsigned int>(screenEffect->overlayAlpha) << 24));
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
        screenEffect->rawParameter0 | (static_cast<unsigned int>(screenEffect->overlayAlpha) << 24));

    rect.left = 336.0f;
    rect.top = 16.0f;
    rect.right = 624.0f;
    rect.bottom = 464.0f;
    ScreenEffect::DrawSquare(
        &rect,
        screenEffect->rawParameter0 | (static_cast<unsigned int>(screenEffect->overlayAlpha) << 24));
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
        (static_cast<unsigned int>(screenEffect->overlayAlpha) << 24) |
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
    case SCREEN_EFFECT_FULL_FADE_OUT:
        calcChain = g_Chain.CreateElem((ChainCallback)ScreenEffect::CalcFadeOut);
        drawChain = g_Chain.CreateElem((ChainCallback)ScreenEffect::DrawFullFade);
        break;
    case SCREEN_EFFECT_ARCADE_PULSE:
        calcChain = g_Chain.CreateElem((ChainCallback)ScreenEffect::CalcArcadePulse);
        drawChain = g_Chain.CreateElem((ChainCallback)ScreenEffect::DrawArcadePulse);
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
