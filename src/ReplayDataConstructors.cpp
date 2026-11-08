#include "ReplayDataOriginalView.hpp"

// The native payload constructor initializes its configuration subobject at
// payload +0x1C through the ordinary GameConfiguration constructor. Spell the
// member initialization explicitly so the maintained object relationship is
// visible here; this does not claim that retail authored either constructor.
ReplayPayloadView::ReplayPayloadView()
    : configSnapshot()
{
}

// ReplayDataView's second native subobject begins at +0xC0. Its constructor
// delegates to the payload constructor with that exact base receiver; the
// header subobject remains the ordinary implicit trivial initialization.
ReplayDataView::ReplayDataView()
{
}
