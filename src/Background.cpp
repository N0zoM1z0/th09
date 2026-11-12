#include "Background.hpp"

#include "AnmManager.hpp"
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

struct BackgroundSideStateView
{
    unsigned char unknown000[0x34];
    unsigned char flags34;
    unsigned char unknown035[3];
};

typedef char BackgroundSideStateSizeIs38[
    (sizeof(BackgroundSideStateView) == 0x38) ? 1 : -1];

struct BackgroundGameManagerView
{
    BackgroundSideStateView sides[2];
    unsigned char unknown070[0xF4 - 0x70];
    int currentStage;
    unsigned char unknown0F8[0x134 - 0xF8];
    unsigned int flags134;
};

extern BackgroundGameManagerView g_GameManager;

struct BackgroundStageFiles
{
    const char *anmPath;
    const char *stdPath;
};
extern BackgroundStageFiles g_BackgroundStageFiles[];

// Spell VM and loaded stage VM indices select genuine 0x2A4-byte AnmVm
// records from two independent allocations, with unsigned DWORD offsets.
#define TH09_BG_NATIVE_SPELL_VM(owner, index) \
    (*reinterpret_cast<AnmVm *>( \
        reinterpret_cast<unsigned int>(&(owner)->spellVms[0]) + \
        static_cast<unsigned int>(index) * sizeof((owner)->spellVms[0])))
#define TH09_BG_NATIVE_STAGE_VM(owner, index) \
    (*reinterpret_cast<AnmVm *>( \
        reinterpret_cast<unsigned int>((owner)->stageObjectVms) + \
        static_cast<unsigned int>(index) * sizeof(AnmVm)))


// Loaded stage path descriptors occupy the original two 4-byte pointers;
// the live signed stage selector scales to one native 0x08-byte row.
typedef char BackgroundNativeStageFileRowSizeIs08[
    sizeof(BackgroundStageFiles) == 0x08 ? 1 : -1];
static __forceinline const BackgroundStageFiles &NativeBackgroundStageFile(int index)
{
    return *reinterpret_cast<const BackgroundStageFiles *>(
        reinterpret_cast<unsigned int>(&g_BackgroundStageFiles[0]) +
        static_cast<unsigned int>(index) * sizeof(g_BackgroundStageFiles[0]));
}


struct BackgroundAnmLoadedView
{
    unsigned char unknown000[4];
    void *rawData;
    unsigned char unknown008[0x20 - 0x08];
    char filename[0x100];
};

struct BackgroundSupervisorView
{
    unsigned char unknown000[8];
    IDirect3DDevice8 *d3dDevice;
};

struct BackgroundStageVmRuntimeView
{
    unsigned char unknown000[0x1F0];
    unsigned int color1;
    unsigned char unknown1F4[0x214 - 0x1F4];
    short scriptIndex;
};

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

struct BackgroundAnmManagerMixView
{
    unsigned int mixColor;
    int useMixColor;
};

typedef char BackgroundRenderColorSizeIs4[
    (sizeof(BackgroundRenderColor) == 4) ? 1 : -1];
typedef char BackgroundAnmManagerUseMixColorAt4[
    (offsetof(BackgroundAnmManagerMixView, useMixColor) == 4) ? 1 : -1];

static unsigned char MixRenderColor(
    unsigned char color, unsigned char mixColor)
{
    unsigned int result = ((color * mixColor) / 128U);
    if (result >= 256)
        result = 255;
    return static_cast<unsigned char>(result);
}

struct BackgroundSpellVmDrawView
{
    unsigned char unknown000[0x208];
    BackgroundPoint position208;
    unsigned char unknown214[0x288 - 0x214];
    BackgroundPoint position288;
    unsigned char unknown294[0x2A4 - 0x294];
};

struct AsciiGameManagerView
{
    float TransformPopupX(float value);
    float TransformPopupY(float value);
};

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

    background->sideState = reinterpret_cast<BackgroundSideStateView *>(
        reinterpret_cast<unsigned int>(&g_GameManager.sides[0]) +
        static_cast<unsigned int>(viewportIndex) *
        sizeof(g_GameManager.sides[0]));
    background->frameCounter = 0;
    background->viewportIndex = viewportIndex;
    background->otherSideState = reinterpret_cast<BackgroundSideStateView *>(
        reinterpret_cast<unsigned int>(&g_GameManager.sides[0]) +
        static_cast<unsigned int>(static_cast<int>(1u - static_cast<unsigned int>(viewportIndex))) *
        sizeof(g_GameManager.sides[0]));

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
        reinterpret_cast<unsigned int>(this->stageData) +
        static_cast<unsigned int>(this->stageData->objectInstancesOffset));
    this->stageScript = reinterpret_cast<RawStageInstr *>(
        reinterpret_cast<unsigned int>(this->stageData) +
        static_cast<unsigned int>(this->stageData->scriptOffset));
    this->stageObjects = reinterpret_cast<RawStageObject **>(
        reinterpret_cast<unsigned int>(this->stageData) + sizeof(RawStageHeader));

    for (objectIndex = 0; objectIndex < this->stageObjectCount; objectIndex++)
    {
        // A loaded STD object-pointer slot contains a serialized file
        // offset. Retail fixes it up using two native Win32 address words.
        this->stageObjects[objectIndex] = reinterpret_cast<RawStageObject *>(
            reinterpret_cast<unsigned int>(this->stageObjects[objectIndex]) +
            reinterpret_cast<unsigned int>(this->stageData));
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
                &TH09_BG_NATIVE_STAGE_VM(this, vmIndex), currentQuad->anmScript);
            currentQuad->vmIndex = static_cast<short>(vmIndex++);
            currentQuad = reinterpret_cast<RawStageQuadBasic *>(
                reinterpret_cast<unsigned int>(currentQuad) +
                static_cast<unsigned int>(currentQuad->byteSize));
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

    BackgroundAnmLoadedView *loaded =
        reinterpret_cast<BackgroundAnmLoadedView *>(g_AnmManager->GetAnm(4));
    if (loaded->rawData != NULL &&
        strcmp(loaded->filename, NativeBackgroundStageFile(g_GameManager.currentStage).anmPath) == 0)
    {
        background->stageAnm = reinterpret_cast<AnmLoaded *>(loaded);
    }
    else
    {
        background->stageAnm = g_AnmManager->PreloadAnm(
            4, NativeBackgroundStageFile(g_GameManager.currentStage).anmPath);
        if (background->stageAnm == NULL)
            return -1;
    }

    background->sideAnm = g_AnmManager->GetAnm(background->viewportIndex + 5);
    if (background->LoadStageData(
            NativeBackgroundStageFile(g_GameManager.currentStage).stdPath) != 0)
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
    if (g_GameManager.currentStage == 5)
    {
        background->cullingDistanceSq = 3062500.0f;
    }
    else if (g_GameManager.currentStage == 9)
    {
        background->cullingDistanceSq = 3062500.0f;
    }
    else if (g_GameManager.currentStage == 11)
    {
        background->cullingDistanceSq = 4202500.0f;
    }
    else if (g_GameManager.currentStage == 12 ||
             g_GameManager.currentStage == 15)
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
        --*reinterpret_cast<unsigned int *>(&background->transitionFrames);
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

    if ((g_GameManager.flags134 & 0x1800) != 0)
        return CHAIN_CALLBACK_RESULT_CONTINUE;

    BackgroundSideStateView *side =
        static_cast<BackgroundSideStateView *>(background->sideState);
    if ((side->flags34 & 1) != 0)
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
            ++*reinterpret_cast<unsigned int *>(&background->spellBackgroundState);
        ++*reinterpret_cast<unsigned int *>(&background->spellBackgroundFrame);
        for (int i = 0; i < background->spellVmCount; i++)
            g_AnmManager->ExecuteScript(&TH09_BG_NATIVE_SPELL_VM(background, i));
    }

    BackgroundStageVmRuntimeView *stageVm0 =
        reinterpret_cast<BackgroundStageVmRuntimeView *>(&background->stageVm0);
    if (stageVm0->scriptIndex > 0)
        g_AnmManager->ExecuteScript(&background->stageVm0);

    BackgroundStageVmRuntimeView *stageVm1 =
        reinterpret_cast<BackgroundStageVmRuntimeView *>(&background->stageVm1);
    if (stageVm1->scriptIndex > 0)
        g_AnmManager->ExecuteScript(&background->stageVm1);

    if (reinterpret_cast<BackgroundStageVmRuntimeView *>(
            &background->stageVm2)->scriptIndex > 0)
    {
        g_AnmManager->ExecuteScript(&background->stageVm2);
        background->clearColor =
            reinterpret_cast<BackgroundStageVmRuntimeView *>(
                &background->stageVm2)->color1;
    }

    ++*reinterpret_cast<unsigned int *>(&background->frameCounter);
    return CHAIN_CALLBACK_RESULT_CONTINUE;
}

int Background::OnDrawHighPrio(Background *background)
{
    BackgroundSupervisorView *supervisor =
        reinterpret_cast<BackgroundSupervisorView *>(&g_Supervisor);
    BackgroundTintColorView *tint =
        reinterpret_cast<BackgroundTintColorView *>(&background->tintColor);

    if (!g_Supervisor.IsFogDisabled())
        g_Supervisor.DisableFog();
    g_AnmManager->FlushVertexBuffer();
    g_AnmManager->ClearTexture();
    g_Supervisor.ConfigureGameplayViewport(background->viewportIndex);

    if (background->clearPending != 0)
    {
        supervisor->d3dDevice->Clear(
            0, NULL, D3DCLEAR_TARGET, 0xFF000000, 1.0f, 0);
        background->clearPending = 0;
    }

    if (tint->alpha > 0)
        g_AnmManager->SetMixColor(background->tintColor);

    if ((background->clearColor & 0xFF000000) == 0xFF000000)
    {
        unsigned int color = background->skyFog.color;
        if (reinterpret_cast<BackgroundAnmManagerMixView *>(g_AnmManager)
                ->useMixColor != 0)
        {
            BackgroundTintColorView *colorChannels =
                reinterpret_cast<BackgroundTintColorView *>(&color);
            BackgroundTintColorView *mixChannels =
                reinterpret_cast<BackgroundTintColorView *>(
                    &reinterpret_cast<BackgroundAnmManagerMixView *>(g_AnmManager)
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
        supervisor->d3dDevice->Clear(
            0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
            color, 1.0f, 0);
    }
    else
    {
        if (background->clearColor != 0)
        {
            ScreenEffectRect rect = {32.0f, 16.0f, 416.0f, 464.0f};
            ScreenEffect::DrawSquare(&rect, background->skyFog.color);
            supervisor->d3dDevice->Clear(
                0, NULL, D3DCLEAR_ZBUFFER, background->clearColor, 1.0f, 0);
        }
        else
        {
            supervisor->d3dDevice->Clear(
                0, NULL, D3DCLEAR_ZBUFFER, 0, 1.0f, 0);
        }
    }

    if (background->spellBackgroundState <= 1)
    {
        BackgroundStageVmRuntimeView *stageVm0 =
            reinterpret_cast<BackgroundStageVmRuntimeView *>(
                &background->stageVm0);
        if (stageVm0->scriptIndex > 0)
        {
            AsciiGameManagerView *gameManager =
                reinterpret_cast<AsciiGameManagerView *>(&g_GameManager);
            reinterpret_cast<BackgroundSpellVmDrawView *>(
                &background->stageVm0)->position208.x =
                gameManager->TransformPopupX(-144.0f);
            reinterpret_cast<BackgroundSpellVmDrawView *>(
                &background->stageVm0)->position208.z = 0.99f;
            g_AnmManager->Draw2DAndFlush(&background->stageVm0);
        }

        BackgroundStageVmRuntimeView *stageVm1 =
            reinterpret_cast<BackgroundStageVmRuntimeView *>(
                &background->stageVm1);
        if (stageVm1->scriptIndex > 0)
            g_AnmManager->Draw2DAndFlush(&background->stageVm1);
    }

    g_Supervisor.SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
    if (reinterpret_cast<BackgroundAnmManagerMixView *>(g_AnmManager)
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
                &reinterpret_cast<BackgroundAnmManagerMixView *>(g_AnmManager)
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
            BackgroundSpellVmDrawView *vm =
                reinterpret_cast<BackgroundSpellVmDrawView *>(
                    &TH09_BG_NATIVE_SPELL_VM(background, i));
            AsciiGameManagerView *gameManager =
                reinterpret_cast<AsciiGameManagerView *>(&g_GameManager);
            vm->position208.x =
                gameManager->TransformPopupX(vm->position288.x);
            vm->position208.y =
                gameManager->TransformPopupY(vm->position288.y);
            vm->position208.z = vm->position288.z;
            g_AnmManager->Draw2DAndFlush(&TH09_BG_NATIVE_SPELL_VM(background, i));
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
