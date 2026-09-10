#include "Chain.hpp"
#include "Supervisor.hpp"

ChainElem::ChainElem()
{
    this->prev = NULL;
    this->next = NULL;
    this->callback = NULL;
    this->releaseTarget = this;
    this->addedCallback = NULL;
    this->deletedCallback = NULL;
    this->priority = 0;
    this->isHeapAllocated = false;
}

ChainElem::~ChainElem()
{
    if (this->deletedCallback != NULL)
        this->deletedCallback(this->arg);

    this->prev = NULL;
    this->next = NULL;
    this->callback = NULL;
    this->addedCallback = NULL;
    this->deletedCallback = NULL;
}

Chain::Chain()
{
}

Chain::~Chain()
{
}

int Chain::AddToCalcChain(ChainElem *elem, int priority)
{
    ChainElem *current = &this->calcChain;
    int result = 0;

    if (elem->addedCallback != NULL)
    {
        result = elem->addedCallback(elem->arg);
        elem->addedCallback = NULL;
    }

    g_Supervisor.EnterCriticalSectionWrapper(0);
    elem->priority = priority;
    while (current->next != NULL)
    {
        if (current->priority > priority)
            break;
        current = current->next;
    }

    if (current->priority > priority)
    {
        elem->next = current;
        elem->prev = current->prev;
        if (elem->prev != NULL)
            elem->prev->next = elem;
        current->prev = elem;
    }
    else
    {
        elem->next = NULL;
        elem->prev = current;
        current->next = elem;
    }

    g_Supervisor.LeaveCriticalSectionWrapper(0);
    return result;
}

int Chain::AddToDrawChain(ChainElem *elem, int priority)
{
    ChainElem *current = &this->drawChain;
    int result = 0;

    if (elem->addedCallback != NULL)
    {
        result = elem->addedCallback(elem->arg);
        elem->addedCallback = NULL;
    }

    g_Supervisor.EnterCriticalSectionWrapper(0);
    elem->priority = priority;
    while (current->next != NULL)
    {
        if (current->priority > priority)
            break;
        current = current->next;
    }

    if (current->priority > priority)
    {
        elem->next = current;
        elem->prev = current->prev;
        if (elem->prev != NULL)
            elem->prev->next = elem;
        current->prev = elem;
    }
    else
    {
        elem->next = NULL;
        elem->prev = current;
        current->next = elem;
    }

    g_Supervisor.LeaveCriticalSectionWrapper(0);
    return result;
}
