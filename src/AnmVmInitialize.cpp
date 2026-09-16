#include "AnmManager.hpp"
#include "AnmMatrixRuntime.hpp"
#include "AsciiManager.hpp"

#include <stddef.h>
#include <string.h>

struct AnmVmInitializeView
{
    unsigned char unknown000[0x18];
    float scaleX18;
    float scaleY1C;
    unsigned char unknown020[0x38 - 0x20];
    ZunTimer currentTimeInScript38;
    unsigned char unknown044[0x130 - 0x44];
    AnmMatrixRuntime matrix130;
    unsigned char unknown170[0x1F0 - 0x170];
    unsigned long color1F0;
    unsigned long color2F4;
    unsigned short flags1F8;
    unsigned char unknown1FA[0x208 - 0x1FA];
};

typedef char AnmVmInitializeViewSizeIs208[
    (sizeof(AnmVmInitializeView) == 0x208) ? 1 : -1];
typedef char AnmVmInitializeTimerAt38[
    (offsetof(AnmVmInitializeView, currentTimeInScript38) == 0x38) ? 1 : -1];
typedef char AnmVmInitializeMatrixAt130[
    (offsetof(AnmVmInitializeView, matrix130) == 0x130) ? 1 : -1];
typedef char AnmVmInitializeColorAt1F0[
    (offsetof(AnmVmInitializeView, color1F0) == 0x1F0) ? 1 : -1];
typedef char AnmVmInitializeFlagsAt1F8[
    (offsetof(AnmVmInitializeView, flags1F8) == 0x1F8) ? 1 : -1];

void AnmVm::Initialize()
{
    AnmVmInitializeView *view =
        reinterpret_cast<AnmVmInitializeView *>(this);

    memset(view, 0, sizeof(*view));
    view->scaleX18 = 1.0f;
    view->scaleY1C = 1.0f;
    view->color1F0 = 0xffffffff;
    view->matrix130.SetIdentity();
    view->flags1F8 = 7;
    view->currentTimeInScript38.Initialize();
}

void AnmLoaded::InitializeAndSetSprite(AnmVm *vm, int spriteIndex)
{
    vm->Initialize();
    vm->anmFile = this;
    this->SetSprite(vm, spriteIndex);
}
