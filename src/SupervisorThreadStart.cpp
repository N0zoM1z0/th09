#include <windows.h>
#include <stddef.h>

struct SupervisorThreadStartView
{
    unsigned char unknown000[0x6AC];
    HANDLE runningSubthreadHandle;
    DWORD runningSubthreadId;
    unsigned char unknown6B4[4];
    int subthreadState;

    void ThreadClose();
    int ThreadStart(LPTHREAD_START_ROUTINE startFunction, void *startParam);
};

typedef char SupervisorThreadHandleAt6AC[
    (offsetof(SupervisorThreadStartView, runningSubthreadHandle) == 0x6AC) ? 1 : -1];
typedef char SupervisorThreadIdAt6B0[
    (offsetof(SupervisorThreadStartView, runningSubthreadId) == 0x6B0) ? 1 : -1];
typedef char SupervisorThreadStateAt6B8[
    (offsetof(SupervisorThreadStartView, subthreadState) == 0x6B8) ? 1 : -1];

int SupervisorThreadStartView::ThreadStart(
    LPTHREAD_START_ROUTINE startFunction, void *startParam)
{
    ThreadClose();
    runningSubthreadHandle = CreateThread(
        NULL, 0, startFunction, startParam, 0, &runningSubthreadId);
    subthreadState = 1;
    return runningSubthreadHandle != NULL ? 0 : -1;
}
