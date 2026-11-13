#pragma once

// FrontFactory owns exactly two 0x64-byte panel strips of five actual
// 0x14-byte FrontPanelVertexView elements, followed by its independent
// side color and timer DWORDs. Keep the live Win32 base + native strides.
#define FRONT_NATIVE_VERTEX(front, side, vertex) \
    (*reinterpret_cast<FrontPanelVertexView *>(reinterpret_cast<unsigned int>(&(front)->panelVerticesE86C[0][0]) + \
        static_cast<unsigned int>(side) * sizeof((front)->panelVerticesE86C[0]) + \
        static_cast<unsigned int>(vertex) * sizeof((front)->panelVerticesE86C[0][0])))
