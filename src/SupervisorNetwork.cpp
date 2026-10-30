#include "Supervisor.hpp"
#include "RngRuntimeLeaves.hpp"
#include "ReplayRuntimeLeaves.hpp"
#include "AnmManager.hpp"
#include "Controller.hpp"
#include "SupervisorNetworkState.hpp"
#include "SupervisorFrameQueueView.hpp"
#include "SupervisorFrameQueueStorageView.hpp"

#include <windows.h>
#include <mmsystem.h>
#include <stddef.h>

namespace
{
struct NetworkHandshakePacket
{
    unsigned char type;
    unsigned char side;
    unsigned short seed;
    unsigned char syncRate;
    unsigned char unknown05[3];
    unsigned int syncValue;
};

struct NetworkFramePacket
{
    unsigned char type;
    signed char side;
    unsigned short seed;
    int frame;
    // The checksum reads the complete word pair, including the previous high
    // word. Publishing input replaces the low word; checksum replaces the high.
    union
    {
        unsigned int packedInput;
        struct
        {
            unsigned short input;
            unsigned short checksum;
        } words;
    };
};

struct SupervisorNetworkView
{
    unsigned char unknown000[0x458];
    int frameCounter;
    int confirmedFrame;
    int frameMode;
    unsigned int frameStartTime;
    unsigned int lastFrameTime;
    unsigned int waitTime;
    unsigned char unknown470[0x04];
    unsigned int connectionTime;
    int networkBusy;
    SupervisorFrameEntryStorageView entries47C[20];
    SupervisorFrameEntryStorageView lastReceived56C[2];
    AnmVm *messageVm;
    unsigned int connectStartTime;
    unsigned char unknown58C[0x5D4 - 0x58C];
    signed char flags5D4;
};

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
typedef char SupervisorNetworkFrameAt458[
    (offsetof(SupervisorNetworkView, frameCounter) == 0x458) ? 1 : -1];
typedef char SupervisorNetworkQueuesAt47C[
    (offsetof(SupervisorNetworkView, entries47C) == 0x47C) ? 1 : -1];
typedef char SupervisorNetworkLastReceivedAt56C[
    (offsetof(SupervisorNetworkView, lastReceived56C) == 0x56C) ? 1 : -1];
typedef char SupervisorNetworkQueueBytesAreF0[
    (sizeof(((SupervisorNetworkView *)0)->entries47C) == 0xF0) ? 1 : -1];
typedef char SupervisorNetworkLastReceivedBytesAre18[
    (sizeof(((SupervisorNetworkView *)0)->lastReceived56C) == 0x18) ? 1 : -1];
typedef char SupervisorNetworkMessageAt584[
    (offsetof(SupervisorNetworkView, messageVm) == 0x584) ? 1 : -1];
typedef char SupervisorNetworkFlagsAt5D4[
    (offsetof(SupervisorNetworkView, flags5D4) == 0x5D4) ? 1 : -1];
}

typedef char NetworkNativeRngSize[
    (sizeof(ReplayRngView) == 8 && sizeof(RngRuntimeView) == 8) ? 1 : -1];
typedef char NetworkNativeRngSeed[
    (offsetof(ReplayRngView, seed) == offsetof(RngRuntimeView, seed)) ? 1 : -1];
typedef char NetworkNativeRngCount[
    (offsetof(ReplayRngView, generationCount) == offsetof(RngRuntimeView, generationCount)) ? 1 : -1];
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
    SupervisorNetworkView *s =
        reinterpret_cast<SupervisorNetworkView *>(supervisor);
    SupervisorFrameQueueView *frameQueues =
        reinterpret_cast<SupervisorFrameQueueView *>(supervisor);
    int result = 0;
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
            s->connectStartTime = timeGetTime();
            g_SupervisorNetworkState->connectionState = 1;
            s->networkBusy = 1;
            g_AnmManager->DrawTextCentered(
                s->messageVm, 0xFFFFFF, 0,
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
                s->messageVm, 0xFFFFFF, 0,
                g_NetworkWaitingMessage);
            g_SupervisorNetworkState->connectionState = 2;
            s->connectionTime = timeGetTime();
        }
    }

    if (g_SupervisorNetworkState->connectionState == 1)
    {
        now = timeGetTime();
        s->networkBusy = 1;
        if (now < s->connectStartTime)
            s->connectStartTime = now;

        if (now - s->connectStartTime <
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
            if (s->frameCounter != 0)
            {
                returnState = 3;
                goto RETURN_STATE;
            }

            s->networkBusy = 1;
            now = timeGetTime();
            if (now < s->connectionTime)
                s->connectionTime = now;

            if (now - s->connectionTime >
                g_ReplayRng.GetRandomU32InRange(4000) + 6000)
            {
                g_SupervisorNetworkState->ResetSession();
                g_SupervisorNetworkState->connectionState = 0;
                s->connectionTime = timeGetTime();
            }
            goto NETWORK_WAIT;
        }

        s->networkBusy = 0;
        s->connectionTime = timeGetTime();
    }

    // Prefill and the first prediction retain this session across queue/seed calls.
    SupervisorNetworkState *frameNetwork = g_SupervisorNetworkState;

    if (s->frameMode == 0 || s->frameMode == 2)
    {
        unsigned int frame = s->frameCounter;

        if (frame != 0 &&
            frame % (60 / g_SupervisorNetworkState->syncRate) != 0)
        {
            frameQueues->InsertPredictedFrame(
                frameNetwork->localSide, frame,
                (short)reinterpret_cast<ReplayRngView &>(g_ReplayRng).GetSeed());
            frameQueues->InsertPredictedFrame(
                1 - g_SupervisorNetworkState->localSide, s->frameCounter,
                (short)reinterpret_cast<ReplayRngView &>(g_ReplayRng).GetSeed());
            s->frameCounter++;
        }
        else
        {
            if (s->frameMode == 0)
            {
                if (frameNetwork->initialFramesQueued == 0)
                {
                    if (frameNetwork->localSide == 0 &&
                        60 / frameNetwork->syncRate - 1 > 0)
                    {
                        int queuedFrame = -9998;
                        do
                        {
                            reinterpret_cast<SupervisorFrameQueueView *>(&g_Supervisor)->InsertReceivedFrame(
                                0, queuedFrame, 0,
                                (short)reinterpret_cast<ReplayRngView &>(g_ReplayRng).GetSeed());
                            reinterpret_cast<SupervisorFrameQueueView *>(&g_Supervisor)->InsertReceivedFrame(
                                1, queuedFrame++, 0,
                                (short)reinterpret_cast<ReplayRngView &>(g_ReplayRng).GetSeed());
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
                        reinterpret_cast<ReplayRngView &>(g_ReplayRng).GetSeed();
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
                result = 1;
                supervisor->LeaveCriticalSectionWrapper(4);
                return result;

            HAVE_PACKET:
                g_NetworkFramePacket.words.input =
                    (unsigned short)Controller::GetInput(g_SupervisorNetworkState->localSide);
                s->frameCounter++;
                s->frameStartTime = timeGetTime();
            }

            s->lastFrameTime = timeGetTime();
            g_SupervisorNetworkState->packetReady = 0;
            SupervisorNetworkState *packetNetwork = g_SupervisorNetworkState;
            g_NetworkFramePacket.type = 0;
            // The checksum uses the side captured before GetSeed.
            signed char packetSide = (signed char)g_SupervisorNetworkState->localSide;
            g_NetworkFramePacket.side = packetSide;
            g_NetworkFramePacket.frame =
                60 / g_SupervisorNetworkState->syncRate +
                s->frameCounter - 1;
            g_NetworkFramePacket.seed =
                reinterpret_cast<ReplayRngView &>(g_ReplayRng).GetSeed();

            unsigned int checksum =
                static_cast<unsigned int>(packetSide) +
                g_NetworkFramePacket.frame +
                g_NetworkFramePacket.seed +
                g_NetworkFramePacket.packedInput;
            g_NetworkFramePacket.words.checksum =
                (unsigned short)checksum;

            packetNetwork->SendPacket(
                &g_NetworkFramePacket, sizeof(g_NetworkFramePacket));
            s->frameMode = 1;
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
        s->waitTime = now;
        if (now < s->frameStartTime)
            s->frameStartTime = now;

        if (now - s->frameStartTime <= 20000)
            goto WAIT_FOR_FRAME;

        if (now - s->frameStartTime > 60000)
        {
            returnState = 3;
            goto RETURN_STATE;
        }

        s->networkBusy = 1;
        g_AnmManager->DrawTextCentered(
            s->messageVm, 0xFFFFFF, 0,
            g_NetworkDisconnectedMessage);

    NETWORK_WAIT:
        returnState = 2;

    RETURN_STATE:
        supervisor->LeaveCriticalSectionWrapper(4);
        if ((Controller::GetInput(2) & 2) != 0)
            return 3;
        if (s->flags5D4 >= 0 || SupervisorSubthreadIsRunning(supervisor))
            return returnState;
        return 4;
    }

    {
        unsigned short inputs[2];
        unsigned short seed0;
        unsigned short seed1;
        unsigned int predicted;

        s->confirmedFrame = s->frameCounter;
        s->frameMode = 0;
        inputs[0] = (unsigned short)frameQueues->PopFrame(0, &seed0, &predicted);
        inputs[1] = (unsigned short)frameQueues->PopFrame(1, &seed1, &predicted);
        ApplyNetworkInput(0, inputs[0]);
        ApplyNetworkInput(1, inputs[1]);
        ApplyNetworkInput(2, inputs[0] | inputs[1]);
        supervisor->LeaveCriticalSectionWrapper(4);
        return result;
    }
}
