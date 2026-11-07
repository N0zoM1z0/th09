# Native Replay playback stage checks its frame-table row address

Original target: Japanese TH09 v1.50a, verified SHA-256
`10350095bcf95edb59e03bee9849a2dc8a7714b4927ad5909c569c550fce6822`.
This source-facing layout/ABI checkpoint does **not** claim complete
byte-exact reconstruction of the 540-byte BeginPlaybackStage owner or
uniquely established historic C++ spelling.

## One physical replay header and three input stream rows

The complete original ReplayDataView owns a native `frameStart[3][10]`
array at byte +0x20. Each ten-pointer row occupies **0x28 bytes** and
shares the underlying 32-bit locations of its serialized `frameOffset`
representation. The original 540-byte BeginPlaybackStage at 0x00420840
checks the **address of one such frame-table row**, not the frame data
pointer stored at `frameStart[0][stage]`. The maintained implementation
previously formed that address via a `u8 *` cast, an explicit offsetof,
and a separate integer byte-stride multiplication. The source now uses
`replayManager->replayData->frameStart[stage] == NULL` directly. A
compile-time native layout check pins +0x20 and both 0x28-byte and 3-row
array sizes to the existing replay wire and recording-capacity types.

This deliberately preserves the original's unusual row-address test.
Replacing it with a stored pointer null test, correcting the stage index,
adding new bounds guards, or reordering the file-load/rebase behavior would
be a **behavior change**, not a compiler-matching type recovery. Nothing
in the current original-byte evidence authorizes those changes.

## Whole original function, sibling exact and SaveReplay controls

Nine natural C++ variants (byte-address expression, array decay,
row/index/address forms, pointer casts and literal zero/null comparisons)
were compiled under the unchanged pinned VC7.1 ReplayInput profile. Every
variant reproduced the **same 538-byte candidate** against the immutable
**540-byte original**, with the **same 32 original-bound fields, five
ordered calls, 12 direct blocks, 39 unlike overlapping bytes and two
missing target bytes**. The adopted row-index expression preserves all
previously equal original positions. It does not turn the boss into a new
complete exact match.

The two changed-source canonical replay carrier profiles cold-compiled and
passed **12 complete already-exact functions / 4,093 physical bytes /
232 relocation fields**. The independently recompiled full SaveReplay boss
retained **1,238 candidate / 1,238 original bytes, 53 fields, 23 calls,
49 blocks, and precisely two ordinary byte differences** at +0xB7/+0xBB.
This is a source-level Replay producer/consumer closure, not a whole-game
or complete-library claim. The 69 focused replay tests pass; all original
symbols, fields, literals and pinned compiler flags remain unchanged.

Bounded local experiments and strict comparison receipts are at
`.analysis/gpt-web-replay-native-frame-guard-20261107/`. Overall tracking
remains **1,034 source-present / 1,021 complete exact / 13 nonexact**.
No all-owner cold replay, Windows product, runtime acceptance, Factory
exactness receipt or remote push is asserted.
