#include "ReplayDataOriginalView.hpp"

// The original bodies call the configuration constructor at payload +0x1C,
// then the payload constructor at record +0xC0. Original authored versus
// compiler-generated origin remains unknown; neither has authored exact credit.
ReplayPayloadView::ReplayPayloadView()
{
}

ReplayDataView::ReplayDataView()
{
}
