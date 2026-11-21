#include "Supervisor.hpp"
#include "SupervisorNetworkService.hpp"
#include "RngRuntimeLeaves.hpp"
#include "AnmManager.hpp"
#include "Controller.hpp"
#include "SupervisorNetworkState.hpp"
#include "SupervisorNetworkPackets.hpp"
#include "SupervisorFrameQueueStorageView.hpp"

#include <windows.h>
#include <mmsystem.h>
#include <stddef.h>

namespace
{
// Keep the physical sender's original global symbol identity intact.
struct NetworkHandshakePacket : SupervisorNetworkHandshakeWire {};

// Preserve the original named packet symbol, while sharing its actual
// 12-byte wire payload with the native receive-message state allocation.
struct NetworkFramePacket : SupervisorNetworkState::LastReceivedFramePacketView {};

typedef char NetworkHandshakePacketSizeIs0C[
    (sizeof(NetworkHandshakePacket) == 0x0C) ? 1 : -1];
typedef char NetworkFramePacketSizeIs0C[
    (sizeof(NetworkFramePacket) == 0x0C) ? 1 : -1];
typedef char NetworkFramePacketInputAt08[
    (offsetof(NetworkFramePacket, words.input) == 0x08) ? 1 : -1];
typedef char NetworkFramePacketChecksumAt0A[
    (offsetof(NetworkFramePacket, words.checksum) == 0x0A) ? 1 : -1];
typedef char NetworkFramePacketPackedInputAt08[
    (offsetof(NetworkFramePacket, packedInput) == 0x08) ? 1 : -1];

}

extern RngRuntimeView g_ReplayRng;
extern NetworkFramePacket g_NetworkFramePacket;
extern NetworkHandshakePacket g_NetworkHandshakePacket;
extern const char g_NetworkDisconnectedMessage[];
extern const char g_NetworkWaitingMessage[];
extern const char g_NetworkSearchingMessage[];

extern unsigned short __fastcall ApplyNetworkInput(int side, unsigned short input);
extern int __fastcall SupervisorSubthreadIsRunning(Supervisor *supervisor);

// PopFrame returns a seed and a predicted-frame value through pointers.
// Retail uses the same eight-byte stack window in two mutually exclusive
// phases. The handshake writes seeds at -0xC/-0xA and prediction at -0x8;
// the final frame instead writes prediction at -0xC and seeds at -0x8/-0x6.
// The final input pair uses the adjacent stack words at EBP-0x14 and EBP-0x12.
struct NetworkPopFrameScratch
{
    union
    {
        struct First
        {
            unsigned short seed;
            unsigned short overlap;
            unsigned int predicted;
        } first;
        struct Second
        {
            unsigned short overlap;
            unsigned short seed;
            unsigned int predicted;
        } second;
        struct FinalFrame
        {
            unsigned int predicted;
            unsigned short firstSeed;
            unsigned short secondSeed;
        } finalFrame;
    };
};

typedef char NetworkPopFrameScratchLayout[
    (sizeof(NetworkPopFrameScratch) == 8 &&
     offsetof(NetworkPopFrameScratch, first.seed) == 0 &&
     offsetof(NetworkPopFrameScratch, second.seed) == 2 &&
     offsetof(NetworkPopFrameScratch, first.predicted) == 4 &&
     offsetof(NetworkPopFrameScratch, second.predicted) == 4 &&
     offsetof(NetworkPopFrameScratch, finalFrame.predicted) == 0 &&
     offsetof(NetworkPopFrameScratch, finalFrame.firstSeed) == 4 &&
     offsetof(NetworkPopFrameScratch, finalFrame.secondSeed) == 6) ? 1 : -1];
struct NetworkServiceStackScratch
{
    // The first two words hold the final input pair.
    unsigned short inputs[4];
    NetworkPopFrameScratch popFrame;
    int returnValue;
};
typedef char NetworkServiceStackScratchLayout[
    (sizeof(NetworkServiceStackScratch) == 20 &&
     offsetof(NetworkServiceStackScratch, inputs) == 0 &&
     offsetof(NetworkServiceStackScratch, popFrame) == 8 &&
     offsetof(NetworkServiceStackScratch, returnValue) == 16 &&
     sizeof(int) == 4 &&
     offsetof(NetworkServiceStackScratch, popFrame.first.seed) == 8 &&
     offsetof(NetworkServiceStackScratch, popFrame.first.predicted) == 12 &&
     offsetof(NetworkServiceStackScratch, popFrame.finalFrame.predicted) == 8 &&
     offsetof(NetworkServiceStackScratch, popFrame.finalFrame.firstSeed) == 12 &&
     offsetof(NetworkServiceStackScratch, popFrame.finalFrame.secondSeed) == 14) ? 1 : -1];

// Values are taken from the original network service's state and packet
// transitions. The original identifier spelling is not established.
enum NetworkSessionPhase
{
    NETWORK_SESSION_IDLE = 0,
    NETWORK_SESSION_SEARCHING = 1,
    NETWORK_SESSION_HOST_WAIT = 2,
    NETWORK_SESSION_PEER_READY = 3
};

enum NetworkFramePhase
{
    NETWORK_FRAME_PREPARING = 0,
    NETWORK_FRAME_AWAITING_PACKET = 1,
    NETWORK_FRAME_PREDICTING = 2
};

enum NetworkWireMessageKind
{
    NETWORK_MESSAGE_INPUT_FRAME = 0,
    NETWORK_MESSAGE_HANDSHAKE = 2
};

// timeGetTime is measured in milliseconds; these bounds come from the
// original host/join state machine rather than from a new retry policy.
// The prefill begins at the signed frame index -9998 and uses the matching
// positive offset to count through the native frame queue history.
enum NetworkServiceLimits
{
    NETWORK_HOST_POLL_MS = 100,
    NETWORK_PEER_JITTER_MS = 4000,
    NETWORK_JOIN_RETRY_MS = 4000,
    NETWORK_PEER_RETRY_MS = 6000,
    NETWORK_FRAME_WAIT_MS = 20000,
    NETWORK_DISCONNECT_TIMEOUT_MS = 60000,
    NETWORK_FRAMES_PER_SECOND = 60,
    NETWORK_PREFILL_START_FRAME = -9998,
    NETWORK_PREFILL_FRAME_OFFSET = 9998
};

int Supervisor::UpdateNetworkService()
{
    // The native return DWORD serves completion and reset/stop paths;
    // the early retry result is a fixed value. The input words and PopFrame
    // output retain their original stack offsets.
    NetworkServiceStackScratch scratch;
    scratch.returnValue = NETWORK_SERVICE_FRAME_READY;
    int stateReturnValue;

    this->EnterCriticalSectionWrapper(4);

    SupervisorNetworkState *frameNetwork = g_SupervisorNetworkState;
    {
        DWORD (WINAPI *getTime)() = timeGetTime;

    // The initial state/join decisions use the same network object, before
    // the connection callbacks independently reload their live receiver.
    if (frameNetwork->connectionState == NETWORK_SESSION_IDLE)
    {
        if (frameNetwork->joinMode != 0)
        {
            frameNetwork->localSide = 1;
            g_SupervisorNetworkState->CreateDeviceAddress();
            g_SupervisorNetworkState->CreateHostAddress();
            g_SupervisorNetworkState->ConnectSession();
            this->networkConnectStartTime588 = getTime();
            g_SupervisorNetworkState->connectionState = NETWORK_SESSION_SEARCHING;
            this->networkBusy478 = 1;
            g_AnmManager->DrawTextCentered(
                this->networkMessageVm584, 0xFFFFFF, 0,
                g_NetworkSearchingMessage);
            frameNetwork = g_SupervisorNetworkState;
        }
        else
        {
            goto HOST_SESSION;
        }
    }

    if (frameNetwork->connectionState == NETWORK_SESSION_SEARCHING)
    {
        DWORD now = getTime();
        this->networkBusy478 = 1;
        if (now < this->networkConnectStartTime588)
            this->networkConnectStartTime588 = now;

        if (now - this->networkConnectStartTime588 <
                g_ReplayRng.GetRandomU32InRange(NETWORK_PEER_JITTER_MS) + NETWORK_JOIN_RETRY_MS ||
            g_SupervisorNetworkState->joinMode == 1)
            goto NETWORK_WAIT;
        g_SupervisorNetworkState->ResetSession();
        goto HOST_SESSION;
    }
    goto HOST_WAIT_STATE_CHECK;

HOST_SESSION:
    Sleep(NETWORK_HOST_POLL_MS);
    g_SupervisorNetworkState->localSide = 0;
    g_SupervisorNetworkState->CreateDeviceAddress();
    g_SupervisorNetworkState->HostSession();
    g_AnmManager->DrawTextCentered(
        this->networkMessageVm584, 0xFFFFFF, 0,
        g_NetworkWaitingMessage);
    g_SupervisorNetworkState->connectionState = NETWORK_SESSION_HOST_WAIT;
    this->networkTiming458.connectionTime = getTime();
    frameNetwork = g_SupervisorNetworkState;

HOST_WAIT_STATE_CHECK:
        if (frameNetwork->connectionState == NETWORK_SESSION_HOST_WAIT || frameNetwork->connectionState == NETWORK_SESSION_PEER_READY)
    {
        if (frameNetwork->GetRemotePeerId() == 0)
        {
            if (this->networkTiming458.frameCounter != 0)
            {
                stateReturnValue = NETWORK_SERVICE_RESET;
                goto RETURN_STATE;
            }

            this->networkBusy478 = 1;
            DWORD now = getTime();
            if (now < this->networkTiming458.connectionTime)
                this->networkTiming458.connectionTime = now;

            if (now - this->networkTiming458.connectionTime >
                g_ReplayRng.GetRandomU32InRange(NETWORK_PEER_JITTER_MS) + NETWORK_PEER_RETRY_MS)
            {
                g_SupervisorNetworkState->ResetSession();
                g_SupervisorNetworkState->connectionState = NETWORK_SESSION_IDLE;
                this->networkTiming458.connectionTime = getTime();
            }
            goto NETWORK_WAIT;
        }

        this->networkBusy478 = 0;
        this->networkTiming458.connectionTime = getTime();
        frameNetwork = g_SupervisorNetworkState;
                }
        }

    // The native prediction edge is the nonzero, non-sync frame case.
    // Keep that frame in its own branch-local value; the preparing branch has
    // a separate signed prefill induction variable and never needs this value.
    if (this->networkTiming458.frameMode == NETWORK_FRAME_PREPARING ||
        this->networkTiming458.frameMode == NETWORK_FRAME_PREDICTING)
    {
        const unsigned int frame = static_cast<unsigned int>(
            this->networkTiming458.frameCounter);
        if (frame != 0 &&
            frame % (NETWORK_FRAMES_PER_SECOND / frameNetwork->syncRate) != 0)
        {
            const short firstSeed = static_cast<short>(g_ReplayRng.GetSeed());
            this->InsertPredictedFrame(
                frameNetwork->localSide, static_cast<int>(frame), firstSeed);
            const short predictedSeed = static_cast<short>(g_ReplayRng.GetSeed());
            SupervisorNetworkState *predictionNetwork = g_SupervisorNetworkState;
            const int opponentSide = 1 - predictionNetwork->localSide;
            this->InsertPredictedFrame(
                opponentSide, this->networkTiming458.frameCounter, predictedSeed);
            ++this->networkTiming458.frameCounter;
        }
        else
        {
            if (this->networkTiming458.frameMode == NETWORK_FRAME_PREPARING)
            {
                if (frameNetwork->initialFramesQueued == 0)
                {
                    if (frameNetwork->localSide == 0 &&
                        NETWORK_FRAMES_PER_SECOND / frameNetwork->syncRate - 1 > 0)
                    {
                        int queuedFrame = NETWORK_PREFILL_START_FRAME;
                        do
                        {
                            g_Supervisor.InsertReceivedFrame(
                                0, queuedFrame, 0,
                                static_cast<short>(g_ReplayRng.GetSeed()));
                            g_Supervisor.InsertReceivedFrame(
                                1, queuedFrame++, 0,
                                static_cast<short>(g_ReplayRng.GetSeed()));
                        } while (static_cast<int>(
                                     static_cast<unsigned int>(queuedFrame) +
                                     NETWORK_PREFILL_FRAME_OFFSET) <
                                 NETWORK_FRAMES_PER_SECOND / frameNetwork->syncRate - 1);
                    }

                    frameNetwork->packetReady = 0;
                    // Retain the selected packet sender across the seed call.
                    SupervisorNetworkState *handshakeNetwork = g_SupervisorNetworkState;
                    g_NetworkHandshakePacket.type = NETWORK_MESSAGE_HANDSHAKE;
                    g_NetworkHandshakePacket.side =
                        static_cast<unsigned char>(handshakeNetwork->localSide);
                    g_NetworkHandshakePacket.syncRate = handshakeNetwork->syncRate;
                    g_NetworkHandshakePacket.syncValue = handshakeNetwork->syncValue;
                    g_NetworkHandshakePacket.seed = g_ReplayRng.GetSeed();
                    handshakeNetwork->SendPacket(
                        &g_NetworkHandshakePacket,
                        sizeof(g_NetworkHandshakePacket));

                    if (g_SupervisorNetworkState->syncValue != 0)
                    {
                        this->PopFrame(0, &scratch.popFrame.first.seed,
                                       &scratch.popFrame.first.predicted);
                        this->PopFrame(1, &scratch.popFrame.second.seed,
                                       &scratch.popFrame.second.predicted);
                    }
                    g_SupervisorNetworkState->initialFramesQueued = 1;
                    frameNetwork = g_SupervisorNetworkState;
                }

                if (frameNetwork->packetReady == 0)
                {
                    scratch.returnValue = NETWORK_SERVICE_RETRY_FRAME;
                    goto RETURN_FRAME_RESULT;
                }

                g_NetworkFramePacket.words.input =
                    static_cast<unsigned short>(Controller::GetInput(
                        frameNetwork->localSide));
                ++this->networkTiming458.frameCounter;
                this->networkTiming458.frameStartTime = timeGetTime();
            }

            this->networkTiming458.lastFrameTime = timeGetTime();
            g_SupervisorNetworkState->packetReady = 0;
            SupervisorNetworkState *packetNetwork = g_SupervisorNetworkState;
            g_NetworkFramePacket.type = NETWORK_MESSAGE_INPUT_FRAME;
            signed char packetSide = static_cast<signed char>(packetNetwork->localSide);
            g_NetworkFramePacket.side = packetSide;
            g_NetworkFramePacket.frame =
                NETWORK_FRAMES_PER_SECOND / packetNetwork->syncRate - 1 +
                this->networkTiming458.frameCounter;
            g_NetworkFramePacket.seed = g_ReplayRng.GetSeed();

            unsigned int checksum =
                static_cast<unsigned int>(packetSide) +
                g_NetworkFramePacket.frame +
                g_NetworkFramePacket.seed +
                g_NetworkFramePacket.words.input;
            g_NetworkFramePacket.words.checksum =
                static_cast<unsigned short>(checksum);

            packetNetwork->SendPacket(
                &g_NetworkFramePacket, sizeof(g_NetworkFramePacket));
            this->networkTiming458.frameMode = NETWORK_FRAME_AWAITING_PACKET;
            this->InsertReceivedFrame(
                g_SupervisorNetworkState->localSide,
                g_NetworkFramePacket.frame,
                g_NetworkFramePacket.words.input,
                static_cast<short>(g_NetworkFramePacket.seed));
        }
        frameNetwork = g_SupervisorNetworkState;
    }

    // Publishing and prediction can replace the receiver. The bypass path
    // carries the receiver already selected by the session phase.
    if (!this->AreFrameQueuesSynchronized(1 - frameNetwork->localSide))
    {
        DWORD now = timeGetTime();
        this->networkTiming458.waitTime = now;
        if (now < this->networkTiming458.frameStartTime)
            this->networkTiming458.frameStartTime = now;

        if (now - this->networkTiming458.frameStartTime <= NETWORK_FRAME_WAIT_MS)
        {
            scratch.returnValue = NETWORK_SERVICE_RETRY_FRAME;
            goto RETURN_FRAME_RESULT;
        }

        if (now - this->networkTiming458.frameStartTime > NETWORK_DISCONNECT_TIMEOUT_MS)
        {
            stateReturnValue = NETWORK_SERVICE_RESET;
            goto RETURN_STATE;
        }

        this->networkBusy478 = 1;
        g_AnmManager->DrawTextCentered(
            this->networkMessageVm584, 0xFFFFFF, 0,
            g_NetworkDisconnectedMessage);

    NETWORK_WAIT:
        stateReturnValue = NETWORK_SERVICE_WAIT;

    RETURN_STATE:
        this->LeaveCriticalSectionWrapper(4);
        if ((Controller::GetInput(2) & 2) != 0)
            return NETWORK_SERVICE_RESET;
        if ((this->flagsLow5D4 & 0x80) == 0 || SupervisorSubthreadIsRunning(this))
            return stateReturnValue;
        return NETWORK_SERVICE_STOP;
    }

    {
        this->networkTiming458.confirmedFrame = this->networkTiming458.frameCounter;
        this->networkTiming458.frameMode = NETWORK_FRAME_PREPARING;
        scratch.inputs[0] = static_cast<unsigned short>(
            this->PopFrame(0, &scratch.popFrame.finalFrame.firstSeed,
                           &scratch.popFrame.finalFrame.predicted));
        scratch.inputs[1] = static_cast<unsigned short>(
            this->PopFrame(1, &scratch.popFrame.finalFrame.secondSeed,
                           &scratch.popFrame.finalFrame.predicted));
        // Publish each 16-bit input, then publish their combined mask.
        const unsigned short firstInput = scratch.inputs[0];
        ApplyNetworkInput(0, firstInput);
        const unsigned short secondInput = scratch.inputs[1];
        ApplyNetworkInput(1, secondInput);
        ApplyNetworkInput(2, firstInput | secondInput);
    RETURN_FRAME_RESULT:
        this->LeaveCriticalSectionWrapper(4);
        return scratch.returnValue;
    }
}
