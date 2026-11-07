# EffectManager dynamic sentinel pool and live update bounds — 2026-11-07

Target: original Japanese TH09 v1.50a
`10350095bcf95edb59e03bee9849a2dc8a7714b4927ad5909c569c550fce6822`.
This checkpoint actually edits the native `EffectManager.cpp` functions,
including the **475-byte full OnUpdate**, 327-byte Create and the real
allocation/spawn/release leaf owners. It does not claim the new C++ helper
name is historically original.

## Dynamic physical allocation, not a fixed container

`EffectManager` is the one original-observed 0x2DC-byte manager with a
single `Effect *effects` pool pointer at +0x30 and two signed bounds at
+0x34/+0x38. Each `Effect` occupies the original 0xD8 bytes. Original
producer and consumer behavior distinguishes:

| Native role | Recovered source expression |
| --- | --- |
| Live records scanned by OnUpdate | `primaryCount + secondaryCount` |
| Allocated records in Create/Reset/Release | `primaryCount + secondaryCount + 1` |
| Failed spawn's return value | `effects + (primaryCount + secondaryCount)` |

That last entry is the dedicated extra failure-sentinel record, not an
active effect. Both `SpawnEffect` and `SpawnEffectWithVelocity` return this
*same* pointer when their primary-slot search fails. `EffectNativePoolCapacity`
now owns the typed source expressions for `ActiveCount`, `AllocatedCount`,
`AllocatedFromCounts` and `FailureSentinel`; compile-time layout assertions
pin the actual native manager/effect sizes and pointer/bound positions.

The 475-byte exact `OnUpdate` has an independently documented original
register/load order: its primary bound is a live volatile-qualified member
read before `secondaryCount` on each iteration. The source's already
existing `static_cast<const volatile int &>` expression is **preserved** in
`ActiveCountWithNativeOrder`, rather than replaced with a cached snapshot.
The normal non-volatile `ActiveCount` accessor would change four existing
original-equal instruction bytes; it is deliberately **not** used for this
loop. No new volatile member, alignment override, profile tweak, assembly or
dummy register hint is introduced.

## Complete source-cold original comparison

Seven coherent C++ variants were compiled serially under the same pinned
VC7.1 effect profile: baseline, direct sentinel count, typed allocation
counts, separate active-count access, typed failure sentinel pointer, and
both ordered/unordered OnUpdate accessor models. The successful native
storage controls each retained all **15 exact** source functions. The
unordered update model regressed the 475-byte owner by four bytes and is
rejected. The chosen model preserves all original bytes including complete
function tails, relocations, and actual read-only definition objects.

Fresh `--owner effect` original-bound replay after the live source edit
confirms the **16 affected exact owners** (including the independently exact
AnmLoaded resource callee): **2,571 physical bytes and 101 relocations**,
all exactly equal to the original. The still-open 51-byte
`EffectManager::AddedCallback` retains all four bound source fields and two
ordered original calls, with **8 ordinary entry differences** in ESI/EDI
load scheduling. The type change does not falsely claim an exact callback.
The global count remains 1,034 source-present / 1,021 exact / 13 nonexact.

Private isolated source/compiled object/strict complete-owner reports are
under `.analysis/gptweb-effect-pool-capacity-20261107/`. The original PE,
compiler flags and canonical original target addresses were unchanged. No
full-project cold replay, whole Windows executable build, runtime/semantic
Oracle, Factory acceptance, portability or remote push is claimed.
