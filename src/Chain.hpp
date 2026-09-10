#pragma once

#include <stddef.h>

typedef int (*ChainCallback)(void *);
typedef int (*ChainLifetimeCallback)(void *);

enum ChainCallbackResult
{
    CHAIN_CALLBACK_RESULT_CONTINUE_AND_REMOVE_JOB = 0,
    CHAIN_CALLBACK_RESULT_CONTINUE = 1,
    CHAIN_CALLBACK_RESULT_EXECUTE_AGAIN = 2,
    CHAIN_CALLBACK_RESULT_BREAK = 3,
    CHAIN_CALLBACK_RESULT_EXIT_GAME_SUCCESS = 4,
    CHAIN_CALLBACK_RESULT_EXIT_GAME_ERROR = 5,
    CHAIN_CALLBACK_RESULT_RESTART_FROM_FIRST_JOB = 6,
};

class ChainElem
{
  public:
    ChainElem();
    ~ChainElem();
    void SetCallback(ChainCallback callback);

    short priority;
    unsigned short isHeapAllocated : 1;
    ChainCallback callback;
    ChainLifetimeCallback addedCallback;
    ChainLifetimeCallback deletedCallback;
    ChainElem *prev;
    ChainElem *next;
    ChainElem *releaseTarget;
    void *arg;
};

typedef char ChainElemSizeIs20[(sizeof(ChainElem) == 0x20) ? 1 : -1];
typedef char ChainElemReleaseTargetAt18[
    (offsetof(ChainElem, releaseTarget) == 0x18) ? 1 : -1];

class Chain
{
  private:
    ChainElem calcChain;
    ChainElem drawChain;

    void CutImpl(ChainElem *toRemove);

  public:
    Chain();
    ~Chain();

    int AddToCalcChain(ChainElem *elem, int priority);
    int AddToDrawChain(ChainElem *elem, int priority);
    ChainElem *CreateElem(ChainCallback callback);
    void Cut(ChainElem *toRemove);
    int RunCalcChain();
    int RunDrawChain();
};

typedef char ChainSizeIs40[(sizeof(Chain) == 0x40) ? 1 : -1];
