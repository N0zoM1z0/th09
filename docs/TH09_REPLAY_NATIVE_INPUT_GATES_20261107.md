# Replay native two-side gates and independent third input state (2026-11-07)

Source of truth is the Japanese TH09 1.50a executable, verified SHA-256
`10350095bcf95edb59e03bee9849a2dc8a7714b4927ad5909c569c550fce6822`.
This checkpoint changes the *actual Replay input C++ code* used by native
record and playback leaves, with separate original-bound coverage of two
nonexact large Replay bosses. It does not infer historical names or add any
new input stream or gameplay side.

## One runtime, two distinct array cardinalities

The real `ReplayInputState g_ReplayInputStates[3]` is a single globally shared
array of three physical 0x8E-byte input states. Its last element is the
shared/controller input state used by recording and playback as the third
input cursor; it is **not** an additional game side. The actual
`GameConfiguration::sideFocusMode[2]` at +0xB4 configures only the two
player-side input gates. Both original source loops iterate those two
side modes while separately recording all three state histories and
updating the third shared state.

The new narrowly scoped `ReplayNativeInputGateCapacity` C++ type derives
`SideCount` from `GameConfiguration::sideFocusMode`, `StateCount` from the
actual `g_ReplayInputStates` array, and `SharedInputStateIndex` as the
last physical array element. Compile-time ABI checks pin the 2:3 relationship,
0x8E-byte input element, +0xB4 mode field, and the independent
`ReplayRecordingCapacity::InputStreamCount` (three 16-bit replay streams).

`ReplayManager::PlaybackInputAndFps`, `RecordInputAndFps` and
`CaptureFrameSyncState` now use these true native array extents instead of
treating every occurrence of literal `2` as an unrelated dimension. The
explicit first and second game sides still use indices 0 and 1; the shared
input state uses `SharedInputStateIndex`, and each gate loop stops after
the true two player-side configuration bytes. No branching order, input
cursors, flags, or external callback ABI is changed.

## Source-cold PE/COFF proof

Four isolated original-fixed-profile source variants (baseline, native
side-bound only, shared input-index only and both combined) were compiled
serially under the real EH and no-EH VC7.1 profiles. All four passed all
**12 already exact ReplayInput functions**, including the complete 460-byte
RecordInputAndFps, 299-byte PlaybackInputAndFps, 257-byte
CaptureFrameSyncState and 1,323-byte BeginRecordingStage. The exact owners
retain **4,093 original bytes and 232 relocation records**. The selected
version was adopted only after a fresh original-bound `--owner replay`
replay validated **15 exact dependencies (4,141 bytes / 234 fields)**
and the complete open BeginPlaybackStage: still **538/540 bytes, 32 fields,
39 overlapping unlike bytes and two missing target bytes**.

The separate `--owner replaysave` replay verified the other complete
**1,238-byte SaveReplay** owner and its 12 exact siblings. SaveReplay still
has just the known `+0xB7/+0xBB` two ordinary byte differences with all
53 source-bound fields, 23 ordered calls and 49 agreeing control-flow
blocks. The new type does **not** claim either nonexact boss is complete.

Frozen private original inputs, compiler profiles, full object hashes and
field-by-field diagnostics are retained in
`.analysis/gptweb-replay-input-gate-20261107/`. No full cold replay,
original target mutation, whole Windows executable build, runtime/semantic
Oracle, Factory acceptance, portability or Git push is claimed. Canonical
counts remain **1,034 source-present, 1,021 exact, 13 nonexact**.
