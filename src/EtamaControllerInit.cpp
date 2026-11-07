#include "BulletManager.hpp"
#include "EtamaNativePoolLimits.hpp"

#include <string.h>

static const unsigned short kBulletStateSentinel = 6;

void EtamaController::Initialize()
{
    Bullet *bullet;
    int i;

    memset(this, 0, sizeof(*this));
    this->primaryPoolStart = &this->primaryBullets[0];
    this->primaryBullets[EtamaNativePoolLimits::PrimaryUsableCount].state = kBulletStateSentinel;
    this->secondaryBullets[EtamaNativePoolLimits::SecondaryUsableCount].state = kBulletStateSentinel;
    this->secondaryPoolStart = &this->secondaryBullets[0];
    this->cancelItemType = 5;

    bullet = &this->primaryBullets[0];
    for (i = 0; i < EtamaNativePoolLimits::PrimaryUsableCount; ++i, ++bullet)
    {
        bullet->sprites.bulletVm.scriptIndex = -1;
        bullet->sprites.despawnVm.scriptIndex = -1;
        bullet->sprites.spawnFastVm.scriptIndex = -1;
        bullet->sprites.spawnNormalVm.scriptIndex = -1;
        bullet->sprites.spawnSlowVm.scriptIndex = -1;
    }

    bullet = &this->secondaryBullets[0];
    for (i = 0; i < EtamaNativePoolLimits::SecondaryUsableCount; ++i, ++bullet)
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
    this->primaryBullets[EtamaNativePoolLimits::PrimaryUsableCount].state = kBulletStateSentinel;
    this->secondaryBullets[EtamaNativePoolLimits::SecondaryUsableCount].state = kBulletStateSentinel;
    this->cancelItemType = 5;
}
