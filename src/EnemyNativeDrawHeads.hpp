#pragma once

// EnemyManager's four real +0x2AC410 draw-head pointer slots are the same
// allocation in both the giant OnUpdate append boss and the exact OnDraw
// traversal boss. The destination type is the caller's original view type;
// byte addressing uses one native unsigned DWORD group stride.
#define TH09_ENEMY_NATIVE_DRAW_HEAD(type, owner, index) \
    (*reinterpret_cast<type **>( \
        reinterpret_cast<unsigned int>(&(owner)->drawGroupHeads2AC410[0]) + \
        static_cast<unsigned int>(index) * sizeof((owner)->drawGroupHeads2AC410[0])))
