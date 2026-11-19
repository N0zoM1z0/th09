#pragma once

// The Ending owner constructs a native sixteen-element array of 0x2A4-byte
// AnmVm records. Keep the live index and the original repeated field reads.
#define TH09_ENDING_NATIVE_VM(owner, index) ((owner)->endingVms[(index)])
