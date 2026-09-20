#include <windows.h>
#include <stddef.h>

struct SupervisorCriticalSectionArrayView
{
    unsigned char unknown000[0x6C0];
    CRITICAL_SECTION criticalSections6C0[5];

    void InitializeCriticalSections();
    void DeleteCriticalSections();
};

typedef char SupervisorCriticalSectionsAt6C0[
    (offsetof(SupervisorCriticalSectionArrayView, criticalSections6C0) == 0x6C0) ? 1 : -1];

void SupervisorCriticalSectionArrayView::InitializeCriticalSections()
{
    for (int i = 0; i < 5; ++i)
        InitializeCriticalSection(&criticalSections6C0[i]);
}

void SupervisorCriticalSectionArrayView::DeleteCriticalSections()
{
    for (int i = 0; i < 5; ++i)
        DeleteCriticalSection(&criticalSections6C0[i]);
}
