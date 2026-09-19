#include "SupervisorFrameQueueView.hpp"
#include <stddef.h>

struct NetworkFrameQueueHeadView
{
    int frame00;
    unsigned char unknown04[0x74];
};

struct SupervisorNetworkQueueSyncLayout
{
    unsigned char unknown000[0x480];
    NetworkFrameQueueHeadView queues480[2];
};

typedef char NetworkFrameQueueHeadSizeIs78[
    (sizeof(NetworkFrameQueueHeadView) == 0x78) ? 1 : -1];
typedef char SupervisorNetworkQueuesAt480[
    (offsetof(SupervisorNetworkQueueSyncLayout, queues480) == 0x480) ? 1 : -1];

int SupervisorFrameQueueView::AreFrameQueuesSynchronized(int side)
{
    SupervisorNetworkQueueSyncLayout *view =
        reinterpret_cast<SupervisorNetworkQueueSyncLayout *>(this);

    if (view->queues480[side].frame00 != 0 &&
        view->queues480[1].frame00 == view->queues480[0].frame00)
    {
        return 1;
    }
    return 0;
}
