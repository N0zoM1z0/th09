#include "EffectManager.hpp"
#include "AsciiManager.hpp"
#include "Supervisor.hpp"
#include "ZunMemory.hpp"

#include <new>
#include <string.h>

struct AnmVm;
struct EffectAnmLoadedMethodView
{
    void SetAndExecuteScriptIdx(AnmVm *vm, int scriptIndex);
};

struct EffectGameManagerSideView
{
    unsigned char unknown000[0x34];
    unsigned char flags34;
    unsigned char unknown035[3];
};
typedef char EffectGameManagerSideSizeIs38[(sizeof(EffectGameManagerSideView) == 0x38) ? 1 : -1];

struct EffectGameManagerView
{
    float TransformPopupX(float value);
    float TransformPopupY(float value);
    EffectGameManagerSideView sides[3];
    unsigned char unknown0A8[0x134 - 0xA8];
    unsigned int flags134;
};

struct EffectAnmManagerView
{
    void *GetAnm(int anmIndex);
    void *PreloadAnm(int anmIndex, const char *path);
    int ExecuteScript(EffectVmView *vm);
    int Draw2D(EffectVmView *vm);
};

struct EffectTemplate
{
    int scriptIndex;
    int scriptStep;
    int vmCount;
    EffectCallback initializeCallback;
    EffectCallback updateCallback;
    EffectDrawCallback drawCallback;
};
typedef char EffectTemplateSizeIs18[(sizeof(EffectTemplate) == 0x18) ? 1 : -1];

extern EffectGameManagerView g_GameManager;
extern EffectAnmManagerView *g_AnmManager;
extern EffectTemplate g_EffectTemplates[];
extern Chain g_Chain;
extern ZunMemory g_ZunMemory;

Effect::Effect() {}


struct EffectVmUpdateView
{
    unsigned char unknown000[0x04];
    int value004;
    int value008;
    unsigned char unknown00C[0x18 - 0x0C];
    int value018;
    unsigned char unknown01C[0x100 - 0x1C];
    int value100;
    int value104;
    unsigned char unknown108[0x288 - 0x108];
    int value288;
    int value28C;
};
typedef char EffectVmUpdateValue100At100[(offsetof(EffectVmUpdateView, value100) == 0x100) ? 1 : -1];
typedef char EffectVmUpdateValue288At288[(offsetof(EffectVmUpdateView, value288) == 0x288) ? 1 : -1];

int __fastcall EffectUpdateFromVm(Effect *effect)
{
    EffectVmUpdateView *vm = reinterpret_cast<EffectVmUpdateView *>(effect->vms);
    effect->value8C = vm->value100;
    effect->value9C = static_cast<float>(vm->value104);
    effect->value88 = vm->value018;
    effect->value7C = vm->value288;
    effect->value94 = vm->value28C;
    effect->value80 = vm->value008;
    effect->value98 = vm->value004;
    effect->unknown0C9 = 1;
    return 1;
}


void EffectManager::Clear()
{
    memset(this, 0, sizeof(EffectManager));
}

void EffectManager::ResetPool()
{
    Effect *effect = this->effects;
    this->drawSentinel0.nextInDrawGroup = NULL;
    for (int i = 0; i < this->primaryCount + this->secondaryCount + 1; ++i, ++effect)
    {
        if (effect->vertices != NULL)
        {
            g_ZunMemory.Free(effect->vertices);
            effect->vertices = NULL;
        }
        if (effect->vms != NULL)
        {
            g_ZunMemory.Free(effect->vms);
            effect->vertices = NULL;
        }
    }
    g_ZunMemory.Free(this->effects);
    this->effects = static_cast<Effect *>(g_ZunMemory.Alloc(
        sizeof(Effect) * (this->primaryCount + this->secondaryCount + 1),
        "./system\\global.h"));
    memset(this->effects, 0,
           sizeof(Effect) * (this->primaryCount + this->secondaryCount + 1));
}

Effect *EffectManager::InitializeEffect(
    Effect *effect, int effectId, const EffectFloat3 *position,
    unsigned int color, const EffectFloat3 *velocity)
{
    if (effect->vertices != NULL)
        g_ZunMemory.Free(effect->vertices);
    if (effect->vms != NULL)
        g_ZunMemory.Free(effect->vms);
    memset(effect, 0, sizeof(Effect));
    effect->effectId = static_cast<unsigned char>(effectId);
    effect->active = 1;
    effect->position = *position;
    effect->velocity = *velocity;

    effect->vmCount = g_EffectTemplates[effectId].vmCount;
    effect->manager = this;
    if (effect->vmCount != 0)
    {
        effect->vms = static_cast<EffectVmView *>(
            g_ZunMemory.Alloc(
                sizeof(EffectVmView) * g_EffectTemplates[effectId].vmCount,
                "./system\\global.h"));
        int scriptIndex = g_EffectTemplates[effectId].scriptIndex;
        for (int i = 0; i < g_EffectTemplates[effectId].vmCount; ++i)
        {
            reinterpret_cast<EffectAnmLoadedMethodView *>(this->effectAnm)
                ->SetAndExecuteScriptIdx(
                    reinterpret_cast<AnmVm *>(&effect->vms[i]), scriptIndex);
            scriptIndex += g_EffectTemplates[effectId].scriptStep;
            effect->vms[i].flags |= 0x2000;
            effect->vms[i].color = color;
            effect->vms[i].pos2.x = 0.0f;
            effect->vms[i].pos2.y = 0.0f;
            effect->vms[i].pos2.z = 0.0f;
        }
    }
    effect->updateCallback = g_EffectTemplates[effectId].updateCallback;
    effect->drawCallback = g_EffectTemplates[effectId].drawCallback;
    if (g_EffectTemplates[effectId].initializeCallback != NULL &&
        g_EffectTemplates[effectId].initializeCallback(effect) != 0)
        effect->active = 0;
    return effect;
}

Effect *EffectManager::SpawnEffect(
    int effectId, const EffectFloat3 *position, int count, unsigned int color)
{
    int effectIndex = this->nextEffectIndex;
    Effect *effect = this->effects + effectIndex;
    Float3 velocity(0.0f, 0.0f, 0.0f);
    int i;

    for (i = 0; i < this->primaryCount; ++i)
    {
        effectIndex++;
        this->nextEffectIndex = effectIndex;
        if (effectIndex >= this->primaryCount)
            this->nextEffectIndex = 0;

        if (effect->active == 0)
        {
            this->InitializeEffect(
                effect, effectId, position, color,
                reinterpret_cast<const EffectFloat3 *>(&velocity));
            count--;
            if (count == 0)
                break;
        }

        effectIndex = this->nextEffectIndex;
        if (effectIndex == 0)
            effect = this->effects;
        else
            effect++;
    }

    return i >= this->primaryCount
        ? &this->effects[this->primaryCount + this->secondaryCount]
        : effect;
}

Effect *EffectManager::SpawnEffectWithVelocity(
    int effectId, const EffectFloat3 *position, const EffectFloat3 *velocity,
    int count, unsigned int color)
{
    Effect *effect = this->effects + this->nextEffectIndex;
    int i;

    for (i = 0; i < this->primaryCount; ++i)
    {
        this->nextEffectIndex++;
        if (this->nextEffectIndex >= this->primaryCount)
            this->nextEffectIndex = 0;

        if (effect->active == 0)
        {
            this->InitializeEffect(effect, effectId, position, color, velocity);
            count--;
            if (count == 0)
                break;
        }

        if (this->nextEffectIndex == 0)
            effect = this->effects;
        else
            effect++;
    }

    return i >= this->primaryCount
        ? &this->effects[this->primaryCount + this->secondaryCount]
        : effect;
}

Effect *EffectManager::SpawnEffectInFixedSlot(
    int effectId, const EffectFloat3 *position, int slotIndex, unsigned int color)
{
    Effect *effect = &this->effects[this->primaryCount + slotIndex];
    Float3 velocity(0.0f, 0.0f, 0.0f);
    this->InitializeEffect(
        effect, effectId, position, color,
        reinterpret_cast<const EffectFloat3 *>(&velocity));
    effect->fixedSlotIndex = slotIndex;
    return effect;
}

int EffectManager::OnUpdate(EffectManager *effectManager)
{
    if ((g_GameManager.flags134 & 0x1800) != 0 || (g_GameManager.sides[0].flags34 & 1) != 0)
        return CHAIN_CALLBACK_RESULT_CONTINUE;

    g_Supervisor.SelectSide(effectManager->sideIndex);
    effectManager->activeCount = 0;
    effectManager->drawTail0 = &effectManager->drawSentinel0;
    effectManager->drawTail1 = &effectManager->drawSentinel1;
    effectManager->drawTail2 = &effectManager->drawSentinel2;
    effectManager->drawSentinel0.nextInDrawGroup = NULL;
    effectManager->drawSentinel1.nextInDrawGroup = NULL;
    effectManager->drawSentinel2.nextInDrawGroup = NULL;

    Effect *effect = effectManager->effects;
    for (int i = 0;
         i < effectManager->secondaryCount + effectManager->primaryCount;
         ++i, ++effect)
    {
        if (effect->active == 0)
        {
            if (effect->vertices != NULL)
            {
                g_ZunMemory.Free(effect->vertices);
                effect->vertices = NULL;
            }
            if (effect->vms != NULL)
            {
                g_ZunMemory.Free(effect->vms);
                effect->vms = NULL;
            }
            continue;
        }

        effectManager->activeCount++;
        if (effect->updateCallback != NULL && effect->updateCallback(effect) != 1)
        {
            effect->active = 0;
            continue;
        }

        int liveVmCount = effect->vmCount;
        for (int vmIndex = 0; vmIndex < effect->vmCount; ++vmIndex)
        {
            if (g_AnmManager->ExecuteScript(&effect->vms[vmIndex]) != 0)
                --liveVmCount;
        }
        if (liveVmCount == 0)
        {
            effect->active = 0;
            continue;
        }

        effect->timer++;
        effect->nextInDrawGroup = NULL;
        if (effect->suppressDraw != 0)
            continue;
        if (effect->backgroundDrawGroup != 0)
        {
            effectManager->drawTail2->nextInDrawGroup = effect;
            effectManager->drawTail2 = effect;
        }
        else if (effect->alternateDrawGroup != 0)
        {
            effectManager->drawTail1->nextInDrawGroup = effect;
            effectManager->drawTail1 = effect;
        }
        else
        {
            effectManager->drawTail0->nextInDrawGroup = effect;
            effectManager->drawTail0 = effect;
        }
    }
    effectManager->updateCounter++;
    return CHAIN_CALLBACK_RESULT_CONTINUE;
}


int EffectManager::OnDraw(EffectManager *effectManager)
{
    Effect *effect = effectManager->drawSentinel0.nextInDrawGroup;
    g_Supervisor.ConfigureGameplayViewport(effectManager->sideIndex);
    for (; effect != NULL; effect = effect->nextInDrawGroup)
    {
        if (effect->drawCallback != NULL)
        {
            effect->drawCallback(effect);
        }
        else
        {
            EffectVmView *vm = effect->vms;
            if (effect->vmCount > 0)
            {
                int vmCount = effect->vmCount;
                do
                {
                    vm->position.x = g_GameManager.TransformPopupX(effect->position.x);
                    vm->position.y = g_GameManager.TransformPopupY(effect->position.y);
                    vm->position.z = 0.08f;
                    vm->position.Add(&vm->pos2);
                    g_AnmManager->Draw2D(vm);
                    ++vm;
                } while (--vmCount != 0);
            }
        }
    }
    return CHAIN_CALLBACK_RESULT_CONTINUE;
}

int EffectManager::OnDrawShared(EffectManager *effectManager)
{
    Effect *effect = effectManager->drawSentinel0.nextInDrawGroup;
    g_Supervisor.ConfigureGameplayViewport(effectManager->sideIndex);
    for (; effect != NULL; effect = effect->nextInDrawGroup)
    {
        if (effect->drawCallback != NULL)
        {
            effect->drawCallback(effect);
        }
        else
        {
            int vmCount = effect->vmCount;
            EffectVmView *vm = effect->vms;
            if (vmCount > 0)
            {
                do
                {
                    vm->position.x = effect->position.x;
                    vm->position.y = effect->position.y;
                    vm->position.z = 0.0f;
                    vm->position.Add(&vm->pos2);
                    g_AnmManager->Draw2D(vm);
                    ++vm;
                } while (--vmCount != 0);
            }
        }
    }
    return CHAIN_CALLBACK_RESULT_CONTINUE;
}

int EffectManager::AddedCallback(EffectManager *effectManager)
{
    EffectManager *manager = effectManager;
    EffectAnmManagerView *anmManager = g_AnmManager;
    void *effectAnm = anmManager->GetAnm(8);
    if (reinterpret_cast<void **>(effectAnm)[1] == NULL)
        effectAnm = anmManager->PreloadAnm(8, "etama.anm");
    manager->effectAnm = reinterpret_cast<AnmLoaded *>(effectAnm);
    return 0;
}

int EffectManager::ReleasePool()
{
    Effect *effect = this->effects;
    for (int i = 0; i < this->primaryCount + this->secondaryCount + 1; ++i, ++effect)
    {
        if (effect->vertices != NULL)
        {
            g_ZunMemory.Free(effect->vertices);
            effect->vertices = NULL;
        }
        if (effect->vms != NULL)
        {
            g_ZunMemory.Free(effect->vms);
            effect->vertices = NULL;
        }
    }
    g_ZunMemory.Free(this->effects);
    return 0;
}

void EffectManager::Release(EffectManager *effectManager)
{
    if (effectManager == NULL)
        return;
    effectManager->ReleasePool();
    g_Chain.Cut(effectManager->calcChain);
    g_Chain.Cut(effectManager->drawChain);
    delete effectManager;
}

EffectManager::EffectManager()
{
    this->Clear();
    this->scaleX = 1.0f;
    this->scaleY = 1.0f;
    this->scaleZ = 1.0f;
    this->scaleW = 1.0f;
}

EffectManager *EffectManager::Create(int sideIndex, int primaryCount, int secondaryCount)
{
    EffectManager *effectManager = new EffectManager;
    effectManager = static_cast<EffectManager *>(
        g_ZunMemory.AddToRegistry(effectManager, sizeof(EffectManager), const_cast<char *>("EffectSysInf")));
    effectManager->Clear();
    effectManager->sideIndex = sideIndex;
    if (sideIndex < 2)
    {
        effectManager->sideState = &g_GameManager.sides[sideIndex];
        effectManager->opposingSideState = &g_GameManager.sides[1 - sideIndex];
    }
    else
    {
        effectManager->sideState = &g_GameManager.sides[sideIndex];
        effectManager->opposingSideState = effectManager->sideState;
    }
    effectManager->primaryCount = primaryCount;
    effectManager->secondaryCount = secondaryCount;

    int count = primaryCount + secondaryCount + 1;
    unsigned int bytes = sizeof(Effect) * count;
    effectManager->effects = static_cast<Effect *>(g_ZunMemory.Alloc(bytes, "./system\\global.h"));
    memset(effectManager->effects, 0, bytes);

    effectManager->calcChain = g_Chain.CreateElem(reinterpret_cast<ChainCallback>(EffectManager::OnUpdate));
    effectManager->calcChain->arg = effectManager;
    effectManager->calcChain->addedCallback = reinterpret_cast<ChainLifetimeCallback>(EffectManager::AddedCallback);
    if (g_Chain.AddToCalcChain(effectManager->calcChain, sideIndex + 19) != 0)
        return NULL;

    if (sideIndex < 2)
    {
        effectManager->drawChain = g_Chain.CreateElem(reinterpret_cast<ChainCallback>(EffectManager::OnDraw));
        effectManager->drawChain->arg = effectManager;
        g_Chain.AddToDrawChain(effectManager->drawChain, sideIndex + 19);
    }
    else
    {
        effectManager->drawChain = g_Chain.CreateElem(reinterpret_cast<ChainCallback>(EffectManager::OnDrawShared));
        effectManager->drawChain->arg = effectManager;
        g_Chain.AddToDrawChain(effectManager->drawChain, 30);
    }
    return effectManager;
}
