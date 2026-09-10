#include "Chain.hpp"
#include "Supervisor.hpp"

void Chain::Cut(ChainElem *toRemove)
{
    g_Supervisor.EnterCriticalSectionWrapper(0);
    CutImpl(toRemove);
    g_Supervisor.LeaveCriticalSectionWrapper(0);
}

void Chain::CutImpl(ChainElem *toRemove)
{
    if (toRemove == NULL)
        return;

    ChainElem *current = &this->calcChain;
    while (current != NULL)
    {
        if (current == toRemove)
            goto destroy_elem;
        current = current->next;
    }

    current = &this->drawChain;
    while (current != NULL)
    {
        if (current == toRemove)
            goto destroy_elem;
        current = current->next;
    }
    return;

destroy_elem:
    if (toRemove->prev != NULL)
    {
        toRemove->callback = NULL;
        toRemove->prev->next = toRemove->next;
        if (toRemove->next != NULL)
            toRemove->next->prev = toRemove->prev;
        toRemove->prev = NULL;
        toRemove->next = NULL;

        if (toRemove->isHeapAllocated)
        {
            g_Supervisor.LeaveCriticalSectionWrapper(0);
            delete toRemove;
            g_Supervisor.EnterCriticalSectionWrapper(0);
        }
        else if (toRemove->deletedCallback != NULL)
        {
            ChainLifetimeCallback callback = toRemove->deletedCallback;
            toRemove->deletedCallback = NULL;
            g_Supervisor.LeaveCriticalSectionWrapper(0);
            callback(toRemove->arg);
            g_Supervisor.EnterCriticalSectionWrapper(0);
        }
    }
}
