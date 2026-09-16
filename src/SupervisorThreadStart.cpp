#include <windows.h>
#include <stddef.h>

struct SupervisorThreadStartView
{
    unsigned char unknown000[0x6AC];
    HANDLE runningSubthreadHandle;
    DWORD runningSubthreadId;
    int subthreadCloseRequestActive;
    int subthreadState;

    void ThreadClose();
    int ThreadStart(LPTHREAD_START_ROUTINE startFunction, void *startParam);
};

typedef char SupervisorThreadHandleAt6AC[
    (offsetof(SupervisorThreadStartView, runningSubthreadHandle) == 0x6AC) ? 1 : -1];
typedef char SupervisorThreadIdAt6B0[
    (offsetof(SupervisorThreadStartView, runningSubthreadId) == 0x6B0) ? 1 : -1];
typedef char SupervisorThreadCloseRequestAt6B4[
    (offsetof(SupervisorThreadStartView, subthreadCloseRequestActive) == 0x6B4) ? 1 : -1];
typedef char SupervisorThreadStateAt6B8[
    (offsetof(SupervisorThreadStartView, subthreadState) == 0x6B8) ? 1 : -1];

void SupervisorThreadStartView::ThreadClose()
{
    if (this->runningSubthreadHandle != NULL)
    {
        this->subthreadCloseRequestActive = 1;
        while (WaitForSingleObject(this->runningSubthreadHandle, 1000) == WAIT_TIMEOUT)
            Sleep(1);
        CloseHandle(this->runningSubthreadHandle);
        this->runningSubthreadHandle = NULL;
        this->subthreadCloseRequestActive = 0;
    }
}

int SupervisorThreadStartView::ThreadStart(
    LPTHREAD_START_ROUTINE startFunction, void *startParam)
{
    ThreadClose();
    runningSubthreadHandle = CreateThread(
        NULL, 0, startFunction, startParam, 0, &runningSubthreadId);
    subthreadState = 1;
    return runningSubthreadHandle != NULL ? 0 : -1;
}
