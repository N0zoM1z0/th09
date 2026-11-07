#include <windows.h>
#include <stddef.h>

#include "SupervisorThreadStartView.hpp"

void SupervisorThreadStartView::ThreadClose()
{
    if (this->runningSubthreadHandlePtr != NULL)
    {
        this->subthreadCloseRequestActive = 1;
        while (WaitForSingleObject(this->runningSubthreadHandlePtr, 1000) == WAIT_TIMEOUT)
            Sleep(1);
        CloseHandle(this->runningSubthreadHandlePtr);
        this->runningSubthreadHandlePtr = NULL;
        this->subthreadCloseRequestActive = 0;
    }
}

int SupervisorThreadStartView::ThreadStart(
    LPTHREAD_START_ROUTINE startFunction, void *startParam)
{
    ThreadClose();
    runningSubthreadHandlePtr = CreateThread(
        NULL, 0, startFunction, startParam, 0, &runningSubthreadId6B0);
    subthreadActive = 1;
    return runningSubthreadHandlePtr != NULL ? 0 : -1;
}
