#include "ExAttackController.hpp"
#include "AsciiManager.hpp"
#include "AsciiGameManagerView.hpp"
#include "AnmManager.hpp"
#include "Supervisor.hpp"

#include <stddef.h>

typedef int (__fastcall *ExAttackDrawRecordCallback)(void *record);

struct ExAttackDrawRecordView
{
    int unknown00;
    int opponentSide04;
    int side08;
    int active0C;
    ZunTimer timer10;
    AnmVm *vms1C;
    ExAttackVec3 position20;
    int vmCount2C;
    ExAttackDrawRecordView *next30;
    int unknown34;
    int parameter38;
    float rotation3C;
    ExAttackDrawRecordCallback updateCallback40;
    ExAttackDrawRecordCallback drawCallback44;
    ExAttackDrawRecordCallback releaseCallback48;
};

typedef char ExAttackDrawRecordSizeIs4C[
    (sizeof(ExAttackDrawRecordView) == 0x4C) ? 1 : -1];
// ExAttack's draw callbacks iterate actual AnmVm[vmCount2C] records.
// The former raw +0x1FC probe is AnmVmBase::type in this same 0x2A4 VM.
typedef char ExAttackNativeVmTypeAndStride[
    (offsetof(ExAttackDrawRecordView, vms1C) == 0x1C &&
     offsetof(AnmVmBase, type) == 0x1FC &&
     sizeof(AnmVm) == 0x2A4) ? 1 : -1];

struct ExAttackDrawControllerView
{
    unsigned char unknown0000[0x4C68];
    ExAttackDrawRecordView *draw0Heads4C68[2];
    ExAttackDrawRecordView *draw1Head4C70;
    unsigned char unknown4C74[0x0C];
};

typedef char ExAttackDrawControllerSizeIs4C80[
    (sizeof(ExAttackDrawControllerView) == 0x4C80) ? 1 : -1];

int ExAttackController::OnDraw0(ExAttackController *controller)
{
    ExAttackDrawControllerView *view =
        reinterpret_cast<ExAttackDrawControllerView *>(controller);
    int side = 0;
    ExAttackDrawRecordView **head = view->draw0Heads4C68;

    do
    {
        g_Supervisor.ConfigureScreenViewport(side);

        ExAttackDrawRecordView *record = *head;
        while (record != 0)
        {
            int vmIndex = 0;
            if (record->vmCount2C > 0)
            {
                do
                {
                    if (record->vms1C[vmIndex].type != 0)
                    {
                        record->vms1C[vmIndex].SetZRotation(record->rotation3C);
                    }

                    record->vms1C[vmIndex].pos.x =
                        g_GameManager.TransformPopupX(
                            record->vms1C[vmIndex].pos2.x +
                            record->position20.x);
                    record->vms1C[vmIndex].pos.y =
                        g_GameManager.TransformPopupY(
                            record->vms1C[vmIndex].pos2.y +
                            record->position20.y);
                    record->vms1C[vmIndex].pos.z = 0.06f;
                    g_AnmManager->Draw2D(
                        &record->vms1C[vmIndex]);

                    ++vmIndex;
                }
                while (vmIndex < record->vmCount2C);
            }

            if (record->drawCallback44 != 0)
                record->drawCallback44(record);
            record = record->next30;
        }

        ++side;
        ++head;
    }
    while (side < 2);

    return 1;
}

int ExAttackController::OnDraw1(ExAttackController *controller)
{
    ExAttackDrawControllerView *view =
        reinterpret_cast<ExAttackDrawControllerView *>(controller);

    g_Supervisor.ConfigureScreenViewport(2);

    ExAttackDrawRecordView *record = view->draw1Head4C70;
    while (record != 0)
    {
        int vmIndex = 0;
        if (record->vmCount2C > 0)
        {
            do
            {
                if (record->vms1C[vmIndex].type != 0)
                {
                    record->vms1C[vmIndex].SetZRotation(record->rotation3C);
                }

                record->vms1C[vmIndex].pos.x =
                    record->vms1C[vmIndex].pos2.x +
                    record->position20.x;
                record->vms1C[vmIndex].pos.y =
                    record->vms1C[vmIndex].pos2.y +
                    record->position20.y;
                record->vms1C[vmIndex].pos.z = 0.06f;
                g_AnmManager->Draw2D(
                    &record->vms1C[vmIndex]);

                ++vmIndex;
            }
            while (vmIndex < record->vmCount2C);
        }

        if (record->drawCallback44 != 0)
            record->drawCallback44(record);
        record = record->next30;
    }

    return 1;
}
