# TH09 recording buffer and SaveReplay capacity recovery (2026-11-07)

The original Japanese 1.50a target is SHA-256
`10350095bcf95edb59e03bee9849a2dc8a7714b4927ad5909c569c550fce6822`.
The source-facing 0x18-byte `ReplayBufferLink` has exactly three 16-bit input
pointers, one FPS pointer, a frame count and a next pointer. Both
`RecordInputAndFps` and `BeginRecordingStage` allocate/zero `0x1C20` bytes
for each input cursor and `0x79` bytes for FPS. These values are not
independent SaveReplay guesses:

| Original immediate | Recovered storage relation |
| --- | --- |
| `0x54D9` | `3 * 0x1C20 + 0x79` bytes reserved per linked recording buffer |
| `0x60` | `3 * sizeof(ReplayFrameDataStartView)` (each frame header is `0x20`) |
| `0x101EC` | `sizeof(ReplayDataView) + 0x10000`; the serialized header is `0x1EC` |

`ReplayRecordingCapacity` in `ReplayManagerRuntimeView.hpp` now derives the
stream and header counts from the native arrays and struct sizes and encodes
the exact target capacities. Static layout assertions reject source-side shape
drift. The shared `ReplayInput.cpp` producers and full 1,238-byte SaveReplay
consumer use one consistent C++ layout rather than duplicating five numeric
immediates. These names are reconstruction abstractions, not proven original
historical C++ identifiers or a new allocation algorithm.

A fresh pinned VC7.1 build and original-bound `--owner replaysave` guard
confirmed all **12 scoped exact same-carrier siblings** unchanged:
**4,093 complete original bytes and 232 relocation records**. SaveReplay is
**still nonexact**, with 1,238/1,238 code bytes, 53/53 independently bound
fields, 23 ordered call operands, 49 matching control-flow blocks, and only
2 unequal ordinary bytes at offsets `0xB7` and `0xBB`. The opcode/target
census has not changed. This improves native semantic ownership, not the
accepted exact-function count. Focused Replay layout/codec/save tests,
tracking, documentation and whitespace checks pass.

The independent native C++ source-cold SaveReplay study in
`.analysis/gptweb-save-natural/` also tried explicit destination locals,
end pointers, offset locals, array indexing, typed destination pointers,
and an actual typed 0x20-byte struct assignment; all retained the same two
bytes of residual and no field/graph drift. This is bounded negative
compiler evidence, not a proof that the remaining machine-code difference
is irreducible. No experiment was adopted merely for appearance.

## Native stage and stream counts (second scoped checkpoint)

The serialized `ReplayDataView` defines a 3-by-10 `frameStart` table, a
10-element FPS table and the runtime `ReplayManagerView` owns ten
`ReplayBufferLink` stage roots. `ReplayRecordingCapacity::StageCount` now
comes from the **actual FPS wire array length**, and `InputStreamCount`
comes from the native link input array; compile-time identity assertions
prove the wire/frame table and runtime stage buffer have the same dimensions.
Eight actual loop bounds in `LoadReplayData`, `BeginRecordingStage`,
`Release`, and the complete `SaveReplay` now use these shared extents rather
than unrelated literals. The original logic (including fixed header copy
sizes, stage flags, and zero-based indexing) is unchanged.

A new serial original-bound `--owner replaysave` invocation checked all
12 exact same-TU functions (4,093 physical bytes, 232 fields), unchanged
and exact. The nonexact 1,238-byte SaveReplay still has the same two
unlike bytes at `+0xB7` and `+0xBB`, 53 fields, 23 calls and 49 matching
blocks. No whole-build or new exact credit is claimed.

The live original-bound source/machine-code results and temporary compiler
objects are under `.analysis/gptweb-replay-capacity-owner.log` and
`build/matching/`. Neither private artifacts nor the original executable
are committed; no full cold replay, Factory acceptance, product closure,
portability or remote push is claimed.
