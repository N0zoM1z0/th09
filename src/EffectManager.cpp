#include "EffectManager.hpp"
#include "EffectNativePoolCapacity.hpp"
#include "GameplaySetupNativeSides.hpp"
#include "AnmManager.hpp"
#include "AnmVmLifecycle.hpp"
#include "AsciiManager.hpp"
#include "AsciiGameManagerView.hpp"
#include "Supervisor.hpp"
#include "ZunMemory.hpp"

#include <new>
#include <string.h>

struct AnmVm;
// The shared view has two typed sides; other context addresses remain opaque.
typedef char EffectSharedSideFlagsAt34[
    (offsetof(AsciiGameManagerSideView, flags) == 0x34) ? 1 : -1];

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

extern int __fastcall EffectInitializeCallbackType1Type2(Effect *effect);
extern int __fastcall EffectUpdateCallbackType1Type2(Effect *effect);
extern int __fastcall EffectInitializeCallbackType3Type4(Effect *effect);
extern int __fastcall EffectUpdateCallbackType3Type4(Effect *effect);
extern int __fastcall EffectInitializeCallbackRing(Effect *effect);
extern int __fastcall EffectUpdateFromVm(Effect *effect);
extern int __fastcall EffectDrawCallbackRing(Effect *effect);
extern int __fastcall EffectFollowPlayerUpdate(Effect *effect);
extern int __fastcall EffectInitializeCallbackRadialFan(Effect *effect);
extern int __fastcall EffectUpdateCallbackType10(Effect *effect);
extern int __fastcall EffectDrawCallbackType10(Effect *effect);
extern int __fastcall EffectInitializeCallbackOrientedOffset(Effect *effect);
extern int __fastcall EffectInitializeCallbackType16(Effect *effect);
extern int __fastcall EffectUpdateCallbackType16(Effect *effect);
extern int __fastcall EffectDrawCallbackType16(Effect *effect);
extern int __fastcall EffectInitializeCallbackType17(Effect *effect);
extern int __fastcall EffectUpdateCallbackType17(Effect *effect);
extern int __fastcall EffectDrawCallbackType17(Effect *effect);
extern int __fastcall EffectUpdateCallbackType18(Effect *effect);
extern int __fastcall EffectDrawCallbackType18(Effect *effect);
extern int __fastcall EffectUpdateCallbackType24(Effect *effect);
extern int __fastcall EffectDrawCallbackType24(Effect *effect);
extern int __fastcall EffectUpdateCallbackPlayerOffsetFan(Effect *effect);
extern int __fastcall EffectDrawCallbackPlayerOffsetFan(Effect *effect);
extern int __fastcall EffectUpdateCallbackType26(Effect *effect);
extern int __fastcall EffectDrawCallbackType26(Effect *effect);
extern int __fastcall EffectInitializeCallbackStripType27Type36(Effect *effect);
extern int __fastcall EffectUpdateCallbackType27(Effect *effect);
extern int __fastcall EffectDrawCallbackType27(Effect *effect);
extern int __fastcall EffectUpdateCallbackType28(Effect *effect);
extern int __fastcall EffectDrawCallbackType28(Effect *effect);
extern int __fastcall EffectInitializeCallbackType29(Effect *effect);
extern int __fastcall EffectUpdateCallbackType29(Effect *effect);
extern int __fastcall EffectDrawCallbackType29(Effect *effect);
extern int __fastcall EffectInitializeCallbackType30(Effect *effect);
extern int __fastcall EffectUpdateCallbackType30(Effect *effect);
extern int __fastcall EffectDrawCallbackType30(Effect *effect);
extern int __fastcall EffectInitializeCallbackType31(Effect *effect);
extern int __fastcall EffectUpdateCallbackType31(Effect *effect);
extern int __fastcall EffectDrawCallbackType31(Effect *effect);
extern int __fastcall EffectInitializeCallbackType32(Effect *effect);
extern int __fastcall EffectUpdateCallbackType32(Effect *effect);
extern int __fastcall EffectDrawCallbackType32(Effect *effect);
extern int __fastcall EffectUpdateCallbackType33(Effect *effect);
extern int __fastcall EffectDrawCallbackType33(Effect *effect);
extern int __fastcall EffectInitializeCallbackType34(Effect *effect);
extern int __fastcall EffectUpdateCallbackType34(Effect *effect);
extern int __fastcall EffectDrawCallbackType34(Effect *effect);
extern int __fastcall EffectInitializeCallbackType35(Effect *effect);
extern int __fastcall EffectUpdateCallbackType35(Effect *effect);
extern int __fastcall EffectUpdateCallbackType36(Effect *effect);
extern int __fastcall EffectDrawCallbackType36(Effect *effect);
extern int __fastcall EffectInitializeCallbackType37(Effect *effect);
extern int __fastcall EffectUpdateCallbackType37(Effect *effect);
extern int __fastcall EffectDrawCallbackType37(Effect *effect);

EffectTemplate g_EffectTemplates[38] = {
    { 28, 0, 1, NULL, NULL, NULL },
    { 33, 0, 1, EffectInitializeCallbackType1Type2, EffectUpdateCallbackType1Type2, NULL },
    { 34, 0, 1, EffectInitializeCallbackType1Type2, EffectUpdateCallbackType1Type2, NULL },
    { 35, 0, 1, EffectInitializeCallbackType3Type4, EffectUpdateCallbackType3Type4, NULL },
    { 36, 0, 1, EffectInitializeCallbackType3Type4, EffectUpdateCallbackType3Type4, NULL },
    { 32, 0, 1, NULL, NULL, NULL },
    { 86, 0, 1, EffectInitializeCallbackRing, EffectUpdateFromVm, reinterpret_cast<EffectDrawCallback>(EffectDrawCallbackRing) },
    { 63, 0, 1, NULL, EffectFollowPlayerUpdate, NULL },
    { 87, 0, 1, EffectInitializeCallbackRing, EffectUpdateFromVm, reinterpret_cast<EffectDrawCallback>(EffectDrawCallbackRing) },
    { 85, 0, 1, NULL, NULL, NULL },
    { 88, 0, 1, EffectInitializeCallbackRadialFan, EffectUpdateCallbackType10, reinterpret_cast<EffectDrawCallback>(EffectDrawCallbackType10) },
    { 89, 0, 1, EffectInitializeCallbackOrientedOffset, NULL, NULL },
    { 90, 0, 1, EffectInitializeCallbackOrientedOffset, NULL, NULL },
    { 91, 0, 1, EffectInitializeCallbackRing, EffectUpdateFromVm, reinterpret_cast<EffectDrawCallback>(EffectDrawCallbackRing) },
    { 92, 0, 1, EffectInitializeCallbackRing, EffectUpdateFromVm, reinterpret_cast<EffectDrawCallback>(EffectDrawCallbackRing) },
    { 93, 0, 1, EffectInitializeCallbackRing, EffectUpdateFromVm, reinterpret_cast<EffectDrawCallback>(EffectDrawCallbackRing) },
    { 88, 0, 1, EffectInitializeCallbackType16, EffectUpdateCallbackType16, reinterpret_cast<EffectDrawCallback>(EffectDrawCallbackType16) },
    { 88, 0, 1, EffectInitializeCallbackType17, EffectUpdateCallbackType17, reinterpret_cast<EffectDrawCallback>(EffectDrawCallbackType17) },
    { 88, 0, 1, EffectInitializeCallbackRadialFan, EffectUpdateCallbackType18, reinterpret_cast<EffectDrawCallback>(EffectDrawCallbackType18) },
    { 53, 0, 1, NULL, NULL, NULL },
    { 28, 0, 1, NULL, NULL, NULL },
    { 29, 0, 1, NULL, NULL, NULL },
    { 30, 0, 1, NULL, NULL, NULL },
    { 31, 0, 1, NULL, NULL, NULL },
    { 88, 0, 1, EffectInitializeCallbackRadialFan, EffectUpdateCallbackType24, reinterpret_cast<EffectDrawCallback>(EffectDrawCallbackType24) },
    { 88, 0, 1, EffectInitializeCallbackRadialFan, EffectUpdateCallbackPlayerOffsetFan, reinterpret_cast<EffectDrawCallback>(EffectDrawCallbackPlayerOffsetFan) },
    { 88, 0, 1, EffectInitializeCallbackRadialFan, EffectUpdateCallbackType26, reinterpret_cast<EffectDrawCallback>(EffectDrawCallbackType26) },
    { 88, 0, 1, EffectInitializeCallbackStripType27Type36, EffectUpdateCallbackType27, reinterpret_cast<EffectDrawCallback>(EffectDrawCallbackType27) },
    { 88, 0, 1, EffectInitializeCallbackRadialFan, EffectUpdateCallbackType28, reinterpret_cast<EffectDrawCallback>(EffectDrawCallbackType28) },
    { 88, 0, 1, EffectInitializeCallbackType29, EffectUpdateCallbackType29, reinterpret_cast<EffectDrawCallback>(EffectDrawCallbackType29) },
    { 88, 0, 1, EffectInitializeCallbackType30, EffectUpdateCallbackType30, reinterpret_cast<EffectDrawCallback>(EffectDrawCallbackType30) },
    { 88, 0, 1, EffectInitializeCallbackType31, EffectUpdateCallbackType31, reinterpret_cast<EffectDrawCallback>(EffectDrawCallbackType31) },
    { 88, 0, 1, EffectInitializeCallbackType32, EffectUpdateCallbackType32, reinterpret_cast<EffectDrawCallback>(EffectDrawCallbackType32) },
    { 88, 0, 1, EffectInitializeCallbackRadialFan, EffectUpdateCallbackType33, reinterpret_cast<EffectDrawCallback>(EffectDrawCallbackType33) },
    { 88, 0, 1, EffectInitializeCallbackType34, EffectUpdateCallbackType34, reinterpret_cast<EffectDrawCallback>(EffectDrawCallbackType34) },
    { 88, 0, 1, EffectInitializeCallbackType35, EffectUpdateCallbackType35, NULL },
    { 88, 0, 1, EffectInitializeCallbackStripType27Type36, EffectUpdateCallbackType36, reinterpret_cast<EffectDrawCallback>(EffectDrawCallbackType36) },
    { 88, 0, 1, EffectInitializeCallbackType37, EffectUpdateCallbackType37, reinterpret_cast<EffectDrawCallback>(EffectDrawCallbackType37) },
};
extern Chain g_Chain;
extern ZunMemory g_ZunMemory;

Effect::Effect() {}


// The native 0x2A4-byte ANM VM and Effect share these typed float slots.
// Scalar assignments preserve the original 32-bit transfers and avoid
// replacing the same physical fields with an unrelated byte-copy view.
typedef char EffectNativeVmSizeIs2A4[(sizeof(AnmVm) == 0x2A4) ? 1 : -1];
typedef char EffectUpdateVmNativeCoordinates[
    (sizeof(AnmVm) == 0x2A4 &&
     offsetof(AnmVmBase, rotation) == 0x00 &&
     offsetof(AnmVmBase, scale) == 0x18 &&
     offsetof(AnmVmBase, intVar0) == 0x100 &&
     offsetof(AnmVm, pos2) == 0x288) ? 1 : -1];

int __fastcall EffectUpdateFromVm(Effect *effect)
{
    AnmVm *vm = effect->vms;
    effect->value8C = vm->intVar0;
    effect->value9C = static_cast<float>(vm->intVar1);
    effect->thickness88 = vm->scale.x;
    effect->radius7C = vm->pos2.x;
    effect->ellipseY94 = vm->pos2.y;
    effect->angle80 = vm->rotation.z;
    effect->rotation98 = vm->rotation.y;
    effect->drawFlag0C9 = 1;
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
    for (int i = 0; i < EffectNativePoolCapacity::AllocatedCount(*this); ++i, ++effect)
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
        sizeof(Effect) * (EffectNativePoolCapacity::AllocatedCount(*this)),
        "./system\\global.h"));
    memset(this->effects, 0,
           sizeof(Effect) * (EffectNativePoolCapacity::AllocatedCount(*this)));
}

// Effect templates are an actual array of 0x18-byte records. The native
// unsigned effect ID supplies its original scaled index at every read;
// no extra range check or cached template snapshot is introduced.
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

    effect->vmCount = g_EffectTemplates[static_cast<unsigned int>(effectId)].vmCount;
    effect->manager = this;
    if (effect->vmCount != 0)
    {
        effect->vms = static_cast<AnmVm *>(
            g_ZunMemory.Alloc(
                sizeof(AnmVm) * g_EffectTemplates[static_cast<unsigned int>(effectId)].vmCount,
                "./system\\global.h"));
        // Script-template VM counts select real allocated 0x2A4-byte
        // ANM VMs by their i386 DWORD ordinal, preserving every distinct
        // live call and publish/read in the original loop.
        int scriptIndex = g_EffectTemplates[static_cast<unsigned int>(effectId)].scriptIndex;
        for (int i = 0; i < g_EffectTemplates[static_cast<unsigned int>(effectId)].vmCount; ++i)
        {
            this->effectAnm->SetAndExecuteScriptIdx(
                    &effect->vms[i], scriptIndex);
            scriptIndex = static_cast<int>(static_cast<unsigned int>(scriptIndex) +
                static_cast<unsigned int>(g_EffectTemplates[static_cast<unsigned int>(effectId)].scriptStep));
            effect->vms[i].flagsWord |= 0x2000;
            effect->vms[i].color1 = color;
            effect->vms[i].pos2.x = 0.0f;
            effect->vms[i].pos2.y = 0.0f;
            effect->vms[i].pos2.z = 0.0f;
        }
    }
    effect->updateCallback = g_EffectTemplates[static_cast<unsigned int>(effectId)].updateCallback;
    effect->drawCallback = g_EffectTemplates[static_cast<unsigned int>(effectId)].drawCallback;
    if (g_EffectTemplates[static_cast<unsigned int>(effectId)].initializeCallback != NULL &&
        g_EffectTemplates[static_cast<unsigned int>(effectId)].initializeCallback(effect) != 0)
        effect->active = 0;
    return effect;
}

Effect *EffectManager::SpawnEffect(
    int effectId, const EffectFloat3 *position, int count, unsigned int color)
{
    int effectIndex = this->nextEffectIndex;
    // Native Win32 pool address = allocation base + 0xD8-byte record stride.
    Effect *effect = &this->effects[effectIndex];
    Float3 velocity(0.0f, 0.0f, 0.0f);
    int i;

    for (i = 0; i < this->primaryCount; ++i)
    {
        effectIndex = static_cast<int>(static_cast<unsigned int>(effectIndex) + 1u);
        this->nextEffectIndex = effectIndex;
        if (effectIndex >= this->primaryCount)
            this->nextEffectIndex = 0;

        if (effect->active == 0)
        {
            this->InitializeEffect(
                effect, effectId, position, color,
                reinterpret_cast<const EffectFloat3 *>(&velocity));
            count = static_cast<int>(static_cast<unsigned int>(count) - 1u);
            if (count == 0)
                break;
        }

        effectIndex = this->nextEffectIndex;
        if (effectIndex == 0)
            effect = this->effects;
        else
            ++effect;
    }

    return i >= this->primaryCount
        ? EffectNativePoolCapacity::FailureSentinel(*this)
        : effect;
}

Effect *EffectManager::SpawnEffectWithVelocity(
    int effectId, const EffectFloat3 *position, const EffectFloat3 *velocity,
    int count, unsigned int color)
{
    Effect *effect = &this->effects[this->nextEffectIndex];
    int i;

    for (i = 0; i < this->primaryCount; ++i)
    {
        this->nextEffectIndex = static_cast<int>(
            static_cast<unsigned int>(this->nextEffectIndex) + 1u);
        if (this->nextEffectIndex >= this->primaryCount)
            this->nextEffectIndex = 0;

        if (effect->active == 0)
        {
            this->InitializeEffect(effect, effectId, position, color, velocity);
            count = static_cast<int>(static_cast<unsigned int>(count) - 1u);
            if (count == 0)
                break;
        }

        if (this->nextEffectIndex == 0)
            effect = this->effects;
        else
            ++effect;
    }

    return i >= this->primaryCount
        ? EffectNativePoolCapacity::FailureSentinel(*this)
        : effect;
}

Effect *EffectManager::SpawnEffectInFixedSlot(
    int effectId, const EffectFloat3 *position, int slotIndex, unsigned int color)
{
    // The fixed-slot index sum and address scale also wrap as native DWORDs.
    Effect *effect = &this->effects[
        static_cast<unsigned int>(this->primaryCount) +
        static_cast<unsigned int>(slotIndex)];
    Float3 velocity(0.0f, 0.0f, 0.0f);
    this->InitializeEffect(
        effect, effectId, position, color,
        reinterpret_cast<const EffectFloat3 *>(&velocity));
    effect->fixedSlotIndex = slotIndex;
    return effect;
}

int EffectManager::OnUpdate(EffectManager *effectManager)
{
    Effect *effect = effectManager->effects;
    if ((g_GameManager.flags & 0x1800) != 0 || (g_GameManager.sides[0].flags & 1) != 0)
        return CHAIN_CALLBACK_RESULT_CONTINUE;

    g_Supervisor.SelectSide(effectManager->sideIndex);
    effectManager->activeCount = 0;
    effectManager->drawTail0 = &effectManager->drawSentinel0;
    effectManager->drawTail1 = &effectManager->drawSentinel1;
    effectManager->drawTail2 = &effectManager->drawSentinel2;
    effectManager->drawSentinel0.nextInDrawGroup = NULL;
    effectManager->drawSentinel1.nextInDrawGroup = NULL;
    effectManager->drawSentinel2.nextInDrawGroup = NULL;

    // Read the live primary bound before the secondary one on each iteration.
    // This is a volatile-qualified access to an ordinary field, not a change
    // to the EffectManager layout or the other pool consumers.
    for (int i = 0;
         i < EffectNativePoolCapacity::ActiveCountWithNativeOrder(*effectManager);
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

        effectManager->activeCount = static_cast<int>(
            static_cast<unsigned int>(effectManager->activeCount) + 1u);
        if (effect->updateCallback != NULL && effect->updateCallback(effect) != 1)
        {
            effect->active = 0;
            continue;
        }

        // Walk the actual runtime-counted 0x2A4-byte VM array. Preserve the
        // live count and independently observed script outcomes.
        int liveVmCount = effect->vmCount;
        for (int vmIndex = 0; vmIndex < effect->vmCount; ++vmIndex)
        {
            if (g_AnmManager->ExecuteScript(
                    &effect->vms[vmIndex]) != 0)
                liveVmCount = static_cast<int>(
                    static_cast<unsigned int>(liveVmCount) - 1u);
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
    effectManager->updateCounter = static_cast<int>(
        static_cast<unsigned int>(effectManager->updateCounter) + 1u);
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
            AnmVm *vm = effect->vms;
            if (effect->vmCount > 0)
            {
                int vmCount = effect->vmCount;
                do
                {
                    vm->pos.x = g_GameManager.TransformPopupX(effect->position.x);
                    vm->pos.y = g_GameManager.TransformPopupY(effect->position.y);
                    vm->pos.z = 0.08f;
                    vm->pos += vm->pos2;
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
            AnmVm *vm = effect->vms;
            if (vmCount > 0)
            {
                do
                {
                    vm->pos.x = effect->position.x;
                    vm->pos.y = effect->position.y;
                    vm->pos.z = 0.0f;
                    vm->pos += vm->pos2;
                    g_AnmManager->Draw2D(vm);
                    ++vm;
                } while (--vmCount != 0);
            }
        }
    }
    return CHAIN_CALLBACK_RESULT_CONTINUE;
}

int EffectManager::ReleasePool()
{
    Effect *effect = this->effects;
    for (int i = 0; i < EffectNativePoolCapacity::AllocatedCount(*this); ++i, ++effect)
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
    // Only the two playable records belong to the native side array.
    // The physical third record is a separate shared-effect setup slot.
    if (sideIndex < GameplaySetupNativeSides::PlayableCount)
    {
        // Retail forms both native side addresses from the GameManager base
        // using a 0x38-byte stride, with modulo-32 arithmetic. Do not form
        // 1 - sideIndex as a potentially overflowing signed C++ expression.
        effectManager->sideState = reinterpret_cast<AsciiGameManagerSideView *>(
            reinterpret_cast<unsigned int>(&g_GameManager.sides[0]) +
            static_cast<unsigned int>(sideIndex) * sizeof(AsciiGameManagerSideView));
        effectManager->opposingSideState = reinterpret_cast<AsciiGameManagerSideView *>(
            reinterpret_cast<unsigned int>(&g_GameManager.sides[1]) -
            static_cast<unsigned int>(sideIndex) * sizeof(AsciiGameManagerSideView));
    }
    else
    {
        // Address setup's shared record as bytes: subscripting a third
        // element in g_GameManager.sides[2] would leave its true bounds.
        effectManager->sideState = reinterpret_cast<unsigned char *>(&g_GameManager) +
            sideIndex * sizeof(AsciiGameManagerSideView);
        effectManager->opposingSideState = effectManager->sideState;
    }
    effectManager->primaryCount = primaryCount;
    effectManager->secondaryCount = secondaryCount;

    int count = EffectNativePoolCapacity::AllocatedFromCounts(primaryCount, secondaryCount);
    unsigned int bytes = sizeof(Effect) * count;
    effectManager->effects = static_cast<Effect *>(g_ZunMemory.Alloc(bytes, "./system\\global.h"));
    memset(effectManager->effects, 0, bytes);

    effectManager->calcChain = g_Chain.CreateElem(reinterpret_cast<ChainCallback>(EffectManager::OnUpdate));
    effectManager->calcChain->arg = effectManager;
    effectManager->calcChain->addedCallback = EffectManager::AddedCallback;
    if (g_Chain.AddToCalcChain(
            effectManager->calcChain,
            static_cast<int>(static_cast<unsigned int>(sideIndex) + 19u)) != 0)
        return NULL;

    if (sideIndex < GameplaySetupNativeSides::PlayableCount)
    {
        effectManager->drawChain = g_Chain.CreateElem(reinterpret_cast<ChainCallback>(EffectManager::OnDraw));
        effectManager->drawChain->arg = effectManager;
        g_Chain.AddToDrawChain(
            effectManager->drawChain,
            static_cast<int>(static_cast<unsigned int>(sideIndex) + 19u));
    }
    else
    {
        effectManager->drawChain = g_Chain.CreateElem(reinterpret_cast<ChainCallback>(EffectManager::OnDrawShared));
        effectManager->drawChain->arg = effectManager;
        g_Chain.AddToDrawChain(effectManager->drawChain, 30);
    }
    return effectManager;
}
