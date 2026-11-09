#include "NativeVmGroupConstructors.hpp"

// The original constructors use VC7.1's real EH vector-construction helper.
// Let ordinary C++ member construction generate it; never call it manually.
NativeVmSevenPlusOneView::NativeVmSevenPlusOneView()
{
}

NativeVmFivePlusOneView::NativeVmFivePlusOneView()
{
}

NativeVmThreeView::NativeVmThreeView()
{
}
