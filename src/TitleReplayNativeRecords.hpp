#pragma once

// Both Title replay renderers index the actual 50-element ReplayDataView
// array at Title+0x68E4. The caller maintains the original 0..49 slot range;
// this is a live native record lvalue, not a copied or shadow record.
#define TH09_TITLE_NATIVE_REPLAY_RECORD(owner, index) \
    ((owner)->replays[(index)])
