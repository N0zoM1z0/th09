# TH09 native replay input frames and FPS sampling capacity (2026-11-07)

The target is the original Japanese TH09 1.50a PE, SHA-256
`10350095bcf95edb59e03bee9849a2dc8a7714b4927ad5909c569c550fce6822`.
This checkpoint recovers actual, **shared C++ source geometry** for recording,
playback and the open 1,238-byte SaveReplay big function. The struct and enum
names are source-facing reconstruction names, not proven original C++ spelling.

## Native capacity relationship, independently grounded in exact owners

`ReplayBufferLink` has three `u16 *input` streams, one `u8 *fps` stream, a
signed 32-bit `frameCount`, and an actual link pointer. The exact recording
allocator historically requested `0x1C20` bytes per input stream and `0x79`
bytes for FPS. Independently original-exact record/playback functions check
`frameCounter % 30 == 0`, and playback reads **`fpsCursor[1]`** (a one-byte
lookahead). The exact recorder rolls its link when the current link reaches
3,598 frames; the same physical arrays hold 3,600 16-bit frame values.

`ReplayRecordingCapacity` now expresses that with normal C++ constant
expressions and static layout guards:

| Capacity or bound | Native source calculation | Value |
| --- | --- | ---: |
| Per input frame | `sizeof(input[0][0])` | 2 bytes |
| Per input stream | retained original allocation | `0x1C20 = 7,200` bytes |
| Frame capacity | `InputStreamBytes / InputFrameBytes` | 3,600 frames |
| FPS sample period | retained original modulo, matching wire write | 30 frames |
| FPS buffer | `InputFrameCapacity / FramesPerFpsSample + 1` | `0x79 = 121` bytes |
| Link rollover threshold | `InputFrameCapacity - 2` | 3,598 frames |

The source uses these values in both playback and recording modulo checks, the
recording link rollover, SaveReplay's signed `frameCount`-to-input byte-count
calculation, its FPS-size division and the actual `ReplayDataView::value1D0`
wire field publication. These changes do **not** add a stream, alter pointer
strides or rearrange the original 0x18-byte linked record.

## Full original-bound compiler and regression scope

A source-cold private A/B test compiled complete ReplayInput.cpp under **both
pinned /EHsc and no-EH profiles** (12 exact functions), with unchanged real
source includes. The baseline, typed local frame-size experiment, shared
capacity-only version and complete shared version each preserved all 12 exact
owners. The live selected version also preserves the full 1,238-byte SaveReplay
with 53 bound original fields, 23 ordered calls, 49 matching control-flow
blocks, and **the same two ordinary differing bytes at +0xB7/+0xBB**.
The independently checked 540-byte target BeginPlaybackStage remains 538
candidate bytes, 32 fields, 39 unlike overlaps and two missing bytes: its
still-open mismatch was not hidden by the new geometry.

Because `ReplayManagerRuntimeView.hpp` is shared, all **7 affected original
compiler carriers from 4 source TUs** were compiled serially under their
*existing pinned flags*, including GameManagerSetup and TitleScreen. A
separate full physical PE/COFF original-bound replay passes **all 65 already
exact units, 26,115 complete physical bytes and 1,431 relocations**.
Only nine VC7 compiler-private `$L...` symbol spellings changed in the
TitleScreen carrier. They were each tied to the actual same-section definition
and independently known original field VA before the private manifest passed
the unchanged strict full-function canonical verifier; no target address,
external symbol, field offset/kind/addend, or ordinary byte was modified.
The normal focused SaveReplay diagnostic also passes all 12 exact ReplayInput
siblings (4,093 original bytes and 232 fields).

Private cold input hashes, compiler objects, actual owner receipts and a
reviewed manifest are retained under
`.analysis/gptweb-replay-frame-capacity-20261107/`. No all-owner cold replay,
original game-file mutation, whole Windows product build, runtime validation,
Factory acceptance, portability or remote push is claimed. Canonical totals
remain 1,034 source-present, 1,021 exact and 13 nonexact functions.
