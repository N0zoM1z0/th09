# TH09 native Player owner Reset: the old handle-release call was actually AddScore

Target: independently SHA-256 verified Japanese TH09 v1.50a executable
`10350095bcf95edb59e03bee9849a2dc8a7714b4927ad5909c569c550fce6822`.
This recovers natural C++ receiver and state semantics, not new exact credit,
original class spelling certainty, runtime game acceptance or product linking.

## Correct an earlier wrong semantic inference from actual original call targets

The exact **41-byte `Reset` at 0x0041D7E0** previously appeared in the
maintained source as `PlayerAddedOwnerStateView::Reset` on an independent
0x44-byte prefix. It called `PlayerAddedOwnerPrepareReset` at
**0x0040F8C0**, then `PlayerAddedHandleManagerView::ReleaseHandle` at
**0x0040F860** through a fabricated pointer chain before zeroing two
pointer-looking slots and resetting `timer14`.

The target **0x0040F860** is not a handle-release method. It is the
independently exact **33-byte `AsciiGameManagerSideView::AddScore`**.
The `owner00` of this state points to the native `PlayerLifecycleView`;
its +0x0C pointer is the real `AsciiGameManagerSideView *sideState`.
The apparent handles at state +0x0C/+0x10 are the *actual* reward
`value0C` and `maxValue10` integer counters. Source originally called
`AddScore(maxValue10)` to publish the value through the side and then
zeroed these counters. The old handle-release explanation was a false
semantic inference; it is explicitly superseded here.

The native full `PlayerOwnerStateView` now owns both `PrepareReset`
(**29 exact bytes**) and `Reset` (**41 exact bytes**). Its natural source
calls `PrepareReset()`, then the genuine
`playerOwner00->sideState->AddScore(maxValue10)`, then sets `maxValue10`,
`value0C` and `timer14` to zero. The complete `PlayerLifecycleView::
InitializeAddedState` at 0x0041EBC0 uses the already-embedded actual
`ownerState30410.Reset()` rather than an independently sized overlay.
This restores the original state lifetime and side-score link without
inventing a second Player/GameManager allocation.

## Complete original PE, COFF and big initializer proof

Four natural pinned VC7.1 original-profile experiments (direct native
state call, explicit `this`, direct native main Player owner and a local
side-pointer alias) all reproduced the exact complete **41-byte Reset**
and **572-byte physical `InitializeAddedState`** (552 function code plus
20 bytes of original jump data). The Reset retains three original fields,
including `PrepareReset @0x0040F8C0`, `AddScore @0x0040F860`; the large
initializer retains **25 original fields** and its +0x16A call to
`Reset @0x0041D7E0`. Immutable original PE operands independently prove
both corrected original call identities, and the six initializer
compiler-private `$L` names were rebound only after checking the real
same-section COFF symbol definitions and original physical target VAs.

A complete scoped original-bound replay of AddedState, owner reset,
PlayerRuntime, Reward, all sixteen callback leaves, FrontResult,
EclManager exact siblings and Player collision helpers passes
**132 preexisting exact functions, 40,858 physical bytes and 1,937
relocation fields**. Changing the native Player type also shifted two
ECL, eleven PlayerRuntime and six reward private names; all were
separately checked against immutable original PE word and actual owner
section before rebinding. None changes an original call target.

The still-open **3,900-byte Enemy OnUpdate** retains 97 original fields,
69 calls and 13 unlike bytes; **862-byte Player CheckBulletCollision**
retains 849 candidate bytes, 26 fields, twelve calls and 13 missing bytes;
**15,564-byte RunEcl** retains 598 fields and 30 unlike ordinary bytes.
The checkpoint does not turn any of these into byte-exact functions.
All focused receipts are retained locally in
`.analysis/gpt-web-owner-reset-real-score-20261108/`.

No all-project cold replay, final Windows game link, runtime semantic
acceptance, Factory receipt or remote push is claimed. The global ledger
remains **1,034 source-present / 1,021 exact / 13 nonexact**.
