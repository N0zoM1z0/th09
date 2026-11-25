#include "AnmManager.hpp"
#include "BulletManager.hpp"
#include "PlayerAngleView.hpp"

extern int g_BulletSpriteOffsetSmall[];

// The original sprite offset table contains 32-bit native words selected
// by the descriptor color. Retail has no C++ array-bounds condition.
static __forceinline int NativeLaserColorOffset(int color)
{
    return g_BulletSpriteOffsetSmall[color];
}

Laser *EtamaController::SpawnLaserPattern(BulletSpawnDescriptor *descriptor)
{
    Laser *laser = &this->lasers[0];

    if (this->spawnSuppressionFrames == 0 ||
        (descriptor->transformFlags & 4u) != 0)
    {
        int index = 0;
        do
        {
            if (laser->inUse == 0)
                goto found_laser;
            // Scan the original 48-element native Laser allocation.
            ++index;
            ++laser;
        }
        while (index < 48);
        return laser;

    found_laser:
        this->bulletAnm->
            SetAndExecuteScriptIdx(
                &laser->bodyVm, static_cast<int>(
                    static_cast<unsigned int>(descriptor->bulletType) + 10u));
        this->bulletAnm->SetSprite(
            &laser->bodyVm,
            static_cast<int>(
                    static_cast<unsigned int>(laser->bodyVm.activeSpriteIndex) +
                    static_cast<unsigned int>(descriptor->color)));
        this->bulletAnm->
            InitializeAndSetSprite(
                &laser->startCapVm,
                static_cast<int>(
                    static_cast<unsigned int>(NativeLaserColorOffset(descriptor->color)) +
                    146u));

        laser->startCapVm.flagsWord =
            (laser->startCapVm.flagsWord & ~0x20u) | 0x10u;

        laser->position = descriptor->position;
        laser->color = descriptor->color;
        laser->inUse = 1;
        laser->angle = descriptor->angle;

        if (descriptor->aimMode == 0)
        {
            laser->angle +=
                reinterpret_cast<PlayerAngleView *>(this->sideState->player04)->AngleToPoint(
                    reinterpret_cast<PlayerAngleFloat3View *>(&descriptor->position));
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
