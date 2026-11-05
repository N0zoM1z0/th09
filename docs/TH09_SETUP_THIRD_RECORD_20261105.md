# Gameplay setup third record, 2026-11-05

The continuation starts at clean `246d4b70`. Original-target/tracking/status
checks pass. Fresh Ghidra attests disk/Program SHA, i386, base/entry and all
685056 mapped file bytes for the setup worker, ResetGameManager, gameplay
cleanup and DeletedCallback.

TH09 reset traverses three flag words with0x38 stride: +0x34,+0x6C,+0xA4.
The worker creates EffectManager(2,0x320,1) into manager+0x7C; cleanup resets
that same slot and DeletedCallback releases and clears it. The setup view now
models three physical side records and uses record2.effectManager0C in all
three owners. The old standalone sharedSubsystem and surrounding opaque
ranges are removed. Only records0/1 create player-side controllers. The unused
members of record2 have no asserted gameplay role, and the original C++ class
spelling/declaration remains unknown. Size0x390, slot7C, flagA4 and subsystem
pointersA8/AC are compiler-checked.

Three real source/profile builds pass all12 affected canonical comparisons:
3987 physical bytes/335 fields. Both setup profiles are verified independently;
the Supervisor update owns its full996 bytes including its152-byte tail.
Forty Supervisor owner-local field spellings change. Each actual definition,
relative offset and original PE operand is verified before changing only the
manifest spelling. No destination, byte or comparison boundary changes.

The worker remains1689 bytes/171 fields/35 ordered calls/77 blocks, with904
unlike bytes and the same unresolved referent multisets. Twenty focused tests
pass. One obsolete source-text assertion demanding exactly two setup records
is removed; original-byte and field guards remain. No new exact credit follows.

## Fresh controls for the complete worker

Indexed creation, direct short-circuit score conditions, their combination,
assignment-in-condition, direct global access and opaque AdvanceTimedState
contexts all retain the same1689/171/904 result. Two delayed/rate-first limit
models produce905 differences and are rejected. The third-record source
change preserves every raw worker byte and actual COFF field.

An independent full Cirno-Labs gamemanager.cpp build at pinned revision
`d7653152becabb8c86f2f43d49015ecc11bd0652`, using the maintained recorded profile,
emits1682 bytes/172 fields,734 unequal overlaps and7 missing bytes with no
unresolved fields. It is not a replacement or canonical match. Reference
comments are routing hypotheses; only TH09 complete comparisons govern credit.
No reference implementation is adopted in this checkpoint.

## Reproduction and retained proof

Rebuild the10 setup units through `game-manager-on-update`, then the distinct
`supervisor-setup-loading-vms-and-capture` profile and `supervisor-on-update`.
Compare each unit before reusing the setup object's manifest path, since the
two setup profiles intentionally share that path. The private
`.analysis/setup-flow-20261105/verify-source.py` retains separate objects and
compares the correct complete units for each actual source/profile build.
`native-proof.json`, `native-worker.json` and `label-proof.json` retain the
receipts. Frozen inputs, changed controls, compiler logs and Ghidra exports
are also retained there; experiment objects are below
`build/analysis/setup-flow-20261105/`. Tracking, progress, docs and whitespace
checks pass. Totals remain1019 exact/16 nonexact; product/runtime, Factory
acceptance and remote publication are independent open claims.
