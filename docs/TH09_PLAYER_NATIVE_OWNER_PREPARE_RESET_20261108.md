# TH09 native Player owner PrepareReset and exact 41-byte AddedState caller

Original retail target: verified Japanese TH09 v1.50a PE SHA-256
`10350095bcf95edb59e03bee9849a2dc8a7714b4927ad5909c569c550fce6822`.
This checkpoint is C++ receiver/ABI and physical owner reconstruction,
not a new complete byte-exact function, a proven historic class name or
runtime acceptance of the whole game.

## Actual PlayerOwnerStateView::PrepareReset at 0x40F8C0

The original **29-byte exact `PrepareReset` leaf at 0x0040F8C0** clears
three fields on the genuine 0x44-byte Player owner state embedded at
Player +0x30410: timer20 at state +0x20, state04 at +0x04, and state38
at +0x38. Its maintained source previously declared another independent
`PlayerOwnerStateResetView` with 0x44 bytes of padded fields. The exact
**41-byte `PlayerAddedOwnerStateView::Reset` at 0x0041D7E0** called the
same entry through a fictitious global
`PlayerAddedOwnerPrepareReset()` symbol, although its real this pointer
is the native owner state receiver.

The actual `PlayerOwnerStateView` now declares and defines `PrepareReset`
with no extra storage, altered calling convention or extra temporary
object. The 41-byte AddedState reset now naturally calls
`reinterpret_cast<PlayerOwnerStateView *>(this)->PrepareReset()`, the
real thiscall target at **0x0040F8C0**. Its subsequent native handle
release/field clears and timer assignment remain in the original order.
The broader 572-byte physical `PlayerLifecycleView::InitializeAddedState`
continues to reach the exact 41-byte reset on the real embedded owner.
The local AddedState reset/source-specific handle role remains a
separately reviewed overlay until its constructor/lifetime is established;
this change does not silently invent a new owning allocation.

## Original compiler and whole-boss comparison

Three ordinary VC7.1 `/O2 /Ob1` compile experiments tried an explicit
native receiver, a native reference, and a native pointer alias. All
three reproduced **the original 29/29-byte leaf (one timer-call field)**
and **41/41-byte caller (three fields)**. Only the C++ symbol spelling
at caller offset +0x04 changed; its immutable original REL32 target
remained 0x0040F8C0. No register forcing, asm, volatile or modified
compiler profile was used.

The scoped complete-original PE/COFF cold replay across AddedState,
PlayerOwnerStateReset, PlayerRuntime, complete 1,540-byte reward state,
FrontResult and ECL/Player collision siblings passes **116 preexisting
exact full functions / 39,254 physical bytes / 1,858 relocation fields**.
The native header renumbered two ECL, 17 PlayerRuntime/AddedState, and
six reward compiler-private `$L` labels. Each was verified against its
actual same-section COFF symbol definition, immutable original PE
operand/target and complete physical owner before only the local name
was rebound in the strict match manifest.

The separately nonexact **3,900-byte Enemy OnUpdate** still has 97
original fields, 69 calls and 13 unlike ordinary bytes; the **862-byte
Player CheckBulletCollision** retains an 849-byte candidate, 26 fields,
12 calls and 13 missing bytes. Neither is promoted. Independent pinned
original receipts and natural trials:
`.analysis/gpt-web-native-owner-reset-20261108/`.

No full cold replay, Windows product link, runtime acceptance, Factory
receipt or remote push is claimed. Global status remains **1,034
source-present / 1,021 exact / 13 nonexact**.
