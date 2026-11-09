# Player death owner uses actual Front transition gate (2026-11-09)

The original TH09 v1.50a `PlayerLifecycleView::EnterDeathState` at
`0x41E420` was already physically exact but still declared an independent
`PlayerDeathSharedView` receiver for a seven-byte block check. That old
facade has now been removed. The complete Player death routine reads the
real `g_GameManager.frontFactoryA8` pointer and calls the constructed
`FrontFactoryConstructView::GetTransitionBlockFlag()` member.

The original PE at owner-relative +0x09 encodes the absolute address
**0x4A7E38**, identical to `g_GameManager` at 0x4A7D90 plus the real
Front pointer field at +0xA8. The REL32 field at +0x0E encodes call
**0x40D4F0**. Both original operand bytes were checked before the two
source-symbol records were changed; no call or data destination moved.
The previously recovered 222-byte `SelectOpponentSide` member and
Panel colors/timers writes already use that same real Front allocation.

Pinned VC7.1 recompilation of the *complete* 753-byte function matches
**753/753 original physical bytes and all 38/38 relocated fields**, with
no compiler-profile changes. Six targeted Player-death and shared-runtime
tests pass. This is a meaningful C++ owner correction to an already-exact
function; the 1,024 authored exact count does not increase. No full cold
replay, Windows product link or runtime semantic closure was performed.
