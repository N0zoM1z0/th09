#include "ScoreFileView.hpp"

void __fastcall ReleaseScoreFileBuffer(void *scoreFile);

int InitializeScoreDataStartup()
{
    ScoreFileView *scoreFile = ScoreFileView::OpenScore("score.dat");
    g_ScoreFileHeader = *scoreFile;
    scoreFile->LoadScoreRecords();
    scoreFile->LoadLastName();
    scoreFile->LoadPlayStats();
    ReleaseScoreFileBuffer(scoreFile);
    return 0;
}
