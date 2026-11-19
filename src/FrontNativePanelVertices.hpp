#pragma once

// The native FrontFactory allocation contains two five-vertex panels, each
// vertex occupying 0x14 bytes. The actual [side][vertex] array is shared by
// the Front creation, calculation and draw callbacks; no copied view or
// bounds check is added here.
#define FRONT_NATIVE_VERTEX(front, side, vertex) \
    ((front)->panelVerticesE86C[(side)][(vertex)])
