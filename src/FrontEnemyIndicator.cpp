#include "EnemyManager.hpp"
#include "Supervisor.hpp"

#include <stddef.h>

struct FrontEnemyIndicatorColorView
{
    unsigned char blue;
    unsigned char green;
    unsigned char red;
    unsigned char alpha;
};

struct FrontEnemyIndicatorVmView
{
    unsigned char unknown000[0x1F0];
    FrontEnemyIndicatorColorView color1F0;
    unsigned char unknown1F4[0x208 - 0x1F4];
    EnemyFloat3 position208;
    unsigned char unknown214[0x2A4 - 0x214];
};
typedef char FrontEnemyIndicatorVmSize2A4[
    (sizeof(FrontEnemyIndicatorVmView) == 0x2A4) ? 1 : -1];
typedef char FrontEnemyIndicatorVmColorAt1F0[
    (offsetof(FrontEnemyIndicatorVmView, color1F0) == 0x1F0) ? 1 : -1];
typedef char FrontEnemyIndicatorVmPositionAt208[
    (offsetof(FrontEnemyIndicatorVmView, position208) == 0x208) ? 1 : -1];

struct FrontEnemyIndicatorView
{
    unsigned char unknown00000[0x123F4];
    FrontEnemyIndicatorVmView enemyIndicatorVms123F4[2];

    void SetSideEnemyIndicatorPosition(
        int sideIndex, const EnemyFloat3 *enemyPosition);
};

struct FrontEnemyIndicatorPlayerView
{
    unsigned char unknown0000[0x1B88];
    EnemyFloat3 position1B88;
};

struct FrontEnemyIndicatorGameSideView
{
    unsigned char unknown00[0x04];
    FrontEnemyIndicatorPlayerView *player04;
    unsigned char unknown08[0x38 - 0x08];
};
typedef char FrontEnemyIndicatorSideSize38[
    (sizeof(FrontEnemyIndicatorGameSideView) == 0x38) ? 1 : -1];

struct FrontEnemyIndicatorGameManagerView
{
    FrontEnemyIndicatorGameSideView sides[2];
    float TransformPopupX(float value);
};

extern FrontEnemyIndicatorGameManagerView g_GameManager;
extern Supervisor g_Supervisor;

void FrontEnemyIndicatorView::SetSideEnemyIndicatorPosition(
    int sideIndex, const EnemyFloat3 *enemyPosition)
{
    g_Supervisor.SelectSide(sideIndex);

    if (enemyPosition->x < -112.0f || enemyPosition->x > 112.0f)
    {
        this->enemyIndicatorVms123F4[sideIndex].color1F0.alpha = 0;
        return;
    }

    float distance =
        g_GameManager.sides[sideIndex].player04->position1B88.x -
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
