# TH09 Title replay menu — native replay record snapshot, 2026-11-06

The original Japanese TH09 v1.50a `TitleScreenView::DrawReplayMenu` at
`0x004234F6` is **still not byte-exact**. Its maintained complete owner is
1,280 physical bytes, with 57 independently original-bound fields, 13
ordered calls, 376 decoded instructions and 72 matching control-flow blocks.
The remaining 29 unequal bytes are predominantly ECX/EDX/stack-lifetime
choices in the selected-replay and interpolation path.

The current source now borrows the selected record as a natural C++
`const ReplayDataView &replayValue = replays[selectedReplay]`, with the
`selectedReplay` index snapshot immediately after acquiring that record.
The actual subsequent replay header, replay player/time text, difficulty,
mode and ten frame-start entries all use the same native record reference.
The three existing dynamic `replays[selected].value1E4` tests are preserved:
changing them to use the borrowed reference causes a real original-codegen
regression, despite apparently equivalent values. This fixes the source's
record-ownership model without inventing any new replay type or allocation.

## Non-crediting whole-function experiments

**23** whole C++ source variants were compiled under the existing pinned
VC7.1 profile. The tests included selected-index vs record acquisition
ordering, mutable/const pointer/reference forms, offset-based record lookup,
frame snapshot ordering and representation, the interpolation target's
constness, and replacing repeated direct `replays[selected]` accesses.

- Taking the record **before** the integer index snapshot is necessary for
  the currently matching 1,280-byte layout; taking the index first emits
  only 1,264 bytes and introduces hundreds of unequal positions.
- A native `const ReplayDataView&` preserves exactly the same 29 unequal
  offsets and all 1,251 formerly equal bytes, full 57-field identities,
  literal inventory, call sequence and direct CFG graph.
- Reusing the record reference for the three existing mode comparisons
  emits a 1,258-byte owner and adds hundreds of differences. It was rejected.
- Several other source shapes preserve the same 29-byte frontier but do not
  reduce it; they are retained as tests, not adopted as exactness proof.

Focused current-source reproduction is
`python3 -B scripts/check-boss-frontiers.py --owner replaymenu`.
The canonical inspector separately verifies every field and literal, the
full physical range, 13 ordered calls and all 72 control-flow blocks. Its
accepted non-crediting result remains **1,280/57/29**, with **zero newly
unequal original positions**. No full cold replay, artificial register
forcing, assembly, profile substitution, padding, original PE patch or
Fabric/Factory acceptance is claimed.

The evidence packet (ignored, local) is
`.analysis/gpt-web-title-menu-snapshot-20261106/` and
`.analysis/gpt-web-title-menu-branch-20261106/`; the accepted original-bound
result is `.analysis/gpt-web-title-menu-adopted-20261106-diff.json`.
Target SHA-256:
`10350095bcf95edb59e03bee9849a2dc8a7714b4927ad5909c569c550fce6822`.
The game's precise original C++ nominal type/method spelling, 29 residual
bytes, native product link, runtime and semantic acceptance remain open.
