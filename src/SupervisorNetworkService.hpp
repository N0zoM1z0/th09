#pragma once

class Supervisor;

// Shared service/caller protocol observed at 0x430AA0 and 0x431110.
// These names describe the caller's actions; original enum spelling is unknown.
enum SupervisorNetworkServiceResult
{
    NETWORK_SERVICE_FRAME_READY = 0,
    NETWORK_SERVICE_RETRY_FRAME = 1,
    NETWORK_SERVICE_WAIT = 2,
    NETWORK_SERVICE_RESET = 3,
    NETWORK_SERVICE_STOP = 4
};

int __fastcall SupervisorServiceUpdate(Supervisor *supervisor);
