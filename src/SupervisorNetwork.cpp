#include "Supervisor.hpp"
#include "SupervisorNetworkService.hpp"
#include "RngRuntimeLeaves.hpp"
#include "AnmManager.hpp"
#include "Controller.hpp"
#include "SupervisorNetworkState.hpp"
#include "SupervisorNetworkPackets.hpp"
#include "SupervisorFrameQueueView.hpp"
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

extern int __fastcall ApplyNetworkInput(int side, int input);
extern int __fastcall SupervisorSubthreadIsRunning(Supervisor *supervisor);

int __fastcall SupervisorServiceUpdate(Supervisor *supervisor)
{
    Supervisor *s = supervisor;
    SupervisorFrameQueueView *frameQueues =
        static_cast<SupervisorFrameQueueView *>(supervisor);
    int result = NETWORK_SERVICE_FRAME_READY;
    DWORD now;
    int returnState;

    supervisor->EnterCriticalSectionWrapper(4);

    if (g_SupervisorNetworkState->connectionState == 0)
    {
        if (g_SupervisorNetworkState->joinMode != 0)
        {
            g_SupervisorNetworkState->localSide = 1;
            g_SupervisorNetworkState->CreateDeviceAddress();
            g_SupervisorNetworkState->CreateHostAddress();
            g_SupervisorNetworkState->ConnectSession();
            s->networkConnectStartTime588 = timeGetTime();
            g_SupervisorNetworkState->connectionState = 1;
            s->networkBusy478 = 1;
            g_AnmManager->DrawTextCentered(
                s->networkMessageVm584, 0xFFFFFF, 0,
                g_NetworkSearchingMessage);
        }
        else
        {
        HOST_SESSION:
            Sleep(100);
            g_SupervisorNetworkState->localSide = 0;
            g_SupervisorNetworkState->CreateDeviceAddress();
            g_SupervisorNetworkState->HostSession();
            g_AnmManager->DrawTextCentered(
                s->networkMessageVm584, 0xFFFFFF, 0,
                g_NetworkWaitingMessage);
            g_SupervisorNetworkState->connectionState = 2;
            s->networkTiming458.connectionTime = timeGetTime();
        }
    }

    if (g_SupervisorNetworkState->connectionState == 1)
    {
        now = timeGetTime();
        s->networkBusy478 = 1;
        if (now < s->networkConnectStartTime588)
            s->networkConnectStartTime588 = now;

        if (now - s->networkConnectStartTime588 <
                g_ReplayRng.GetRandomU32InRange(4000) + 4000 ||
            g_SupervisorNetworkState->joinMode == 1)
        {
            goto NETWORK_WAIT;
        }

        g_SupervisorNetworkState->ResetSession();
        goto HOST_SESSION;
    }

    if (g_SupervisorNetworkState->connectionState == 2 || g_SupervisorNetworkState->connectionState == 3)
    {
        if (g_SupervisorNetworkState->GetRemotePeerId() == 0)
        {
            if (s->networkTiming458.frameCounter != 0)
            {
                returnState = NETWORK_SERVICE_RESET;
                goto RETURN_STATE;
            }

            s->networkBusy478 = 1;
            now = timeGetTime();
            if (now < s->networkTiming458.connectionTime)
                s->networkTiming458.connectionTime = now;

            if (now - s->networkTiming458.connectionTime >
                g_ReplayRng.GetRandomU32InRange(4000) + 6000)
            {
                g_SupervisorNetworkState->ResetSession();
                g_SupervisorNetworkState->connectionState = 0;
                s->networkTiming458.connectionTime = timeGetTime();
            }
            goto NETWORK_WAIT;
        }

        s->networkBusy478 = 0;
        s->networkTiming458.connectionTime = timeGetTime();
    }

    // Prefill and the first prediction retain this session across queue/seed calls.
    SupervisorNetworkState *frameNetwork = g_SupervisorNetworkState;

    if (s->networkTiming458.frameMode == 0 || s->networkTiming458.frameMode == 2)
    {
        unsigned int frame = s->networkTiming458.frameCounter;

        if (frame != 0 &&
            frame % (60 / g_SupervisorNetworkState->syncRate) != 0)
        {
            frameQueues->InsertPredictedFrame(
                frameNetwork->localSide, frame,
                (short)g_ReplayRng.GetSeed());
            frameQueues->InsertPredictedFrame(
                1 - g_SupervisorNetworkState->localSide, s->networkTiming458.frameCounter,
                (short)g_ReplayRng.GetSeed());
            s->networkTiming458.frameCounter++;
        }
        else
        {
            if (s->networkTiming458.frameMode == 0)
            {
                if (frameNetwork->initialFramesQueued == 0)
                {
                    if (frameNetwork->localSide == 0 &&
                        60 / frameNetwork->syncRate - 1 > 0)
                    {
                        int queuedFrame = -9998;
                        do
                        {
                            static_cast<SupervisorFrameQueueView *>(&g_Supervisor)->InsertReceivedFrame(
                                0, queuedFrame, 0,
                                (short)g_ReplayRng.GetSeed());
                            static_cast<SupervisorFrameQueueView *>(&g_Supervisor)->InsertReceivedFrame(
                                1, queuedFrame++, 0,
                                (short)g_ReplayRng.GetSeed());
                        } while (queuedFrame + 9998 <
                                 60 / frameNetwork->syncRate - 1);
                    }

                    frameNetwork->packetReady = 0;
                    // Retain the selected receiver across GetSeed.
                    SupervisorNetworkState *handshakeNetwork = g_SupervisorNetworkState;
                    g_NetworkHandshakePacket.type = 2;
                    g_NetworkHandshakePacket.side =
                        (unsigned char)g_SupervisorNetworkState->localSide;
                    g_NetworkHandshakePacket.syncRate = g_SupervisorNetworkState->syncRate;
                    g_NetworkHandshakePacket.syncValue = g_SupervisorNetworkState->syncValue;
                    g_NetworkHandshakePacket.seed =
                        g_ReplayRng.GetSeed();
                    handshakeNetwork->SendPacket(
                        &g_NetworkHandshakePacket,
                        sizeof(g_NetworkHandshakePacket));

                    if (g_SupervisorNetworkState->syncValue != 0)
                    {
                        unsigned short ignoredSeed0;
                        unsigned short ignoredSeed1;
                        unsigned int ignoredPredicted;
                        frameQueues->PopFrame(0, &ignoredSeed0, &ignoredPredicted);
                        frameQueues->PopFrame(1, &ignoredSeed1, &ignoredPredicted);
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
                    (unsigned short)Controller::GetInput(g_SupervisorNetworkState->localSide);
                s->networkTiming458.frameCounter++;
                s->networkTiming458.frameStartTime = timeGetTime();
            }

            s->networkTiming458.lastFrameTime = timeGetTime();
            g_SupervisorNetworkState->packetReady = 0;
            SupervisorNetworkState *packetNetwork = g_SupervisorNetworkState;
            g_NetworkFramePacket.type = 0;
            // The checksum uses the side captured before GetSeed.
            signed char packetSide = (signed char)g_SupervisorNetworkState->localSide;
            g_NetworkFramePacket.side = packetSide;
            g_NetworkFramePacket.frame =
                60 / g_SupervisorNetworkState->syncRate +
                s->networkTiming458.frameCounter - 1;
            g_NetworkFramePacket.seed =
                g_ReplayRng.GetSeed();

            // The transmitted checksum is the low word of these four fields.
            unsigned int checksum =
                static_cast<unsigned int>(packetSide) +
                g_NetworkFramePacket.frame +
                g_NetworkFramePacket.seed +
                g_NetworkFramePacket.words.input;
            g_NetworkFramePacket.words.checksum =
                (unsigned short)checksum;

            packetNetwork->SendPacket(
                &g_NetworkFramePacket, sizeof(g_NetworkFramePacket));
            s->networkTiming458.frameMode = 1;
            frameQueues->InsertReceivedFrame(
                g_SupervisorNetworkState->localSide,
                g_NetworkFramePacket.frame,
                g_NetworkFramePacket.words.input,
                (short)g_NetworkFramePacket.seed);
        }
    }

    if (!frameQueues->AreFrameQueuesSynchronized(1 - g_SupervisorNetworkState->localSide))
    {
        now = timeGetTime();
        s->networkTiming458.waitTime = now;
        if (now < s->networkTiming458.frameStartTime)
            s->networkTiming458.frameStartTime = now;

        if (now - s->networkTiming458.frameStartTime <= 20000)
            goto WAIT_FOR_FRAME;

        if (now - s->networkTiming458.frameStartTime > 60000)
        {
            returnState = NETWORK_SERVICE_RESET;
            goto RETURN_STATE;
        }

        s->networkBusy478 = 1;
        g_AnmManager->DrawTextCentered(
            s->networkMessageVm584, 0xFFFFFF, 0,
            g_NetworkDisconnectedMessage);

    NETWORK_WAIT:
        returnState = NETWORK_SERVICE_WAIT;

    RETURN_STATE:
        supervisor->LeaveCriticalSectionWrapper(4);
        if ((Controller::GetInput(2) & 2) != 0)
            return NETWORK_SERVICE_RESET;
        if (s->flagsLow5D4 >= 0 || SupervisorSubthreadIsRunning(supervisor))
            return returnState;
        return NETWORK_SERVICE_STOP;
    }

    {
        unsigned short inputs[2];
        unsigned short seed0;
        unsigned short seed1;
        unsigned int predicted;

        s->networkTiming458.confirmedFrame = s->networkTiming458.frameCounter;
        s->networkTiming458.frameMode = 0;
        inputs[0] = (unsigned short)frameQueues->PopFrame(0, &seed0, &predicted);
        inputs[1] = (unsigned short)frameQueues->PopFrame(1, &seed1, &predicted);
        ApplyNetworkInput(0, inputs[0]);
        ApplyNetworkInput(1, inputs[1]);
        ApplyNetworkInput(2, inputs[0] | inputs[1]);
        supervisor->LeaveCriticalSectionWrapper(4);
        return result;
    }
}
