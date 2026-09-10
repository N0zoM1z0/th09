#include "Chain.hpp"
#include "Supervisor.hpp"

int Chain::RunCalcChain()
{
    ChainElem *tmp;
    ChainElem *current;
    int updatedCount;
    ChainCallbackResult result;

    g_Supervisor.EnterCriticalSectionWrapper(0);

restart_from_first_job:
    updatedCount = 0;
    current = &this->calcChain;
    while (current != NULL)
    {
        if (current->callback != NULL)
        {
execute_again:
            g_Supervisor.LeaveCriticalSectionWrapper(0);
            result = (ChainCallbackResult)current->callback(current->arg);
            g_Supervisor.EnterCriticalSectionWrapper(0);
            switch (result)
            {
            case CHAIN_CALLBACK_RESULT_CONTINUE_AND_REMOVE_JOB:
                tmp = current;
                current = current->next;
                CutImpl(tmp);
                updatedCount++;
                continue;
            case CHAIN_CALLBACK_RESULT_EXECUTE_AGAIN:
                goto execute_again;
            case CHAIN_CALLBACK_RESULT_EXIT_GAME_SUCCESS:
                updatedCount = 0;
                goto loop_exit;
            case CHAIN_CALLBACK_RESULT_BREAK:
                updatedCount = 1;
                goto loop_exit;
            case CHAIN_CALLBACK_RESULT_EXIT_GAME_ERROR:
                updatedCount = -1;
                goto loop_exit;
            case CHAIN_CALLBACK_RESULT_RESTART_FROM_FIRST_JOB:
                goto restart_from_first_job;
            default:
                break;
            }
            updatedCount++;
        }
        current = current->next;
    }

loop_exit:
    g_Supervisor.LeaveCriticalSectionWrapper(0);
    return updatedCount;
}

int Chain::RunDrawChain()
{
    ChainElem *tmp;
    ChainElem *current;
    int updatedCount;
    ChainCallbackResult result;

    updatedCount = 0;
    current = &this->drawChain;
    g_Supervisor.EnterCriticalSectionWrapper(0);
    while (current != NULL)
    {
        if (current->callback != NULL)
        {
execute_again:
            g_Supervisor.LeaveCriticalSectionWrapper(0);
            result = (ChainCallbackResult)current->callback(current->arg);
            g_Supervisor.EnterCriticalSectionWrapper(0);
            switch (result)
            {
            case CHAIN_CALLBACK_RESULT_CONTINUE_AND_REMOVE_JOB:
                tmp = current;
                current = current->next;
                CutImpl(tmp);
                updatedCount++;
                continue;
            case CHAIN_CALLBACK_RESULT_EXECUTE_AGAIN:
                goto execute_again;
            case CHAIN_CALLBACK_RESULT_EXIT_GAME_SUCCESS:
                updatedCount = 0;
                goto loop_exit;
            case CHAIN_CALLBACK_RESULT_BREAK:
                updatedCount = 1;
                goto loop_exit;
            case CHAIN_CALLBACK_RESULT_EXIT_GAME_ERROR:
                updatedCount = -1;
                goto loop_exit;
            default:
                break;
            }
            updatedCount++;
        }
        current = current->next;
    }

loop_exit:
    g_Supervisor.LeaveCriticalSectionWrapper(0);
    return updatedCount;
}
