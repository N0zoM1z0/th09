#include "BulletManager.hpp"

// Original TH09 constructor semantics arise from the actual native members:
// AnmVm groups, Bullet's ZunTimer pair and transform-state array, and Laser's
// two AnmVm members plus its timer. The Bullet carrier is /Ob0: VC7.1 then
// emits the native EH vector-constructor call for its seven 0x2C states;
// /Ob1 instead expands that array into a loop and misses the retail body.
BulletTypeSprites::BulletTypeSprites()
{
}

Bullet::Bullet()
{
}

Laser::Laser()
{
}

// These physical offsets are proved independently by the exact Etama
// constructor and the original Bullet/Laser caller operands.
typedef char BulletNativeConstructionMembersAtTargetOffsets[
    (sizeof(BulletTypeSprites) == 0xD48 &&
     offsetof(BulletTypeSprites, bulletVm) == 0x00 &&
     offsetof(BulletTypeSprites, spawnFastVm) == 0x2A4 &&
     offsetof(BulletTypeSprites, spawnNormalVm) == 0x548 &&
     offsetof(BulletTypeSprites, spawnSlowVm) == 0x7EC &&
     offsetof(BulletTypeSprites, despawnVm) == 0xA90 &&
     sizeof(Bullet) == 0x10C4 &&
     offsetof(Bullet, sprites) == 0x04 &&
     offsetof(Bullet, stateTimer) == 0xD88 &&
     offsetof(Bullet, activeTimer) == 0xD94 &&
     offsetof(Bullet, exStates) == 0xF88 &&
     sizeof(BulletExState) == 0x2C &&
     offsetof(BulletExState, timer) == 0x00 &&
     sizeof(Laser) == 0x59C &&
     offsetof(Laser, startCapVm) == 0x2A4 &&
     offsetof(Laser, timer) == 0x588) ? 1 : -1];
