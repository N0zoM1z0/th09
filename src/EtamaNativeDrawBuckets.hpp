#pragma once

// The six bullet draw-head pointers are a real contiguous array in the
// original EtamaController. Keep live reads/writes and unsigned i386 index
// strides identical in both its OnUpdate and OnDraw original function owners.
#define TH09_ETAMA_NATIVE_DRAW_BUCKET(owner, index) \
    (*reinterpret_cast<Bullet **>( \
        reinterpret_cast<unsigned int>(&(owner)->drawBuckets[0]) + \
        static_cast<unsigned int>(index) * sizeof((owner)->drawBuckets[0])))
