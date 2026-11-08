# TH09 Player full native 0x44-byte owner state across reward and runtime

Target: independently verified Japanese TH09 1.50a executable, SHA-256
`10350095bcf95edb59e03bee9849a2dc8a7714b4927ad5909c569c550fce6822`.
This is source-level native C++ layout and receiver reconstruction, not
newly earned byte-exact credit or a whole-game runtime acceptance receipt.

## One actual owner state, not two identical-sized C++ allocations

The Player embeds a `PlayerOwnerStateView ownerState30410` at original
Player +0x30410. The complete exact **1,519-byte `ApplyReward` source
function** at 0x0041D150 previously cast that `this` pointer to a
separately declared *another* 0x44-byte `PlayerOwnerRewardStateView`.
That duplicate mapped reward counters, the two 0x0C-byte ZunTimers,
callback and owner pointer, while the native Player view kept most of
these same fields opaque. That source separation was misleading.

The real `PlayerOwnerStateView` now owns all 0x44 bytes with explicit
original offsets: native Player pointer +0x00, state +0x04, maximum
state +0x08, current/maximum counters +0x0C/+0x10, actual constructed
ZunTimers +0x14/+0x20, meter +0x2C, spawn counter +0x30, reward count
+0x34, transition +0x38, frame counter +0x3C and the original callback
at +0x40. The typed pointer/callback aliases share their preexisting
four-byte slots; the class size and constructor sequence are unchanged.

`ApplyReward` now uses an ordinary `PlayerOwnerStateView *state = this`
and the real `playerOwner00` and `rewardCallback40` members. It no longer
casts its receiver to a second independently sized object. The original
side-score, RNG, Supervisor, ASCIIPopup, effect scheduling and callback
paths are unchanged. The 553-byte exact FrontResult bonus independently
consumes the same `maxState08` at Player +0x30418.

## Complete code/table original proof

A copy-on-write fixed-profile VC7.1 compiler experiment confirmed that
eliminating the reward-only C++ class preserved **all 1,540 physical
original bytes and all 91 COFF field positions/types/addends**. The
canonical complete target-byte comparator agrees, including the
21-byte post-return jump data. This is not a nonexact partial match.

The shared class declaration renumbered six compiler-private `$L`
labels in the full reward owner, 17 in PlayerRuntime/AddedState
caller carriers, and two in the ECL exact sibling carriers. For each, the real same-section COFF definition,
owner-relative offset and immutable original PE operand were checked
before rebinding only the label name. No call destination, original
field operand, compiler profile or comparator was loosened.

An expanded scope-only source-cold replay also covered real Player
runtime and added-state methods, 16 callback owners, FrontResult,
ECL's exact siblings and Player collision leaves: **131 complete
previously exact functions, 40,829 physical bytes, 1,936 relocation
fields** all retain their full original matches.

The separately nonexact 862/849-byte CheckBulletCollision boss still has
26 original-bound fields, 12 calls and 13 missing bytes, and the 3,900-byte
Enemy OnUpdate still differs at 13 ordinary bytes. No nonexact boss was
promoted by layout recovery. Compiler/original proof packets are kept in
`.analysis/gpt-web-native-full-player-owner-state-20261108/`.

No all-project cold replay, Windows executable link, Factory receipt,
runtime semantic acceptance or remote push is asserted. Global ledger
remains **1,034 source-present / 1,021 exact / 13 nonexact**.
