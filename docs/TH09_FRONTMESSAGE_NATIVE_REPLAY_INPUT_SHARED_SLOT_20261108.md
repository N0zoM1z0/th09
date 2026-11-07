# TH09 FrontMessage shares the third native ReplayInputState, not a new input object

Original target: independently verified Japanese TH09 v1.50a executable,
SHA-256 `10350095bcf95edb59e03bee9849a2dc8a7714b4927ad5909c569c550fce6822`.
This is an ordinary C++ global-storage and callsite reconstruction,
not newly credited byte-exact coverage, a final game product or a semantic
acceptance receipt.

## A separate input global was actually a member of one three-slot array

The full **2,852-byte FrontMessage script** (0x00416590, 2,968 bytes
including its physical switch table) previously declared another
`FrontMessageInputView g_FrontMessageInput` global, at the original
record address **0x004ACF34**. The real Replay/Controller code maintains
`ReplayInputState g_ReplayInputStates[3]` at **0x004ACE18** with an exact
**0x8E-byte stride**. The original address is precisely
`0x004ACE18 + 2 × 0x8E = 0x004ACF34`: the input controlling FrontMessage
skip/confirmation is the existing *third shared input record*.

The FrontMessage source now uses
`g_ReplayInputStates[ReplayNativeInputGateCapacity::SharedInputStateIndex]`,
where the capacity derives `SharedInputStateIndex == StateCount - 1 == 2`
from the real three-slot native array. It invokes that object's genuine
`ReplayInputState::IsHeld` four times and `WasPressed` once, with the same
input masks and scripting/clock control flow. The fake input class and its
separately named global declaration are removed. This also shares the
identical physical input state used by Replay playback/recording,
networking and Player movement. No extra allocation, input state copy,
changed button mask or new source-side input queue was introduced.

## Original PE addresses are authoritative, not unresolved COFF immediates

Two copy-on-write natural VC7.1 C++ models, one with literal index 2 and
one with the source-native derived constant, preserved the exact physical
function extent (**2,968 bytes**) and all **167 field offsets/types**.
The unresolved COFF byte stream changed at ten bytes because the five
global pointer operands changed from a standalone symbol at `0x4ACF34`
with addend zero to the *actual* `g_ReplayInputStates` symbol at
`0x4ACE18` with genuine COFF addend `+0x11C`. An independent existing
original-bound Replay owner confirms that exact base symbol and target.

Every old and new relocation was separately bound to the immutable PE:
**all five effective addresses remain 0x004ACF34**, and the four
`IsHeld` calls still target **0x00415CB0**, while `WasPressed` still
targets **0x00415CD0**. Both 18-byte leaf methods are separately
original-exact. The source change additionally renumbered 30
compiler-private `$L`/`$run_scripts$` symbols; their real same-owner
COFF definition section, offset and immutable original PE destination
were individually checked before updating *only* their names in the
strict original-bound manifest.

After resolving all relocation fields, the complete original-bound
comparator again proves **2,852/2,852 code bytes and 2,968/2,968 physical
bytes, all 167 fields**. The focused replay-source cold rebuild passes
**16 existing exact functions / 7,136 complete physical bytes / 399
original-bound fields**. No other original function or link symbol was
altered, and no new exactness was credited.

Bounded original PE/COFF receipts and natural source trials:
`.analysis/gpt-web-front-native-replay-input-20261108/`.
There was no all-project cold replay, Windows executable product link,
runtime gameplay acceptance, Factory exactness receipt or remote push.
The global ledger remains **1,034 source-present / 1,021 exact / 13
nonexact**.
