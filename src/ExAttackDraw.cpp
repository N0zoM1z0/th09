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

// Real ExAttack VM storage is a native 0x2A4-byte record sequence,
// indexed by the current signed draw word without a C++ bounds gate.
#define TH09_EXATTACK_DRAW_VM(rec, index) \
    (*reinterpret_cast<AnmVm *>( \
        reinterpret_cast<unsigned char *>((rec)->vms1C) + \
        static_cast<unsigned int>(index) * sizeof(AnmVm)))

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
                    if (TH09_EXATTACK_DRAW_VM(record, vmIndex).type != 0)
                    {
                        TH09_EXATTACK_DRAW_VM(record, vmIndex).SetZRotation(record->rotation3C);
                    }

                    TH09_EXATTACK_DRAW_VM(record, vmIndex).pos.x =
                        g_GameManager.TransformPopupX(
                            TH09_EXATTACK_DRAW_VM(record, vmIndex).pos2.x +
                            record->position20.x);
                    TH09_EXATTACK_DRAW_VM(record, vmIndex).pos.y =
                        g_GameManager.TransformPopupY(
                            TH09_EXATTACK_DRAW_VM(record, vmIndex).pos2.y +
                            record->position20.y);
                    TH09_EXATTACK_DRAW_VM(record, vmIndex).pos.z = 0.06f;
                    g_AnmManager->Draw2D(
                        &TH09_EXATTACK_DRAW_VM(record, vmIndex));

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
                if (TH09_EXATTACK_DRAW_VM(record, vmIndex).type != 0)
                {
                    TH09_EXATTACK_DRAW_VM(record, vmIndex).SetZRotation(record->rotation3C);
                }

                TH09_EXATTACK_DRAW_VM(record, vmIndex).pos.x =
                    TH09_EXATTACK_DRAW_VM(record, vmIndex).pos2.x +
                    record->position20.x;
                TH09_EXATTACK_DRAW_VM(record, vmIndex).pos.y =
                    TH09_EXATTACK_DRAW_VM(record, vmIndex).pos2.y +
                    record->position20.y;
                TH09_EXATTACK_DRAW_VM(record, vmIndex).pos.z = 0.06f;
                g_AnmManager->Draw2D(
                    &TH09_EXATTACK_DRAW_VM(record, vmIndex));

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

#undef TH09_EXATTACK_DRAW_VM
