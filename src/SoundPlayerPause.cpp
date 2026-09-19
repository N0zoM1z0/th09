struct AsciiSoundPlayerView
{
    void QueueCommand(int opcode, int argument, char *path);
    void ResumeAfterPause();
};

void AsciiSoundPlayerView::ResumeAfterPause()
{
    QueueCommand(7, 0, "unpause");
}
