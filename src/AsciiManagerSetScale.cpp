// The Title /O1 unit emits AsciiManager::SetScale at 0x004234DD.
#include "AnmManager.hpp"
#include "AsciiGameManagerView.hpp"
#include "AsciiManager.hpp"
#include "Supervisor.hpp"

void AsciiManager::SetScale(float scaleX, float scaleY)
{
    this->scaleX = scaleX;
    this->scaleY = scaleY;
}
