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

int Background::UpdateStageObjectVms()
{
    for (int objectIndex = 0;
         objectIndex < this->stageObjectCount;
         ++objectIndex)
    {
        RawStageObject *object = this->stageObjects[objectIndex];
        if ((object->flags & 1) != 0)
        {
            RawStageQuadBasic *quad = &object->firstQuad;
            int activeScriptCount = 0;
            if (quad->type >= 0)
            {
                do
                {
                    AnmVm *vm =
                        &this->stageObjectVms[quad->vmIndex];
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
                        ++activeScriptCount;
                    }

                    quad = reinterpret_cast<RawStageQuadBasic *>(
                        reinterpret_cast<unsigned char *>(quad) +
                        quad->byteSize);
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
