#include "FrontEnemyIndicatorView.hpp"
#include "AsciiGameManagerView.hpp"
#include "Supervisor.hpp"

#include <stddef.h>

extern AsciiGameManagerView g_GameManager;
extern Supervisor g_Supervisor;

void FrontEnemyIndicatorView::SetSideEnemyIndicatorPosition(
    int sideIndex, const Float3 *enemyPosition)
{
    g_Supervisor.SelectSide(sideIndex);

    if (enemyPosition->x < -112.0f || enemyPosition->x > 112.0f)
    {
        this->enemyIndicatorVms123F4[sideIndex].color1F0.alpha = 0;
        return;
    }

    float distance =
        g_GameManager.sides[sideIndex].player->position.x -
        enemyPosition->x;
    if (distance < 0.0f)
        distance = -distance;

    if (distance < 64.0f)
        this->enemyIndicatorVms123F4[sideIndex].color1F0.alpha =
            static_cast<unsigned char>(distance) * 3 + 64;
    else
        this->enemyIndicatorVms123F4[sideIndex].color1F0.alpha = 0xFF;

    this->enemyIndicatorVms123F4[sideIndex].position208.x =
        g_GameManager.TransformPopupX(enemyPosition->x);
    this->enemyIndicatorVms123F4[sideIndex].position208.y = 472.0f;
    this->enemyIndicatorVms123F4[sideIndex].position208.z = 0.0f;
}
