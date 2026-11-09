# FrontSide auxiliary transition uses the constructed HUD owner

The original TH09 v1.50a `BeginAuxTransition` at `0x415D50` has a complete
24-byte body. Its receiver is the same `FrontSide` allocation constructed
by the original 307-byte FrontSide constructor and registered by gameplay
at side +0x18. The former `FrontSideAuxTransitionView` was an independent
short shadow containing two fields already owned by `FrontSide`.

The source now implements `FrontSide::BeginAuxTransition()`: it sets the
actual `auxA678.unknown554` at +0xABCC to 1 and assigns zero to the
native `transitionTimerABD0` at +0xABD0. The complete **24/24 bytes and
one timer-assignment relocation** match the original pinned VC7.1 output.

The original 1,491-byte Front calc callback now invokes this real member
on both side +0x18 FrontSide receivers. Its two original REL32 operands
at +0x523 and +0x52E independently resolve to `0x415D50`; after changing
the C++ callee identity, **1,491/1,491 bytes and 107/107 relocations**
still match the original. The affected source consumers include the
3,094-byte FrontSide draw and 2,968-byte message Update; 32 complete
exact owners across those touched units stayed exact. All 30 private
message-switch COFF label changes were checked against actual same-section
symbols and identical original PE target addresses. The 307-byte native
FrontSide construction proof and targeted HUD/side tests also pass.

This improves original source ownership without adding new distinct exact
functions or running project-wide cold replay.
