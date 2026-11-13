#include "Chain.hpp"
#include "ZunMemory.hpp"
#include "SupervisorThreadStartView.hpp"

#include <new>

// The release routines are methods of the same actual 0x40-byte Chain
// allocation that owns the calc/draw roots and exact Cut callbacks.
void Chain::ReleaseSingleChain(ChainElem *root)
{
    // The target separates raw allocation from ChainElem construction.
    ChainElem releaseSnapshotHead;
    ChainElem *releaseSnapshotCursor;
    ChainElem *current;
    ChainElem *nextSnapshotEntry;

    releaseSnapshotCursor = new ChainElem();
    releaseSnapshotCursor = static_cast<ChainElem *>(g_ZunMemory.AddToRegistry(
        releaseSnapshotCursor, sizeof(ChainElem), "funcChainInf"));
    releaseSnapshotHead.next = releaseSnapshotCursor;

    current = root;
    while (current != NULL)
    {
        releaseSnapshotCursor->releaseTarget = current;
        nextSnapshotEntry = new ChainElem();
        releaseSnapshotCursor->next = static_cast<ChainElem *>(
            g_ZunMemory.AddToRegistry(nextSnapshotEntry, sizeof(ChainElem),
                                      "funcChainInf"));
        releaseSnapshotCursor = releaseSnapshotCursor->next;
        current = current->next;
    }

    current = &releaseSnapshotHead;
    do
    {
        this->Cut(current->releaseTarget);
        current = current->next;
    }
    while (current != NULL);

    releaseSnapshotCursor = releaseSnapshotHead.next;
    if (releaseSnapshotCursor != NULL)
    {
        do
        {
            nextSnapshotEntry = releaseSnapshotCursor->next;
            delete releaseSnapshotCursor;
            releaseSnapshotCursor = nextSnapshotEntry;
        }
        while (releaseSnapshotCursor != NULL);
    }
}

void Chain::Release()
{
    static_cast<SupervisorThreadStartView *>(&g_Supervisor)->ThreadClose();
    ReleaseSingleChain(&calcChain);
    ReleaseSingleChain(&drawChain);
}
