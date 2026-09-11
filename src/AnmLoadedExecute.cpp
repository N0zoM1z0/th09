#include "AnmManager.hpp"
#include "AsciiManager.hpp"

struct AnmLoadedExecuteLayout
{
    unsigned char unknown_0000[0x10];
    AnmRawInstr **scripts;
};

struct AnmVmExecuteLayout
{
    unsigned char unknown_0000[0x208];
    Float3 position;
    unsigned char unknown_0214[6];
    short scriptIndex;
    unsigned char unknown_021C[0x6C];
    Float3 secondaryPosition;
    unsigned char unknown_0294[4];
    unsigned char fontWidth;
    unsigned char fontHeight;
};

void AnmLoaded::ExecuteAnmIdx(AnmVm *vm, int scriptIndex)
{
    AnmVmExecuteLayout *layout = reinterpret_cast<AnmVmExecuteLayout *>(vm);
    AnmLoadedExecuteLayout *anm = reinterpret_cast<AnmLoadedExecuteLayout *>(this);

    layout->scriptIndex = (short)scriptIndex;
    layout->position = Float3(0.0f, 0.0f, 0.0f);
    layout->secondaryPosition = Float3(0.0f, 0.0f, 0.0f);
    layout->fontHeight = 15;
    layout->fontWidth = 15;
    this->SetAndExecuteScript(vm, anm->scripts[scriptIndex]);
}
