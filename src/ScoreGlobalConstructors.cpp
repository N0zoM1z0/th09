#include "ScoreFileView.hpp"
#include <string.h>

ScoreFileView::ScoreFileView()
{
    memset(this, 0, sizeof(*this));
}

struct ScoreRecordCtorView
{
    unsigned char storage[0x2C];
    ScoreRecordCtorView();
};

ScoreRecordCtorView::ScoreRecordCtorView()
{
    memset(this, 0, sizeof(*this));
}
