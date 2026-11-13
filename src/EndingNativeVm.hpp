#pragma once

// Both the ending-script interpreter and its resource/VM creation callback
// operate on this exact, real 16-element 0x2A4-byte AnmVm allocation.
// Preserve the live Win32 DWORD byte-stride lookup and no extra bounds gate.
#define TH09_ENDING_NATIVE_VM(owner, index) \
    (*reinterpret_cast<AnmVm *>( \
        reinterpret_cast<unsigned int>(&(owner)->endingVms[0]) + \
        static_cast<unsigned int>(index) * sizeof((owner)->endingVms[0])))
