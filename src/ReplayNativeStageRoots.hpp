#pragma once

// All ReplayManager recording, release and serialization paths walk the
// same ten authentic 0x18-byte inline ReplayBufferLink stage roots at +0x1C.
// Preserve the actual signed stage-selector DWORD stride and live lvalues.
#define TH09_REPLAY_NATIVE_STAGE_ROOT(owner, index) \
    (*reinterpret_cast<ReplayBufferLink *>( \
        reinterpret_cast<unsigned int>(&(owner)->stageBuffers[0]) + \
        static_cast<unsigned int>(index) * sizeof((owner)->stageBuffers[0])))
