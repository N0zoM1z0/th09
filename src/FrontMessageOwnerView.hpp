#pragma once

// Original source data pointer for Front message initialization.
// The actual runtime owner is FrontFactoryConstructView, not a short
// separately instantiated 0x11EAC-byte shadow Front object.
struct FrontMessageInitSourceView
{
    unsigned char unknown00[0x06];
    short messageIndex06;
};
