#include "AnmManager.hpp"
#include "AsciiGameManagerView.hpp"
#include "AsciiManager.hpp"
#include "Supervisor.hpp"

void AsciiManager::ResetStrings()
{
    this->numStrings = 0;
}

void AsciiManager::SetScale(float scaleX, float scaleY)
{
    this->scaleX = scaleX;
    this->scaleY = scaleY;
}

void PauseMenu::OnDraw()
{
    unsigned int i;

    if (!g_GameManager.inGameMenu) {
        return;
    }

    g_AnmManager->FlushVertexBuffer();
    g_Supervisor.ConfigureGameplayViewport(2);
    for (i = 0; i < 7; i++) {
        if (this->menuSprites[i].IsVisible()) {
            g_AnmManager->DrawNoRotation(&this->menuSprites[i]);
        }
    }
}
