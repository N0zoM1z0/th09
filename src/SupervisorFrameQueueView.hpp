#pragma once

#include "Supervisor.hpp"
#include <stddef.h>

// Compatibility façade for still-open network service callsite models.
// Every real queue method is declared on Supervisor itself, whose 0x7B0-byte
// allocation physically owns the +0x47C/+0x56C frame records. This empty
// derived tag adds no storage or virtual methods.
struct SupervisorFrameQueueView : Supervisor
{
    // Legacy zero-storage caller façade; method ownership is Supervisor.
    // Caller removal and this facade's deletion are a separate bounded step.
};
typedef char SupervisorQueueNativeReceiverLayout[
    (sizeof(SupervisorFrameQueueView) == sizeof(Supervisor) &&
     offsetof(SupervisorFrameQueueView, entries47C) == 0x47C &&
     offsetof(SupervisorFrameQueueView, lastReceived56C) == 0x56C) ? 1 : -1];
