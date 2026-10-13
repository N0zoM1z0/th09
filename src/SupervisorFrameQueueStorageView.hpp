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

struct SupervisorFrameQueueStorageView
{
    unsigned char unknown000[0x47C];
    SupervisorFrameEntryStorageView entries47C[20];
    SupervisorFrameEntryStorageView lastReceived56C[2];
};
typedef char SupervisorFrameQueueEntriesAt47C[
    (offsetof(SupervisorFrameQueueStorageView, entries47C) == 0x47C) ? 1 : -1];
typedef char SupervisorFrameQueueLastReceivedAt56C[
    (offsetof(SupervisorFrameQueueStorageView, lastReceived56C) == 0x56C) ? 1 : -1];
typedef char SupervisorFrameQueueStorageSizeIs584[
    (sizeof(SupervisorFrameQueueStorageView) == 0x584) ? 1 : -1];

// Source-only role names retained for the two independently compiled bodies.
typedef SupervisorFrameEntryStorageView SupervisorFrameEntryView;
typedef SupervisorFrameEntryStorageView SupervisorFrameEntryOpsView;
typedef SupervisorFrameQueueStorageView SupervisorFrameQueueInsertLayout;
typedef SupervisorFrameQueueStorageView SupervisorFrameQueueOpsLayout;
