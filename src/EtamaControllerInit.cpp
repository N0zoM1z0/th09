#include "BulletManager.hpp"

#include <string.h>

static const unsigned short kBulletStateSentinel = 6;

void EtamaController::Initialize()
{
    Bullet *bullet;
    int i;

    memset(this, 0, sizeof(*this));
    this->primaryPoolStart = &this->primaryBullets[0];
    this->primaryBullets[175].state = kBulletStateSentinel;
    this->secondaryBullets[360].state = kBulletStateSentinel;
    this->secondaryPoolStart = &this->secondaryBullets[0];
    this->cancelItemType = 5;

    bullet = &this->primaryBullets[0];
    for (i = 0; i < 175; ++i, ++bullet)
    {
        bullet->sprites.bulletVm.scriptIndex = -1;
        bullet->sprites.despawnVm.scriptIndex = -1;
        bullet->sprites.spawnFastVm.scriptIndex = -1;
        bullet->sprites.spawnNormalVm.scriptIndex = -1;
        bullet->sprites.spawnSlowVm.scriptIndex = -1;
    }

    bullet = &this->secondaryBullets[0];
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
    memset(&this->primaryBullets[0], 0, sizeof(this->primaryBullets));
    memset(&this->secondaryBullets[0], 0, sizeof(this->secondaryBullets));
    memset(this->lasers, 0, sizeof(this->lasers));
    this->primaryPoolStart = &this->primaryBullets[0];
    this->secondaryPoolStart = &this->secondaryBullets[0];
    this->primaryBullets[175].state = kBulletStateSentinel;
    this->secondaryBullets[360].state = kBulletStateSentinel;
    this->cancelItemType = 5;
}
