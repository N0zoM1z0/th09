#include "Chain.hpp"
#include "ZunMemory.hpp"

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
    ChainElem releaseSnapshotHead;
    ChainElem *releaseSnapshotCursor;
    ChainElem *current;
    ChainElem *nextSnapshotEntry;

    releaseSnapshotCursor =
        static_cast<ChainElem *>(g_ZunMemory.AddToRegistry(
            new ChainElem(), sizeof(ChainElem), "funcChainInf"));
    releaseSnapshotHead.next = releaseSnapshotCursor;

    current = root;
    while (current != NULL)
    {
        releaseSnapshotCursor->releaseTarget = current;
        releaseSnapshotCursor->next =
            static_cast<ChainElem *>(g_ZunMemory.AddToRegistry(
                new ChainElem(), sizeof(ChainElem), "funcChainInf"));
        releaseSnapshotCursor = releaseSnapshotCursor->next;
        current = current->next;
    }

    current = &releaseSnapshotHead;
    while (current != NULL)
    {
        reinterpret_cast<Chain *>(this)->Cut(current->releaseTarget);
        current = current->next;
    }

    releaseSnapshotCursor = releaseSnapshotHead.next;
    while (releaseSnapshotCursor != NULL)
    {
        nextSnapshotEntry = releaseSnapshotCursor->next;
        delete releaseSnapshotCursor;
        releaseSnapshotCursor = NULL;
        releaseSnapshotCursor = nextSnapshotEntry;
    }
}

void ChainReleaseView::Release()
{
    g_Supervisor.ThreadClose();
    ReleaseSingleChain(&calcChain00);
    ReleaseSingleChain(&drawChain20);
}
