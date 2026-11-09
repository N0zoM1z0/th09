#include "FrontSide.hpp"

// The original TH09 constructor is a separate /Ob0 carrier. VC7.1 emits
// EH vector-constructor calls for all the native AnmVm member arrays here;
// compiling these same definitions in FrontSide.cpp's /Ob1 carrier instead
// expands the arrays into loops and changes the two complete target bodies.
FrontSideAuxView::FrontSideAuxView()
{
}

FrontSide::FrontSide()
{
}
