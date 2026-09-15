#include "ExAttackController.hpp"
#include "ZunMemory.hpp"

#include <new>
#include <string.h>

extern Chain g_Chain;
extern ZunMemory g_ZunMemory;

struct ExAttackSoundPlayerView
{
    void PlaySoundByIdx(int soundIndex, int pan);
};
extern ExAttackSoundPlayerView g_SoundPlayer;

extern ExAttackTemplate g_ExAttackTemplates[];

ExAttackRecord::ExAttackRecord()
{
}

ExAttackController::ExAttackController()
{
}

static int ReleaseExAttackRecords(ExAttackController *controller)
{
    ExAttackRecord *record = controller->records;
    for (int i = 0; i < 256; ++i, ++record)
    {
        if (record->active)
        {
            if (record->releaseCallback != NULL)
                record->releaseCallback(record);
            if (record->dynamicData != NULL)
            {
                g_ZunMemory.Free(record->dynamicData);
                record->dynamicData = NULL;
            }
        }
    }
    return 0;
}

void ExAttackController::Release(ExAttackController *controller)
{
    if (controller == NULL)
        return;
    ReleaseExAttackRecords(controller);
    g_Chain.Cut(controller->drawChain1);
    g_Chain.Cut(controller->drawChain0);
    g_Chain.Cut(controller->calcChain);
    delete controller;
}

ExAttackRecord *ExAttackController::Spawn(
    int type, const Float3 *position, int side, int parameter)
{
    ExAttackRecord *record = this->records;
    int activeForSide = 0;
    int i;
    for (i = 0; i < 256; ++i, ++record)
    {
        if (record->active && record->side == side)
            activeForSide++;
    }
    if (activeForSide >= this->sideLimits[side])
        return &this->records[256];

    g_SoundPlayer.PlaySoundByIdx(45, side != 0 ? 500 : -500);
    record = this->records;
    i = 0;
    while (record->active)
    {
        i++;
        record++;
        if (i >= 256)
            return record;
    }

    memset(record, 0, sizeof(ExAttackRecord));
    record->active = 1;
    record->position = *reinterpret_cast<const ExAttackVec3 *>(position);
    record->parameter38 = parameter;
    record->side = side;
    record->opponentSide = 1 - side;
    record->updateCallback = g_ExAttackTemplates[type].updateCallback;
    record->drawCallback = g_ExAttackTemplates[type].drawCallback;
    record->releaseCallback = g_ExAttackTemplates[type].releaseCallback;
    record->timer = 0;
    if (g_ExAttackTemplates[type].initializeCallback(record) != 0)
        record->active = 0;
    return record;
}

ExAttackController *ExAttackController::Register()
{
    ExAttackController *controller = new ExAttackController;
    controller = static_cast<ExAttackController *>(g_ZunMemory.AddToRegistry(
        controller, sizeof(ExAttackController), const_cast<char *>("ExAttackCtrlInf")));
    memset(controller, 0, sizeof(ExAttackController));

    controller->calcChain = g_Chain.CreateElem(
        reinterpret_cast<ChainCallback>(ExAttackController::OnUpdate));
    controller->calcChain->arg = controller;
    controller->calcChain->addedCallback =
        reinterpret_cast<ChainLifetimeCallback>(ExAttackController::AddedCallback);
    if (g_Chain.AddToCalcChain(controller->calcChain, 14) != 0)
        return NULL;

    controller->drawChain0 = g_Chain.CreateElem(
        reinterpret_cast<ChainCallback>(ExAttackController::OnDraw0));
    controller->drawChain0->arg = controller;
    g_Chain.AddToDrawChain(controller->drawChain0, 16);

    controller->drawChain1 = g_Chain.CreateElem(
        reinterpret_cast<ChainCallback>(ExAttackController::OnDraw1));
    controller->drawChain1->arg = controller;
    g_Chain.AddToDrawChain(controller->drawChain1, 29);
    return controller;
}
