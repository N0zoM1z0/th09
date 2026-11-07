// SPDX-License-Identifier: GPL-3.0-only
// Adapted from Cirno-Labs/th09 src/app.cpp, App::AddRecord,
// commit d7653152becabb8c86f2f43d49015ecc11bd0652.
// Original MIT portions retain their grant in LICENSE; see THIRD_PARTY_NOTICES.md.
#include "SupervisorFrameQueueStorageView.hpp"
#include "Supervisor.hpp"

void Supervisor::InsertReceivedFrame(
    int side, int frame, unsigned short packedInput, short seed)
{
    int i;
    int j;
    Supervisor *view = static_cast<Supervisor *>(this);
    view->lastReceived56C[side].frame04 = frame;
    view->lastReceived56C[side].packedInput00 = (short)packedInput;
    view->lastReceived56C[side].seed02 = seed;
    for (i = 0; i < 10; i++)
    {
        if (view->entries47C[side * 10 + i].frame04 == 0 ||
            view->entries47C[side * 10 + i].frame04 > frame)
            break;
    }
    if (view->entries47C[side * 10 + i].frame04 != 0 && i < 9)
    {
        for (j = 9; j > i; j--)
        {
            view->entries47C[side * 10 + j].packedInput00 = view->entries47C[side * 10 + j - 1].packedInput00;
            view->entries47C[side * 10 + j].seed02 = view->entries47C[side * 10 + j - 1].seed02;
            view->entries47C[side * 10 + j].frame04 = view->entries47C[side * 10 + j - 1].frame04;
            view->entries47C[side * 10 + j].value08 = view->entries47C[side * 10 + j - 1].value08;
        }
    }
    view->entries47C[side * 10 + i].packedInput00 = (short)packedInput;
    view->entries47C[side * 10 + i].frame04 = frame;
    view->entries47C[side * 10 + i].seed02 = seed;
    view->entries47C[side * 10 + i].value08 = 0;
}
