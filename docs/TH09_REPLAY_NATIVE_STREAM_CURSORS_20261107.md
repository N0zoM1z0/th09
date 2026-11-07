# Replay manager: three native frame-stream cursors — 2026-11-07

This is a source reconstruction checkpoint, not a source-name/TU uniqueness,
whole-program runtime, or Factory acceptance claim. Original: Japanese TH09
v1.50a SHA-256 `10350095bcf95edb59e03bee9849a2dc8a7714b4927ad5909c569c550fce6822`.

## One physical array used by recording and playback

The original `ReplayManagerView` is 0x164 bytes. Its adjacent 32-bit pointers
at +0x0C/+0x10/+0x14 are recording and playback cursors for three native
16-bit input streams. Its FPS cursor at +0x18 immediately follows them.
These three streams have precisely the same indexing as the recorded
`ReplayBufferLink::input[3]` and the replay's `frameStart[3][10]` wire
representation. There are only two *gameplay side-focus configuration bytes*;
the third stream is the shared input state, not a third configurable side.

`ReplayManagerRuntimeView.hpp` now expresses these native cursor words as
`u16 *inputCursors[ReplayRecordingCapacity::InputStreamCount]`, using the
existing array-derived three-stream bound. Compile-time C++ assertions check
its exact size, offset, pointer stride and adjacency to `fpsCursor`, as well
as the unchanged 0x164-byte manager layout. `ReplayInput.cpp` uses these
coordinates consistently in the complete record, playback, stage setup,
link allocation, zeroing and cursor advancement paths. No cast aliasing,
compiler register steering, profile change, or separate fake cursor layout was
introduced. The reconstructed type name remains provisional.

## Complete affected-source comparison

A fresh source-cold fixed-VC7.1 comparison of all **seven** actually affected
compiler carriers passed **65 existing exact functions / 26,115 complete
physical bytes / 1,431 relocation fields**, covering ReplayInput (both
profiles), ReplayPlaybackFrameAdvance, GameManagerSetup (both profiles) and
TitleScreen (both profiles). There is no new exact-function credit.

The unchanged **1,238-byte SaveReplay** remains nonexact only at +0xB7 and
+0xBB: all 53 fields, 23 original-position calls and 49 control blocks agree.
The separate **540-byte BeginPlaybackStage** continues to emit 538 bytes,
with 32 bound fields, 39 unlike overlapping positions, two missing bytes and
an agreeing direct control-flow graph. Its original dataflow remains open.

The TitleScreen carrier recompiled nine compiler-private `$L` names in its
exact start-menu owner. All nine have the same original PE target VA, same
source-owner section and offset, identical relocation type/addend/site, and
no shifted jump table. The strict local-label checker was not loosened;
only the verified spelling in the match-unit manifest changed. The independent
65-owner symbol census in
`.analysis/gpt-web-replay-native-cursors-20261107/label-proof.json`
found no other relocation-name drifts. The original-bound Title locations
are held fixed, not solved from source output.

Twenty-seven focused source/layout tests pass. Both actual C++ owner changes
and the touched structural tests are committed, while earlier hypotheses
remain available as history. The separate ten-case natural source probe of
SaveReplay's stage-header copy (same pinned profile) did not remove either
remaining byte; a copy/write reordering regressed eleven more bytes. Controls
remain under `.analysis/gpt-web-replaysave-natural-20261107/`.

Ledger stays **1,034 source-present, 1,021 exact and 13 nonexact**. No
all-owner cold replay, whole Windows product closure, runtime validation,
or remote publication is claimed.
