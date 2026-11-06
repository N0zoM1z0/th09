# Replay serialized locations use the shared header words

SaveReplay now writes 32-bit file offsets through `frameOffset` and `fpsOffset`
in the shared header. LoadReplayData reads those offsets before publishing the
rebased pointers through the existing `frameStart` and `fpsStart` members.
The two representations occupy the same original words. This removes numerical
pointer casts from the serialization boundary without changing emitted game code.

The header remains `0xC0` bytes and the complete record remains `0x1EC`. Compiler
assertions check the offset tables at `+0x20/+0x98`, their four-byte entries,
and the existing pointer-array strides and surrounding fields. These unions are
maintained reconstruction views, not proof of original C++ declarations.

## Target and complete compiler evidence

Fresh direct Ghidra exports cover SaveReplay `420C60` and LoadReplayData
`4205E0`. Each attests the original disk and Program SHA-256, i386 language,
base `400000`, entry `47D45F` and all 685056 file-backed mapped bytes. The
591-byte loader tests the four tables' words for zero, adds the decoded
record's base to nonzero words and leaves zero words unchanged. SaveReplay
writes the stream-relative locations later consumed by that rebasing loop.
No source-level lifetime, original class identity or runtime-domain proof is
inferred from decompilation.

The quoted-include dependency walk identifies seven source carriers. Nine
existing source/profile groups are rebuilt into isolated objects;
the separate nonexact replay-menu carrier is also rebuilt. Earlier accepted
canonical objects are preserved. Full byte, tail/table and relocation checks
pass for 66 existing canonical units: 26915 physical bytes and 1472 fields.
Two origin-unknown constructor bodies also reproduce 33 bytes/two fields,
remaining outside authored exact credit. Total: 68 comparisons, 26948 bytes,
1474 fields. This includes the complete original-equal 591-byte loader.

Nine compiler-private Title start-menu labels receive new spelling. Each
actual same-owner COFF definition and original PE operand independently proves
the unchanged destination before the manifest name is updated. No original
address, relocation target, physical extent or exact ledger count changes.

The complete SaveReplay output in both existing profiles remains1238 bytes /
53 fields / two differing bytes at `+0xB7/+0xBB`. All1236 equal positions remain
protected. The original header-copy destination loads EDI and then addresses
through EDI; the current output first loads ECX and addresses through ECX.
No register hint or artificial source scheduling is introduced to force this.
The original pre-copy uninitialized table scan and file/error behavior remain.

Other affected nonexact owners retain their entire previous raw bodies and
effective field identities: BeginPlaybackStage538/540 with39 unlike overlaps
and two missing bytes; Title result484/485 with218 unlike overlaps and one
missing; replay menu1280/57/29; GameplaySetupThread1689 in its existing profile.
Both setup source/profile bodies are preserved, including the alternate profile
which is not the original-sized frontier. These are complete comparisons,
not new exactness claims.

## Current-source controls and proof scope

The earlier `7d510243` Replay receipt cannot stand in for the current source:
ReplayInput, GameManager storage and the replay schema have since changed.
This session freezes all twelve current quoted ReplayInput inputs at `5db810b8`
and freshly rebuilds its baseline before evaluating controls. The historical
receipt supplies a recipe reference, not current input/object exactness credit.

Six fresh complete contexts cover that baseline, typed frame assignment,
grouped byte offset, unsigned wire offset, raw record copy and the file-offset
union writer. Every context retains1238/53/two and all original fields, calls,
literals and control-flow observations. The final shared-header implementation
also converts the actual loader and adds layout assertions. The other source
controls are not adopted. Older similar experiments remain bounded historical
evidence rather than claims that all natural source hypotheses are exhausted.

The ignored `.analysis/replay-save-current-20261106/` packet retains frozen
inputs/Git hashes, all changed controls, serial bounded compile recipes,
actual objects/logs, full comparisons, Ghidra reports and the independently
replayed current-source manifest. `build-current.py` rebuilds the affected
cohort; `verify-current.py` rechecks its complete owners and preserved nonexact
frontiers. `build-menu.py` records the separate nonexact menu build.

All177 focused Replay, Title, gameplay-setup and shared-layout tests pass.
Constructor layout checks, target/tracking, progress, documentation and
whitespace checks pass. No new authored exact function is claimed: totals stay
1035 source-present /1022 exact /13 nonexact. Whole-product/runtime closure,
dual-Oracle semantics, portability and Factory acceptance remain open.


## Immutable local source receipt

Source checkpoint `36002b4b272f56f1daf410ce32baaff488687538` is bound by
`.analysis/replay-save-current-20261106/source-proof.json`, SHA-256
`be937055150692469c349828bfa2a42738f8f4eaaeddf23a0baf6656dcf73584`.
It records71 tracked inputs and normalized Git blobs, ten actual objects and
profiles, eight compiler/SDK files, all68 complete comparisons, both mapped-byte
Ghidra attestations and the unchanged nonexact carrier diagnostics. The
independent `source-receipt.py` replay passes and writes a separate report.
This is local compiler evidence, not Factory or remote-publication acceptance.

The six control contexts are separately sealed by SHA-256
`408cb342a6264bd5f5e28f7790f11468cb53d30631fc424fe1b0b6eaf63deabe`.
66 SHA-identical staged copies are pruned. Restoring the native-wire control
reproduces its entire1238-byte raw body and all53 actual COFF rows; ten restored
unchanged inputs are then pruned again. Frozen inputs, changed sources,
objects, logs and unreproduced evidence remain available.

Read-only adjacent sources were also checked. TH08 `adfdc969` separates file
offsets from live pointers in its portable representation; its exact branch
still spells numerical pointer casts. TH095 `72409b7` instead has a24-byte
container header and another payload shape. These suggest representation
questions only: no adjacent byte, layout, behavior, type identity or compiler
hint is adopted as TH09 evidence. The ignored adjacent-hypothesis receipt pins
the inspected files and Git state. All TH09 claims above come from TH09 target
and compiler observations.
