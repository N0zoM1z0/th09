#include "Midi.hpp"

void MidiPlaybackPathView::PlayPath(const char *path)
{
    this->StopPlayback();
    this->LoadFile(path);
    this->Play();
}
