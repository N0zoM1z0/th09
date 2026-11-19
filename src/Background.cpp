#include "Background.hpp"
#include "GameManagerRuntimeView.hpp"

#include "AnmManager.hpp"
#include "AnmManagerRenderCacheView.hpp"
#include "AnmManagerFileSlotView.hpp"
#include "FileSystem.hpp"
#include "GameErrorContext.hpp"
#include "ScreenEffect.hpp"
#include "StageResourcePolicy.hpp"
#include "Supervisor.hpp"
#include "ZunMemory.hpp"

#include <windows.h>
#include <d3d8.h>
#include <new>
#include <string.h>

extern Chain g_Chain;
extern ZunMemory g_ZunMemory;
extern GameErrorContext g_GameErrorContext;
extern char g_BackgroundStageMissingMessage[];

// Runtime reads the authentic game manager and its two 0x38-byte side records.
typedef char BackgroundNativeGameManagerLayout[
    (offsetof(AsciiGameManagerView, valueF4) == 0xF4 &&
     offsetof(AsciiGameManagerView, flags) == 0x134 &&
     sizeof(AsciiGameManagerSideView) == 0x38) ? 1 : -1];
extern AsciiGameManagerView g_GameManager;

struct BackgroundStageFiles
{
    const char *anmPath;
    const char *stdPath;
};
extern BackgroundStageFiles g_BackgroundStageFiles[];

// Spell and stage VM indices select the real 0x2A4-byte ANM allocations.
// Stage visibility checks use the original VM's +0x214 activeSpriteIndex,
// not a fabricated scriptIndex projection onto the wrong native field.
typedef char BackgroundNativeStageFileRowSizeIs08[
    sizeof(BackgroundStageFiles) == 0x08 ? 1 : -1];

struct BackgroundTintColorView
{
    unsigned char blue;
    unsigned char green;
    unsigned char red;
    unsigned char alpha;
};

union BackgroundRenderColor
{
    unsigned int value;
    BackgroundTintColorView channels;
};

typedef char BackgroundRenderColorSizeIs4[
    (sizeof(BackgroundRenderColor) == 4) ? 1 : -1];
typedef char BackgroundAnmManagerUseMixColorAt4[
    (offsetof(AnmManagerRenderCacheView, useMixColor) == 4) ? 1 : -1];

static unsigned char MixRenderColor(
    unsigned char color, unsigned char mixColor)
{
    unsigned int result = ((color * mixColor) / 128U);
    if (result >= 256)
        result = 255;
    return static_cast<unsigned char>(result);
}

extern void BackgroundRunStageScriptPhase(Background *background);
extern void BackgroundUpdateRuntimePhase(Background *background);

Background::Background()
{
    memset(this, 0, sizeof(Background));
}

Background *Background::Create(int viewportIndex)
{
    Background *background = static_cast<Background *>(
        g_ZunMemory.AddToRegistry(new Background, sizeof(Background), "BGScrollInf"));

    background->sideState = &g_GameManager.sides[viewportIndex];
    background->frameCounter = 0;
    background->viewportIndex = viewportIndex;
    background->otherSideState = &g_GameManager.sides[
        1 - viewportIndex];

    background->calcChain = g_Chain.CreateElem((ChainCallback)Background::OnUpdate);
    background->calcChain->arg = background;
    background->calcChain->addedCallback = (ChainLifetimeCallback)Background::AddedCallback;
    background->calcChain->deletedCallback = (ChainLifetimeCallback)Background::DeletedCallback;
    if (g_Chain.AddToCalcChain(background->calcChain, viewportIndex + 8) != 0)
        return NULL;

    background->drawHighChain = g_Chain.CreateElem((ChainCallback)Background::OnDrawHighPrio);
    background->drawLowChain = g_Chain.CreateElem((ChainCallback)Background::OnDrawLowPrio);
    background->drawHighChain->arg = background;
    background->drawLowChain->arg = background;
    g_Chain.AddToDrawChain(background->drawHighChain, viewportIndex + 6);
    g_Chain.AddToDrawChain(background->drawLowChain, viewportIndex + 8);
    return background;
}

int Background::LoadStageData(const char *path)
{
    RawStageObject *currentObject;
    RawStageQuadBasic *currentQuad;
    int objectIndex;
    int vmIndex;

    this->stageData = reinterpret_cast<RawStageHeader *>(
        FileSystem::OpenFile(path, NULL, 0));
    if (this->stageData == NULL)
    {
        g_GameErrorContext.Log(g_BackgroundStageMissingMessage);
        return -1;
    }

    this->stageObjectCount = this->stageData->objectCount;
    this->stageQuadCount = this->stageData->quadCount;
    this->stageObjectInstances = reinterpret_cast<RawStageObjectInstance *>(
        reinterpret_cast<unsigned char *>(this->stageData) + this->stageData->objectInstancesOffset);
    this->stageScript = reinterpret_cast<RawStageInstr *>(
        reinterpret_cast<unsigned char *>(this->stageData) + this->stageData->scriptOffset);
    this->stageObjects = reinterpret_cast<RawStageObject **>(
        reinterpret_cast<unsigned char *>(this->stageData) + sizeof(RawStageHeader));

    for (objectIndex = 0; objectIndex < this->stageObjectCount; objectIndex++)
    {
        // Each STD object-pointer slot holds a serialized byte offset.
        // Resolve it inside the original loaded stage-file buffer.
        this->stageObjects[objectIndex] = reinterpret_cast<RawStageObject *>(
            reinterpret_cast<unsigned char *>(this->stageData) +
            reinterpret_cast<unsigned int>(this->stageObjects[objectIndex]));
    }

    this->stageObjectVms = reinterpret_cast<AnmVm *>(
        g_ZunMemory.Alloc(this->stageQuadCount * sizeof(AnmVm), "bgscroll"));

    for (objectIndex = 0, vmIndex = 0;
         objectIndex < this->stageObjectCount; objectIndex++)
    {
        currentObject = this->stageObjects[objectIndex];
        currentObject->flags = 1;
        currentQuad = &currentObject->firstQuad;
        while (currentQuad->type >= 0)
        {
            this->stageAnm->ExecuteAnmIdx(
                &this->stageObjectVms[vmIndex], currentQuad->anmScript);
            currentQuad->vmIndex = static_cast<short>(vmIndex++);
            currentQuad = reinterpret_cast<RawStageQuadBasic *>(
                reinterpret_cast<unsigned char *>(currentQuad) +
                currentQuad->byteSize);
        }
    }
    return 0;
}

int Background::DeletedCallback(Background *background)
{
    if (ShouldReleaseStageResources())
        g_AnmManager->ReleaseAnm(4);

    if (background->stageObjectVms != NULL)
    {
        g_ZunMemory.Free(background->stageObjectVms);
        background->stageObjectVms = NULL;
    }

    if (background->stageData != NULL)
    {
        g_ZunMemory.Free(background->stageData);
        background->stageData = NULL;
    }
    return 0;
}

int Background::AddedCallback(Background *background)
{
    background->stageScriptTimer = 0;
    background->stageScriptInstructionIndex = 0;
    background->stagePosition.x = 0.0f;
    background->stagePosition.y = 0.0f;
    background->stagePosition.z = 0.0f;
    background->skyFogInterpolationDuration = 0;
    background->spellVmScriptBase = 0;
    background->spellVmCount = 2;

    AnmManagerReadSlotView *loaded =
        reinterpret_cast<AnmManagerReadSlotView *>(g_AnmManager->GetAnm(4));
    if (loaded->loaded.rawData04 != NULL &&
        strcmp(loaded->path,
               g_BackgroundStageFiles[
                   static_cast<unsigned int>(g_GameManager.valueF4)].anmPath) == 0)
    {
        background->stageAnm = &loaded->loaded;
    }
    else
    {
        background->stageAnm = g_AnmManager->PreloadAnm(
            4, g_BackgroundStageFiles[
                   static_cast<unsigned int>(g_GameManager.valueF4)].anmPath);
        if (background->stageAnm == NULL)
            return -1;
    }

    background->sideAnm = g_AnmManager->GetAnm(background->viewportIndex + 5);
    if (background->LoadStageData(
            g_BackgroundStageFiles[
                static_cast<unsigned int>(g_GameManager.valueF4)].stdPath) != 0)
        return -1;

    background->skyFog.color = 0xFF000000;
    background->skyFog.nearPlane = 200.0f;
    background->skyFog.farPlane = 500.0f;
    background->cameraMotionMode = 0;

    for (int i = 0; i < 4; i++)
    {
        background->interpolationDuration[i] = 0;
        background->interpolationTimers[i] = 0;
    }

    background->pendingStageLabel18 = 0;
    background->cullingDistanceSq = 1822500.0f;
    if (g_GameManager.valueF4 == 5)
    {
        background->cullingDistanceSq = 3062500.0f;
    }
    else if (g_GameManager.valueF4 == 9)
    {
        background->cullingDistanceSq = 3062500.0f;
    }
    else if (g_GameManager.valueF4 == 11)
    {
        background->cullingDistanceSq = 4202500.0f;
    }
    else if (g_GameManager.valueF4 == 12 ||
             g_GameManager.valueF4 == 15)
    {
        background->cullingDistanceSq = 6002500.0f;
    }

    background->ClearSpellBackgroundState();
    return 0;
}

void Background::ClearSpellBackgroundState()
{
    this->spellBackgroundState = 0;
}

int Background::OnUpdate(Background *background)
{
    // Native stage, spell and transition counters are signed DWORD storage.
    // The original i386 INC/DEC wraps their live physical words at 32 bits.
    if (background->transitionMode != 0)
    {
        --background->transitionFrames;
        if (background->transitionFrames <= 0)
        {
            background->transitionMode = 0;
            background->transitionFrames = 0;
        }
        else
        {
            switch (background->transitionMode)
            {
            case 1:
                background->tintColor = 0x80F00000;
                break;
            case 2:
                background->tintColor = 0x80303040;
                break;
            }
        }
    }

    if ((g_GameManager.flags & 0x1800) != 0)
        return CHAIN_CALLBACK_RESULT_CONTINUE;

    AsciiGameManagerSideView *side =
        static_cast<AsciiGameManagerSideView *>(background->sideState);
    if ((side->flags & 1) != 0)
    {
        background->tintColor = 0x40303030;
        return CHAIN_CALLBACK_RESULT_CONTINUE;
    }

    g_Supervisor.SelectSide(background->viewportIndex);

    if (background->stageData == NULL)
        return CHAIN_CALLBACK_RESULT_CONTINUE;

    background->ApplyPendingStageLabel();
    BackgroundRunStageScriptPhase(background);

    RawStageInstr *currentInstruction =
        &background->stageScript[background->stageScriptInstructionIndex];
    if (currentInstruction->opcode != 3)
        background->stageScriptTimer++;

    background->UpdateStageObjectVms();

    if (background->spellBackgroundState >= 1)
    {
        if (background->spellBackgroundFrame == 60)
            ++background->spellBackgroundState;
        ++background->spellBackgroundFrame;
        for (int i = 0; i < background->spellVmCount; i++)
            g_AnmManager->ExecuteScript(&background->spellVms[i]);
    }

    if (background->stageVm0.activeSpriteIndex > 0)
        g_AnmManager->ExecuteScript(&background->stageVm0);

    if (background->stageVm1.activeSpriteIndex > 0)
        g_AnmManager->ExecuteScript(&background->stageVm1);

    if (background->stageVm2.activeSpriteIndex > 0)
    {
        g_AnmManager->ExecuteScript(&background->stageVm2);
        background->clearColor =
            background->stageVm2.color1;
    }

    ++background->frameCounter;
    return CHAIN_CALLBACK_RESULT_CONTINUE;
}

int Background::OnDrawHighPrio(Background *background)
{
    Supervisor *supervisor = &g_Supervisor;
    BackgroundTintColorView *tint =
        reinterpret_cast<BackgroundTintColorView *>(&background->tintColor);

    if (!g_Supervisor.IsFogDisabled())
        g_Supervisor.DisableFog();
    g_AnmManager->FlushVertexBuffer();
    g_AnmManager->ClearTexture();
    g_Supervisor.ConfigureGameplayViewport(background->viewportIndex);

    if (background->clearPending != 0)
    {
        supervisor->d3dDevice08->Clear(
            0, NULL, D3DCLEAR_TARGET, 0xFF000000, 1.0f, 0);
        background->clearPending = 0;
    }

    if (tint->alpha > 0)
        g_AnmManager->SetMixColor(background->tintColor);

    if ((background->clearColor & 0xFF000000) == 0xFF000000)
    {
        unsigned int color = background->skyFog.color;
        if (reinterpret_cast<AnmManagerRenderCacheView *>(g_AnmManager)
                ->useMixColor != 0)
        {
            BackgroundTintColorView *colorChannels =
                reinterpret_cast<BackgroundTintColorView *>(&color);
            BackgroundTintColorView *mixChannels =
                reinterpret_cast<BackgroundTintColorView *>(
                    &reinterpret_cast<AnmManagerRenderCacheView *>(g_AnmManager)
                         ->mixColor);
            colorChannels->red =
                MixRenderColor(colorChannels->red, mixChannels->red);
            colorChannels->green =
                MixRenderColor(colorChannels->green, mixChannels->green);
            colorChannels->blue =
                MixRenderColor(colorChannels->blue, mixChannels->blue);
            colorChannels->alpha =
                MixRenderColor(colorChannels->alpha, mixChannels->alpha);
        }
        supervisor->d3dDevice08->Clear(
            0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
            color, 1.0f, 0);
    }
    else
    {
        if (background->clearColor != 0)
        {
            ScreenEffectRect rect = {32.0f, 16.0f, 416.0f, 464.0f};
            ScreenEffect::DrawSquare(&rect, background->skyFog.color);
            supervisor->d3dDevice08->Clear(
                0, NULL, D3DCLEAR_ZBUFFER, background->clearColor, 1.0f, 0);
        }
        else
        {
            supervisor->d3dDevice08->Clear(
                0, NULL, D3DCLEAR_ZBUFFER, 0, 1.0f, 0);
        }
    }

    if (background->spellBackgroundState <= 1)
    {
        if (background->stageVm0.activeSpriteIndex > 0)
        {
            AsciiGameManagerView *gameManager =
                reinterpret_cast<AsciiGameManagerView *>(&g_GameManager);
            background->stageVm0.pos.x =
                gameManager->TransformPopupX(-144.0f);
            background->stageVm0.pos.z = 0.99f;
            g_AnmManager->Draw2DAndFlush(&background->stageVm0);
        }

        if (background->stageVm1.activeSpriteIndex > 0)
            g_AnmManager->Draw2DAndFlush(&background->stageVm1);
    }

    g_Supervisor.SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
    if (reinterpret_cast<AnmManagerRenderCacheView *>(g_AnmManager)
            ->useMixColor == 0)
    {
        g_Supervisor.SetRenderState(
            D3DRS_FOGCOLOR, background->skyFog.color);
    }
    else
    {
        BackgroundRenderColor fogColor;
        fogColor.value = background->skyFog.color;
        BackgroundTintColorView *mixChannels =
            reinterpret_cast<BackgroundTintColorView *>(
                &reinterpret_cast<AnmManagerRenderCacheView *>(g_AnmManager)
                     ->mixColor);
        fogColor.channels.red =
            MixRenderColor(fogColor.channels.red, mixChannels->red);
        fogColor.channels.green =
            MixRenderColor(fogColor.channels.green, mixChannels->green);
        fogColor.channels.blue = MixRenderColor(
            fogColor.channels.blue, mixChannels->blue);
        g_Supervisor.SetRenderState(D3DRS_FOGCOLOR, fogColor.value);
    }
    g_Supervisor.SetRenderState(
        D3DRS_FOGSTART, *reinterpret_cast<int *>(&background->skyFog.nearPlane));
    g_Supervisor.SetRenderState(
        D3DRS_FOGEND, *reinterpret_cast<int *>(&background->skyFog.farPlane));
    if (!g_Supervisor.IsFogDisabled())
        g_Supervisor.EnableFog();

    if (background->spellBackgroundState <= 1)
    {
        background->RenderObjects(0);
        background->RenderObjects(1);
    }
    if (!background->retainTint)
        g_AnmManager->SetMixColorDefault();
    return CHAIN_CALLBACK_RESULT_CONTINUE;
}

int Background::OnDrawLowPrio(Background *background)
{
    g_Supervisor.ConfigureGameplayViewport(background->viewportIndex);
    if (background->spellBackgroundState <= 1)
    {
        background->RenderObjects(2);
        background->RenderObjects(3);
        if (!g_Supervisor.IsFogDisabled())
            g_Supervisor.DisableFog();
    }

    BackgroundTintColorView *tint =
        reinterpret_cast<BackgroundTintColorView *>(&background->tintColor);
    if (tint->alpha > 0)
        g_AnmManager->SetMixColor(background->tintColor);
    tint->alpha = 0;
    tint->red = 0x80;
    tint->green = 0x80;
    tint->blue = 0x80;

    g_AnmManager->FlushVertexBuffer();
    g_Supervisor.SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);
    if (!g_Supervisor.IsFogDisabled())
        g_Supervisor.DisableFog();

    if (background->spellBackgroundState >= 1)
    {
        for (int i = 0; i < background->spellVmCount; i++)
        {
            AnmVm *vm = &background->spellVms[i];
            AsciiGameManagerView *gameManager =
                reinterpret_cast<AsciiGameManagerView *>(&g_GameManager);
            vm->pos.x =
                gameManager->TransformPopupX(vm->pos2.x);
            vm->pos.y =
                gameManager->TransformPopupY(vm->pos2.y);
            vm->pos.z = vm->pos2.z;
            g_AnmManager->Draw2DAndFlush(&background->spellVms[i]);
        }
        if (background->spellBackgroundDrawCallback != NULL)
            background->spellBackgroundDrawCallback();
    }

    g_AnmManager->SetCameraMode(0);
    g_Supervisor.ApplyCameraMode0();
    float zValue = 1000.0f;
    g_Supervisor.SetRenderState(
        D3DRS_FOGSTART, *reinterpret_cast<int *>(&zValue));
    zValue = 2000.0f;
    g_Supervisor.SetRenderState(
        D3DRS_FOGEND, *reinterpret_cast<int *>(&zValue));
    if (!background->retainTint)
        g_AnmManager->SetMixColorDefault();
    background->retainTint = 0;
    background->collectSpecialEffectPoints = 0;
    return CHAIN_CALLBACK_RESULT_CONTINUE;
}
