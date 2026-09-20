#include "PlayerLifecycleView.hpp"
#include "RngRuntimeLeaves.hpp"
#include "ZunTimer.hpp"

#include <stddef.h>

struct PlayerEnemyCallback30408EnemyView
{
    unsigned char unknown0000[0x2D98];
    float value2D98;
    float value2D9C;
    int value2DA0;
    unsigned char unknown2DA4[0x5424 - 0x2DA4];
    ZunTimer specialAttackTimer5424;
};

typedef char PlayerEnemy30408ValueAt2D98[
    (offsetof(PlayerEnemyCallback30408EnemyView, value2D98) == 0x2D98) ? 1 : -1];
typedef char PlayerEnemy30408TimerAt5424[
    (offsetof(PlayerEnemyCallback30408EnemyView, specialAttackTimer5424) == 0x5424) ? 1 : -1];

extern RngRuntimeView g_ReplayRng;

int __fastcall PlayerEnemyCallback30408Default(
    PlayerLifecycleView *,
    PlayerEnemyCallback30408EnemyView *enemy)
{
    enemy->value2D98 = g_ReplayRng.GetRandomF32SignedInRange(0.1f);
    float value;
    if (enemy->specialAttackTimer5424 > 120)
        value = -1.0f;
    else
        value = static_cast<float>(enemy->specialAttackTimer5424) *
            -0.008333333767950535f;
    enemy->value2D9C = value;
    enemy->value2DA0 = 0;
    enemy->specialAttackTimer5424++;
    return 0;
}

int __fastcall PlayerEnemyCallback30408Type2(
    PlayerLifecycleView *,
    PlayerEnemyCallback30408EnemyView *enemy)
{
    enemy->value2D98 = 0.0f;
    enemy->value2D9C = 0.0f;
    enemy->value2DA0 = 0;
    enemy->specialAttackTimer5424++;
    return 0;
}

int __fastcall PlayerEnemyCallback30408Type4(
    PlayerLifecycleView *,
    PlayerEnemyCallback30408EnemyView *enemy)
{
    enemy->value2D98 = g_ReplayRng.GetRandomF32SignedInRange(0.1f);
    float value;
    if (enemy->specialAttackTimer5424 > 120)
        value = -1.5f;
    else
        value = static_cast<float>(enemy->specialAttackTimer5424) *
            -0.012500000186264515f;
    enemy->value2D9C = value;
    enemy->value2DA0 = 0;
    enemy->specialAttackTimer5424++;
    return 0;
}
