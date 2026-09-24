#include "Chain.hpp"
#include "ZunMemory.hpp"

#include <new>

struct SupervisorThreadStartView
{
    void ThreadClose();
};

extern SupervisorThreadStartView g_Supervisor;

struct ChainReleaseView
{
    ChainElem calcChain00;
    ChainElem drawChain20;

    void ReleaseSingleChain(ChainElem *root);
    void Release();
};

void ChainReleaseView::ReleaseSingleChain(ChainElem *root)
{
    // The target separates raw allocation from ChainElem construction.
    ChainElem releaseSnapshotHead;
    ChainElem *releaseSnapshotCursor;
    ChainElem *current;
    ChainElem *nextSnapshotEntry;

    releaseSnapshotCursor = static_cast<ChainElem *>(
        ::operator new(sizeof(ChainElem)));
    if (releaseSnapshotCursor != NULL)
        releaseSnapshotCursor = new (releaseSnapshotCursor) ChainElem();
    releaseSnapshotCursor = static_cast<ChainElem *>(g_ZunMemory.AddToRegistry(
        releaseSnapshotCursor, sizeof(ChainElem), "funcChainInf"));
    releaseSnapshotHead.next = releaseSnapshotCursor;

    current = root;
    while (current != NULL)
    {
        releaseSnapshotCursor->releaseTarget = current;
        nextSnapshotEntry = static_cast<ChainElem *>(
            ::operator new(sizeof(ChainElem)));
        if (nextSnapshotEntry != NULL)
            nextSnapshotEntry = new (nextSnapshotEntry) ChainElem();
        releaseSnapshotCursor->next = static_cast<ChainElem *>(
            g_ZunMemory.AddToRegistry(nextSnapshotEntry, sizeof(ChainElem),
                                      "funcChainInf"));
        releaseSnapshotCursor = releaseSnapshotCursor->next;
        current = current->next;
    }

    current = &releaseSnapshotHead;
    do
    {
        reinterpret_cast<Chain *>(this)->Cut(current->releaseTarget);
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

void ChainReleaseView::Release()
{
    g_Supervisor.ThreadClose();
    ReleaseSingleChain(&calcChain00);
    ReleaseSingleChain(&drawChain20);
}
