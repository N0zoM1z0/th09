#include "Midi.hpp"

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
