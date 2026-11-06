# TH09 shared Player runtime: native blocker state and replay caller

The original Japanese v1.50a shared Player runtime exposes an `IsBlocked()`
method called by both Player update/selector owners and Replay playback. Its
exact 24-byte leaf reads the signed state at runtime +`0xE94C` and returns
true when the state is nonnegative or equals -2. The maintained source used a
separate raw `this + 0xE94C` read even though its Player runtime callers
already owned the later `updateBlock1095C` and `drawCounter10960` fields.

`src/PlayerSharedRuntimeView.hpp` now declares these three offsets in one
native object layout and checks their original i386 `offsetof` values.
`PlayerSharedRuntimeView::IsBlocked()` reads `blockStateE94C` directly. The
same declaration is used by the genuine PlayerRuntime and Replay control
translation units, replacing their duplicated local `struct` declarations.
The PlayerGameplay selector retains its established **anonymous-namespace
local type**: unifying that translation unit changed native symbol linkage
and was rejected rather than relabeling an external reference.

## Bounded original-bound proof

The pinned TH09 Japanese 1.50a PE is SHA-256
`10350095bcf95edb59e03bee9849a2dc8a7714b4927ad5909c569c550fce6822`.
Only source-dependent units were rebuilt under their canonical VC7.1 profiles:
21 PlayerRuntime owners, the exact IsBlocked leaf, the 59-byte Replay control,
and both original-exact Player selector/offset resolver carriers.
**25 complete units / 9,124 original physical bytes / 445 bound fields**
are strict exact. The 24-byte IsBlocked body, 522-byte Player OnUpdate,
3,238-byte selector and 59-byte Replay control all match without modifying
call or external data targets.

The common header renumbered 11 compiler-private `$L` labels in the 852-byte
physical Player UpdateBeforeState owner. The changed labels have the same
actual COFF section-relative addresses, original destinations, field types,
and positions. Only local label spellings were refreshed in the existing
manifest. There is no new exact authored credit, no binary patch, no full
cold replay, and no product/runtime/Factory acceptance claim.
