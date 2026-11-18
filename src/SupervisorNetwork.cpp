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
// The final-phase scalar locals remain separate until their exact compiler
// lifetime is recovered; all three views describe the observed wire layout.
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
// Final PopFrame produces two adjacent native input words. Retail reloads
// both words through one DWORD home, sends each input independently, then
// ORs them for the aggregate input callback. Keep their physical pair typed.
struct NetworkFrameInputPair
{
    union
    {
        struct { unsigned short side0; unsigned short side1; };
        // The native first input is read as one DWORD after both 16-bit
        // PopFrame results have been stored. The callee consumes its low
        // word, while the side fields preserve the physical wire order.
        unsigned int packed;
    };
};
typedef char NativeNetworkFrameInputPairLayout[
    (sizeof(NetworkFrameInputPair) == 4 &&
     offsetof(NetworkFrameInputPair, side0) == 0 &&
     offsetof(NetworkFrameInputPair, side1) == 2 &&
     offsetof(NetworkFrameInputPair, packed) == 0) ? 1 : -1];

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

int __fastcall SupervisorServiceUpdate(Supervisor *supervisor)
{
    Supervisor *s = supervisor;
    // Retail maintains independent frame-completion and wait/reset results.
    // The frame result begins at zero; the waiting status is assigned by the
    // session and timeout branches before reaching RETURN_STATE.
    int result = NETWORK_SERVICE_FRAME_READY;
    int status;
    DWORD now;

    supervisor->EnterCriticalSectionWrapper(4);

    // The initial state/join decisions use the same network object, before
    // the connection callbacks independently reload their live receiver.
    SupervisorNetworkState *network = g_SupervisorNetworkState;
    if (network->connectionState == NETWORK_SESSION_IDLE)
    {
        if (network->joinMode != 0)
        {
            network->localSide = 1;
            g_SupervisorNetworkState->CreateDeviceAddress();
            g_SupervisorNetworkState->CreateHostAddress();
            g_SupervisorNetworkState->ConnectSession();
            s->networkConnectStartTime588 = timeGetTime();
            g_SupervisorNetworkState->connectionState = NETWORK_SESSION_SEARCHING;
            s->networkBusy478 = 1;
            g_AnmManager->DrawTextCentered(
                s->networkMessageVm584, 0xFFFFFF, 0,
                g_NetworkSearchingMessage);
        }
        else
        {
        HOST_SESSION:
            Sleep(NETWORK_HOST_POLL_MS);
            g_SupervisorNetworkState->localSide = 0;
            g_SupervisorNetworkState->CreateDeviceAddress();
            g_SupervisorNetworkState->HostSession();
            g_AnmManager->DrawTextCentered(
                s->networkMessageVm584, 0xFFFFFF, 0,
                g_NetworkWaitingMessage);
            g_SupervisorNetworkState->connectionState = NETWORK_SESSION_HOST_WAIT;
            s->networkTiming458.connectionTime = timeGetTime();
        }
    }

    if (g_SupervisorNetworkState->connectionState == NETWORK_SESSION_SEARCHING)
    {
        now = timeGetTime();
        s->networkBusy478 = 1;
        if (now < s->networkConnectStartTime588)
            s->networkConnectStartTime588 = now;

        if (now - s->networkConnectStartTime588 <
                g_ReplayRng.GetRandomU32InRange(NETWORK_PEER_JITTER_MS) + NETWORK_JOIN_RETRY_MS ||
            g_SupervisorNetworkState->joinMode == 1)
        {
            goto NETWORK_WAIT;
        }

        g_SupervisorNetworkState->ResetSession();
        goto HOST_SESSION;
    }

    if (g_SupervisorNetworkState->connectionState == NETWORK_SESSION_HOST_WAIT || g_SupervisorNetworkState->connectionState == NETWORK_SESSION_PEER_READY)
    {
        if (g_SupervisorNetworkState->GetRemotePeerId() == 0)
        {
            if (s->networkTiming458.frameCounter != 0)
            {
                status = NETWORK_SERVICE_RESET;
                goto RETURN_STATE;
            }

            s->networkBusy478 = 1;
            now = timeGetTime();
            if (now < s->networkTiming458.connectionTime)
                s->networkTiming458.connectionTime = now;

            if (now - s->networkTiming458.connectionTime >
                g_ReplayRng.GetRandomU32InRange(NETWORK_PEER_JITTER_MS) + NETWORK_PEER_RETRY_MS)
            {
                g_SupervisorNetworkState->ResetSession();
                g_SupervisorNetworkState->connectionState = NETWORK_SESSION_IDLE;
                s->networkTiming458.connectionTime = timeGetTime();
            }
            goto NETWORK_WAIT;
        }

        s->networkBusy478 = 0;
        s->networkTiming458.connectionTime = timeGetTime();
    }

    // Prefill and the first prediction retain this session across queue/seed calls.
    SupervisorNetworkState *frameNetwork = g_SupervisorNetworkState;

    if (s->networkTiming458.frameMode == NETWORK_FRAME_PREPARING || s->networkTiming458.frameMode == NETWORK_FRAME_PREDICTING)
    {
        unsigned int frame = s->networkTiming458.frameCounter;

        if (frame == 0 ||
            frame % (NETWORK_FRAMES_PER_SECOND / g_SupervisorNetworkState->syncRate) == 0)
        {
            if (s->networkTiming458.frameMode == NETWORK_FRAME_PREPARING)
            {
                if (frameNetwork->initialFramesQueued == 0)
                {
                    if (frameNetwork->localSide == 0 &&
                        NETWORK_FRAMES_PER_SECOND / frameNetwork->syncRate - 1 > 0)
                    {
                        unsigned int queuedFrame = static_cast<unsigned int>(NETWORK_PREFILL_START_FRAME);
                        do
                        {
                            g_Supervisor.InsertReceivedFrame(
                                0, static_cast<int>(queuedFrame), 0,
                                static_cast<short>(g_ReplayRng.GetSeed()));
                            g_Supervisor.InsertReceivedFrame(
                                1, static_cast<int>(queuedFrame++), 0,
                                static_cast<short>(g_ReplayRng.GetSeed()));
                        // The original i386 loop adds 9998 as a DWORD and
                        // performs a signed comparison on that wrapped sum.
                        } while (static_cast<int>(queuedFrame + static_cast<unsigned int>(NETWORK_PREFILL_FRAME_OFFSET)) <
                                 NETWORK_FRAMES_PER_SECOND / frameNetwork->syncRate - 1);
                    }

                    frameNetwork->packetReady = 0;
                    // Retain the selected receiver across GetSeed.
                    SupervisorNetworkState *handshakeNetwork = g_SupervisorNetworkState;
                    g_NetworkHandshakePacket.type = NETWORK_MESSAGE_HANDSHAKE;
                    g_NetworkHandshakePacket.side =
                        static_cast<unsigned char>(g_SupervisorNetworkState->localSide);
                    g_NetworkHandshakePacket.syncRate = g_SupervisorNetworkState->syncRate;
                    g_NetworkHandshakePacket.syncValue = g_SupervisorNetworkState->syncValue;
                    g_NetworkHandshakePacket.seed =
                        g_ReplayRng.GetSeed();
                    handshakeNetwork->SendPacket(
                        &g_NetworkHandshakePacket,
                        sizeof(g_NetworkHandshakePacket));

                    if (g_SupervisorNetworkState->syncValue != 0)
                    {
                        NetworkPopFrameScratch prefill;
                        s->PopFrame(0, &prefill.first.seed, &prefill.first.predicted);
                        s->PopFrame(1, &prefill.second.seed, &prefill.second.predicted);
                    }
                    g_SupervisorNetworkState->initialFramesQueued = 1;
                }

                if (g_SupervisorNetworkState->packetReady != 0)
                    goto HAVE_PACKET;

            WAIT_FOR_FRAME:
                result = NETWORK_SERVICE_RETRY_FRAME;
                supervisor->LeaveCriticalSectionWrapper(4);
                return result;

            HAVE_PACKET:
                g_NetworkFramePacket.words.input =
                    static_cast<unsigned short>(Controller::GetInput(g_SupervisorNetworkState->localSide));
                s->networkTiming458.frameCounter = static_cast<int>(
                    static_cast<unsigned int>(s->networkTiming458.frameCounter) + 1u);
                s->networkTiming458.frameStartTime = timeGetTime();
            }

            s->networkTiming458.lastFrameTime = timeGetTime();
            g_SupervisorNetworkState->packetReady = 0;
            SupervisorNetworkState *packetNetwork = g_SupervisorNetworkState;
            g_NetworkFramePacket.type = NETWORK_MESSAGE_INPUT_FRAME;
            // The checksum uses the side captured before GetSeed.
            signed char packetSide = static_cast<signed char>(g_SupervisorNetworkState->localSide);
            g_NetworkFramePacket.side = packetSide;
            // The packet's signed frame field is serialized from a native
            // DWORD sum; preserve the same wraparound at the boundary.
            g_NetworkFramePacket.frame = static_cast<int>(
                static_cast<unsigned int>(NETWORK_FRAMES_PER_SECOND / g_SupervisorNetworkState->syncRate) +
                static_cast<unsigned int>(s->networkTiming458.frameCounter) - 1u);
            g_NetworkFramePacket.seed =
                g_ReplayRng.GetSeed();

            // The transmitted checksum is the low word of these four fields.
            unsigned int checksum =
                static_cast<unsigned int>(packetSide) +
                g_NetworkFramePacket.frame +
                g_NetworkFramePacket.seed +
                g_NetworkFramePacket.words.input;
            g_NetworkFramePacket.words.checksum =
                static_cast<unsigned short>(checksum);

            packetNetwork->SendPacket(
                &g_NetworkFramePacket, sizeof(g_NetworkFramePacket));
            s->networkTiming458.frameMode = NETWORK_FRAME_AWAITING_PACKET;
            s->InsertReceivedFrame(
                g_SupervisorNetworkState->localSide,
                g_NetworkFramePacket.frame,
                g_NetworkFramePacket.words.input,
                static_cast<short>(g_NetworkFramePacket.seed));
                }
        else
        {
            // The first predicted frame uses the captured network receiver,
            // but reads its side after the seed callback has finished.
            const short firstSeed = static_cast<short>(g_ReplayRng.GetSeed());
            volatile const int &firstSide = frameNetwork->localSide;
            s->InsertPredictedFrame(firstSide, frame, firstSeed);
            // Retail obtains the prediction seed before re-reading the
            // network side; the peer can change during service callbacks.
            const short predictedSeed = static_cast<short>(g_ReplayRng.GetSeed());
            volatile const int &liveSide = g_SupervisorNetworkState->localSide;
            const int opponentSide = static_cast<int>(
                1u - static_cast<unsigned int>(liveSide));
            s->InsertPredictedFrame(
                opponentSide, s->networkTiming458.frameCounter, predictedSeed);
            // The original service increments the signed frame counter with
            // an i386 DWORD INC, whose physical result wraps modulo 2^32.
            s->networkTiming458.frameCounter = static_cast<int>(
                static_cast<unsigned int>(s->networkTiming458.frameCounter) + 1u);
                }
    }

    // Retail side inversion uses i386 32-bit subtraction even if the
    // network-side DWORD is outside the expected zero/one range.
    if (!s->AreFrameQueuesSynchronized(static_cast<int>(
            1u - static_cast<unsigned int>(
                g_SupervisorNetworkState->localSide))))
    {
        now = timeGetTime();
        s->networkTiming458.waitTime = now;
        if (now < s->networkTiming458.frameStartTime)
            s->networkTiming458.frameStartTime = now;

        if (now - s->networkTiming458.frameStartTime <= NETWORK_FRAME_WAIT_MS)
            goto WAIT_FOR_FRAME;

        if (now - s->networkTiming458.frameStartTime > NETWORK_DISCONNECT_TIMEOUT_MS)
        {
            status = NETWORK_SERVICE_RESET;
            goto RETURN_STATE;
        }

        s->networkBusy478 = 1;
        g_AnmManager->DrawTextCentered(
            s->networkMessageVm584, 0xFFFFFF, 0,
            g_NetworkDisconnectedMessage);

    NETWORK_WAIT:
        status = NETWORK_SERVICE_WAIT;

    RETURN_STATE:
        supervisor->LeaveCriticalSectionWrapper(4);
        if ((Controller::GetInput(2) & 2) != 0)
            return NETWORK_SERVICE_RESET;
        if (s->flagsLow5D4 >= 0 || SupervisorSubthreadIsRunning(supervisor))
            return status;
        return NETWORK_SERVICE_STOP;
    }

    {
        NetworkFrameInputPair inputs;
        unsigned short seed0;
        unsigned short seed1;
        unsigned int predicted;

        s->networkTiming458.confirmedFrame = s->networkTiming458.frameCounter;
        s->networkTiming458.frameMode = NETWORK_FRAME_PREPARING;
        inputs.side0 = static_cast<unsigned short>(s->PopFrame(0, &seed0, &predicted));
        inputs.side1 = static_cast<unsigned short>(s->PopFrame(1, &seed1, &predicted));
        ApplyNetworkInput(0, static_cast<unsigned short>(inputs.packed));
        ApplyNetworkInput(1, inputs.side1);
        ApplyNetworkInput(2, inputs.side0 | inputs.side1);
        supervisor->LeaveCriticalSectionWrapper(4);
        return result;
    }
}
