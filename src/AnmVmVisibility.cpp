#include "AsciiManager.hpp"

int AnmVm::IsVisible()
{
    return this->flagsWord & 1;
}

void AnmVm::SetInvisible()
{
    this->flagsWord &= ~1u;
}
