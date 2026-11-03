#pragma once

#include <stddef.h>

// Target-observed Supervisor-owned twin frame queues: twenty 0x0C records
// (two 10-frame ranges) at +0x47C, followed by two last-received records at
// +0x56C. Both the exact predicted-frame/consume functions and the nonexact
// received-frame producer use this same physical wire/data layout.
struct SupervisorFrameEntryStorageView
{
    unsigned short packedInput00;
    unsigned short seed02;
    int frame04;
    union
    {
        int value08;
        unsigned int predicted08;
    };
};
typedef char SupervisorFrameEntryStorageSizeIs0C[
    (sizeof(SupervisorFrameEntryStorageView) == 0x0C) ? 1 : -1];
typedef char SupervisorFrameEntryInputAt00[
    (offsetof(SupervisorFrameEntryStorageView, packedInput00) == 0x00) ? 1 : -1];
typedef char SupervisorFrameEntrySeedAt02[
    (offsetof(SupervisorFrameEntryStorageView, seed02) == 0x02) ? 1 : -1];
typedef char SupervisorFrameEntryFrameAt04[
    (offsetof(SupervisorFrameEntryStorageView, frame04) == 0x04) ? 1 : -1];
typedef char SupervisorFrameEntryFlagsAt08[
    (offsetof(SupervisorFrameEntryStorageView, predicted08) == 0x08) ? 1 : -1];

// The owning +0x47C..+0x584 array is declared only in Supervisor.hpp.
// Do not manufacture another 0x584-byte queue-owner overlay here.
// Keep the exact element names for original method source ABI.
typedef SupervisorFrameEntryStorageView SupervisorFrameEntryView;
typedef SupervisorFrameEntryStorageView SupervisorFrameEntryOpsView;
