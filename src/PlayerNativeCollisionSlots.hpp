#pragma once

// The Player's active collision-region chain is held in authentic pointer
// words at activeB100. Advance its live pointer-slot cursor as an i386
// byte address, not an independently bounded C++ pointer array.
// Both CalcDamageToEnemy and CheckBulletCollision use this same allocation.
#define TH09_PLAYER_NATIVE_NEXT_REGION_SLOT(slot) \
    reinterpret_cast<PlayerCollisionRegionCreateRegionView **>( \
        reinterpret_cast<unsigned int>(slot) + sizeof(*(slot)))
