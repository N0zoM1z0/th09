#include "CardAttack.hpp"

#include "AnmManager.hpp"
#include "Background.hpp"
#include "ZunMemory.hpp"

#include <stdlib.h>
#include <string.h>

struct CardAttackGameManagerView
{
    CardAttackSideStateView sides[2];

    float TransformPopupX(float value);
    float TransformPopupY(float value);
};

struct CardAttackSupervisorView
{
    // Reconstruction alias for the target helper at 0x00403D90.
    void PreparePlayerDraw(int sideIndex);
};

struct CardAttackSoundPlayerView
{
    void PlaySoundByIdx(int soundIndex, int pan);
};

struct CardAttackResourceView
{
    unsigned char unknown00[8];
    AnmLoaded *anm08;
};

extern Chain g_Chain;
extern CardAttackGameManagerView g_GameManager;
extern CardAttackSupervisorView g_Supervisor;
extern CardAttackSoundPlayerView g_SoundPlayer;
extern CardAttackResourceView *g_CardAttackResource;
extern AnmLoaded *g_CardAttackTextAnm;
extern unsigned int g_CardAttackInputFlags;

static __forceinline CardAttackVmView *CardAttackVm(AnmVm *vm)
{
    return reinterpret_cast<CardAttackVmView *>(vm);
}

CardAttack::CardAttack()
{
}

void CardAttack::RefreshResource()
{
    this->presentationAnm14 = g_CardAttackResource->anm08;
}

static __forceinline void PrepareCardAttackPresentation(CardAttack *cardAttack)
{
    AnmVm *vm = &cardAttack->mainVms0AC[1];
    CardAttackVmView *view = CardAttackVm(vm);

    cardAttack->presentationAnm14->SetAndExecuteScriptIdx(
        &cardAttack->mainVms0AC[0], cardAttack->sideIndex00 + 18);
    g_CardAttackTextAnm->SetSprite(vm, cardAttack->sideIndex00 + 4);
    view->scale18.x = 1.0f;
    view->scale18.y = 1.0f;
    g_AnmManager->DrawTextCentered(
        vm, 0x00FFFFFE, 0, cardAttack->text18);
}

int CardAttack::OnUpdate(CardAttack *cardAttack)
{
    if (cardAttack->primaryPresentationCounterA4 >= 2)
    {
        if (cardAttack->secondaryUpdate1334 == NULL &&
            cardAttack->primaryUpdate1328 != NULL)
        {
            PrepareCardAttackPresentation(cardAttack);
        }
        cardAttack->primaryPresentationCounterA4 = 0;
    }

    if (cardAttack->secondaryPresentationCounterA8 >= 2)
    {
        if (cardAttack->secondaryUpdate1334 != NULL)
            PrepareCardAttackPresentation(cardAttack);
        cardAttack->secondaryPresentationCounterA8 = 0;
    }

    if ((g_CardAttackInputFlags & 0x1800) != 0 && cardAttack->timer98 > 30)
        g_CardAttackInputFlags &= ~(0x800U << cardAttack->sideIndex00);

    if (cardAttack->primaryUpdate1328 != NULL &&
        cardAttack->primaryUpdate1328(cardAttack) != 0)
    {
        if (cardAttack->secondaryUpdate1334 == NULL)
            CardAttackVm(&cardAttack->mainVms0AC[0])->flags1F8 &= ~2U;
        cardAttack->primaryEnd1330(cardAttack);
        cardAttack->primaryUpdate1328 = NULL;
        cardAttack->primaryDraw132C = NULL;
        cardAttack->primaryEnd1330 = NULL;
    }

    if (cardAttack->secondaryUpdate1334 != NULL &&
        cardAttack->secondaryUpdate1334(cardAttack) != 0)
    {
        CardAttackVm(&cardAttack->mainVms0AC[0])->flags1F8 &= ~2U;
        cardAttack->secondaryEnd133C(cardAttack);
        cardAttack->secondaryUpdate1334 = NULL;
        cardAttack->secondaryDraw1338 = NULL;
        cardAttack->secondaryEnd133C = NULL;
        g_SoundPlayer.PlaySoundByIdx(15, 0);
        cardAttack->sideState04->background00->ClearSpellBackgroundState();
    }

    if (g_AnmManager->ExecuteScript(&cardAttack->mainVms0AC[0]) != 0)
        CardAttackVm(&cardAttack->mainVms0AC[0])->flags1F8 &= ~2U;

    for (int i = 0; i < 5; ++i)
    {
        if (g_AnmManager->ExecuteScript(&cardAttack->secondaryVms5F4[i]) != 0)
            CardAttackVm(&cardAttack->secondaryVms5F4[i])->flags1F8 &= ~2U;
    }

    cardAttack->timer98++;
    return 1;
}

int CardAttack::OnDraw(CardAttack *cardAttack)
{
    g_Supervisor.PreparePlayerDraw(cardAttack->sideIndex00);

    if (cardAttack->primaryDraw132C != NULL)
        cardAttack->primaryDraw132C(cardAttack);
    if (cardAttack->secondaryDraw1338 != NULL)
        cardAttack->secondaryDraw1338(cardAttack);

    CardAttackVmView *main = CardAttackVm(&cardAttack->mainVms0AC[0]);
    if ((main->flags1F8 & 2) != 0)
    {
        main->position208.x = g_GameManager.TransformPopupX(main->offset288.x);
        main->position208.y = g_GameManager.TransformPopupY(main->offset288.y);
        main->position208.z = 0.0f;
        g_AnmManager->DrawNoRotation(&cardAttack->mainVms0AC[0]);

        CardAttackVmView *copy = CardAttackVm(&cardAttack->mainVms0AC[1]);
        copy->position208 = main->position208;
        copy->position208.y += 10.0f;
        copy->color1F0 = main->color1F0;
        copy->flags1F8 = main->flags1F8;
        g_AnmManager->DrawNoRotation(&cardAttack->mainVms0AC[1]);
    }

    AnmVm *secondaryVm = &cardAttack->secondaryVms5F4[0];
    for (int i = 0; i < 5; ++i, ++secondaryVm)
    {
        CardAttackVmView *vm = CardAttackVm(secondaryVm);
        if ((vm->flags1F8 & 2) == 0)
            continue;
        vm->position208.x = g_GameManager.TransformPopupX(vm->offset288.x);
        vm->position208.y = g_GameManager.TransformPopupY(vm->offset288.y);
        vm->position208.z = 0.0f;
        g_AnmManager->DrawNoRotation(secondaryVm);
    }
    return 1;
}

int CardAttack::AddedCallback(CardAttack *cardAttack)
{
    cardAttack->RefreshResource();
    return 0;
}

static int RunEndCallbacks(CardAttack *cardAttack)
{
    if (cardAttack->primaryEnd1330 != NULL)
        cardAttack->primaryEnd1330(cardAttack);
    if (cardAttack->secondaryEnd133C != NULL)
        cardAttack->secondaryEnd133C(cardAttack);
    return 0;
}

void CardAttack::Destroy(CardAttack *cardAttack)
{
    if (cardAttack == NULL)
        return;

    RunEndCallbacks(cardAttack);
    g_Chain.Cut(cardAttack->calcChain0C);
    g_Chain.Cut(cardAttack->drawChain10);
    free(cardAttack);
}

CardAttack *CardAttack::Create(int sideIndex)
{
    CardAttack *cardAttack = static_cast<CardAttack *>(
        g_ZunMemory.AddToRegistry(new CardAttack, sizeof(CardAttack), "CardAttackInf"));
    memset(cardAttack, 0, sizeof(CardAttack));

    cardAttack->sideState04 = &g_GameManager.sides[sideIndex];
    cardAttack->sideIndex00 = sideIndex;
    cardAttack->otherSideState08 = &g_GameManager.sides[1 - sideIndex];

    cardAttack->calcChain0C =
        g_Chain.CreateElem(reinterpret_cast<ChainCallback>(CardAttack::OnUpdate));
    cardAttack->calcChain0C->addedCallback =
        reinterpret_cast<ChainLifetimeCallback>(CardAttack::AddedCallback);
    cardAttack->calcChain0C->arg = cardAttack;
    if (g_Chain.AddToCalcChain(cardAttack->calcChain0C, sideIndex + 17) != 0)
        return NULL;

    cardAttack->drawChain10 =
        g_Chain.CreateElem(reinterpret_cast<ChainCallback>(CardAttack::OnDraw));
    cardAttack->drawChain10->arg = cardAttack;
    g_Chain.AddToDrawChain(cardAttack->drawChain10, sideIndex + 24);
    return cardAttack;
}
