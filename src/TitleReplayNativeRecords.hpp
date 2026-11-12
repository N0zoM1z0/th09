#pragma once

// Both Title replay renderers consume one actual 50-element ReplayDataView
// allocation at Title+0x68E4, with the original physical 0x1EC stride.
// This is a live lvalue: no snapshot, bounds gate, or shadow record owner.
#define TH09_TITLE_NATIVE_REPLAY_RECORD(owner, index) \
    (*reinterpret_cast<const ReplayDataView *>( \
        reinterpret_cast<unsigned int>(&(owner)->replays[0]) + \
        static_cast<unsigned int>(index) * sizeof((owner)->replays[0])))
