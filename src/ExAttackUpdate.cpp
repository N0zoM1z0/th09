#include "ExAttackController.hpp"
#include "AsciiManager.hpp"
#include "AsciiGameManagerView.hpp"
#include "AnmManager.hpp"
#include "ZunMemory.hpp"

#include <stddef.h>

typedef int (__fastcall *ExAttackUpdateCallback)(void *record);

struct ExAttackUpdateRecordView
{
    int drawListIndex00;
    int opponentSide04;
    int side08;
    int active0C;
    ZunTimer timer10;
    void *dynamicData1C;
    ExAttackVec3 position20;
    int vmCount2C;
    ExAttackUpdateRecordView *next30;
    int unknown34;
    int parameter38;
    float rotation3C;
    ExAttackUpdateCallback updateCallback40;
    ExAttackUpdateCallback drawCallback44;
    ExAttackUpdateCallback releaseCallback48;
};

typedef char ExAttackUpdateRecordSizeIs4C[
    (sizeof(ExAttackUpdateRecordView) == 0x4C) ? 1 : -1];

struct ExAttackUpdateControllerView
{
    unsigned char unknown0000[0x0C];
    int activeCounts0C[2];
    unsigned char unknown0014[0x08];
    ExAttackUpdateRecordView records1C[257];
    ExAttackUpdateRecordView *heads4C68[3];
    ExAttackUpdateRecordView *tails4C74[3];
};

typedef char ExAttackUpdateControllerSizeIs4C80[
    (sizeof(ExAttackUpdateControllerView) == 0x4C80) ? 1 : -1];

extern ZunMemory g_ZunMemory;

int ExAttackController::OnUpdate(ExAttackController *controller)
{
    ExAttackUpdateControllerView *view =
        reinterpret_cast<ExAttackUpdateControllerView *>(controller);
    ExAttackUpdateRecordView *record = view->records1C;

    if ((g_GameManager.flags & 0x1800u) == 0 &&
        (g_GameManager.sides[0].flags & 1u) == 0)
    {
        view->tails4C74[0] = 0;
        view->tails4C74[1] = 0;
        view->tails4C74[2] = 0;
        view->heads4C68[0] = 0;
        view->heads4C68[1] = 0;
        view->heads4C68[2] = 0;
        view->activeCounts0C[1] = 0;
        view->activeCounts0C[0] = 0;

        int remaining = 256;
        do
        {
            if (record->active0C)
            {
                if (record->updateCallback40(record))
                {
                    if (record->releaseCallback48 != 0)
                        record->releaseCallback48(record);

                    if (record->dynamicData1C != 0)
                    {
                        g_ZunMemory.Free(record->dynamicData1C);
                        record->dynamicData1C = 0;
                    }
                    record->active0C = 0;
                }
                else
                {
                    int vmIndex = 0;
                    if (record->vmCount2C > 0)
                    {
                        int vmOffset = 0;
                        do
                        {
                            g_AnmManager->ExecuteScript(
                                reinterpret_cast<AnmVm *>(
                                    reinterpret_cast<unsigned char *>(
                                        record->dynamicData1C) + vmOffset));
                            ++vmIndex;
                            vmOffset += sizeof(AnmVm);
                        }
                        while (vmIndex < record->vmCount2C);
                    }

                    if (view->tails4C74[record->drawListIndex00] != 0)
                    {
                        view->tails4C74[record->drawListIndex00]->next30 =
                            record;
                    }
                    else
                    {
                        view->heads4C68[record->drawListIndex00] = record;
                    }

                    ++view->activeCounts0C[record->side08];
                    view->tails4C74[record->drawListIndex00] = record;
                    record->next30 = 0;
                    record->timer10++;
                }
            }

            ++record;
            --remaining;
        }
        while (remaining != 0);
    }

    return 1;
}
