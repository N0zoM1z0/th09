#include "Midi.hpp"

void __fastcall EmitMidiPlayFileByIndex(MidiOutput *midiOutput, int index)
{
    midiOutput->PlayFile(index);
}

struct MidiPlaybackPathView : MidiOutput
{
    void PlayPath(const char *path);
};

void MidiPlaybackPathView::PlayPath(const char *path)
{
    this->StopPlayback();
    this->LoadFile(path);
    this->Play();
}
