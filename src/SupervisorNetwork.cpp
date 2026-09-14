#include "Supervisor.hpp"

#include <windows.h>
#include <mmsystem.h>
#include <stddef.h>

namespace
{
struct ReplayRngNetworkView
{
    unsigned short seed;
    unsigned short unknown02;
    int generationCount;

    unsigned short GetSeed();
    unsigned int GetRandomU32InRange(unsigned int max);
};

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
    unsigned char side;
    unsigned short seed;
    int frame;
    unsigned int packedInput;
};

struct SupervisorNetworkState
{
    void *peer;
    void *deviceAddress;
    unsigned char unknown008[0x08];
    void *hostAddress;
    unsigned int sendAsyncHandle;
    unsigned int connectAsyncHandle;
    unsigned int peerIds[2];
    char hostname[0x80];
    unsigned int port;
    int active;
    int localSide;
    int packetReady;
    int initialFramesQueued;
    int connectionState;
    int syncValue;
    int joinMode;
    unsigned char syncRate;
    unsigned char unknown0C5[0x480 - 0x0C5];

    int GetRemotePeerId();
    void MarkActive();
    void InsertReceivedFrame(int side, int frame, int packedInput, short seed);
    void InsertPredictedFrame(int side, int frame, short seed);
    int PopFrame(int side, unsigned short *seed, unsigned int *predicted);
    int AreFrameQueuesSynchronized(int side);
    int SendPacket(void *packet, int size);
    int CreateDeviceAddress();
    int CreateHostAddress();
    unsigned int HostSession();
    unsigned int ConnectSession();
    int ResetSession();
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
    unsigned char unknown47C[0x584 - 0x47C];
    int messageScript;
    unsigned int connectStartTime;
    unsigned char unknown58C[0x5D4 - 0x58C];
    signed char flags5D4;
};

typedef char NetworkHandshakePacketSizeIs0C[
    (sizeof(NetworkHandshakePacket) == 0x0C) ? 1 : -1];
typedef char NetworkFramePacketSizeIs0C[
    (sizeof(NetworkFramePacket) == 0x0C) ? 1 : -1];
typedef char NetworkStateActiveAtA8[
    (offsetof(SupervisorNetworkState, active) == 0xA8) ? 1 : -1];
typedef char NetworkStateSideAtAC[
    (offsetof(SupervisorNetworkState, localSide) == 0xAC) ? 1 : -1];
typedef char NetworkStateRateAtC4[
    (offsetof(SupervisorNetworkState, syncRate) == 0xC4) ? 1 : -1];
typedef char SupervisorNetworkFrameAt458[
    (offsetof(SupervisorNetworkView, frameCounter) == 0x458) ? 1 : -1];
typedef char SupervisorNetworkMessageAt584[
    (offsetof(SupervisorNetworkView, messageScript) == 0x584) ? 1 : -1];
typedef char SupervisorNetworkFlagsAt5D4[
    (offsetof(SupervisorNetworkView, flags5D4) == 0x5D4) ? 1 : -1];
}

extern ReplayRngNetworkView g_ReplayRng;
extern SupervisorNetworkState *g_SupervisorNetworkState;
extern NetworkFramePacket g_NetworkFramePacket;
extern NetworkHandshakePacket g_NetworkHandshakePacket;
extern int g_NetworkMessageManager;
extern const char g_NetworkDisconnectedMessage[];
extern const char g_NetworkWaitingMessage[];
extern const char g_NetworkSearchingMessage[];

extern short __fastcall SampleNetworkInput(int side);
extern int __fastcall ApplyNetworkInput(int side, int input);
extern int __fastcall SupervisorSubthreadIsRunning(Supervisor *supervisor);
extern int __cdecl DrawNetworkMessage(
    int manager, int script, COLORREF color, int mode,
    const char *format, ...);

int __fastcall SupervisorServiceUpdate(Supervisor *supervisor)
{
    SupervisorNetworkView *s =
        reinterpret_cast<SupervisorNetworkView *>(supervisor);
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
            DrawNetworkMessage(
                g_NetworkMessageManager, s->messageScript, 0xFFFFFF, 0,
                g_NetworkSearchingMessage);
        }
        else
        {
        HOST_SESSION:
            Sleep(100);
            g_SupervisorNetworkState->localSide = 0;
            g_SupervisorNetworkState->CreateDeviceAddress();
            g_SupervisorNetworkState->HostSession();
            DrawNetworkMessage(
                g_NetworkMessageManager, s->messageScript, 0xFFFFFF, 0,
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

    if (s->frameMode == 0 || s->frameMode == 2)
    {
        unsigned int frame = s->frameCounter;

        if (frame != 0 &&
            frame % (60 / g_SupervisorNetworkState->syncRate) != 0)
        {
            g_SupervisorNetworkState->InsertPredictedFrame(
                g_SupervisorNetworkState->localSide, frame, (short)g_ReplayRng.GetSeed());
            g_SupervisorNetworkState->InsertPredictedFrame(
                1 - g_SupervisorNetworkState->localSide, s->frameCounter,
                (short)g_ReplayRng.GetSeed());
            s->frameCounter++;
        }
        else
        {
            if (s->frameMode == 0)
            {
                if (g_SupervisorNetworkState->initialFramesQueued == 0)
                {
                    if (g_SupervisorNetworkState->localSide == 0 &&
                        60 / g_SupervisorNetworkState->syncRate - 1 > 0)
                    {
                        int queuedFrame = -9998;
                        do
                        {
                            g_SupervisorNetworkState->InsertReceivedFrame(
                                0, queuedFrame, 0,
                                (short)g_ReplayRng.GetSeed());
                            g_SupervisorNetworkState->InsertReceivedFrame(
                                1, queuedFrame++, 0,
                                (short)g_ReplayRng.GetSeed());
                        } while (queuedFrame + 9998 <
                                 60 / g_SupervisorNetworkState->syncRate - 1);
                    }

                    g_SupervisorNetworkState->packetReady = 0;
                    g_NetworkHandshakePacket.type = 2;
                    g_NetworkHandshakePacket.side =
                        (unsigned char)g_SupervisorNetworkState->localSide;
                    g_NetworkHandshakePacket.syncRate = g_SupervisorNetworkState->syncRate;
                    g_NetworkHandshakePacket.syncValue = g_SupervisorNetworkState->syncValue;
                    g_NetworkHandshakePacket.seed = g_ReplayRng.GetSeed();
                    g_SupervisorNetworkState->SendPacket(
                        &g_NetworkHandshakePacket,
                        sizeof(g_NetworkHandshakePacket));

                    if (g_SupervisorNetworkState->syncValue != 0)
                    {
                        unsigned short ignoredSeed0;
                        unsigned short ignoredSeed1;
                        unsigned int ignoredPredicted;
                        g_SupervisorNetworkState->PopFrame(0, &ignoredSeed0, &ignoredPredicted);
                        g_SupervisorNetworkState->PopFrame(1, &ignoredSeed1, &ignoredPredicted);
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
                reinterpret_cast<unsigned short *>(
                    &g_NetworkFramePacket.packedInput)[0] =
                    (unsigned short)SampleNetworkInput(g_SupervisorNetworkState->localSide);
                s->frameCounter++;
                s->frameStartTime = timeGetTime();
            }

            s->lastFrameTime = timeGetTime();
            g_SupervisorNetworkState->packetReady = 0;
            g_NetworkFramePacket.type = 0;
            g_NetworkFramePacket.side = (unsigned char)g_SupervisorNetworkState->localSide;
            g_NetworkFramePacket.frame =
                60 / g_SupervisorNetworkState->syncRate +
                s->frameCounter - 1;
            g_NetworkFramePacket.seed = g_ReplayRng.GetSeed();

            unsigned int checksum =
                g_NetworkFramePacket.packedInput +
                g_NetworkFramePacket.seed +
                g_NetworkFramePacket.frame +
                g_NetworkFramePacket.side;
            reinterpret_cast<unsigned short *>(
                &g_NetworkFramePacket.packedInput)[1] =
                (unsigned short)checksum;

            g_SupervisorNetworkState->SendPacket(
                &g_NetworkFramePacket, sizeof(g_NetworkFramePacket));
            s->frameMode = 1;
            g_SupervisorNetworkState->InsertReceivedFrame(
                g_SupervisorNetworkState->localSide,
                g_NetworkFramePacket.frame,
                (int)g_NetworkFramePacket.packedInput,
                (short)g_NetworkFramePacket.seed);
        }
    }

    if (!g_SupervisorNetworkState->AreFrameQueuesSynchronized(1 - g_SupervisorNetworkState->localSide))
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
        DrawNetworkMessage(
            g_NetworkMessageManager, s->messageScript, 0xFFFFFF, 0,
            g_NetworkDisconnectedMessage);

    NETWORK_WAIT:
        returnState = 2;

    RETURN_STATE:
        supervisor->LeaveCriticalSectionWrapper(4);
        if ((SampleNetworkInput(2) & 2) != 0)
            return 3;
        if (s->flags5D4 >= 0 || SupervisorSubthreadIsRunning(supervisor))
            return returnState;
        return 4;
    }

    {
        unsigned short input0;
        unsigned short input1;
        unsigned short seed0;
        unsigned short seed1;
        unsigned int predicted;

        s->confirmedFrame = s->frameCounter;
        s->frameMode = 0;
        input0 = (unsigned short)g_SupervisorNetworkState->PopFrame(0, &seed0, &predicted);
        input1 = (unsigned short)g_SupervisorNetworkState->PopFrame(1, &seed1, &predicted);
        ApplyNetworkInput(0, input0);
        ApplyNetworkInput(1, input1);
        ApplyNetworkInput(2, input0 | input1);
        supervisor->LeaveCriticalSectionWrapper(4);
        return result;
    }
}
