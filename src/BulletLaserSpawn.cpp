#include "AnmManager.hpp"
#include "BulletManager.hpp"

struct BulletPlayerView
{
    float AngleToPoint(Float3 *position);
};

extern int g_LaserColorSpriteOffsets[];

Laser *EtamaController::SpawnLaserPattern(BulletSpawnDescriptor *descriptor)
{
    Laser *laser = &this->lasers[0];

    if (this->spawnSuppressionFrames == 0 ||
        (descriptor->transformFlags & 4u) != 0)
    {
        int index = 0;
        for (;;)
        {
            if (laser->inUse == 0)
                break;
            ++index;
            ++laser;
            if (index >= 48)
                return laser;
        }

        reinterpret_cast<AnmLoaded *>(this->bulletAnm)->
            SetAndExecuteScriptIdx(
                &laser->bodyVm, descriptor->bulletType + 10);
        reinterpret_cast<AnmLoaded *>(this->bulletAnm)->SetSprite(
            &laser->bodyVm,
            laser->bodyVm.activeSpriteIndex + descriptor->color);
        reinterpret_cast<AnmLoaded *>(this->bulletAnm)->
            InitializeAndSetSprite(
                &laser->startCapVm,
                g_LaserColorSpriteOffsets[descriptor->color] + 146);

        laser->startCapVm.flagsWord =
            (laser->startCapVm.flagsWord & ~0x20u) | 0x10u;

        laser->position = descriptor->position;
        laser->color = descriptor->color;
        laser->inUse = 1;
        laser->angle = descriptor->angle;

        if (descriptor->aimMode == 0)
        {
            laser->angle +=
                this->sideState->player->AngleToPoint(&descriptor->position);
        }

        laser->flags =
            static_cast<unsigned short>(descriptor->transformFlags);
        laser->timer = 0;

        laser->startOffset = descriptor->laserStartOffset;
        laser->endOffset = descriptor->laserEndOffset;
        laser->startLength = descriptor->laserStartLength;
        laser->width = descriptor->laserWidth;
        laser->speed = descriptor->speed1;
        laser->startTime = descriptor->laserStartTime;
        laser->duration = descriptor->laserDuration;
        laser->despawnDuration = descriptor->laserDespawnDuration;
        laser->hitboxStartTime = descriptor->laserHitboxStartTime;
        laser->hitboxEndDelay = descriptor->laserHitboxEndDelay;

        laser->hideCapDuringStartup = 0;
        laser->state = laser->startTime == 0;
    }

    return laser;
}
