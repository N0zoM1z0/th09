#include "SoundPlayer.hpp"

void SoundPlayer::ResumeAfterPause()
{
    QueueCommand(7, 0, "UnPause");
}
