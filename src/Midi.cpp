#include "Midi.hpp"

#include <windows.h>
#include <mmsystem.h>
#include <string.h>

#include "FileSystem.hpp"
#include "GameErrorContext.hpp"
#include "Supervisor.hpp"
#include "ZunMemory.hpp"

struct SupervisorMidiView
{
    unsigned char unknown000[0x74];
    HWND window;
};
typedef char SupervisorMidiWindowAt74[(offsetof(SupervisorMidiView, window) == 0x74) ? 1 : -1];

extern GameErrorContext g_GameErrorContext;


LARGE_INTEGER g_DummyMidiTimerPerformanceCounter;

// The original MidiOutput owns 32 DWORD file/header pointers, real 0x20-byte
// track records, and 0x17-byte MIDI channel state. Access every live index
// through an unsigned Win32 address word, retaining original call order.
#define TH09_MIDI_NATIVE_FILE_DATA(idx) \
    (*reinterpret_cast<LPBYTE *>( \
        reinterpret_cast<unsigned int>(&this->fileData[0]) + \
        static_cast<unsigned int>(idx) * sizeof(this->fileData[0])))
#define TH09_MIDI_NATIVE_HEADER(idx) \
    (*reinterpret_cast<LPMIDIHDR *>( \
        reinterpret_cast<unsigned int>(&this->pendingLongMessageHeaders[0]) + \
        static_cast<unsigned int>(idx) * sizeof(this->pendingLongMessageHeaders[0])))
#define TH09_MIDI_NATIVE_TRACK(idx) \
    (*reinterpret_cast<MidiTrack *>( \
        reinterpret_cast<unsigned int>(this->tracks) + \
        static_cast<unsigned int>(idx) * sizeof(MidiTrack)))
#define TH09_MIDI_NATIVE_CHANNEL(idx) \
    (*reinterpret_cast<MidiChannel *>( \
        reinterpret_cast<unsigned int>(&this->channels[0]) + \
        static_cast<unsigned int>(idx) * sizeof(this->channels[0])))
#define TH09_MIDI_NATIVE_SYSEX_BYTE(idx) \
    (*reinterpret_cast<char *>( \
        reinterpret_cast<unsigned int>(longMessageHeader->lpData) + \
        static_cast<unsigned int>(idx) + 1u))



void MidiTimer::OnTimerElapsed()
{
}

MidiDevice::MidiDevice()
{
    handle = NULL;
    this->deviceId = 0;
}

MidiDevice::~MidiDevice()
{
    Close();
}

BOOL MidiDevice::OpenDevice(UINT uDeviceId)
{
    if (handle != NULL)
    {
        if (this->deviceId != uDeviceId)
        {
            Close();
        }
        else
        {
            return FALSE;
        }
    }

    this->deviceId = uDeviceId;

    return midiOutOpen(&handle, uDeviceId, (DWORD_PTR)reinterpret_cast<SupervisorMidiView *>(&g_Supervisor)->window, NULL, CALLBACK_WINDOW) !=
           MMSYSERR_NOERROR;
}

ZunResult MidiDevice::Close()
{
    if (handle == NULL)
    {
        return ZUN_ERROR;
    }

    midiOutReset(handle);
    midiOutClose(handle);
    handle = NULL;

    return ZUN_SUCCESS;
}

BOOL MidiDevice::SendLongMsg(LPMIDIHDR pmh)
{
    if (handle == NULL)
    {
        return FALSE;
    }
    else
    {
        if (midiOutPrepareHeader(handle, pmh, sizeof(*pmh)) != MMSYSERR_NOERROR)
        {
            return TRUE;
        }

        return midiOutLongMsg(handle, pmh, sizeof(*pmh)) != MMSYSERR_NOERROR;
    }
}

union MidiShortMsg {
    struct
    {
        u8 midiStatus;
        i8 firstByte;
        i8 secondByte;
        i8 unused;
    } msg;
    DWORD dwMsg;
};

BOOL MidiDevice::SendShortMsg(u8 midiStatus, u8 firstByte, u8 secondByte)
{
    MidiShortMsg pkt;

    if (handle == NULL)
    {
        return FALSE;
    }
    else
    {
        pkt.msg.midiStatus = midiStatus;
        pkt.msg.firstByte = firstByte;
        pkt.msg.secondByte = secondByte;
        return midiOutShortMsg(handle, pkt.dwMsg) != MMSYSERR_NOERROR;
    }
}

MidiTimer::MidiTimer()
{
    timeGetDevCaps(&this->timeCaps, sizeof(TIMECAPS));
    this->timerId = 0;
}

MidiTimer::~MidiTimer()
{
    StopTimerImpl();
    timeEndPeriod(this->timeCaps.wPeriodMin);
}

UINT MidiTimer::StartTimerImpl(u32 delay, LPTIMECALLBACK cb, DWORD_PTR data)
{
    StopTimerImpl();
    timeBeginPeriod(this->timeCaps.wPeriodMin);

    if (cb != NULL)
    {
        this->timerId = timeSetEvent(delay, this->timeCaps.wPeriodMin, cb, data, TIME_PERIODIC);
    }
    else
    {
        this->timerId = timeSetEvent(delay, this->timeCaps.wPeriodMin, (LPTIMECALLBACK)MidiTimer::DefaultTimerCallback,
                                     (DWORD_PTR)this, TIME_PERIODIC);
    }

    return this->timerId;
}

BOOL MidiTimer::StopTimerImpl()
{
    if (this->timerId != 0)
    {
        timeKillEvent(this->timerId);
    }
    timeEndPeriod(this->timeCaps.wPeriodMin);
    this->timerId = 0;
    return TRUE;
}

void CALLBACK MidiTimer::DefaultTimerCallback(u32 uTimerID, u32 uMsg, DWORD_PTR dwUser, DWORD_PTR dw1, DWORD_PTR dw2)
{
    MidiTimer *timer = (MidiTimer *)dwUser;
    timer->OnTimerElapsed();
}

u16 MidiOutput::Ntohs(u16 val)
{
    u8 tmp[2];

    tmp[0] = ((u8 *)&val)[1];
    tmp[1] = ((u8 *)&val)[0];

    return *(const u16 *)(&tmp);
}

u32 MidiOutput::SkipVariableLength(LPBYTE *curTrackDataCursor)
{
    u32 length;
    u8 tmp;

    length = 0;
    do
    {
        tmp = **curTrackDataCursor;
        *curTrackDataCursor = reinterpret_cast<LPBYTE>(
            reinterpret_cast<u32>(*curTrackDataCursor) + 1u);
        length = length * 0x80 + (tmp & 0x7f);
    } while ((tmp & 0x80) != 0);

    return length;
}

MidiOutput::MidiOutput()
{
    this->tracks = NULL;
    this->ticksPerQuarterNote = 0;
    this->microsecondsPerQuarterNote = 0;
    this->numTracks = 0;
    this->noteTranspose = 0;
    this->fadeOutVolumeMultiplier = 0;
    this->fadeOutLastSetVolume = 0;
    this->constructorClearedDword2D0 = 0;
    this->volumeUpdatesSuppressed = 0;
    this->constructorClearedDword2D8 = 0;
    this->resetOnlyFadeState2DC = 0;
    this->fadeOutActive = FALSE;

    for (int i = 0; i < 32; i++)
    {
        this->fileData[i] = NULL;
    }

    for (int i = 0; i < 32; i++)
    {
        TH09_MIDI_NATIVE_HEADER(i) = NULL;
    }

    this->activeFileIndex = -1;
    this->pendingLongMessageHeaderCursor = 0;
}

MidiOutput::~MidiOutput()
{
    StopPlayback();
    ClearTracks();
    for (i32 i = 0; i < 32; i++)
    {
        ReleaseFileData(i);
    }
}

ZunResult MidiOutput::ReadFileData(int idx, LPCSTR path)
{
    if (this->activeFileIndex == idx)
    {
        StopPlayback();
    }

    ReleaseFileData(idx);

    TH09_MIDI_NATIVE_FILE_DATA(idx) = FileSystem::OpenFile((LPSTR)path, NULL, false);
    if (TH09_MIDI_NATIVE_FILE_DATA(idx) == NULL)
    {
        g_GameErrorContext.Log("error : MIDI File \x82\xAA\x93\xC7\x82\xDD\x8D\x9E\x82\xDF\x82\xC8\x82\xA2 %s \rv\r\n", path);
        return ZUN_ERROR;
    }

    return ZUN_SUCCESS;
}

void MidiOutput::ReleaseFileData(int idx)
{
    g_ZunMemory.Free(TH09_MIDI_NATIVE_FILE_DATA(idx));
    TH09_MIDI_NATIVE_FILE_DATA(idx) = NULL;
}

void MidiOutput::ClearTracks()
{
    for (i32 trackIndex = 0; trackIndex < this->numTracks; trackIndex++)
    {
        g_ZunMemory.Free(TH09_MIDI_NATIVE_TRACK(trackIndex).data);
    }

    g_ZunMemory.Free(this->tracks);
    this->tracks = NULL;
    this->numTracks = 0;
}

ZunResult MidiOutput::ParseFile(int fileIdx)
{
    u8 hdrRaw[8];
    u32 trackLength;
    LPBYTE currentCursor, currentCursorTrack, endOfHeaderPointer;
    i32 trackIdx;
    LPBYTE fileData;
    u32 hdrLength;

    ClearTracks();
    currentCursor = TH09_MIDI_NATIVE_FILE_DATA(fileIdx);
    fileData = currentCursor;
    if (currentCursor == NULL)
    {
        return ZUN_ERROR;
    }

    // MIDI input is a file-backed variable-length byte stream. The original
    // i386 cursor additions wrap through a full DWORD; no C++ array-object
    // pointer arithmetic or new file bounds gate is introduced.
    // Read midi header chunk
    // First, read the header len
    memcpy(&hdrRaw, currentCursor, 8);

    // Get a pointer to the end of the header chunk
    currentCursor = reinterpret_cast<LPBYTE>(
        reinterpret_cast<u32>(currentCursor) + sizeof(hdrRaw));
    hdrLength = MidiOutput::Ntohl(*(u32 *)(&hdrRaw[4]));

    endOfHeaderPointer = currentCursor;
    currentCursor = reinterpret_cast<LPBYTE>(
        reinterpret_cast<u32>(currentCursor) + static_cast<u32>(hdrLength));

    // Read the SMF format. Only three values are specified:
    //  0: the file contains a single multi-channel track
    //  1: the file contains one or more simultaneous tracks (or MIDI outputs) of a
    //  sequence
    //  2: the file contains one or more sequentially independent single-track
    //  patterns
    this->fileFormat = MidiOutput::Ntohs(*(u16 *)endOfHeaderPointer);

    // Read the divisions in this track. Note that this doesn't appear to support
    // "negative SMPTE format", which happens when the MSB is set.
    this->ticksPerQuarterNote = MidiOutput::Ntohs(*(u16 *)(reinterpret_cast<LPBYTE>(
        reinterpret_cast<u32>(endOfHeaderPointer) + 4u)));
    // Read the number of tracks in this midi file.
    this->numTracks = MidiOutput::Ntohs(*(u16 *)(reinterpret_cast<LPBYTE>(
        reinterpret_cast<u32>(endOfHeaderPointer) + 2u)));

    // Allocate one runtime state record per track.
    this->tracks = (MidiTrack *)g_ZunMemory.Alloc(sizeof(MidiTrack) * this->numTracks, "midi");
    memset(this->tracks, 0, sizeof(MidiTrack) * this->numTracks);
    for (trackIdx = 0; trackIdx < this->numTracks; trackIdx += 1)
    {
        currentCursorTrack = currentCursor;
        currentCursor = reinterpret_cast<LPBYTE>(
            reinterpret_cast<u32>(currentCursor) + 8u);

        // Read a track (MTrk) chunk.
        //
        // First, read the length of the chunk
        trackLength = MidiOutput::Ntohl(*(u32 *)reinterpret_cast<LPBYTE>(
            reinterpret_cast<u32>(currentCursorTrack) + 4u));
        TH09_MIDI_NATIVE_TRACK(trackIdx).dataSize = trackLength;
        TH09_MIDI_NATIVE_TRACK(trackIdx).data = (LPBYTE)g_ZunMemory.Alloc(trackLength, "midi");
        TH09_MIDI_NATIVE_TRACK(trackIdx).trackPlaying = TRUE;
        memcpy(TH09_MIDI_NATIVE_TRACK(trackIdx).data, currentCursor, trackLength);
        currentCursor = reinterpret_cast<LPBYTE>(
            reinterpret_cast<u32>(currentCursor) + static_cast<u32>(trackLength));
    }
    this->microsecondsPerQuarterNote = 1000000;
    this->activeFileIndex = fileIdx;
    return ZUN_SUCCESS;
}

u32 MidiOutput::Ntohl(u32 val)
{
    u8 tmp[4];

    tmp[0] = ((u8 *)&val)[3];
    tmp[1] = ((u8 *)&val)[2];
    tmp[2] = ((u8 *)&val)[1];
    tmp[3] = ((u8 *)&val)[0];

    return *(const u32 *)tmp;
}

ZunResult MidiOutput::LoadFile(LPCSTR midiPath)
{
    if (ReadFileData(0x1f, midiPath) != ZUN_SUCCESS)
    {
        return ZUN_ERROR;
    }

    ParseFile(0x1f);
    ReleaseFileData(0x1f);

    return ZUN_SUCCESS;
}

void MidiOutput::LoadTracks()
{
    i32 trackIndex;
    MidiTrack *track = this->tracks;

    this->fadeOutVolumeMultiplier = 1.0;
    this->resetOnlyFadeState2DC = 0;
    this->fadeOutActive = FALSE;
    this->elapsedMillisecondsAtCurrentTempo = 0;
    this->elapsedTicksBeforeTempoChange = 0;

    for (trackIndex = 0; trackIndex < this->numTracks; trackIndex++, track++)
    {
        track->cursor = track->data;
        track->loopCursor = track->cursor;
        track->trackPlaying = TRUE;
        track->nextEventTick = MidiOutput::SkipVariableLength(&track->cursor);
    }
}

ZunResult MidiOutput::Play()
{
    if (this->tracks == NULL)
    {
        return ZUN_ERROR;
    }

    LoadTracks();
    this->outputDevice.OpenDevice(MIDI_MAPPER);
    StartTimerImpl(1, NULL, NULL);

    return ZUN_SUCCESS;
}

ZunResult MidiOutput::StopPlayback()
{
    if (this->tracks == NULL)
    {
        return ZUN_ERROR;
    }

    for (i32 i = 0; i < 32; i++)
    {
        if (TH09_MIDI_NATIVE_HEADER(i) != NULL)
        {
            UnprepareHeader(TH09_MIDI_NATIVE_HEADER(i));
        }
    }

    StopTimerImpl();
    this->outputDevice.Close();
    this->activeFileIndex = -1;

    return ZUN_SUCCESS;
}

ZunResult MidiOutput::UnprepareHeader(LPMIDIHDR pmh)
{
    for (i32 i = 0; i < 32; i++)
    {
        if (TH09_MIDI_NATIVE_HEADER(i) == pmh)
        {
            TH09_MIDI_NATIVE_HEADER(i) = NULL;
            goto success;
        }
    }

    return ZUN_ERROR;

success:
    midiOutUnprepareHeader(this->outputDevice.handle, pmh, sizeof(*pmh));

    g_ZunMemory.Free(pmh->lpData);
    g_ZunMemory.Free(pmh);
    return ZUN_SUCCESS;
}

ZunResult MidiOutput::SetFadeOut(u32 ms)
{
    this->fadeOutVolumeMultiplier = 0.0;
    this->fadeOutDurationMs = ms;
    this->fadeOutElapsedMs = 0;
    this->resetOnlyFadeState2DC = 0;
    this->fadeOutActive = TRUE;

    return ZUN_SUCCESS;
}

void MidiOutput::OnTimerElapsed()
{
    BOOL trackLoaded = FALSE;
    ULONGLONG currentPlaybackTick = this->elapsedTicksBeforeTempoChange +
                                    (this->elapsedMillisecondsAtCurrentTempo * this->ticksPerQuarterNote * 1000) /
                                        this->microsecondsPerQuarterNote;
    if (this->fadeOutActive != FALSE)
    {
        if (this->fadeOutElapsedMs < this->fadeOutDurationMs)
        {
            this->fadeOutVolumeMultiplier = 1.0f - (f32)this->fadeOutElapsedMs / (f32)this->fadeOutDurationMs;
            if ((u32)(this->fadeOutVolumeMultiplier * 128.0f) != this->fadeOutLastSetVolume)
            {
                FadeOutSetVolume(0);
            }
            this->fadeOutLastSetVolume = this->fadeOutVolumeMultiplier * 128.0f;
            ++*reinterpret_cast<u32 *>(&this->fadeOutElapsedMs);
        }
        else
        {
            this->fadeOutVolumeMultiplier = 0.0;
            return;
        }
    }
    i32 trackIndex;
    for (trackIndex = 0; trackIndex < this->numTracks; trackIndex++)
    {
        if (TH09_MIDI_NATIVE_TRACK(trackIndex).trackPlaying)
        {
            trackLoaded = TRUE;
            while (TH09_MIDI_NATIVE_TRACK(trackIndex).trackPlaying)
            {
                if (TH09_MIDI_NATIVE_TRACK(trackIndex).nextEventTick <= currentPlaybackTick)
                {
                    ProcessMsg(&TH09_MIDI_NATIVE_TRACK(trackIndex));
                    currentPlaybackTick =
                        this->elapsedTicksBeforeTempoChange +
                        (this->elapsedMillisecondsAtCurrentTempo * this->ticksPerQuarterNote * 1000 /
                         this->microsecondsPerQuarterNote);
                    continue;
                }
                break;
            }
        }
    }
    this->elapsedMillisecondsAtCurrentTempo++;
    if (!trackLoaded)
    {
        LoadTracks();
    }
}

void MidiOutput::ProcessMsg(MidiTrack *track)
{
    i32 adjustedChannelVolume;
    i32 eventDataLength, nextEventDeltaTicks;
    MidiTrack *loopResetTrack;
    MidiTrack *loopCheckpointTrack;
    u8 eventData1, eventData2;
    u8 statusByte, messageType, channel;
    u8 metaEventType;
    LPMIDIHDR longMessageHeader;
    i32 index;
    i32 beatsPerMinute;

    // MIDI events and variable-length meta chunks are file-backed byte
    // addresses. Preserve native i386 DWORD cursor wrap with no new
    // file bounds gate or extra read of the original track stream.
    statusByte = *track->cursor;
    if (statusByte < MIDI_OPCODE_NOTE_OFF)
    {
        statusByte = track->runningStatus;
    }
    else
    {
        track->cursor = reinterpret_cast<LPBYTE>(
            reinterpret_cast<u32>(track->cursor) + 1u);
    }
    messageType = statusByte & 0xf0;
    channel = statusByte & 0x0f;

    switch (messageType)
    {
    case MIDI_OPCODE_SYSTEM_EXCLUSIVE:
        if (statusByte == MIDI_OPCODE_SYSTEM_EXCLUSIVE)
        {
            if (TH09_MIDI_NATIVE_HEADER(this->pendingLongMessageHeaderCursor) != NULL)
            {
                UnprepareHeader(TH09_MIDI_NATIVE_HEADER(this->pendingLongMessageHeaderCursor));
            }

            longMessageHeader = TH09_MIDI_NATIVE_HEADER(this->pendingLongMessageHeaderCursor) =
                (LPMIDIHDR)g_ZunMemory.Alloc(sizeof(MIDIHDR), "midiHDR");
            eventDataLength = MidiOutput::SkipVariableLength(&track->cursor);
            memset(longMessageHeader, 0, sizeof(MIDIHDR));
            longMessageHeader->lpData = (LPSTR)g_ZunMemory.Alloc(eventDataLength + 1, "midiHDR->lpData");
            longMessageHeader->lpData[0] = MIDI_OPCODE_SYSTEM_EXCLUSIVE;
            longMessageHeader->dwFlags = 0;
            longMessageHeader->dwBufferLength = eventDataLength + 1;
            for (index = 0; index < eventDataLength; index++)
            {
                TH09_MIDI_NATIVE_SYSEX_BYTE(index) = *track->cursor;
                track->cursor = reinterpret_cast<LPBYTE>(
                    reinterpret_cast<u32>(track->cursor) + 1u);
            }
            if (this->outputDevice.SendLongMsg(longMessageHeader))
            {
                g_ZunMemory.Free(longMessageHeader->lpData);
                g_ZunMemory.Free(longMessageHeader);
                TH09_MIDI_NATIVE_HEADER(this->pendingLongMessageHeaderCursor) = NULL;
            }
            // Retail advances the signed 32-slot MIDI long-header cursor
            // with a native i386 DWORD before applying modulo 32.
            ++*reinterpret_cast<u32 *>(&this->pendingLongMessageHeaderCursor);
            this->pendingLongMessageHeaderCursor = this->pendingLongMessageHeaderCursor % 32;
        }
        else if (statusByte == MIDI_OPCODE_SYSTEM_RESET)
        {
            // Meta-Event. In a MIDI file, SYSTEM_RESET gets reused as a
            // sort of escape code to introducde its own meta-events system,
            // which are events that make sense in the context of a MIDI
            // file, but not in the context of the MIDI protocol itself.
            metaEventType = *track->cursor;
            track->cursor = reinterpret_cast<LPBYTE>(
            reinterpret_cast<u32>(track->cursor) + 1u);
            eventDataLength = MidiOutput::SkipVariableLength(&track->cursor);
            // End of Track meta-event.
            if (metaEventType == MIDI_META_END_OF_TRACK)
            {
                track->trackPlaying = 0;
                return;
            }
            // Set Tempo meta-event.
            if (metaEventType == MIDI_META_SET_TEMPO)
            {
                this->elapsedTicksBeforeTempoChange +=
                    (this->elapsedMillisecondsAtCurrentTempo * this->ticksPerQuarterNote * 1000 /
                     this->microsecondsPerQuarterNote);
                this->elapsedMillisecondsAtCurrentTempo = 0;
                this->microsecondsPerQuarterNote = 0;
                for (index = 0; index < eventDataLength; index += 1)
                {
                    *reinterpret_cast<u32 *>(&this->microsecondsPerQuarterNote) +=
                        static_cast<u32>(this->microsecondsPerQuarterNote) * 0x100u +
                        static_cast<u32>(*track->cursor);
                    track->cursor = reinterpret_cast<LPBYTE>(
            reinterpret_cast<u32>(track->cursor) + 1u);
                }
                beatsPerMinute = 60000000 / this->microsecondsPerQuarterNote;
                break;
            }
            track->cursor = reinterpret_cast<LPBYTE>(
                reinterpret_cast<u32>(track->cursor) +
                static_cast<u32>(eventDataLength));
        }
        break;
    case MIDI_OPCODE_NOTE_OFF:
    case MIDI_OPCODE_NOTE_ON:
    case MIDI_OPCODE_POLYPHONIC_AFTERTOUCH:
    case MIDI_OPCODE_CONTROL_CHANGE:
    case MIDI_OPCODE_PITCH_BEND_CHANGE:
        eventData1 = *track->cursor;
        track->cursor = reinterpret_cast<LPBYTE>(
            reinterpret_cast<u32>(track->cursor) + 1u);
        eventData2 = *track->cursor;
        track->cursor = reinterpret_cast<LPBYTE>(
            reinterpret_cast<u32>(track->cursor) + 1u);
        break;
    case MIDI_OPCODE_PROGRAM_CHANGE:
    case MIDI_OPCODE_CHANNEL_AFTERTOUCH:
        eventData1 = *track->cursor;
        track->cursor = reinterpret_cast<LPBYTE>(
            reinterpret_cast<u32>(track->cursor) + 1u);
        eventData2 = 0;
        break;
    }
    switch (messageType)
    {
    case MIDI_OPCODE_NOTE_ON:
        if (eventData2 != 0)
        {
            eventData1 += this->noteTranspose;
            TH09_MIDI_NATIVE_CHANNEL(channel).keyPressedFlags[eventData1 >> 3] |= (u8)(1 << (eventData1 & 7));
            break;
        }
    case MIDI_OPCODE_NOTE_OFF:
        eventData1 += this->noteTranspose;
        TH09_MIDI_NATIVE_CHANNEL(channel).keyPressedFlags[eventData1 >> 3] &= (u8)(~(1 << (eventData1 & 7)));
        break;
    case MIDI_OPCODE_PROGRAM_CHANGE:
        // Program Change
        TH09_MIDI_NATIVE_CHANNEL(channel).instrument = eventData1;
        break;
    case MIDI_OPCODE_CONTROL_CHANGE:
        switch (eventData1)
        {
        case MIDI_CONTROLLER_BANK_SELECT:
            // Bank Select
            TH09_MIDI_NATIVE_CHANNEL(channel).instrumentBank = eventData2;
            break;
        case MIDI_CONTROLLER_CHANNEL_VOLUME:
            // Channel Volume
            TH09_MIDI_NATIVE_CHANNEL(channel).channelVolume = eventData2;
            adjustedChannelVolume = (f32)eventData2 * this->fadeOutVolumeMultiplier;
            if (adjustedChannelVolume < 0)
            {
                adjustedChannelVolume = 0;
            }
            else if (0x7f < adjustedChannelVolume)
            {
                adjustedChannelVolume = 0x7f;
            }
            eventData2 = TH09_MIDI_NATIVE_CHANNEL(channel).modifiedVolume = adjustedChannelVolume;
            break;
        case MIDI_CONTROLLER_EFFECT_ONE_DEPTH:
            // Effects 1 Depth
            TH09_MIDI_NATIVE_CHANNEL(channel).effectOneDepth = eventData2;
            break;
        case MIDI_CONTROLLER_EFFECT_THREE_DEPTH:
            // Effects 3 Depth
            TH09_MIDI_NATIVE_CHANNEL(channel).effectThreeDepth = eventData2;
            break;
        case MIDI_CONTROLLER_PAN:
            // Pan
            TH09_MIDI_NATIVE_CHANNEL(channel).pan = eventData2;
            break;
        // Both loop markers traverse the real runtime-counted 0x20-byte
        // MIDI track records, keeping the live pointer and opcode scheduling.
        // A raw native byte address does not assume a separate C++ array for
        // a file-provided track count.
        case MIDI_CONTROLLER_LOOP_START:
            // TH08 repurposes breath control as the MIDI loop checkpoint.
            for (loopCheckpointTrack = &this->tracks[0], index = 0;
                 index < this->numTracks;
                 index += 1,
                 loopCheckpointTrack = reinterpret_cast<MidiTrack *>(
                     reinterpret_cast<u32>(loopCheckpointTrack) + sizeof(*loopCheckpointTrack)))
            {
                loopCheckpointTrack->loopCursor = loopCheckpointTrack->cursor;
                loopCheckpointTrack->loopNextEventTick = loopCheckpointTrack->nextEventTick;
            }
            this->tempoAtLoopPoint = this->microsecondsPerQuarterNote;
            this->elapsedMillisecondsAtLoopPoint = this->elapsedMillisecondsAtCurrentTempo;
            this->elapsedTicksAtLoopPoint = this->elapsedTicksBeforeTempoChange;
            break;
        case MIDI_CONTROLLER_LOOP_END:
            // TH08 repurposes the foot controller as the loop jump.
            for (loopResetTrack = &this->tracks[0], index = 0;
                 index < this->numTracks;
                 index += 1,
                 loopResetTrack = reinterpret_cast<MidiTrack *>(
                     reinterpret_cast<u32>(loopResetTrack) + sizeof(*loopResetTrack)))
            {
                loopResetTrack->cursor = (u8 *)loopResetTrack->loopCursor;
                loopResetTrack->nextEventTick = loopResetTrack->loopNextEventTick;
            }
            this->microsecondsPerQuarterNote = this->tempoAtLoopPoint;
            this->elapsedMillisecondsAtCurrentTempo = this->elapsedMillisecondsAtLoopPoint;
            this->elapsedTicksBeforeTempoChange = this->elapsedTicksAtLoopPoint;
        }
        break;
    }
    if (statusByte < MIDI_OPCODE_SYSTEM_EXCLUSIVE)
    {
        this->outputDevice.SendShortMsg(statusByte, eventData1, eventData2);
    }
    track->runningStatus = statusByte;
    nextEventDeltaTicks = MidiOutput::SkipVariableLength(&track->cursor);
    track->nextEventTick = static_cast<i32>(
        static_cast<u32>(track->nextEventTick) +
        static_cast<u32>(nextEventDeltaTicks));
}

void MidiOutput::FadeOutSetVolume(i32 volumeOffset)
{
    i32 volumeClamped;
    u32 volumeByte;
    i32 channelIndex;
    i32 controllerNumber;
    u32 statusByte;

    if (this->volumeUpdatesSuppressed != 0)
    {
        return;
    }
    controllerNumber = MIDI_CONTROLLER_CHANNEL_VOLUME;
    for (channelIndex = 0; channelIndex < 16; channelIndex += 1)
    {
        statusByte = (u8)(channelIndex + MIDI_OPCODE_CONTROL_CHANGE);
        volumeClamped =
            (i32)(TH09_MIDI_NATIVE_CHANNEL(channelIndex).channelVolume * this->fadeOutVolumeMultiplier) + volumeOffset;
        if (volumeClamped < 0)
        {
            volumeClamped = 0;
        }
        else if (volumeClamped > 127)
        {
            volumeClamped = 127;
        }
        volumeByte = (u8)volumeClamped;
        this->outputDevice.SendShortMsg(statusByte, controllerNumber, volumeByte);
    }
    return;
}

void DummyMidiTimer::OnTimerElapsed()
{
    QueryPerformanceCounter(&g_DummyMidiTimerPerformanceCounter);
}

void MidiTimer::StartTimer()
{
    StartTimerImpl(16, NULL, NULL);
}

void MidiTimer::StopTimer()
{
    StopTimerImpl();
}

#undef TH09_MIDI_NATIVE_FILE_DATA
#undef TH09_MIDI_NATIVE_HEADER
#undef TH09_MIDI_NATIVE_TRACK
#undef TH09_MIDI_NATIVE_CHANNEL
#undef TH09_MIDI_NATIVE_SYSEX_BYTE
