#pragma once

#include "Float3.hpp"
#include <stddef.h>

// Native 0x14-byte untextured D3DFVF_XYZRHW | D3DFVF_DIFFUSE vertex.
// The ScreenEffect DrawSquare owner observes its constructor and names the
// homogeneous reciprocal coordinate `w`; Effect/ANM draw owners call it `rhw`.
// These are the same physical +0x0C float and one nominal C++ type.
struct VertexDiffuseXyzrhw
{
    VertexDiffuseXyzrhw();
    Float3 pos;
    union { float w; float rhw; };
    unsigned long diffuse;
};

typedef char VertexDiffuseNativeSizeIs14[
    (sizeof(VertexDiffuseXyzrhw) == 0x14 &&
     offsetof(VertexDiffuseXyzrhw, pos) == 0 &&
     offsetof(VertexDiffuseXyzrhw, w) == 0x0C &&
     offsetof(VertexDiffuseXyzrhw, rhw) == 0x0C &&
     offsetof(VertexDiffuseXyzrhw, diffuse) == 0x10) ? 1 : -1];
