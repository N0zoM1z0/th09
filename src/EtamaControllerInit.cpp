#include "BulletManager.hpp"

#include <string.h>

static const unsigned short kBulletStateSentinel = 6;

void EtamaController::Initialize()
{
    Bullet *bullet;
    int i;

    memset(this, 0, sizeof(*this));
    this->primaryPoolStart = &this->bullets[0];
    this->bullets[175].state = kBulletStateSentinel;
    this->bullets[536].state = kBulletStateSentinel;
    this->secondaryPoolStart = &this->bullets[176];
    this->cancelItemType = 5;

    bullet = &this->bullets[0];
    for (i = 0; i < 175; ++i, ++bullet)
    {
        bullet->sprites.bulletVm.scriptIndex = -1;
        bullet->sprites.despawnVm.scriptIndex = -1;
        bullet->sprites.spawnFastVm.scriptIndex = -1;
        bullet->sprites.spawnNormalVm.scriptIndex = -1;
        bullet->sprites.spawnSlowVm.scriptIndex = -1;
    }

    bullet = &this->bullets[176];
    for (i = 0; i < 360; ++i, ++bullet)
    {
        bullet->sprites.bulletVm.scriptIndex = -1;
        bullet->sprites.despawnVm.scriptIndex = -1;
        bullet->sprites.spawnFastVm.scriptIndex = -1;
        bullet->sprites.spawnNormalVm.scriptIndex = -1;
        bullet->sprites.spawnSlowVm.scriptIndex = -1;
    }
}

void EtamaController::ResetProjectiles()
{
    memset(&this->bullets[0], 0, sizeof(Bullet) * 176);
    memset(&this->bullets[176], 0, sizeof(Bullet) * 361);
    memset(this->lasers, 0, sizeof(this->lasers));
    this->primaryPoolStart = &this->bullets[0];
    this->secondaryPoolStart = &this->bullets[176];
    this->bullets[175].state = kBulletStateSentinel;
    this->bullets[536].state = kBulletStateSentinel;
    this->cancelItemType = 5;
}
