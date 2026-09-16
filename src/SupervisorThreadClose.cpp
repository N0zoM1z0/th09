#include <stddef.h>
#include <windows.h>

struct SupervisorThreadCloseView
{
    unsigned char unknown000[0x6AC];
    HANDLE runningSubthreadHandle;
    unsigned char unknown6B0[0x04];
    int subthreadCloseRequestActive;
    int subthreadActive;

    void ThreadClose();
};

typedef char SupervisorThreadHandleAt6AC[
    (offsetof(SupervisorThreadCloseView, runningSubthreadHandle) == 0x6AC) ? 1 : -1];
typedef char SupervisorThreadCloseAt6B4[
    (offsetof(SupervisorThreadCloseView, subthreadCloseRequestActive) == 0x6B4) ? 1 : -1];
typedef char SupervisorThreadActiveAt6B8[
    (offsetof(SupervisorThreadCloseView, subthreadActive) == 0x6B8) ? 1 : -1];

void SupervisorThreadCloseView::ThreadClose()
{
    this->runningSubthreadHandle = NULL;
    this->subthreadCloseRequestActive = 0;
    this->subthreadActive = 0;
}
