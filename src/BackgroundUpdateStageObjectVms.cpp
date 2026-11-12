#include "Background.hpp"
#include "AnmManager.hpp"

#include <stddef.h>

struct BackgroundAnmVmActivityView
{
    unsigned char unknown000[0x220];
    void *currentInstruction220;
};
typedef char BackgroundAnmVmActivityAt220[
    (offsetof(BackgroundAnmVmActivityView, currentInstruction220) == 0x220)
        ? 1 : -1];

// The stage owns a file-backed object pointer table and an independently
// allocated 0x2A4-byte animation VM array. Opcode quad widths are signed
// 16-bit byte strides and original counters wrap as i386 DWORD words.
int Background::UpdateStageObjectVms()
{
    for (int objectIndex = 0;
         objectIndex < this->stageObjectCount;
         ++*reinterpret_cast<unsigned int *>(&objectIndex))
    {
        RawStageObject *object = *reinterpret_cast<RawStageObject **>(
            reinterpret_cast<unsigned int>(&this->stageObjects[0]) +
            static_cast<unsigned int>(objectIndex) * sizeof(this->stageObjects[0]));
        if ((object->flags & 1) != 0)
        {
            RawStageQuadBasic *quad = &object->firstQuad;
            int activeScriptCount = 0;
            if (quad->type >= 0)
            {
                do
                {
                    AnmVm *vm =
                        reinterpret_cast<AnmVm *>(
                            reinterpret_cast<unsigned int>(&this->stageObjectVms[0]) +
                            static_cast<unsigned int>(quad->vmIndex) *
                                sizeof(this->stageObjectVms[0]));
                    switch (quad->type)
                    {
                    case 0:
                        g_AnmManager->ExecuteScript(vm);
                        break;
                    case 1:
                        g_AnmManager->ExecuteScript(vm);
                        break;
                    default:
                        break;
                    }

                    if (reinterpret_cast<BackgroundAnmVmActivityView *>(vm)
                            ->currentInstruction220 != 0)
                    {
                        ++*reinterpret_cast<unsigned int *>(&activeScriptCount);
                    }

                    quad = reinterpret_cast<RawStageQuadBasic *>(
                        reinterpret_cast<unsigned int>(quad) +
                        static_cast<unsigned int>(static_cast<int>(quad->byteSize)));
                }
                while (quad->type >= 0);
            }

            if (activeScriptCount != 0)
                continue;
            object->flags &= ~1u;
        }
    }
    return 0;
}
